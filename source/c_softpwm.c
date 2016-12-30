/*
Copyright (c) 2016 Brady Hurlburt

Original BBIO Author Justin Cooper
Modified for CHIP_IO Author Brady Hurlburt

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

#define PERIOD 0
#define DUTY 1

int pwm_initialized = 0;

struct pwm_params
{
  float duty;
  float freq;
  bool enabled;
  bool stop_flag;
  int polarity;
};

struct softpwm
{
    char key[KEYLEN+1]; /* leave room for terminating NUL byte */
    int gpio;
    struct pwm_params params;
    pthread_mutex_t* params_lock;
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
    struct softpwm *pwm;

    if (freq <= 0.0)
        return -1;

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        return -1;
    }

    pthread_mutex_lock(pwm->params_lock);
    pwm->params.freq = freq;
    pthread_mutex_unlock(pwm->params_lock);

    return 0;
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

    pthread_mutex_lock(pwm->params_lock);
    pwm->params.polarity = polarity;
    pthread_mutex_unlock(pwm->params_lock);

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

    pthread_mutex_lock(pwm->params_lock);
    pwm->params.duty = duty;
    pthread_mutex_unlock(pwm->params_lock);

    return 0;
}

void *softpwm_thread_toggle(void *arg)
{
  struct softpwm *pwm = (struct softpwm *)arg;
  int gpio = pwm->gpio;
  struct timespec tim_on;
  struct timespec tim_off;
  unsigned int sec;
  unsigned int period_ns;
  unsigned int on_ns;
  unsigned int off_ns;

  /* Used to determine if something has
   * has changed
   */
  float freq_local = 0;
  float duty_local = 0;
  unsigned int polarity_local = 0;
  bool stop_flag_local = false;
  bool enabled_local = false;
  bool recalculate_timing = false;

  while (!stop_flag_local) {
    /* Take a snapshot of the parameter block */
    pthread_mutex_lock(pwm->params_lock);
    if ((freq_local != pwm->params.freq) || (duty_local != pwm->params.duty)) {
      recalculate_timing = true;
    }
    freq_local = pwm->params.freq;
    duty_local = pwm->params.duty;
    enabled_local = pwm->params.enabled;
    stop_flag_local = pwm->params.stop_flag;
    polarity_local = pwm->params.polarity;
    pthread_mutex_unlock(pwm->params_lock);

    /* If freq or duty has been changed, update the
     * sleep times
     */
    if (recalculate_timing) {
      period_ns = (unsigned long)(1e9 / freq_local);
      on_ns = (unsigned long)(period_ns * (duty_local/100));
      off_ns = period_ns - on_ns;
      sec = (unsigned int)(on_ns/1e9); /* Intentional truncation */
      tim_on.tv_sec = sec;
      tim_on.tv_nsec = on_ns - (sec*1e9);
      sec = (unsigned int)(off_ns/1e9); /* Intentional truncation */
      tim_off.tv_sec = sec;
      tim_off.tv_nsec = off_ns - (sec*1e9);
      recalculate_timing = false;
    }

    if (enabled_local)
    {

      /* Force 0 duty cycle to be 0 */
      if (duty_local != 0)
      {
        /* Set gpio */
        if (!polarity_local)
          gpio_set_value(gpio, HIGH);
        else
          gpio_set_value(gpio, LOW);
      }

      nanosleep(&tim_on, NULL);

      /* Force 100 duty cycle to be 100 */
      if (duty_local != 100)
      {
        /* Unset gpio */
        if (!polarity_local)
          gpio_set_value(gpio, LOW);
        else
          gpio_set_value(gpio, HIGH);
      }

      nanosleep(&tim_off, NULL);
    }  /* if enabled_local */
  }  /* while !stop_flag_local */

  if (!polarity_local)
    gpio_set_value(gpio, LOW);
  else
    gpio_set_value(gpio, HIGH);

  /* This pwm has been disabled */
  pthread_exit(NULL);
}

int softpwm_start(const char *key, float duty, float freq, int polarity)
{
    struct softpwm *new_pwm, *pwm;
    pthread_t new_thread;
    pthread_mutex_t *new_params_lock;
    int gpio;
    int ret;

    if (get_gpio_number(key, &gpio) < 0)
        return -1;
    if (gpio_export(gpio) < 0) {
       char err[2000];
       snprintf(err, sizeof(err), "Error setting up softpwm on pin %d, maybe already exported? (%s)", gpio, get_error_msg());
       add_error_msg(err);
       return -1;
    }
    if (gpio_set_direction(gpio, OUTPUT) < 0)
        return -1;

    // add to list
    new_pwm = malloc(sizeof(struct softpwm));  ASSRT(new_pwm != NULL);
    new_params_lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if (new_pwm == 0) {
        return -1; // out of memory
    }
    pthread_mutex_init(new_params_lock, NULL);
    pthread_mutex_lock(new_params_lock);

    strncpy(new_pwm->key, key, KEYLEN);  /* can leave string unterminated */
    new_pwm->key[KEYLEN] = '\0'; /* terminate string */
    new_pwm->gpio = gpio;
    new_pwm->params.enabled = true;
    new_pwm->params.stop_flag = false;
    new_pwm->params_lock = new_params_lock;
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
    pthread_mutex_unlock(new_params_lock);

    ASSRT(softpwm_set_duty_cycle(new_pwm->key, duty) == 0);
    ASSRT(softpwm_set_frequency(new_pwm->key, freq) == 0);
    ASSRT(softpwm_set_polarity(new_pwm->key, polarity) == 0);

    pthread_mutex_lock(new_params_lock);
    // create thread for pwm
    ret = pthread_create(&new_thread, NULL, softpwm_thread_toggle, (void *)new_pwm);
    ASSRT(ret == 0);

    new_pwm->thread = new_thread;

    pthread_mutex_unlock(new_params_lock);

    return 1;
}

int softpwm_disable(const char *key)
{
    struct softpwm *pwm, *temp, *prev_pwm = NULL;

    // remove from list
    pwm = exported_pwms;
    while (pwm != NULL)
    {
        if (strcmp(pwm->key, key) == 0)
        {
            pthread_mutex_lock(pwm->params_lock);
            pwm->params.stop_flag = true;
            pthread_mutex_unlock(pwm->params_lock);
            pthread_join(pwm->thread, NULL);  /* wait for thread to exit */

            gpio_unexport(pwm->gpio);

            if (prev_pwm == NULL)
            {
                exported_pwms = pwm->next;
                prev_pwm = pwm;
            } else {
                prev_pwm->next = pwm->next;
            }

            temp = pwm;
            pwm = pwm->next;
            free(temp->params_lock);
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
