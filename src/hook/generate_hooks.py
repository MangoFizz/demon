import json
import sys

if len(sys.argv) != 3:
    print("Usage: {} <hooks.json> <out.cpp>")
    sys.exit(1)

with open(sys.argv[1], "r") as f:
    hooks = json.loads(f.read())

lib_fns = ""
hook_fn = ""
for hook in hooks:
    hook_info = hooks[hook]
    if "disabled" in hook_info and hook_info["disabled"]:
        continue

    if "flag" in hook_info:
        hook_fn += "        #ifdef {flag}\n".format(flag=hook_info["flag"])

    if "replace" in hook_info and hook_info["replace"]:
        lib_fns += "    extern int {};\n".format(hook)
        hook_fn += "        Hook({address}, reinterpret_cast<std::uintptr_t>(&{output}))".format(address=hook_info["address"], output=hook)
    else:
        hook_fn += "        Hook({address})".format(address=hook_info["address"])

    if "arguments" in hook_info:
        for arg in hook_info["arguments"]:
            if arg.isnumeric():
                hook_fn += "\n            .push_parameter(Stack, {index})".format(index=arg)
            else:
                hook_fn += "\n            .push_parameter(Register, {index})".format(index=arg.upper())

    if "use_return_value" in hook_info and hook_info["use_return_value"]:
        hook_fn += "\n            .has_return_value()"

    hook_fn += "\n            .write_hook();\n"

    if "flag" in hook_info:
        hook_fn += "        #endif\n"

cpp_source_code = """// AUTO-GENERATED! DO NOT EDIT UNLESS YOU LIKE REGRETTING THINGS!

#include "src/hook/hook.hpp"

extern "C" {
""" + lib_fns + """}

namespace Demon {
    void set_up_all_hooks() {
""" + hook_fn + """    }
}
"""

with open(sys.argv[2], "w") as f:
    f.write(cpp_source_code)
