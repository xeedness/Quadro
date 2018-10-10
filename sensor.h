#ifndef SENSOR_INO
#define SENSOR_INO

#include <Wire.h>
#include "general.h"

#define SWAP_X
#define SWAP_Y
#define SWAP_Z

const int MPU_addr=0x68;  // I2C address of the MPU-6050
struct SensorData {
    int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
};

void setupSensor() {
    Wire.begin();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(0);     // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
}

SensorData getSensorData() {
    SensorData data;
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
    data.AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    data.AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    data.AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    data.Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    data.GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    data.GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    data.GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
#ifdef SWAP_X
    data.AcX = -data.AcX;
#endif
#ifdef SWAP_Y
    data.AcY = -data.AcY;
#endif
#ifdef SWAP_Z
    data.AcZ = -data.AcZ;
#endif
    return data;
}

#define OFF_THRESH 1000
#define LOW_THRESH 3000
#define HIGH_THRESH 5000

void sensorAxisTest() {;
    Serial.println("Running Sensor Axis Test. Move the sensor in all directions, while the direction is transmitted to the monitor. To finish type 'e'. Rdy?");
    pause();
    while (true) {
        SensorData data = getSensorData();
        if(data.AcX < -LOW_THRESH && data.AcY < -LOW_THRESH) {
            Serial.println("Back Left");
        } else if(data.AcX > LOW_THRESH && data.AcY < -LOW_THRESH) {
            Serial.println("Back Right");
        } else if(data.AcX < -LOW_THRESH && data.AcY > LOW_THRESH) {
            Serial.println("Front Left");
        } else if(data.AcX > LOW_THRESH && data.AcY > LOW_THRESH) {
            Serial.println("Front Right");
        } else if(data.AcX < -HIGH_THRESH && abs(data.AcY) < OFF_THRESH) {
            Serial.println("Left");
        } else if(data.AcX > HIGH_THRESH && abs(data.AcY) < OFF_THRESH) {
            Serial.println("Right");
        } else if(data.AcY < -HIGH_THRESH && abs(data.AcX) < OFF_THRESH) {
            Serial.println("Back");
        } else if(data.AcY > HIGH_THRESH && abs(data.AcX) < OFF_THRESH) {
            Serial.println("Front");
        } else {
            Serial.print("No Tilt: ");
            Serial.print("AcX = "); Serial.print(data.AcX);
            Serial.print(" | AcY = "); Serial.print(data.AcY);
            Serial.print(" | AcZ = "); Serial.print(data.AcZ);
            Serial.print(" | Tmp = "); Serial.print(data.Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
            Serial.print(" | GyX = "); Serial.print(data.GyX);
            Serial.print(" | GyY = "); Serial.print(data.GyY);
            Serial.println(" | GyZ = "); Serial.println(data.GyZ);
        }
        char currentChar = Serial.read();
        if (currentChar == 'e') break;
        delay(333);
    }
    pause();
}



#endif //SENSOR_INO