#pragma once

// ============================================================
// PIN MAP  (XIAO RP2040 — 11 usable GPIO, ALL 11 used, 0 free)
// ============================================================
// I2C bus for OLED (SSD1306) only — no expander needed
#define PIN_SDA           4   // D4
#define PIN_SCL           5   // D5

// Rotary encoder — direct GPIO, interrupt driven (no push switch;
// not used by the current feature set)
#define PIN_ENCODER_A     0   // D0
#define PIN_ENCODER_B     1   // D1

// Button matrix — 2 rows x 4 columns = 8 buttons on 6 pins.
// No diodes: safe because buttons are only ever pressed one at a
// time in this application (timer select / start-pause / mute).
#define PIN_MATRIX_ROW0   2   // D2
#define PIN_MATRIX_ROW1   3   // D3
#define PIN_MATRIX_COL0   6   // D6
#define PIN_MATRIX_COL1   7   // D7
#define PIN_MATRIX_COL2   8   // D8
#define PIN_MATRIX_COL3   9   // D9

// Buzzer — GPIO -> 1k base resistor -> 2N3904 base -> buzzer switched to GND
#define PIN_BUZZER        10  // D10

// NOTE: all 11 usable GPIO are now committed. No pins remain free
// for future RGB LEDs / battery ADC — revisit if you need headroom.

// ============================================================
// I2C ADDRESSES
// ============================================================
#define OLED_I2C_ADDR     0x3C

// ============================================================
// DISPLAY
// ============================================================
#define OLED_WIDTH        128
#define OLED_HEIGHT       32

// ============================================================
// BUTTON MAP (matrix index = row * NUM_MATRIX_COLS + col)
//   Row0: TIMER_1 TIMER_2 TIMER_3 TIMER_4
//   Row1: TIMER_5 TIMER_6 START_PAUSE MUTE
// ============================================================
#define NUM_MATRIX_ROWS   2
#define NUM_MATRIX_COLS   4

#define BTN_TIMER_1       0
#define BTN_TIMER_2       1
#define BTN_TIMER_3       2
#define BTN_TIMER_4       3
#define BTN_TIMER_5       4
#define BTN_TIMER_6       5
#define BTN_START_PAUSE   6
#define BTN_MUTE          7
#define NUM_BUTTONS       8
#define NUM_TIMERS        6

// ============================================================
// TIMING / BEHAVIOR
// ============================================================
#define DEFAULT_TIMER_SECONDS   (60UL * 60UL)   // 60 minutes
#define ENCODER_STEP_SECONDS    60               // 1 minute per detent
#define DEBOUNCE_MS             25
#define DISPLAY_REFRESH_MS      100
#define BUZZER_ON_MS            300
#define BUZZER_OFF_MS           700
