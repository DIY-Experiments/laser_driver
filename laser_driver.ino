#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

void setLaser(float value) {
  // Value range: 0.0–1.0
  value = constrain(value, 0.0f, 1.0f);

  // MCP4725: 12 бит, код 0–4095
  uint16_t dacValue =
      static_cast<uint16_t>(value * 4095.0f + 0.5f);

  Serial.print("Laser = ");
  Serial.print(value, 2);
  Serial.print(" | DAC = ");
  Serial.println(dacValue);

  // false — don't save to EEPROM
  dac.setVoltage(dacValue, false);
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("Serial is ready");

  Wire.begin();

  // Regular address MCP4725 — 0x60
  if (!dac.begin(0x60)) {
    Serial.println("❌ MCP4725 not found. Check connection!");

    while (true) {
      delay(10);
    }
  }

  Serial.println("✅ MCP4725 is ready");

  // Set 0 level before run main loop
  setLaser(0.0f);
}

void smoothTransition(float from, float to, int durationMs) {
  const int steps = 50;

  for (int i = 0; i <= steps; i++) {
    float t = (float)i / steps;

    // Smoothstep
    float smooth = t * t * (3.0f - 2.0f * t);

    float value = from + (to - from) * smooth;

    setLaser(value);
    delay(durationMs / steps);
  }
}

// Heartbeat animation
void loop() {
  // First, strong beat
  smoothTransition(0.30f, 0.90f, 450);

  // Smooth deem
  smoothTransition(0.90f, 0.42f, 300);

  // Second, weak beat
  smoothTransition(0.42f, 0.72f, 300);

  // Slowly to background
  smoothTransition(0.72f, 0.30f, 500);

  // Pause before next heartbeat
  delay(700);
}

