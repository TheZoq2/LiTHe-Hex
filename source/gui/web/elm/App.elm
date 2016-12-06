module App exposing (..)

import Html exposing (Html, h1, img, text, div, input, br, form)
import Html.Attributes exposing (style, value, src, placeholder)
import Html.Events exposing (onInput, onSubmit)
import Html.Lazy exposing (lazy)
import Json.Encode as JE
import Json.Decode as JD
import Json.Decode.Pipeline exposing (decode, required)
import Time exposing (Time, millisecond)
import Dict exposing (Dict)
import String
import Debug
import Phoenix.Socket exposing (Socket)
import Phoenix.Channel
import Phoenix.Push
import Material
import Material.Elevation as Elevation
import Material.Color as Color
import Material.Options as Options
import Material.Textfield as Textfield
import Material.List as Lists
import Material.Button as Button
import Material.Card as Card
import Material.Layout as Layout
import Joystick
import Sensors


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


type alias PIDParameter =
    String


type Msg
    = PhoenixMsg (Phoenix.Socket.Msg Msg)
    | SetNewMessage String
    | SendMessage
    | ReceiveChatMessage JE.Value
    | GamepadConnected Int
    | GamepadDisconnected Int
    | AxisData Joystick.JoystickData
    | UpdateControlDisplay Time
    | SendControlToServer Time
    | SelectTab Int
    | ChangeParameter PIDParameter String
    | SendParameters
    | Mdl (Material.Msg Msg)


type alias Model =
    { phxSocket : Socket Msg
    , currentMessage : String
    , messages : List String
    , joystick : Joystick.JoystickData
    , joystickIndex : Maybe Int
    , sensorData : List Sensors.SensorData
    , autoMode : Bool
    , selectedTab : Int
    , parameters : Dict String Float
    , mdl : Material.Model
    }


type BotMessage
    = DebugMessage String
    | AutoMessage Bool
    | SensorMessage Sensors.SensorData


type alias Flags =
    { host : String }


init : Flags -> ( Model, Cmd Msg )
init { host } =
    let
        ( phxSocket, phxCmd ) =
            Phoenix.Socket.init ("ws://" ++ host ++ "/socket/websocket")
                |> Phoenix.Socket.withDebug
                |> Phoenix.Socket.on "new_msg" "client" ReceiveChatMessage
                |> Phoenix.Socket.join (Phoenix.Channel.init "client")
    in
        { phxSocket = phxSocket
        , currentMessage = ""
        , messages = []
        , joystick = { x = 0, y = 0, rotation = 0, thrust = 0 }
        , joystickIndex = Nothing
        , sensorData = []
        , autoMode = False
        , selectedTab = 0
        , parameters = Dict.empty
        , mdl = Material.model
        }
            ! [ Cmd.map PhoenixMsg phxCmd ]


debugMessageDecoder : JD.Decoder BotMessage
debugMessageDecoder =
    JD.map DebugMessage
        (JD.field "debug" JD.string)


autoMessageDecoder : JD.Decoder BotMessage
autoMessageDecoder =
    JD.map AutoMessage
        (JD.field "auto" JD.bool)


sensorMessageDecoder : JD.Decoder BotMessage
sensorMessageDecoder =
    decode (Sensors.SensorData)
        |> required "ir_down" JD.float
        |> required "ir_fl" JD.float
        |> required "ir_fr" JD.float
        |> required "ir_bl" JD.float
        |> required "ir_br" JD.float
        |> required "lidar" JD.float
        |> required "angle_l" JD.float
        |> required "angle_r" JD.float
        |> required "angle_avg" JD.float
        |> JD.map SensorMessage


chatMessageDecoder : JD.Decoder BotMessage
chatMessageDecoder =
    JD.oneOf [ debugMessageDecoder, autoMessageDecoder, sensorMessageDecoder ]


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        PhoenixMsg phxMsg ->
            let
                ( phxSocket, phxCmd ) =
                    Phoenix.Socket.update phxMsg model.phxSocket
            in
                ( { model | phxSocket = phxSocket }
                , Cmd.map PhoenixMsg phxCmd
                )

        Mdl mdlMsg ->
            Material.update mdlMsg model

        ReceiveChatMessage raw ->
            case JD.decodeValue chatMessageDecoder raw of
                Ok (DebugMessage msg) ->
                    ( { model | messages = msg :: model.messages }
                    , Cmd.none
                    )

                Ok (AutoMessage enabled) ->
                    let
                        _ =
                            Debug.log "Auto mode set to " enabled
                    in
                        ( { model | autoMode = enabled }
                        , Cmd.none
                        )

                Ok (SensorMessage sensorData) ->
                    let
                        newData =
                            (sensorData :: model.sensorData)
                                |> List.take (5 * Sensors.sensorMessagesPerSecond)
                    in
                        ( { model | sensorData = newData }
                        , Cmd.none
                        )

                Err error ->
                    ( model, Cmd.none )

        SetNewMessage str ->
            { model | currentMessage = str } ! []

        UpdateControlDisplay _ ->
            case model.joystickIndex of
                Nothing ->
                    ( model, Cmd.none )

                Just index ->
                    ( model, Joystick.poll index )

        SendControlToServer _ ->
            let
                payload =
                    JE.object
                        [ ( "x", JE.float model.joystick.x )
                        , ( "y", JE.float model.joystick.y )
                        , ( "rotation", JE.float model.joystick.rotation )
                        , ( "thrust", JE.float model.joystick.thrust )
                        ]

                push =
                    Phoenix.Push.init "joystick" "client"
                        |> Phoenix.Push.withPayload payload

                ( phxSocket, phxCmd ) =
                    Phoenix.Socket.push push model.phxSocket
            in
                ( { model | phxSocket = phxSocket }
                , Cmd.map PhoenixMsg phxCmd
                )

        GamepadConnected index ->
            ( { model | joystickIndex = Just index }, Cmd.none )

        GamepadDisconnected index ->
            if Just index == model.joystickIndex then
                ( { model | joystickIndex = Nothing }, Cmd.none )
            else
                ( model, Cmd.none )

        AxisData data ->
            ( { model | joystick = data }, Cmd.none )

        SendMessage ->
            let
                payload =
                    JE.object [ ( "body", JE.string model.currentMessage ) ]

                push =
                    Phoenix.Push.init "new_msg" "client"
                        |> Phoenix.Push.withPayload payload

                ( phxSocket, phxCmd ) =
                    Phoenix.Socket.push push model.phxSocket
            in
                ( { model
                    | currentMessage = ""
                    , phxSocket = phxSocket
                  }
                , Cmd.map PhoenixMsg phxCmd
                )

        SelectTab num ->
            ( { model | selectedTab = num }, Cmd.none )

        ChangeParameter par value ->
            case String.toFloat value of
                Err msg ->
                    let
                        _ =
                            Debug.log "ERROR Could not parse text field value as float: "
                                msg

                        newParameters =
                            Dict.remove par model.parameters
                    in
                        ( { model | parameters = newParameters }, Cmd.none )

                Ok res ->
                    let
                        newParameters =
                            Dict.insert par res model.parameters
                    in
                        ( { model | parameters = newParameters }
                        , Cmd.none
                        )

        SendParameters ->
            let
                payload =
                    Dict.toList model.parameters
                        |> List.map (\( par, v ) -> ( par, JE.float v ))
                        |> JE.object

                push =
                    Phoenix.Push.init "joystick" "client"
                        |> Phoenix.Push.withPayload payload

                ( phxSocket, phxCmd ) =
                    Phoenix.Socket.push push model.phxSocket
            in
                ( { model
                    | parameters = Dict.empty
                    , phxSocket = phxSocket
                  }
                , Cmd.map PhoenixMsg phxCmd
                )


subscriptions : Model -> Sub Msg
subscriptions model =
    [ Phoenix.Socket.listen model.phxSocket PhoenixMsg
    , Joystick.axisData AxisData
    , Joystick.connected GamepadConnected
    , Joystick.disconnected GamepadDisconnected
    , Time.every (millisecond * 10) UpdateControlDisplay
    , Time.every (millisecond * 500) SendControlToServer
    ]
        |> Sub.batch


showMessage : String -> Html a
showMessage str =
    Lists.li []
        [ Lists.content []
            [ Html.text str ]
        ]


messageList : List String -> Html a
messageList messages =
    Lists.ul [] <| List.map showMessage messages


white : Options.Property c m
white =
    Color.text Color.white


view : Model -> Html Msg
view model =
    Layout.render Mdl
        model.mdl
        [ Layout.fixedHeader
        , Layout.selectedTab model.selectedTab
        , Layout.onSelectTab SelectTab
        , Layout.scrolling
        ]
        { header =
            [ h1 [ Html.Attributes.style [ ( "padding", "2rem" ) ] ]
                [ img [ src "/images/logo_small.png" ] []
                , Html.text "LiTHe Hex"
                ]
            ]
        , drawer = []
        , tabs = ( [ text "Control", text "Debug" ], [] )
        , main = [ viewBody model ]
        }


createInputField : Model -> Int -> ( String, PIDParameter ) -> Html Msg
createInputField model idx ( desc, field ) =
    let
        indexOffset =
            1
    in
        Textfield.render Mdl
            [ idx + indexOffset ]
            model.mdl
            [ Textfield.onInput (ChangeParameter field)
            , Textfield.label desc
            ]


viewControl : Model -> List (Html Msg)
viewControl model =
    [ Joystick.joystickDisplay model.joystick
    , Card.view [ Elevation.e2 ]
        [ Card.title [] [ Card.head [] [ text "PID parameters" ] ]
        , Card.actions [ Card.border ]
            (List.indexedMap (createInputField model)
                [ ( "Base movement", "base_movement" )
                , ( "Command Y", "command_y" )
                , ( "Goal angle", "goal_angle" )
                , ( "Angle scaledown", "angle_scaledown" )
                , ( "Movement scaledown", "movement_scaledown" )
                , ( "Angle adjustment", "angle_adjustment_border" )
                ]
                ++ [ Button.render Mdl
                        [ 0 ]
                        model.mdl
                        [ Button.onClick SendParameters ]
                        [ text "duck" ]
                   ]
            )
        ]
    ]


viewDebug : Model -> List (Html Msg)
viewDebug model =
    [ lazy Sensors.viewSensors model.sensorData
    , form [ onSubmit SendMessage ]
        [ Textfield.render Mdl
            [ 0 ]
            model.mdl
            [ Textfield.onInput SetNewMessage, Textfield.value model.currentMessage ]
        ]
    , div []
        [ (messageList model.messages) ]
    ]


viewBody : Model -> Html Msg
viewBody model =
    if model.selectedTab == 0 then
        div [ Html.Attributes.style [ ( "padding", "2rem" ) ] ] (viewControl model)
    else
        div [ Html.Attributes.style [ ( "padding", "2rem" ) ] ] (viewDebug model)


main : Program Flags Model Msg
main =
    Html.programWithFlags
        { init = init
        , update = update
        , subscriptions = subscriptions
        , view = view
        }
