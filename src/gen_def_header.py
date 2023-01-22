import sys
import json
import os
import copy

if len(sys.argv) != 4:
    print("Usage: {} <name> <json-dir> <header-out>")
    sys.exit(1)

with open(sys.argv[2] + "/" + sys.argv[1] + ".json", "r") as f:
    j = json.loads(f.read())

all_defs = []
for q in os.listdir(sys.argv[2]):
    with open(sys.argv[2] + "/" + q) as f:
        all_defs.extend(json.loads(f.read()))

ext_structs = []

def make_field(s):
    if s["type"] == "pad":
        return "    uint8_t pad_{o}[{length}];\n".format(o=len(struct_data), length=s["size"])
    if "bounds" in s and s["bounds"]:
        f = copy.copy(s)
        del(f["bounds"])
        t = copy.copy(f)
        f["name"] = f["name"] + " from"
        t["name"] = t["name"] + " to"
        return make_field(f) + make_field(t)

    name = s["name"].replace(" ", "_").replace("'","")
    if name[0].isnumeric():
        name = "_" + name

    types = {
        "float": "float",
        "Angle": "float",
        "Fraction": "float",
        "Vector2D": "VectorIJ",
        "Vector3D": "VectorIJK",
        "Euler2D": "VectorPY",
        "Euler3D": "VectorPYR",
        "Point3D": "VectorXYZ",
        "Point2D": "VectorXY",
        "Plane3D": "Plane3D",
        "Plane2D": "Plane2D",
        "ScenarioScriptNodeValue": "ScenarioScriptNodeValue",
        "Quaternion": "Quaternion",
        "Point2DInt": "VectorXYInt",
        "Pointer": "void *",
        "TagID": "TableID",
        "String32": "String32",
        "Index": "uint16_t",
        "ColorRGB": "ColorRGB",
        "ColorARGB": "ColorARGB",
        "ColorARGBInt": "ColorARGBInt",
        "Matrix": "Matrix",
        "Rectangle": "Rectangle2D",
        "TagReference": "TagReference",
        "Data": "Data",
        "int8": "int8_t",
        "int16": "int16_t",
        "int32": "int32_t",
        "uint8": "uint8_t",
        "uint16": "uint16_t",
        "uint32": "uint32_t",
        "FourCC": "FourCC",
    }

    t = types.get(s["type"], None)

    if t is None:
        for m in all_defs:
            if m["name"] == s["type"]:
                if m["type"] == "enum":
                    t = "uint16_t"
                elif m["type"] == "bitfield":
                    t = "uint{}_t".format(m["width"])
                break

    if t is None:
        if s["type"] == "Reflexive":
            if s["struct"] not in ext_structs:
                ext_structs.append(s["struct"])
            t = "struct {{ uint32_t count; struct {} *elements; uint8_t padding[4]; }}".format(s["struct"])

    if t is None:
        print("Unknown type " + s["type"])
        sys.exit(1)

    if "count" in s:
        count = "[{}]".format(s["count"])
    else:
        count = ""

    return "    {t} {name}{count};\n".format(t=t, name=name, count=count)

header_code = ""

for s in j:
    if s["type"] == "struct":
        struct_data = "typedef struct {} {{\n".format(s["name"])
        for f in s["fields"]:
            struct_data = struct_data + make_field(f)

        struct_data = struct_data + "}} {name};\n_Static_assert(sizeof({name}) == {size});\n\n".format(name=s["name"],size=s["size"])
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


ext = ""

for e in ext_structs:
    ext += "struct {e};\n".format(e=e)

header_file = """/* This file is auto-generated. Do not edit unless you like regretting things. */

#ifndef RINGHOPPER_HEADER_{hname}
#define RINGHOPPER_HEADER_{hname}

#include "src/impl/3d.h"
#include "src/impl/tag.h"
#include "src/impl/misc_types.h"

#include <stdint.h>

{ext}
{header_code}#endif
""".format(hname=sys.argv[1].upper(), header_code=header_code, ext=ext)

with open(sys.argv[3], "w") as f:
    f.write(header_file)
