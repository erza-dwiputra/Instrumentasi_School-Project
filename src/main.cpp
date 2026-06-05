#include <Arduino.h>

// ===== PIN =====
#define SENSOR_PIN    34
#define POMPA_PIN     26
#define VALVE_PIN     17
#define FLOW_SENSOR   15

// ===== KALIBRASI SENSOR MERAH =====
const int THRESHOLD_3CM = 1800;

// ===== SENSOR DEBIT =====
volatile int pulseCount = 0;
float flowRate = 0.0;
const int MIN_PULSE = 3;

// ===== TIMING =====
const unsigned long POMPA_DURASI  = 25000;
const unsigned long VALVE_BUKA_MS = 30000;

// ===== STATE MACHINE =====
enum State { IDLE, POMPA_ON, TUNGGU_3CM, VALVE_OPEN };
State currentState = IDLE;

unsigned long stateTimer    = 0;
unsigned long lastFlowCheck = 0;
unsigned long lastPrint     = 0;

// ===== INTERRUPT =====
void IRAM_ATTR countPulse() {
  pulseCount++;
}

// ===== HELPER =====
// Confirmed dari test: HIGH = relay ON = valve MENUTUP
//                      LOW  = relay OFF = valve MEMBUKA
void setValve(bool tutup) {
  digitalWrite(VALVE_PIN, tutup ? HIGH : LOW);
  Serial.println(tutup ? "[Valve] TUTUP" : "[Valve] BUKA");
}

void setPompa(bool nyala) {
  digitalWrite(POMPA_PIN, nyala ? HIGH : LOW);
  Serial.println(nyala ? "[Pompa] ON" : "[Pompa] OFF");
}

// ===== SETUP =====
void setup() {
  Serial.begin(9600);

  pinMode(SENSOR_PIN,  INPUT);
  pinMode(POMPA_PIN,   OUTPUT);
  pinMode(VALVE_PIN,   OUTPUT);
  pinMode(FLOW_SENSOR, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR), countPulse, FALLING);

  // Kondisi awal: pompa OFF, valve BUKA
  setPompa(false);
  setValve(false);

  currentState = IDLE;
  Serial.println("=== Sistem Ready ===");
  Serial.println("Menunggu aliran air...");
}

// ===== LOOP =====
void loop() {
  unsigned long now = millis();

  // ===== BACA SENSOR RESISTIF =====
  int nilaiAir     = analogRead(SENSOR_PIN);
  bool airSudah3cm = (nilaiAir >= THRESHOLD_3CM);

  // ===== HITUNG DEBIT (non-blocking, setiap 1 detik) =====
  bool adaAliran = false;
  if (now - lastFlowCheck >= 1000) {
    flowRate  = pulseCount / 7.5;
    adaAliran = (pulseCount >= MIN_PULSE);
    Serial.print("[Flow] Pulse: "); Serial.print(pulseCount);
    Serial.print(" | Debit: ");     Serial.print(flowRate);
    Serial.print(" L/min | Aliran: "); Serial.println(adaAliran ? "YA" : "TIDAK");
    pulseCount    = 0;
    lastFlowCheck = now;
  }

  // ===== PRINT STATUS (non-blocking, setiap 500ms) =====
  if (now - lastPrint >= 500) {
    lastPrint = now;
    Serial.print("[Sensor Air] ADC: "); Serial.print(nilaiAir);
    Serial.print(" | 3cm: ");           Serial.println(airSudah3cm ? "YA" : "TIDAK");
    Serial.print("[State]      ");
    switch (currentState) {
      case IDLE:        Serial.println("IDLE");        break;
      case POMPA_ON:    Serial.println("POMPA_ON");    break;
      case TUNGGU_3CM:  Serial.println("TUNGGU_3CM");  break;
      case VALVE_OPEN:  Serial.println("VALVE_OPEN");  break;
    }
    Serial.println("---");
  }

  // ===== STATE MACHINE =====
  switch (currentState) {

    case IDLE:
      if (adaAliran) {
        Serial.println(">> Aliran terdeteksi! Pompa ON + Valve TUTUP");
        setPompa(true);
        setValve(true);
        stateTimer   = now;
        currentState = POMPA_ON;
      }
      break;

    case POMPA_ON:
      if (now - stateTimer >= POMPA_DURASI) {
        Serial.println(">> 20 detik selesai: Pompa OFF | Valve TETAP TUTUP");
        setPompa(false);
        currentState = TUNGGU_3CM;
      }
      break;

    case TUNGGU_3CM:
      if (airSudah3cm) {
        Serial.println(">> Sensor 3cm tersentuh! Valve BUKA");
        setValve(false);
        stateTimer   = now;
        currentState = VALVE_OPEN;
      }
      break;

    case VALVE_OPEN:
      if (now - stateTimer >= VALVE_BUKA_MS) {
        Serial.println(">> Valve buka selesai: Pompa ON + Valve TUTUP");
        setPompa(true);
        setValve(true);
        stateTimer   = now;
        currentState = POMPA_ON;
      }
      break;
  }

  delay(200);
}

//Notes: Everything is built using AI
//Here is the finalize result, this is coming from 3 AI: Blackbox, Claude, and ChatGPT
