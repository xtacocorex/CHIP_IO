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

#include <dirent.h>
#include <time.h>
#include "common.h"
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

int setup_error = 0;
int module_setup = 0;

pins_t pins_info[] = {
  { "GND",       "GND",         "U13_1",   -1, BASE_METHOD_AS_IS, -1, -1},
  { "CHG-IN",    "CHG-IN",      "U13_2",   -1, BASE_METHOD_AS_IS, -1, -1},
  { "VCC-5V",    "VCC-5V",      "U13_3",   -1, BASE_METHOD_AS_IS, -1, -1},
  { "GND",       "GND",         "U13_4",   -1, BASE_METHOD_AS_IS, -1, -1},
  { "VCC-3V3",   "VCC-3V3",     "U13_5",   -1, BASE_METHOD_AS_IS, -1, -1},
  { "TS",        "TS",          "U13_6",   -1, BASE_METHOD_AS_IS, -1, -1},
  { "VCC-1V8",   "VCC-1V8",     "U13_7",   -1, BASE_METHOD_AS_IS, -1, -1},
  { "BAT",       "BAT",         "U13_8",   -1, BASE_METHOD_AS_IS, -1, -1},
  { "TWI1-SDA",  "KPD-I2C-SDA", "U13_9",   48, BASE_METHOD_AS_IS, -1, -1},
  { "PWRON",     "PWRON",       "U13_10",  -1, BASE_METHOD_AS_IS, -1, -1},
  { "TWI1-SCK",  "KPD-I2C-SCL", "U13_11",  47, BASE_METHOD_AS_IS, -1, -1},
  { "GND",       "GND",         "U13_12",  -1, BASE_METHOD_AS_IS, -1, -1},
  { "X1",        "X1",          "U13_13",  -1, BASE_METHOD_AS_IS, -1, -1},
  { "X2",        "X2",          "U13_14",  -1, BASE_METHOD_AS_IS, -1, -1},
  { "Y1",        "Y1",          "U13_15",  -1, BASE_METHOD_AS_IS, -1, -1},
  { "Y2",        "Y2",          "U13_16",  -1, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D2",    "LCD-D2",      "U13_17",  98, BASE_METHOD_AS_IS, -1, -1},
  { "PWM0",      "PWM0",        "U13_18",  34, BASE_METHOD_AS_IS,  0, -1},
  { "LCD-D4",    "LCD-D4",      "U13_19", 100, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D3",    "LCD-D3",      "U13_20",  99, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D6",    "LCD-D6",      "U13_21", 102, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D5",    "LCD-D5",      "U13_22", 101, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D10",   "LCD-D10",     "U13_23", 106, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D7",    "LCD-D7",      "U13_24", 103, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D12",   "LCD-D12",     "U13_25", 108, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D11",   "LCD-D11",     "U13_26", 107, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D14",   "LCD-D14",     "U13_27", 110, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D13",   "LCD-D13",     "U13_28", 109, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D18",   "LCD-D18",     "U13_29", 114, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D15",   "LCD-D15",     "U13_30", 111, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D20",   "LCD-D20",     "U13_31", 116, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D19",   "LCD-D19",     "U13_32", 115, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D22",   "LCD-D22",     "U13_33", 118, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D21",   "LCD-D21",     "U13_34", 117, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-CLK",   "LCD-CLK",     "U13_35", 120, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-D23",   "LCD-D23",     "U13_36", 119, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-VSYNC", "LCD-VSYNC",   "U13_37", 123, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-HSYNC", "LCD-HSYNC",   "U13_38", 122, BASE_METHOD_AS_IS, -1, -1},
  { "GND",       "GND",         "U13_39",  -1, BASE_METHOD_AS_IS, -1, -1},
  { "LCD-DE",    "LCD-DE",      "U13_40", 121, BASE_METHOD_AS_IS, -1, -1},
  { "GND",       "GND",         "U14_1",   -1, BASE_METHOD_AS_IS, -1, -1},
  { "VCC-5V",    "VCC-5V",      "U14_2",   -1, BASE_METHOD_AS_IS, -1, -1},
  { "UART1-TX",  "UART-TX",     "U14_3",  195, BASE_METHOD_AS_IS, -1, -1},
  { "HPL",       "HPL",         "U14_4",   -1, BASE_METHOD_AS_IS, -1, -1},
  { "UART1-RX",  "UART-RX",     "U14_5",  196, BASE_METHOD_AS_IS, -1, -1},
  { "HPCOM",     "HPCOM",       "U14_6",   -1, BASE_METHOD_AS_IS, -1, -1},
  { "FEL",       "FEL",         "U14_7",   -1, BASE_METHOD_AS_IS, -1, -1},
  { "HPR",       "HPR",         "U14_8",   -1, BASE_METHOD_AS_IS, -1, -1},
  { "VCC-3V3",   "VCC-3V3",     "U14_9",   -1, BASE_METHOD_AS_IS, -1, -1},
  { "MICM",      "MICM",        "U14_10",  -1, BASE_METHOD_AS_IS, -1, -1},
  { "LRADC",     "ADC",         "U14_11",  -1, BASE_METHOD_AS_IS, -1,  0},
  { "MICIN1",    "MICIN1",      "U14_12",  -1, BASE_METHOD_AS_IS, -1, -1},
  { "XIO-P0",    "XIO-P0",      "U14_13",   0, BASE_METHOD_XIO,   -1, -1},
  { "XIO-P1",    "XIO-P1",      "U14_14",   1, BASE_METHOD_XIO,   -1, -1},
  { "XIO-P2",    "GPIO1",       "U14_15",   2, BASE_METHOD_XIO,   -1, -1},
  { "XIO-P3",    "GPIO2",       "U14_16",   3, BASE_METHOD_XIO,   -1, -1},
  { "XIO-P4",    "GPIO3",       "U14_17",   4, BASE_METHOD_XIO,   -1, -1},
  { "XIO-P5",    "GPIO4",       "U14_18",   5, BASE_METHOD_XIO,   -1, -1},
  { "XIO-P6",    "GPIO5",       "U14_19",   6, BASE_METHOD_XIO,   -1, -1},
  { "XIO-P7",    "GPIO6",       "U14_20",   7, BASE_METHOD_XIO,   -1, -1},
  { "GND",       "GND",         "U14_21",  -1, BASE_METHOD_AS_IS, -1, -1},
  { "GND",       "GND",         "U14_22",  -1, BASE_METHOD_AS_IS, -1, -1},
  { "AP-EINT1",  "KPD-INT",     "U14_23", 193, BASE_METHOD_AS_IS, -1, -1},
  { "AP-EINT3",  "AP-INT3",     "U14_24",  35, BASE_METHOD_AS_IS, -1, -1},
  { "TWI2-SDA",  "I2C-SDA",     "U14_25",  50, BASE_METHOD_AS_IS, -1, -1},
  { "TWI2-SCK",  "I2C-SCL",     "U14_26",  49, BASE_METHOD_AS_IS, -1, -1},
  { "CSIPCK",    "SPI-SEL",     "U14_27", 128, BASE_METHOD_AS_IS, -1, -1},
  { "CSICK",     "SPI-CLK",     "U14_28", 129, BASE_METHOD_AS_IS, -1, -1},
  { "CSIHSYNC",  "SPI-MOSI",    "U14_29", 130, BASE_METHOD_AS_IS,  1, -1},
  { "CSIVSYNC",  "SPI-MISO",    "U14_30", 131, BASE_METHOD_AS_IS, -1, -1},
  { "CSID0",     "CSID0",       "U14_31", 132, BASE_METHOD_AS_IS,  1, -1},
  { "CSID1",     "CSID1",       "U14_32", 133, BASE_METHOD_AS_IS, -1, -1},
  { "CSID2",     "CSID2",       "U14_33", 134, BASE_METHOD_AS_IS, -1, -1},
  { "CSID3",     "CSID3",       "U14_34", 135, BASE_METHOD_AS_IS, -1, -1},
  { "CSID4",     "CSID4",       "U14_35", 136, BASE_METHOD_AS_IS, -1, -1},
  { "CSID5",     "CSID5",       "U14_36", 137, BASE_METHOD_AS_IS, -1, -1},
  { "CSID6",     "CSID6",       "U14_37", 138, BASE_METHOD_AS_IS, -1, -1},
  { "CSID7",     "CSID7",       "U14_38", 139, BASE_METHOD_AS_IS, -1, -1},
  { "GND",       "GND",         "U14_39",  -1, BASE_METHOD_AS_IS, -1, -1},
  { "GND",       "GND",         "U14_40",  -1, BASE_METHOD_AS_IS, -1, -1},
  { NULL,        NULL,          NULL,      -1, 0,                 -1, -1}
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

/* returns -1 for error */
int get_xio_base(void)
{
  char label_file[FILENAME_BUFFER_SIZE];
  FILE *label_fp;
  char base_file[FILENAME_BUFFER_SIZE];
  FILE *base_fp;
  // Makes use of static variable xio_base_address to maintain state between
  // calls.  First time this is called, xio_base_address is -1, so the actual
  // base address is calculated and stored in xio_base_address.  Subsequent
  // calls just use the previously-calculated value.
  static int xio_base_address = -1;

  DIR *dir;
  struct dirent *ent;
  struct stat sbuf;

  if (xio_base_address != -1)
    return xio_base_address;

  dir = opendir (GPIO_PATH);
  if (dir == NULL) {
    char err[256];
    snprintf(err, sizeof(err), "get_xio_base: could not open '%s' (%s)", GPIO_PATH, strerror(errno));
    add_error_msg(err);
    return -1;
  }

  while (xio_base_address == -1 && (ent = readdir (dir)) != NULL) {
    lstat(ent->d_name,&sbuf);
    if (S_ISDIR(sbuf.st_mode)) {
      if (strcmp(".",ent->d_name) == 0 || strcmp("..",ent->d_name) == 0) {
        continue;  /* skip "." and ".." entries */
      }

      snprintf(label_file, sizeof(label_file), "%s/%s/label", GPIO_PATH, ent->d_name); BUF2SMALL(label_file);
      label_fp = fopen(label_file, "r");
      if (label_fp != NULL) {
        char input_line[80];  input_line[0] = '\0';
        char *s = fgets(input_line, sizeof(input_line), label_fp); BUF2SMALL(input_line);
        fclose(label_fp);
        if (s != NULL && strcmp(input_line, EXPANDER) == 0) {
          /* Found the expander, get the contents of base */
          snprintf(base_file, sizeof(base_file), "%s/%s/base", GPIO_PATH, ent->d_name); BUF2SMALL(base_file);
          base_fp = fopen(base_file, "r");
          if (base_fp == NULL) {
            char err[256];
            snprintf(err, sizeof(err), "get_xio_base: could not open '%s' (%s)", base_file, strerror(errno));
            add_error_msg(err);
            break;  /* error, exit loop */
          }
          s = fgets(input_line, sizeof(input_line), base_fp); BUF2SMALL(input_line);
          fclose(base_fp);
          if (s == NULL) {
            char err[256];
            snprintf(err, sizeof(err), "get_xio_base: could not read '%s' (%s)", base_file, strerror(errno));
            add_error_msg(err);
            break;  /* error, exit loop */
          }
          /* Remember the value in the static local. */
          xio_base_address = atoi(input_line);
        }
      }  /* if label file is open */
    }  /* if isdir */
  }  /* while */
  closedir (dir);

  return xio_base_address; 
}  /* get_xio_base */


int gpio_number(pins_t *pin)
{
  int gpio_num = -1;
  int xio_base;

  switch (pin->base_method) {
    case BASE_METHOD_AS_IS:
      gpio_num = pin->gpio;
      break;

    case BASE_METHOD_XIO:
      xio_base = get_xio_base();
      if (xio_base <= 0) {
        char err[256];  snprintf(err, sizeof(err), "gpio_number: %d found for %s", xio_base, pin->name);
        add_error_msg(err);
        break;  /* error, exit switch */
      }
      gpio_num = pin->gpio + xio_base;
      break;
  }

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
  return -1;
}

int lookup_gpio_by_name(const char *name)
{
  pins_t *p;
  for (p = pins_info; p->name != NULL; ++p) {
      if (strcmp(p->name, name) == 0) {
          return gpio_number(p);
      }
  }
  return -1;
}

int lookup_gpio_by_altname(const char *altname)
{
  pins_t *p;
  for (p = pins_info; p->altname != NULL; ++p) {
      if (strcmp(p->altname, altname) == 0) {
          return gpio_number(p);
      }
  }
  return -1;
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
            printf("## FOUND PWM KEY, VALIDATING MUX MODE ##\n");
            //validate it's a valid pwm pin
            if (p->pwm_mux_mode == -1)
                return 0;

            printf("## PWM KEY IS VALID ##\n");
            strncpy(key, p->key, 7);
            key[7] = '\0';
            return 1;
        }
    }
    return 0;
}

int get_gpio_number(const char *key, int *gpio)
{
    int status = 0;  /* assume success */
    *gpio = lookup_gpio_by_key(key);

    if (*gpio <= 0) {
        *gpio = lookup_gpio_by_name(key);
        if (*gpio <= 0) {
             *gpio = lookup_gpio_by_altname(key);
             if (*gpio <=0) {
                 status = -1;  /* error */
             }
        }
    }
    return status;
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
            // Enforce that the prefix must be the first part of the file name
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


// We do not know at compile time how many GPIOs there are, so it is not safe
// to declare per-GPIO arrays with a static size.  The "dyn_int_array_*"
// functions implement a dynamic integer array which grows as needed at run
// time.  Users of a dynamic array do not access the array directly, but by
// access methods.
//
// To use, declare a dyn_int_array_t pointer initialized to NULL:
//   dyn_int_array_t *my_per_gpio_array = NULL;
// If declared as a local to a function, consider making it "static":
//   static dyn_int_array_t *my_per_gpio_array = NULL;
// The access methods must be passed a pointer to this pointer.
// Then when reading and writing to array elements, it is usually important
// to supply a "default value".  This is so that as the array grows, the
// as-yet unused elements can be initialzied to a desired value.
//   my_index = 100;
//   dyn_int_array_set(&my_per_gpio_array, my_index, my_value, -1);
// This initializes elements 0-99 to -1, and element 100 to my_value.  For
// efficiency purposes, it grows the array to half again larger than it needs
// to.  Thus, elements 101-149 will be initialized to -1.
//
// If an element past the end of the array is read, the array is once again
// grown, with initialization:
//   my_index = 500;
//   int val = dyn_int_array_get(&my_per_gpio_array, my_index, -1);
// This grows the array to 749 (half again larger than 500) and initializes all
// the new elements to -1, including element 500, which is then returned.

/* internal function; only called by access methods. */
dyn_int_array_t *dyn_int_array_create(int initial_num_elements, int initial_val)
{
  dyn_int_array_t *new_array = (dyn_int_array_t *)malloc(sizeof(dyn_int_array_t));
  ASSRT(new_array != NULL);  /* out of memory */

  new_array->num_elements = initial_num_elements;
  new_array->content = (int *)malloc(initial_num_elements * sizeof(int));
  ASSRT(new_array->content != NULL);  /* out of memory */

  int i;
  for (i = 0; i < initial_num_elements; i++)
    new_array->content[i] = initial_val;

  return new_array;
}  /* dyn_int_array_create */


void dyn_int_array_set(dyn_int_array_t **in_array, int i, int val, int initial_val)
{
  dyn_int_array_t *array = *in_array;
  if (array == NULL) {  /* if array hasn't been created yet */
    array = dyn_int_array_create((i+1) * 3 / 2, initial_val);
    *in_array = array;
  }

  if (i >= array->num_elements) {
    int new_num_elements = (i+1) * 3 / 2;  /* half-again larger than current request */
    array->content = realloc(array->content, new_num_elements * sizeof(int));
    ASSRT(array->content != NULL);  /* out of memory */
    // Zero out the newly allocated elements.
    while (array->num_elements < new_num_elements) {
      array->content[array->num_elements] = initial_val;
      array->num_elements ++;
    }
  }

  array->content[i] = val;
}  /* dyn_int_array_set */


int dyn_int_array_get(dyn_int_array_t **in_array, int i, int initial_val)
{
  dyn_int_array_t *array = *in_array;
  if (array == NULL) {  /* if array hasn't been created yet */
    array = dyn_int_array_create((i+1) * 3 / 2, initial_val);
    *in_array = array;
  }

  if (i >= array->num_elements) {
    int new_num_elements = (i+1) * 3 / 2;  /* half-again larger than current request */
    array->content = realloc(array->content, new_num_elements * sizeof(int));
    ASSRT(array->content != NULL);  /* out of memory */
    // Zero out the newly allocated elements.
    while (array->num_elements < new_num_elements) {
      array->content[array->num_elements] = initial_val;
      array->num_elements ++;
    }
  }

  return array->content[i];
}  /* dyn_int_array_get */


void dyn_int_array_delete(dyn_int_array_t **in_array)
{
  dyn_int_array_t *array = *in_array;
  free(array->content);
  free(array);
}  /* dyn_int_array_delete */


char error_msg_buff[1024];  /* written to when an error must be returned */

void clear_error_msg(void)
{
  error_msg_buff[0] = '\0';
}


char *get_error_msg(void)
{
  return error_msg_buff;
}


void add_error_msg(char *msg)
{
  int buff_len = strnlen(error_msg_buff, sizeof(error_msg_buff));  ASSRT(buff_len < sizeof(error_msg_buff));
  int remaining = sizeof(error_msg_buff) - buff_len - 1;  /* leave room for final null */

  if (buff_len > 0 && remaining > 0) {  /* include newline between messages */
    error_msg_buff[buff_len] = '\n';
    buff_len ++;
    remaining --;
  }

  char *start = &error_msg_buff[buff_len];  /* points at null in the buffer */
  int msg_len = strlen(msg);
  if (msg_len >= remaining)
    msg_len = remaining;  /* don't overflow buffer; truncate message */
  if (msg_len > 0)
    memcpy(start, msg, msg_len);

  start[msg_len] = '\0';
}
