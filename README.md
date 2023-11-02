<h1 align="center">document viewer with less and soft-wrap functionality</h1>
<p align="center">Or simple <b>docvls</b> - It's TUI (ncursesw) program, which shows files as multipage document in cosy interface and include features: turn page in both side, scroll page up and down. Inspired by <a href="https://github.com/boroveen/mohar">mohar</a>. This program is released under license GPL-3.0 or later.</p>

## compilation
```bash
$ git clone https://github.com/gH2843/docvls && cd docvls && cmake CMakeLists.txt && make -j
```
## how to use
You can test program by use test doc - `./test/doc.aml`. In other words use `./docvls ./test/doc.aml`.
Doc.aml uses text from [this site](https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/) by Pradeep Padala.
Program takes as argument path to .aml file - my implementation of simple markup language: the file should start and end with '*'. Star symbol - mean new page. Thus file can include more than two a star symbols
Also '#' - mean page name. All text  from '#' to '\n' writes into "name" variable into Document class. But this feature doesn't use at this release.
### what with roadmap?
I'm planing to add alternative Document class initialization by passing path to directory with .txt files, because it's more useful than initialization by .aml file.
Also, I need to add file's name displaying
## footnote
* Document - multi-page file
* Less - Unix program, that shows a file's contents one screen at a time. [Link to wiki](https://en.m.wikipedia.org/wiki/Less_(Unix))
