#include "ButtonMatrix.h"

const uint8_t ButtonMatrix::_rowPins[NUM_MATRIX_ROWS] = { PIN_MATRIX_ROW0, PIN_MATRIX_ROW1 };
const uint8_t ButtonMatrix::_colPins[NUM_MATRIX_COLS] = { PIN_MATRIX_COL0, PIN_MATRIX_COL1,
                                                            PIN_MATRIX_COL2, PIN_MATRIX_COL3 };

void ButtonMatrix::begin() {
  for (uint8_t r = 0; r < NUM_MATRIX_ROWS; r++) {
    pinMode(_rowPins[r], OUTPUT);
    digitalWrite(_rowPins[r], HIGH);   // idle inactive
  }
  for (uint8_t c = 0; c < NUM_MATRIX_COLS; c++) {
    pinMode(_colPins[c], INPUT_PULLUP);
  }
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    _rawState[i] = _stableState[i] = false; // false = released
    _pressedEdge[i] = false;
    _lastChangeMs[i] = 0;
  }
}

void ButtonMatrix::update() {
  unsigned long now = millis();

  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    _pressedEdge[i] = false;
  }

  for (uint8_t r = 0; r < NUM_MATRIX_ROWS; r++) {
    // Drive only this row LOW; all other rows stay HIGH (inactive)
    digitalWrite(_rowPins[r], LOW);
    delayMicroseconds(20); // let the line settle

    for (uint8_t c = 0; c < NUM_MATRIX_COLS; c++) {
      uint8_t idx = r * NUM_MATRIX_COLS + c;
      bool pressedNow = (digitalRead(_colPins[c]) == LOW); // active low

      if (pressedNow != _rawState[idx]) {
        _lastChangeMs[idx] = now;
        _rawState[idx] = pressedNow;
      }

      if (now - _lastChangeMs[idx] >= DEBOUNCE_MS) {
        if (_stableState[idx] != pressedNow) {
          bool wasReleased = !_stableState[idx];
          _stableState[idx] = pressedNow;
          if (wasReleased && pressedNow) {
            _pressedEdge[idx] = true;
          }
        }
      }
    }

    digitalWrite(_rowPins[r], HIGH);
  }
}

bool ButtonMatrix::wasPressed(uint8_t buttonIndex) const {
  if (buttonIndex >= NUM_BUTTONS) return false;
  return _pressedEdge[buttonIndex];
}
