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
import Rendering
import Mouse


type alias Model =
    { lastClick : Mouse.Position }


type Msg
    = NoOp
    | MouseDown Mouse.Position


init : ( Model, Cmd Msg )
init =
    ( {lastClick = {x = 131, y = 32}}, Cmd.none )


subscriptions : Model -> Sub Msg
subscriptions model =
    Sub.batch [Mouse.downs MouseDown]


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        NoOp ->
            ( model, Cmd.none )

        MouseDown pos ->
            ( {model | lastClick = pos}, Cmd.none )


view : Model -> Html Msg
view model =
    WebGL.toHtml [ width Rendering.canvasWidth, height Rendering.canvasHeight ]
        (Rendering.renderAll model.lastClick)


main : Program Never
main =
    App.program
        { init = init
        , subscriptions = subscriptions
        , update = update
        , view = view
        }
