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
#include <errno.h>
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

// pwm exports
struct pwm_exp
{
    char key[KEYLEN+1]; /* leave room for terminating NUL byte */
    int iscpro;
    int gpio;
    int initialized;
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

int initialize_pwm(int *initialized, int gpio)
{
    int e_no;
    if (!*initialized) {
        int fd, len;
        char str_gpio[80];

        if (DEBUG)
            printf(" ** initialize_pwm **\n");
        if ((fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY)) < 0)
        {
            char err[256];
            snprintf(err, sizeof(err), "initialize_pwm: could not open export file");
            add_error_msg(err);
            return -1;
        }
        len = snprintf(str_gpio, sizeof(str_gpio), "%d", gpio); BUF2SMALL(str_gpio);
        ssize_t s = write(fd, str_gpio, len); e_no = errno;
        close(fd);
        
        if (s != len) {
            char err[256];
            snprintf(err, sizeof(err), "initialize_pwm: could not export pwm (%s)", strerror(e_no));
            add_error_msg(err);
            return -1;
        }
        
        if (DEBUG)
            printf(" ** initialize_pwm: export pin: s = %d, len = %d\n", s, len);
        
        *initialized = 1;
        return 1;
    } else {
        if (DEBUG)
            printf(" ** initialize_pwm: pwm is already initialized\n");
    }

    return 0;
}

int pwm_set_frequency(const char *key, float freq) {
    int len, e_no;
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
        if (period_ns != pwm->period_ns) {
            pwm->period_ns = period_ns;

            len = snprintf(buffer, sizeof(buffer), "%lu", period_ns); BUF2SMALL(buffer);
            ssize_t s = write(pwm->period_fd, buffer, len);  e_no = errno;
            if (DEBUG) {
                printf(" ** pwm_set_frequency: pwm_initialized = %d\n", pwm->initialized);
                printf(" ** pwm_set_frequency: buffer: %s\n", buffer);
                printf(" ** pwm_set_frequency: s = %d, len = %d\n", s, len);
            }
            if (s != len) {
                char err[256];
                snprintf(err, sizeof(err), "pwm_set_frequency: could not change frequency of pwm (%s)", strerror(e_no));
                add_error_msg(err);
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

int pwm_set_period_ns(const char *key, unsigned long period_ns) {
    int len, e_no;
    int rtnval = -1;
    char buffer[80];
    struct pwm_exp *pwm;

    //TODO: ADD CHECK FOR period_ns

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        return rtnval;
    }
    
    if (pwm->enable) {
        if (period_ns != pwm->period_ns) {
            pwm->period_ns = period_ns;

            len = snprintf(buffer, sizeof(buffer), "%lu", period_ns); BUF2SMALL(buffer);
            ssize_t s = write(pwm->period_fd, buffer, len);  e_no = errno;
            if (DEBUG) {
                printf(" ** pwm_set_period_ns: pwm_initialized = %d\n", pwm->initialized);
                printf(" ** pwm_set_period_ns: buffer: %s\n", buffer);
                printf(" ** pwm_set_period_ns: s = %d, len = %d\n", s, len);
            }
            if (s != len) {
                char err[256];
                snprintf(err, sizeof(err), "pwm_set_period_ns: could not change period of pwm (%s)", strerror(e_no));
                add_error_msg(err);
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

int pwm_get_period_ns(const char *key, unsigned long *period_ns) {
    int rtnval = -1;
    struct pwm_exp *pwm;
    
    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        return rtnval;
    }
    
    if (DEBUG)
        printf(" ** pwm_get_period_ns: %lu **\n",pwm->period_ns);
    
    // Set period_ns to what we have in the struct
    *period_ns = pwm->period_ns;
    
    rtnval = 0;
    return rtnval;
}

int pwm_set_polarity(const char *key, int polarity) {
    int len, e_no;
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

    // THIS ONLY WORKS WHEN PWM IS NOT ENABLED
    if (pwm->enable == 0) {
        if (polarity == 0) {
            len = snprintf(buffer, sizeof(buffer), "%s", "normal"); BUF2SMALL(buffer);
        }
        else
        {
            len = snprintf(buffer, sizeof(buffer), "%s", "inversed"); BUF2SMALL(buffer);
        }
        ssize_t s = write(pwm->polarity_fd, buffer, len);  e_no = errno;
        if (DEBUG) {
            printf(" ** pwm_set_polarity: pwm_initialized = %d\n", pwm->initialized);
            printf(" ** pwm_set_polarity: buffer: %s\n", buffer);
            printf(" ** pwm_set_polarity: s = %d, len = %d\n", s, len);
        }
        if (s != len) {
            char err[256];
            snprintf(err, sizeof(err), "pwm_set_polarity: could not change polarity of pwm (%s)", strerror(e_no));
            add_error_msg(err);
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
    int len, e_no;
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
        len = snprintf(buffer, sizeof(buffer), "%lu", pwm->duty); BUF2SMALL(buffer);
        ssize_t s = write(pwm->duty_fd, buffer, len);  e_no = errno;
        if (DEBUG) {
            printf(" ** pwm_set_duty_cycle: pwm_initialized = %d\n", pwm->initialized);
            printf(" ** pwm_set_duty_cycle: buffer: %s\n", buffer);
            printf(" ** pwm_set_duty_cycle: s = %d, len = %d\n", s, len);
        }
        if (s != len) {
            char err[256];
            snprintf(err, sizeof(err), "pwm_set_duty_cycle: could not change duty cycle of pwm (%s)", strerror(e_no));
            add_error_msg(err);
            rtnval = -1;
        } else {
            rtnval = 1;
        }
    } else {
        rtnval = 0;
    }

    return rtnval;
}

int pwm_set_pulse_width_ns(const char *key, unsigned long pulse_width_ns) {
    int len, e_no;
    int rtnval = -1;
    char buffer[80];
    struct pwm_exp *pwm;

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        return rtnval;
    }

    if (pulse_width_ns < 0 || pulse_width_ns > pwm->period_ns)
        return rtnval;

    pwm->duty = pulse_width_ns / pwm->period_ns;
    
    if (pwm->enable) {
        len = snprintf(buffer, sizeof(buffer), "%lu", pwm->duty); BUF2SMALL(buffer);
        ssize_t s = write(pwm->duty_fd, buffer, len);  e_no = errno;
        if (DEBUG) {
            printf(" ** pwm_set_pulse_width_ns: pwm_initialized = %d\n", pwm->initialized);
            printf(" ** pwm_set_pulse_width_ns: buffer: %s\n", buffer);
            printf(" ** pwm_set_pulse_width_ns: s = %d, len = %d\n", s, len);
        }
        if (s != len) {
            char err[256];
            snprintf(err, sizeof(err), "pwm_set_pulse_width_ns: could not change pulse width of pwm (%s)", strerror(e_no));
            add_error_msg(err);
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
    int len, e_no;
    int rtnval = -1;
    char buffer[80];
    struct pwm_exp *pwm;

    if (enable != 0 && enable != 1) {
        if (DEBUG)
            printf(" ** pwm_set_enable: enable needs to be 0 or 1! **\n");
        return rtnval;
    }

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        if (DEBUG)
            printf(" ** pwm_set_enable: pwm struct is null **\n");
        return rtnval;
    }


    len = snprintf(buffer, sizeof(buffer), "%d", enable); BUF2SMALL(buffer);
    ssize_t s = write(pwm->enable_fd, buffer, len);  e_no = errno;
    if (DEBUG) {
        printf(" ** pwm_set_enable: pwm_initialized = %d\n", pwm->initialized);
        printf(" ** pwm_set_enable: buffer: %s\n", buffer);
        printf(" ** pwm_set_enable: s = %d, len = %d\n", s, len);
    }

    if (s == len)
    {
        if (DEBUG)
            printf(" ** pwm_set_enable: pwm->enable to %d\n", enable);
        pwm->enable = enable;
        rtnval = 0;
    } else {
        char err[256];
        snprintf(err, sizeof(err), "pwm_set_enable: could not enable/disable pwm (%s)", strerror(e_no));
        add_error_msg(err);
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
    int gpio = 0;
    int initialized = 0;
    int iscpro = 0;

    // Figure out if we are a CPro
    iscpro = is_this_chippro();

    // Figure out which pin we are
    if (strcmp(key,"U13_18") == 0) {
        gpio = 0;
    } else if (strcmp(key,"EINT13") == 0) {
        gpio = 1;
    }

    // Try to get the pwm
    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        initialize_pwm(&initialized, gpio);
        if (DEBUG)
            printf(" ** pwm_start: pwm(%d) initialized = %d\n", gpio, initialized);
    } else {
        if (DEBUG)
            printf(" ** pwm_start: pwm(%d) already initialized, cleaning up **", gpio);
        pwm_cleanup();   
    }

    //setup the pwm base path, the chip only has one pwm
    snprintf(pwm_base_path, sizeof(pwm_base_path), "/sys/class/pwm/pwmchip0/pwm%d", gpio); BUF2SMALL(pwm_base_path);

    //create the path for the period and duty
    snprintf(enable_path, sizeof(enable_path), "%s/enable", pwm_base_path); BUF2SMALL(enable_path);
    snprintf(period_path, sizeof(period_path), "%s/period", pwm_base_path); BUF2SMALL(period_path);
    snprintf(duty_path, sizeof(duty_path), "%s/duty_cycle", pwm_base_path); BUF2SMALL(duty_path);
    snprintf(polarity_path, sizeof(polarity_path), "%s/polarity", pwm_base_path); BUF2SMALL(polarity_path);

    if (DEBUG) {
        printf(" ** pwm_start: pwm_base_path: %s\n", pwm_base_path);
        printf(" ** pwm_start: enable_path:   %s\n", enable_path);
        printf(" ** pwm_start: period_path:   %s\n", period_path);
        printf(" ** pwm_start: duty_path:     %s\n", duty_path);
        printf(" ** pwm_start: polarity_path: %s\n", polarity_path);
    }
    
    //add period and duty fd to pwm list
    if ((enable_fd = open(enable_path, O_WRONLY)) < 0) {
        char err[256];
        snprintf(err, sizeof(err), "pwm_start: could not open enable file");
        add_error_msg(err);
        return -1;
    }

    if ((period_fd = open(period_path, O_WRONLY)) < 0) {
        close(enable_fd);
        char err[256];
        snprintf(err, sizeof(err), "pwm_start: could not open period file");
        add_error_msg(err);
        return -1;
    }

    if ((duty_fd = open(duty_path, O_WRONLY)) < 0) {
        //error, close already opened period_fd.
        close(enable_fd);
        close(period_fd);
        char err[256];
        snprintf(err, sizeof(err), "pwm_start: could not open duty cycle file");
        add_error_msg(err);
        return -1;
    }

    if ((polarity_fd = open(polarity_path, O_WRONLY)) < 0) {
        //error, close already opened period_fd and duty_fd.
        close(enable_fd);
        close(period_fd);
        close(duty_fd);
        char err[256];
        snprintf(err, sizeof(err), "pwm_start: could not open polarity file");
        add_error_msg(err);
        return -1;
    }

    // add to list
    new_pwm = malloc(sizeof(struct pwm_exp));
    if (new_pwm == 0) {
        char err[256];
        snprintf(err, sizeof(err), "pwm_start: unable to allocate memory");
        add_error_msg(err);
        return -1;
    }

    if (DEBUG)
        printf(" ** pwm_start: sysfs files opened successfully **\n");
    strncpy(new_pwm->key, key, KEYLEN);  /* can leave string unterminated */
    new_pwm->key[KEYLEN] = '\0'; /* terminate string */
    new_pwm->gpio = gpio;
    new_pwm->iscpro = iscpro;
    new_pwm->initialized = initialized;
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
    // Fix for issue #53
    // Always set polarity first
    rtnval = pwm_set_polarity(key, polarity);
    if (rtnval != -1) {
        rtnval = 0;
        rtnval = pwm_set_enable(key, ENABLE);
        if (rtnval != -1) {
            rtnval = 0;
            rtnval = pwm_set_frequency(key, freq);
            if (rtnval != -1) {
                rtnval = 0;
                rtnval = pwm_set_duty_cycle(key, duty);
            }
        }
    }
    return rtnval;
}

int pwm_disable(const char *key)
{
    struct pwm_exp *pwm, *temp, *prev_pwm = NULL;

    int fd, len, e_no;
    char str_gpio[80];

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        if (DEBUG)
            printf(" ** pwm_disable: pwm struct is null **\n");
        return -1;
    }

    // Disable the PWM
    pwm_set_frequency(key, 0);
    pwm_set_duty_cycle(key, 0);
    pwm_set_enable(key, DISABLE);
    pwm_set_polarity(key, 0);

    if ((fd = open("/sys/class/pwm/pwmchip0/unexport", O_WRONLY)) < 0)
    {
        char err[256];
        snprintf(err, sizeof(err), "pwm_disable: could not open unexport file");
        add_error_msg(err);
        return -1;
    }
    len = snprintf(str_gpio, sizeof(str_gpio), "%d", pwm->gpio); BUF2SMALL(str_gpio);
    ssize_t s = write(fd, str_gpio, len); e_no = errno;
    close(fd);
    if (s != len) {
        char err[256];
        snprintf(err, sizeof(err), "pwm_disable: could not unexport pwm (%s)", strerror(e_no));
        add_error_msg(err);
        return -1;
    }
    
    // remove from list
    pwm = exported_pwms;
    while (pwm != NULL)
    {
        if (strcmp(pwm->key, key) == 0)
        {
            if (DEBUG) {
                printf(" ** pwm_disable: freeing memory %s\n", key);
            }
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

