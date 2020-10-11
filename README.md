# Building-a-Basic-Shell
Objective: The shell is a program that interprets commands and acts as
an intermediary between the user and the inner workings of the operating
system and as such is arguably one of the most important parts of a Unix
system.I start making my own version of
a Unix shell.

The shell implement following features:
1) The shell should run continuously, and display a prompt (>, similar to $)
when waiting for input. Include the full path of the current directory also in
the prompt followed by the “>” sign (for ex., /usr/home/me/Desktop>).
2) The shell read a line from input one at a time.
3) After interpreting the command, the shell execute it.
4) Implement the following Built-in Commands,
   A) clear: clear the screen
   B) env: displays all environment parameters
   C) cd: change to the directory as specfied by the user
   D) pwd: prints the current directory
   E) mkdir : creates a directory called "dir", “dir” is the argument from the user
   F) rmdir : removes the directory called "dir"
   G) ls: lists files in current directory
   H) exit: exits the shell
