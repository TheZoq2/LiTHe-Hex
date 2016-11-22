module App exposing (..)

import Html exposing (Html, h1, img, text, div, input, br, form)
import Html.Attributes exposing (style, value, src)
import Html.Events exposing (onInput, onSubmit)
import Html.App
import Json.Encode as JE
import Json.Decode as JD exposing ((:=))
import Phoenix.Socket exposing (Socket)
import Phoenix.Channel
import Phoenix.Push
import Material
import Material.Textfield as Textfield
import Material.List as Lists
import Material.Layout as Layout
import Array exposing (Array)
import Time exposing (Time, millisecond)
import Joystick


type Msg
    = PhoenixMsg (Phoenix.Socket.Msg Msg)
    | SetNewMessage String
    | SendMessage
    | ReceiveChatMessage JE.Value
    | AxisData Joystick.JoystickData
    | Tick Time
    | Mdl (Material.Msg Msg)


type alias Model =
    { phxSocket : Socket Msg
    , currentMessage : String
    , messages : Array String
    , joystick : Joystick.JoystickData
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
                |> Phoenix.Socket.on "new_msg" "debug:lobby" ReceiveChatMessage
                |> Phoenix.Socket.join (Phoenix.Channel.init "debug:lobby")
    in
        { phxSocket = phxSocket
        , currentMessage = ""
        , messages = Array.empty
        , mdl = Material.model
        , joystick = { x = 0, y = 0, rotation = 0, thrust = 0 }
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
                            Array.push chatMessage.body model.messages
                      }
                    , Cmd.none
                    )

                Err error ->
                    ( model, Cmd.none )

        SetNewMessage str ->
            { model | currentMessage = str } ! []

        Tick time ->
            (model, Joystick.poll 0)

        AxisData data ->
            ( { model | joystick = data }, Cmd.none )

        SendMessage ->
            let
                payload =
                    JE.object [ ( "body", JE.string model.currentMessage ) ]

                push =
                    Phoenix.Push.init "new_msg" "debug:lobby"
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
    , Time.every (millisecond*10) Tick
    ]
        |> Sub.batch


showMessage : String -> Html a
showMessage str =
    Lists.li []
        [ Lists.content []
            [ Lists.avatarIcon "photo_camera" []
            , Html.text str
            ]
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
        [ div []
            [ (messageList <| Array.toList model.messages) ]
        , form [ onSubmit SendMessage ]
            [ Textfield.render Mdl
                [ 0 ]
                model.mdl
                [ Textfield.onInput SetNewMessage, Textfield.value model.currentMessage ]
            ]
        , Joystick.joystickDisplay model.joystick
        ]


main : Program Flags
main =
    Html.App.programWithFlags
        { init = init
        , update = update
        , subscriptions = subscriptions
        , view = view
        }
