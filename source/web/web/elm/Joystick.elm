port module Joystick exposing (joystickDisplay, connected, disconnected, poll, axisData, JoystickData)

import Svg exposing (..)
import Svg.Attributes exposing (..)
import Html exposing (Html)
import Html.Attributes


type alias JoystickData =
    { x : Float
    , y : Float
    , rotation : Float
    , thrust : Float
    }


port connected : (Int -> msg) -> Sub msg


port disconnected : (Int -> msg) -> Sub msg


port poll : Int -> Cmd msg


port axisData : (JoystickData -> msg) -> Sub msg


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
