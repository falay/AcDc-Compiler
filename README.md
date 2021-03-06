# AcDc-Compiler

Introduction
------------
The implementation of the compiler of the ac language :
*  Implement 4 phases of a general compiler. 
*  Support constant folding optimization.

Compatibility
-------------
AcDc compiler is capable of compiling ac language:
*  Support integer and floating number declaration and computaion.
*  Support assignment and print statement.

Usage 
-------------
```
make
```
or
```
make clean all
```
to compile the source

```
./AcDc [file.ac] [target.dc]
```
It generates code of dc language which is a stack machine code,
we provide simulator to execute the target dc code to check the result.

```
cd Simulator
make
./simulator [target.dc]
```

Acknowledgements
----------------
AcDc Compiler is based on the infrastructure of chapter 2 of [Crafting a Compiler](https://www.pearsonhighered.com/program/Fischer-Crafting-A-Compiler/PGM315544.html)

