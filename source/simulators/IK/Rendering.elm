module Rendering exposing (renderAll, canvasWidth, canvasHeight)

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
import Math.Vector2 as Vector2 exposing (..)
import Math.Matrix4 exposing (..)


type alias Vertex =
    { position : Vec2, color : Vec3 }


canvasWidth : Int
canvasWidth =
    640


canvasHeight : Int
canvasHeight =
    480


ortho : Mat4
ortho =
    makeOrtho2D 0 (toFloat canvasWidth / 2) 0 (toFloat canvasHeight / 2)


viewLegs : Drawable Vertex
viewLegs =
    Triangle []


renderAll : List Renderable
renderAll =
    [ render vertexShader fragmentShader viewLegs { ortho = ortho }
    ]



-- Shaders


vertexShader :
    Shader
        { attr
            | position : Vec2
            , color : Vec3
        }
        { unif | ortho : Mat4 }
        { vcolor : Vec3 }
vertexShader =
    [glsl|

attribute vec2 position;
attribute vec3 color;
uniform mat4 ortho;
varying vec3 vcolor;

void main () {
    gl_Position = ortho * vec4(position, 0.0, 1.0);
    vcolor = color;
}

|]


fragmentShader : Shader {} u { vcolor : Vec3 }
fragmentShader =
    [glsl|

precision mediump float;
varying vec3 vcolor;

void main () {
    gl_FragColor = vec4(vcolor, 1.0);
}

|]
