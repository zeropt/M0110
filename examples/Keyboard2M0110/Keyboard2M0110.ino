/**
 * Keyboard2M0110.ino
 * Author: Riley Mann
 * 
 * The SAMD Board will act as a USB host and read key transitions from a
 * connected USB keyboard. These events will be sent to a connected Macintosh
 * via the M0110 protocol.
 * 
 * Mac 128k/512k/Plus Keyboard Pinout (Socket on Computer)
 * .---------.
 * | 1 2 3 4 | 1 GND
 * |         | 2 Clock
 * |         | 3 Data
 * '---___---' 4 5V
 * 
 * Created on 19 Mar. 2024
 */

#include <M0110.h>
#include "USBKeyboard.h"

#define DAT_PIN 1
#define CLK_PIN 0

// Oem Key to Ascii Table
const uint8_t OemToAscii[100] = {
	// 0
	0x00,
	0x00,
	0x00,
	0x00,
	'a',
	'b',
	'c',
	'd',
	'e',
	'f',
	// 10
	'g',
	'h',
	'i',
	'j',
	'k',
	'l',
	'm',
	'n',
	'o',
	'p',
	// 20
	'q',
	'r',
	's',
	't',
	'u',
	'v',
	'w',
	'x',
	'y',
	'z',
	// 30
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'0',
	// 40
	M0110_RETURN,    // Return
	0x00,            // Esc
	M0110_BACKSPACE, // Backspace
	M0110_TAB,       // Tab
	' ',
	'-',
	'=',
	'[',
	']',
	'\\', // Backslash
	// 50
	0x00, // Hash
	';',
	'\'', // Apostrophe
	'`',
	',',
	'.',
	'/',
	M0110_CAPS_LOCK, // Caps Lock
	0x00, // F1
	0x00, // F2
	// 60
	0x00, // F3
	0x00, // F4
	0x00, // F5
	0x00, // F6
	0x00, // F7
	0x00, // F8
	0x00, // F9
	0x00, // F10
	0x00, // F11
	0x00, // F12
	// 70
	0x00, // Print Screen
	0x00, // Scoll Lock
	0x00, // Pause
	0x00, // Insert
	0x00, // Home
	0x00, // Page Up
	0x00, // Delete
	0x00, // End
	0x00, // Page Down
	M0110_RIGHT_ARROW, // Arrow Right
	// 80
	M0110_LEFT_ARROW,  // Arrow Left
	M0110_DOWN_ARROW,  // Arrow Down
	M0110_UP_ARROW,    // Arrow Up
	M0110_KP_CLEAR,              // Num Lock
	M0110_KP_SLASH,    // KP /
	M0110_KP_ASTERISK, // KP *
	M0110_KP_MINUS,    // KP -
	M0110_KP_PLUS,     // KP +
	M0110_KP_ENTER,    // KP Enter
	M0110_KP_1,        // KP 1
	// 90
	M0110_KP_2,        // KP 2
	M0110_KP_3,        // KP 3
	M0110_KP_4,        // KP 4
	M0110_KP_5,        // KP 5
	M0110_KP_6,        // KP 6
	M0110_KP_7,        // KP 7
	M0110_KP_8,        // KP 8
	M0110_KP_9,        // KP 9
	M0110_KP_0,        // KP 0
	M0110_KP_DOT       // KP .
};

// Initialize USB controller
USBHost usb;

// Attack keyboard controller to USB
USBKeyboard keyboard(usb);

// Initialize M0110 interface
M0110 m0110(DAT_PIN, CLK_PIN);

// Key state trackers
bool command_state = false;
bool shift_state = false;
bool capslock_state = false;
bool option_state = false;

void controlKeysPressed() {
	Serial1.print("CTRL PRESSED: ");
	uint8_t ctrl = keyboard.getCtrlKeysPressed();
	Serial1.print(ctrl, HEX);
	Serial1.print(" > ");
	bool command = ctrl & Cmd;
	bool shift = ctrl & Shift;
	bool option = ctrl & Alt;
	if (command && !command_state) {
		m0110.press(M0110_COMMAND);
		command_state = true;
		Serial1.print("COMMAND");
	}
	if (shift && !shift_state) {
		m0110.press(M0110_SHIFT);
		shift_state = true;
		Serial1.print("SHIFT");
	}
	if (option && !option_state) {
		m0110.press(M0110_OPTION);
		option_state = true;
		Serial1.print("OPTION");
	}
	Serial1.println();
}

void controlKeysReleased() {
	Serial1.print("CTRL RELEASED: ");
	uint8_t ctrl = keyboard.getCtrlKeysReleased();
	Serial1.print(ctrl, HEX);
	Serial1.print(" > ");
	bool command = ctrl & Cmd;
	bool shift = ctrl & Shift;
	bool option = ctrl & Alt;
	if (command && command_state) {
		m0110.release(M0110_COMMAND);
		command_state = false;
		Serial1.print("COMMAND");
	}
	if (shift && shift_state) {
		m0110.release(M0110_SHIFT);
		shift_state = false;
		Serial1.print("SHIFT");
	}
	if (option && option_state) {
		m0110.release(M0110_OPTION);
		option_state = false;
		Serial1.print("OPTION");
	}
	Serial1.println();
}

void keyPressed() {
	Serial1.print("KEY PRESSED: ");
	uint8_t k = keyboard.getOemKey();
	Serial1.print(k, HEX);
	Serial1.print(" > ");
	if (k < 100) {
		k = OemToAscii[k];
		switch (k) {
			case 0x00:
			break;
			case M0110_CAPS_LOCK:
			if (capslock_state) m0110.release(M0110_CAPS_LOCK);
			else m0110.press(M0110_CAPS_LOCK);
			capslock_state = !capslock_state;
			Serial1.print("CAPS");
			break;
			default:
			m0110.press(k);
			Serial1.print(k, HEX);
		}
	}
	Serial1.println();
}

void keyReleased() {
	Serial1.print("KEY RELEASED: ");
	uint8_t k = keyboard.getOemKey();
	Serial1.print(k, HEX);
	Serial1.print(" > ");
	if (k < 100) {
		k = OemToAscii[k];
		switch (k) {
			case 0x00:
			break;
			case M0110_CAPS_LOCK:
			Serial1.print("CAPS");
			break;
			default:
			m0110.release(k);
			Serial1.print(k, HEX);
		}
	}
	Serial1.println();
}

void setup() {
	// Initialize serial using Serial1
	Serial1.begin(115200);
	while (!Serial1);
	Serial1.println("USB Keyboard to M0110");

	// Initialize M0110
	m0110.begin();

	if (usb.Init() == -1) Serial1.println("OSC did not start.");

	delay(20);
}

void loop() {
	// Process USB tasks
	usb.Task();
}
