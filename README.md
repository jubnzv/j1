# J1 Emulator
Emulator for a small stack-based CPU J1.

### Usage
Build:
```bash
cmake .
make -j$(nproc)
```

Running unit tests:
```bash
tests/output/bin/unit_tests
```

Execute demo ROM:
```bash
output/bin/j1 assets/add.bin
```

Start interactive session:
```bash
output/bin/j1
```

### Resources
1. [doc/j1.pdf](doc/j1.pdf)
2. Forth CPU. Что это такое? ([часть 1](https://habr.com/ru/post/133338/), [часть 2](https://habr.com/ru/post/133380/), [source](https://bitbucket.org/zserge/j1vm/))
3. [dim13/j1](https://github.com/dim13/j1)
4. [jamesbowman/j1](https://github.com/jamesbowman/j1)
5. [Forth Wizard](http://sovietov.com/app/forthwiz.html)
6. [samawati/j1eforth](https://github.com/samawati/j1eforth/)
7. [The J1 Forth CPU](https://www.excamera.com/sphinx/fpga-j1.html)
8. [opencores: forth-cpu](https://opencores.org/websvn/filedetails?repname=forth-cpu&path=%2Fforth-cpu%2Ftrunk%2Freadme.md)
9. [Создаём виртуальный Forth-процессор](https://lhs-blog.info/programming/dlang/sozdaem-virtualnyiy-protsessor-forth/)
