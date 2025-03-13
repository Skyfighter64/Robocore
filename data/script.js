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
