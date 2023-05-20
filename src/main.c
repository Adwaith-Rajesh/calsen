/*
MIT License

Copyright (c) 2023 Adwaith Rajesh

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int verbose_flag;

void print_help(int type, char *filename) {
    char *simple_usage =
        "Usage: %s <search/reindex> [OPTIONS]\n"
        "--help, -h \tShow this message and quit.\n";

    char *reindex_usage =
        "Usage: %s reindex [OPTIONS]\n"
        "--output, -o \tThe file to output the indexed data to.\n"
        "--verbose, -v \tGet verbose output.\n"
        "--help, -h \tShow this message and quit.\n";
    char *search_usage =
        "Usage: %s search [OPTIONS]\n"
        "--index, -i \tThe file that contains the indexed data.\n"
        "--verbose, -v \tGet verbose output.\n"
        "--help, -h \tShow this message and quit.\n";

    switch (type) {
        case 2:
            printf(reindex_usage, filename);
            exit(0);
        case 3:
            printf(search_usage, filename);
            exit(0);
        default:  // this include case 1
            printf(simple_usage, filename);
            exit(0);
    }
}

int main(int argc, char **argv) {
    // --dir -d  --> the dir to include during indexing
    // --verbose -v --> give info on the current state / more output
    // --help -h --> show help and quit
    // --output -o --> the output index file for the index subcommand
    // --index -i -->  the index file for the search subcommand
    // subcommand:
    //      reindex
    //      search

    if (argc < 2) {
        print_help(1, argv[0]);
    }

    while (1) {
    }

    return 0;
}
