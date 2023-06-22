## The `.calsenconfig` file

Unlike the previous version of calsen where the configs where separated into different
files, here all the config will be in one file

### A sample config file

```txt
INIT: {
    CONFIG: {
        index_dir: /path/to/index/dir   # dir where all the indexes will be stored
        # if not provided then the dir where the config file is stored will be used.

    }

    IGNORE: {
        *venv*
        *env*
        *node_modules*  # ignores anything that contains `node_modules`
    }
}

QUERY: {
    # WIP
}

```

### Wildcard matching in ignore section

`*` - matches everything

`?` - matches any single character

`[seq]` - matches any of the characters in the sequence

`[^seq]` - matches any character that is NOT in the sequence Any other character in pattern, is matched 1:1 to the corresponding character in name, including / and .
You can wrap the meta-characters in brackets too, i.e. [?] matches ? in the string, and [*] matches \* in the string.

> Note: wildcard patterns are NOT the same as regular expressions.
