/*
Copyright (c) 2016 Robert Wolterman

Original BBIO Author Justin Cooper
Modified for CHIP_IO Author Robert Wolterman

This file incorporates work covered by the following copyright and
permission notice, all modified code adopts the original license:

Copyright (c) 2013 Adafruit
Author: Justin Cooper

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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "c_pwm.h"
#include "common.h"
#include "event_gpio.h"

#define KEYLEN 7

#define PERIOD 0
#define DUTY 1

int pwm_initialized = 0;

// pwm exports
struct softpwm
{
    char key[KEYLEN+1]; /* leave room for terminating NUL byte */
    unsigned long on_ns;
    unsigned long off_ns;
    pthread_t thread;
    struct softpwm *next;
};
struct softpwm *exported_pwms = NULL;

struct softpwm *lookup_exported_pwm(const char *key)
{
    struct softpwm *pwm = exported_pwms;

    while (pwm != NULL)
    {
        if (strcmp(pwm->key, key) == 0) {
            return pwm;
        }
        pwm = pwm->next;
    }

    return NULL; /* standard for pointers */
}

int softpwm_set_frequency(const char *key, float freq) {
    int len;
    char buffer[20];
    unsigned long period_ns;
    struct softpwm *pwm;

    if (freq <= 0.0)
        return -1;

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        return -1;
    }

    period_ns = (unsigned long)(1e9 / freq);

    if (period_ns != pwm->period_ns) {
        pwm->period_ns = period_ns;

        len = snprintf(buffer, sizeof(buffer), "%lu", period_ns);
        write(pwm->period_fd, buffer, len);
    }

    return 1;
}

int softpwm_set_polarity(const char *key, int polarity) {
    int len;
    char buffer[9]; /* allow room for trailing NUL byte */
    struct softpwm *pwm;

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        return -1;
    }

    if (polarity < 0 || polarity > 1) {
        return -1;
    }

    if (polarity == 0) {
        len = snprintf(buffer, sizeof(buffer), "%s", "normal");
    }
    else
    {
        len = snprintf(buffer, sizeof(buffer), "%s", "inverted");
    }
    write(pwm->polarity_fd, buffer, len);

    return 0;
}

int softpwm_set_duty_cycle(const char *key, float duty) {
    int len;
    char buffer[20];
    struct softpwm *pwm;

    if (duty < 0.0 || duty > 100.0)
        return -1;

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        return -1;
    }

    pwm->duty = (unsigned long)(pwm->period_ns * (duty / 100.0));

    len = snprintf(buffer, sizeof(buffer), "%lu", pwm->duty);
    write(pwm->duty_fd, buffer, len);

    return 0;
}

int softpwm_set_enable(const char *key, int enable)
{
    int len;
    char buffer[20];
    struct softpwm *pwm;

    if (enable != 0 || enable != 1)
        return -1;

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        return -1;
    }

    pwm->enable = enable;

    len = snprintf(buffer, sizeof(buffer), "%d", pwm->enable);
    write(pwm->enable_fd, buffer, len);

    return 0;
}

void* softpwm_toggle(void *key)
{
  struct softpwm *pwm;
  unsigned int gpio;
  struct timespec tim;
  unsigned int sec;

  get_gpio_number(key, &gpio);
  pwm = lookup_exported_pwm((char*)key);

  while (1) {
    sec = (unsigned int)(pwm->on_ns/1e9);
    tim.tv_sec = sec;
    tim.tv_nsec = pwm->on_ns - (sec*1e9);
    gpio_set_value(gpio, HIGH);
    nanosleep(&tim, NULL);
    sec = (unsigned int)(pwm->off_ns/1e9);
    tim.tv_sec = sec;
    tim.tv_nsec = pwm->off_ns - (sec*1e9);
    gpio_set_value(gpio, LOW);
    nanosleep(&tim, NULL);
  }
}

int softpwm_start(const char *key, float duty, float freq, int polarity)
{
    struct softpwm *new_pwm, *pwm;
    pthread_t *new_thread;
    unsigned int gpio;
    unsigned long period_ns;
    unsigned long on_ns;
    int ret;

    get_gpio_number(key, &gpio);
    gpio_export(gpio);
    gpio_set_direction(gpio, OUTPUT);

    // add to list
    new_pwm = malloc(sizeof(struct softpwm));
    if (new_pwm == 0) {
        return -1; // out of memory
    }

    // calculate period and on time from freq
    period_ns = (unsigned long)(1e9 / freq);
    on_ns = (unsigned long)(period_ns * (duty/100));

    // create thread for pwm
    ret = pthread_create(new_thread, NULL, softpwm_toggle, (void *)key);
    if (ret){
       printf("ERROR; return code from pthread_create() is %d\n", ret);
       exit(-1);
    }

    strncpy(new_pwm->key, key, KEYLEN);  /* can leave string unterminated */
    new_pwm->key[KEYLEN] = '\0'; /* terminate string */
    new_pwm->on_ns = on_ns;
    new_pwm->off_ns = period_ns - on_ns;
    new_pwm->thread = *new_thread;
    new_pwm->next = NULL;

    if (exported_pwms == NULL)
    {
        // create new list
        exported_pwms = new_pwm;
    } else {
        // add to end of existing list
        pwm = exported_pwms;
        while (pwm->next != NULL)
            pwm = pwm->next;
        pwm->next = new_pwm;
    }

    softpwm_set_polarity(key, polarity);

    return 1;
}

int softpwm_disable(const char *key)
{
    struct softpwm *pwm, *temp, *prev_pwm = NULL;
    char fragment[18];

    int fd, len;
    char str_gpio[2];
    // Per https://github.com/NextThingCo/CHIP-linux/pull/4
    // we need to export 0 here to enable pwm0
    int gpio = 0;

    // Disable the PWM
    softpwm_set_frequency(key, 0);
    softpwm_set_polarity(key, 0);
    softpwm_set_enable(key, 0);
    softpwm_set_duty_cycle(key, 0);

    if ((fd = open("/sys/class/pwm/pwmchip0/unexport", O_WRONLY)) < 0)
    {
        return -1;
    }
    len = snprintf(str_gpio, sizeof(str_gpio), "%d", gpio);
    write(fd, str_gpio, len);
    close(fd);

    // remove from list
    pwm = exported_pwms;
    while (pwm != NULL)
    {
        if (strcmp(pwm->key, key) == 0)
        {
            //close the fd
            close(pwm->enable_fd);
            close(pwm->period_fd);
            close(pwm->duty_fd);
            close(pwm->polarity_fd);

            if (prev_pwm == NULL)
            {
                exported_pwms = pwm->next;
                prev_pwm = pwm;
            } else {
                prev_pwm->next = pwm->next;
            }

            temp = pwm;
            pwm = pwm->next;
            free(temp);
        } else {
            prev_pwm = pwm;
            pwm = pwm->next;
        }
    }
    return 0;
}

void softpwm_cleanup(void)
{
    while (exported_pwms != NULL) {
        softpwm_disable(exported_pwms->key);
    }
}
