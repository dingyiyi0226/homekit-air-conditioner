 # HomeKit Air Conditioner

HomeKit A/C remote control built with NodeMCU-32S (ESP 32) and [HomeSpan](https://github.com/HomeSpan/HomeSpan) library.


## Devices

- [NodeMCU-32S](https://docs.ai-thinker.com/esp32/boards/nodemcu_32s)
- DHT22: Temperature, humidity sensor
- Keyes IR Transmitter: Sending the signal to A/C
- Keyes IR Receiver: Decoding the IR signal from your remote control
- Homepod mini: HomeKit accessories require Homepod/AppleTV as home hub


## Source code

- air-conditioner: the main project for the remote control
- passive-sensor: the project testing for DHT22 sensor
- irsend: the project for IR signal decoding and transmitting


## Libraries

- [HomeSpan](https://github.com/HomeSpan/HomeSpan)
- [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)
- [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266)


## Layout

![Layout](https://raw.githubusercontent.com/dingyiyi0226/homekit-air-conditioner/master/src/IMG_5974.jpg)
