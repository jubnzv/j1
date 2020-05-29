# J1
Toy virtual machine and Forth to bytecode translator that emulates a small stack-based CPU J1.

The J1 is a simple 16-bit CPU. It has some RAM, a program counter (PC), a data stack and a call/return stack. It has a small set of built-in arithmetic instructions. Fields in the J1 instructions control the arithmetic function, and write the results back to the data stacks.

### Build
```bash
cmake . -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
ctest  # running unit tests
```

### Usage
```bash
output/bin/j1c examples/add.ft -o examples/add.bin
output/bin/j1vm examples/add.bin
```

```
RAM:
<empty>
 <255 empty lines>
PC:     0000000000000001
Data stack:
 0000000000000010
 0000000000000001
 0000000000000001
Return stack:
 <empty>
```

### Resources
1. [The J1 Forth CPU](https://www.excamera.com/sphinx/fpga-j1.html)
2. Forth CPU. Что это такое? ([часть 1](https://habr.com/ru/post/133338/), [часть 2](https://habr.com/ru/post/133380/), [source](https://bitbucket.org/zserge/j1vm/))
3. [dim13/j1](https://github.com/dim13/j1)
4. [jamesbowman/j1](https://github.com/jamesbowman/j1)
5. [Forth Wizard](http://sovietov.com/app/forthwiz.html)
6. [samawati/j1eforth](https://github.com/samawati/j1eforth/)
7. [opencores: forth-cpu](https://opencores.org/websvn/filedetails?repname=forth-cpu&path=%2Fforth-cpu%2Ftrunk%2Freadme.md)
8. [Создаём виртуальный Forth-процессор](https://lhs-blog.info/programming/dlang/sozdaem-virtualnyiy-protsessor-forth/)
