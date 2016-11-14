module App exposing (..)

import Html exposing (Html, h1, text, div, input, br, form)
import Html.Attributes exposing (style, value)
import Html.Events exposing (onInput, onSubmit)
import Html.App
import Json.Encode as JE
import Json.Decode as JD exposing ((:=))
import Phoenix.Socket exposing (Socket)
import Phoenix.Channel
import Phoenix.Push
import Material
import Material.Scheme
import Material.Button as Button
import Material.Textfield as Textfield
import Material.List as Lists
import Material.Layout as Layout
import Material.Options exposing (css)
import Debug
import Array exposing (Array)


type Msg
    = PhoenixMsg (Phoenix.Socket.Msg Msg)
    | Mdl (Material.Msg Msg)
    | SetNewMessage String
    | SendMessage
    | ReceiveChatMessage JE.Value


type alias Model =
    { phxSocket : Socket Msg
    , currentMessage : String
    , messages : Array String
    , mdl : Material.Model
    }


type alias ChatMessage =
    { body : String }


init : ( Model, Cmd Msg )
init =
    let
        ( phxSocket, phxCmd ) =
            Phoenix.Socket.init "wss://emiluren.se:443/socket/websocket"
                |> Phoenix.Socket.withDebug
                |> Phoenix.Socket.on "new_msg" "room:lobby" ReceiveChatMessage
                |> Phoenix.Socket.join (Phoenix.Channel.init "room:lobby")
    in
        { phxSocket = phxSocket
        , currentMessage = ""
        , messages = Array.empty
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
                            Array.push chatMessage.body model.messages
                      }
                    , Cmd.none
                    )

                Err error ->
                    ( model, Cmd.none )

        SetNewMessage str ->
            { model | currentMessage = str } ! []

        SendMessage ->
            let
                payload =
                    JE.object [ ( "body", JE.string model.currentMessage ) ]

                push =
                    Phoenix.Push.init "new_msg" "room:lobby"
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
    Phoenix.Socket.listen model.phxSocket PhoenixMsg


showMessage : String -> Html a
showMessage str =
    Lists.li []
        [ Lists.content []
            [ Lists.avatarIcon "photo_camera" []
            , text str
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
        ]
        { header = [ h1 [ style [ ( "padding", "2rem" ) ] ] [ text "Emiluren.se" ] ]
        , drawer = []
        , tabs = ( [], [] )
        , main = [ viewBody model ]
        }


viewBody : Model -> Html Msg
viewBody model =
    div [ style [ ( "padding", "2rem" ) ] ]
        [ div []
            [ (messageList <| Array.toList model.messages) ]
        , form [ onSubmit SendMessage ]
            [ Textfield.render Mdl
                [ 0 ]
                model.mdl
                [ Textfield.onInput SetNewMessage, Textfield.value model.currentMessage ]
            ]
        ]


main : Program Never
main =
    Html.App.program
        { init = init
        , update = update
        , subscriptions = subscriptions
        , view = view
        }

