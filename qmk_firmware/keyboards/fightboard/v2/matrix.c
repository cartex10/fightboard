// Copyright 2022 Carlos Ortiz (@Cartex10)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "mcp23018.h"
#include "i2c_master.h"
#include <math.h>
#ifndef NO_PRINT
#include "print.h"
#endif
#ifndef LAST_LAYER
#define LAST_LAYER 6
#endif

#define MCP_ADDR (0x20)
extern matrix_row_t matrix[MATRIX_ROWS];
extern int LAYER;
extern const int indicatorHSV[LAST_LAYER+2][3];

int LEDHoldCount = 0;
#ifdef SOCD
bool vSOCD = true;
bool hSOCD = true;
#endif

void matrix_init_custom(void) {
	setPinOutput(GP27);
	writePinLow(GP27);
	wait_ms(100);
	togglePin(GP27);

	mcp23018_init(MCP_ADDR);
	mcp23018_set_config(MCP_ADDR, mcp23018_PORTA, ALL_INPUT);
	mcp23018_set_config(MCP_ADDR, mcp23018_PORTB, ALL_INPUT);
	setPinInputHigh(GP28);
}

bool matrix_scan_custom(uint16_t current_matrix[]) {
	bool matrix_has_changed = false;

	uint16_t mcpPins = 0x0000;
	mcp23018_readPins_all(MCP_ADDR, &mcpPins);
	mcpPins = ~mcpPins;

	bool LEDpin = readPin(GP28);
	LEDpin = !LEDpin;

	for (int row = 0; row < 2; row++) {
		int temp;
		switch (row) {
			case 0:
				temp = mcpPins;
				#ifdef SOCD
				if (((temp & 0x00a0) == 0x00a0) && vSOCD) { // SOCD for vertical inputs
					temp = (temp & ~(current_matrix[row] & 0x00a0));
					vSOCD = false;
				}
				else if (((temp & 0x00a0) == 0x00a0)) { temp = temp & (~(~current_matrix[row] & 0x00a0)); }
				else { vSOCD = true; }
				if (((temp & 0x0050) == 0x0050) && hSOCD) { // SOCD for horizontal inputs
					temp = temp & (~(current_matrix[row] & 0x0050));
					hSOCD = false;
				}
				else if (((temp & 0x0050) == 0x0050)) { temp = temp & (~(~current_matrix[row] & 0x0050)); }
				else { hSOCD = true; }
				#endif
				#ifdef SOCD2
				if (((temp & 0x00a0) == 0x00a0)) { // SOCD for vertical inputs
					temp = (temp & ~(temp & 0x0080));
				}
				if (((temp & 0x0050) == 0x0050)) { // SOCD for horizontal inputs
					temp = temp & (~(temp & 0x0050));
				}
				#endif
				// Change all leds if lower layer is changed
				if (LAST_LAYER + 1 == get_highest_layer(layer_state|default_layer_state)) {
					if (((temp | 0xfeff) == 0xffff) || ((temp | 0xfdff) == 0xffff)) {
						HSV layerHSV = { indicatorHSV[LAYER][0], indicatorHSV[LAYER][1], indicatorHSV[LAYER][2] };
						RGB layerRGB = hsv_to_rgb(layerHSV);
						rgb_matrix_set_color_all(layerRGB.r, layerRGB.g, layerRGB.b);
					}
				}
				break;
			case 1:
				temp = LEDpin;
				break;
		}
		if (current_matrix[row] != temp) {
			current_matrix[row] = temp;
			matrix_has_changed = true;
		}
		uprintf("Row #%d = %x\n", row, temp);
	}

	if (LEDpin) { LEDHoldCount += 1; }
	else { LEDHoldCount = 0; }
	if (LEDHoldCount == 3000) { rgb_matrix_toggle(); }
	
	//uprintf("current_matrix = %x\n\n\n\n", *current_matrix);
	return matrix_has_changed;
}
