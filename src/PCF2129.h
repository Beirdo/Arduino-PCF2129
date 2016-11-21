/**
 @file PCF2129.h
 @brief This is a library for the PCF2129 RTC IC

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

   Forked from FaBo-PCF2129-Library - authored by FaBo <info@fabo.io>

 @author Gavin Hurlbut <gjhurlgu@gmail.com>
*/

#ifndef PCF2129_H__
#define PCF2129_H__

#include <Arduino.h>
#include <Wire.h>

#define PCF2129_SLAVE_ADDRESS 0x51 ///< I2C Slave Address

/// @name Register Addresses
/// @{
#define PCF2129_REG_CONTROL_1 0x00
#define PCF2129_REG_CONTROL_2 0x01
#define PCF2129_REG_CONTROL_3 0x02
#define PCF2129_REG_SECONDS 0x03
#define PCF2129_REG_MINUTES 0x04
#define PCF2129_REG_HOURS 0x05
#define PCF2129_REG_DAYS 0x06
#define PCF2129_REG_WEEKDAYS 0x07
#define PCF2129_REG_MONTHS 0x08
#define PCF2129_REG_YEARS 0x09
#define PCF2129_REG_SECOND_ALARM 0x0A
#define PCF2129_REG_MINUTE_ALARM 0x0B
#define PCF2129_REG_HOUR_ALARM 0x0C
#define PCF2129_REG_DAY_ALARM 0x0D
#define PCF2129_REG_WEEKDAY_ALARM 0x0E
#define PCF2129_REG_CLKOUT_CTL 0x0F
#define PCF2129_REG_WATCHDG_TIM_CTL 0x10
#define PCF2129_REG_WATCHDG_TIM_VAL 0x11
#define PCF2129_REG_TIMESTP_CTL 0x12
#define PCF2129_REG_SEC_TIMESTP 0x13
#define PCF2129_REG_MIN_TIMESTP 0x14
#define PCF2129_REG_HOUR_TIMESTP 0x15
#define PCF2129_REG_DAY_TIMESTP 0x16
#define PCF2129_REG_MON_TIMESTP 0x17
#define PCF2129_REG_YEAR_TIMESTP 0x18
#define PCF2129_REG_AGING_OFFSET 0x19
#define PCF2129_REG_INTERNAL1 0x1A
#define PCF2129_REG_INTERNAL2 0x1B
/// @}

/// @name Register Bits
/// @{
#define PCF2129_CONTROL1_EXT_TEST	_BV(7)
#define PCF2129_CONTROL1_STOP		_BV(5)
#define PCF2129_CONTROL1_TSF1		_BV(4)
#define PCF2129_CONTROL1_POR_OVRD	_BV(3)
#define PCF2129_CONTROL1_12_24		_BV(2)
#define PCF2129_CONTROL1_MI		_BV(1)
#define PCF2129_CONTROL1_SI		_BV(0)

#define PCF2129_CONTROL2_MSF		_BV(7)
#define PCF2129_CONTROL2_WDTF		_BV(6)
#define PCF2129_CONTROL2_TSF2		_BV(5)
#define PCF2129_CONTROL2_AF		_BV(4)
#define PCF2129_CONTROL2_TSIE		_BV(2)
#define PCF2129_CONTRIL2_AIE		_BV(1)

#define PCF2129_CONTROL3_PWRMNG2	_BV(7)
#define PCF2129_CONTROL3_PWRMNG1	_BV(6)
#define PCF2129_CONTROL3_PWRMNG0	_BV(5)
#define PCF2129_CONTROL3_BTSE		_BV(4)
#define PCF2129_CONTROL3_BF		_BV(3)
#define PCF2129_CONTROL3_BLF		_BV(2)
#define PCF2129_CONTROL3_BIE		_BV(1)
#define PCF2129_CONTROL3_BLIE		_BV(0)

#define PCF2129_SECONDS_OSF		_BV(7)

#define PCF2129_HOURS_AMPM		_BV(5)

#define PCF2129_SECOND_ALARM_AE_S	_BV(7)

#define PCF2129_MINUTE_ALARM_AE_M	_BV(7)

#define PCF2129_HOUR_ALARM_AE_H		_BV(7)
#define PCF2129_HOUR_ALARM_AMPM		_BV(5)

#define PCF2129_DAY_ALARM_AE_D		_BV(7)

#define PCF2129_WEEKDAY_ALARM_AE_W	_BV(7)

#define PCF2129_CLKOUT_CTL_TCR1		_BV(7)
#define PCF2129_CLKOUT_CTL_TCR0		_BV(6)
#define PCF2129_CLKOUT_CTL_OTPR		_BV(5)
#define PCF2129_CLKOUT_CTL_COF2		_BV(2)
#define PCF2129_CLKOUT_CTL_COF1		_BV(1)
#define PCF2129_CLKOUT_CTL_COF0		_BV(0)

#define PCF2129_WATCHDG_CTL_WD_CD	_BV(7)
#define PCF2129_WATCHDG_CTL_TI_TP	_BV(5)
#define PCF2129_WATCHDG_CTL_TF1		_BV(1)
#define PCF2129_WATCHDG_CTL_TF0		_BV(0)

#define PCF2129_TIMESTP_CTL_TSM		_BV(7)
#define PCF2129_TIMESTP_CTL_TSOFF	_BV(6)
#define PCF2129_TIMESTP_CTL_1_O_16_TIMESTP_MASK	0x0F

#define PCF2129_HOUR_TIMESTP_AMPM	_BV(5)

#define PCF2129_AGING_OFFSET_MASK	0x0F
/// @}

/**
 @class DateTime
 @brief RTC DateTime class
*/
class DateTime {
  public:
    DateTime (uint16_t years, uint8_t months, uint8_t days,
              uint8_t hours, uint8_t minutes, uint8_t seconds);
    uint16_t year()  const { return y+2000; }
    uint8_t month()  const { return m; }
    uint8_t day()    const { return d; }
    uint8_t hour()   const { return hh; }
    uint8_t minute() const { return mm; }
    uint8_t second() const { return ss; }
  protected:
    uint8_t y;
    uint8_t m;
    uint8_t d;
    uint8_t hh;
    uint8_t mm;
    uint8_t ss;
};

/**
 @class PCF2129
 @brief PCF2129 RTC I2C Control class
*/
class PCF2129 {
  public:
    PCF2129(uint8_t addr = PCF2129_SLAVE_ADDRESS);
    bool searchDevice(void);
    void configure(void);
    uint8_t getSeconds(void);
    void setSeconds(uint8_t seconds);
    uint8_t getMinutes(void);
    void setMinutes(uint8_t minutes);
    uint8_t getHours(void);
    void setHours(uint8_t hours);
    uint8_t getDays(void);
    void setDays(uint8_t days);
    uint8_t getWeekdays(void);
    void setWeekdays(uint8_t weekdays);
    uint8_t getMonths(void);
    void setMonths(uint8_t months);
    uint8_t getYears(void);
    void setYears(uint8_t years);
    DateTime now(void);
    void setDate(uint16_t years, uint8_t months, uint8_t days,
                 uint8_t hours, uint8_t minutes, uint8_t seconds);
    void set12mode(void);
    void set24mode(void);
  private:
    uint8_t _i2caddr;
    uint8_t bcdToDec(uint8_t value);
    uint8_t decToBcd(uint8_t value);
    uint8_t readI2c(uint8_t address);
    void writeI2c(uint8_t address, uint8_t data);
};

#endif // PCF2129_H__
