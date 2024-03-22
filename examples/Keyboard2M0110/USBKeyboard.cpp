/**
 * USBKeyboard.cpp
 * Author: Riley Mann
 * 
 * Created on 20 Mar. 2024
 */

#include "USBKeyboard.h"

extern "C" {
	void __USBKeyboardEmptyCallback() {}
}

void controlKeysPressed() __attribute__ ((weak, alias("__USBKeyboardEmptyCallback")));
void controlKeysReleased() __attribute__ ((weak, alias("__USBKeyboardEmptyCallback")));
void keyPressed() __attribute__ ((weak, alias("__USBKeyboardEmptyCallback")));
void keyReleased() __attribute__ ((weak, alias("__USBKeyboardEmptyCallback")));

void USBKeyboard::OnControlKeysChanged(uint8_t _before, uint8_t _after) {
	// Combine left and right mod keys
	uint8_t before = (_before & 0x0F) | ((_before >> 4) & 0x0F);
	uint8_t after = (_after & 0x0F) | ((_after >> 4) & 0x0F);

	// Check for transitions
	ctrlp = after & ~before;
	ctrlr = before & ~after;

	// Callbacks
	if (ctrlp) controlKeysPressed();
	if (ctrlr) controlKeysReleased();
}

void USBKeyboard::OnKeyDown(uint8_t _mod, uint8_t _oemKey) {
	modifiers = _mod;
	keyOem = _oemKey;
	key = OemToAscii(_mod, _oemKey);
	keyPressed();
}

void USBKeyboard::OnKeyUp(uint8_t _mod, uint8_t _oemKey) {
	modifiers = _mod;
	keyOem = _oemKey;
	key = OemToAscii(_mod, _oemKey);
	keyReleased();
}
