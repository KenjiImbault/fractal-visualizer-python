#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

// Complex numbers definition and operations
typedef struct {
    float real;
    float img;
} Complex;

void printComplex(Complex a) {
    // Check if both real and imaginary parts are zero
    if (a.real == 0 && a.img == 0) {
        printf("0");
    }
    else {
        // Print the real part if it's not zero
        if (a.real != 0) {
            printf("%.2f", a.real);
        }

        // Print the imaginary part if it's not zero
        if (a.img != 0) {
            if (a.real != 0) {
                // If real part is also present, determine whether to print + or -
                printf(" %c %.2fi", (a.img > 0) ? '+' : '-', (a.img > 0) ? a.img : -a.img);
            } else {
                // If only imaginary part is present and it's negative, print it directly
                printf("%.2fi", a.img);
            }
        }

        // Print a new line at the end
        //printf("\n");
    }
}

Complex conjugate(Complex a) {
    Complex c;
    c.real = a.real;
    c.img = -a.img;
    return c;
}

Complex add(Complex a, Complex b) {
    Complex c;
    c.real = a.real + b.real;
    c.img = a.img + b.img;
    return c;
}

Complex add_float_complex(float b, Complex a) {
    Complex c;
    c.real = a.real+b;
    c.img = a.img;
    return c;
}

Complex add_complex_float(Complex a, float b) {
    Complex c;
    c.real = a.real+b;
    c.img = a.img;
    return c;
}

Complex sub(Complex a, Complex b) {
    Complex c;
    c.real = a.real - b.real;
    c.img = a.img - b.img;
    return c;
}

Complex multComplex(Complex a, Complex b) {
    Complex c;
    c.real = a.real*b.real - a.img*b.img;
    c.img = a.real*b.img + a.img*b.real;
    return c;
}

Complex mult_float_complex(float a, Complex b) {
    Complex c;
    c.real = b.real*a;
    c.img = b.img*a;
    return c;
}

Complex mult_complex_float(Complex b, float a) {
    Complex c;
    c = mult_float_complex(a,b);
    return c;
}

Complex reciprocal(Complex a) {
    Complex c;
    c.real = a.real/(a.real*a.real+a.img*a.img);
    c.img = -a.img/(a.real*a.real+a.img*a.img);
    return c;
}

Complex divComplex(Complex a, Complex b) {
    Complex c;
    c = multComplex(a,reciprocal(b));
    return c;
}

float absComplex(Complex a) {
    float result = sqrt(a.real*a.real+a.img*a.img);
    return result;
}

float arg(Complex a) {
    float result;
    if ( a.img<0 ){
        result = -acos(a.real/absComplex(a));
    }
    else {
        result = acos(a.real/absComplex(a));
    }
    return result;
}

Complex powComplex(Complex a, int n) {
    Complex c;
    float r;
    float phi;
    if (a.real == 0 && a.img == 0 && n>0) {
        c.real = 0;
        c.img = 0;
    }
    else {
        r = absComplex(a);
        phi = arg(a);
        c.real = pow((double) r,n) * cos((double) n*phi);
        c.img = pow((double) r,n) * sin((double) n*phi);
    }
    return c;
}

// Fractal computation

// Can change the equation here:
Complex computeNext(Complex z, Complex c) {
    /*
    Mandelbrot: add(powComplex(z,2),c)
    Tricorn: add(powComplex(conjugate(z),2),c);
    */
    Complex result;
    result = add(powComplex(z,2),c);
    return result;
}

Complex computeN(Complex z, Complex c, int n) {
    for (int i=0; i<n; i++) {
        z = computeNext(z,c);
    }
    return z;
}

int checkIfInsideSet(Complex z, Complex c, int n, int r) {
    //n: max number of time we calculate the equation
    //r: if the calculation is over the value of r, then the point is not inside the set

    for (int i=0; i<n; i++) {
        z = computeNext(z,c);
        if (absComplex(z)>r) {
            return 0;
        }
    }
    return 1;
}

Complex** generateMatrix(int n, int m) {
    // Allocate memory for the matrix
    Complex** matrix = (Complex**)malloc(n * sizeof(Complex*));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        matrix[i] = (Complex*)malloc(m * sizeof(Complex));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Initialize the matrix (optional)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j].real = 0;
            matrix[i][j].img = 0;
        }
    }

    return matrix;
}

int** generateMatrixInt(int n, int m) {
   int** matrix = (int**)malloc(n * sizeof(int*));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(m * sizeof(int));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Initialize the matrix (optional)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j]= 0;
        }
    }

    return matrix; 
}

void deleteMatrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void deleteComplexMatrix(Complex** matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void printMatrix(int** matrix, int n, int m) {
    printf("[\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%4d", matrix[i][j]); // Adjust the format specifier as needed
        }
        printf("\n");
    }
    printf("]");
}

void printComplexMatrix(Complex** matrix, int n, int m) {
    printf("[");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("[");
            printComplex(matrix[i][j]);
            printf("] ");
        }
        printf("]\n");
    }
}

float* lin_int(float min, float max, int n) {
    if (n <= 0) {
        fprintf(stderr, "Invalid value for n\n");
        exit(EXIT_FAILURE);
    }

    float* result = (float*)malloc(n * sizeof(float));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    float step = (max - min) / (float)(n - 1);

    for (int i = 0; i < n; i++) {
        result[i] = min + i * step;
    }

    return result;
}

void printFloatArray(float* arr, int n) {
    printf("{");
    for (int i = 0; i < n; i++) {
        printf("%.2f", arr[i]);
        if (i < n - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

Complex** generateMatrixValues(int n, int m, int minValueReal, int maxValueReal, int minValueImg, int maxValueImg) {
    float* realValues = lin_int(minValueReal, maxValueReal, m);
    float* imaginaryValues = lin_int(minValueImg, maxValueImg, n);
    Complex** matrix = generateMatrix(n,m);
    Complex** reversedMatrix = generateMatrix(n,m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j].real = realValues[j];
            matrix[i][j].img = imaginaryValues[i];
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            reversedMatrix[i][j] = matrix[n - 1 - i][j];
        }
    }

    return reversedMatrix;
}

int** checkMatrixInsideSet(Complex** matrix, Complex z, int n, int r, int nSize, int mSize) {
    int** newMatrix = generateMatrixInt(nSize,mSize);
    for (int i=0; i<nSize; i++) {
        for (int j=0; j<mSize; j++) {
            if (checkIfInsideSet(z, matrix[i][j], n, r) == 1) {
                newMatrix[i][j]=1;
            }
        }
    }
    return newMatrix;
}

int ** checkMatrixInsideJuliaSet(Complex** matrix, Complex c, int n, int r, int nSize, int mSize) {
    int** newMatrix = generateMatrixInt(nSize,mSize);
    for (int i=0; i<nSize; i++) {
        for (int j=0; j<mSize; j++) {
            if (checkIfInsideSet(matrix[i][j], c, n, r) == 1) {
                newMatrix[i][j]=1;
            }
        }
    }
    return newMatrix;
}

void matrixToHTML(FILE* file, char* fileName, int** matrix, int n, int m) {
    file = fopen(fileName, "w");
    fprintf(file, "<!DOCTYPE html>\n");
    fprintf(file, "<html>\n");
    fprintf(file, "<head>\n");
    fprintf(file, "<title>Fractal</title>\n");
    fprintf(file, "</head>\n");
    fprintf(file, "<body style=\"background-color:white;\">\n");

    // Draws the matrix
    int space=10;
    int intTop = 0;
    int intLeft;
    char left[16];
    char top[16];

    for (int i=0; i<n; i++) {
        sprintf(left, "%d", 0);
        intLeft = atoi(left);
        intTop = intTop+space;
        sprintf(top, "%d", intTop);

        for (int j=0; j<m; j++) {
            if (matrix[i][j] == 1) {
                fprintf(file, "<div style=\"position:absolute; top:%dpx; bottom:0px; left:%dpx; right:0px; margin:auto; width:5px; height:5px; background-color:black;\"></div>\n", intTop, intLeft);
            }
            intLeft=intLeft+space;
            sprintf(left, "%d", intLeft);
        }
    }
    fprintf(file,"</body>\n");
    fprintf(file,"</html>\n");
    fclose(file);
}

int main(){
    
    Complex z;
    z.real = 0;
    z.img = 0;

    FILE *file;

    // Calculate m so that the fractal is not squashed
    int minReal = -2;
    int maxReal = 1;
    int minIm = -1;
    int maxIm = 1;
    int n = 100;

    int m = round( n*((float)(abs(minReal)+abs(maxReal)+1) / (float)(abs(minIm)+abs(maxIm)+1)) );

    printf("%i\n",m);
    clock_t t0 = clock();
    printf("Generating matrix grid...\n");
    Complex** matrix = generateMatrixValues(n,m,minReal,maxReal,minIm,maxIm);
    clock_t t1 = clock();
    double time01 = (double) (t1-t0)/CLOCKS_PER_SEC;
    printf("Generating matrix grid, done. %fs\n",time01);
    
    printf("Generating fractal...\n");
    int** fractalMatrix = checkMatrixInsideSet(matrix, z, 20, 2,n,m);
    clock_t t2 = clock();
    double time12 = (double) (t2-t1)/CLOCKS_PER_SEC;
    printf("Generating fractal, done. %fs\n", time12);
    //printMatrix(fractalMatrix,n,m);
    
    printf("Generating HTML file...\n");
    matrixToHTML(file, "fractal.html", fractalMatrix,n,m);
    clock_t t3 = clock();
    double time23 = (double) (t3-t2)/CLOCKS_PER_SEC;
    printf("Generating HTML file, done. %fs\n", time23);
    deleteMatrix(fractalMatrix, n);

    double total = (double) (t3-t0)/CLOCKS_PER_SEC;
    printf("Total %fs\n", total);

    /*
    Complex c;
    c.real=0.292539267;
    c.img = -0.02421466;
    int** juliaMatrix = checkMatrixInsideJuliaSet(matrix, c, 150, 2, n, m);
    //printMatrix(juliaMatrix, n, m);
    matrixToHTML(file, "juliaFractal.html", juliaMatrix,n,m);
    deleteMatrix(juliaMatrix, n);
    */
    return 0;
}

