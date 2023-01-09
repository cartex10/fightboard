// 14 Buttons
typedef struct {
	uint8_t index;
	uint8_t mask;
} XInputMap_Button;

void newButton(XInputMap_Button *butt, uint8_t i, uint8_t o) {
	butt->index = index(i);
	butt->mask = BuildMask(o);
}
uint8_t BuildMask(uint8_t offset) {
	return (1 << offset);  // Bitmask of bit to flip
}

XInputMap_Button Map_DpadUp(2, 0);
XInputMap_Button Map_DpadDown(2, 1);
XInputMap_Button Map_DpadLeft(2, 2);
XInputMap_Button Map_DpadRight(2, 3);
XInputMap_Button Map_ButtonStart(2, 4);
XInputMap_Button Map_ButtonBack(2, 5);
XInputMap_Button Map_ButtonL3(2, 6);
XInputMap_Button Map_ButtonR3(2, 7);

XInputMap_Button Map_ButtonLB(3, 0);
XInputMap_Button Map_ButtonRB(3, 1);
XInputMap_Button Map_ButtonA(3, 4);
XInputMap_Button Map_ButtonB(3, 5);
XInputMap_Button Map_ButtonX(3, 6);
XInputMap_Button Map_ButtonY(3, 7);

XInputMap_Button * getButtonFromEnum(int ctrl) {
	switch (ctrl) {
	case(DPAD_UP):      return &Map_DpadUp;
	case(DPAD_DOWN):    return &Map_DpadDown;
	case(DPAD_LEFT):    return &Map_DpadLeft;
	case(DPAD_RIGHT):   return &Map_DpadRight;
	case(BUTTON_A):     return &Map_ButtonA;
	case(BUTTON_B):     return &Map_ButtonB;
	case(BUTTON_X):     return &Map_ButtonX;
	case(BUTTON_Y):     return &Map_ButtonY;
	case(BUTTON_LB):    return &Map_ButtonLB;
	case(BUTTON_RB):    return &Map_ButtonRB;
	case(BUTTON_L3):    return &Map_ButtonL3;
	case(BUTTON_R3):    return &Map_ButtonR3;
	case(BUTTON_START): return &Map_ButtonStart;
	case(BUTTON_BACK):  return &Map_ButtonBack;
	default: return nullptr;
	}
}

// 2 Triggers

typedef struct {
	struct Range range;
	uint8_t index;
} XInputMap_Trigger;

void newTrigger(XInputMap_Trigger *trig, uint8_t i) {
	trig->index = index(i);
}

struct Range range = { 0, 255 };  // uint8_t

XInputMap_Trigger Map_TriggerLeft(4);
XInputMap_Trigger Map_TriggerRight(5);

XInputMap_Trigger * getTriggerFromEnum(int ctrl) {
	switch (ctrl) {
	case(TRIGGER_LEFT): return &Map_TriggerLeft;
	case(TRIGGER_RIGHT): return &Map_TriggerRight;
	default: return nullptr;
	}
}

// --------------------------------------------------------
// XInputController Class (API)                           |
// --------------------------------------------------------

void XInputController(void) :
	tx(), rumble() // Zero initialize arrays
{
	reset();
}

void begin(void) {
	// Empty for now
}

void press(uint8_t button) {
	setButton(button, true);
}

void release(uint8_t button) {
	setButton(button, false);
}

void setButton(uint8_t button, bool state) {
	XInputMap_Button * buttonData = getButtonFromEnum((int) button);
	if (buttonData != nullptr) {
		if (getButton(button) == state) return;  // Button hasn't changed

		if (state) { tx[buttonData->index] |= buttonData->mask; }  // Press
		else { tx[buttonData->index] &= ~(buttonData->mask); }  // Release
		newData = true;
		autosend();
	}
	else {
		struct Range * triggerRange = getRangeFromEnum((int) button);
		if (triggerRange == nullptr) return;  // Not a trigger (or joystick, but the trigger function will ignore that)
		setTrigger((int) button, state ? triggerRange->max : triggerRange->min);  // Treat trigger like a button
	}
}

void setDpad(int pad, bool state) {
	setButton(pad, state);
}

void setDpadMask(bool up, bool down, bool left, bool right) {
	const boolean autoSendTemp = autoSendOption;  // Save autosend state
	autoSendOption = false;  // Disable temporarily

	setDpad(DPAD_UP, up);
	setDpad(DPAD_DOWN, down);
	setDpad(DPAD_LEFT, left);
	setDpad(DPAD_RIGHT, right);

	autoSendOption = autoSendTemp;  // Re-enable from option
	autosend();
}

void setTrigger(int trigger, int32_t val) {
	const XInputMap_Trigger * triggerData = getTriggerFromEnum(trigger);
	if (triggerData == nullptr) return;  // Not a trigger

	val = rescaleInput(val, *getRangeFromEnum(trigger), XInputMap_Trigger::range);
	if (getTrigger(trigger) == val) return;  // Trigger hasn't changed

	tx[triggerData->index] = val;
	newData = true;
	autosend();
}

void releaseAll() {
	const uint8_t offset = 2;  // Skip message type and packet size
	memset(tx + offset, 0x00, sizeof(tx) - offset);  // Clear TX array
	newData = true;  // Data changed and is unsent
	autosend();
}

void setAutoSend(bool a) {
	autoSendOption = a;
}

boolean getButton(uint8_t button) {
	const XInputMap_Button* buttonData = getButtonFromEnum((int) button);
	if (buttonData != nullptr) {
		return tx[buttonData->index] & buttonData->mask;
	}
	const XInputMap_Trigger* triggerData = getTriggerFromEnum((int) button);
	if (triggerData != nullptr) {
		return getTrigger((int) button) != 0 ? 1 : 0;
	}
	return 0;  // Not a button or a trigger
}

boolean getDpad(int dpad) {
	return getButton(dpad);
}

uint8_t getTrigger(int trigger) {
	const XInputMap_Trigger * triggerData = getTriggerFromEnum(trigger);
	if (triggerData == nullptr) return 0;  // Not a trigger
	return tx[triggerData->index];
}

uint8_t getPlayer() {
	return player;
}

struct Range * getRangeFromEnum(int ctrl) {
	switch (ctrl) {
	case(TRIGGER_LEFT): return &rangeTrigLeft;
	case(TRIGGER_RIGHT): return &rangeTrigRight;
	default: return nullptr;
	}
}

int32_t rescaleInput(int32_t val, struct Range& in, struct Range& out) {
	if (val <= in.min) return out.min;  // Out of range -
	if (val >= in.max) return out.max;  // Out of range +
	if (in.min == out.min && in.max == out.max) return val;  // Ranges identical
	return map(val, in.min, in.max, out.min, out.max);
}

int16_t invertInput(int16_t val, struct Range& range) {
	return range.max - val + range.min;
}

void setTriggerRange(int32_t rangeMin, int32_t rangeMax) {
	setRange(TRIGGER_LEFT, rangeMin, rangeMax);
	setRange(TRIGGER_RIGHT, rangeMin, rangeMax);
}

void setRange(int ctrl, int32_t rangeMin, int32_t rangeMax) {
	if (rangeMin >= rangeMax) return;  // Error: Max < Min

	struct Range * range = getRangeFromEnum(ctrl);
	if (range == nullptr) return;  // Not an addressable range

	range->min = rangeMin;
	range->max = rangeMax;
}

// Resets class back to initial values
void reset() {
	// Reset control data (tx)
	releaseAll();  // Clear TX buffer
	tx[0] = 0x00;  // Set tx message type
	tx[1] = 0x14;  // Set tx packet size (20)

	// Reset received data (rx)
	player = 0;  // Not connected, no player

	// Clear user-set options
	recvCallback = nullptr;
	autoSendOption = true;
}

void fillBuffer(char* buff, const char fill) {
	uint8_t i = 0;
	while (true) {
		if (buff[i] == 0) break;
		buff[i] = fill;
		i++;
	}
}

/*void printDebug(Print &output) const {
	const char fillCharacter = '_';
	char buffer[34];

	output.print("XInput Debug: ");

	// Left Side Controls
	char leftBumper[3] = "LB";
	char leftJoyBtn[3] = "L3";

	if (!getButton(BUTTON_LB)) fillBuffer(leftBumper, fillCharacter);
	if (!getButton(BUTTON_L3)) fillBuffer(leftJoyBtn, fillCharacter);

	sprintf(buffer,
		"LT: %3u %s L:(%s)",

		getTrigger(TRIGGER_LEFT),
		leftBumper,
		leftJoyBtn
	);
	output.print(buffer);

	// Face Buttons
	const char dpadLPrint = getButton(DPAD_LEFT)  ? '<' : fillCharacter;
	const char dpadUPrint = getButton(DPAD_UP)    ? '^' : fillCharacter;
	const char dpadDPrint = getButton(DPAD_DOWN)  ? 'v' : fillCharacter;
	const char dpadRPrint = getButton(DPAD_RIGHT) ? '>' : fillCharacter;

	const char aButtonPrint = getButton(BUTTON_A) ? 'A' : fillCharacter;
	const char bButtonPrint = getButton(BUTTON_B) ? 'B' : fillCharacter;
	const char xButtonPrint = getButton(BUTTON_X) ? 'X' : fillCharacter;
	const char yButtonPrint = getButton(BUTTON_Y) ? 'Y' : fillCharacter;

	const char startPrint = getButton(BUTTON_START) ? '>' : fillCharacter;
	const char backPrint  = getButton(BUTTON_BACK)  ? '<' : fillCharacter;

	sprintf(buffer,
		" %c%c%c%c | %c%c | %c%c%c%c ",

		dpadLPrint, dpadUPrint, dpadDPrint, dpadRPrint,
		backPrint, startPrint,
		aButtonPrint, bButtonPrint, xButtonPrint, yButtonPrint
	);
	output.print(buffer);

	// Right Side Controls
	char rightBumper[3] = "RB";
	char rightJoyBtn[3] = "R3";

	if (!getButton(BUTTON_RB)) fillBuffer(rightBumper, fillCharacter);
	if (!getButton(BUTTON_R3)) fillBuffer(rightJoyBtn, fillCharacter);

	sprintf(buffer,
		"R:(%s) %s RT: %3u",
		
		rightJoyBtn,
		rightBumper,
		getTrigger(TRIGGER_RIGHT)
	);
	output.println(buffer);
}*/