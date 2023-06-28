# calsen

A search engine for local files

---

Currently WIP anything can change at any time.

## Why ...

I just wanted to a search to engine to search through my files, Because I'm really good at organizing them.

---

Why rewrite the [previous project](https://github.com/Adwaith-Rajesh/calsen/tree/main) in `V`

- The previous was a bit unstable, even though it never crashed I have this weird feeling that it will.
- V offers some great features that I was implementing it by hand (hash tables, lists, string ...etc).
- and, last but not the least, I have 0 experience in V

---

## Usage

### Building

> Make sure you have installed V

- Clone the repo and cd into it

```console
git clone --depth=1 https://github.com/Adwaith-Rajesh/calsen
cd calsen
```

- Build the exe

```console
v -keepc -prod .
```

### Making the index

```console
./calsen reindex -dir /path/to/dir -dir /path/to/another/dir -i calsen.index
```

### Searching for files

```console
./calsen search -i ./calsen.index -q 'the query to search'
```

> This will print out a list of file that match the query with the most relevant filename first

Bye .....
