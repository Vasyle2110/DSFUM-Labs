//Exercitiul 1
#include <WiFi.h>

const char* ssid     = "Info Orange";
const char* password = "qwerty1230";

WiFiServer server(80);

int state = 0;

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}


void sendHighPage(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();

  client.println("<h2>Ai accesat URL-ul: /H</h2>");
  client.println("<p>Starea este HIGH</p>");
  client.println("<a href=\"/L\">Mergi la LOW</a>");

  client.println();
}

void sendLowPage(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();

  client.println("<h2>Ai accesat URL-ul: /L</h2>");
  client.println("<p>Starea este LOW</p>");
  client.println("<a href=\"/H\">Mergi la HIGH</a>");

  client.println();
}

void sendDefaultPage(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();

  client.println("<h2>Selecteaza o stare:</h2>");
  client.println("<a href=\"/H\">HIGH</a><br>");
  client.println("<a href=\"/L\">LOW</a><br>");

  client.println();
}


void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("Client conectat.");

  String request = client.readStringUntil('\r');  
  client.flush();


  Serial.print("Cerere primita: ");
  Serial.println(request);

  if (request.indexOf("GET /H") >= 0) {
    state = 1;
    sendHighPage(client);
  }
  else if (request.indexOf("GET /L") >= 0) {
    state = 2;
    sendLowPage(client);
  }
  else {
    sendDefaultPage(client);
  }

  delay(1);
  client.stop();
  Serial.println("Client deconectat.\n");
}


//Exercitiul 2
/*#include <WiFi.h>

const char* ssid     = "Info Orange";
const char* password = "qwerty1230";

WiFiServer server(80);

int state = 0;

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void sendCSS(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/css");
  client.println();

  // Aici e „conținutul” fișierului CSS
  client.println("body { font-family: Arial, sans-serif; background-color: #f0f0f0; }");
  client.println("h2 { color: darkblue; text-align: center; }");
  client.println("a { color: darkred; font-weight: bold; text-decoration: none; }");
  client.println("a:hover { text-decoration: underline; }");

  client.println();
}

void sendHighPage(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();

  client.println("<!DOCTYPE html><html><head><meta charset=\"UTF-8\">");
  client.println("<title>HIGH</title>");
  client.println("<link rel=\"stylesheet\" href=\"/style.css\">");  
  client.println("</head><body>");

  client.println("<h2>Ai accesat URL-ul: /H</h2>");
  client.println("<p>Starea este <b>HIGH</b></p>");
  client.println("<a href=\"/L\">Mergi la LOW</a>");

  client.println("</body></html>");
}

void sendLowPage(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();

  client.println("<!DOCTYPE html><html><head><meta charset=\"UTF-8\">");
  client.println("<title>LOW</title>");
  client.println("<link rel=\"stylesheet\" href=\"/style.css\">");  
  client.println("</head><body>");

  client.println("<h2>Ai accesat URL-ul: /L</h2>");
  client.println("<p>Starea este <b>LOW</b></p>");
  client.println("<a href=\"/H\">Mergi la HIGH</a>");

  client.println("</body></html>");
}

void sendDefaultPage(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();

  client.println("<!DOCTYPE html><html><head><meta charset=\"UTF-8\">");
  client.println("<title>Selectare stare</title>");
  client.println("<link rel=\"stylesheet\" href=\"/style.css\">");   
  client.println("</head><body>");

  client.println("<h2>Selecteaza o stare:</h2>");
  client.println("<a href=\"/H\">HIGH</a><br>");
  client.println("<a href=\"/L\">LOW</a><br>");

  client.println("</body></html>");
}


void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("Client conectat.");

  String request = client.readStringUntil('\r');  
  client.flush();

  // Afișăm în Serial URL-ul accesat
  Serial.print("Cerere primita: ");
  Serial.println(request);
  if (request.indexOf("GET /style.css") >= 0) {
    sendCSS(client);
  }
  else if (request.indexOf("GET /H") >= 0) {
    state = 1;
    sendHighPage(client);
  }
  else if (request.indexOf("GET /L") >= 0) {
    state = 2;
    sendLowPage(client);
  }
  else {
    sendDefaultPage(client);
  }

  delay(1);
  client.stop();
  Serial.println("Client deconectat.\n");
}*/

//Exercitiul 3
/*#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid     = "ESP32-Culoare";   
const char *password = "parola1234";      

WiFiServer server(80);

String currentColor = "white";

void sendWebPage(WiFiClient &client, const String &color) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head><meta charset=\"UTF-8\"><title>ESP32 Culoare</title></head>");

  client.print("<body style=\"background-color:");
  client.print(color);
  client.println("; text-align:center; font-family:Arial;\">");

  client.println("<h1>Schimbă culoarea ecranului</h1>");

  client.print("<p>Culoare curentă: <b>");
  client.print(color);
  client.println("</b></p>");

  client.println("<p><a href=\"/red\"><button style=\"font-size:20px; padding:10px;\">Rosu</button></a></p>");
  client.println("<p><a href=\"/green\"><button style=\"font-size:20px; padding:10px;\">Verde</button></a></p>");

  client.println("</body></html>");
  client.println();
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  if (!WiFi.softAP(ssid, password)) {
    Serial.println("Soft AP creation failed.");
    while (1);
  }

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("Client conectat");

  String request = client.readStringUntil('\r');
  Serial.print("Cerere: ");
  Serial.println(request);
  client.flush();

  if (request.indexOf("GET /red") >= 0) {
    currentColor = "red";
  } 
  else if (request.indexOf("GET /green") >= 0) {
    currentColor = "green";
  }

  sendWebPage(client, currentColor);

  delay(1);
  client.stop();
  Serial.println("Client deconectat");
}
*/