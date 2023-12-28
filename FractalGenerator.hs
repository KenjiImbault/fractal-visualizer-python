data Complex = Complex { realPart :: Double, imgPart :: Double }

instance Eq Complex where
    (==) :: Complex -> Complex -> Bool
    (Complex x y) == (Complex u v) = x==u && y==v

instance Num Complex where
    (+) :: Complex -> Complex -> Complex
    (Complex x y) + (Complex u v) = Complex (x+u) (y+v)

    negate :: Complex -> Complex
    negate (Complex x y) = Complex (-x) (-y)

    (*) :: Complex -> Complex -> Complex
    (Complex x y) * (Complex u v) = Complex (x*u-y*v) (x*v+y*u)

    abs :: Complex -> Complex
    abs (Complex x y) = Complex (sqrt (x**2 + y**2)) 0

    signum :: Complex -> Complex
    signum z@(Complex x y) = z/abs z

    fromInteger :: Integer -> Complex
    fromInteger n = Complex (fromInteger n) 0

complexAbs :: Complex -> Double
complexAbs (Complex x y) = sqrt (x**2 + y**2)

instance Fractional Complex where
    (/) :: Complex -> Complex -> Complex
    (Complex u v) / (Complex x y) = Complex ((u*x+v*y)/(x**2+y**2)) ((v*x-u*y)/(x**2+y**2))

    fromRational :: Rational -> Complex
    fromRational r = Complex (fromRational r) 0

instance Floating Complex where
    pi :: Complex
    pi = Complex pi 0

    exp :: Complex -> Complex
    exp (Complex x y) = Complex (exp x * cos y) (exp x * sin y)

    log :: Complex -> Complex
    log z@(Complex x y) = Complex (log $ complexAbs z) (arg z)

    sin :: Complex -> Complex
    sin (Complex x y) = Complex (sin x * cosh y) (cos x * sinh y)

    cos :: Complex -> Complex
    cos (Complex x y) = Complex (cos x * cosh y) (- sin x * sinh y)

    (**) :: Complex -> Complex -> Complex
    z ** w
        | z == 0 && realPart w > 0 = 0
        | otherwise = exp (log z * w)

    acos :: Complex -> Complex
    acos z@(Complex x y) = (1/2) * (pi-2*asin z)

    asin :: Complex -> Complex
    asin z@(Complex x y) = (-i) * log (i*z + (1-z**2)**(1/2)) where i=Complex 0 1

    atan :: Complex -> Complex
    atan z@(Complex x y) = (1/2) * log (1-i*z) - (1/2) * log (1+i*z) where i = Complex 0 1

    sinh :: Complex -> Complex
    sinh z@(Complex x y) = (1/2) * (exp z - exp (-z))

    cosh :: Complex -> Complex
    cosh z@(Complex x y) = (1/2) * (exp z + exp (-z))

    acosh :: Complex -> Complex
    acosh z@(Complex x y) = log ((z-1)**(1/2) * (z+1)**(1/2) + z)

    asinh :: Complex -> Complex
    asinh z@(Complex x y) = log ((1+z**2)**(1/2) + z)

    atanh :: Complex -> Complex
    atanh z@(Complex x y) = (1/2) * log (z+1) - (1/2) * log (-z+1)


instance Show Complex where
    show :: Complex -> String
    show (Complex a b)
        | a==0 && b==0 = "0"
        | b==0 = show a
        | a==0 = show b ++ "i"
        | b<0 = show a ++ " - " ++ show (-b) ++ "i"
        |otherwise = show a ++ " + " ++ show b ++"i"

complex2Double :: Complex -> Double
complex2Double (Complex x y) = x

makeComplex :: Double -> Double -> Complex
makeComplex real img = Complex {realPart=real, imgPart=img}

reciprocalComplex :: Complex -> Complex
reciprocalComplex (Complex x y) = Complex (x/(x**2+y**2)) (-y/(x**2+y**2))

arg :: Complex -> Double
arg z@(Complex x y) = if y<0 then -acos (x/complexAbs z) else acos (x/complexAbs z)


--Can change the equation here:
computeNext :: Complex -> Complex -> Complex
computeNext z c = z**2 + c

computeN :: Complex -> Complex -> Int -> Complex
computeN z c 0 = z
computeN z c n = computeN (computeNext z c) c (n-1)

checkIfInsideSet :: Complex -> Complex -> Int -> Double -> Bool
checkIfInsideSet z c n r = complexAbs (computeN z c n) <= r

computeNJulia :: Complex -> Complex -> Int -> Complex
computeNJulia z c 0 = z
computeNJulia z c n = computeN z (computeNext z c) (n-1)

checkIfInsideJuliaSet :: Complex -> Complex -> Int -> Double -> Bool
checkIfInsideJuliaSet z c n r = complexAbs (computeNJulia z c n) <= r

generateMatrixValue :: Int -> Int -> Double -> Double -> Double -> Double -> [[Complex]]
generateMatrixValue n m minReal maxReal minIm maxIm =
  [ [ complexValue r c | c <- [0 .. m - 1] ] | r <- [0 .. n - 1] ]
  where
    complexValue r c =
      let realPartValue = minReal + fromIntegral c * (maxReal - minReal) / fromIntegral (m - 1)
          imgPartValue = minIm + fromIntegral (n - 1 - r) * (maxIm - minIm) / fromIntegral (n - 1)
      in Complex { realPart = realPartValue, imgPart = imgPartValue }

checkMatrixInsideSet :: [[Complex]] -> Complex -> Int -> Double -> [[Bool]]
checkMatrixInsideSet matrix z n r =
  [ [ checkIfInsideSet z (matrix !! i !! j) n r | j <- [0 .. length (matrix !! i) - 1] ] | i <- [0 .. length matrix - 1] ]

checkMatrixInsideJuliaSet :: [[Complex]] -> Complex -> Int -> Double -> [[Bool]]
checkMatrixInsideJuliaSet matrix c n r =
  [ [ checkIfInsideJuliaSet (matrix !! i !! j) c  n r | j <- [0 .. length (matrix !! i) - 1] ] | i <- [0 .. length matrix - 1] ]

matrixToHTML :: FilePath -> [[Bool]] -> String -> IO ()
matrixToHTML file matrix backgroundColor = do
  let top = 0
      bottom = 0
      left = 0
      right = 0
      space = 10
      pixelSize = 5
      insideColor = "black"
      outsideColor = "white"

  -- Generate the beginning of the HTML file
  writeFile file $ unlines
    [ "<!DOCTYPE html>"
    , "<html>"
    , "<head>"
    , "<title>Fractal</title>"
    , "</head>"
    , "<body style=\"background-color:" ++ backgroundColor ++ ";\">"
    ]

  -- Draws the matrix
  let drawPixel i j val = do
        let intLeft = left + j * space
            intTop = top + i * space
        appendFile file $ unlines $
          [ "<div style=\"position:absolute; top:" ++ show intTop ++ "px; bottom:" ++ show bottom ++ "px; left:" ++ show intLeft ++ "px; right:" ++ show right ++ "px; margin:auto; width:" ++ show pixelSize ++ "px; height:" ++ show pixelSize ++ "px; background-color:" ++ insideColor ++ ";\"></div>"
          | val
          ]

  let drawRow i row = mapM_ (uncurry (drawPixel i)) (zip [0..] row)

  let drawMatrix [] = return ()
      drawMatrix (row:rest) = do
        drawRow (length matrix - length rest) row
        drawMatrix rest

  drawMatrix matrix

  -- Generate the ending of the HTML file
  appendFile file $ unlines
    [ "</body>"
    , "</html>"
    ]

calculateM :: Int -> Int -> Int -> Int -> Int -> Int
calculateM n minReal maxReal minIm maxIm =
  round $ fromIntegral n * ((fromIntegral (abs minReal) + fromIntegral (abs maxReal) + 1) / (fromIntegral (abs minIm) + fromIntegral (abs maxIm) + 1))


main :: IO ()

main = do
    let z = makeComplex 0 0

    let minReal :: Int
        minReal = -2

    let maxReal :: Int
        maxReal = 1

    let minIm :: Int
        minIm = -1

    let maxIm :: Int
        maxIm = 1

    let n :: Int
        n = 60

    let m :: Int
        m = calculateM n minReal maxReal minIm maxIm

    let minRealDouble :: Double
        minRealDouble = fromIntegral minReal

    let maxRealDouble :: Double
        maxRealDouble = fromIntegral maxReal

    let minImDouble :: Double
        minImDouble = fromIntegral minIm

    let maxImDouble :: Double
        maxImDouble = fromIntegral maxIm

    let matrix = generateMatrixValue n m minRealDouble maxRealDouble minImDouble maxImDouble
    let fractalMatrix = checkMatrixInsideSet matrix z 20 2
    matrixToHTML "fractalHaskell.html" fractalMatrix "white"

    {-
    let c :: Complex
        c = Complex (-1) 0
    let juliaMatrix = checkMatrixInsideJuliaSet matrix c 20 2
    matrixToHTML "juliaHaskell.html" juliaMatrix "white"
    -}