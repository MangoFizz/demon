import sys
import json
import os
import copy

if len(sys.argv) != 4:
    print("Usage: {} <name> <json-dir> <header-out>")
    sys.exit(1)

with open(sys.argv[2] + "/" + sys.argv[1] + ".json", "r") as f:
    j = json.loads(f.read())

# Load all JSONs
all_defs = []
for q in os.listdir(sys.argv[2]):
    with open(sys.argv[2] + "/" + q) as f:
        everything = json.loads(f.read())
        for e in everything:
            e["origin"] = q.replace(".json", "")
        all_defs.extend(everything)

# External structs that do require loading a header (members)
required_headers = []

# Make a field. Returns [field_data, length]
def make_field(s):
    length = 0
    comment = ""

    if s["type"] == "pad":
        length = s["size"]
        return ["    uint8_t pad_{o}[{length}];\n".format(o=len(struct_data), length=length), length]
    if "bounds" in s and s["bounds"]:
        f = copy.copy(s)
        del(f["bounds"])
        t = copy.copy(f)
        f["name"] = f["name"] + " from"
        t["name"] = t["name"] + " to"

        a = make_field(f)
        b = make_field(t)

        return [a[0] + b[0], a[1] + b[1]]

    name = s["name"].replace(" ", "_").replace("'","")
    if name[0].isnumeric():
        name = "_" + name

    types = {
        "float": ["float", 4],
        "Angle": ["float", 4],
        "Fraction": ["float", 4],
        "Vector2D": ["VectorIJ", 4*2],
        "Vector3D": ["VectorIJK", 4*3],
        "Euler2D": ["VectorPY", 4*2],
        "Euler3D": ["VectorPYR", 4*3],
        "Point3D": ["VectorXYZ", 4*3],
        "Point2D": ["VectorXY", 4*2],
        "Plane3D": ["Plane3D", 4*4],
        "Plane2D": ["Plane2D", 4*3],
        "ScenarioScriptNodeValue": ["ScenarioScriptNodeValue", 4],
        "Quaternion": ["Quaternion", 4*4],
        "Point2DInt": ["VectorXYInt", 2*2],
        "Pointer": ["void *", 4],
        "TagID": ["TableID", 4],
        "String32": ["String32", 32],
        "Index": ["uint16_t", 2],
        "ColorRGB": ["ColorRGB", 4*3],
        "ColorARGB": ["ColorARGB", 4*4],
        "ColorARGBInt": ["ColorARGBInt", 4],
        "Matrix": ["Matrix", 4*3*3],
        "Rectangle": ["Rectangle2D", 2*4],
        "TagReference": ["TagReference", 4*4],
        "Data": ["Data", 4*5],
        "int8": ["int8_t", 1],
        "int16": ["int16_t", 2],
        "int32": ["int32_t", 4],
        "uint8": ["uint8_t", 1],
        "uint16": ["uint16_t", 2],
        "uint32": ["uint32_t", 4],
        "FourCC": ["FourCC", 4],
    }

    t = types.get(s["type"], None)

    if t is None:
        for m in all_defs:
            if m["name"] == s["type"]:
                if m["type"] == "enum":
                    t = ["uint16_t", 2]
                    comment = " // enum: " + m["name"]
                elif m["type"] == "bitfield":
                    t = ["uint{}_t".format(m["width"]), m["width"] // 8]
                    comment = " // bitfield: " + m["name"]
                comment += " @ {}.h".format(m["origin"])
                break

    if t is None:
        if s["type"] == "Reflexive":
            t = ["struct {{ uint32_t count; struct {} *elements; uint8_t padding[4]; }}".format(s["struct"]), 12]
            for m in all_defs:
                if m["name"] == s["struct"]:
                    comment += " // struct {} @ {}.h".format(m["name"], m["origin"])
                    break

    if t is None:
        print("Unknown type " + s["type"])
        sys.exit(1)

    length = t[1]
    if "count" in s:
        count = "[{}]".format(s["count"])
        length = length * s["count"]
    else:
        count = ""

    return ["    {t} {name}{count};{comment}\n".format(t=t[0], name=name, count=count, comment=comment), length]

header_code = ""

for s in j:
    if s["type"] == "struct":
        struct_data = "typedef struct {} {{\n".format(s["name"])

        current_offset = 0
        struct_data += "    // 0x{:04X}\n".format(current_offset)
        prev_offset = 0

        # If we inherit a struct, we put it as the first member. This requires importing that struct's header too though, but only if it isn't the same.
        if "inherits" in s:
            inherited_origin = None
            for n in all_defs:
                if n["type"] == "struct" and n["name"] == s["inherits"]:
                    current_offset = n["size"]
                    inherited_origin = n["origin"]
                    break
            if inherited_origin != sys.argv[1]:
                required_headers.append(inherited_origin)

            struct_data = struct_data + "    {} base_struct;\n".format(s["inherits"])

        for f in s["fields"]:
            field = make_field(f)

            if prev_offset // 0x10 < current_offset // 0x10:
                struct_data += "\n    // 0x{:04X}\n".format(current_offset)

            prev_offset = current_offset
            current_offset = current_offset + field[1]
            struct_data = struct_data + field[0]

        if current_offset != s["size"]:
            print("Struct {name} has a bad size (expected 0x{expected:04X} bytes, got 0x{calculated:04X})", name=s["name"], expected=s["size"], calculated=current_offset)

        struct_data = struct_data + "}} {name};\n_Static_assert(sizeof({name}) == 0x{size:04X});\n\n".format(name=s["name"],size=s["size"])
        header_code = header_code + struct_data
    elif s["type"] == "enum":
        struct_data = "enum {} {{\n".format(s["name"])
        for i in range(0, len(s["options"])):
            f = s["options"][i]
            if isinstance(f, str):
                name = f
            else:
                name = f["name"]
            struct_data = struct_data + "    {}_{} = {},\n".format(s["name"], name.replace(" ", "_").replace("'", ""), i)
        struct_data = struct_data + "};\n\n"
        header_code = header_code + struct_data
    elif s["type"] == "bitfield":
        struct_data = "enum {} {{\n".format(s["name"])
        for i in range(0, len(s["fields"])):
            f = s["fields"][i]
            if isinstance(f, str):
                name = f
            else:
                name = f["name"]
            struct_data = struct_data + "    {}_{} = 0x{:X},\n".format(s["name"], name.replace(" ", "_").replace("'", ""), 2**i)
        struct_data = struct_data + "};\n\n"
        header_code = header_code + struct_data


ext_headers_def = ""

for e in required_headers:
    ext_headers_def += "#include \"{e}.h\"\n".format(e=e)

header_file = """/* This file is auto-generated. Do not edit unless you like regretting things. */

#ifndef RINGHOPPER_HEADER_{hname}
#define RINGHOPPER_HEADER_{hname}

#include "src/impl/tag/tag.h"
#include "src/impl/types/types.h"
{ext_headers_def}
#include <stdint.h>

{header_code}#endif
""".format(hname=sys.argv[1].upper(), header_code=header_code, ext_headers_def=ext_headers_def)

with open(sys.argv[3], "w") as f:
    f.write(header_file)
