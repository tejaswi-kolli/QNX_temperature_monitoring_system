# QNX RTOS Based Real-Time Temperature Monitoring System

A real-time temperature monitoring system built on **QNX Neutrino RTOS** using
native **message-passing IPC**, direct **GPIO register access**, and a
multi-process architecture.  
The system reads temperature from a **DHT11 sensor**, logs data centrally, and
allows user interaction via a CLI process.

---

## 🔧 Hardware & Software Stack

### Hardware
- Raspberry Pi (BCM2711 SoC)
- DHT11 Temperature Sensor
- LED (Threshold indication)

### Software
- QNX Neutrino RTOS 7.x / 8.x
- QNX Momentics Toolchain
- C (POSIX-compliant)

---

## 🧩 System Architecture

The project follows a **microkernel-oriented, multi-process design**:


