#include <Arduino.h>

// ===== PIN =====
#define SENSOR_PIN 34
#define POMPA_PIN 18
#define VALVE_PIN 21
#define FLOW_SENSOR 15

// ===== SENSOR DEBIT =====
volatile int pulseCount = 0;
float flowRate;

// ===== SETTING =====
int batasAir = 2000; // sesuaikan hasil kalibrasi sensor

// ===== INTERRUPT SENSOR DEBIT =====
void IRAM_ATTR countPulse() {
  pulseCount++;
}

void setup() {

  Serial.begin(115200);

  pinMode(SENSOR_PIN, INPUT);
  pinMode(POMPA_PIN, OUTPUT);
  pinMode(VALVE_PIN, OUTPUT);

  // sensor debit
  pinMode(FLOW_SENSOR, INPUT_PULLUP);

  // interrupt sensor debit
  attachInterrupt(
    digitalPinToInterrupt(FLOW_SENSOR),
    countPulse,
    FALLING
  );

  // kondisi awal
  digitalWrite(POMPA_PIN, LOW);
  digitalWrite(VALVE_PIN, LOW);
}

void loop() {

  // ===== SENSOR AIR =====
  int nilaiAir = analogRead(SENSOR_PIN);

  Serial.print("Level Air: ");
  Serial.println(nilaiAir);

  // ===== HITUNG DEBIT =====
  pulseCount = 0;

  delay(1000);

  // rumus umum sensor debit
  flowRate = pulseCount / 7.5;

  Serial.print("Debit Air: ");
  Serial.print(flowRate);
  Serial.println(" L/min");

  // ===== KONDISI AIR RENDAH =====
  if (nilaiAir < batasAir) {

    Serial.println("Air RENDAH");

    digitalWrite(POMPA_PIN, HIGH); // pompa ON
    digitalWrite(VALVE_PIN, LOW);  // valve OFF
  }

  // ===== KONDISI AIR TINGGI =====
  else {

    Serial.println("Air TINGGI");

    digitalWrite(POMPA_PIN, LOW);   // pompa OFF
    digitalWrite(VALVE_PIN, HIGH);  // valve ON
  }

  Serial.println("------------------");
}

//Notes: Everything is built using AI
//Additional Note: at first we want to use the gate (indicated by servo), but it's to hard so we decide to delete it
