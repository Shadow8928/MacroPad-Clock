#pragma once
#include <Arduino.h>

// Minimal interrupt-driven quadrature decoder for a standard
// mechanical rotary encoder (A, B, and an active-low push switch).
class RotaryEncoder {
public:
  void begin(uint8_t pinA, uint8_t pinB);

  // Returns +1, -1, or 0 detents since the last call, and clears
  // the internal accumulator. Call once per loop().
  int readDelta();

  // ISR — must be a static/free function; call via lambda or wrapper
  // registered in begin(). Public so attachInterrupt can reach it.
  void handleInterrupt();

private:
  uint8_t _pinA = 0, _pinB = 0;
  volatile int8_t _accumulator = 0;
  uint8_t _prevState = 0;
};

// Only one encoder instance is needed for this project; a global
// pointer lets the C-style ISR reach the instance's handler.
extern RotaryEncoder encoder;
