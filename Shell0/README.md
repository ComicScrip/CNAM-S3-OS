# Shell0

A school project whose aim is to implement a minimalist bash-like shell in C.

## Run the program

```
$ make && bin/Shell0
```

## Running the tests

Download, build and install CMocka unit testing framework :

```
$ sudo apt-get install cmake && wget https://cmocka.org/files/1.1/cmocka-1.1.1.tar.xz && tar xf cmocka-1.1.1.tar.xz && cd cmocka-1.1.1 && mkdir build && cd build && cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug .. && make && sudo make install && cd ../.. && rm -rf cmocka-1.1.1 && rm cmocka-1.1.1.tar.xz
```
Each module of the project is to be tested individually. For exemple, to run the tests for the "list" module :
```
$ make tests && tests/bin/list.test
```

## Checking for memory leaks

You can use valgrind to check for memory leaks :
```
$ sudo apt-get install valgrind
```

Then you can check memory leaks for each module :
```
$ make tests && valgrind --leak-check=full --track-origins=yes tests/bin/list.test
```

## Generating and browsing the docs

```
$ sudo apt-get install doxygen graphviz
$ make doc && xdg-open doc/html/index.html
```

## Authors

* Pierre Genthon
* Mathilde Wendel
