var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

function initWebSocket() 
{
  console.log('Trying to open a WebSocket connection...');
  websocket = new WebSocket(gateway);
  websocket.onopen    = onOpen;
  websocket.onclose   = onClose;
  websocket.onmessage = onMessage; // <-- add this line
}
function onOpen(event) 
{
  console.log('Connection opened');
}

function onClose(event) 
{
  console.log('Connection closed');
  setTimeout(initWebSocket, 5000);
}

function onMessage(event) 
{
  // append log data to the log
  appendLog(event.data);
}
// initialize web socket and buttons
window.addEventListener('load', onLoad);
function onLoad(event) {
  initWebSocket();
  initButton();
}

function initButton() 
{
  document.getElementById('stopButton').addEventListener('click', stopRobot);
  document.getElementById('forwardButton').addEventListener('click', forwardRobot);
  document.getElementById('turnLeftButton').addEventListener('click', function(){ websocket.send("turnLeft");});
  document.getElementById('turnRightButton').addEventListener('click', function(){ websocket.send("turnRight");});
}
function toggle()
{
  websocket.send('toggle');
}

function appendLog(text)
{
  // escape any html, etc...
  text = document.createTextNode(text);
  console.log('Appending text: ' + text);
  document.getElementById('log_text').appendChild(text);
};

function stopRobot(){
  websocket.send('stopRobot');
  console.log("stopped robot");
}
function forwardRobot(){
  websocket.send('forward');
  console.log("Forward robot");
}

function driveRobot(speed, angle)
{
    // send a command to drive the robot
    websocket.send(`drive:${speed},${angle}`);
    console.log(`drive:${speed},${angle}`);
}

var speed = 0;
var angle = 0; 

// control robot with wasd
document.addEventListener('keydown', function(event) {
    if(event.repeat)
    {
        return;
    }
    if(event.key == "w") {
        console.log('w was pressed');
        speed = 255;
    }
    else if(event.key == "a") {
        console.log('a was pressed');
        angle = -0.7853981634
    }
    else if(event.key == "s") {
        console.log('s was pressed');
        speed = -255;
    }
    else if(event.key == "d") {
        console.log('d was pressed');
        angle = 0.7853981634
    }

    driveRobot(speed,angle);
});

document.addEventListener('keyup', function(event) {
    if(event.repeat)
    {
        return;
    }
    if(event.key == "w") {
        console.log('w up');
        speed = 0;
    }
    else if(event.key == "a") {
        console.log('a up');
        angle = 0;
    }
    else if(event.key == "s") {
        console.log('s up');
        speed = 0;
    }
    else if(event.key == "d") {
        console.log('d up');
        angle = 0;
    }
    driveRobot(speed,angle);
});