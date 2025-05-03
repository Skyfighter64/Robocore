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
  // parse json event data
  console.log(event.data)
  const data = JSON.parse(event.data)

  // append log data to the log
  if(data["sensor"] == "encoder")
  {
    displayEncoder(data);
  }
  else
  {
    appendLog(data);
  }
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
  document.getElementById('resetEncodersButton').addEventListener('click', function(){ websocket.send("resetEncoders");});
}
function toggle()
{
  websocket.send('toggle');
}

function appendLog(data)
{
  // escape any html, etc...
  textNode = document.createTextNode(data["time"] + ": " + data["text"] + "\n");
  document.getElementById('log_text').appendChild(textNode);
};

function displayEncoder(data)
{
  const wheel = data["wheel"]

  if(wheel == "l")
  {
    if ('speed' in data) document.getElementById('speedL').textContent = data.speed;
    if ('count' in data) document.getElementById('countL').textContent = data.count;
    if ('distance' in data) document.getElementById('distanceL').textContent = data.distance;
  }
  else if (wheel == "r")
  {
    if ('speed' in data) document.getElementById('speedR').textContent = data.speed;
    if ('count' in data) document.getElementById('countR').textContent = data.count;
    if ('distance' in data) document.getElementById('distanceR').textContent = data.distance;
  }

}

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