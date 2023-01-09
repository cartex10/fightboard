// Copyright 2022 Carlos Ortiz (@Cartex10)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

enum my_keycodes {
	NEXT_LAYER = SAFE_RANGE,
	PREV_LAYER,
	GP_A,
	GP_B,
	GP_X,
	GP_Y,
	GP_LB,
	GP_RB,
	GP_BACK,
	GP_START,
	GP_LS,
	GP_RS,
	GP_UP,
	GP_DOWN,
	GP_LEFT,
	GP_RIGHT,
	GP_LT,
	GP_RT,
	REACTIVE_SIMPLE_RGB,
	REACTIVE_FULL_RGB,
	NEW_SAFE_RANGE,
};

#define LAYOUT( \
	k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c, k0d, k0e, k0f, k10 \
	) { \
	{ k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c, k0d, k0e, k0f, k10 }, \
}
