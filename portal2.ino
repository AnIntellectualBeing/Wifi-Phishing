#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const char* ssid = "YourESPAP";
const char* password = "YourESPPassword";

ESP8266WebServer server(80);
DNSServer dnsServer;

void handleRoot() {
  String page = "<html><body>";
  page += "<h1>Welcome to the ESP8266 Captive Portal</h1>";
  page += "<p>Please enter your username and password to connect.</p>";
  page += "<form method='post' action='/login'>";
  page += "<input type='text' name='username' placeholder='Username'><br>";
  page += "<input type='password' name='password' placeholder='Password'><br>";
  page += "<input type='submit' value='Login'>";
  page += "</form>";
  page += "</body></html>";
  server.send(200, "text/html", page);
}

void handleSubmit() {
  String username = server.arg("username");
  String password = server.arg("password");

  String redirectPage = "<html><body>";
  redirectPage += "<h2>Login Successful!</h2>";
  redirectPage += "<p>Username: " + username + "</p>";
  redirectPage += "<p>Password: " + password + "</p>";
  redirectPage += "</body></html>";

  server.send(200, "text/html", redirectPage);
}

void handleNotFound() {
  server.sendHeader("Location", "/", true);  // Redirect to the root page
  server.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(apIP);

  dnsServer.start(53, "*", apIP);
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);

  server.on("/", handleRoot);
  server.on("/login", HTTP_POST, handleSubmit);
  server.onNotFound(handleNotFound);

  server.begin();

  Serial.println("Captive Portal started");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
