port module Joystick exposing (joystickDisplay, connected, disconnected, poll, axisData, getElementPosition, elementPosition, JoystickData)

import Svg exposing (..)
import Svg.Attributes exposing (..)
import Html exposing (Html)
import Html.Attributes
import Html.Events


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


type alias JoystickData =
    { x : Float
    , y : Float
    , rotation : Float
    , thrust : Float
    , reset : Bool
    }


{-| Event that will fire when a new joystick is connected. The included data is
the index of the new joystick.
-}
port connected : (Int -> msg) -> Sub msg


{-| Event that will fire when a joystick is disconnected. The included data is
the index of the new joystick.
-}
port disconnected : (Int -> msg) -> Sub msg


{-| Port to send a joystick index to when you want to know the current state of
that joystick.
-}
port poll : Int -> Cmd msg


{-| Event that will send state for a particular joystick every time that its
index is sent to poll.
-}
port axisData : (JoystickData -> msg) -> Sub msg


{-| Port to send to when you want the position of an element
-}
port getElementPosition : String -> Cmd msg


port elementPosition : ({x : Int, y : Int} -> msg) -> Sub msg


thrustRect : Float -> Svg a
thrustRect thrust =
    rect
        [ fill "#B0B0FF"
        , x "0"
        , y <| toString <| round <| 100 - thrust * 100
        , width "100"
        , height <| toString <| round <| thrust * 100
        ]
        []


steeringRect : Float -> Svg a
steeringRect angle =
    rect
        [ fill "#000000"
        , x "48"
        , y "0"
        , width "4"
        , height "100"
        , transform <| "rotate(" ++ (toString <| round <| angle * 90) ++ ", 50, 50)"
        ]
        []


{-| Make a visualization of the joystick state.
-}
joystickDisplay : JoystickData -> Html a
joystickDisplay data =
    let
        middle x =
            50 + 50 * x |> floor |> toString
    in
        Html.div [ Html.Attributes.style [ ( "width", "400px" ), ( "height", "400px" ) ] ]
            [ svg [ version "1.1", x "0", y "0", viewBox "0 0 100 100" ]
                [ thrustRect data.thrust
                , circle [ fill "#F0F9F0", cx "50", cy "50", r "50" ] []
                , steeringRect data.rotation
                , circle
                    [ fill "#FFB0B0"
                    , cx (middle data.x)
                    , cy (middle data.y)
                    , r "5"
                    ]
                    []
                ]
            ]
