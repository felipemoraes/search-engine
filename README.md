
# Search Engine Project
This is a completed search engine with indexer and searcher implemented with C++11. 
These are assignments from Information Retrieval class at Computer Science Department - UFMG.

---

## Requirements:
1. C++11 (Clang or GCC).
2. [Cmake](http://www.cmake.org/)
3. [Gumbo Parser](https://github.com/google/gumbo-parser)

## Compilation

```bash
cmake -G "Unix Makefiles"
make
```

## Indexer execution

```bash
./bin/indexer <execution flags>
```

#### indexer flags

| flag             | meaning       |
| -------------    |:-------------:|
|* –directory or -d| Indicates the directory that is located collection of documents to be indexed|
|  –fileName or -f | Indicates the name of the index file of the document collection. Default "index.txt"|
|–runSize or -r    | Maximum buffer count (triple <t, d, f, t>) should be stored in main memory. Defaults to 5|
|–output or -o     | Directory that final index should be stored. Default program execution folder|
|–numDocs or -n    | Number of files to be indexed (if not supplied defaults to 999,999,999|
|* –stopwords ou -s| Directory that is located lists of stopwords.|

* *required!

## Searcher execution

```bash
./bin/searcher <execution flags>
```

#### searcher flags

| flag           | meaning       |
| -------------  |:-------------:|
|–directory or -d| Indicates the directory that is located collection of documents to be indexed|

The program reads from stander input. First reads a integer m that represents m queries to be perform. 
Then read a string "and" or "or" representing logical connector beetween query termies.
After follows m lines with a integer n representing query length and then n terms.


