/*
Copyright (c) 2017 Robert Wolterman

Using CHIP_IO servo code from Brady Hurlburt as a basis for the servo code

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
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "c_softservo.h"
#include "common.h"
#include "event_gpio.h"

#define KEYLEN 7

#define PERIOD 0
#define DUTY 1

#define MSTONS 1000000
#define BLOCKNS 20 * MSTONS
#define FUDGEFACTOR 450
#define MSTOMICROS 1000
#define MICROSTONS 1000
#define MINMICROS 1000
#define NEUTRAL 1500
#define MAXMICROS 2000

int servo_initialized = 0;

struct servo_params
{
    float range;
    float min_angle;
    float max_angle;
    float current_angle;
    bool enabled;
    bool stop_flag;
};

struct servo
{
    char key[KEYLEN+1]; /* leave room for terminating NUL byte */
    int gpio;
    struct servo_params params;
    pthread_mutex_t* params_lock;
    pthread_t thread;
    struct servo *next;
};
struct servo *exported_servos = NULL;

struct servo *lookup_exported_servo(const char *key)
{
    struct servo *srv = exported_servos;

    while (srv != NULL)
    {
        if (strcmp(srv->key, key) == 0) {
            return srv;
        }
        srv = srv->next;
    }

    return NULL; /* standard for pointers */
}

void *servo_thread_toggle(void *arg)
{
    struct servo *srv = (struct servo *)arg;
    int gpio = srv->gpio;
    struct timespec tim_on;
    struct timespec tim_off;
    float on_time_microsec = 0;
    unsigned int on_ns;
    unsigned int off_ns;

    /* Used to determine if something has
     * has changed
    */
    float angle_local = 0;
    float range_local = 0;
    bool stop_flag_local = false;
    bool enabled_local = false;
    bool recalculate_timing = false;

    while (!stop_flag_local) {
        /* Take a snapshot of the parameter block */
        pthread_mutex_lock(srv->params_lock);
        if ((angle_local != srv->params.current_angle) || (range_local != srv->params.range)) {
            recalculate_timing = true;
        }
        angle_local = srv->params.current_angle;
        range_local = srv->params.range;
        enabled_local = srv->params.enabled;
        stop_flag_local = srv->params.stop_flag;
        pthread_mutex_unlock(srv->params_lock);

        /* If freq or duty has been changed, update the
         * sleep times
        */
        if (recalculate_timing) {
            if (DEBUG)
                printf(" ** servo updating timing: new angle: (%.2f)\n",angle_local);
            on_time_microsec = (((MAXMICROS - MINMICROS) / range_local) * angle_local) + NEUTRAL;
            on_ns = (unsigned long)(on_time_microsec * MICROSTONS - FUDGEFACTOR);
            off_ns = BLOCKNS - on_ns;
            tim_on.tv_sec = 0;
            tim_on.tv_nsec = on_ns;
            tim_off.tv_sec = 0;
            tim_off.tv_nsec = off_ns;
            recalculate_timing = false;
        }

        if (enabled_local)
        {
            /* Set gpio */
            gpio_set_value(gpio, HIGH);

            nanosleep(&tim_on, NULL);

            /* Unset gpio */
            gpio_set_value(gpio, LOW);

            nanosleep(&tim_off, NULL);
            //printf("AFTER SECOND NANOSLEEP\n");
        }  /* if enabled_local */
    }  /* while !stop_flag_local */

    gpio_set_value(gpio, LOW);

    /* This servo has been disabled */
    pthread_exit(NULL);
}

int servo_start(const char *key, float angle, float range)
{
    struct servo *new_srv, *srv;
    pthread_t new_thread;
    pthread_mutex_t *new_params_lock;
    int gpio;
    int ret;

    if (get_gpio_number(key, &gpio) < 0) {
        if (DEBUG)
            printf(" ** servo_start: invalid gpio specified **\n");
        return -1;
    }
    
    if (gpio_export(gpio) < 0) {
       char err[2000];
       snprintf(err, sizeof(err), "Error setting up servo on pin %d, maybe already exported? (%s)", gpio, get_error_msg());
       add_error_msg(err);
       return -1;
    }
    
    if (DEBUG)
        printf(" ** servo_start: %d exported **\n", gpio);
    
    if (gpio_set_direction(gpio, OUTPUT) < 0) {
        if (DEBUG)
            printf(" ** servo_start: gpio_set_direction failed **\n");
        return -1;
    }

    printf("c_softservo.c: servo_start(%d,%.2f,%.2f)\n",gpio,angle,range);
    
    // add to list
    new_srv = malloc(sizeof(struct servo));  ASSRT(new_srv != NULL);
    new_params_lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if (new_srv == 0) {
        return -1; // out of memory
    }
    pthread_mutex_init(new_params_lock, NULL);
    pthread_mutex_lock(new_params_lock);

    strncpy(new_srv->key, key, KEYLEN);  /* can leave string unterminated */
    new_srv->key[KEYLEN] = '\0'; /* terminate string */
    new_srv->gpio = gpio;
    new_srv->params.enabled = true;
    new_srv->params.stop_flag = false;
    new_srv->params_lock = new_params_lock;
    new_srv->next = NULL;

    if (exported_servos == NULL)
    {
        // create new list
        exported_servos = new_srv;
    } else {
        // add to end of existing list
        srv = exported_servos;
        while (srv->next != NULL)
            srv = srv->next;
        srv->next = new_srv;
    }
    pthread_mutex_unlock(new_params_lock);

    if (DEBUG)
        printf(" ** servo_enable: setting servo parameters **\n");
    ASSRT(servo_set_range(new_srv->key, range) == 0);
    ASSRT(servo_set_angle(new_srv->key, angle) == 0);

    pthread_mutex_lock(new_params_lock);
    // create thread for srv
    if (DEBUG)
        printf(" ** servo_enable: creating thread **\n");
    ret = pthread_create(&new_thread, NULL, servo_thread_toggle, (void *)new_srv);
    ASSRT(ret == 0);

    new_srv->thread = new_thread;

    pthread_mutex_unlock(new_params_lock);

    return 1;
}

int servo_disable(const char *key)
{
    struct servo *srv, *temp, *prev_srv = NULL;

    if (DEBUG)
        printf(" ** in servo_disable **\n");
    // remove from list
    srv = exported_servos;
    while (srv != NULL)
    {
        if (strcmp(srv->key, key) == 0)
        {
            if (DEBUG)
                printf(" ** servo_disable: found pin **\n");
            pthread_mutex_lock(srv->params_lock);
            srv->params.stop_flag = true;
            pthread_mutex_unlock(srv->params_lock);
            pthread_join(srv->thread, NULL);  /* wait for thread to exit */

            if (DEBUG)
                printf(" ** servo_disable: unexporting %d **\n", srv->gpio);
            gpio_unexport(srv->gpio);

            if (prev_srv == NULL)
            {
                exported_servos = srv->next;
                prev_srv = srv;
            } else {
                prev_srv->next = srv->next;
            }

            temp = srv;
            srv = srv->next;
            free(temp->params_lock);
            free(temp);
        } else {
            prev_srv = srv;
            srv = srv->next;
        }
    }
    return 0;
}

int servo_set_range(const char *key, float range)
{
    struct servo *srv;
    float min_angle, max_angle;

    srv = lookup_exported_servo(key);

    if (srv == NULL) {
        return -1;
    }

    // Compute the min and max angle
    max_angle = range / 2.0;
    min_angle = -max_angle;

    if (DEBUG)
        printf(" ** servo_set_range(%d,%.2f = %.2f,%.2f)\n",srv->gpio,range,min_angle,max_angle);
    pthread_mutex_lock(srv->params_lock);
    srv->params.range = range;
    srv->params.min_angle = min_angle;
    srv->params.max_angle = max_angle;
    pthread_mutex_unlock(srv->params_lock);

    return 0;
}

int servo_set_angle(const char *key, float angle)
{
    struct servo *srv;

    srv = lookup_exported_servo(key);

    if (srv == NULL) {
        return -1;
    }

    // Make sure we're between the range of allowable angles
    if (angle < srv->params.min_angle || angle > srv->params.max_angle) {
       char err[2000];
       snprintf(err, sizeof(err), "Angle specified (%.2f) for pin %d, is outside allowable range (%.2f,%.2f)", angle, srv->gpio, srv->params.min_angle,srv->params.max_angle);
       add_error_msg(err);
       return -1;
    }
    
    if (DEBUG)
        printf(" ** servo_set_angle(%d,%.2f)\n",srv->gpio,angle);
    pthread_mutex_lock(srv->params_lock);
    srv->params.current_angle = angle;
    pthread_mutex_unlock(srv->params_lock);

    return 0;
}

void servo_cleanup(void)
{
    while (exported_servos != NULL) {
        servo_disable(exported_servos->key);
    }
}



