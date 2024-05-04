# loCAL Search ENgine

---

A search engine to search for local files based on their contents and not just their file names.
---

### Road Map

- [ ] parsers:

  - [ ] docx
  - [ ] pdf
  - [ ] pptx
        and other files

- [ ] more docs

---

### Why?

- I've never done any file I/O intensive project.
- I'm really bad at organizing files, so I've files everywhere on my system.
  - And with `Calsen` what I aim is to be able to search for a function name that I remembered in a file and then `Calsen` would 'seamlessly' find the source file that contains it.
- It's fun to do something like this.
- And I can learn a lot of things from this.

---

### Getting started

#### Installation

- clone the repo and cd into calsen.

```console
git clone --depth=1 https://github.com/Adwaith-Rajesh/calsen.git
cd calsen
```

- dependencies (I've plans to make this optional [#2](https://github.com/Adwaith-Rajesh/calsen/issues/2))

```console
apt install libmagic-dev
```

#### Compiling

[`Calsen`](https://github.com/Adwaith-Rajesh/calsen/) makes use of [nobuild](https://github.com/tsoding/nobuild) as it's build system. To compile run the following commands

```console
gcc -o nobuild ./nobuild.c
./nobuild --release
ln -s ./build/bin/calsen ./calsen
```

#### Indexing

To index the required directories run.

```console
./calsen reindex --dir path/to/dir/1 --dir path/to/dir/2 -o sample.index
```

> Use `--verbose` to get additional output

This will create a `.index` file that _Calsen_ will use during the search process.

#### Searching

Inorder to search through the indexed file you can use the following command.

```console
./calsen search -i sample.index -q 'search query'
```

_Calsen_ will find all the files that matches the _"search query"_ and arranges them in descending of relevancy

- To get top N files

```calsen
./calsen search -i sample.index -q 'search query' -n 10
```

> Use `--verbose` to get the calculated TF-IDF score for each file

### Bye...
