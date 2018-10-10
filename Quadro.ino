
#include "general.h"
#include "esc.h"
#include "sensor.h"

int CurrentSpeed;
int Step = 50;

// Output 50Hz PWM at a resolution of 14 bits on pin DAC1 (pin: PB16, peripheral: PWML0)
void setup() {
  setupESC();
  setupSensor();

  // Required for I/O from Serial monitor
  Serial.begin(9600);
  Serial.println("Setup: Serial port communication at 9600bps");
}

//#define THROTTLE_SETUP

void run() {
  
  minThrottle();
  
  Serial.println("Running ESC");
  Serial.println("Step = ");
  Serial.print(Step);
  Serial.println("\nPress 'u' to increase speed, 'd' to reduce speed");

  CurrentSpeed = ESC_LOW;
  while (1) {
    while (!Serial.available())
    {
    }
    char currentChar = Serial.read();
    if (currentChar == 'u')
    {
      Serial.println("\nIncreasing motor speed by step");
      if (CurrentSpeed + Step < ESC_HIGH) {
        CurrentSpeed = CurrentSpeed + Step;
        Serial.println("New speed = ");
        Serial.print(CurrentSpeed);
      }

      else
      {
        Serial.println("\nMax speed reached\n");
      }
    }
    if (currentChar == 'i')
    {
      Serial.println("\nIncreasing motor speed by step");
      if (CurrentSpeed + 5 < ESC_HIGH) {
        CurrentSpeed = CurrentSpeed + 5;
        Serial.println("New speed = ");
        Serial.print(CurrentSpeed);
      }

      else
      {
        Serial.println("\nMax speed reached\n");
      }
    }

    if (currentChar == 'd')
    {
      Serial.println("\nDecreasing motor speed by step\n");
      if (CurrentSpeed - Step >= ESC_LOW)
      {
        CurrentSpeed = CurrentSpeed - Step;
        Serial.println("New speed = ");
        Serial.print(CurrentSpeed);
      }

      else
      {
        Serial.println("\nMin speed reached\n");
      }
    }
    if (currentChar == 'f')
    {
      Serial.println("\nDecreasing motor speed by step\n");
      if (CurrentSpeed - 5 >= ESC_LOW)
      {
        CurrentSpeed = CurrentSpeed - 5;
        Serial.println("New speed = ");
        Serial.print(CurrentSpeed);
      }

      else
      {
        Serial.println("\nMin speed reached\n");
      }
    }
    if (currentChar == 'e')
    {
      Serial.println("\nStopping Motors\n");
      CurrentSpeed = ESC_LOW;
    }
    REG_PWM_CDTYUPD0 = REG_PWM_CDTYUPD1 = REG_PWM_CDTYUPD2 = REG_PWM_CDTYUPD3 = CurrentSpeed;
  }
}
void loop() {

#ifdef SENSOR_AXIS_TEST
  sensorAxisTest();
#endif
  /*SensorData sensorData = getSensorData();
  Serial.print("AcX = "); Serial.print(sensorData.AcX);
  Serial.print(" | AcY = "); Serial.print(sensorData.AcY);
  Serial.print(" | AcZ = "); Serial.print(sensorData.AcZ);
  Serial.print(" | Tmp = "); Serial.print(sensorData.Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  Serial.print(" | GyX = "); Serial.print(sensorData.GyX);
  Serial.print(" | GyY = "); Serial.print(sensorData.GyY);
  Serial.print(" | GyZ = "); Serial.println(sensorData.GyZ);
  delay(333);*/
  // put your main code here, to run repeatedly:
#ifdef THROTTLE_SETUP
  setupThrottleRange(); 
#endif
#ifdef AXIS_TEST
  axisTest();
#endif
  run();
}
