"""
================
led_pwm_tlc5947
================

    This module implements the serial interface to the led_pwm_tlc5947 firmware which provides and
    interface to the 24-channel 16-bit TLC5947 LED PWM controller.

    :copyright: (c) 2019  Will Dickson 
    :license: MIT, see LICENSE.txt for more details.
"""
from __future__ import print_function
import serial
import time
import json
import atexit


class PwmControllerException(Exception):
    pass


class PwmController(serial.Serial):

    """Provides a high level interface  performing serial communications with the tlc5947 led driver. 

    :param str port: serial port associated with the Rodeostat, e.g. /dev/ttyACM0, COM1, etc.

    :Keyword Arguments:


    """
    
    ResetSleepDt = 0.0
    Baudrate = 115200

    def __init__(self, port, timeout=10.0, debug=False):
        """Constructor

        """
        params = {'baudrate': self.Baudrate, 'timeout': timeout}
        super(PwmController,self).__init__(port,**params)
        time.sleep(self.ResetSleepDt)
        while self.inWaiting() > 0:
            val = self.read()


    def get_number_of_devices(self):
        """Returns a string representing the hardware variant.
        
        """
        cmd_dict = {"command": "get_num_dev"}
        rsp_dict = self.send_cmd(cmd_dict)
        return rsp_dict["num_dev"]

    def set_one(self, dev_num, pwm_num, value):
        val_dict = {dev_num:{pwm_num:value}}
        cmd_dict = {"command": "set", "value":val_dict}
        self.send_cmd(cmd_dict)


    def set_some(self, val_dict):
        """Sets a subset of the led pwm values. Argument should be a dict where the keys are
        the devices id #s and the values are dictionaries mapping pwm numbers to pwm values e.g.

        val_dict = {
            dev_num0: {
                pwm_num0: pwm_val0, 
                pwm_num1: pwm_val1,
                ...
            }, 
            dev_num1:{
                pwm_num0: pwm_val0,
                pwm_num1: pwm_val1,
                ...
            }, 
            ...
        }

        """

        cmd_dict = {"command":"set","value":val_dict}
        self.send_cmd(cmd_dict)


    def set_all(self, dev_num, value):
        """Sets all led pwms for the specified device (dev_num) to the given value (value).  If 
        dev_num is None then the lwd pwm value will be set for all devices. 
        
        """
        if dev_num is None:
            cmd_dict = {"command": "set_all", "value": value}
        else:
            cmd_dict = {"command": "set_all", "value": {dev_num: value}}
        self.send_cmd(cmd_dict)


    def send_cmd(self,cmd_dict):
        """Sends a command to the device.  Low-level method - command is specified 
        using command dictionary.

        """
        cmd_json = json.dumps(cmd_dict) + '\n'
        self.write(cmd_json.encode())
        rsp_json = self.readline()
        rsp_json = rsp_json.strip()
        rsp_dict = json.loads(rsp_json.decode())
        self.check_cmd_msg(cmd_dict,rsp_dict)
        return rsp_dict


    def check_cmd_msg(self,cmd_dict,rsp_dict):
        self.check_for_success(rsp_dict)


    def check_for_success(self,rsp_dict):
        success = rsp_dict["success"]
        if not success: 
            raise PwmControllerException(rsp_dict['message'])








