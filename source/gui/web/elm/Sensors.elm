module Sensors exposing (viewSensors, SensorData)

import Visualization.Scale as Scale exposing (ContinuousScale, ContinuousTimeScale)
import Visualization.Axis as Axis
import Visualization.List as VisList
import Visualization.Shape as Shape
import Svg exposing (..)
import Svg.Attributes exposing (..)
import Html exposing (Html)
import String
import Debug


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


w : Float
w =
    200


h : Float
h =
    200


padding : Float
padding =
    30


type alias SensorData =
    { irDown : Float
    , irFl : Float
    , irFr : Float
    , irBl : Float
    , irBr : Float
    , lidar : Float
    , angleL : Float
    , angleR : Float
    , angleAvg : Float
    }


sensorNames : List ( SensorData -> Float, String )
sensorNames =
    [ ( .irDown, "IR Down" )
    , ( .irFl, "IR Front left" )
    , ( .irFr, "IR Front right" )
    , ( .irBl, "IR Back left" )
    , ( .irBr, "IR Back right" )
    , ( .lidar, "LIDAR" )
    , ( .angleL, "Angle left" )
    , ( .angleR, "Angle right" )
    , ( .angleAvg, "Average angle" )
    ]


sensorMessagesPerSecond : Float
sensorMessagesPerSecond =
    10.0


timestamps : List Float
timestamps =
    List.range 0 (floor sensorMessagesPerSecond * 5)
        |> List.map (\x -> toFloat x * -1)


viewSensors : List SensorData -> Html msg
viewSensors sensors =
    let
        getSensorData ( accessorFun, name ) =
            List.map accessorFun sensors
                |> List.map2 (,) timestamps
                |> viewSensor name
    in
        List.map getSensorData sensorNames
            |> Html.div []


viewSensor : String -> List ( Float, Float ) -> Svg msg
viewSensor name data =
    let
        xScale : ContinuousScale
        xScale =
            Scale.linear ( -5, 0 ) ( 0, w - 2 * padding )

        yScale : ContinuousScale
        yScale =
            Scale.linear ( 0, 5 ) ( h - 2 * padding, 0 )

        opts : Axis.Options a
        opts =
            Axis.defaultOptions

        xAxis : Svg msg
        xAxis =
            Axis.axis { opts | orientation = Axis.Bottom, tickCount = 5 }
                xScale

        yAxis : Svg msg
        yAxis =
            Axis.axis { opts | orientation = Axis.Left, tickCount = 5 } yScale

        areaGenerator : ( Float, Float ) -> Maybe ( ( Float, Float ), ( Float, Float ) )
        areaGenerator ( x, y ) =
            Just
                ( ( Scale.convert xScale x, Tuple.first (Scale.rangeExtent yScale) )
                , ( Scale.convert xScale x, Scale.convert yScale y )
                )

        lineGenerator : ( Float, Float ) -> Maybe ( Float, Float )
        lineGenerator ( x, y ) =
            Just ( Scale.convert xScale x, Scale.convert yScale y )

        area : String
        area =
            List.map areaGenerator data
                |> Shape.area Shape.monotoneInXCurve

        line : String
        line =
            List.map lineGenerator data
                |> Shape.line Shape.monotoneInXCurve
    in
        svg [ width (toString w ++ "px"), height (toString h ++ "px") ]
            [ g [ transform ("translate(" ++ toString (padding - 1) ++ ", " ++ toString (h - padding) ++ ")") ]
                [ xAxis ]
            , g [ transform ("translate(" ++ toString (padding - 1) ++ ", " ++ toString padding ++ ")") ]
                [ yAxis ]
            , g [ transform ("translate(" ++ toString padding ++ ", " ++ toString padding ++ ")"), class "series" ]
                [ Svg.path [ d area, stroke "none", strokeWidth "3px", fill "rgba(255, 0, 0, 0.54)" ] []
                , Svg.path [ d line, stroke "red", strokeWidth "3px", fill "none" ] []
                ]
            , text_ [ x "50", y "35" ] [ text name ]
            ]
