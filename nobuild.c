#define NOBUILD_IMPLEMENTATION
#include "nobuild.h"

#include <stdio.h>
#include <stdlib.h>

#define C_FLAGS "-Wall", "-Wextra", "-pedantic", "-std=c11"

void build_main() {
    INFO("Building main");
    MKDIRS("build", "bin");
    CMD("cc", C_FLAGS,
        "-o",
        PATH("build", "bin", "calsen"),
        PATH("src", "main.c"));
}

int main(int argc, char **argv) {
    GO_REBUILD_URSELF(argc, argv);
    build_main();
    return EXIT_SUCCESS;
}
