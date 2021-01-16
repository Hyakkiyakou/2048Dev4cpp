# 2048Dev4cpp

## Introduction
2048 is a small puzzle game. You can control and merge the numbers in the lattice by pressing "W, S, A, D" for up, down, left and right until 2048 occurs.
## Regulations
Only two numbers can be merged in a merge procedure, and these two numbers should be adjacent(except 0).
In each merge priocedure, the merge direction has high priority.
When "2048" occurs, you win.
When no numbers can be merged and there is no empty lattice, you lose.
Press "Q" to quit the game.
Press "R" to restart.
## Dependent Library
linux: apt-get install libncurses5-dev 
Mac: brew install ncurses 
Windows: 
Install Compiler MinGW, https://sourceforge.net/projects/mingw-w64/files/mingw-w64/mingw-w64-release/
Download mingw-w64-install.exe 5.0.4 version, Unzip to local directory，For example, C:\mingw64, Then add C:\mingw64\bin to PATH, open cmd and enter g++，ensure for install success.

Compile library "pdcurses", https://sourceforge.net/projects/pdcurses/files/pdcurses/3.6/pdcurs36.zip/download
Download pdcurses and unzip to C:\pdcurs36，open cmd to directory C:\pdcurs36\wincon，run command mingw32-make to compile library "pdcurses".
