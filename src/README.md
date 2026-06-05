# 💧 Automated Water Filling System — ESP32

An Arduino-based embedded system for **automatic water level detection and pump-valve control** using an ESP32 microcontroller. Designed for smart water filling applications such as tanks, containers, or agricultural reservoirs.

---

## 📋 Overview

This firmware implements a **finite state machine (FSM)** that automates the process of filling a container with water. It monitors incoming water flow using a flow sensor, activates a pump to fill the container, waits until the water reaches a target level (3 cm), then opens a valve to release or distribute the water — all without blocking execution.

---

## ⚙️ Hardware Requirements

| Component | Pin | Notes |
|---|---|---|
| Resistive Water Level Sensor | GPIO 34 | Analog input (ADC) |
| Water Pump (Relay) | GPIO 26 | Digital output |
| Solenoid Valve (Relay) | GPIO 17 | Digital output |
| Flow Sensor (YF-S201 or similar) | GPIO 15 | Interrupt-driven, INPUT_PULLUP |

> **Relay logic:** `HIGH` = relay ON = valve **CLOSED** / pump **ON**; `LOW` = relay OFF = valve **OPEN** / pump **OFF**

---

## 🔁 State Machine

The system operates through four states:

```
IDLE ──(flow detected)──► POMPA_ON ──(25s elapsed)──► TUNGGU_3CM ──(water at 3cm)──► VALVE_OPEN
                                ▲                                                           │
                                └────────────────────(30s elapsed)─────────────────────────┘
```

| State | Description |
|---|---|
| `IDLE` | Waiting for water flow. Pump OFF, Valve OPEN. |
| `POMPA_ON` | Flow detected. Pump ON, Valve CLOSED. Runs for 25 seconds. |
| `TUNGGU_3CM` | Pump OFF. Waiting for water sensor to reach 3 cm threshold. |
| `VALVE_OPEN` | Valve OPEN for 30 seconds, then cycles back to `POMPA_ON`. |

---

## 📡 Sensor Details

### Flow Sensor (Pulse Counter)
- Uses a hardware interrupt (`FALLING` edge) on GPIO 15 to count pulses.
- Flow rate calculated every **1 second**: `flowRate = pulseCount / 7.5` (L/min).
- A minimum of **3 pulses/second** is required to confirm active flow.

### Resistive Water Level Sensor
- Read via ADC on GPIO 34.
- Threshold for 3 cm water level: **ADC ≥ 1800**.
- Used to trigger the transition from `TUNGGU_3CM` → `VALVE_OPEN`.

---

## ⏱️ Timing Configuration

| Constant | Value | Purpose |
|---|---|---|
| `POMPA_DURASI` | 25,000 ms | Duration pump stays ON before shutting off |
| `VALVE_BUKA_MS` | 30,000 ms | Duration valve stays OPEN before next pump cycle |

---

## 📤 Serial Output

The system outputs real-time diagnostics at **9600 baud** via Serial Monitor:

- Flow pulse count and calculated flow rate (every 1 second)
- Water level ADC value and 3 cm flag (every 500 ms)
- Current FSM state (every 500 ms)
- State transition events with timestamps

---

## 🚀 Getting Started

1. Open the project in [PlatformIO](https://platformio.org/) or Arduino IDE with ESP32 board support.
2. Connect hardware according to the pin table above.
3. Calibrate `THRESHOLD_3CM` based on your sensor's actual ADC readings at 3 cm depth.
4. Upload the firmware and open the Serial Monitor at **9600 baud**.

---

## 📁 File Structure

```
├── src/
│   └── main.cpp       # Main firmware source
├── platformio.ini     # PlatformIO configuration (if applicable)
└── README.md
```

---

## 📝 Notes

- All timing is **non-blocking** using `millis()` — no `delay()` is used inside the FSM logic (only a 200 ms debounce delay at the end of `loop()`).
- The flow sensor pulse counter uses `IRAM_ATTR` to place the ISR in IRAM for reliable interrupt handling on ESP32.
- Relay logic is **active HIGH** — double-check your relay module before wiring.

---

## 📜 License

MIT License — free to use and modify for personal or educational projects.
