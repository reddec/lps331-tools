/* *
 * This file contains simple CLI utility for reading data from LPS 332 sensor
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

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <memory.h>
#include <stdlib.h>
#include "lps331.h"

void summary(const char *command) {
    printf("LPS331 CLI reader 1.0.0\n");
    printf("\n");
    printf("Usage: %s <device> <address> [profile]\n", command);
    printf("\n");
    printf("  device             - path of I2C device (like /dev/i2c-1)\n");
    printf("  address            - address of sensor in device in HEX (like 0x52)\n");
    printf("  profile (optional) - Index of rates profile (see below). Default 4\n");
    printf("\n");
    printf("Profiles:\n");
    printf("    0 - Updates rates: 1Hz temperature and 1Hz pressure\n");
    printf("    1 - Updates rates: 7Hz temperature and 1Hz pressure\n");
    printf("    2 - Updates rates: 12Hz temperature and 1Hz pressure\n");
    printf("    3 - Updates rates: 25Hz temperature and 1Hz pressure\n");
    printf("    4 - Updates rates: 7Hz temperature and 7Hz pressure\n");
    printf("    5 - Updates rates: 12Hz temperature and 12Hz pressure\n");
    printf("    6 - Updates rates: 25Hz temperature and 25Hz pressure\n");
    printf("\n");
    printf("Version: 1.0.0\n");
    printf("Author: Baryshnikov Alexander <dev@baryshnikov.net>\n");
}

static const lps331_basic_profile profile_map[] = {
        LPS331_PROFILE_PRESS_1HZ_TEMP_1HZ,
        LPS331_PROFILE_PRESS_7HZ_TEMP_1HZ,
        LPS331_PROFILE_PRESS_12HZ_TEMP_1HZ,
        LPS331_PROFILE_PRESS_25HZ_TEMP_1HZ,
        LPS331_PROFILE_PRESS_7HZ_TEMP_7HZ,
        LPS331_PROFILE_PRESS_12HZ_TEMP_12HZ,
        LPS331_PROFILE_PRESS_25HZ_TEMP_25HZ
};

int main(int argc, char **argv) {
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--help") == 0 ||
                      strcmp(argv[1], "---version") == 0)) {
        summary(argv[0]);
        return 0;
    }
    if (argc < 3) {
        summary(argv[0]);
        return 1;
    }
    long profile_index = argc > 3 ? strtol(argv[3], NULL, 0) : 4;
    const char *device = argv[1];
    const uint8_t address = (uint8_t) strtol(argv[2], NULL, 0);
    if (profile_index < 0 || profile_index > (long) sizeof(profile_map)) {
        fprintf(stderr, "Invalid profile index!\n");
        summary(argv[0]);
        return 1;
    }
    lps331_basic_profile profile = profile_map[profile_index];
    struct timespec interval;// Max rate is 25Hz. How much interval will be in milliseconds?
    interval.tv_sec = 0;
    interval.tv_nsec = (uint64_t) ((1000.0 / 25) * 1000);
    int bus_fd = lps331_open_device(device, address);
    if (bus_fd < 0) {
        perror("open bus");
        return 2;
    }
    if (!lps331_check_device(bus_fd)) {
        fprintf(stderr, "device is not LPS331\n");
        return 3;
    }
    if (!lps331_configure(bus_fd, profile)) {
        perror("configure");
        return 4;
    }
    uint8_t status;
    bool skipped;
    while (1) {
        skipped = true;
        status = lps331_status(bus_fd);
        if (lps331_status_has_temperature(status)) {
            printf("temp %f %s\n", lps331_temperature(bus_fd), "c");
            skipped = false;
        }
        if (lps331_status_has_pressure(status)) {
            printf("pressure %f %s\n", lps331_pressure(bus_fd), "mbar");
            skipped = false;
        }
        if (!skipped) {
            fflush(stdout);
            fflush(stderr);
        } else {
            if (nanosleep(&interval, NULL) < 0) break;
        }
    }
    return 0;
}