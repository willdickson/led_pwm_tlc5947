#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include <Arduino.h>
#include "constants.h"
#include "message_receiver.h"
#include "ArduinoJson.h"
#include "Adafruit_TLC5947.h"

class SystemState
{
    public:

        SystemState();
        void initialize();
        void process_messages();
        void update_on_serial_event();

    private:

        MessageReceiver message_receiver_;
        Adafruit_TLC5947 tlc5947_ = Adafruit_TLC5947(Number_Of_Device, Clock_Pin, Data_Pin, Latch_Pin);

        void handle_json_message(JsonObject &json_msg, JsonObject &json_rsp);
        void handle_set_cmd(JsonObject &json_msg, JsonObject &json_rsp);
        void handle_set_all_cmd(JsonObject &json_msg, JsonObject &json_rsp);
        void handle_get_num_device_cmd(JsonObject &json_msg, JsonObject &json_rsp);

        bool set(uint16_t dev_num, uint16_t pwm_num, uint16_t value, bool write=true);
        bool set_all(uint16_t dev_num, uint16_t value);
        bool set_all(uint16_t value);
};
#endif
