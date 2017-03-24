This is a fork of https://shop.heise.de/katalog/make-block-reloaded

# Features and modifications so far
* refactoring of original tetris engine
  * removed linux code, converted *.ino files in *.h and *.cpp file to introduce folder structure, etc.
  * extracted methods, renamed variables etc. to increase maintainability
* added wireless control by using https://github.com/felis/USB_Host_Shield_2.0 with appropriate USB Host Shield (Wii U Pro Controller tested only so far)
* added basic version of Snake
* added config menu to select between functions (Tetris, Snake, etc.)
# Features to come
* add wifi support by attaching an ESP8266
  * OTA update
  * multiplayer support
* more games/functions
  * Pong
  * use as dimmable light bulb
  * ...
# Hardware modifications
Since the firmware exceeds the originally used Arduino Nano's flash (32kb), I now use a Arduino Mega 2560 which is stacked with a [compatible USB Host Shield](https://www.amazon.de/gp/product/B00GHD2ZIE/ref=oh_aui_detailpage_o02_s00?ie=UTF8&psc=1). 
