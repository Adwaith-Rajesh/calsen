#define NOBUILD_IMPLEMENTATION
#include "nobuild.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define C_FLAGS "-Wall", "-Wextra", "-pedantic", "-std=c11", "-I./src", "-I./src/utils"

#define OUT_DIR "./build/out"
#define BIN_DIR "./build/bin"

static void _build_object_file(const char *file, const char *o_path) {
    CMD("cc", C_FLAGS, "-o",
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
    const char *out_dir = "./build/out";
    const char *bin_dir = "./build/bin";

    Cstr_Array line = cstr_array_make("cc", C_FLAGS, "-o",
                                      PATH(bin_dir, "calsen"),
                                      NULL);
    FOREACH_FILE_IN_DIR(file, out_dir, {
        if (ENDS_WITH(file, ".o")) {
            line = cstr_array_append(line, PATH(out_dir, file));
        }
    });
    Cmd cmd = {
        .line = line};
    cmd_run_sync(cmd);
}

int main(int argc, char **argv) {
    MKDIRS("build", "out");
    MKDIRS("build", "bin");

    GO_REBUILD_URSELF(argc, argv);
    build_src();
    build_src_utils();
    build_calsen();
    return EXIT_SUCCESS;
}
