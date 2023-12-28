# Python
It's just a fractal visualizer in Python.

I made it as an exercise for myself: I did not use any librairies.

Therefore, I had to create complex numbers, as well as cosine and sine functions.

(Therefore, it is *not* optimised, and runs super slowly)

You can change the equation in the *computeNext(z,c)* function.

You can either visualize the fractal by printing the matrix (using printMatrix(fractalMatrix))

Or by generating a .html file (using matrixToHTML("fractal.html",fractalMatrix))

You can also create the corresponding Julia fractal at point C by doing this:

juliaMatrix = checkMatrixInsideJuliaSet(matrix, C, 20, 2)

(You can then visualize the fractal by using printMatrix or matrixToHTML)

## Performance for a n = 100 Mandelbrot fractal
Generate the matrix grid: 0.004s

Generate the fractal: 26.897s

Generate the HTML file: 0.0132s

Total: 26.9141s

# C
Same thing as Python. But way quicker. (700x faster than Python from my testing)

Did not use complex.h header file.

## Performance for a n = 100 Mandelbrot fractal
Generate the matrix grid: 0.001s

Generate the fractal: 0.025s

Generate the HTML file: 0.013s

Total: 0.039s

## TODO:
Remove the math.h header file, and implement the math function myself.

# Haskell
Same thing as Python and C.

Did not use the Data.Complex package.

## Performance for a n = 100 Mandelbrot fractal
Generate the matrix grid: 0s

Generate the fractal: 0s

Generate the HTML file: 36.39s

Total: 39.39s

