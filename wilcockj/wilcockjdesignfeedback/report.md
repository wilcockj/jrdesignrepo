---
title: Desgin Feedback Session Submission 
author:
	- James Wilcock
date: \today{}
geometry: margin=1in
---

# Materials Presented

ESP32 data sheet
https://cdn-learn.adafruit.com/downloads/pdf/adafruit-huzzah32-esp32-feather.pdf

IS31FL3218 LED Driver datasheet
https://www.mouser.com/datasheet/2/198/IS31FL3218_DS-1949531.pdf

# Questions asked

I am concerned with if the speed of changing color when controlling 175 leds will be fast enough, we are using i2c from the esp to the IS31FL3218 led driver to change color, which should be able to handle 400kHz.

I am concerned with how much current the power supply with need,we are using 175 rgb leds.

Will the led leads be able to hold the weight of 6 layers above them aka holding 150 leds on the bottom?

# Feedback recieved 

Was advised that creating a skeleton for the led cube could be a good way to ensure that I will have
enough strength for the leds to hold themselves. 

# Feedback given

To Hang Preston in the Temperature Measuring group
Was asking about the simplest way to audibly alert the user 
