// License: GPL v3 Copyright: 2023, Adwaith Rajesh <adwaithrajesh3180@gmail.com>

#define NOBUILD_IMPLEMENTATION
#include "nobuild.h"

#include <getopt.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CC "cc"  // use the default compiler to compile the code
#define C_FLAGS (in_release) ? "-O3" : "-g", "-Wall", "-Wextra", "-std=c11", "-I./src", "-I./src/utils"

#define OUT_DIR "./build/out"
#define BIN_DIR "./build/bin"
#define PARSERS_DIR "./build/parsers"

int in_release = 0;
int no_load_config = 0;

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"

void build_src_config(char *config_file_path) {
    INFO("Building src/config");

    const char *src_config_dir = "./src/config";
    char *config_path = JOIN("", "-DCALSENCONFIG=\"", config_file_path, "\"");
    char *load_config = JOIN("", "-DLOAD_CONFIG=", no_load_config ? "0" : "1");

    if (!IS_DIR(src_config_dir)) {
        INFO("provided src/config dir not a dir!\n");
        exit(EXIT_FAILURE);
    }

    FOREACH_FILE_IN_DIR(file, src_config_dir, {
        if (ENDS_WITH(file, ".c")) {
            CMD(CC, C_FLAGS, "-o",
                PATH("build", "out", CONCAT(NOEXT(file), ".o")),
                "-c",
                PATH("src", "config", file),
                config_path, load_config);
        }
    });
}

#pragma GCC diagnostic pop

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

void build_calsen(char *exe_dir) {
    INFO("Building Calsen");
    if (!IS_DIR(exe_dir)) {
        ERRO("exe directory does not exist");
        return;
    }
    Cstr_Array line = cstr_array_make(CC, C_FLAGS, "-o",
                                      PATH(exe_dir, "calsen"),
                                      NULL);
    FOREACH_FILE_IN_DIR(file, OUT_DIR, {
        if (ENDS_WITH(file, ".o")) {
            line = cstr_array_append(line, PATH(OUT_DIR, file));
        }
    });
    line = cstr_array_append(line, "-ldl");
    // might change in the future: https://github.com/Adwaith-Rajesh/calsen/issues/2
    line = cstr_array_append(line, "-lmagic");
    line = cstr_array_append(line, "-lm");
    Cmd cmd = {
        .line = line};
    INFO("CMD: %s", cmd_show(cmd));
    cmd_run_sync(cmd);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overread"

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

    // this loop will only run if there is an element in the array
    // so its safe to access it and also ignore the "-Wstringop-overread" warning
    for (size_t i = 0; i < cp_commands_len; i++) {
        if (strcmp(file, cp_commands[i].parser_file_name) == 0) {
            INFO("CMD: %s", cmd_show(cp_commands[i].cmd));
            cmd_run_sync(cp_commands[i].cmd);
            return 1;
        }
    }
    return 0;
}

#pragma GCC diagnostic pop

void build_parsers(char *parsers_dir) {
    INFO("Building parsers");
    if (!IS_DIR(parsers_dir)) {
        ERRO("parser directory does not exist");
        return;
    }
    FOREACH_FILE_IN_DIR(file, "./src/parsers", {
        if (ENDS_WITH(file, ".c")) {
            if (!custom_parser_check_execute(file)) {
                CMD(CC, C_FLAGS, "-shared", "-fPIC", "-o",
                    PATH(parsers_dir, CONCAT(NOEXT(file), ".so")),
                    PATH(OUT_DIR, "cstring.o"),
                    PATH("./src/parsers", file));
            }
        }
    });
}

void show_help_and_exit(char *filename) {
    printf(
        "Usage: %s [OPTIONS]\n"
        "--release \t\tBuild calsen in release mode with -O3 optimization\n"
        "--no-load-config \tPrevent calsen from the loading the config file\n"
        "--parser-dir, -p \tSpecify the directory where the parses object file will be stored\n"
        "--exe-dir, -e \t\tSpecify the directory where the calsen executable will be stored\n"
        "--config, -c \t\tSpecify the path to the config file\n"
        "--help \t\t\tShow this message and exit\n",
        filename);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    GO_REBUILD_URSELF(argc, argv);

    int show_help = 0;
    int option_index = 0;
    struct option long_options[] = {
        {"help", no_argument, &show_help, 1},
        {"release", no_argument, &in_release, 1},
        {"no-load-config", no_argument, &no_load_config, 1},
        {"parsers-dir", required_argument, 0, 'p'},
        {"exe-dir", required_argument, 0, 'e'},
        {"config", required_argument, 0, 'c'},
        {0, 0, 0, 0},
    };

    char config_path[PATH_MAX] = "./.calsenconfig";
    char parsers_dir[PATH_MAX] = PARSERS_DIR;
    char exe_dir[PATH_MAX] = BIN_DIR;

    while (1) {
        int c = getopt_long(argc, argv, "c:p:e:", long_options, &option_index);

        if (c == -1) break;
        switch (c) {
            case 'c':
                strcpy(config_path, optarg);
                break;
            case 'p':
                strcpy(parsers_dir, optarg);
                break;
            case 'e':
                strcpy(exe_dir, optarg);
                break;
        }
    }

    if (show_help == 1) show_help_and_exit(argv[0]);

    MKDIRS("build", "out");
    MKDIRS("build", "bin");
    MKDIRS("build", "parsers");
    build_src();
    build_src_utils();
    build_src_config(config_path);
    build_calsen(exe_dir);
    build_parsers(parsers_dir);
    return EXIT_SUCCESS;
}
