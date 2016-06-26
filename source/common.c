/*
Copyright (c) 2016 Robert Wolterman

Original BBIO Author Justin Cooper
Modified for CHIP_IO Author Robert Wolterman

This file incorporates work covered by the following copyright and
permission notice, all modified code adopts the original license:

Copyright (c) 2013 Adafruit

Original RPi.GPIO Author Ben Croston
Modified for BBIO Author Justin Cooper

This file incorporates work covered by the following copyright and
permission notice, all modified code adopts the original license:

Copyright (c) 2013 Ben Croston

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Python.h"
#include <dirent.h>
#include <time.h>
#include "common.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

int setup_error = 0;
int module_setup = 0;

// I have no idea if this table is correct, we shall see - Robert Wolterman
pins_t pins_info[] = {
  { "GND",       "U13_1",  0, BASE_METHOD_AS_IS, -1, -1},
  { "CHG-IN",    "U13_2",  0, BASE_METHOD_AS_IS, -1, -1},
  { "VCC-5V",    "U13_3",  0, BASE_METHOD_AS_IS, -1, -1},
  { "GND",       "U13_4",  0, BASE_METHOD_AS_IS, -1, -1},
  { "VCC-3V3",   "U13_5",  0, BASE_METHOD_AS_IS, -1, -1},
  { "TS",        "U13_6",  0, BASE_METHOD_AS_IS, -1, -1},
  { "VCC-1V8",   "U13_7",  0, BASE_METHOD_AS_IS, -1, -1},
  { "BAT",       "U13_8",  0, BASE_METHOD_AS_IS, -1, -1},
  { "TWI1-SDA",  "U13_9",  48, BASE_METHOD_AS_IS, -1, -1},
  { "PWRON",     "U13_10", 0, BASE_METHOD_AS_IS, -1, -1},
  { "TWI1-SCK",  "U13_11", 47, BASE_METHOD_AS_IS, -1, -1},
  { "GND",       "U13_12", 0, BASE_METHOD_AS_IS, -1, -1},
  { "X1",        "U13_13", 0, BASE_METHOD_AS_IS, -1, -1},
  { "X2",        "U13_14", 0, BASE_METHOD_AS_IS, -1, -1},
  { "Y1",        "U13_15", 0, BASE_METHOD_AS_IS, -1, -1},
  { "Y2",        "U13_16", 0, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D2",    "U13_17", 98, BASE_METHOD_AS_IS, -1, -1},
  { "PWM0",      "U13_18", 34, BASE_METHOD_AS_IS, 0, -1},
  { "LCD-D4",    "U13_19", 100, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D3",    "U13_20", 99, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D6",    "U13_21", 102, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D5",    "U13_22", 101, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D10",   "U13_23", 106, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D7",    "U13_24", 103, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D12",   "U13_25", 108, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D11",   "U13_26", 107, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D14",   "U13_27", 110, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D13",   "U13_28", 109, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D18",   "U13_29", 114, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D15",   "U13_30", 111, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D20",   "U13_31", 116, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D19",   "U13_32", 115, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D22",   "U13_33", 118, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D21",   "U13_34", 117, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-CLK",   "U13_35", 120, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D23",   "U13_36", 119, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-VSYNC", "U13_37", 123, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-HSYNC", "U13_38", 122, BASE_METHOD_AS_IS, -1, -1},
  { "GND",       "U13_39", 0, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-DE",    "U13_40", 121, BASE_METHOD_AS_IS, -1, -1},
  { "GND",       "U14_1",  0, BASE_METHOD_AS_IS, -1, -1},
  { "VCC-5V",    "U14_2",  0, BASE_METHOD_AS_IS, -1, -1},
  { "UART1-TX",  "U14_3",  195, BASE_METHOD_AS_IS, -1, -1},
  { "HPL",       "U14_4",  0, BASE_METHOD_AS_IS, -1, -1},
  { "UART1-RX",  "U14_5",  196, BASE_METHOD_AS_IS, -1, -1},
  { "HPCOM",     "U14_6",  0, BASE_METHOD_AS_IS, -1, -1},
  { "FEL",       "U14_7",  0, BASE_METHOD_AS_IS, -1, -1},
  { "HPR",       "U14_8",  0, BASE_METHOD_AS_IS, -1, -1},
  { "VCC-3V3",   "U14_9",  0, BASE_METHOD_AS_IS, -1, -1},
  { "MICM",      "U14_10", 0, BASE_METHOD_AS_IS, -1, -1},
  { "LRADC",     "U14_11", 0, BASE_METHOD_AS_IS, -1, 0},
  { "MICIN1",    "U14_12", 0, BASE_METHOD_AS_IS, -1, -1},
  { "XIO-P0",    "U14_13", 0, BASE_METHOD_XIO, -1, -1},
  { "XIO-P1",    "U14_14", 1, BASE_METHOD_XIO, -1, -1},
  { "XIO-P2",    "U14_15", 2, BASE_METHOD_XIO, -1, -1},
  { "XIO-P3",    "U14_16", 3, BASE_METHOD_XIO, -1, -1},
  { "XIO-P4",    "U14_17", 4, BASE_METHOD_XIO, -1, -1},
  { "XIO-P5",    "U14_18", 5, BASE_METHOD_XIO, -1, -1},
  { "XIO-P6",    "U14_19", 6, BASE_METHOD_XIO, -1, -1},
  { "XIO-P7",    "U14_20", 7, BASE_METHOD_XIO, -1, -1},
  { "GND",       "U14_21", 0, BASE_METHOD_AS_IS, -1, -1},
  { "GND",       "U14_22", 0, BASE_METHOD_AS_IS, -1, -1},
  { "AP-EINT1",  "U14_23", 193, BASE_METHOD_AS_IS, -1, -1},
  { "AP-EINT3",  "U14_24", 35, BASE_METHOD_AS_IS, -1, -1},
  { "TWI2-SDA",  "U14_25", 50, BASE_METHOD_AS_IS, -1, -1},
  { "TWI2-SCK",  "U14_26", 49, BASE_METHOD_AS_IS, -1, -1},
  { "CSIPCK",    "U14_27", 128, BASE_METHOD_AS_IS, -1, -1},
  { "CSICK",     "U14_28", 129, BASE_METHOD_AS_IS, -1, -1},
  { "CSIHSYNC",  "U14_29", 130, BASE_METHOD_AS_IS, 1, -1},
  { "CSIVSYNC",  "U14_30", 131, BASE_METHOD_AS_IS, -1, -1},
  { "CSID0",     "U14_31", 132, BASE_METHOD_AS_IS, 1, -1},
  { "CSID1",     "U14_32", 133, BASE_METHOD_AS_IS, -1, -1},
  { "CSID2",     "U14_33", 134, BASE_METHOD_AS_IS, -1, -1},
  { "CSID3",     "U14_34", 135, BASE_METHOD_AS_IS, -1, -1},
  { "CSID4",     "U14_35", 136, BASE_METHOD_AS_IS, -1, -1},
  { "CSID5",     "U14_36", 137, BASE_METHOD_AS_IS, -1, -1},
  { "CSID6",     "U14_37", 138, BASE_METHOD_AS_IS, -1, -1},
  { "CSID7",     "U14_38", 139, BASE_METHOD_AS_IS, -1, -1},
  { "GND",       "U14_39", 0, BASE_METHOD_AS_IS, -1, -1},
  { "GND",       "U14_40", 0, BASE_METHOD_AS_IS, -1, -1},
    { NULL, NULL, 0 }
};


// CREDIT FOR THIS FUNCTION DUE TO HOWIE KATZ OF NTC AND STEVE FORD
// THIS WILL FIND THE PROPER XIO BASE SYSFS NUMBER
// PORTED TO C FORM HOWIE'S PYTHON CODE WITH THE HELP OF STEVE:
// https://gist.github.com/howientc/606545e0ff47e2cda61f14fca5c46eee
// HAT TIP TO:
// http://stackoverflow.com/questions/8149569/scan-a-directory-to-find-files-in-c
// http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
#define GPIO_PATH "/sys/class/gpio"
#define EXPANDER "pcf8574a\n"
int num_get_xio_base = 0;  /* for self-test purposes */
int get_xio_base(void)
{
  char label_file[FILENAME_BUFFER_SIZE];
  FILE *label_fp;
  char base_file[FILENAME_BUFFER_SIZE];
  FILE *base_fp;
  char input_line[80];
  // Makes use of static variable xio_base_address to maintain state between
  // calls.  First time this is called, xio_base_address is -1, so the actual
  // base address is calculated and stored in xio_base_address.  Subsequent
  // calls just use the previously-calculated value.
  static int xio_base_address = -1;

  DIR *dir;
  struct dirent *ent;
  struct stat sbuf;

  if (xio_base_address == -1 && (dir = opendir (GPIO_PATH)) != NULL) {
    while (xio_base_address == -1 && (ent = readdir (dir)) != NULL) {
      lstat(ent->d_name,&sbuf);
      if (S_ISDIR(sbuf.st_mode)) {
        if (strcmp(".",ent->d_name) == 0 || strcmp("..",ent->d_name) == 0) {
          continue;  /* skip "." and ".." entries */
        }
        snprintf(label_file, sizeof(label_file), "%s/%s/label", GPIO_PATH, ent->d_name); BUF2SMALL(label_file);
        label_fp = fopen(label_file, "r");
        if (label_fp != NULL) {
          char *s = fgets(input_line, sizeof(input_line), label_fp); BUF2SMALL(input_line); ASSRT(s);
          fclose(label_fp);
          if (strcmp(input_line, EXPANDER) == 0) {
            /* Found the expander, get the contents of base */
            snprintf(base_file, sizeof(base_file), "%s/%s/base", GPIO_PATH, ent->d_name); BUF2SMALL(base_file);
            base_fp = fopen(base_file, "r");  ASSRT(base_fp != NULL);
            s = fgets(input_line, sizeof(input_line), base_fp);
              ASSRT(s == input_line && strlen(input_line) < sizeof(input_line)-1);
            fclose(base_fp);
            xio_base_address = atoi(input_line);  /* remember the result */
            num_get_xio_base++;  /* for self-test purposes */
          }
        }  /* if label file is open */
      }  /* if isdir */
    }  /* while */
    closedir (dir);
  }

  return xio_base_address; 
}  /* get_xio_base */


int gpio_number(pins_t *pin)
{
  int gpio_num = -1;

  switch (pin->base_method) {
    case BASE_METHOD_AS_IS:
      gpio_num = pin->gpio;
      break;

    case BASE_METHOD_XIO:
      gpio_num = pin->gpio + get_xio_base();
      break;
  }

  ASSRT(gpio_num != -1);
  return gpio_num;
}  /* gpio_number */


int lookup_gpio_by_key(const char *key)
{
  pins_t *p;
  for (p = pins_info; p->key != NULL; ++p) {
      if (strcmp(p->key, key) == 0) {
          return gpio_number(p);
      }
  }
  return 0;
}

int lookup_gpio_by_name(const char *name)
{
  pins_t *p;
  for (p = pins_info; p->name != NULL; ++p) {
      if (strcmp(p->name, name) == 0) {
          return gpio_number(p);
      }
  }
  return 0;
}

int lookup_ain_by_key(const char *key)
{
  pins_t *p;
  for (p = pins_info; p->key != NULL; ++p) {
      if (strcmp(p->key, key) == 0) {
        if (p->ain == -1) {
          return -1;
        } else {
          return p->ain;
        }
      }
  }
  return -1;
}

int lookup_ain_by_name(const char *name)
{
  pins_t *p;
  for (p = pins_info; p->name != NULL; ++p) {
      if (strcmp(p->name, name) == 0) {
        if (p->ain == -1) {
          return -1;
        } else {
          return p->ain;
        }
      }
  }
  return -1;
}

int copy_key_by_key(const char *input_key, char *key)
{
    pins_t *p;
    for (p = pins_info; p->key != NULL; ++p) {
        if (strcmp(p->key, input_key) == 0) {
            strncpy(key, p->key, 7);
            key[7] = '\0';
            return 1;
        }
    }
    return 0;
}

int copy_pwm_key_by_key(const char *input_key, char *key)
{
    pins_t *p;
    for (p = pins_info; p->key != NULL; ++p) {
        if (strcmp(p->key, input_key) == 0) {
            //validate it's a valid pwm pin
            if (p->pwm_mux_mode == -1)
                return 0;

            strncpy(key, p->key, 7);
            key[7] = '\0';
            return 1;
        }
    }
    return 0;
}

int get_key_by_name(const char *name, char *key)
{
    pins_t *p;
    for (p = pins_info; p->name != NULL; ++p) {
        if (strcmp(p->name, name) == 0) {
            strncpy(key, p->key, 7);
            key[7] = '\0';
            return 1;
        }
    }
    return 0;
}

int get_pwm_key_by_name(const char *name, char *key)
{
    pins_t *p;
    for (p = pins_info; p->name != NULL; ++p) {
        if (strcmp(p->name, name) == 0) {
            //validate it's a valid pwm pin
            if (p->pwm_mux_mode == -1)
                return 0;

            strncpy(key, p->key, 7);
            key[7] = '\0';
            return 1;
        }
    }
    return 0;
}

int get_gpio_number(const char *key, unsigned int *gpio)
{
    *gpio = lookup_gpio_by_key(key);

    if (!*gpio) {
        *gpio = lookup_gpio_by_name(key);
    }

    return 0;
}

int get_key(const char *input, char *key)
{
    if (!copy_key_by_key(input, key)) {
        return get_key_by_name(input, key);
    }

    return 1;
}

int get_pwm_key(const char *input, char *key)
{
    if (!copy_pwm_key_by_key(input, key)) {
        return get_pwm_key_by_name(input, key);
    }

    return 1;
}

int get_adc_ain(const char *key, unsigned int *ain)
{
    *ain = lookup_ain_by_key(key);

    if (*ain == -1) {
        *ain = lookup_ain_by_name(key);

        if (*ain == -1) {
          return 0;
        }
    }

    return 1;
}

int build_path(const char *partial_path, const char *prefix, char *full_path, size_t full_path_len)
{
    DIR *dp;
    struct dirent *ep;

    dp = opendir (partial_path);
    if (dp != NULL) {
        while ((ep = readdir (dp))) {
            // Enforce that the prefix must be the first part of the file
            char* found_string = strstr(ep->d_name, prefix);

            if (found_string != NULL && (ep->d_name - found_string) == 0) {
                snprintf(full_path, full_path_len, "%s/%s", partial_path, ep->d_name);
                closedir (dp);
                return 1;
            }
        }
        closedir (dp);
    } else {
        return 0;
    }

    return 0;
}

int get_spi_bus_path_number(unsigned int spi)
{
  char path[FILENAME_BUFFER_SIZE];
  char ocp_dir[FILENAME_BUFFER_SIZE];

  build_path("/sys/devices", "ocp", ocp_dir, sizeof(ocp_dir)); BUF2SMALL(ocp_dir);

  if (spi == 0) {
    snprintf(path, sizeof(path), "%s/48030000.spi/spi_master/spi1", ocp_dir); BUF2SMALL(path);
  } else {
    snprintf(path, sizeof(path), "%s/481a0000.spi/spi_master/spi1", ocp_dir); BUF2SMALL(path);
  }

  DIR* dir = opendir(path);
  if (dir) {
    closedir(dir);
    //device is using /dev/spidev1.x
    return 1;
  } else if (ENOENT == errno) {
    //device is using /dev/spidev2.x
    return 2;
  } else {
    return -1;
  }
}
