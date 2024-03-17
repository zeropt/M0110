/**
 * SerialCtrl.ino
 * Author: Riley Mann
 * 
 * Created on 4 Mar. 2024
 */
#define M0110_TEST
#ifdef M0110_TEST

#include <Arduino.h>
#include "M0110.h"

#define OPTION_BTN 10
#define COMMAND_BTN 9

int option_btn_state = HIGH;
int command_btn_state = HIGH;

void setup() {
	pinMode(OPTION_BTN, INPUT_PULLUP);
	pinMode(COMMAND_BTN, INPUT_PULLUP);
	Serial.begin(115200);
	M0110.begin();
}

uint8_t parseCSI(uint8_t k) {
	static int state = 0;
	uint8_t kout = k;
	switch (state) {
		case 0:
			if (k == '\033') {
				kout = 0x00;
				state++;
			}
		break;
		case 1:
			if (k == '[') {
				kout = 0x00;
				state++;
			} else state = 0;
		break;
		case 2:
			if (k == 'A') kout = M0110_UP_ARROW;
			else if (k == 'B') kout = M0110_DOWN_ARROW;
			else if (k == 'C') kout = M0110_RIGHT_ARROW;
			else if (k == 'D') kout = M0110_LEFT_ARROW;
			else kout = 0x00;
			state = 0;
		break;
	}
	return kout;
}

void loop() {
	if (Serial.available() > 0) {
		uint8_t k = Serial.read();
		k = parseCSI(k);
		switch (k) {
			case 0x00:
			break;
			case 0x7F:
				M0110.write(M0110_BACKSPACE);
			break;
			default:
				M0110.write(k);
		}
		Serial.println(k, HEX);
	}
	if (option_btn_state == HIGH && digitalRead(OPTION_BTN) == LOW) {
		M0110.press(M0110_OPTION);
		Serial.println("OPTION DOWN");
	}
	if (option_btn_state == LOW && digitalRead(OPTION_BTN) == HIGH) {
		M0110.release(M0110_OPTION);
		Serial.println("OPTION UP");
	}
	if (command_btn_state == HIGH && digitalRead(COMMAND_BTN) == LOW) {
		M0110.press(M0110_COMMAND);
		Serial.println("COMMAND DOWN");
	}
	if (command_btn_state == LOW && digitalRead(COMMAND_BTN) == HIGH) {
		M0110.release(M0110_COMMAND);
		Serial.println("COMMAND UP");
	}
	option_btn_state = digitalRead(OPTION_BTN);
	command_btn_state = digitalRead(COMMAND_BTN);
	delay(10);
}

#endif // M0110_TEST