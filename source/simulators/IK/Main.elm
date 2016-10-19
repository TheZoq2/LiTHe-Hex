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

import Html exposing (Html)
import Html.App as App
import Html.Attributes exposing (width, height)
import WebGL
import Math.Vector3 as Vector3 exposing (..)
import Rendering
import Mouse
import AnimationFrame
import Time exposing (Time)


type alias Model =
    { lastClick : Vec3, animating : Bool, limbs : List ( Float, Float ), iterations : Int }


type Msg
    = NoOp
    | MouseDown Mouse.Position
    | Tick Time


init : ( Model, Cmd Msg )
init =
    ( { lastClick = vec3 131 32 0
      , animating = False
      , limbs =
            [ ( 0, 50 ), ( pi / 4, 100 ), ( -2 * pi / 3, 40 ) ]
      , iterations = 0
      }
    , Cmd.none
    )


subscriptions : Model -> Sub Msg
subscriptions model =
    Sub.batch
        [ Mouse.downs MouseDown
        , AnimationFrame.diffs Tick
        ]


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        NoOp ->
            ( model, Cmd.none )

        MouseDown pos ->
            let
                offsetPos =
                    (vec3 (toFloat pos.x - Rendering.halfCanvasWidth) -(toFloat pos.y - Rendering.halfCanvasHeight) 0)

                _ =
                    Debug.log "click at " offsetPos

                _ =
                    Debug.log "arm at " (Rendering.calcEndPos model.limbs)
            in
                ( { model | lastClick = offsetPos, animating = True, iterations = 0 }, Cmd.none )

        Tick newTime ->
            let
                currentEnd =
                    Rendering.calcEndPos model.limbs

                toEnd =
                    sub model.lastClick currentEnd
            in
                if model.animating && length toEnd > 10 then
                    ( { model
                        | limbs = Rendering.calculateIK model.lastClick model.limbs
                        , iterations = model.iterations + 1
                      }
                    , Cmd.none
                    )
                else
                    let
                        _ =
                            if model.animating then
                                Debug.log "Found click position in " model.iterations
                            else
                                model.iterations
                    in
                        ( { model | animating = False }, Cmd.none )


view : Model -> Html Msg
view model =
    WebGL.toHtml [ width Rendering.canvasWidth, height Rendering.canvasHeight ]
        (Rendering.renderAll model.limbs)


main : Program Never
main =
    App.program
        { init = init
        , subscriptions = subscriptions
        , update = update
        , view = view
        }
