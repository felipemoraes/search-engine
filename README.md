
# Search Engine Project
This is a completed search engine with indexer and searcher implemented with C++14. 
These are assignments from Information Retrieval class at Computer Science Department - UFMG.

---

## Requirements:
1. C++14 (Clang or GCC).
2. [Cmake](http://www.cmake.org/)
3. [Htmlcxx Parser](htmlcxx.sourceforge.net)
4. [OpenSSL]
5. [Boost]

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

## Searcher execution



```bash
./bin/searcher <execution flags>
```

#### searcher flags

| flag           | meaning       |
| -------------  |:-------------:|
|* –directory or -d| Indicates the directory that is located collection of documents that was indexed|


This start the server for web-search interface deep-Search [deep-Search](https://github.com/felipemoraes/web-search).
## Evaluation execution

```bash
./bin/evaluation <execution flags>
```

#### evaluation flags

| flag           | meaning       |
| -------------  |:-------------:|
|* –directory or -d| Indicates the directory that is located collection of documents that was indexed|
|* –query_file or -q| Text file with queries separated by linebreak|
|* –folder or -f| Indicates the directory that is located the relevant documents for each query|

The program reads each query and print out results for each model in a trec_eval format.

* *required!

