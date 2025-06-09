
from pathlib import Path
from pydantic import BaseModel
import sys

"""
NAMING CONVENTIONS:
    - snake_case
        - regular values and functions
        - class/structure instances
    - lowerCamelCase
        - class/structure definitions
    - UpperCamelCase
        - not used?
"""

if len(sys.argv) <= 1:
    print("Expected 1 or more arguments")
    exit()

class File(BaseModel):
    filename: str
    content: str

output_directory = Path("output")
output_directory.mkdir(parents = True, exist_ok = True)
states_directory = Path("output/states")
states_directory.mkdir(parents = True, exist_ok = True)

state_source: list[File] = []
state_header: list[File] = []
states_source: File
states_header: File
runner_source: File
runner_header: File

snake_case_list: list[str] = []
upper_camel_case_list: list[str] = []
lower_camel_case_list: list[str] = []

states_source_individual_dependencies: list[str] = []
states_source_individual_assign_definitions: list[str] = []
states_header_individual_assign_declarations: list[str] = []

state_header_path = "state.h"

args = sys.argv[1:]
for i, arg in enumerate(args):

    state_name_word_list = arg.lower().split()

    snake_case = "_".join(state_name_word_list)
    snake_case_list.append(snake_case)

    upper_camel_case = "".join(w.capitalize() for w in state_name_word_list)
    upper_camel_case_list.append(upper_camel_case)

    lower_camel_case = upper_camel_case[0].lower() + upper_camel_case[1:]
    lower_camel_case_list.append(lower_camel_case)

    # # # #

    states_source_individual_dependencies.append(f"#include \"states/{snake_case}.h\"\n")
    
    states_source_individual_assign_definitions.append(f"""
void assign_{snake_case}(state *target) {{
    assign(
        target,
        &{snake_case}_init,
        &{snake_case}_step,
        &{snake_case}_exit
    );
}}
""")
    
    states_header_individual_assign_declarations.append(f"""
void assign_{snake_case}(state *target);""")

    # # # #

    state_header_dependencies = f"""
#ifdef TEST
#include "mock_gb.h"
#else
#include <gb/gb.h>
#endif
"""

    state_header_declarations = f"""
void {snake_case}_init(void);
uint8_t {snake_case}_step(void);
uint8_t {snake_case}_exit(void);
"""

    state_source_dependencies = f"""
#include "{snake_case}.h"
"""

    state_data_struct_definition = f"""
typedef struct _{lower_camel_case}Data {{
    uint8_t count;
}} {lower_camel_case}Data;
"""

    state_data_struct_declaration = f"""
{lower_camel_case}Data {snake_case}_data;
"""

    state_init = f"""
void {snake_case}_init(void) {{
    {snake_case}_data.count = 0;
}}
"""

    state_step = f"""
uint8_t {snake_case}_step(void) {{
    {snake_case}_data.count += 1;
    return 0;
}}
"""

    state_exit = f"""
uint8_t {snake_case}_exit(void) {{
    return 0;
}}
"""

    state_source.append(File(
        filename =
            f"{snake_case}.c",
        content = 
            state_source_dependencies +
            state_data_struct_definition +
            state_data_struct_declaration +
            state_init +
            state_step +
            state_exit
        ))

    state_source.append(File(
        filename =
            f"{snake_case}.h",
        content =
            state_header_dependencies +
            state_header_declarations
        ))

states_source_common_dependencies = f"""
#ifdef TEST
#include "mock_gb.h"
#else
#include <gb/gb.h>
#endif

#include "states.h"
"""

states_header_dependencies = f"""
#include "{state_header_path}"
"""

states_source = File(
    filename =
        "states.c",
    content =
        states_source_common_dependencies +
        "".join(states_source_individual_dependencies) +
        "".join(states_source_individual_assign_definitions)
    )

states_header = File(
    filename =
        "states.h",
    content =
        states_header_dependencies +
        "".join(states_header_individual_assign_declarations) +
        "\n"
    )

runner_source_dependencies = f"""
#ifdef TEST
#include "mock_gb.h"
#include "mock_crash_handler.h"
#else
#include <gb/gb.h>
#include <gb/crash_handler.h>
#endif

#include "states.h"
"""

runner_source_struct_definition = f"""
state currentState;
"""

runner_source_init = f"""
void runner_init(void) {{
    assign_{snake_case_list[0]}(&currentState);
}}
"""

runner_source_step = f"""
uint8_t runner_step(void) {{
    switch (run(&currentState)) {{
        case 0:
            break;"""

for i, s in enumerate(snake_case_list):
    runner_source_step += f"""
        case {i + 1}:
            assign_{snake_case_list[i]}(&currentState);
            break;"""
    
runner_source_step += """
        default:
            __HandleCrash();
            break;
    }
}
"""

runner_source_exit = f"""
uint8_t runner_exit(void) {{
    return 0;
}}
"""

runner_source = File(
    filename =
        "runner.c",
    content =
        runner_source_dependencies +
        runner_source_struct_definition +
        runner_source_init +
        runner_source_step +
        runner_source_exit
    )

runner_header = File(
    filename =
        "runner.h",
    content =
        """
#include <gb/gb.h>

void runner_init(void);
uint8_t runner_step(void);
uint8_t runner_exit(void);
"""
    )

for s in state_source:
    with open(file = f"output/states/{s.filename}", mode = "w") as f:
        f.write(s.content)

with open(file = f"output/{states_source.filename}", mode = "w") as f:
    f.write(states_source.content)
 
with open(file = f"output/{states_header.filename}", mode = "w") as f:
    f.write(states_header.content)

with open(file = f"output/{runner_source.filename}", mode = "w") as f:
    f.write(runner_source.content)

with open(file = f"output/{runner_header.filename}", mode = "w") as f:
    f.write(runner_header.content)
