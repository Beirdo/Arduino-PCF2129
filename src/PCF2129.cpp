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
PCF2129::PCF2129(uint8_t addr, uint8_t irq) {
  m_i2caddr = addr;
  Wire1.begin();

  m_watchdog_ctl = PCF2129_WATCHDG_CTL_TI_TP;
  writeI2c(PCF2129_REG_WATCHDG_TIM_CTL, m_watchdog_ctl);

  if (irq) {
    m_control1 = PCF2129_CONTROL1_SI;
  } else {
    m_control1 = 0;
  }
  writeI2C(PCF2129_REG_CONTROL1, m_control1);

  m_control2 = 0;
  writeI2C(PCF2129_REG_CONTROL2, m_control2);

  m_control3 = 0;
  writeI2C(PCF2129_REG_CONTROL3, m_control3);

  m_clkout = PCF2129_CLKOUT_CTL_COF0 | PCF2129_CLKOUT_CTL_COF1 |
             PCF2129_CLKOUT_CTL_COF2;
  writeI2C(PCF2129_REG_CLKOUT_CTL, m_clkout);
  writeI2C(PCF2129_REG_CLKOUT_CTL, m_clkout | PCF2129_CLKOUT_CTL_OTPR);

  m_timestp = PCF2129_TIMESTP_CTL_TSOFF;
  writeI2C(PCF2129_REG_TIMESTP_CTL, m_timestp);

  if (irq) {
    attachInterrupt(digitalPinToInterrupt(irq), ISR, FALLING);
  }
}

void PCF2129::ISR(void)
{
  m_poll = 1;
}

/**
 @brief Search Device
 @retval true device connected
 @retval false device error
*/
bool PCF2129::searchDevice(void) {
  return !(readI2c(m_i2caddr) >> 7);
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
  m_poll = 0;
  Wire1.setClock(400000L);
  Wire1.beginTransmission(m_i2caddr);
  Wire1.write(PCF2129_REG_SECONDS);
  Wire1.endTransmission();
  Wire1.requestFrom(m_i2caddr, (uint8_t)7);
  while(!Wire1.available());
  uint8_t seconds = bcdToDec(Wire1.read());
  uint8_t minutes = bcdToDec(Wire1.read());
  uint8_t hours = bcdToDec(Wire1.read());
  uint8_t days = bcdToDec(Wire1.read());
  Wire1.read(); // blank read weekdays
  uint8_t months = bcdToDec(Wire1.read());
  uint16_t years = bcdToDec(Wire1.read()) + 2000;

  return DateTime(years, months, days, hours, minutes, seconds);
}

/**
 @brief Set to RTC
 @param [in] DateTime DateTime 
*/
void PCF2129::setDate(uint16_t years, uint8_t months, uint8_t days,
                              uint8_t hours, uint8_t minutes, uint8_t seconds) {
  Wire1.setClock(400000L);
  Wire1.beginTransmission(m_i2caddr);
  Wire1.write(PCF2129_REG_SECONDS);
  Wire1.write(decToBcd(seconds) | PCF2129_SECONDS_OSF);
  Wire1.write(decToBcd(minutes));
  Wire1.write(decToBcd(hours));
  Wire1.write(decToBcd(days));
  Wire1.write(0x00);
  Wire1.write(decToBcd(months));
  Wire1.write(decToBcd(years - 2000));
  Wire1.endTransmission();
}

/**
 @brief Set to 12 hour mode
*/
void PCF2129::set12mode(void) {
  m_control1 |= PCF2129_CONTROL1_12_24;
  writeI2c(PCF2129_REG_CONTROL1, m_control1);
}

/**
 @brief Set to 24 hour mode
*/
void PCF2129::set24mode(void) {
  m_control1 &= ~(PCF2129_CONTROL1_12_24);
  writeI2c(PCF2129_REG_CONTROL1, m_control1);
}

uint8_t PCF2129::getBatteryLow(void) {
  uint8_t value = readI2c(PCF2129_REG_CONTROL3);
  return !(!(value & PCF2129_CONTROL3_BLF));
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
  Wire1.setClock(400000L);
  Wire1.beginTransmission(m_i2caddr);
  Wire1.write(address);
  Wire1.endTransmission();
  Wire1.requestFrom(m_i2caddr, (uint8_t)1);
  while(!Wire1.available());
  return Wire1.read();
}

/**
 @brief Write I2C Data
 @param [in] address register address 
 @param [in] data write data 
*/
void PCF2129::writeI2c(uint8_t address, uint8_t data) {
  Wire1.setClock(400000L);
  Wire1.beginTransmission(m_i2caddr);
  Wire1.write(address);
  Wire1.write(data);
  Wire1.endTransmission();
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
