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

#define MODE_UNKNOWN -1
#define BOARD        10
#define BCM          11

#define ARRAY_SIZE(a)  (sizeof(a) / sizeof(a[0]))

// See http://blog.geeky-boy.com/2016/06/of-compiler-warnings-and-asserts-in.html
#define ASSRT(cond_expr) do { \
  if (!(cond_expr)) { \
    fprintf(stderr, "ASSRT failed at %s:%d (%s)", __FILE__, __LINE__, #cond_expr); \
    fflush(stderr); \
    abort(); \
} } while (0)

#define FILENAME_BUFFER_SIZE 128

int setup_error;
int module_setup;

int get_xio_base(void);
int lookup_gpio_by_key(const char *key);
int lookup_gpio_by_name(const char *name);
int lookup_ain_by_key(const char *key);
int lookup_ain_by_name(const char *name);
int copy_key_by_key(const char *input_key, char *key);
int copy_pwm_key_by_key(const char *input_key, char *key);
int get_key_by_name(const char *name, char *key);
int get_pwm_key_by_name(const char *name, char *key);
int get_gpio_number(const char *key, unsigned int *gpio);
int get_key(const char *input, char *key);
int get_pwm_key(const char *input, char *key);
int get_adc_ain(const char *key, unsigned int *ain);
int build_path(const char *partial_path, const char *prefix, char *full_path, size_t full_path_len);
int get_spi_bus_path_number(unsigned int spi);
