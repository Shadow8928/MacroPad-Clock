#pragma once
#include <Arduino.h>
#include "Config.h"

// Scans a 2x4 diode-less button matrix (8 buttons on 6 GPIO pins).
// Rows are driven LOW one at a time; columns are read with internal
// pull-ups. Safe without diodes because this application only ever
// expects one button pressed at a time — no simultaneous combos are
// required by any feature, so ghosting is not a practical concern.
class ButtonMatrix {
public:
  void begin();

  // Scans the full matrix once and updates debounced state.
  // Call every loop().
  void update();

  // True for exactly one update() cycle when a button transitions
  // from released to pressed (post-debounce).
  bool wasPressed(uint8_t buttonIndex) const;

private:
  static const uint8_t _rowPins[NUM_MATRIX_ROWS];
  static const uint8_t _colPins[NUM_MATRIX_COLS];

  bool _rawState[NUM_BUTTONS];
  bool _stableState[NUM_BUTTONS];
  bool _pressedEdge[NUM_BUTTONS];
  unsigned long _lastChangeMs[NUM_BUTTONS];
};
