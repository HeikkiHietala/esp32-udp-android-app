# esp32-udp-android-app
A combination of ESP32 and an Android app to send UDP datagrams to the ESP32, versions with and without an OLED display on the ESP32

For the Heltec WiFi OLED kit installation, see https://heltec.org/project/wifi-kit-32/

This project is explained in detail in my blog at www.sabulo.com

https://www.sabulo.com/sb/esp32-development-board/using-the-udp-protocol-with-an-esp32-and-android-studio-part-1/

https://www.sabulo.com/sb/esp32-development-board/using-the-udp-protocol-with-an-esp32-and-android-studio-part-2/

https://www.sabulo.com/sb/esp32-development-board/using-the-udp-protocol-with-an-esp32-and-android-studio-part-3/

https://www.sabulo.com/sb/esp32-development-board/using-the-udp-protocol-with-an-esp32-and-android-studio-part-4/

The purpose of this repo is to give you a working example of writing an Android app to run an ESP32. My project is a small car that has a DC motor with a servo for steering. The ESP32 is equipped with a small OLED display so you can easily see what IP address to connect to, and which post to use. 

The other version, NO-OLED, uses a standard serial monitor to replicate the OLED display for those who use a regular ESP32.

![Parts of the system](/images/ESP32%20UDP%20motor%20servo%204.jpg)
