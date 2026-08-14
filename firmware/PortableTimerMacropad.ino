// ============================================================
// Portable Timer Macropad
// Seeed Studio XIAO RP2040
//
// 6 independent countdown timers, 0.91" I2C OLED, rotary encoder,
// 8 buttons (via PCF8574 I2C expander), active buzzer via 2N3904.
//
// REQUIRED LIBRARIES (install via Library Manager):
//   - Adafruit GFX Library
//   - Adafruit SSD1306
// BOARD: Seeed XIAO RP2040 (install via Arduino-Pico /
//   Earle Philhower's Board Manager URL)
//
// Buttons are wired as a diode-less 2x4 matrix (8 buttons, 6 pins).
// No I2C GPIO expander is used — all 11 usable GPIO on the XIAO are
// committed (see Config.h), so there is no headroom left for future
// RGB LEDs / battery ADC without revisiting this pin budget.
// ============================================================

#include <Wire.h>
#include "Config.h"
#include "Timer.h"
#include "ButtonMatrix.h"
#include "RotaryEncoder.h"
#include "Display.h"
#include "Buzzer.h"

Timer        timers[NUM_TIMERS];
ButtonMatrix buttons;
Display      display;
Buzzer       buzzer;

int  selectedTimer = 0;
bool muted = false;

unsigned long lastDisplayMs = 0;

void setup() {
  Serial.begin(115200);

  Wire.begin();
  Wire.setClock(400000);

  for (int i = 0; i < NUM_TIMERS; i++) {
    timers[i].begin(DEFAULT_TIMER_SECONDS);
  }

  encoder.begin(PIN_ENCODER_A, PIN_ENCODER_B);
  buzzer.begin(PIN_BUZZER);
  buttons.begin();

  if (!display.begin()) {
    Serial.println("WARNING: OLED not detected!");
  }
}

void loop() {
  // 1. Read buttons
  buttons.update();

  if (buttons.wasPressed(BTN_TIMER_1)) selectedTimer = 0;
  if (buttons.wasPressed(BTN_TIMER_2)) selectedTimer = 1;
  if (buttons.wasPressed(BTN_TIMER_3)) selectedTimer = 2;
  if (buttons.wasPressed(BTN_TIMER_4)) selectedTimer = 3;
  if (buttons.wasPressed(BTN_TIMER_5)) selectedTimer = 4;
  if (buttons.wasPressed(BTN_TIMER_6)) selectedTimer = 5;

  if (buttons.wasPressed(BTN_START_PAUSE)) {
    timers[selectedTimer].toggleRunning();
  }
  if (buttons.wasPressed(BTN_MUTE)) {
    muted = !muted;
  }

  // 2. Read rotary encoder — adjusts the currently selected timer
  int delta = encoder.readDelta();
  if (delta != 0) {
    timers[selectedTimer].adjust((long)delta * ENCODER_STEP_SECONDS);
  }

  // 3. Update all six timers (all run in the background regardless
  //    of which one is currently selected/displayed)
  for (int i = 0; i < NUM_TIMERS; i++) {
    timers[i].update();
  }

  // 4. Refresh OLED (rate-limited; only the selected timer is shown)
  unsigned long now = millis();
  if (now - lastDisplayMs >= DISPLAY_REFRESH_MS) {
    lastDisplayMs = now;
    display.render(selectedTimer, timers, NUM_TIMERS, muted);
  }

  // 5. Buzzer — sounds if ANY timer has finished, not just the
  //    selected one, so background alarms are never missed
  bool anyFinished = false;
  for (int i = 0; i < NUM_TIMERS; i++) {
    if (timers[i].isFinished()) { anyFinished = true; break; }
  }
  buzzer.update(anyFinished, muted);
}
