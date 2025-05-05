#include <WiFi.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

const char* ssid = "Franklyn WiFi ";
const char* password = "franklyn2024";

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define DATA_PIN 23
#define CLK_PIN 18
#define CS_PIN 5

MD_Parola display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
WiFiServer server(80);

String message = "HOLA";
bool isScroll = false;
int scrollDelay = 50;
const int maxFixedChars = 8;

String lastMessages[10];
int messageCount = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Conectando al Wi-Fi...");
  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nError: No se pudo conectar al WiFi.");
  } else {
    Serial.println("\nWiFi conectado, IP: ");
    Serial.println(WiFi.localIP());
  }

  server.begin();
  display.begin();
  display.setIntensity(5);
  display.displayClear();
  updateDisplay();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.println("<!DOCTYPE html>");
            client.println("<html lang=\"es\">");
            client.println("<head>");
            client.println("<meta charset=\"UTF-8\">");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("<title>Control del Display LED</title>");
            client.println("<style>");
            client.println("body { font-family: Arial, sans-serif; background: #f4f4f9; margin: 0; padding: 20px; box-sizing: border-box; }");
            client.println("h1 { color: blue; font-size: 1.5em; text-align: center; }");
            client.println("form { background: #fff; padding: 15px; border-radius: 8px; max-width: 400px; margin: auto; box-shadow: 0 0 10px rgba(0,0,0,0.1); }");
            client.println("input, select { padding: 10px; margin: 10px 0; width: 100%; box-sizing: border-box; }");
            client.println("input[type='submit'] { background: blue; color: #fff; border: none; cursor: pointer; font-size: 1em; }");
            client.println("input[type='submit']:hover { background: #45a049; }");
            client.println(".footer { margin-top: 30px; font-size: 0.8em; color: #888; text-align: center; }");
            client.println(".current { color: blue; font-weight: bold; }");
            client.println("</style>");
            client.println("</head>");
            client.println("<body>");
            client.println("<h1>Control del Display LED</h1>");
            client.println("<form action=\"/\" method=\"GET\">");
            client.println("<label>Mensaje:</label>");
            client.println("<input name=\"msg\" type=\"text\" value=\"" + message + "\" required>");
            client.println("<label>Modo:</label>");
            client.println("<select name=\"mode\"><option value=\"fixed\"" + String(isScroll ? "" : " selected") + ">Fijo</option><option value=\"scroll\"" + String(isScroll ? " selected" : "") + ">Scroll</option></select>");
            client.println("<label>Tiempo de desplazamiento (ms):</label>");
            client.println("<input name=\"delay\" type=\"number\" value=\"" + String(scrollDelay) + "\" min=\"50\" max=\"5000\" required>");
            client.println("<input type=\"submit\" value=\"Enviar\">");
            client.println("</form>");

            client.println("<div style=\"text-align: center; margin-top: 20px;\">");
            client.println("<h3>Mensaje actual:</h3>");
            client.println("<div class=\"current\">" + message + "</div>");
            client.println("<h3>Últimos 10 mensajes:</h3>");
            client.println("<ul style=\"list-style: none; padding: 0;\">");
            for (int i = 0; i < messageCount; i++) {
              if (lastMessages[i] == message) {
                client.println("<li class=\"current\">" + lastMessages[i] + "</li>");
              } else {
                client.println("<li>" + lastMessages[i] + "</li>");
              }
            }
            client.println("</ul>");
            client.println("<div>Modo: <strong>" + String(isScroll ? "Scroll" : "Fijo") + "</strong></div>");
            if (isScroll) {
              client.println("<div>Tiempo de desplazamiento: <strong>" + String(scrollDelay) + " ms</strong></div>");
            }
            client.println("</div>");

            client.println("<div class=\"footer\">by Franklyn Terrero</div>");
            client.println("</body>");
            client.println("</html>");
            client.println();
            break;
          } else {
            if (currentLine.startsWith("GET /?")) {
              int msgIndex = currentLine.indexOf("msg=");
              int modeIndex = currentLine.indexOf("mode=");
              int delayIndex = currentLine.indexOf("delay=");

              if (msgIndex != -1 && modeIndex != -1) {
                String newMsg = currentLine.substring(msgIndex + 4, currentLine.indexOf("&", msgIndex));
                newMsg.replace('+', ' ');
                newMsg = urlDecode(newMsg);

                if (messageCount < 10) {
                  lastMessages[messageCount++] = newMsg;
                } else {
                  for (int i = 0; i < 9; i++) {
                    lastMessages[i] = lastMessages[i + 1];
                  }
                  lastMessages[9] = newMsg;
                }

                message = newMsg;

                String mode = currentLine.substring(modeIndex + 5, currentLine.indexOf("&", modeIndex));
                isScroll = (mode == "scroll");

                if (delayIndex != -1) {
                  String delayStr = currentLine.substring(delayIndex + 6, currentLine.indexOf(" ", delayIndex));
                  scrollDelay = delayStr.toInt();
                  if (scrollDelay < 50) scrollDelay = 50;
                  if (scrollDelay > 5000) scrollDelay = 5000;
                }

                updateDisplay();
              }
            }
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }

  if (isScroll) {
    if (display.displayAnimate()) {
      display.displayReset();
    }
  }
}

void updateDisplay() {
  display.displayClear();
  if (isScroll) {
    display.displayScroll(message.c_str(), PA_LEFT, PA_SCROLL_LEFT, scrollDelay);
  } else {
    display.displayText(message.c_str(), PA_CENTER, 0, 0, PA_PRINT, PA_PRINT);
    display.displayAnimate();
  }
}

String urlDecode(String input) {
  String decoded = "";
  char temp[] = "0x00";
  unsigned int len = input.length();
  unsigned int i = 0;
  while (i < len) {
    char c = input.charAt(i);
    if (c == '%') {
      temp[2] = input.charAt(i + 1);
      temp[3] = input.charAt(i + 2);
      decoded += (char)strtol(temp, NULL, 16);
      i += 3;
    } else {
      decoded += (c == '+') ? ' ' : c;
      i++;
    }
  }
  return decoded;
}
