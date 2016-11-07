module Main exposing (..)

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

import Html exposing (Html, div, button)
import Html.App as App
import Html.Attributes exposing (width, height, style)
import WebGL
import Math.Vector3 as Vector3 exposing (..)
import Mouse
import Rendering
import Limbs
import Debug


type alias Model =
    { lastClick : Vec3
    , legs : Hexapod
    , selectedLeg : LegPosition
    }


type alias Hexapod =
    { frontRight : Leg
    , middleRight : Leg
    , backRight : Leg
    , frontLeft : Leg
    , middleLeft : Leg
    , backLeft : Leg
    }


type alias Leg =
    { pos : Vec3
    , a1 : Float
    , a2 : Float
    , a3 : Float
    }


type LegPosition
    = FrontRight
    | MiddleRight
    | BackRight
    | FrontLeft
    | MiddleLeft
    | BackLeft


type JointPosition
    = Hip
    | Knee
    | Foot


maxAngle : Float
maxAngle =
    pi / 2


minAngle : Float
minAngle =
    -pi / 2


type Msg
    = NoOp
    | MouseDown Mouse.Position


initialLegs : Hexapod
initialLegs =
    { frontRight = { pos = vec3 0 0 0, a1 = 0, a2 = 0, a3 = 0 }
    , middleRight = { pos = vec3 0 0 0, a1 = 0, a2 = 0, a3 = 0 }
    , backRight = { pos = vec3 0 0 0, a1 = 0, a2 = 0, a3 = 0 }
    , frontLeft = { pos = vec3 0 0 0, a1 = 0, a2 = 0, a3 = 0 }
    , middleLeft = { pos = vec3 0 0 0, a1 = 0, a2 = 0, a3 = 0 }
    , backLeft = { pos = vec3 0 0 0, a1 = 0, a2 = 0, a3 = 0 }
    }


init : ( Model, Cmd Msg )
init =
    ( { lastClick = vec3 131 32 0
      , legs = initialLegs
      , selectedLeg = FrontRight
      }
    , Cmd.none
    )


subscriptions : Model -> Sub Msg
subscriptions model =
    Sub.batch
        [ Mouse.downs MouseDown
        ]


setLegJoint : Float -> JointPosition -> Leg -> Leg
setLegJoint angle joint leg =
    case joint of
        Hip ->
            { leg | a1 = angle }

        Knee ->
            { leg | a2 = angle }

        Foot ->
            { leg | a3 = angle }


setJointAngle : Float -> LegPosition -> JointPosition -> Hexapod -> Hexapod
setJointAngle angle leg joint bot =
    case leg of
        FrontLeft ->
            { bot | frontLeft = setLegJoint angle joint bot.frontLeft }

        MiddleLeft ->
            { bot | middleLeft = setLegJoint angle joint bot.middleLeft }

        BackLeft ->
            { bot | backLeft = setLegJoint angle joint bot.backLeft }

        FrontRight ->
            { bot | frontRight = setLegJoint angle joint bot.frontRight }

        MiddleRight ->
            { bot | middleRight = setLegJoint angle joint bot.middleRight }

        BackRight ->
            { bot | backRight = setLegJoint angle joint bot.backRight }


calcLegIK : LegPosition -> Hexapod -> Vec3 -> Hexapod
calcLegIK leg bot pos =
    let
        clickAngle =
            atan2 -(getZ pos) (getX pos)

        horizontalAngle =
            clickAngle |> max minAngle |> min maxAngle

        kneePos =
            vec3 (thighLength * cos horizontalAngle)
                0
                -(thighLength * sin horizontalAngle)

        kneeToeDistance =
            min (distance kneePos pos) (ankleLength + footlength)

        kneeToeVec =
            scale kneeToeDistance (direction kneePos pos)

        alpha =
            acos ((getY kneeToeVec) / kneeToeDistance)
                + acos
                    ((footlength ^ 2 - ankleLength ^ 2 - kneeToeDistance ^ 2)
                        / (-2 * ankleLength * kneeToeDistance)
                    )
                - pi / 2

        beta =
            acos ((kneeToeDistance^2 - footlength^2 - ankleLength^2) /
                      (-2 * footlength * ankleLength)) - pi
    in
        bot
            |> setJointAngle horizontalAngle leg Hip
            |> setJointAngle alpha leg Knee
            |> setJointAngle beta leg Foot


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        NoOp ->
            ( model, Cmd.none )

        MouseDown pos ->
            let
                offsetPos =
                    if pos.x < Rendering.canvasWidth then
                        model.lastClick
                            |> setX (toFloat pos.x - Rendering.halfCanvasWidth)
                            |> setZ (toFloat pos.y - Rendering.halfCanvasHeight)
                    else
                        model.lastClick
                            |> setX (cos model.legs.frontRight.a1 *
                                         (toFloat pos.x -
                                              3 * Rendering.halfCanvasWidth))
                            |> setZ -(sin model.legs.frontRight.a1 *
                                         (toFloat pos.x -
                                              3 * Rendering.halfCanvasWidth))
                            |> setY -(toFloat pos.y - Rendering.halfCanvasHeight)
            in
                ( { model
                    | lastClick = offsetPos
                    , legs = calcLegIK model.selectedLeg model.legs offsetPos
                  }
                , Cmd.none
                )


{-| Render a list of joints to its own canvas.
-}
limbView : List Limbs.Joint -> Html Msg
limbView limbs =
    div
        [ style
            [ ( "width", toString Rendering.canvasWidth )
            , ( "float", "left" )
            , ( "border", "1px solid black" )
            ]
        ]
        [ WebGL.toHtml
            [ width Rendering.canvasWidth
            , height Rendering.canvasHeight
            ]
            (Rendering.renderLegs limbs)
        ]


thighLength : number
thighLength =
    50


ankleLength : number
ankleLength =
    30


footlength : number
footlength =
    40


projectTop : Hexapod -> List Limbs.Joint
projectTop bot =
    let
        leg =
            bot.frontRight
    in
        Limbs.chainTogether
            [ { angle = leg.a1, length = thighLength }
            , { angle = 0, length = ankleLength * cos leg.a2 }
            , { angle = 0, length = footlength * cos (leg.a2 + leg.a3) }
            ]


projectSide : Hexapod -> List Limbs.Joint
projectSide bot =
    let
        leg =
            bot.frontRight
    in
        Limbs.chainTogether
            [ { angle = 0, length = thighLength }
            , { angle = leg.a2, length = ankleLength }
            , { angle = leg.a3, length = footlength }
            ]


view : Model -> Html Msg
view model =
    div []
        [ limbView (projectTop model.legs)
        , limbView (projectSide model.legs)
        , div [ style [ ("float", "left") ] ]
            [ Html.text
                <| "Den vänsta vyn är uppifrån roboten och den högra är "
                ++ "från sidan av benet. Klicka för att flytta benet"
            ]
        ]


main : Program Never
main =
    App.program
        { init = init
        , subscriptions = subscriptions
        , update = update
        , view = view
        }
