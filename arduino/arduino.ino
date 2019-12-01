#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <math.h>

#define LED_PIN 2
#define NUM_LEDS 16

const char* ssid = "your_SSID"; //Fill in the SSID of desired Wi-Fi network
const char* password = "your_password"; //Fill in the password of the Wi-Fi network

ESP8266WebServer server(80);

CRGB leds[NUM_LEDS];

int colortemp = 6600;
int R = 255;
int G = 255;
int B = 255;

void webRoot() {
  server.send(200, "text/html", String("<!doctype HTML>\n") + 
 F("<html>\n") + 
 F("<head>\n") + 
 F("  <meta charset=\"utf-8\">\n") + 
 F("  <meta id=\"viewport\" name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0\">\n") + 
 F("  <title>PiLight</title>\n") + 
 F("  <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\">\n") + 
 F("  <style type=\"text/css\">\n") + 
 F("  body {\n") + 
 F("    background-color: #272822;\n") + 
 F("    color: #FFFFFF;\n") + 
 F("  }\n") + 
 F("  .pane {\n") + 
 F("    position: absolute;\n") + 
 F("    z-index: 0;\n") + 
 F("  }\n") + 
 F("  .pane_scroll {\n") + 
 F("    position: absolute;\n") + 
 F("    z-index: 1;\n") + 
 F("  }\n") + 
 F("  .jscolor {\n") + 
 F("    border: none;\n") + 
 F("  }\n") + 
 F("  #colorcontainer {\n") + 
 F("    margin-top: -20px;\n") + 
 F("  }\n") + 
 F("  </style>\n") + 
 F("  <script>\n") + 
 F("  \"use strict\";\n") + 
 F("  var ct_start = 1500;\n") + 
 F("  var ct_end = 12000;\n") + 
 F("  var tNow = ") + String(colortemp) + F(";\n") + 
 F("  var rNow = ") + String(R) + F(";\n") + 
 F("  var gNow = ") + String(G) + F(";\n") + 
 F("  var bNow = ") + String(B) + F(";\n") + 
 F("\n") + 
 F("  function colorTemptoRGB(colortemp) {\n") + 
 F("    var r, g, b;\n") + 
 F("    colortemp /= 100;\n") + 
 F("    if (colortemp <= 66) {\n") + 
 F("      r = 255;\n") + 
 F("      g = colortemp;\n") + 
 F("      g = 99.47 * Math.log(g) - 161.1;\n") + 
 F("      if (g < 0) g = 0;\n") + 
 F("      if (g > 255) g = 255;\n") + 
 F("      if (colortemp <= 19) {\n") + 
 F("        b = 0;\n") + 
 F("      } else {\n") + 
 F("        b = colortemp - 10;\n") + 
 F("        b = 138.5 * Math.log(b) - 305;\n") + 
 F("        if (b < 0) b = 0;\n") + 
 F("        if (b > 255) b = 255;\n") + 
 F("      }\n") + 
 F("    } else {\n") + 
 F("      r = colortemp - 60;\n") + 
 F("      r = 329.7 * Math.pow(r, -0.1332);\n") + 
 F("      g = colortemp - 60;\n") + 
 F("      g = 288.1 * Math.pow(g, -0.07551);\n") + 
 F("      if (r < 0) r = 0;\n") + 
 F("      if (r > 255) r = 255;\n") + 
 F("      if (g < 0) g = 0;\n") + 
 F("      if (g > 255) g = 255;\n") + 
 F("      b = 255;\n") + 
 F("    }\n") + 
 F("    r = 0|r;\n") + 
 F("    g = 0|g;\n") + 
 F("    b = 0|b;\n") + 
 F("    return {'r': r, 'g': g, 'b': b};\n") + 
 F("  }\n") + 
 F("\n") + 
 F("  function colorTemptoRGBString(colortemp) {\n") + 
 F("    var rgb = colorTemptoRGB(colortemp);\n") + 
 F("    var r = rgb.r, g = rgb.g, b = rgb.b;\n") + 
 F("\n") + 
 F("    r = r.toString(16);\n") + 
 F("    g = g.toString(16);\n") + 
 F("    b = b.toString(16);\n") + 
 F("    if (r.length < 2) r = '0'+r;\n") + 
 F("    if (r.length < 2) r = '0'+r;\n") + 
 F("    if (g.length < 2) g = '0'+g;\n") + 
 F("    if (g.length < 2) g = '0'+g;\n") + 
 F("    if (b.length < 2) b = '0'+b;\n") + 
 F("    if (b.length < 2) b = '0'+b;\n") + 
 F("\n") + 
 F("    return '#'+r+g+b;\n") + 
 F("  }\n") + 
 F("\n") + 
 F("  function ctTouch(e) {\n") + 
 F("    e.preventDefault();\n") + 
 F("    if (ctdown == true) {\n") + 
 F("    var t = e.changedTouches;\n") + 
 F("      for(var i in t) {\n") + 
 F("        var tX = (t[i].pageX - ctx_scroll_ct.canvas.offsetLeft) * window.devicePixelRatio;\n") + 
 F("        ctx_scroll_ct.clearRect(0, 0, ctx_scroll_ct.canvas.width, ctx_scroll_ct.canvas.height);\n") + 
 F("        tNow = 0|(ct_end - ct_start) * (tX / ctx_scroll_ct.canvas.width) + ct_start;\n") + 
 F("        ctx_scroll_ct.beginPath();\n") + 
 F("        ctx_scroll_ct.moveTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width-12, 0);\n") + 
 F("        ctx_scroll_ct.lineTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width-12, ctx_scroll_ct.canvas.height);\n") + 
 F("        ctx_scroll_ct.moveTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width+12, 0);\n") + 
 F("        ctx_scroll_ct.lineTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width+12, ctx_scroll_ct.canvas.height);\n") + 
 F("        ctx_scroll_ct.moveTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width-12, 2);\n") + 
 F("        ctx_scroll_ct.lineTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width+12, 2);\n") + 
 F("        ctx_scroll_ct.moveTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width-12, ctx_scroll_ct.canvas.height-2);\n") + 
 F("        ctx_scroll_ct.lineTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width+12, ctx_scroll_ct.canvas.height-2);\n") + 
 F("        ctx_scroll_ct.stroke();\n") + 
 F("        document.getElementById('label_ct').innerHTML = tNow + 'K';\n") + 
 F("        send();\n") + 
 F("        break;\n") + 
 F("      }\n") + 
 F("    }\n") + 
 F("  }\n") + 
 F("\n") + 
 F("  function ctDownMouse(e) {\n") + 
 F("    ctdown = true;\n") + 
 F("    ctMouse(e);\n") + 
 F("  }\n") + 
 F("  function ctDownTouch(e) {\n") + 
 F("    ctdown = true;\n") + 
 F("    ctTouch(e);\n") + 
 F("  }\n") + 
 F("  function ctUp() {\n") + 
 F("    ctdown = false;\n") + 
 F("  }\n") + 
 F("\n") + 
 F("  function ctMouse(e) {\n") + 
 F("    e.preventDefault();\n") + 
 F("    if (ctdown == true) {\n") + 
 F("      var tX = e.offsetX * window.devicePixelRatio;\n") + 
 F("      ctx_scroll_ct.clearRect(0, 0, ctx_scroll_ct.canvas.width, ctx_scroll_ct.canvas.height);\n") + 
 F("      tNow = 0|(ct_end - ct_start) * (tX / ctx_scroll_ct.canvas.width) + ct_start;\n") + 
 F("      ctx_scroll_ct.beginPath();\n") + 
 F("      ctx_scroll_ct.moveTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width-12, 0);\n") + 
 F("      ctx_scroll_ct.lineTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width-12, ctx_scroll_ct.canvas.height);\n") + 
 F("      ctx_scroll_ct.moveTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width+12, 0);\n") + 
 F("      ctx_scroll_ct.lineTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width+12, ctx_scroll_ct.canvas.height);\n") + 
 F("      ctx_scroll_ct.moveTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width-12, 2);\n") + 
 F("      ctx_scroll_ct.lineTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width+12, 2);\n") + 
 F("      ctx_scroll_ct.moveTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width-12, ctx_scroll_ct.canvas.height-2);\n") + 
 F("      ctx_scroll_ct.lineTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width+12, ctx_scroll_ct.canvas.height-2);\n") + 
 F("      ctx_scroll_ct.stroke();\n") + 
 F("      document.getElementById('label_ct').innerHTML = tNow + 'K';\n") + 
 F("      send();\n") + 
 F("    }\n") + 
 F("  }\n") + 
 F("\n") + 
 F("  function send() {\n") + 
 F("    var xhr = new XMLHttpRequest();\n") + 
 F("    xhr.open('GET', './set?T=' + tNow + '&R=' + rNow + '&G=' + gNow + '&B=' + bNow, true);\n") + 
 F("    xhr.send(null);\n") + 
 F("  }\n") + 
 F("\n") + 
 F("  function sendrgb(jscolor) {\n") + 
 F("    rNow = 0|jscolor.rgb[0];\n") + 
 F("    gNow = 0|jscolor.rgb[1];\n") + 
 F("    bNow = 0|jscolor.rgb[2];\n") + 
 F("    send();\n") + 
 F("  }\n") + 
 F("\n \
  </script>\n") + 
 F("</head>\n") + 
 F("<body>\n") + 
 F("<div class=\"container\">\n") + 
 F("  <h3><s>P*Light</s> PiLight</h3>\n") + 
 F("  <hr>\n") + 
 F("  <h4>Color temperature</h4>\n") + 
 F("  <div class=\"row\" id=\"canvascontainer\">\n") + 
 F("    <div class=\"col-md-12\">\n") + 
 F("    <canvas class=\"pane\" id=\"colortemp\"></canvas>\n") + 
 F("    <canvas class=\"pane_scroll\" id=\"scroll_ct\"></canvas>\n") + 
 F("    <div id=\"spacer1\"></div>\n") + 
 F("    </div>\n") + 
 F("  </div>\n") + 
 F("  <h4 id=\"label_ct\"></h4>\n") + 
 F("  <hr>\n") + 
 F("  <h4>RGB / Luminance</h4>\n") + 
 F("  <div class=\"row\" id=\"colorcontainer\">\n") + 
 F("    <div class=\"col-md-12\">\n") + 
 F("      <input id=\"colorpicker\" class=\"jscolor {width: 240, height: 200}\">\n") + 
 F("    </div>\n") + 
 F("  </div>\n") + 
 F("</div>\n") + 
 F("<script>\n") + 
 F("window.devicePixelRatio = window.devicePixelRatio || 1;\n") + 
 F("var cwidth = parseInt(document.getElementById(\"spacer1\").offsetWidth);\n") + 
 F("var cheight = 100;\n") + 
 F("if (cwidth < 400) cheight = 50;\n") + 
 F("document.getElementById(\"colortemp\").width = cwidth * window.devicePixelRatio;\n") + 
 F("document.getElementById(\"colortemp\").height = cheight * window.devicePixelRatio;\n") + 
 F("document.getElementById(\"scroll_ct\").width = cwidth * window.devicePixelRatio;\n") + 
 F("document.getElementById(\"scroll_ct\").height = cheight * window.devicePixelRatio;\n") + 
 F("\n") + 
 F("document.getElementById(\"colortemp\").style.width = cwidth + 'px';\n") + 
 F("document.getElementById(\"colortemp\").style.height = cheight + 'px';\n") + 
 F("document.getElementById(\"scroll_ct\").style.width = cwidth + 'px';\n") + 
 F("document.getElementById(\"scroll_ct\").style.height = cheight + 'px';\n") + 
 F("\n") + 
 F("document.getElementById(\"spacer1\").setAttribute('style', 'height: ' + cheight + 'px');\n") + 
 F("\n") + 
 F("var c = document.getElementById(\"colortemp\");\n") + 
 F("ctx_colortemp = c.getContext(\"2d\");\n") + 
 F("var gradient = ctx_colortemp.createLinearGradient(0, 0, ctx_colortemp.canvas.width, 0);\n") + 
 F("\n") + 
 F("for(var i = ct_start; i < ct_end; i+=100){\n") + 
 F("  gradient.addColorStop((i - ct_start) / (ct_end - ct_start), colorTemptoRGBString(i));\n") + 
 F("}\n") + 
 F("ctx_colortemp.fillStyle = gradient;\n") + 
 F("ctx_colortemp.fillRect(0, 0, ctx_colortemp.canvas.width, ctx_colortemp.canvas.height);\n") + 
 F("\n") + 
 F("c = document.getElementById(\"scroll_ct\");\n") + 
 F("ctx_scroll_ct = c.getContext(\"2d\");\n") + 
 F("\n") + 
 F("ctx_scroll_ct.beginPath();\n") + 
 F("ctx_scroll_ct.moveTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width-12, 0);\n") + 
 F("ctx_scroll_ct.lineTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width-12, ctx_scroll_ct.canvas.height);\n") + 
 F("ctx_scroll_ct.moveTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width+12, 0);\n") + 
 F("ctx_scroll_ct.lineTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width+12, ctx_scroll_ct.canvas.height);\n") + 
 F("ctx_scroll_ct.moveTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width-12, 2);\n") + 
 F("ctx_scroll_ct.lineTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width+12, 2);\n") + 
 F("ctx_scroll_ct.moveTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width-12, ctx_scroll_ct.canvas.height-2);\n") + 
 F("ctx_scroll_ct.lineTo((tNow - ct_start) / (ct_end - ct_start) * ctx_scroll_ct.canvas.width+12, ctx_scroll_ct.canvas.height-2);\n") + 
 F("\n") + 
 F("ctx_scroll_ct.strokeStyle = '#000';\n") + 
 F("ctx_scroll_ct.lineWidth = 4;\n") + 
 F("ctx_scroll_ct.stroke();\n") + 
 F("\n") + 
 F("document.getElementById('label_ct').innerHTML = tNow + 'K';\n") + 
 F("\n") + 
 F("c.addEventListener(\"touchstart\", ctDownTouch, false);\n") + 
 F("c.addEventListener(\"touchmove\", ctTouch, false);\n") + 
 F("c.addEventListener(\"touchend\", ctUp, false);\n") + 
 F("c.addEventListener(\"touchcancel\", ctUp, false);\n") + 
 F("c.addEventListener(\"touchleave\", ctUp, false);\n") + 
 F("c.addEventListener(\"mousedown\", ctDownMouse, false);\n") + 
 F("c.addEventListener(\"mousemove\", ctMouse, false);\n") + 
 F("c.addEventListener(\"mouseup\", ctUp, false);\n") + 
 F("c.addEventListener(\"mouseout\", ctUp, false);\n") + 
 F("\n") + 
 F("ctdown = false;\n") + 
 F("document.getElementById(\"colorpicker\").setAttribute('class', 'jscolor {width:' + (cwidth-60) + ', height:150, onFineChange:\\'sendrgb(this)\\'}');\n") + 
 F("window.onload = function(){\n") + 
 F("  document.getElementById(\"colorpicker\").jscolor.fromRGB(rNow, gNow, bNow);\n") + 
 F("  document.getElementById(\"colorpicker\").jscolor.show();\n") + 
 F("  document.getElementById(\"colorpicker\").setAttribute('style', 'display: none');\n") + 
 F("}\n") + 
 F("</script>\n") + 
 F("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>\n") + 
 F("</body>\n") + 
 F("</html>"));
}

void webSet() {
  for(int i = 0; i < server.args(); i++) {
    if (server.argName(i)[0] == 'T') {
      colortemp = String(server.arg(i)).toInt();
    } else if (server.argName(i)[0] == 'R') {
      R = String(server.arg(i)).toInt();
    } else if (server.argName(i)[0] == 'G') {
      G = String(server.arg(i)).toInt();
    } else if (server.argName(i)[0] == 'B') {
      B = String(server.arg(i)).toInt();
    }
    Serial.print(server.argName(i));
    Serial.print(" ");
    Serial.println(server.arg(i));
  }
  server.send(200, "text/plain", "ok");

  int Rct, Gct, Bct;
  colorTemptoRGB(colortemp, Rct, Gct, Bct);
  
  float Rtmp, Gtmp, Btmp;
  Rtmp = R / 255. * Rct;
  Gtmp = G / 255. * Gct;
  Btmp = B / 255. * Bct;

  int Rset, Gset, Bset;
  Rset = (int)Rtmp;
  Gset = (int)Gtmp;
  Bset = (int)Btmp;

  if (Rset < 0) Rset = 0;
  if (Rset > 255) Rset = 255;
  if (Gset < 0) Gset = 0;
  if (Gset > 255) Gset = 255;
  if (Bset < 0) Bset = 0;
  if (Bset > 255) Bset = 255;

  setLED(Rset, Gset, Bset);
  Serial.print("R");
  Serial.print(Rset);
  Serial.print("G");
  Serial.print(Gset);
  Serial.print("B");
  Serial.println(Bset);
  
}

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Establishing connection with ");
  Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esplight")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", webRoot);
  server.on("/set", webSet);
  
  server.begin();
  Serial.println("HTTP server started");
}

void setLED(int R, int G, int B) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(R, G, B);
  }
  FastLED.show();
}

void colorTemptoRGB(long colortemp, int &R, int &G, int &B) {
  float tmp = colortemp / 100.;
  if (tmp <= 66) {
    R = 255;
    G = 99.4708025861 * log(tmp) - 161.1195681661;
    if (G < 0) {
      G = 0;
    } else if (G > 255) {
      G = 255;
    }
    if (tmp <= 19) {
      B = 0;
    } else {
      B = 138.5177312231 * log(tmp - 10) - 305.0447927307;
      if (B < 0) {
        B = 0;
      } else if (B > 255) {
        B = 255;
      }
    }
  } else {
    R = 329.698727446 * pow(tmp - 60, -0.1332047592);
    G = 288.1221695283 * pow(tmp - 60, -0.0755148492);
    if (R < 0) {
      R = 0;
    } else if (R > 255) {
      R = 255;
    }
    if (G < 0) {
      G = 0;
    } else if (G > 255) {
      G = 255;
    }
    B = 255;
  }
}

void loop() {
  server.handleClient();
}
