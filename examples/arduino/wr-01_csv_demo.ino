/*
  Arduino Demo for the Argent Data Systems WR-01 Wind/Rain Interface
  https://www.argentdata.com/catalog/product_info.php?products_id=217  
  Created on 13 November 2024 by S. Miller
  Contact: support@argentdata.com

  Description:
  This demo assumes the Serial Monitor is connected via Serial, and the WR-01 is connected via Serial1.
  It reads CSV data from the WR-01 and prints parsed data to the Serial Monitor.

  Connection Details:
  - **WR-01 Default Baud Rate**: 115200
  - **WR-01 "TX" Pin**: Outputs 3.3V TTL serial data (non-inverted).
  - **WR-01 "TXI" Pin**: Outputs inverted (RS-232 style) serial data from 0V to V+.
  - **Arduino Connection**: Connect the WR-01 "TX" pin to the RX pin of Serial1 on your Arduino.
    - Ensure voltage level compatibility (use a logic level converter if necessary).

  CSV Output Format:
  Each line of data from the WR-01 follows this format:

   <wind averaging period>,<wind speed in MPH>,mph,<wind speed in m/s>,m/s,<wind direction in degrees>,
   <1-hour rain total in inches>,<24-hour rain total in inches>,<rain since reset in inches>,in,
   <1-hour rain total in mm>,<24-hour rain total in mm>,<rain since reset in mm>,mm,
   <time since last rain detection in minutes>

Wind Averaging Period:
- 'I' for **Instantaneous**
- '2M' for **2 Minutes**
- '10M' for **10 Minutes**
- 'C' for **Custom** (based on the WR-01's user configuration)

Note that a wind direction of 360 indicates north. A wind direction reading of 0 degrees
indicates a missing wind vane or invalid reading. If no rain has been detected since reset,
the time since last rain field will show 65535. The fixed "mph", "m/s", "in", and "mm" labels
are included to improve human-readability and never change.

Example of a complete set of outputs from the WR-01:

I,0.00,mph,0.00,m/s,180,0.000,0.000,0.000,in,0.00,0.00,0.00,mm,65535
2M,0.00,mph,0.00,m/s,176,0.000,0.000,0.000,in,0.00,0.00,0.00,mm,65535
10M,0.00,mph,0.00,m/s,176,0.000,0.000,0.000,in,0.00,0.00,0.00,mm,65535
C,0.00,mph,0.00,m/s,176,0.000,0.000,0.000,in,0.00,0.00,0.00,mm,65535

*/

void setup() {
  Serial.begin(115200);    // Initialize Serial Monitor
  Serial1.begin(115200);   // Initialize Serial1 for TTL serial input from the WR-01
}

void loop() {
  // Wait for data, read line until newline, trim whitespace, and parse each line
  if (Serial1.available()) {
    String inputLine = Serial1.readStringUntil('\n');
    inputLine.trim();
    parseWR01(inputLine);
  }
  // User code can continue doing other things here
}

void parseWR01(String inputLine) {
  // Convert the input line to a character array for parsing
  char inputCopy[160];
  inputLine.toCharArray(inputCopy, sizeof(inputCopy));

  // Tokenize the input line using strtok()
  char *token = strtok(inputCopy, ",");
  const int expectedFields = 15;
  char *tokens[expectedFields];
  int tokenIndex = 0;

  while (token != NULL && tokenIndex < expectedFields) {
    tokens[tokenIndex++] = token;
    token = strtok(NULL, ",");
  }

  // Check if we have the expected number of fields
  if (tokenIndex == expectedFields) {
    // Assign tokens to variables
    int i = 0;
    String avgInterval = String(tokens[i++]);
    float windSpeedMph = atof(tokens[i++]);
    String mphUnit = String(tokens[i++]);
    float windSpeedMs = atof(tokens[i++]);
    String msUnit = String(tokens[i++]);
    int windDirection = atoi(tokens[i++]);
    float rainfall1hIn = atof(tokens[i++]);
    float rainfall24hIn = atof(tokens[i++]);
    float rainfallSinceResetIn = atof(tokens[i++]);
    String inUnit = String(tokens[i++]);
    float rainfall1hMm = atof(tokens[i++]);
    float rainfall24hMm = atof(tokens[i++]);
    float rainfallSinceResetMm = atof(tokens[i++]);
    String mmUnit = String(tokens[i++]);
    unsigned int minutesSinceLastRain = atoi(tokens[i++]);

    // Print the parsed values to the Serial Monitor
    Serial.println("Parsed Weather Data:");
    Serial.print("Averaging Interval: ");
    Serial.println(avgInterval);

    Serial.print("Wind Speed: ");
    Serial.print(windSpeedMph);
    Serial.print(" ");
    Serial.print(mphUnit);
    Serial.print(" (");
    Serial.print(windSpeedMs);
    Serial.print(" ");
    Serial.print(msUnit);
    Serial.println(")");

    Serial.print("Wind Direction: ");
    Serial.print(windDirection);
    Serial.println(" degrees");

    Serial.print("Rainfall (last 1 hour): ");
    Serial.print(rainfall1hIn);
    Serial.print(" ");
    Serial.print(inUnit);
    Serial.print(" (");
    Serial.print(rainfall1hMm);
    Serial.print(" ");
    Serial.print(mmUnit);
    Serial.println(")");

    Serial.print("Rainfall (last 24 hours): ");
    Serial.print(rainfall24hIn);
    Serial.print(" ");
    Serial.print(inUnit);
    Serial.print(" (");
    Serial.print(rainfall24hMm);
    Serial.print(" ");
    Serial.print(mmUnit);
    Serial.println(")");

    Serial.print("Rainfall (since reset): ");
    Serial.print(rainfallSinceResetIn);
    Serial.print(" ");
    Serial.print(inUnit);
    Serial.print(" (");
    Serial.print(rainfallSinceResetMm);
    Serial.print(" ");
    Serial.print(mmUnit);
    Serial.println(")");

    if (minutesSinceLastRain == 65535) {
      Serial.println("No rain detected.");
    } else {
      Serial.print("Minutes Since Last Rain Detection: ");
      Serial.println(minutesSinceLastRain);
    }

    Serial.println("-----------------------------------");

  } else {
    // Handle parsing error due to unexpected number of fields
    Serial.println("Error: Unexpected number of fields in the input line.");
  }
}
