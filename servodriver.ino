#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

String getToken(String data, char delimiter, int index) {
  int found = 0;
  int startIndex = 0;
  int endIndex = -1;
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == delimiter || i == maxIndex) {
      found++;
      startIndex = endIndex + 1;
      endIndex = (i == maxIndex) ? i + 1 : i;
    }
  }

  if (found > index) {
    return data.substring(startIndex, endIndex);
  }
  return "";
}

void setup() {
  servo1.attach(4);
  servo2.attach(5);
  servo3.attach(7);
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    char buffer[16];
    int len = Serial.readBytesUntil('\n', buffer, sizeof(buffer) - 1);
    buffer[len] = '\0';

    int servo_id = atoi(strtok(buffer, ":"));
    int value = atoi(strtok(NULL, ":"));


    switch (servo_id) {
      case 1: servo1.write(value); break;
      case 2: servo2.write(value); break;
      case 3: servo3.write(value); break;
    }

    delay(20);
  }
}
