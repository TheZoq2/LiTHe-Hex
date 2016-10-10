module Rendering exposing (renderAll, canvasWidth, canvasHeight, chainTogether)

{-
   Copyright 2016 Noak Ringman, Emil Segerbäck, Robin Sliwa, Frans Skarman, Hannes Tuhkala, Malcolm Wigren, Olav Övrebö

   This file is part of LiTHe Hex.

   LiTHe Hex is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   LiTHe Hex is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with LiTHe Hex.  If not, see <http://www.gnu.org/licenses/>.
-}

import Debug
import WebGL exposing (..)
import Math.Vector3 as Vector3 exposing (..)
import Math.Matrix4 exposing (..)
import Matrix exposing (..)
import List
import List.Extra exposing (transpose)


type alias Vertex =
    { position : Vec3 }


canvasWidth : Int
canvasWidth =
    640


canvasHeight : Int
canvasHeight =
    480


halfCanvasWidth : Float
halfCanvasWidth =
    (toFloat canvasWidth / 2)


halfCanvasHeight : Float
halfCanvasHeight =
    (toFloat canvasHeight / 2)


ortho : Mat4
ortho =
    makeOrtho2D -halfCanvasWidth halfCanvasWidth -halfCanvasHeight halfCanvasHeight


legTriangle : ( Float, Float, Vec3 ) -> List ( Vertex, Vertex, Vertex )
legTriangle ( angle, length, pos ) =
    let
        angleA =
            angle + pi / 2

        angleB =
            angle - pi / 2

        posA =
            vec3 (20 * cos angleB) (20 * sin angleB) 0

        posB =
            vec3 (20 * cos angleA) (20 * sin angleA) 0

        posC =
            vec3 (length * cos angle) (length * sin angle) 0
    in
        [ ( { position = add posA pos }
          , { position = add posB pos }
          , { position = add posC pos }
          )
        ]


appendNewToChain :
    ( Float, Float )
    -> List ( Float, Float, Vec3 )
    -> List ( Float, Float, Vec3 )
appendNewToChain ( angle, length ) oldList =
    let
        ( prevAngle, prevLength, prevPos ) =
            case oldList of
                [] ->
                    ( 0, 0, vec3 0 0 0 )

                prev :: _ ->
                    prev

        pos =
            vec3 (prevLength * cos prevAngle) (prevLength * sin prevAngle) 0
    in
        ( angle + prevAngle, length, add pos prevPos ) :: oldList


chainTogether : List ( Float, Float ) -> List ( Float, Float, Vec3 )
chainTogether =
    List.foldl appendNewToChain []


calculateJacobianCol : Vec3 -> Vec3 -> List Float
calculateJacobianCol endPos position =
    let
        crossProd =
            cross (vec3 0 0 1) (sub endPos position)
    in
        [ getX crossProd, getY crossProd, getZ crossProd ]


third : ( a, b, c ) -> c
third ( _, _, x ) =
    x


calculateJacobian : Vec3 -> List ( Float, Float, Vec3 ) -> Matrix
calculateJacobian endPos =
    List.map (third >> calculateJacobianCol endPos)


addAngleLength : (Float, Float) -> (Vec3, Float) -> (Vec3, Float)
addAngleLength (angle, length) (pos, oldAngle) =
    let
        newAngle = angle + oldAngle
    in
        (add pos <| vec3 (length * cos newAngle) (length * sin newAngle) 0, newAngle)


calcEndPos : List (Float, Float) -> Vec3
calcEndPos anglesAndLengths =
    let
        (vec, _) = List.foldl addAngleLength (vec3 0 0 0, 0) anglesAndLengths
    in
        vec


addAngle : Float -> (Float, Float) -> (Float, Float)
addAngle delta (old, length) =
    (delta + old, length)


addAngles : Vec3 -> List (Float, Float) -> List ( Float, Float )
addAngles vec list =
    let
        (x, y, z) = toTuple vec
    in
        List.map2 addAngle [x, y, z] list


calculateIK : Vec3 -> Float -> List ( Float, Float ) -> List ( Float, Float )
calculateIK endPos maxIter current =
    let
        currentEnd = calcEndPos current
        toEnd = sub endPos currentEnd
    in
        if (length toEnd) < 1 || maxIter <= 0 then
            current
        else
            let
                j = calculateJacobian endPos <| chainTogether current
                jPlus = Maybe.withDefault [[1,0,0], [0,1,0], [0,0,1]] <| calcPsuedoInverse j
                deltaTheta = Maybe.withDefault (vec3 0 0 0) <| multMatVec jPlus (Vector3.scale 0.1 toEnd)
            in
                calculateIK endPos (maxIter - 1) (addAngles deltaTheta current)


limbs : List ( Float, Float )
limbs =
    [ ( 0, 50 ), ( pi / 4, 100 ), ( -2 * pi / 3, 40 ) ]


viewLegs : {x: Int, y: Int} -> Drawable Vertex
viewLegs pos =
    let _ = Debug.log "endPos is " pos
    in
        Triangle <| List.concatMap legTriangle <| chainTogether <| calculateIK (vec3 (toFloat pos.x) (toFloat pos.y) 0) 1000 limbs


renderAll : {x : Int, y : Int } -> List Renderable
renderAll lastClick =
    [ render vertexShader fragmentShader (viewLegs lastClick) { ortho = ortho }
    ]



-- Shaders


vertexShader : Shader { attr | position : Vec3 } { unif | ortho : Mat4 } {}
vertexShader =
    [glsl|

attribute vec3 position;
uniform mat4 ortho;

void main () {
    gl_Position = ortho * vec4(position, 1.0);
}

|]


fragmentShader : Shader {} u {}
fragmentShader =
    [glsl|

precision mediump float;

void main () {
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

|]
