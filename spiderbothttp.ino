#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Ramp.h>

rampInt ramp0;
rampInt ramp1;
rampInt ramp2;
rampInt ramp3;
rampInt ramp4;
rampInt ramp5;
rampInt ramp6;
rampInt ramp7;
rampInt ramp8;
rampInt ramp9;
rampInt ramp10;
rampInt ramp11;

//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  400 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  500 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  1900 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define metode SINUSOIDAL_INOUT
#define gentag ONCEFORWARD
#define HASTIGHED_DREJ 2000
#define HASTIGHED_GAA  600
#define LOFT 40

const char* ssid = "FTTH_MK1517";
const char* password = "cigtes7Tweoj";


//til http
WiFiServer server(80);
// Variable to store the HTTP request
String header;
// Auxiliar variables to store the current output state
String output1State = "off";
String output2State = "off";
String output3State = "off";
// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

//serial stuff
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;

//øjne
int rightEyeXInt;
int rightEyeYInt;
int leftEyeXInt;
int leftEyeYInt;

int tid = 1000;
int start = 100;

int pulsen;
int rigtigPulsGammel;

bool tilstand1 = false;
bool tilstand2 = false;

unsigned long gammelTid;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  inputString.reserve(200);

  startOTA();

  Wire.begin(23, 22);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  startRamp();

  hojreBagBen(100, 100, 100);
  Serial.println(hojreBagBenProcent());
  delay(500);
  venstreBagBen(100, 100, 100);
  Serial.println(venstreBagBenProcent());
  delay(500);
  venstreForBen(100, 100, 100);
  Serial.println(venstreForBenProcent());
  hojreForBen(100, 100, 100);
  Serial.println(hojreForBenProcent());
  delay(500);

  pinMode(0, INPUT);
}

void loop() {
  wifiLoop();
  ArduinoOTA.handle();
}

void otaDelay(int tid) {
  for (int i = 0; i < tid; i++) {
    ArduinoOTA.handle();
    delay(1);
  }
}

int graderTilPuls(int grader) {
  int puls = map(grader, 0, 150, USMIN, USMAX);
  return puls;
}

void startRamp() {
  ramp0.setGrain(1);
  ramp0.go(graderTilPuls(start));

  ramp1.setGrain(1);
  ramp1.go(graderTilPuls(start));

  ramp2.setGrain(1);
  ramp2.go(graderTilPuls(start));

  ramp3.setGrain(1);
  ramp3.go(graderTilPuls(start));

  ramp4.setGrain(1);
  ramp4.go(graderTilPuls(start));

  ramp5.setGrain(1);
  ramp5.go(graderTilPuls(start));

  ramp6.setGrain(1);
  ramp6.go(graderTilPuls(start));

  ramp7.setGrain(1);
  ramp7.go(graderTilPuls(start));

  ramp8.setGrain(1);
  ramp8.go(graderTilPuls(start));

  ramp9.setGrain(1);
  ramp9.go(graderTilPuls(start));

  ramp10.setGrain(1);
  ramp10.go(graderTilPuls(start));

  ramp11.setGrain(1);
  ramp11.go(graderTilPuls(start));
}

void hojreBagBen(int servo1, int servo2, int servo3) {
  ramp0.go(graderTilPuls(servo1), tid, metode, gentag);
  ramp1.go(graderTilPuls(servo2), tid, metode, gentag);
  ramp2.go(graderTilPuls(servo3), tid, metode, gentag);
}

int hojreBagBenProcent() {
  pwm.writeMicroseconds(0, ramp0.update());
  pwm.writeMicroseconds(1, ramp1.update());
  pwm.writeMicroseconds(2, ramp2.update());
  float komplet = ramp0.getCompletion();
  int intProcent = int(komplet);
  return intProcent;
}

void venstreBagBen(int servo1, int servo2, int servo3) {
  ramp3.go(graderTilPuls(200 - servo1), tid, metode, gentag);
  ramp4.go(graderTilPuls(200 - servo2), tid, metode, gentag);
  ramp5.go(graderTilPuls(200 - servo3), tid, metode, gentag);
}

int venstreBagBenProcent() {
  pwm.writeMicroseconds(3, ramp3.update());
  pwm.writeMicroseconds(4, ramp4.update());
  pwm.writeMicroseconds(5, ramp5.update());
  float komplet = ramp3.getCompletion();
  int intProcent = int(komplet);
  return intProcent;
}

void venstreForBen(int servo1, int servo2, int servo3) {
  ramp6.go(graderTilPuls(200 - servo1), tid, metode, gentag);
  ramp7.go(graderTilPuls(servo2), tid, metode, gentag);
  ramp8.go(graderTilPuls(servo3), tid, metode, gentag);
}

int venstreForBenProcent() {
  pwm.writeMicroseconds(6, ramp6.update());
  pwm.writeMicroseconds(7, ramp7.update());
  pwm.writeMicroseconds(8, ramp8.update());
  float komplet = ramp6.getCompletion();
  int intProcent = int(komplet);
  return intProcent;
}

void hojreForBen(int servo1, int servo2, int servo3) {
  ramp9.go(graderTilPuls(servo1), tid, metode, gentag);
  ramp10.go(graderTilPuls(200 - servo2), tid, metode, gentag);
  ramp11.go(graderTilPuls(200 - servo3), tid, metode, gentag);
}

int hojreForBenProcent() {
  pwm.writeMicroseconds(9, ramp9.update());
  pwm.writeMicroseconds(10, ramp10.update());
  pwm.writeMicroseconds(11, ramp11.update());
  float komplet = ramp9.getCompletion();
  int intProcent = int(komplet);
  return intProcent;
}

void wifiLoop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {// loop while the client's connected
      serialLoop();
      ArduinoOTA.handle();
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /1/on") >= 0) {
              Serial.println("GPIO 26 on");
              output1State = "on";
              tilstand1 = true;
              gaaFrem();
            } else if (header.indexOf("GET /1/off") >= 0) {
              Serial.println("GPIO 26 off");
              output1State = "off";
              tilstand1 = false;
            } else if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("GPIO 27 on");
              output2State = "on";
              tilstand2 = true;
              startDrej();
              otaDelay(5000);
            } else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("GPIO 27 off");
              output2State = "off";
              tilstand2 = false;
              slutDrej();
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");

            // Display current state, and ON/OFF buttons for GPIO 26
            client.println("<p>gå frem " + output1State + "</p>");
            // If the output26State is off, it displays the ON button
            if (output1State == "off") {
              client.println("<p><a href=\"/1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/1/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 27
            client.println("<p>start drej " + output2State + "</p>");
            // If the output27State is off, it displays the ON button
            if (output2State == "off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 27
            client.println("<p>tredjeKnap" + output3State + "</p>");
            // If the output27State is off, it displays the ON button
            if (output3State == "off") {
              client.println("<p><a href=\"/3/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/3/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void startOTA() {
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void gaaFrem() {
  tid = HASTIGHED_GAA;
  hojreBagBen(100, 100 + LOFT, 100 + LOFT * 2);//sænk højre bagben
  while (hojreBagBenProcent() < 50) {
    ArduinoOTA.handle();
  }

  venstreForBen(150, 100 + LOFT * 2, 100 + LOFT * 2);//løft venstre forben
  while (venstreForBenProcent() < 50 && hojreBagBenProcent() < 100) {
    ArduinoOTA.handle();
  }
  venstreForBen(150, 100, 100); //sæt venstre forben ned
  hojreBagBen(100, 100, 100);//stræk hojre bagben ud
  while (venstreForBenProcent() < 100 && hojreBagBenProcent() < 100) {
    ArduinoOTA.handle();
  }

  venstreBagBen(100, 100 + LOFT, 100 + LOFT * 2);//sænk venstre bagben
  while (venstreBagBenProcent() < 50) {
    ArduinoOTA.handle();
  }

  hojreForBen(150, 100 + LOFT * 2, 100 + LOFT * 2);//løft hojre forben
  while (hojreForBenProcent() < 100 && venstreBagBenProcent() < 100) {
    ArduinoOTA.handle();
  }

  hojreForBen(150, 100, 100);//stræk hojre forben ud
  venstreBagBen(100, 100, 100);//stræk venstre bagben ud
  while (hojreForBenProcent() < 100 && venstreBagBenProcent() < 100) {
    ArduinoOTA.handle();
  }

  venstreBagBen(50, 100, 100);
  hojreBagBen(50, 100, 100);
  venstreForBen(100, 100, 100);
  hojreForBen(100, 100, 100);
  while (venstreBagBenProcent() < 100 && hojreBagBenProcent() < 100 && venstreForBenProcent() < 100 && hojreForBenProcent() < 100) {
    ArduinoOTA.handle();
  }

  hojreForBen(100, 100 + LOFT, 100 + LOFT * 2); // sænk hojre forben
  while (hojreForBenProcent() < 50) {
    ArduinoOTA.handle();
  }

  venstreBagBen(100, 100 + LOFT * 2, 100 + LOFT * 4); // bøj og drej venstre bagben
  while (venstreBagBenProcent() < 50 && hojreForBenProcent() < 100) {
    ArduinoOTA.handle();
  }

  hojreForBen(100, 100, 100);
  venstreBagBen(100, 100, 100);// stræk venstre bagben ud
  while (venstreBagBenProcent() < 100 && hojreForBenProcent() < 100) {
    ArduinoOTA.handle();
  }

  venstreForBen(100, 100 + LOFT, 100 + LOFT * 2); // sænk venstre forben
  while (venstreForBenProcent() < 50) {
    ArduinoOTA.handle();
  }

  hojreBagBen(100, 100 + LOFT * 2, 100 + LOFT * 4);
  while (hojreBagBenProcent() < 50 && venstreForBenProcent() < 100) {
    ArduinoOTA.handle();
  }
  venstreForBen(100, 100, 100);
  hojreBagBen(100, 100, 100);
  while (hojreBagBenProcent() < 100 && venstreForBenProcent() < 100) {
    ArduinoOTA.handle();
  }
}

void bounce(int antalBounces) {
  tid = HASTIGHED_DREJ;
  for (int i = 0; i < antalBounces; i++) {
    hojreForBen(100, 100 + LOFT, 100);
    venstreForBen(100, 100 - LOFT, 100);
    hojreBagBen(100, 100 + LOFT, 100);
    venstreBagBen(100, 100 - LOFT, 100);
    while (hojreForBenProcent() < 100 && venstreForBenProcent() < 100 && hojreBagBenProcent() < 100 && venstreBagBenProcent() < 100) {
      ArduinoOTA.handle();
    }
    hojreForBen(100, 100 - LOFT, 100);
    venstreForBen(100, 100 + LOFT, 100);
    hojreBagBen(100, 100 - LOFT, 100);
    venstreBagBen(100, 100 + LOFT, 100);
    while (hojreForBenProcent() < 100 && venstreForBenProcent() < 100 && hojreBagBenProcent() < 100 && venstreBagBenProcent() < 100) {
      ArduinoOTA.handle();
    }
  }
  hojreForBen(100, 100, 100);
  venstreForBen(100, 100, 100);
  hojreBagBen(100, 100, 100);
  venstreBagBen(100, 100, 100);
  while (hojreForBenProcent() < 100 && venstreForBenProcent() < 100 && hojreBagBenProcent() < 100 && venstreBagBenProcent() < 100) {
    ArduinoOTA.handle();
  }
}


void wiggle(int antalBounces) {
  tid = HASTIGHED_DREJ;
  for (int i = 0; i < antalBounces; i++) {
    hojreForBen(100, 100 + LOFT, 100 + LOFT);
    venstreForBen(100, 100 - LOFT, 100 - LOFT);
    hojreBagBen(100, 100 + LOFT, 100 + LOFT);
    venstreBagBen(100, 100 - LOFT, 100 - LOFT);
    while (hojreForBenProcent() < 100 && venstreForBenProcent() < 100 && hojreBagBenProcent() < 100 && venstreBagBenProcent() < 100) {
      ArduinoOTA.handle();
    }
    hojreForBen(100, 100 - LOFT, 100 - LOFT);
    venstreForBen(100, 100 + LOFT, 100 + LOFT);
    hojreBagBen(100, 100 - LOFT, 100 - LOFT);
    venstreBagBen(100, 100 + LOFT, 100 + LOFT);
    while (hojreForBenProcent() < 100 && venstreForBenProcent() < 100 && hojreBagBenProcent() < 100 && venstreBagBenProcent() < 100) {
      ArduinoOTA.handle();
    }
  }
  hojreForBen(100, 100, 100);
  venstreForBen(100, 100, 100);
  hojreBagBen(100, 100, 100);
  venstreBagBen(100, 100, 100);
  while (hojreForBenProcent() < 100 && venstreForBenProcent() < 100 && hojreBagBenProcent() < 100 && venstreBagBenProcent() < 100) {
    ArduinoOTA.handle();
  }
}

void drej(int drejHoz, int drejVert) {
  tid = HASTIGHED_DREJ;

  hojreForBen(150 + drejHoz, 150 - drejVert, 150 - drejVert);
  venstreForBen(150 - drejHoz, 150 - drejVert, 150 - drejVert);
  hojreBagBen(50 + drejHoz, 150 + drejVert, 150 + drejVert);
  venstreBagBen(50 - drejHoz, 150 + drejVert, 150 + drejVert);
  while (hojreForBenProcent() < 100) {
    ArduinoOTA.handle();
    venstreForBenProcent();
    hojreBagBenProcent();
    venstreBagBenProcent();
  }
}

void startDrej() {
  tid = HASTIGHED_DREJ;

  hojreBagBen(100, 100 + LOFT, 100 + LOFT * 2);
  while (hojreBagBenProcent() < 100) {
    ArduinoOTA.handle();
  }
  venstreForBen(150, 150, 150);
  while (venstreForBenProcent() < 100) {
    ArduinoOTA.handle();
  }
  hojreBagBen(100, 100, 100);
  while (hojreBagBenProcent() < 100) {
    ArduinoOTA.handle();
  }
  venstreBagBen(100, 100 + LOFT, 100 + LOFT * 2);
  while (venstreBagBenProcent() < 100) {
    ArduinoOTA.handle();
  }
  hojreForBen(150, 150, 150);
  while (hojreForBenProcent() < 100) {
    ArduinoOTA.handle();
  }
  hojreBagBen(50, 150, 150);
  while (hojreBagBenProcent() < 100) {
    ArduinoOTA.handle();
  }
  venstreBagBen(50, 150, 150);
  while (venstreBagBenProcent() < 100) {
    ArduinoOTA.handle();
  }
}

void slutDrej() {
  hojreBagBen(50, 150, 150);
  venstreBagBen(50, 150, 150);
  hojreForBen(150, 150, 150);
  venstreForBen(150, 150, 150);
  while (hojreBagBenProcent() < 100) {
    venstreBagBenProcent();
    hojreForBenProcent();
    venstreForBenProcent();
    ArduinoOTA.handle();
  }
  hojreBagBen(100, 100, 100);
  while (hojreBagBenProcent() < 100) {
    ArduinoOTA.handle();
  }
  hojreForBen(100, 100, 100);
  while (hojreForBenProcent() < 100) {
    ArduinoOTA.handle();
  }
  venstreBagBen(100, 100, 100);
  while (venstreBagBenProcent() < 100) {
    ArduinoOTA.handle();
  }
  venstreForBen(100, 100, 100);
  while (venstreForBenProcent() < 100) {
    ArduinoOTA.handle();
  }
}


void serialEvent() {
  while (Serial2.available()) {
    // get the new byte:
    char inChar = (char)Serial2.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void serialLoop() {
  if (stringComplete) {
    Serial.print(inputString);
    // clear the string:
    int indexes[5];
    indexes[0] = inputString.indexOf("left eye: ("); // +11
    indexes[1] = inputString.indexOf(", "); // +2
    indexes[2] = inputString.indexOf("), right eye: ("); // +15
    indexes[3] = inputString.indexOf(", ", indexes[2] + 5); // 3
    indexes[4] = inputString.indexOf("), nose: ("); // 10

    if (indexes[0] > 0) {
      String firstValue = inputString.substring(indexes[0] + 11, indexes[1]);
      leftEyeXInt = firstValue.toInt();
      String secondValue = inputString.substring(indexes[1] + 3, indexes[2]);
      leftEyeYInt = secondValue.toInt();

      String rightEyeXValue = inputString.substring(indexes[2] + 15, indexes[3]);
      rightEyeXInt = rightEyeXValue.toInt();
      String rightEyeYValue = inputString.substring(indexes[3] + 3, indexes[4]);
      rightEyeYInt = rightEyeYValue.toInt();

      Serial.print("leftEye er (");
      Serial.print(leftEyeXInt);
      Serial.print(", ");
      Serial.print(leftEyeYInt);
      Serial.print(") ");
      Serial.print(" rightEye er (");
      Serial.print(rightEyeXInt);
      Serial.print(", ");
      Serial.print(rightEyeYInt);
      Serial.println(")");
    }
    inputString = "";
    stringComplete = false;
  }
  serialEvent();
}
