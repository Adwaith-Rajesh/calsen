// The indexer deals with loading and dumping the indexed data

/*
The indexed data is stored flat file

| --- filenames begin with a colon.
|     this can be guaranteed as the lexer ignores punctuation marks.
v

:filename=/absolute/path/to/file
token1=0.3
token2=0.3
token3=0.3
:filename=/absolute/path/to/other/file
token1=0.444
token1=0.444
token1=0.444
*/
#include <stdio.h>
#include <stdlib.h>
