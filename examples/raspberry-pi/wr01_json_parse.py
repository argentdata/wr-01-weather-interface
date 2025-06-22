# WR-01 JSON parsing demo
# S. Miller 3 June 2025
#
# This Python script provides a simple demonstration of reading and parsing
# data in JSON format from the Argent Data Systems WR-01 wind/rain sensor
# interface.
#
# The WR-01 must be configured for JSON output mode. For the TTL/RS-232 port,
# set register 20 to value 32 (command "!20=32"). For the RS-485 port,
# set register 18 to value 32 (command "!18=32"). Be sure to use the "!save"
# command to save the setting.
#
# This script demonstrates the basics of reading a single line from the
# serial port, parsing the line as JSON, and extracting a few values.
# wind_dir and wind_speed values are grouped by averaging intervals under
# "instant", "2min", "10min", and "custom". "gust" gives the peak wind speed
# in the 2min, 10min, and custom groups. Rainfall values are reported
# as "rain_1hr", "rain24_hr", "rain_total", and "last rain". Units are
# meters/second for speeds, degrees for directions, millimeters for rain,
# and minutes for time since last rain.

import serial
import json

# Open serial port - adjust port and baud rate as needed
ser = serial.Serial('COM5', 115200, timeout=1)

print("Listening on COM5 at 115200 baud...")

while True:
    try:
        # Read a line (assumes JSON is newline-terminated)
        line = ser.readline().decode('utf-8').strip()
        if not line:
            continue

        # Parse the JSON object
        data = json.loads(line)

        # Access nested and flat values
        instant = data.get("instant", {})
        avg_2min = data.get("2min", {})
        rain_1hr = data.get("rain_1hr")
        last_rain = data.get("last_rain")

        # Print out a few things
        print(f"[Instant] Wind: {instant.get('wind_speed', '?')} m/s from {instant.get('wind_dir', '?')}°")
        print(f"[2 Min Avg] Wind: {avg_2min.get('wind_speed', '?')} m/s")
        print(f"Rain (1h): {rain_1hr} mm, Last rain: {last_rain} min ago\n")

    except json.JSONDecodeError as e:
        print(f"JSON parse error: {e}")
    except Exception as e:
        print(f"Unexpected error: {e}")
