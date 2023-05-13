## General Info

The parsers are named after the [MIME type](https://en.wikipedia.org/wiki/Media_type) it parses.
Example:
if the MIME type is `text/plain`, the name of the parser file will be `text_plain.c`. (The `/` is replaced with `_`)

Every parser defined must have a `String *parse_file(const char *path)` function as an entry point to the parser.

Since [`calsen`](https://github.com/Adwaith-Rajesh/calsen) also implements an Arena allocator, if `String *parse_file_arena(const char *path, Arena *arena)` is defined.
Then it will be used.

The returned `String *` has to be freed by the user and not by the parser.

The `String` type is defined in `src/utils/cstring.h`

The `Arena` type is defined in `src/utils/Arena.h`

## compilation

By default the parsers will compiled to a shared object using the following command.

```console
gcc -Wall -Wextra -pedantic -std=c11 -I./src -I./src/utils -shared -fPIC -o <parser-name>.so <parser-name>.c ./build/out/arena.o ./build/out/cstring.o
```

If the parser requires a special command (eg. depends on other packages .. etc), then
a custom command can be used to compile the parser by adding an entry in the `cp_commands` array in the
`custom_parser_check_execute()` function in the `nobuild.c` build script.

A sample custom build command entry for a parser would look something like this.

```c
{
    .parser_file_name = "text_plain.c",
    .cmd = (Cmd){
        .line = cstr_array_make("cc", C_FLAGS, "-c",  //
                                PATH("src", "parsers", "text_plain.c"), "-o",
                                PATH("build", "parsers", "text_plain.o"),
                                NULL),
            },
},
```

> in the above example, instead of creating an `.so` file it crates and `.o` file

## parser directory

By default `calsen` searches for parsers in the `./build/parsers` directory, it can be changed by setting the environment variable `CALSEN_PARSER_DIR=<parser_dir>`.
