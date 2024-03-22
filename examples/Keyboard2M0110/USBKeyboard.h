/**
 * USBKeyboard.h
 * Author: Riley Mann
 * 
 * Created on 20 Mar. 2024
 */

#ifndef USB_KEYBOARD_H
#define USB_KEYBOARD_H

#include <hidboot.h>

// enum KeyboardModifiers {
// 	LeftCtrl = 0x01,
// 	LeftShift = 0x02,
// 	Alt = 0x04,
// 	LeftCmd = 0x08,
// 	RightCtrl = 0x10,
// 	RightShift = 0x20,
// 	AltGr = 0x40,
// 	RightCmd = 0x80
// };

enum KeyboardModifiers {
	Ctrl = 0x01,
	Shift = 0x02,
	Alt = 0x04,
	Cmd = 0x08
};

class USBKeyboard : public KeyboardReportParser {
public:
	USBKeyboard(USBHost &usb) : hostKeyboard(&usb), key(0), keyOem(0), modifiers(0) {
		hostKeyboard.SetReportParser(0, this);
	}

	uint8_t getCtrlKeysPressed() {return ctrlp;}
	uint8_t getCtrlKeysReleased() {return ctrlr;}
	uint8_t getKey() {return key;}
	uint8_t getGetModifiers() {return modifiers;}
	uint8_t getOemKey() {return keyOem;}

protected:
	virtual void OnControlKeysChanged(uint8_t before, uint8_t after);
	virtual void OnKeyDown(uint8_t mod, uint8_t key);
	virtual void OnKeyUp(uint8_t mod, uint8_t key);

private:
	HIDBoot<HID_PROTOCOL_KEYBOARD> hostKeyboard;
	uint8_t ctrlp, ctrlr, key, keyOem, modifiers;
};

#endif // USB_KEYBOARD_H
