#!/bin/sh

for F in /sys/class/gpio/gpio[0-9]*; do :
  GPIO=`echo $F | sed 's/^[^0-9]*//'`
  echo $F $GPIO
  echo $GPIO >/sys/class/gpio/unexport
done
