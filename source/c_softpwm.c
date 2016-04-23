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
#include <stdbool.h>
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

#define NOTCHIPHW

#define PERIOD 0
#define DUTY 1

int pwm_initialized = 0;

// pwm exports
struct softpwm
{
    char key[KEYLEN+1]; /* leave room for terminating NUL byte */

    float duty;
    float freq;
    pthread_t thread;
    struct softpwm *next;
    bool enabled;
    bool stop_flag;
    int polarity;
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
    struct softpwm *pwm;

    if (freq <= 0.0)
        return -1;

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        return -1;
    }

    pwm->freq = freq;

    return 1;
}

int softpwm_set_polarity(const char *key, int polarity) {
    struct softpwm *pwm;

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        return -1;
    }

    if (polarity < 0 || polarity > 1) {
        return -1;
    }

    pwm->polarity = polarity;

    return 0;
}

int softpwm_set_duty_cycle(const char *key, float duty) {;
    struct softpwm *pwm;

    if (duty < 0.0 || duty > 100.0)
        return -1;

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        return -1;
    }

    pwm->duty = duty;

    return 0;
}

void* softpwm_thread_toggle(void *key)
{
  struct softpwm *pwm;
  unsigned int gpio;
  struct timespec tim_on;
  struct timespec tim_off;
  unsigned int sec;
  unsigned int period_ns;
  unsigned int on_ns;
  unsigned int off_ns;

  /* Used to determine if something has
   * has changed
   */
  unsigned int freq_local = 0;
  unsigned int duty_local = 0;

  get_gpio_number(key, &gpio);
  pwm = lookup_exported_pwm((char*)key);

  while (pwm->enabled) {
    /* If freq or duty has been changed, update the
     * sleep times
     */
    if ((freq_local != pwm->freq) || (duty_local != pwm->duty)) {
      period_ns = (unsigned long)(1e9 / pwm->freq);
      on_ns = (unsigned long)(period_ns * (pwm->duty/100));
      off_ns = period_ns - on_ns;
      sec = (unsigned int)(on_ns/1e9); /* Intentional truncation */
      tim_on.tv_sec = sec;
      tim_on.tv_nsec = on_ns - (sec*1e9);
      sec = (unsigned int)(off_ns/1e9); /* Intentional truncation */
      tim_off.tv_sec = sec;
      tim_off.tv_nsec = off_ns - (sec*1e9);
      freq_local = pwm->freq;
      duty_local = pwm->duty;
    }

    /* Set gpio */
    if (pwm->polarity)
    {
#ifdef NOTCHIPHW
      printf("Setting gpio high");
#else
      gpio_set_value(gpio, HIGH);
#endif

    }
    else
    {
#ifdef NOTCHIPHW
      printf("Setting gpio low");
#else
      //gpio_set_value(gpio, LOW);
#endif
    }

    nanosleep(&tim_on, NULL);

    /* Unset gpio */
    if (pwm->polarity)
    {
#ifdef NOTCHIPHW
      printf("Setting gpio low");
#else
      gpio_set_value(gpio, LOW);
#endif
    }
    else
    {
#ifdef NOTCHIPHW
      printf("Setting gpio high");
#else
      gpio_set_value(gpio, HIGH);
#endif
    }

    nanosleep(&tim_off, NULL);
  }

  /* This pwm has been disabled */
  pthread_exit(NULL);
}

int softpwm_start(const char *key, float duty, float freq, int polarity)
{
    struct softpwm *new_pwm, *pwm;
    pthread_t *new_thread;
    unsigned int gpio;
    int ret;

    get_gpio_number(key, &gpio);
#ifndef NOTCHIPHW
    gpio_export(gpio);
    gpio_set_direction(gpio, OUTPUT);
#endif

    // add to list
    new_pwm = malloc(sizeof(struct softpwm));
    if (new_pwm == 0) {
        return -1; // out of memory
    }

    strncpy(new_pwm->key, key, KEYLEN);  /* can leave string unterminated */
    new_pwm->key[KEYLEN] = '\0'; /* terminate string */
    new_pwm->enabled = false;
    new_pwm->stop_flag = false;
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

    softpwm_set_duty_cycle(key, duty);
    softpwm_set_frequency(key, freq);
    softpwm_set_polarity(key, polarity);

    // create thread for pwm
    ret = pthread_create(new_thread, NULL, softpwm_thread_toggle, (void *)key);
    if (ret) {
       printf("ERROR; return code from pthread_create() is %d\n", ret);
       exit(-1);
    }
    new_pwm->thread = *new_thread;
    new_pwm->enabled = true;

    return 1;
}

int softpwm_disable(const char *key)
{
    struct softpwm *pwm, *temp, *prev_pwm = NULL;
    unsigned int gpio = 0;

    // Disable the PWM
    softpwm_set_frequency(key, 0);
    softpwm_set_polarity(key, 0);
    softpwm_set_duty_cycle(key, 0);

    // remove from list
    pwm = exported_pwms;
    while (pwm != NULL)
    {
        if (strcmp(pwm->key, key) == 0)
        {
            pwm->stop_flag = true;
            get_gpio_number(key, &gpio);
#ifndef NOTCHIPHW
            gpio_unexport(gpio);
#endif

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
