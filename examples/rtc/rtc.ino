/**
 @file rtc.ino
 @brief This is an Example for the PCF2129 RTC I2C IC.

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

   Forked from FaBo-PCF2129-Library - authored by FaBo <info@fabo.io>

 @author Gavin Hurlbut <gjhurlgu@gmail.com>

#include <Wire.h>
#include <PCF2129.h>

PCF2129 rtc;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("RESET");

  Serial.println("Checking I2C device...");
  if (rtc.searchDevice()) {
    Serial.println("configuring FaBo RTC I2C Brick");
    rtc.configure();
  } else {
    Serial.println("device not found");
    while(1);
  }

  Serial.println("set date/time");
  rtc.setDate(2016,4,1,12,1,50);
}

void loop() {
  DateTime now = rtc.now();

  Serial.print("Time: ");
  Serial.print(now.year());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.day());
  Serial.print(" ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.print(now.second());
  Serial.println();

  delay(1000);
}
