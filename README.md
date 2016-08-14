# Minesweeper

[![Build Status](https://travis-ci.org/MRoundDE/minesweeper.svg?branch=master)](https://travis-ci.org/MRoundDE/minesweeper)

Yet another
[Minesweeper](https://en.wikipedia.org/wiki/Minesweeper_(video_game))
implementation using [libsdl2](https://libsdl.org/),
[libsdl2_ttf](https://www.libsdl.org/projects/SDL_ttf/), and
[GNU FreeFont](http://www.gnu.org/software/freefont/).

![Screenshot of Minesweeper](https://raw.githubusercontent.com/siko1056/minesweeper/master/doc/screen-0.1.png)


## Development

The development platform is Linux.  If all requirements are installed, just
type

    make && ./minesweeper

from the project root directory to compile and run Minesweeper.


### Windows

Using [mingw](http://www.mingw.org/), it is possible to cross compile this
project from Linux to create a Windows executable.  Just download the
[SDL2 development library](https://www.libsdl.org/release/SDL2-devel-2.0.4-mingw.tar.gz)
and the
[SDL2_ttf development library](https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.14-mingw.tar.gz),
extract them to a path of your choice and type

    mingw64-make \
      SDL2_PATH_WIN=/your/path/to/SDL2-2.0.4 \
      SDL2_TTF_PATH_WIN=/your/path/to/SDL2_ttf-2.0.14 dist-win \
      dist-win

from the project root directory.  A zip-archive, containing all relevant
DLLs, assets, and executable files for Windows, is created within the project
root directory.
