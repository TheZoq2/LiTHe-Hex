--module Rendering exposing (renderAll, canvasWidth, canvasHeight, chainTogether)


module Rendering exposing (..)

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

import WebGL exposing (..)
import Math.Vector3 as Vector3 exposing (..)
import Math.Matrix4 exposing (..)
import List
import Limbs


type alias Vertex =
    { position : Vec3 }


canvasWidth : number
canvasWidth =
    320


canvasHeight : number
canvasHeight =
    240


halfCanvasWidth : Float
halfCanvasWidth =
    (toFloat canvasWidth / 2)


halfCanvasHeight : Float
halfCanvasHeight =
    (toFloat canvasHeight / 2)


{-| The projection matrix to use when rendering.
-}
ortho : Mat4
ortho =
    makeOrtho2D -halfCanvasWidth halfCanvasWidth -halfCanvasHeight halfCanvasHeight


{-| Convert a Joint into a triangle of vertices to display.
-}
legTriangle : Limbs.Joint -> ( Vertex, Vertex, Vertex )
legTriangle { angle, length, pos } =
    let
        angleA =
            angle + pi / 2

        angleB =
            angle - pi / 2

        legW =
            7

        posA =
            vec3 (legW * cos angleB) (legW * sin angleB) 0

        posB =
            vec3 (legW * cos angleA) (legW * sin angleA) 0

        posC =
            vec3 (length * cos angle) (length * sin angle) 0
    in
        ( { position = add posA pos }
        , { position = add posB pos }
        , { position = add posC pos }
        )


{-| Render some joints.
-}
renderLegs : List Limbs.Joint -> List Renderable
renderLegs limbs =
    [ render vertexShader
        fragmentShader
        (Triangle <| List.map legTriangle limbs)
        { ortho = ortho }
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
