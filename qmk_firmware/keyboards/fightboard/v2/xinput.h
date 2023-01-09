//#include <Arduino.h>
// With help from https://github.com/dmadison/ArduinoXInput
enum XInputControl {
	BUTTON_LOGO = 0,
	BUTTON_A = 1,
	BUTTON_B = 2,
	BUTTON_X = 3,
	BUTTON_Y = 4,
	BUTTON_LB = 5,
	BUTTON_RB = 6,
	BUTTON_BACK = 7,
	BUTTON_START = 8,
	BUTTON_L3 = 9,
	BUTTON_R3 = 10,
	DPAD_UP = 11,
	DPAD_DOWN = 12,
	DPAD_LEFT = 13,
	DPAD_RIGHT = 14,
	TRIGGER_LEFT = 15,
	TRIGGER_RIGHT = 16,
	JOY_LEFT,
	JOY_RIGHT,
};

enum XInputReceiveType{
	Rumble = 0x00,
	LEDs = 0x01,
};

void XInputController(void);
void begin(void);

// Set Control Surfaces
void press(uint8_t button);
void release(uint8_t button);
void setButton(uint8_t button, bool state);

void setDpad(int pad, bool state);
void setDpadMask(bool up, bool down, bool left, bool right);

void setTrigger(int trigger, int32_t val);

void releaseAll(void);

// Auto-Send Data
void setAutoSend(bool a);

// Read Control Surfaces
bool getButton(uint8_t button);
bool getDpad(int dpad);
uint8_t getTrigger(int trigger);

// Received Data
uint8_t getPlayer(void);  // Player # assigned to the controller (0 is unassigned)

// Received Data Callback
uint8_t RecvCallbackType = 0x00;

// Control Input Ranges
struct Range { int32_t min; int32_t max; };

void setTriggerRange(int32_t rangeMin, int32_t rangeMax);
void setRange(int ctrl, int32_t rangeMin, int32_t rangeMax);

// Setup
void reset(void);

// Debug
//void printDebug(Print output=Serial);

// Sent Data
uint8_t tx[20];  // USB transmit data
bool newData;  // Flag for tx data changed
bool autoSendOption;  // Flag for automatically sending data

// Received Data
uint8_t player;  // Gamepad player #, buffered
uint8_t recvCallback;  // User-set callback for received data

// Control Input Ranges
struct Range rangeTrigLeft, rangeTrigRight;
struct Range * getRangeFromEnum(int ctrl);
int32_t rescaleInput(int32_t val, struct Range in, struct Range out);
int16_t invertInput(int16_t val, struct Range range);