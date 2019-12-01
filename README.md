# ESPLight
* A simple network-controlled light using ESP8266 and WS2812B.
* Based on Arduino core for ESP8266, and jscolor color picker library, which follows GNU GPL 3.0.

## How to use
* Configure your Wi-Fi network in ./arduino/arduino.ino file, and upload to ESP8266.
* Connect data pin of your WS2812Bs to a GPIO of ESP8266, and type in the GPIO pin where WS2812Bs are connected and the number of LEDs in ./arduino/arduino.ino.
* That's it.
