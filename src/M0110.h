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

#define M0110_BUFFER_SIZE 100
#define NULL_CODE  0x7B

/* Keys */

#define M0110_RETURN    0x0D
#define M0110_TAB       0x09
#define M0110_SPACE     0x20
#define M0110_BACKSPACE 0x08
#define M0110_ENTER     0x0A

// Extended ASCII

// Modifier Keys
#define M0110_COMMAND     0x80
#define M0110_SHIFT       0x81
#define M0110_CAPS_LOCK   0x82
#define M0110_OPTION      0x83
// Key Pad Keys
#define M0110_RIGHT_ARROW 0x84
#define M0110_LEFT_ARROW  0x85
#define M0110_DOWN_ARROW  0x86
#define M0110_UP_ARROW    0x87
#define M0110_KP_DOT      0x88
#define M0110_KP_CLEAR    0x89
#define M0110_KP_ENTER    0x8A
#define M0110_KP_MINUS    0x8B
#define M0110_KP_0        0x8C
#define M0110_KP_1        0x8D
#define M0110_KP_2        0x8E
#define M0110_KP_3        0x8F
#define M0110_KP_4        0x90
#define M0110_KP_5        0x91
#define M0110_KP_6        0x92
#define M0110_KP_7        0x93
#define M0110_KP_8        0x94
#define M0110_KP_9        0x95
// Key Pad Extended Keys
#define M0110_KP_ASTERISK 0x96
#define M0110_KP_PLUS     0x97
#define M0110_KP_EQUALS   0x98
#define M0110_KP_SLASH    0x99

/* ASCII to Scan Code Table */

#define _SCSHIFT 0x80

const uint8_t _M0110_AsciiToScanCode[154] = {
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
	NULL_CODE,       // DEL
	// 0x80
	0x37, // COMMAND
	0x38, // SHIFT
	0x39, // CAPS LOCK
	0x3A, // OPTION
	0x02, // RIGHT ARROW
	0x06, // LEFT ARROW
	0x08, // DOWN ARROW
	0x0D, // UP ARROW
	0x01, // KP DOT
	0x07, // KP CLEAR
	0x0C, // KP ENTER
	0x0E, // KP MINUS
	0x12, // KP 0
	0x13, // KP 1
	0x14, // KP 2
	0x15, // KP 3
	// 0x90
	0x16, // KP 4
	0x17, // KP 5
	0x18, // KP 6
	0x19, // KP 7
	0x1B, // KP 8
	0x1C, // KP 9
	0x02, // KP ASTERISK
	0x06, // KP PLUS
	0x08, // KP EQUALS
	0x0D // KP SLASH
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
	static void _sm(size_t event);
	static void _queueKeyCode(uint8_t keycode);
	static uint8_t _dequeueKeyCode(void);
};

#endif // M0110_H
