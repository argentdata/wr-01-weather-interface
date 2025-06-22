# WR-01 Weather Sensor Interface

The **Argent Data Systems WR-01** is a compact wind and rain sensor with robust digital outputs for embedded and industrial applications. This repository contains example code and documentation for using the WR-01 with Arduino, Raspberry Pi, and other platforms.

## Features

- Wind speed and direction (instantaneous and averaged)
- Rainfall tracking with hourly, daily, and total accumulation
- Fault detection for vane and anemometer
- Configurable alarm output
- 3.3V TTL serial and RS-485 (Modbus RTU)
- Optional JSON or CSV automatic outputs
- 12-bit analog output (voltage proportional to wind or other parameter)

---

## Contents

examples/
+-- arduino/ # Arduino sketches using Modbus or JSON
+-- raspberry-pi/ # Python scripts for serial reading
docs/
+-- WR-01_Manual.pdf # Full user manual

---

## ?? Example JSON Output

```json
{
  "instant": { "wind_dir": 90, "wind_speed": 9 },
  "2min":    { "wind_dir": 91, "wind_speed": 8.4 },
  "10min":   { "wind_dir": 91, "wind_speed": 7.6 },
  "custom":  { "wind_dir": 91, "wind_speed": 6.6 },
  "rain_1hr": 0.27,
  "rain_24hr": 0.27,
  "rain_total": 0.27,
  "last_rain": 0
}