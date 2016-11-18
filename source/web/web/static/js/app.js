// Brunch automatically concatenates all files in your
// watched paths. Those paths can be configured at
// config.paths.watched in "brunch-config.js".
//
// However, those files will only be executed if
// explicitly imported. The only exception are files
// in vendor, which are never wrapped in imports and
// therefore are always executed.

// Import dependencies
//
// If you no longer want to use a dependency, remember
// to also remove its path from "config.paths.watched".
import "phoenix_html"

// Import local files
//
// Local files can be imported directly using relative
// paths "./socket" or full ones "web/static/js/socket".

// import socket from "./socket"

var haveEvents = 'ongamepadconnected' in window;
var controllers = {};

function addGamepad(gamepad) {
    console.log("Gamepad connected at index " + gamepad.index + ": " + gamepad.id +
        ". It has " + gamepad.buttons.length + " buttons and " + gamepad.axes.length + " axes.");
    controllers[gamepad.index] = gamepad;
}

function pollRequestReceived(gamePadIndex) {
    if (!haveEvents) {
        scangamepads();
    }

    if (gamePadIndex in controllers) {
        var gamepad = controllers[gamePadIndex];
        var data = {
            x: gamepad.axes[0],
            y: gamepad.axes[1],
            rotation: gamepad.axes[3],
            thrust: (1 - gamepad.axes[2]) / 2
        }
        elmApp.ports.axisData.send(data);
    }
}

function removeGamepad(gamepad) {
    delete controllers[gamepad.index];
}

function scangamepads() {
    var gamepads = navigator.getGamepads ? navigator.getGamepads() : (navigator.webkitGetGamepads ? navigator.webkitGetGamepads() : []);
    for (var i = 0; i < gamepads.length; i++) {
        if (gamepads[i]) {
            if (gamepads[i].index in controllers) {
                controllers[gamepads[i].index] = gamepads[i];
            } else {
                addGamepad(gamepads[i]);
            }
        }
    }
}

// Set up Elm
const elmDiv = document.querySelector('#elm-container');
const elmApp = Elm.App.embed(elmDiv);

elmApp.ports.poll.subscribe(pollRequestReceived);

window.addEventListener("gamepadconnected", function(e) {
    addGamepad(e.gamepad);
});

window.addEventListener("gamepaddisconnected", function(e) {
    removeGamepad(e.gamepad);
});

if (!haveEvents) {
    setInterval(scangamepads, 500);
}
