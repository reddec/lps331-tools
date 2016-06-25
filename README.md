# lps331-tools

API libraries and CLI Tools for LPS331 sensor.

[Datasheet](http://www.st.com/content/ccc/resource/technical/document/datasheet/40/c2/46/1e/99/22/4c/60/DM00036196.pdf/files/DM00036196.pdf/jcr:content/translations/en.DM00036196.pdf)

## Obtain

[You may use precompilled versions](https://github.com/reddec/lps331-tools/releases) 
or build from source

### Build requirements

1. CMake 2.8
2. Make
3. C11 (gnu)  compiler
4. I2C-supported linux kernel (all that I know)
5. git (optional) for cloning

### Build

This is generic CMake process.

First, create new empty directory:

    mkdir -p /tmp/build-lps331
    cd /tmp

Then clone Git repo

    git clone https://github.com/reddec/lps331-tools.git

Use CMake for prepare MAKE files:

    cd /tmp/build-lps331
    cmake -DCMAKE_BUILD_TYPE=Release /tmp/lps331-tools

Compile and package:

    make package


### Install

After build you can use one of following method:

1. Use `lps331-1.0.0-Linux.sh` as self-installer
2. Use `lps331-1.0.0-Linux.tar.gz` as binary tar-ball
3. Use built files directly =)

## Use CLI

Usage: `.lps331 <device> <address> [profile]`

* `device`             - path of I2C device (like /dev/i2c-1)
* `address`            - address of sensor in device in HEX (like 0x52)
* `profile` (optional) - Index of rates profile (see below). Default 4

### Profiles

| # | Update temperature (Hz)  | Update pressure (Hz) |
|---|--------------------------|----------------------|
| 0 | 1                        | 1                    |
| 1 | 7                        | 1                    |
| 2 | 12.5                     | 1                    |
| 3 | 25                       | 1                    |
| 4 | 7                        | 7                    |
| 5 | 12.5                     | 12.5                 |
| 6 | 25                       | 25                   |

### Output

Space separated lines.

Pattern:

    <name><SPACE><value><SPACE><units><NEWLINE>

* names: `temp` and `pressure`
* units: `c` and `mbar`
* value: text view of `double` value

Examples:

    temp 22.5 c
    pressure 997.1123 mbar

## API



**TODO:**

### Linking

You may use static or shared library.
