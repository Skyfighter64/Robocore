#include <credentials.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <webserver/index.html>

#ifndef ROBOT_WEB_SERVER_CPP
#define ROBOT_WEB_SERVER_CPP

/**
 * A webserver for the robot displaying updated information
 * using websockets
*/
class RobotWebServer
{

  private:
    AsyncWebServer* server_ptr;
    AsyncWebSocket* webSocket_ptr;

  public:

  /**
   * Constructor
   * 
   * Create an async web server using the ESPAsyncWebserver
  */
  RobotWebServer()
  {
    this->server_ptr = new AsyncWebServer(80);
    this->webSocket_ptr = new AsyncWebSocket("/ws");
  }

  ~RobotWebServer()
  {
    // delete dynamically allocated objects
    delete this->server_ptr;
    delete this->webSocket_ptr;
  }

  static void eventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
  {
    switch (type) 
    {
      case WS_EVT_CONNECT:
        //Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        //Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
      case WS_EVT_DATA:
      // web socket events come in here
        //handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
    }
  }


  /**
   * Initialize and start the webserver
  */
  void start()
  {
    // open serial if not already done
    /*if(!Serial)
    {
      Serial.begin(115200);
    }*/

    //Serial.print("Connecting to ");
    //Serial.println(WIFI_SSID);

    // Connect to Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PW);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(1000);
      //Serial.print(".");
    }

    //Serial.println("");
    //Serial.println("Connected..!");
    //Serial.print("Got IP: ");  
    //Serial.println(WiFi.localIP());

    // add event handler
    webSocket_ptr->onEvent(this->eventHandler);
    server_ptr->addHandler(webSocket_ptr);

    // set up callback for http get requests
    server_ptr->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Start server
  server_ptr->begin();
  }

  void sendText(String text)
  {
    this->webSocket_ptr->textAll(text);
  }

  void cleanUpClients()
  {
    this->webSocket_ptr->cleanupClients();
  }

};
#endif //ROBOT_WEB_SERVER_CPP