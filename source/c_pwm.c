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
#include "c_pwm.h"
#include "common.h"

#define KEYLEN 7

#define PERIOD 0
#define DUTY 1
#define ENABLE 1
#define DISABLE 0

// Global variables
int pwm_initialized = 0;
int DEBUG = 0;
//int ENABLE = 1;
//int DISABLE = 0;

// pwm devices (future chip pro use)
struct pwm_dev
{
    char key[KEYLEN+1]; /* leave room for terminating NUL byte */
    int gpio;
    int initialized;
    struct pwm_dev *next;
};
struct pwm_dev *initialized_pwms = NULL;

struct pwm_dev *lookup_initialized_pwm(const char *key)
{
    struct pwm_dev *dev = initialized_pwms;
    while (dev != NULL)
    {
        if (strcmp(dev->key, key) == 0) {
            return dev;
        }
        dev = dev->next;
    }
    return NULL;
}

// pwm exports
struct pwm_exp
{
    char key[KEYLEN+1]; /* leave room for terminating NUL byte */
    int period_fd;
    int duty_fd;
    int polarity_fd;
    int enable_fd;
    int enable;
    unsigned long duty;
    unsigned long period_ns;
    struct pwm_exp *next;
};
struct pwm_exp *exported_pwms = NULL;

struct pwm_exp *lookup_exported_pwm(const char *key)
{
    struct pwm_exp *pwm = exported_pwms;

    while (pwm != NULL)
    {
        if (strcmp(pwm->key, key) == 0) {
            return pwm;
        }
        pwm = pwm->next;
    }
    return NULL; /* standard for pointers */
}

int initialize_pwm(void)
{
    if (!pwm_initialized) {
        int fd, len;
        char str_gpio[80];
        // Per https://github.com/NextThingCo/CHIP-linux/pull/4
        // we need to export 0 here to enable pwm0
        int gpio = 0;

        if (DEBUG)
            printf(" ** EXPORTING PWM0 **\n");
        if ((fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY)) < 0)
        {
            return -1;
        }
        len = snprintf(str_gpio, sizeof(str_gpio), "%d", gpio); BUF2SMALL(str_gpio);
        ssize_t s = write(fd, str_gpio, len);  ASSRT(s == len);
        if (DEBUG)
            printf(" ** IN initialize_pwm: s = %d, len = %d\n", s, len);
        close(fd);

        pwm_initialized = 1;
        return 1;
    }

    return 0;
}

int pwm_set_frequency(const char *key, float freq) {
    int len;
    int rtnval = -1;
    char buffer[80];
    unsigned long period_ns;
    struct pwm_exp *pwm;

    if (freq <= 0.0)
        return rtnval;

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        return rtnval;
    }

    period_ns = (unsigned long)(1e9 / freq);

    if (pwm->enable) {
        if (DEBUG)
            printf(" ** IN pwm_set_frequency: pwm_initialized = %d\n", pwm_initialized);
        if (period_ns != pwm->period_ns) {
            pwm->period_ns = period_ns;

            len = snprintf(buffer, sizeof(buffer), "%lu", period_ns); BUF2SMALL(buffer);
            if (DEBUG)
                printf(" ** pwm_set_frequency: buffer: %s\n", buffer);
            ssize_t s = write(pwm->period_fd, buffer, len);  //ASSRT(s == len);
            if (DEBUG)
                printf(" ** IN pwm_set_frequency: s = %d, len = %d\n", s, len);
            if (s != len) {
                rtnval = -1;
            } else {
                rtnval = 1;
            }
        } else {
            rtnval = 0;
        }
    } else {
        rtnval = 0;
    }

    return rtnval;
}

int pwm_set_polarity(const char *key, int polarity) {
    int len;
    int rtnval = -1;
    char buffer[80];
    struct pwm_exp *pwm;

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        return rtnval;
    }

    if (polarity != 0 && polarity != 1) {
        return rtnval;
    }

    if (pwm->enable) {
        if (DEBUG)
            printf(" ** IN pwm_set_polarity: pwm_initialized = %d\n", pwm_initialized);
        if (polarity == 0) {
            len = snprintf(buffer, sizeof(buffer), "%s", "normal"); BUF2SMALL(buffer);
        }
        else
        {
            len = snprintf(buffer, sizeof(buffer), "%s", "inverted"); BUF2SMALL(buffer);
        }
        if (DEBUG)
            printf(" ** pwm_set_poliarity: buffer: %s\n", buffer);
        ssize_t s = write(pwm->polarity_fd, buffer, len);  //ASSRT(s == len);
        if (DEBUG)
            printf(" ** IN pwm_set_polarity: s = %d, len = %d\n", s, len);
        if (s != len) {
            rtnval = -1;
        } else {
            rtnval = 1;
        }
    } else {
        rtnval = 0;
    }
    return rtnval;
}

int pwm_set_duty_cycle(const char *key, float duty) {
    int len;
    int rtnval = -1;
    char buffer[80];
    struct pwm_exp *pwm;

    if (duty < 0.0 || duty > 100.0) {
        return rtnval;
    }

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        return rtnval;
    }

    pwm->duty = (unsigned long)(pwm->period_ns * (duty / 100.0));

    if (pwm->enable) {
        if (DEBUG)
            printf(" ** IN pwm_set_duty_cycle: pwm_initialized = %d\n", pwm_initialized);
        len = snprintf(buffer, sizeof(buffer), "%lu", pwm->duty); BUF2SMALL(buffer);
        if (DEBUG)
            printf(" ** pwm_set_duty_cycle: buffer: %s\n", buffer);
        ssize_t s = write(pwm->duty_fd, buffer, len);  //ASSRT(s == len);
        if (DEBUG)
            printf(" ** IN pwm_set_duty_cycle: s = %d, len = %d\n", s, len);
        if (s != len) {
            rtnval = -1;
        } else {
            rtnval = 1;
        }
    } else {
        rtnval = 0;
    }

    return rtnval;
}

int pwm_set_enable(const char *key, int enable)
{
    int len;
    int rtnval = -1;
    char buffer[80];
    struct pwm_exp *pwm;

    if (enable != 0 && enable != 1) {
        if (DEBUG)
            printf(" ** INVALID ENABLE OPTION, NEEDS TO BE EITHER 0 OR 1! **\n");
        return rtnval;
    }

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        if (DEBUG)
            printf(" ** SOMETHING BAD HAPPEND IN pwm_set_enable, WE'RE EXITING WITH -1 NOW! **\n");
        return rtnval;
    }


    len = snprintf(buffer, sizeof(buffer), "%d", enable); BUF2SMALL(buffer);
    ssize_t s = write(pwm->enable_fd, buffer, len);  //ASSRT(s == len);
    if (DEBUG) {
        printf(" ** IN pwm_set_enable: pwm_initialized = %d\n", pwm_initialized);
        printf(" ** pwm_set_enable: buffer: %s\n", buffer);
        printf(" ** IN pwm_set_enable: s = %d, len = %d\n", s, len);
    }

    if (s == len)
    {
        if (DEBUG)
            printf(" ** SETTING pwm->enable to %d\n", enable);
        pwm->enable = enable;
        rtnval = 0;
    } else {
        rtnval = -1;
    }

    return rtnval;
}

int pwm_start(const char *key, float duty, float freq, int polarity)
{
    char pwm_base_path[80];
    char period_path[80];
    char duty_path[80];
    char enable_path[80];
    char polarity_path[80];
    int period_fd, duty_fd, polarity_fd, enable_fd;
    struct pwm_exp *new_pwm, *pwm;

    if (DEBUG)
        printf(" ** IN pwm_start: pwm_initialized = %d\n", pwm_initialized);
    if(!pwm_initialized) {
        initialize_pwm();
    } else {
        if (DEBUG)
            printf(" ** ALREADY INITIALIZED, CALLING CLEANUP TO START FRESH **");
        pwm_cleanup();   
    }
    if (DEBUG)
        printf(" ** IN pwm_start: pwm_initialized = %d\n", pwm_initialized);

    //setup the pwm base path, the chip only has one pwm
    snprintf(pwm_base_path, sizeof(pwm_base_path), "/sys/class/pwm/pwmchip0/pwm%d", 0); BUF2SMALL(pwm_base_path);

    //create the path for the period and duty
    snprintf(enable_path, sizeof(enable_path), "%s/enable", pwm_base_path); BUF2SMALL(enable_path);
    snprintf(period_path, sizeof(period_path), "%s/period", pwm_base_path); BUF2SMALL(period_path);
    snprintf(duty_path, sizeof(duty_path), "%s/duty_cycle", pwm_base_path); BUF2SMALL(duty_path);
    snprintf(polarity_path, sizeof(polarity_path), "%s/polarity", pwm_base_path); BUF2SMALL(polarity_path);

    if (DEBUG) {
        printf(" ** IN pwm_start: pwm_base_path: %s\n", pwm_base_path);
        printf(" ** IN pwm_start: enable_path:   %s\n", enable_path);
        printf(" ** IN pwm_start: period_path:   %s\n", period_path);
        printf(" ** IN pwm_start: duty_path:     %s\n", duty_path);
        printf(" **IN pwm_start: polarity_path: %s\n", polarity_path);
    }
    
    //add period and duty fd to pwm list
    if ((enable_fd = open(enable_path, O_WRONLY)) < 0)
        return -1;

    if ((period_fd = open(period_path, O_WRONLY)) < 0) {
        close(enable_fd);
        return -1;
    }

    if ((duty_fd = open(duty_path, O_WRONLY)) < 0) {
        //error, close already opened period_fd.
        close(enable_fd);
        close(period_fd);
        return -1;
    }

    if ((polarity_fd = open(polarity_path, O_WRONLY)) < 0) {
        //error, close already opened period_fd and duty_fd.
        close(enable_fd);
        close(period_fd);
        close(duty_fd);
        return -1;
    }

    // add to list
    new_pwm = malloc(sizeof(struct pwm_exp));
    if (new_pwm == 0) {
        return -1; // out of memory
    }

    if (DEBUG)
        printf(" ** IN pwm_start: IF WE MAKE IT HERE, THE FILES WERE SUCCESSFULLY OPEN **\n");
    strncpy(new_pwm->key, key, KEYLEN);  /* can leave string unterminated */
    new_pwm->key[KEYLEN] = '\0'; /* terminate string */
    new_pwm->period_fd = period_fd;
    new_pwm->duty_fd = duty_fd;
    new_pwm->polarity_fd = polarity_fd;
    new_pwm->enable_fd = enable_fd;
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

    int rtnval = 0;
    rtnval = pwm_set_enable(key, ENABLE);
    rtnval = 0;
    rtnval = pwm_set_frequency(key, freq);
    rtnval = 0;
    //rtnval = pwm_set_polarity(key, polarity);
    //rtnval = 0;
    rtnval = pwm_set_duty_cycle(key, duty);

    return rtnval;
}

int pwm_disable(const char *key)
{
    struct pwm_exp *pwm, *temp, *prev_pwm = NULL;

    int fd, len;
    char str_gpio[80];
    // Per https://github.com/NextThingCo/CHIP-linux/pull/4
    // we need to export 0 here to enable pwm0
    int gpio = 0;

    // Disable the PWM
    pwm_set_frequency(key, 0);
    //pwm_set_polarity(key, 0);
    pwm_set_duty_cycle(key, 0);
    pwm_set_enable(key, DISABLE);

    if ((fd = open("/sys/class/pwm/pwmchip0/unexport", O_WRONLY)) < 0)
    {
        return -1;
    }
    len = snprintf(str_gpio, sizeof(str_gpio), "%d", gpio); BUF2SMALL(str_gpio);
    ssize_t s = write(fd, str_gpio, len);  ASSRT(s == len);
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

void pwm_cleanup(void)
{
    while (exported_pwms != NULL) {
        pwm_disable(exported_pwms->key);
    }
}

void pwm_toggle_debug(void)
{
    if (DEBUG) {
        DEBUG = 0;
    } else {
        DEBUG = 1;
    }
}
