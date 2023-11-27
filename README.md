<h1 align="center">document viewer with less and soft-wrap functionality</h1>
<p align="center">Or simple <b>docvls</b> - It's TUI (ncursesw) program, which shows files as multipage document in cosy interface and include features: turn page in both side, scroll page up and down. Inspired by <a href="https://github.com/boroveen/mohar">mohar</a>. This program is released under license GPL-3.0 or later.</p>

## compilation
**main program:**
```bash
$ git clone https://github.com/gH2843/docvls && cd docvls && cmake CMakeLists.txt && make -j
```
**plugin "multiply method of russ":**
```bash
$ cd ./plugins
$ g++ -shared -o librusMultiply.so russMultiplyMethod_tab.cc -lncursesw -fPIC -std=c++2b
$ mkdir -p ~/.config/docvls/plugins
$ mv librusMultiply.so ~/.config/docvls/plugins/
```
## how to use
You can test program by use test doc - `./test/doc.aml`. In other words use `./docvls ./test/doc.aml`.
Doc.aml uses text from [this site](https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/) by Pradeep Padala.
Program takes as argument path to .aml file - my implementation of simple markup language: the file should start and end with '*'. Star symbol - mean new page. Thus file can include more than two a star symbols
Also '#' - mean page name. All text  from '#' to '\n' writes into "name" variable into Document class. But this feature doesn't use at this release.

**After start:**
Use arrow down/up to move down/up on page. Use arrow left/right to turn page to next/prev. Use 1..9 to enter a page number to go, backspace to delete one digit from a page number to go. And use KEY_ENTER to go to page, that you typed.

**Plugins:**
Press F1 to go to plugin manager(PluM). The PluM will try to found .so files inside ~/.config/docvls/plugins/ dir. You can chose needed plugin by use up/down arrow and KEY_ENTER to load it. After this it will aperred at top. To use plugin you need press 'q' to exit plugin manager. After this press key defined by plugin. usually it F* keys.
### official plugins
* **Russian multiply method(librussMultiplyMethod.so):** shows the multiplplications steps in old rus style. Use 1..9 keys to enter first operand, then press *, after this use 1..9 keys again to enter second operand. Use enter to calculate a expression. You can use backspace to erase one char from a expression. 'q' to exit to main program.
### what with roadmap?
I'm planing to add alternative Document class initialization by passing path to directory with .txt files, because it's more useful than initialization by .aml file. Displays key of the plugin.
Also, I need to add file's name displaying
## footnote
* Document - multi-page file
* Less - Unix program, that shows a file's contents one screen at a time. [Link to wiki](https://en.m.wikipedia.org/wiki/Less_(Unix))
