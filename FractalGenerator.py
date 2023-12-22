class Complex:
    def __init__(self, real, imaginary):
        self.real = real
        self.imaginary = imaginary

    def __str__(self):
        sign='+'
        real=str(self.real)
        imaginary=str(self.imaginary)+'i'

        if self.imaginary<=0:
            sign=''
        if self.imaginary==0:
            imaginary=''
        if self.real==0:
            real=''
        if self.real==0 and self.imaginary==0:
            return '0'
        return real+sign+imaginary

    def __repr__(self):
        sign='+'
        real=str(round(self.real,2))
        imaginary=str(round(self.imaginary,2))+'i'

        if self.imaginary<=0:
            sign=''
        if self.imaginary==0:
            imaginary=''
        if self.real==0:
            real=''
        if self.real==0 and self.imaginary==0:
            return '0'
        return real+sign+imaginary

    def __eq__(self,other):
        if type(other)!=Complex:
            other = Complex(other,0)
        return (self.real==other.real and self.imaginary==other.imaginary)

    def conj(self):
        newComplex = Complex(self.real,-self.imaginary)
        return newComplex
        
    def __add__(self, other):
        if type(other)!=Complex:
            other = Complex(other,0)
        newReal = self.real+other.real
        newImaginary = self.imaginary+other.imaginary
        newComplex = Complex(newReal, newImaginary)
        return newComplex

    def __radd__(self,other):
        return self+other

    def __sub__(self, other):
        if type(other)!=Complex:
            other = Complex(other,0)
        newReal = self.real-other.real
        newImaginary = self.imaginary-other.imaginary
        newComplex = Complex(newReal, newImaginary)
        return newComplex

    def __mul__(self, other):
        if type(other)!=Complex:
            return Complex(self.real*other, self.imaginary*other)
        newReal = self.real*other.real - self.imaginary*other.imaginary
        newImaginary = self.real*other.imaginary + self.imaginary*other.real
        newComplex = Complex(newReal, newImaginary)
        return newComplex

    def __rmul__(self, other):
        return self*other
    
    def reciprocal(self):
        newReal = self.real/(self.real**2+self.imaginary**2)
        newImaginary =  -self.imaginary/(self.real**2+self.imaginary**2)
        newComplex = Complex(newReal, newImaginary)
        return newComplex

    def __truediv__(self, other):
        if type(other)!=Complex:
            other = Complex(other,0)
        return self*other.reciprocal()

    def __abs__(self):
        return (self.real**2+self.imaginary**2)**(1/2)

    def arg(self):
        if(self.imaginary<0):
            return -arccos(self.real/abs(self))
        return arccos(self.real/abs(self))

    def __pow__(self, n):
        if self==Complex(0,0) and n>0:
            return 0
        r = abs(self)
        phi = self.arg()
        return(r**n * (Complex(cos(n*phi), sin(n*phi))))

# Def math functions
def factorial(n):
    prod=1
    for i in range(1,n+1):
        prod=prod*i
    return prod

def cos(x):
    maxN=20
    maxError=0.0000000001
    sum=1
    for i in range(1,maxN+1):
        sum = sum + (-1)**i * (x**(2*i))/factorial(2*i)
        if (abs(sum + (-1)**i * (x**(2*i))/factorial(2*i)- sum) < maxError):
            break
    return sum

def sin(x):
    maxN=20
    maxError=0.0000000001
    sum=x
    for i in range(1,maxN):
        sum = sum + (-1)**i * (x**(2*i+1))/factorial(2*i+1)
        if (abs(sum + (-1)**i * (x**(2*i+1))/factorial(2*i+1) - sum) < maxError):
            break
    return sum

def arccos(x):
    if x < -1 or x > 1:
        raise ValueError("Input must be in the range [-1, 1]")
    #Compute arrcos using the Newton-Raphson method

    maxN=20
    guess = 1.0
    maxError = 0.0000000001

    for i in range(maxN):
        f = cos(guess)-x
        df = -sin(guess)
        guess = guess - f/df
        
        if abs(f)<maxError:
            return guess
    return guess

def lin_int(init, end, n):
    if n<2:
        raise ValueError("Number of points (n) must be 2 or greater")
    
    step = (end - init) / (n - 1)
    values = [init + i * step for i in range(n)]

    return values

#Can change the equation here:
def computeNext(z,c):
    # Mandelbrot: z**2+c
    # Tricorn: z.conj**2+c
    # Burning ship: (abs(z.real) + Complex(0,1)*abs(z.imaginary))**2+c
    return z**2+c

def computeN(z,c,n):
    for i in range(n):
        z = computeNext(z,c)
    return z

def checkIfInsideSet(z,c,n,r):
    #n: max number of time we calculate the equation
    #r: if the calculation is over the value of r, then the point is not inside the set
    for i in range(n):
        z = computeNext(z,c)
        if(abs(z)>r):
            return False
    return True

def generateMatrix(n,m):
    matrix=[]
    for i in range(n):
        col=[]
        for j in range(m):
            col.append(0)
        matrix.append(col)
    return matrix

def generateMatrixValues(n, m, minValueReal, maxValueReal, minValueImaginary, maxValueImaginary):
    realValues = lin_int(minValueReal,maxValueReal,m)
    imaginaryValues = lin_int(minValueImaginary, maxValueImaginary, n)
    matrix=[]
    for i in imaginaryValues:
        val=[]
        for r in realValues:
            val.append(Complex(r,i))
        matrix.append(val)
    return list(reversed(matrix))

def checkMatrixInsideSet(matrix, z, n, r):
    newMatrix=matrix.copy()
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            if (checkIfInsideSet(z,matrix[i][j],n,r) == True):
                newMatrix[i][j]=1
            else:
                newMatrix[i][j]=0
    return newMatrix
                
def checkMatrixInsideJuliaSet(matrix, c, n, r):
    newMatrix=matrix.copy()
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            if (checkIfInsideSet(matrix[i][j],c,n,r) == True):
                newMatrix[i][j]=1
            else:
                newMatrix[i][j]=0
    return newMatrix

def printMatrix(A):
    print('\n'.join([''.join(['{:4}'.format(item) for item in row]) 
      for row in A]))

def matrixToHTML(file,matrix, backgroundColor="white"):
    top="0"
    intTop=int(top)
    bottom="0"
    intBottom=int(bottom)
    left="0"
    intLeft=int(left)
    right="0"
    intRight=int(right)

    space=10
    pixelSize=str(5)
    insideColor="black"
    outsideColor="white"

    #Generate the beginning of the html file
    fp=open(file,"w")
    fp.write("<!DOCTYPE html>\n")
    fp.write("<html>\n")
    fp.write("<head>\n")
    fp.write("<title>"+"Fractal"+"</title>\n")
    fp.write("</head>\n")
    fp.write('<body style="background-color:'+backgroundColor+';">\n')  

    #Draws the matrix
    for i in range(len(matrix)):
        left="0"
        intLeft=int(left)
        intTop=intTop+space
        top=str(intTop)
        for j in range(len(matrix[i])):
            if matrix[i][j]==1:
                fp.write('<div style="position:absolute; top:'+top+'px; bottom:'+bottom+'px; left:'+left+'px; right:'+right+'px; margin:auto; width:'+pixelSize+'px; height:'+pixelSize+'px; background-color:'+insideColor+';"></div>\n')
                                         
            intLeft=intLeft+space
            left=str(intLeft)

    #Generate the ending of the html file
    fp.write("</body>\n")
    fp.write("</html>\n")
    fp.close()

z=Complex(0,0)

# Calculate n and m so that the fractal is not squashed
minReal = -2
maxReal = 1
minIm = -1
maxIm = 1
n=60

m=round( n*((abs(minReal)+abs(maxReal)+1) / (abs(minIm)+abs(maxIm)+1)) )

matrix = generateMatrixValues(n, m, minReal, maxReal, minIm, maxIm)

fractalMatrix = checkMatrixInsideSet(matrix, z, 20, 2)
matrixToHTML("fractal.html",fractalMatrix)

#juliaMatrix = checkMatrixInsideJuliaSet(matrix, Complex(-1,0), 20, 2)
#matrixToHTML("fractal.html",juliaMatrix)
