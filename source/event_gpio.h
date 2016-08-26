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

#define NO_EDGE      0
#define RISING_EDGE  1
#define FALLING_EDGE 2
#define BOTH_EDGE    3

#define INPUT  0
#define OUTPUT 1
#define ALT0   4

#define HIGH 1
#define LOW  0

#define MAX_FILENAME 50

#define PUD_OFF  0
#define PUD_DOWN 1
#define PUD_UP   2

int gpio_export(int gpio);
int gpio_unexport(int gpio);
void exports_cleanup(void);
int gpio_set_direction(int gpio, unsigned int in_flag);
int gpio_get_direction(int gpio, unsigned int *value);
int gpio_set_value(int gpio, unsigned int value);
int gpio_get_value(int gpio, unsigned int *value);
int fd_lookup(int gpio);
int open_value_file(int gpio);

int fde_lookup(int gpio);
int open_edge_file(int gpio);
int gpio_set_edge(int gpio, unsigned int edge);
int gpio_get_edge(int gpio);
int add_edge_detect(int gpio, unsigned int edge);
void remove_edge_detect(int gpio);
int add_edge_callback(int gpio, int edge, void (*func)(int gpio, void* data), void* data);
int event_detected(int gpio);
int gpio_event_add(int gpio);
int gpio_event_remove(int gpio);
int gpio_is_evented(int gpio);
int event_initialise(void);
void event_cleanup(void);
int blocking_wait_for_edge(int gpio, unsigned int edge);
