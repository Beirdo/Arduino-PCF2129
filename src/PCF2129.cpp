/**
 @file PCF2129.cpp
 @brief This is a library for the PCF2129 RTC IC

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

   Forked from FaBo-PCF2129-Library - authored by FaBo <info@fabo.io>

 @author Gavin Hurlbut <gjhurlgu@gmail.com>
*/

#include "PCF2129.h"

/**
 @brief Constructor
*/
PCF2129::PCF2129(uint8_t addr) {
  _i2caddr = addr;
  Wire.begin();
}

/**
 @brief Search Device
 @retval true device connected
 @retval false device error
*/
bool PCF2129::searchDevice(void) {
  return !(readI2c(_i2caddr) >> 7);
}

/**
 @brief Configure Device
*/
void PCF2129::configure(void) {
  set24mode();
}

/**
 @brief Get Seconds from RTC
 @param [out] seconds seconds 
*/
uint8_t PCF2129::getSeconds(void) {
  return bcdToDec(readI2c(PCF2129_REG_SECONDS));
}

/**
 @brief Set Seconds to RTC
 @param [in] seconds seconds 
*/
void PCF2129::setSeconds(uint8_t seconds) {
  if ( seconds>59 && seconds<0 ) {
    seconds = 0;
  }
  writeI2c(PCF2129_REG_SECONDS, (decToBcd(seconds) | PCF2129_SECONDS_OSF));
}

/**
 @brief Get Minutes from RTC
 @param [out] minutes minutes 
*/
uint8_t PCF2129::getMinutes(void) {
  return bcdToDec(readI2c(PCF2129_REG_MINUTES));
}

/**
 @brief Set Minutes to RTC
 @param [in] minutes minutes 
*/
void PCF2129::setMinutes(uint8_t minutes) {
  if ( minutes>59 && minutes<0 ) {
    minutes = 0;
  }
  writeI2c(PCF2129_REG_MINUTES, decToBcd(minutes));
}

/**
 @brief Get Hours from RTC
 @param [out] hours hours 
*/
uint8_t PCF2129::getHours(void) {
  return bcdToDec(readI2c(PCF2129_REG_HOURS));
}

/**
 @brief Set Hours to RTC
 @param [in] hours hours 
*/
void PCF2129::setHours(uint8_t hours) {
  set24mode();
  if (hours > 23 && hours < 0) {
    hours = 0;
  }
  writeI2c(PCF2129_REG_HOURS, decToBcd(hours));
}

/**
 @brief Get Days from RTC
 @param [out] days days 
*/
uint8_t PCF2129::getDays(void) {
  return bcdToDec(readI2c(PCF2129_REG_DAYS));
}

/**
 @brief Set Days to RTC
 @param [in] days days 
*/
void PCF2129::setDays(uint8_t days) {
  if (days > 31 && days < 1) {
    days = 1;
  }
  writeI2c(PCF2129_REG_DAYS, decToBcd(days));
}

/**
 @brief Get Weekdays from RTC
 @param [out] weekdays weekdays 
*/
uint8_t PCF2129::getWeekdays(void) {
  return bcdToDec(readI2c(PCF2129_REG_WEEKDAYS));
}

/**
 @brief Set Weekdays to RTC
 @param [in] weekdays weekdays 
*/
void PCF2129::setWeekdays(uint8_t weekdays) {
  if (weekdays > 6 && weekdays < 0) {
    weekdays = 0;
  }
  writeI2c(PCF2129_REG_WEEKDAYS, decToBcd(weekdays));
}

/**
 @brief Get Months from RTC
 @param [out] months months 
*/
uint8_t PCF2129::getMonths(void) {
  return bcdToDec(readI2c(PCF2129_REG_MONTHS));
}

/**
 @brief Set Months to RTC
 @param [in] months months 
*/
void PCF2129::setMonths(uint8_t months) {
  if (months > 12 && months < 1) {
    months = 1;
  }
  writeI2c(PCF2129_REG_MONTHS, decToBcd(months));
}

/**
 @brief Get Years from RTC
 @param [out] years years 
*/
uint8_t PCF2129::getYears(void) {
  return bcdToDec(readI2c(PCF2129_REG_YEARS));
}

/**
 @brief Set Years to RTC
 @param [in] years years 
*/
void PCF2129::setYears(uint8_t years) {
  if (years > 99 && years < 0) {
    years = 0;
  }
  writeI2c(PCF2129_REG_YEARS, decToBcd(years));
}

/**
 @brief Read from RTC
 @param [out] DateTime DateTime 
*/
DateTime PCF2129::now(void) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(PCF2129_REG_SECONDS);
  Wire.endTransmission();
  Wire.requestFrom(_i2caddr, (uint8_t)7);
  while(!Wire.available());
  uint8_t seconds = bcdToDec(Wire.read());
  uint8_t minutes = bcdToDec(Wire.read());
  uint8_t hours = bcdToDec(Wire.read());
  uint8_t days = bcdToDec(Wire.read());
  Wire.read(); // blank read weekdays
  uint8_t months = bcdToDec(Wire.read());
  uint16_t years = bcdToDec(Wire.read()) + 2000;

  return DateTime(years, months, days, hours, minutes, seconds);
}

/**
 @brief Set to RTC
 @param [in] DateTime DateTime 
*/
void PCF2129::setDate(uint16_t years, uint8_t months, uint8_t days,
                              uint8_t hours, uint8_t minutes, uint8_t seconds) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(PCF2129_REG_SECONDS);
  Wire.write(decToBcd(seconds) | PCF2129_SECONDS_OSF);
  Wire.write(decToBcd(minutes));
  Wire.write(decToBcd(hours));
  Wire.write(decToBcd(days));
  Wire.write(0x00);
  Wire.write(decToBcd(months));
  Wire.write(decToBcd(years - 2000));
  Wire.endTransmission();
}

/**
 @brief Set to 12 hour mode
*/
void PCF2129::set12mode(void) {
  uint8_t ctrl;
  ctrl = readI2c(PCF2129_REG_CONTROL1);
  ctrl |= PCF2129_CONTROL1_12_24;
  writeI2c(PCF2129_REG_CONTROL1, ctrl);
}

/**
 @brief Set to 24 hour mode
*/
void PCF2129::set24mode(void) {
  uint8_t ctrl;
  ctrl = readI2c(PCF2129_REG_CONTROL1);
  ctrl &= ~(PCF2129_CONTROL1_12_24);
  writeI2c(PCF2129_REG_CONTROL1, ctrl);
}

////////////////////////////////////////////////////////////////

/**
 @brief BCD to DEC
 @param [in] value BCD value 
 @param [out] value DEC value 
*/
uint8_t PCF2129::bcdToDec(uint8_t value) {
  return (uint8_t)(((value >> 4) * 10) + (value & 0x0F));
}

/**
 @brief DEC to BCD
 @param [in] value DEC value 
 @param [out] value BCD value 
*/
uint8_t PCF2129::decToBcd(uint8_t value) {
  return (uint8_t)(((value / 10) << 4) + (value % 10));
}

/**
 @brief Read I2C Data
 @param [in] address register address 
 @param [out] data read data 
*/
uint8_t PCF2129::readI2c(uint8_t address) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(_i2caddr, (uint8_t)1);
  while(!Wire.available());
  return Wire.read();
}

/**
 @brief Write I2C Data
 @param [in] address register address 
 @param [in] data write data 
*/
void PCF2129::writeI2c(uint8_t address, uint8_t data) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

////////////////////////////////////////////////////////////////

/**
 @brief Constructor
*/
DateTime::DateTime(uint16_t years, uint8_t months, uint8_t days,
                   uint8_t hours, uint8_t minutes, uint8_t seconds) {
  if (years >= 2000)
      years -= 2000;
  y = years;
  m = months;
  d = days;
  hh = hours;
  mm = minutes;
  ss = seconds;
}
