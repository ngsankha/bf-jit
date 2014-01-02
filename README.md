# Brainf*ck JIT

A JIT compiler for Brainf*ck programs made using LibJIT.

It does very naive optimizations right now, like folding of multiple same instructions into one.

Blog post about the same is [here](http://thelimbeck.wordpress.com/2013/12/31/a-brainfck-jit/).

## Build Instructions

* Download and build [LibJIT](http://www.gnu.org/software/libjit/). I have a compiled version for my own system (Linux 32-bit) in the repo itself, you can use that.
* Update the `LIBJIT_PATH` variable in the `Makefile` if you are using your own LibJIT binaries.
* Run `make`.

## Benchmarks

The `bench` folder has a `bench.sh` script that tests the Mandelbrot program with different implementations. The results on my system are:

```
Implementation                      Time
----------------------------------------
Mandelbrot (C Unoptimized)       22.343s
Mandelbrot (C Optimized)          1.241s
Mandelbrot (BF Interpreter)       8.304s
Mandelbrot (BF JIT)               1.609s
```

The unoptimized JIT is 5x faster than the optimizing interpreter and almost at par with the optimized C code.
