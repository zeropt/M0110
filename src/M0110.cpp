/**
 * M0110.cpp
 * Author: Riley Mann
 * 
 * Created on 26 Feb. 2024
 */

#include "M0110.h"

/* Raw Codes */
#define INQUIRY_CMD 0x10
#define INSTANT_CMD 0x14
#define MODEL_CMD   0x16
#define TEST_CMD    0x36

#define ACK_CODE   0x7D
#define NACK_CODE  0x77
#define MODEL_CODE 0x0B

#define KEYPAD             0x79
#define KEYPAD_EXT_KEYDOWN 0x71
#define KEYPAD_EXT_KEYUP   0xF1

/* Protocol Timing */
#define PACKET_SIZE 8
#define RX_CLK_DELAY  200 // us
#define TX_CLK_DELAY  165 // us
#define RX_WAIT_DELAY 840 // us
#define TX_WAIT_DELAY 100 // us
#define TX_POLL_DELAY 2500 // us
#define TX_POLL_N 100

/* SM EVENTS */
#define N0_EVENTS 0
#define DAT_EVENT 1
#define TIM_EVENT 2

/* SM STATES */
#define RXTRIG_STATE 0
#define RXREAD_STATE 1
#define TXTRIG_STATE 2
#define TXBLOC_STATE 3
#define TXWRIT_STATE 4

/* Timer Config */
#define TIM3_PRESCALER 16

/* Timer Function Declarations */

void tc3_init(void);
void tc3_reset(void);
void tc3_enable(void);
void tc3_disable(void);
void tc3_setcount(uint32_t us);
void tc3_setperiod(uint32_t us);

/* Interrupt Handler Declarations */

void DAT_Handler(void);

/* Class Method Implementations */

uint16_t M0110::_dat_pin;
uint16_t M0110::_clk_pin;
uint8_t *M0110::_kbuf;
unsigned int M0110::_kbuf_tail;
unsigned int M0110::_kbuf_used;

M0110::M0110(uint16_t dat_pin, uint16_t clk_pin) {
	_dat_pin = dat_pin;
	_clk_pin = clk_pin;
}

void M0110::begin(void) {
	// Allocate memory to buffer
	_kbuf = (uint8_t *)malloc(M0110_BUFFER_SIZE);

	// Configure pins
	pinMode(_dat_pin, INPUT);
	pinMode(_clk_pin, OUTPUT);
	digitalWrite(_clk_pin, HIGH);

	// Configure Interrupts
	tc3_init();
	attachInterrupt(digitalPinToInterrupt(_dat_pin), DAT_Handler, FALLING);
}

void M0110::end() {
	// Disable Interrupts
	tc3_disable();
	tc3_reset();
	detachInterrupt(digitalPinToInterrupt(_dat_pin));

	// Release pins
	pinMode(_dat_pin, INPUT);
	pinMode(_clk_pin, INPUT);

	// free buffer
	free(_kbuf);
}

size_t M0110::press(uint8_t k) {
	// Get Scan Code
	if (k > 0x99) return 1; // invalid character
	uint8_t scancode = _M0110_AsciiToScanCode[k];
	if (scancode == NULL_CODE) return 1; // null code

	// [0x00 - 0x83] Normal Key
	if (k < 0x84) {
		if (scancode & _SCSHIFT) {
			if (M0110_BUFFER_SIZE - _kbuf_used < 2) return 0; // no room
			_queueKeyCode(((_M0110_AsciiToScanCode[M0110_SHIFT] & 0x7F) << 1) | 0x01);
		} else {
			if (_kbuf_used == M0110_BUFFER_SIZE) return 0; // no room
		}
	}

	// [0x84 - 0x95] Key Pad Key
	else if (k < 0x96) {
		if (M0110_BUFFER_SIZE - _kbuf_used < 2) return 0; // no room
		_queueKeyCode(KEYPAD);
	}

	// [0x96 - 0x99] Key Pad Extended Key
	else {
		if (M0110_BUFFER_SIZE - _kbuf_used < 3) return 0; // no room
		_queueKeyCode(KEYPAD_EXT_KEYDOWN);
		_queueKeyCode(KEYPAD);
	}

	// Queue Scan Code
	_queueKeyCode(((scancode & 0x7F) << 1) | 0x01);
	return 1;
}

size_t M0110::release(uint8_t k) {
	// Get Scan Code
	if (k > 0x99) return 1; // invalid character
	uint8_t scancode = _M0110_AsciiToScanCode[k];
	if (scancode == NULL_CODE) return 1; // null code

	// [0x00 - 0x83] Normal Key
	if (k < 0x84) {
		if (scancode & _SCSHIFT) {
			if (M0110_BUFFER_SIZE - _kbuf_used < 2) return 0; // no room
			_queueKeyCode(((_M0110_AsciiToScanCode[M0110_SHIFT] & 0x7F) << 1) | 0x81);
		} else {
			if (_kbuf_used == M0110_BUFFER_SIZE) return 0; // no room
		}
	}

	// [0x84 - 0x95] Key Pad Key
	else if (k < 0x96) {
		if (M0110_BUFFER_SIZE - _kbuf_used < 2) return 0; // no room
		_queueKeyCode(KEYPAD);
	}

	// [0x96 - 0x99] Key Pad Extended Key
	else {
		if (M0110_BUFFER_SIZE - _kbuf_used < 3) return 0; // no room
		_queueKeyCode(KEYPAD_EXT_KEYUP);
		_queueKeyCode(KEYPAD);
	}

	// Queue Scan Code
	_queueKeyCode(((scancode & 0x7F) << 1) | 0x81);
	return 1;
}

size_t M0110::write(uint8_t k) {
	// Get Scan Code
	if (k > 0x99) return 1; // invalid character
	uint8_t scancode = _M0110_AsciiToScanCode[k];
	if (scancode == NULL_CODE) return 1; // null code

	// [0x00 - 0x83] Normal Key
	if (k < 0x84) {
		if (scancode & _SCSHIFT) {
			if (M0110_BUFFER_SIZE - _kbuf_used < 4) return 0; // no room
			_queueKeyCode(((_M0110_AsciiToScanCode[M0110_SHIFT] & 0x7F) << 1) | 0x01);
			_queueKeyCode(((scancode & 0x7F) << 1) | 0x01);
			_queueKeyCode(((_M0110_AsciiToScanCode[M0110_SHIFT] & 0x7F) << 1) | 0x81);
			_queueKeyCode(((scancode & 0x7F) << 1) | 0x81);
		} else {
			if (M0110_BUFFER_SIZE - _kbuf_used < 2) return 0; // no room
			_queueKeyCode(((scancode & 0x7F) << 1) | 0x01);
			_queueKeyCode(((scancode & 0x7F) << 1) | 0x81);
		}
	}

	// [0x84 - 0x95] Key Pad Key
	else if (k < 0x96) {
		if (M0110_BUFFER_SIZE - _kbuf_used < 4) return 0; // no room
		_queueKeyCode(KEYPAD);
		_queueKeyCode(((scancode & 0x7F) << 1) | 0x01);
		_queueKeyCode(KEYPAD);
		_queueKeyCode(((scancode & 0x7F) << 1) | 0x81);
	}

	// [0x96 - 0x99] Key Pad Extended Key
	else {
		if (M0110_BUFFER_SIZE - _kbuf_used < 6) return 0; // no room
		_queueKeyCode(KEYPAD_EXT_KEYDOWN);
		_queueKeyCode(KEYPAD);
		_queueKeyCode(((scancode & 0x7F) << 1) | 0x01);
		_queueKeyCode(KEYPAD_EXT_KEYUP);
		_queueKeyCode(KEYPAD);
		_queueKeyCode(((scancode & 0x7F) << 1) | 0x81);
	}
	return 1;
}

size_t M0110::write(const uint8_t *buffer, size_t size) {
	size_t n = 0;
	while (size--) {
		if (write(*buffer)) n++;
		else break;
		buffer++;
	}
	return n;
}

/* State Machine */

void M0110::_sm(size_t event) {
	static int sm_state = RXTRIG_STATE;
	static unsigned int clk_counter = 0;
	static uint8_t rxbyte = 0x00;
	static uint8_t txbyte = 0x09;

	switch (sm_state) {
		case RXTRIG_STATE: // Wait for data line trigger
			if (event == DAT_EVENT && digitalRead(_dat_pin) == LOW) { // transition to RXREAD_STATE
				detachInterrupt(digitalPinToInterrupt(_dat_pin));
				tc3_setperiod(RX_CLK_DELAY);
				tc3_setcount(RX_WAIT_DELAY);
				tc3_enable();
				rxbyte = 0x00;
				clk_counter = 0;
				sm_state = RXREAD_STATE;
			}
		break;
		case RXREAD_STATE: // Read byte
			if (event == TIM_EVENT) {
				digitalWrite(_clk_pin, clk_counter % 2); // generate clock signal
				if (clk_counter % 2) { // read data on rising clock signal
					uint8_t rxdata = digitalRead(_dat_pin) & 0x01;
					rxbyte |= rxdata << (7 - (clk_counter >> 1));
				}
				if (clk_counter >= PACKET_SIZE*2 - 1) { // transition to TXWAIT_STATE
					tc3_disable();
					attachInterrupt(digitalPinToInterrupt(_dat_pin), DAT_Handler, RISING);
					// rxbyte = INQUIRY_CMD;
					sm_state = TXTRIG_STATE;
				}
				clk_counter++;
			}
		break;
		case TXTRIG_STATE: // Wait for data line trigger
			if (event == DAT_EVENT && digitalRead(_dat_pin) == HIGH) {
				detachInterrupt(digitalPinToInterrupt(_dat_pin));
				pinMode(_dat_pin, OUTPUT);
				if (rxbyte == INQUIRY_CMD) { // if inquiry, transition to TXBLOC_STATE
					tc3_setperiod(TX_POLL_DELAY);
					tc3_enable();
					txbyte = NULL_CODE;
					clk_counter = 0;
					sm_state = TXBLOC_STATE;
					break;
				}
				// otherwise transition to TXWRIT_STATE
				tc3_setperiod(TX_CLK_DELAY);
				tc3_setcount(TX_WAIT_DELAY);
				tc3_enable();
				if (rxbyte == INSTANT_CMD) txbyte = _dequeueKeyCode();
				else if (rxbyte == MODEL_CMD) txbyte = MODEL_CODE;
				else if (rxbyte == TEST_CMD) txbyte = ACK_CODE;
				else txbyte = NULL_CODE;
				clk_counter = 0;
				sm_state = TXWRIT_STATE;
			}
		break;
		case TXBLOC_STATE: // Poll key code for 0.25s max
			if (event == TIM_EVENT) {
				txbyte = _dequeueKeyCode();
				if (txbyte != NULL_CODE || clk_counter >= TX_POLL_N) { // transition to TXWRIT_STATE
					tc3_setperiod(TX_CLK_DELAY);
					clk_counter = 0;
					sm_state = TXWRIT_STATE;
				} else
				clk_counter++;
			}
		break;
		case TXWRIT_STATE: // Write byte
			if (event == TIM_EVENT) {
				digitalWrite(_clk_pin, clk_counter % 2); // generate clock signal
				if ((clk_counter + 1) % 2) { // write data on falling clock edge
					uint8_t txdata = (txbyte >> (7 - (clk_counter >> 1))) & 0x01;
					digitalWrite(_dat_pin, txdata);
				}
				if (clk_counter >= PACKET_SIZE*2 - 1) { // transition to RXWAIT_STATE
					tc3_disable();
					pinMode(_dat_pin, INPUT);
					attachInterrupt(digitalPinToInterrupt(_dat_pin), DAT_Handler, FALLING);
					sm_state = RXTRIG_STATE;
				}
				clk_counter++;
			}
		break;
		default:
			digitalWrite(_clk_pin, HIGH); // clock at idle
			pinMode(_dat_pin, INPUT); // read from data line
			attachInterrupt(digitalPinToInterrupt(_dat_pin), DAT_Handler, FALLING);
			sm_state = RXTRIG_STATE;
	}
}

/* Buffer Functions */

void M0110::_queueKeyCode(uint8_t keycode) {
	if (_kbuf_used < M0110_BUFFER_SIZE) { // queue if not full
		_kbuf[(_kbuf_tail + _kbuf_used) % M0110_BUFFER_SIZE] = keycode;
		_kbuf_used++;
	}
}

uint8_t M0110::_dequeueKeyCode(void) {
	if (_kbuf_used == 0) return NULL_CODE; // if empty, return NULL_CODE
	uint8_t keycode = _kbuf[_kbuf_tail];
	_kbuf_used--;
	_kbuf_tail++;
	_kbuf_tail %= M0110_BUFFER_SIZE;
	return keycode;
}

/* Timer Functions */

void tc3_init() {
	// Select clock generator TC3
	GCLK->CLKCTRL.reg = (uint16_t)(GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TCC2_TC3)) ;
	while (GCLK->STATUS.bit.SYNCBUSY);

	tc3_reset();

	// Set 16-bit mode and match freq waveform
	TC3->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_MFRQ;

	// Set prescaler [DIV1,2,4,8,16,64,256,1024]
	#if TIM3_PRESCALER == 1
		TC3->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1; // | TC_CTRLA_ENABLE;
	#elif TIM3_PRESCALER == 2
		TC3->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV2; // | TC_CTRLA_ENABLE;
	#elif TIM3_PRESCALER == 4
		TC3->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV4; // | TC_CTRLA_ENABLE;
	#elif TIM3_PRESCALER == 8
		TC3->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV8; // | TC_CTRLA_ENABLE;
	#elif TIM3_PRESCALER == 16
		TC3->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV16; // | TC_CTRLA_ENABLE;
	#elif TIM3_PRESCALER == 64
		TC3->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV64; // | TC_CTRLA_ENABLE;
	#elif TIM3_PRESCALER == 256
		TC3->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV256; // | TC_CTRLA_ENABLE;
	#elif TIM3_PRESCALER == 1024
		TC3->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024; // | TC_CTRLA_ENABLE;
	#else
		#error Valid values for TIM3_PRESCALER are 1, 2, 4, 8, 16, 64, 256, or 1024
	#endif

	while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);

	// Set IRQ
	NVIC_DisableIRQ(TC3_IRQn);
	NVIC_ClearPendingIRQ(TC3_IRQn);
	NVIC_SetPriority(TC3_IRQn, 0);
	NVIC_EnableIRQ(TC3_IRQn);

	// Enable interrupt
	TC3->COUNT16.INTENSET.bit.MC0 = 1;
	while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);
}

void tc3_reset() {
	TC3->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
	while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);
	while (TC3->COUNT16.CTRLA.bit.SWRST);
}

void tc3_enable() {
	TC3->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
	while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);
}

void tc3_disable() {
	TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
	while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);
}

void tc3_setcount(uint32_t us) {
	TC3->COUNT16.COUNT.reg = 65536 - (uint16_t)round(us * (SystemCoreClock / (TIM3_PRESCALER * 1e6)));
}

void tc3_setperiod(uint32_t us) {
	// Set compare register
	TC3->COUNT16.CC[0].reg = (uint16_t)round(us * (SystemCoreClock / (TIM3_PRESCALER * 1e6))) - 1;
	while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);
}

/* Interrupt Handlers */

void DAT_Handler() {
	M0110::_sm(DAT_EVENT);
}

void TC3_Handler() {
	M0110::_sm(TIM_EVENT);
	TC3->COUNT16.INTFLAG.bit.MC0 = 1; // clear interrupt
}
