#include "RotaryEncoder.h"
#include "Config.h"

RotaryEncoder encoder;

// Standard 4x quadrature lookup table (2-bit prev state, 2-bit new state)
static const int8_t QUAD_TABLE[16] = {
   0, -1,  1,  0,
   1,  0,  0, -1,
  -1,  0,  0,  1,
   0,  1, -1,  0
};

static void isrTrampoline() {
  encoder.handleInterrupt();
}

void RotaryEncoder::begin(uint8_t pinA, uint8_t pinB) {
  _pinA = pinA;
  _pinB = pinB;

  pinMode(_pinA, INPUT_PULLUP);
  pinMode(_pinB, INPUT_PULLUP);

  _prevState = (digitalRead(_pinA) << 1) | digitalRead(_pinB);

  attachInterrupt(digitalPinToInterrupt(_pinA), isrTrampoline, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_pinB), isrTrampoline, CHANGE);
}

void RotaryEncoder::handleInterrupt() {
  uint8_t state = (digitalRead(_pinA) << 1) | digitalRead(_pinB);
  uint8_t idx = (_prevState << 2) | state;
  _accumulator += QUAD_TABLE[idx & 0x0F];
  _prevState = state;
}

int RotaryEncoder::readDelta() {
  // 4 ISR transitions per mechanical detent on most encoders
  noInterrupts();
  int8_t acc = _accumulator;
  interrupts();

  int detents = acc / 4;
  if (detents != 0) {
    noInterrupts();
    _accumulator -= detents * 4;
    interrupts();
  }
  return detents;
}
