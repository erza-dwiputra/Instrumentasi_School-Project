# Instrumentasi_School-Project
Documetation for ESP32 code_built for model of movable Weir Gates and Sensors

## 📌 Key System Features
* **Water Level Monitoring:** Real-time tracking of upstream and downstream water levels.
* **Flow Rate Measurement:** Monitoring discharge or flow volume passing through the channel.
* **Automated Inflation & Deflation:** Controlling pneumatic/hydraulic mechanics via a micro-controller to inflate or deflate the rubber body based on real-time sensor data.
* **High-Voltage Isolation:** Safely switching higher loads (such as pumps) using a relay module.

## 🛠️ Hardware & Components Used
Based on the project design, the following instrumentation components are integrated into the system:

1. **Mini Water Pump**
   Functions as a water pump to simulate river flow and circulation within the miniature model channel.
2. **ESP32 Microcontroller**
   Serves as the central automation control unit. It receives data from the water level and flow sensors, processes the inputs, and triggers the actuators (pump, relay, etc.).
3. **Water Level Sensor**
   Used to detect changes in water elevation and surface level inside the miniature canal.
4. **5V Relay Module**
   Acts as an electronic switch to connect or disconnect higher-voltage power lines safely (e.g., driving the water pump).
5. **Jumper Wires**
   Used to establish electrical connections between components such as the ESP32, sensors, relay, and actuators.
6. **Breadboard**
   Serves as a temporary prototyping board to construct and test the electronic circuit without needing permanent soldering.
7. **Flow Sensor**
   Functions as a detector to measure the fluid discharge and water flow rate entering the channel.
8. **Solenoid Valve**
   Acts as an electronically controlled valve or "faucet" to hold back, regulate, or release the water/air inside the rubber dam model.

## 📁 Repository Structure
* `/src` : Contains the main source code (.cpp) for sensor processing and actuator controls.
* `/hardware` : Schematic diagrams, circuit layouts, and component wiring guides.
