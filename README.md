# loCAL Search ENgine

---

A search engine to search for local files based on their contents and not just their file names.

---

### Road Map

- [x] Build Util Libs for the project (HashMaps and stuff like that.)
- [x] Dynamic Parser Loading for individual files based on MIME type (getting the text content from the files)
- [x] Lexing the contents from the parser into tokens
- [ ] Calculate the TF score for each token in a file
  - Calsen uses [TF-IDF](https://en.wikipedia.org/wiki/Tf%E2%80%93idf) for ranking files based on the query.
- [ ] Figure out a way to store the indexed data.
- [ ] Get the query, calculate the TF-IDF score for each file, then display the top N files.
  - Parsing the query
- [ ] Build a new UI (MayBe)

---

### Why?

- I've never done any file I/O intensive project.
- I'm really bad at organizing files, so I've files everywhere on my system.
  - And with `Calsen` what I aim is to be able to search for a function name that I remembered in a file and then `Calsen` would 'seamlessly' find the source file that contains it.
- It's fun to do something like this.
- And I can learn a lot of things from this.

---

### Bye...
