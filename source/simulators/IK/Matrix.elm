module Matrix exposing (..)

import Math.Vector3 as Vector3 exposing (..)
import List.Extra


type alias Matrix =
    List (List Float)


multiplyMatrices : Matrix -> Matrix -> Maybe Matrix
multiplyMatrices a b =
    case ( a, b ) of
        ( [ [ a11, a12, a13 ]
          , [ a21, a22, a23 ]
          , [ a31, a32, a33 ]
          ]
        , [ [ b11, b12, b13 ]
          , [ b21, b22, b23 ]
          , [ b31, b32, b33 ]
          ]
        ) ->
            Just [ [ (a11 * b11) + (a12 * b21) + (a13 * b31)
                   , (a11 * b12) + (a12 * b22) + (a13 * b32)
                   , (a11 * b13) + (a12 * b23) + (a13 * b33)
                   ]
                 , [ (a21 * b11) + (a22 * b21) + (a23 * b31)
                   , (a21 * b12) + (a22 * b22) + (a23 * b32)
                   , (a21 * b13) + (a22 * b23) + (a23 * b33)
                   ]
                 , [ (a31 * b11) + (a32 * b21) + (a33 * b31)
                   , (a31 * b12) + (a32 * b22) + (a33 * b32)
                   , (a31 * b13) + (a32 * b23) + (a33 * b33)
                   ]
                 ]

        _ ->
            Nothing

multMatScal : Float -> Matrix -> Matrix
multMatScal scal =
    List.map (List.map (\x -> x * scal))


multMatVec : Matrix -> Vec3 -> Maybe Vec3
multMatVec mat vec =
    let
        (x, y, z) = toTuple vec
    in
        case mat of
            [ [ a, b, c ]
            , [ d, e, f ]
            , [ g, h, i ]
            ] ->
                Just <| vec3 (a*x + b*y + c*z) (d*x + e*y + f*z) (g*x + h*y + i*z)
            _ ->
                Nothing



calcDeterminant : Matrix -> Maybe Float
calcDeterminant mat =
    case mat of
        [ [ a, b, c ]
        , [ d, e, f ]
        , [ g, h, i ]
        ] ->
            Just <| a*e*i + b*f*g + c*d*h - c*e*g - b*d*i - a*f*h

        _ ->
            Nothing


invertMatrix : Matrix -> Maybe Matrix
invertMatrix mat =
    case (mat, calcDeterminant mat) of
        ( [ [ a, b, c ]
          , [ d, e, f ]
          , [ g, h, i ]
        ]
        , Just det
        ) ->
             let
                 iA = e*i - f*h
                 iB = -(d*i - f*g)
                 iC = d*h - e*g
                 iD = -(b*i - c*h)
                 iE = a*i - c*g
                 iF = -(a*h - b*g)
                 iG = b*f - c*e
                 iH = -(a*f - c*d)
                 iI = a*e - b*d
             in
                 if det == 0 then Nothing else
                     [ [iA, iD, iG]
                     , [iB, iE, iH]
                     , [iC, iF, iI]
                     ]
                     |> multMatScal (1/det)
                     |> Just

        _ ->
            Nothing


calcPsuedoInverse : Matrix -> Maybe Matrix
calcPsuedoInverse mat =
    multiplyMatrices mat (List.Extra.transpose mat)
        `Maybe.andThen` invertMatrix
        `Maybe.andThen` ((flip multiplyMatrices) mat)
