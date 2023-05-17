#define NOBUILD_IMPLEMENTATION
#include "nobuild.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CC "cc"  // use the default compiler to compile the code
#define C_FLAGS "-g", "-Wall", "-Wextra", "-std=c11", "-I./src", "-I./src/utils"

#define OUT_DIR "./build/out"
#define BIN_DIR "./build/bin"

typedef struct {
    char parser_file_name[20];
    Cmd cmd;
} CParserCompileCommand;  // custom parser compile command

static void _build_object_file(const char *file, const char *o_path) {
    CMD(CC, C_FLAGS, "-o",
        PATH("build", "out", CONCAT(NOEXT(file), ".o")),
        "-c",
        o_path);
}

void build_src_utils() {
    INFO("Building src/utils");

    const char *src_utils_dir = "./src/utils";

    if (!IS_DIR(src_utils_dir)) {
        INFO("provided src utils dir not a dir!\n");
        exit(EXIT_FAILURE);
    }

    FOREACH_FILE_IN_DIR(file, src_utils_dir, {
        if (ENDS_WITH(file, ".c")) {
            _build_object_file(file, PATH("src", "utils", file));
        }
    });
}

void build_src() {
    INFO("Building main");
    const char *src_dir = "./src";

    if (IS_DIR(src_dir)) {
        FOREACH_FILE_IN_DIR(file, src_dir, {
            if (ENDS_WITH(file, ".c")) {
                _build_object_file(file, PATH("src", file));
            }
        });
    }
}

void build_calsen() {
    INFO("Building Calsen");
    const char *out_dir = "./build/out";
    const char *bin_dir = "./build/bin";

    Cstr_Array line = cstr_array_make(CC, C_FLAGS, "-o",
                                      PATH(bin_dir, "calsen"),
                                      NULL);
    FOREACH_FILE_IN_DIR(file, out_dir, {
        if (ENDS_WITH(file, ".o")) {
            line = cstr_array_append(line, PATH(out_dir, file));
        }
    });
    line = cstr_array_append(line, "-ldl");
    Cmd cmd = {
        .line = line};
    INFO("CMD: %s", cmd_show(cmd));
    cmd_run_sync(cmd);
}

int custom_parser_check_execute(Cstr file) {
    CParserCompileCommand cp_commands[] = {
        // sample code
        // {
        //     .parser_file_name = "text_plain.c",
        //     .cmd = (Cmd){
        //         .line = cstr_array_make(CC, C_FLAGS, "-c",  //
        //                                 PATH("src", "parsers", "text_plain.c"), "-o",
        //                                 PATH("build", "parsers", "text_plain.o"),
        //                                 NULL),
        //     },
        // },
    };

    size_t cp_commands_len = sizeof(cp_commands) / sizeof(CParserCompileCommand);

    if (!cp_commands_len) return 0;

    for (size_t i = 0; i < cp_commands_len; i++) {
        if (strcmp(file, cp_commands[i].parser_file_name) == 0) {
            INFO("CMD: %s", cmd_show(cp_commands[i].cmd));
            cmd_run_sync(cp_commands[i].cmd);
            return 1;
        }
    }
    return 0;
}

void build_parsers() {
    INFO("Building parsers");
    if (!IS_DIR("./src/parsers")) {
        ERRO("parser directory does not exist");
        return;
    }
    FOREACH_FILE_IN_DIR(file, "./src/parsers", {
        if (ENDS_WITH(file, ".c")) {
            if (!custom_parser_check_execute(file)) {
                CMD(CC, C_FLAGS, "-shared", "-fPIC", "-o",
                    PATH("build", "parsers", CONCAT(NOEXT(file), ".so")),
                    PATH(OUT_DIR, "cstring.o"),
                    PATH("./src/parsers", file));
            }
        }
    });
}

int main(int argc, char **argv) {
    MKDIRS("build", "out");
    MKDIRS("build", "bin");
    MKDIRS("build", "parsers");

    GO_REBUILD_URSELF(argc, argv);
    build_src();
    build_src_utils();
    build_calsen();
    build_parsers();
    return EXIT_SUCCESS;
}
