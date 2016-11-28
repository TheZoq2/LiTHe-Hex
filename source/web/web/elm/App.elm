module App exposing (..)

import Html exposing (Html, h1, img, text, div, input, br, form)
import Html.Attributes exposing (style, value, src)
import Html.Events exposing (onInput, onSubmit)
import Html.Lazy exposing (lazy)
import Html.App
import Json.Encode as JE
import Json.Decode as JD exposing ((:=))
import Time exposing (Time, millisecond)
import Date
import Phoenix.Socket exposing (Socket)
import Phoenix.Channel
import Phoenix.Push
import Material
import Material.Textfield as Textfield
import Material.List as Lists
import Material.Layout as Layout
import Joystick
import Sensors


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
    | Mdl (Material.Msg Msg)


type alias Model =
    { phxSocket : Socket Msg
    , currentMessage : String
    , messages : List String
    , joystick : Joystick.JoystickData
    , joystickIndex : Maybe Int
    , sensorData : List ( Date.Date, Float )
    , mdl : Material.Model
    }


type alias ChatMessage =
    { body : String }


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
        , sensorData =
            [ ( Date.fromTime 1448928000000, 1.7 )
            , ( Date.fromTime 1451606400000, 2 )
            , ( Date.fromTime 1454284800000, 1 )
            , ( Date.fromTime 1456790400000, 1 )
            ]
        , mdl = Material.model
        }
            ! [ Cmd.map PhoenixMsg phxCmd ]


chatMessageDecoder : JD.Decoder ChatMessage
chatMessageDecoder =
    JD.object1 ChatMessage
        ("body" := JD.string)


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        PhoenixMsg msg' ->
            let
                ( phxSocket, phxCmd ) =
                    Phoenix.Socket.update msg' model.phxSocket
            in
                ( { model | phxSocket = phxSocket }
                , Cmd.map PhoenixMsg phxCmd
                )

        Mdl msg' ->
            Material.update msg' model

        ReceiveChatMessage raw ->
            case JD.decodeValue chatMessageDecoder raw of
                Ok chatMessage ->
                    ( { model
                        | messages =
                            chatMessage.body :: model.messages
                      }
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


view : Model -> Html Msg
view model =
    Layout.render Mdl
        model.mdl
        [ Layout.fixedHeader
        , Layout.scrolling
        ]
        { header =
            [ h1 [ Html.Attributes.style [ ( "padding", "2rem" ) ] ]
                [ img [ src "/images/logo_small.png" ] []
                , Html.text "LiTHe Hex"
                ]
            ]
        , drawer = []
        , tabs = ( [], [] )
        , main = [ viewBody model ]
        }


viewBody : Model -> Html Msg
viewBody model =
    div [ Html.Attributes.style [ ( "padding", "2rem" ) ] ]
        [ Joystick.joystickDisplay model.joystick
        , lazy Sensors.viewSensor model.sensorData
        , form [ onSubmit SendMessage ]
            [ Textfield.render Mdl
                [ 0 ]
                model.mdl
                [ Textfield.onInput SetNewMessage, Textfield.value model.currentMessage ]
            ]
        , div []
            [ (messageList model.messages) ]
        ]


main : Program Flags
main =
    Html.App.programWithFlags
        { init = init
        , update = update
        , subscriptions = subscriptions
        , view = view
        }
