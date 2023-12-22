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
