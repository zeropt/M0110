/**
 * M0110.h
 * Author: Riley Mann
 * 
 * Created on 26 Feb. 2024
 */

#ifndef M0110_H
#define M0110_H

#if !defined(ARDUINO_ARCH_SAMD)
	#error This code is designed for SAMD processors
#endif

#include <Arduino.h>

/* Pin Configuration */

#define M0110_CLK_PIN 0
#define M0110_DAT_PIN 1

#define M0110_BUFFER_SIZE 100

#define NULL_CODE  0x7B

/* Timer Config */

#define TIM3_PRESCALER 16

/* Keys */

#define M0110_RETURN    0x0D // 0x24
#define M0110_TAB       0x09 // 0x30
#define M0110_SPACE     0x20 // 0x31
#define M0110_BACKSPACE 0x08 // 0x33
#define M0110_ENTER     0x0A // 0x34

#define M0110_COMMAND   0x80 // 0x37
#define M0110_SHIFT     0x81 // 0x38
#define M0110_CAPS_LOCK 0x82 // 0x39
#define M0110_OPTION    0x83 // 0x3A

#define M0110_RIGHT_ARROW 0x85 // 0x42
#define M0110_LEFT_ARROW  0x89 // 0x46
#define M0110_DOWN_ARROW  0x8B // 0x48
#define M0110_UP_ARROW    0x90 // 0x4D

#define M0110_KP_DOT   0x84 // 0x41
#define M0110_KP_CLEAR 0x8A // 0x47
#define M0110_KP_ENTER 0x8F // 0x4C
#define M0110_KP_MINUS 0x91 // 0x4E
#define M0110_KP_0 0x95 // 0x52
#define M0110_KP_1 0x96 // 0x53
#define M0110_KP_2 0x97 // 0x54
#define M0110_KP_3 0x98 // 0x55
#define M0110_KP_4 0x99 // 0x56
#define M0110_KP_5 0x9A // 0x57
#define M0110_KP_6 0x9B // 0x58
#define M0110_KP_7 0x9C // 0x59
#define M0110_KP_8 0x9E // 0x5B
#define M0110_KP_9 0x9F // 0x5C

#define M0110_KP_ASTERISK 0xA0 // 0x62
#define M0110_KP_PLUS     0xA4 // 0x66
#define M0110_KP_EQUALS   0xA6 // 0x68
#define M0110_KP_SLASH    0xAB // 0x6D

/* ASCII to Scan Code Table */

#define _SCSHIFT 0x80

const uint8_t _M0110_AsciiToScanCode[128] = {
	// 0x00
	NULL_CODE, // NUL
	NULL_CODE, // SOH
	NULL_CODE, // STX
	NULL_CODE, // ETX
	NULL_CODE, // EOT
	NULL_CODE, // ENQ
	NULL_CODE, // ACK
	NULL_CODE, // BEL
	0x33,      // BS : Backspace
	0x30,      // HT : Tab
	0x34,      // LF : Enter
	NULL_CODE, // VT
	NULL_CODE, // FF
	0x24,      // CR : Return
	NULL_CODE, // SO
	NULL_CODE, // SI
	// 0x10
	NULL_CODE, // DLE
	NULL_CODE, // DC1
	NULL_CODE, // DC2
	NULL_CODE, // DC3
	NULL_CODE, // DC4
	NULL_CODE, // NAK
	NULL_CODE, // SYN
	NULL_CODE, // ETB
	NULL_CODE, // CAN
	NULL_CODE, // EM
	NULL_CODE, // SUB
	NULL_CODE, // ESC
	NULL_CODE, // FS
	NULL_CODE, // GS
	NULL_CODE, // RS
	NULL_CODE, // US
	// 0x20
	0x31, // SP
	0x12 | _SCSHIFT, // !
	0x27 | _SCSHIFT, // "
	0x14 | _SCSHIFT, // #
	0x15 | _SCSHIFT, // $
	0x17 | _SCSHIFT, // %
	0x1A | _SCSHIFT, // &
	0x27,            // '
	0x19 | _SCSHIFT, // (
	0x1D | _SCSHIFT, // )
	0x1C | _SCSHIFT, // *
	0x18 | _SCSHIFT, // +
	0x2B,            // ,
	0x1B,            // -
	0x2F,            // .
	0x2C,            // /
	// 0x30
	0x1D, // 0
	0x12, // 1
	0x13, // 2
	0x14, // 3
	0x15, // 4
	0x17, // 5
	0x16, // 6
	0x1A, // 7
	0x1C, // 8
	0x19, // 9
	0x29 | _SCSHIFT, // :
	0x29,            // ;
	0x2B | _SCSHIFT, // <
	0x18,            // =
	0x2F | _SCSHIFT, // >
	0x2C | _SCSHIFT, // ?
	// 0x40
	0x13 | _SCSHIFT, // @
	0x00 | _SCSHIFT, // A
	0x0B | _SCSHIFT, // B
	0x08 | _SCSHIFT, // C
	0x02 | _SCSHIFT, // D
	0x0E | _SCSHIFT, // E
	0x03 | _SCSHIFT, // F
	0x05 | _SCSHIFT, // G
	0x04 | _SCSHIFT, // H
	0x22 | _SCSHIFT, // I
	0x26 | _SCSHIFT, // J
	0x28 | _SCSHIFT, // K
	0x25 | _SCSHIFT, // L
	0x2E | _SCSHIFT, // M
	0x2D | _SCSHIFT, // N
	0x1F | _SCSHIFT, // O
	// 0x50
	0x23 | _SCSHIFT, // P
	0x0C | _SCSHIFT, // Q
	0x0F | _SCSHIFT, // R
	0x01 | _SCSHIFT, // S
	0x11 | _SCSHIFT, // T
	0x20 | _SCSHIFT, // U
	0x09 | _SCSHIFT, // V
	0x0D | _SCSHIFT, // W
	0x07 | _SCSHIFT, // X
	0x10 | _SCSHIFT, // Y
	0x06 | _SCSHIFT, // Z
	0x21,            // [
	0x2A,            // BSLASH
	0x1E,            // ]
	0x16 | _SCSHIFT, // ^
	0x1B | _SCSHIFT, // _
	// 0x60
	0x32, // `
	0x00, // a
	0x0B, // b
	0x08, // c
	0x02, // d
	0x0E, // e
	0x03, // f
	0x05, // g
	0x04, // h
	0x22, // i
	0x26, // j
	0x28, // k
	0x25, // l
	0x2E, // m
	0x2D, // n
	0x1F, // o
	// 0x70
	0x23, // p
	0x0C, // q
	0x0F, // r
	0x01, // s
	0x11, // t
	0x20, // u
	0x09, // v
	0x0D, // w
	0x07, // x
	0x10, // y
	0x06, // z
	0x21 | _SCSHIFT, // {
	0x2A | _SCSHIFT, // |
	0x1E | _SCSHIFT, // }
	0x32 | _SCSHIFT, // ~
	NULL_CODE       // DEL
};

/* M0110 Class */

class M0110 : public Print {
private:
	static uint16_t _dat_pin;
	static uint16_t _clk_pin;
	static uint8_t *_kbuf;
	static unsigned int _kbuf_tail;
	static unsigned int _kbuf_used;
public:
	M0110(uint16_t dat_pin, uint16_t clk_pin);
	void begin(void);
	void end(void);
	size_t write(uint8_t k);
	size_t write(const uint8_t *buffer, size_t size);
	size_t press(uint8_t k);
	size_t release(uint8_t k);
	void releaseAll(void);
	static void _sm(size_t event);
	static void _queueKeyCode(uint8_t keycode);
	static uint8_t _dequeueKeyCode(void);
};

#endif // M0110_H
