echo "Building mandelbrot.c ..."
gcc -o mandelbrot-unopt.out mandelbrot.c
echo "Building mandelbrot.c (optimized) ..."
gcc -O3 -o mandelbrot-opt.out mandelbrot.c
echo "Running Mandelbrot C (Unoptimized) ..."
time ./mandelbrot-unopt.out > out.txt
echo "Running Mandelbrot C (Optimized) ..."
time ./mandelbrot-opt.out > out.txt
echo "Running Mandelbrot Brainf*ck (Interpreter) ..."
time bf mandelbrot.bf > out.txt
echo "Running Mandelbrot Brainf*ck (JIT) ..."
time ./../bf-jit mandelbrot.bf > out.txt

