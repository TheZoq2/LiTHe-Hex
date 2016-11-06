module Limbs exposing (..)

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

import Math.Vector3 as Vector3 exposing (..)


type alias Joint =
    { pos : Vec3, angle : Float, length : Float }


{-| An Appendix is like a joint but it does not have its own position and is
reliant on its parent joint
-}
type alias Appendix =
    { angle : Float, length : Float }


{-| Puts a new appendix to the end of a limb. The input angle is a local
coordinate relative to the previous end joint.
-}
appendAppendix : Appendix -> List Joint -> List Joint
appendAppendix { angle, length } oldList =
    let
        noLimb =
            { angle = 0, length = 0, pos = (vec3 0 0 0) }

        prev =
            oldList |> List.head |> Maybe.withDefault noLimb

        pos =
            vec3 (prev.length * cos prev.angle) (prev.length * sin prev.angle) 0
    in
        { angle = angle + prev.angle
        , length = length
        , pos = add pos prev.pos
        }
            :: oldList


{-| Convert a list of angles and lengths to a limb.
-}
chainTogether : List Appendix -> List Joint
chainTogether =
    List.foldl appendAppendix []



-- TODO: Check if the functions below are really needed


addAngleLength : ( Float, Float ) -> ( Vec3, Float ) -> ( Vec3, Float )
addAngleLength ( angle, length ) ( pos, oldAngle ) =
    let
        newAngle =
            angle + oldAngle
    in
        ( add pos <| vec3 (length * cos newAngle) (length * sin newAngle) 0, newAngle )


calcEndPos : List ( Float, Float ) -> Vec3
calcEndPos anglesAndLengths =
    let
        ( vec, _ ) =
            List.foldl addAngleLength ( vec3 0 0 0, 0 ) anglesAndLengths
    in
        vec


addAngle : Float -> ( Float, Float ) -> ( Float, Float )
addAngle delta ( old, length ) =
    ( delta + old, length )


addAngles : Vec3 -> List ( Float, Float ) -> List ( Float, Float )
addAngles vec list =
    let
        ( x, y, z ) =
            toTuple vec
    in
        List.map2 addAngle [ x, y, z ] list
