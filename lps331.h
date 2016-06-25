/* *
 * This file contains API for basic communication with LPS 332 sensor
 *
 * This file is part of lps331-tools.
 *
 * lps331-tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lps331-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lps331-tools.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2016 Baryshnikov Alexander <dev@baryshnikov.net>
 *
 * Please feel free to contact with author
 */


#include <stdbool.h>
#include <stdint.h>

#ifndef LPS331_LPS331_H
#define LPS331_LPS331_H

// List of basic profiles for sensor.
// All profiles includes ACTIVE power mode and different rates
typedef enum {
    LPS331_PROFILE_PRESS_1HZ_TEMP_1HZ = 0b10010000, //Updates rates: 1Hz temperature and 1Hz pressure
    LPS331_PROFILE_PRESS_7HZ_TEMP_1HZ = 0b10100000, //Updates rates: 1Hz temperature and 7Hz pressure
    LPS331_PROFILE_PRESS_12HZ_TEMP_1HZ = 0b10110000, //Updates rates: 12Hz temperature and 1Hz pressure
    LPS331_PROFILE_PRESS_25HZ_TEMP_1HZ = 0b11000000, //Updates rates: 25Hz temperature and 1Hz pressure
    LPS331_PROFILE_PRESS_7HZ_TEMP_7HZ = 0b11010000, //Updates rates: 7Hz temperature and 7Hz pressure
    LPS331_PROFILE_PRESS_12HZ_TEMP_12HZ = 0b11100000, //Updates rates: 12Hz temperature and 12Hz pressure
    LPS331_PROFILE_PRESS_25HZ_TEMP_25HZ = 0b11110000  //Updates rates: 25Hz temperature and 25Hz pressure
} lps331_basic_profile;

/*
 * Check, that device is LPS331 by reading WHO_AM_I register
 * @param fd file descriptor pointed to I2C device
 * @return true if response is correct otherwise false
 */
bool lps331_check_device(int fd);

/**
 * Configure sensor by REG_CONF register. You can use basic flags from lps331_basic_profile enum
 * @param fs file descriptor pointed to I2C device
 * @return true write success otherwise false
 */
bool lps331_configure(int fd, uint8_t flags);

/**
 * Get current status of sensor by STATUS_REG register. You can use helper function to read status by lps331_status_xxx
 * @param fd file descriptor pointed to I2C device
 * @return bit flags or 0 if something was wrong (check errno)
 */
uint8_t lps331_status(int fd);

/**
 * Check status bitmask for flag when temperature value is ready to be read
 * @param status result of lps331_status
 * @return true if temperature registers are ready to be read
 */
bool lps331_status_has_temperature(uint8_t status);

/**
 * Check status bitmask for flag when pressure value is ready to be read
 * @param status result of lps331_status
 * @return true if pressure registers are ready to be read
 */
bool lps331_status_has_pressure(uint8_t status);

/**
 * Get temperature in Celsium from sensor
 * @param fd file descriptor pointed to I2C device
 * @return value of temperature or -10000 if something wrong
 */
double lps331_temperature(int fd);


/**
 * Get pressure in mbars from sensor
 * @param fd file descriptor pointed to I2C device
 * @return value of pressure or -10000 if something wrong
 */
double lps331_pressure(int fd);

/**
 * Helper function to open LPS331 sensor over I2C protocol
 * @param device name of I2C bus (ex: /dev/i2c-1)
 * @param address address in I2C of sensor (ex: 0x5C)
 * @return file descriptor pointed to sensor or -1
 */
int lps331_open_device(const char *device, uint8_t address);

#endif //LPS331_LPS331_H
