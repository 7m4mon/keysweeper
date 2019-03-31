/*
 * Library for 16segment LED by 7M4MON. May 2018
 * A 16segment LED is connected to MCP23017 by I2C, and dp to GPIO.
 * This library is based on Adafuluit MCP23017 library.
 */


#include <Wire.h>
#include <avr/pgmspace.h> 

/* prototype declaration for global */
extern void init_i2c_16seg(uint8_t mpc23017_addr, uint8_t pin_dp);
extern void display_str_to_16segment_led(char* msg, uint16_t interval);
extern void display_char_to_16segment_led(char letter);
extern void set_dp(bool high_or_low);

#define MCP23017_ADDRESS 0x20
#define MCP23017_IODIRA  0x00
#define MCP23017_GPIOA   0x12
#define CHARSET_SIZE 96

char *led_message;
uint16_t led_interval;
uint8_t i2caddr, pin_dp_w;

/* Font defined by */
/* https://github.com/dmadison/LED-Segment-ASCII */
/* sorted by */
/* https://github.com/twam/16segment-charset/blob/master/main.c */
const uint16_t charset[CHARSET_SIZE] PROGMEM = {
    0x0000, 0x0140, 0x0048, 0xADC8, 0xADAD, 0x9D9D, 0xE60E, 0x0008, 0x0210, 0x1002, 0x9A9A, 0x8888, 0x1000, 0x8080, 0x0000, 0x1010, //  !"#$%&'()*+,-./
    0x7575, 0x0150, 0xE4E4, 0x25E4, 0x81C1, 0xA625, 0xE5A5, 0x0164, 0xE5E5, 0xA5E5, 0x0808, 0x1008, 0x8210, 0xA480, 0x1082, 0x08E4, // 0123456789:;<=>?
    0x64ED, 0xC1E5, 0x2DEC, 0x6425, 0x2D6C, 0xE425, 0xC025, 0x65A5, 0xC1C1, 0x2C2C, 0x6540, 0xC211, 0x6401, 0x4153, 0x4343, 0x6565, // @ABCDEFGHIJKLMNO
    0xC0E5, 0x6765, 0xC2E5, 0xA5A5, 0x082C, 0x6541, 0x5011, 0x5341, 0x1212, 0xA5C1, 0x3434, 0x0C28, 0x0202, 0x280C, 0x1200, 0x2400, // PQRSTUVWXYZ[\]^_
    0x0002, 0xEC00, 0xE801, 0xE000, 0x0DC0, 0xF000, 0x88A8, 0xA80D, 0xC801, 0x0800, 0x6808, 0x0A18, 0x4001, 0xC980, 0xC800, 0xE800, // `abcdefghijklmno
    0xC00D, 0x880D, 0xC000, 0xA805, 0xE001, 0x6800, 0x5000, 0x5300, 0x1212, 0x05C8, 0xB000, 0x8C28, 0x0808, 0x288C, 0x9090, 0x0000, // pqrstuvwxyz{|}~
};

/**
 * Writes all the pins in one go. This method is very useful if you are implementing a multiplexed matrix and want to get a decent refresh rate.
 */
void write_to_mcp(uint8_t reg, uint16_t ba) {
    Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
    Wire.write(reg);
    Wire.write(ba & 0xFF);
    Wire.write(ba >> 8);        // After wrote to A, next byte go to B automatically.
    Wire.endTransmission();
}

void mcp23017_init(uint8_t addr){
    i2caddr = addr;
    Wire.begin();
    write_to_mcp(MCP23017_IODIRA, 0);   // all output for 16segment led.
    write_to_mcp(MCP23017_GPIOA, 0);    // blank
}


void display_char_to_16segment_led(char letter){
    uint16_t seg;
    bool dp;
    seg = (letter >= 0x20) ? pgm_read_word_near(charset + letter - 0x20) : 0;
    write_to_mcp(MCP23017_GPIOA, seg);
    dp =  (letter == '!' || letter == '.' || letter == '?') ? HIGH : LOW;
    digitalWrite(pin_dp_w,dp);
}

void display_str_to_16segment_led(char* msg, uint16_t interval){
    uint16_t i,len;
    len = strlen(msg);
    for(i=0; i < len; i++){
        display_char_to_16segment_led(*msg++);
        delay(interval);
    }
}

void init_i2c_16seg(uint8_t mpc23017_addr, uint8_t pin_dp) {
    pin_dp_w = pin_dp;
    pinMode(pin_dp_w, OUTPUT);
    digitalWrite(pin_dp_w, LOW);
    mcp23017_init(mpc23017_addr);
}

void set_dp(bool high_or_low){
    digitalWrite(pin_dp_w, high_or_low);
}

