 KeySweeper, by Samy Kamkar

 modefied by 7M4MON for Japanese Keyboard
 
 ![](https://github.com/7m4mon/keysweeper/blob/master/KeySweeperCF_1.jpg)
 
 KeySweeperCF
 
 May 23, 2019

 The fifth byte of device ID on my keyboard is not 0xCD, but 0xCF.
 
 * Samy's 0xCD (English).
 * Horiai's 0xCE (Japanese).  
 
 So, I modefied conditional expression "(p[4] == 0xCD) || (p[4] == 0xCE) ||(p[4] == 0xCF)"
 

 I attached 16 segment LED for display keystroke, 

It was connected to MCP23017 by I2C, and DP by GPIO.   


 The keycodes are differrent from English and Japanese.

So, I modefied hidcodes.h to hidcodes_jp.h


 Thank you.
