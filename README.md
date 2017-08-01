# AcDc-Compiler

Introduction
------------
The implementation of the simulation of an unix-based file system.
*  Implement 7 commands: 
```
ls, cd, mkdir, read, write, rm, rmdir
```
* The form of the usage of the *read, write* commands:
```
write [file path] "file content"
```
It creates a new file if the file name does not exit, it appends the contents to the file if it already exists.

```
read [file path]
```
It outputs the corresponding file content of file path. 

* The other 5 commands have the same function in the common unix based file system.


Usage 
-------------
Compiling:
```
make
```
or
```
make clean all
```
Executing:

```
./fileSystem
```
