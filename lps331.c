/* *
 * This file contains implemetation of API for basic communication with LPS 332 sensor
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

#include "lps331.h"
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

static const uint8_t LPS331_ID = 0xbb;

typedef enum {
    LPS331_ADDR_WHO_AM_I = 0xf,
    LPS331_ADDR_TEMP_L = 0x2b,
    LPS331_ADDR_TEMP_H = 0x2c,
    LPS331_ADDR_PRESS_XL = 0x28,
    LPS331_ADDR_PRESS_L = 0x29,
    LPS331_ADDR_PRESS_H = 0x2a,
    LPS331_ADDR_CTRL_REG1 = 0x20,
    LPS331_ADDR_STATUS_REG = 0x27
} LPS331_ADDR;

// Helper function to read I2C register
ssize_t read_at(int fd, uint8_t addr, void *buffer, size_t size) {
    if (write(fd, &addr, 1) <= 0) {
        return -1;
    }
    return read(fd, buffer, size);
}


bool lps331_check_device(int fd) {
    uint8_t id;
    if (read_at(fd, LPS331_ADDR_WHO_AM_I, &id, 1) < 0) {
        return false;
    }
    return id == LPS331_ID;
}

bool lps331_configure(int fd, uint8_t flags) {
    uint8_t data[] = {LPS331_ADDR_CTRL_REG1, flags};
    return write(fd, data, sizeof(data)) == 2;
}

uint8_t lps331_status(int fd) {
    uint8_t v;
    if (read_at(fd, LPS331_ADDR_STATUS_REG, &v, 1) < 1) {
        return 0;
    }
    return v;
}

bool lps331_status_has_temperature(uint8_t status) {
    return (status & 0b00000001) == 0b00000001;
}

bool lps331_status_has_pressure(uint8_t status) {
    return (status & 0b00000010) == 0b00000010;
}

double lps331_temperature(int fd) {
    uint8_t l, h;
    if (read_at(fd, LPS331_ADDR_TEMP_L, &l, 1) <= 0) {
        return -10000;
    }
    if (read_at(fd, LPS331_ADDR_TEMP_H, &h, 1) <= 0) {
        return -10000;
    }
    return 42.5 + ((int16_t) ((h << 8) | l)) / 480.0;
}

double lps331_pressure(int fd) {
    uint8_t l, h, xl;
    if (read_at(fd, LPS331_ADDR_PRESS_XL, &xl, 1) <= 0) {
        return 0;
    }
    if (read_at(fd, LPS331_ADDR_PRESS_L, &l, 1) <= 0) {
        return 0;
    }
    if (read_at(fd, LPS331_ADDR_PRESS_H, &h, 1) <= 0) {
        return 0;
    }

    return ((uint32_t) ((h << 16) | (l << 8) | (xl))) / 4096.0;
}

int lps331_open_device(const char *device, uint8_t address) {
    int bus_fd = open(device, O_RDWR);
    if (bus_fd < 0) {
        return -1;
    }
    if (ioctl(bus_fd, I2C_SLAVE, address) < 0) {
        close(bus_fd);
        return -1;
    }
    return bus_fd;
}