from __future__ import print_function
import time

from led_pwm_tlc5947 import PwmController

port = '/dev/ttyACM0'
num_led = 18
dt = 0.05

dev = PwmController(port)

# Try the get_number_of_devices command
# -----------------------------------------------------------------------------
number_of_devices  = dev.get_number_of_devices()
print('number_of_devices = {}'.format(number_of_devices))


# Try the set_one command
# -----------------------------------------------------------------------------
for i in range(num_led):
    dev.set_one(0,i,2000)
    time.sleep(dt)

for i in range(num_led):
    dev.set_one(0,i,0)
    time.sleep(dt)

dev.set_all(None,0)


# Try the set_some command
# -----------------------------------------------------------------------------
val_dict = {0:{}}
val_list = [(x*1000)/num_led for x in range(0,num_led)]
for i in range(num_led):
    val_dict[0][i] = val_list[i]
    dev.set_some(val_dict)
    time.sleep(dt)

dev.set_all(None,0)

val_dict = {0:{}}
val_list.reverse()
for i in range(num_led):
    val_dict[0][i] = val_list[i]
    dev.set_some(val_dict)
    time.sleep(dt)

dev.set_all(None,0)


# Try the  set_all command
# -----------------------------------------------------------------------------
val_list = range(0,3000,100)
for i in val_list:
    dev.set_all(0,i)
    time.sleep(dt)

val_list.reverse()
for i in val_list:
    dev.set_all(0,i)
    time.sleep(dt)

dev.set_all(None,0)

