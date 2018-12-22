#include "system_state.h"
#include "Streaming.h"

SystemState::SystemState() { }


void SystemState::initialize()
{
    Serial.begin(Baudrate);
    tlc5947_.begin();
    pinMode(Enable_Pin,OUTPUT);
    digitalWrite(Enable_Pin,LOW);
    set_all(0);
}


void SystemState::process_messages()
{
    if (message_receiver_.available())
    {
        String message = message_receiver_.next();

        StaticJsonBuffer<Json_Message_Buffer_Size> json_msg_buffer;
        StaticJsonBuffer<Json_Message_Buffer_Size> json_rsp_buffer;

        JsonObject &json_msg = json_msg_buffer.parse(message);
        JsonObject &json_rsp = json_rsp_buffer.createObject();

        Serial << "json_msg = ";
        json_msg.printTo(Serial);
        Serial << endl;

        if (json_msg.success())
        {
            handle_json_message(json_msg, json_rsp);
        }
        else
        {
            json_rsp["success"] = false;
            json_rsp["message"] = "parse error";
        }

        Serial << "json_rsp = ";
        json_rsp.printTo(Serial);
        Serial << endl;
    }
}


void SystemState::update_on_serial_event()
{
    message_receiver_.read_data();
}


void SystemState::handle_json_message(JsonObject &json_msg, JsonObject &json_rsp)
{
    String command = json_msg["command"] | "missing";

    if (command.equals("missing"))
    {
        json_rsp["success"] = false;
        json_rsp["message"] = "command key missing";
    } 
    else if (command.equals("set"))
    {
        handle_set_cmd(json_msg,json_rsp);
    }
    else if (command.equals("set_all"))
    {
        handle_set_all_cmd(json_msg,json_rsp);

    }
    else if (command.equals("get_num_dev"))
    {
        handle_get_num_device_cmd(json_msg,json_rsp);
    }
    else
    {
        json_rsp["success"] = false;
        json_rsp["message"] = "unknown command";
    }
} 


void SystemState::handle_set_cmd(JsonObject &json_msg, JsonObject &json_rsp)
{
    JsonVariant value_var = json_msg["value"];

    for (auto dev_kv : value_var.as<JsonObject>())
    { 

        uint16_t dev_num = String(dev_kv.key).toInt();
        JsonVariant pwm_var = dev_kv.value;
        
        if ((dev_num <0) || (dev_num >= Number_Of_Device))
        {
            continue;
        }

        for (auto pwm_kv : pwm_var.as<JsonObject>())
        {
            uint16_t pwm_num = String(pwm_kv.key).toInt();
            if ((pwm_num < 0) || (pwm_num >= Number_Of_Pwm))
            {
                continue;
            }

            uint16_t  pwm_val = pwm_kv.value.as<uint16_t>();
            if ((pwm_val < Min_Pwm_Value) || (pwm_val > Max_Pwm_Value)) 
            {
                continue;
            }

            set(dev_num, pwm_num, pwm_val, false);
        }
    }
    tlc5947_.write();
}


void SystemState::handle_set_all_cmd(JsonObject &json_msg, JsonObject &json_rsp)
{
    int value = json_msg["value"] | -1;

    if ((value < Min_Pwm_Value) || (value > Max_Pwm_Value))
    {
        json_rsp["success"] = false;
        json_rsp["message"] = "value must be integer in range [0,255]";
    }
    else
    {
        set_all(value);
        json_rsp["success"] = true;
    }
}


void SystemState::handle_get_num_device_cmd(JsonObject &json_msg, JsonObject &json_rsp)
{ 
    json_rsp["success"] = true;
    json_rsp["num_dev"] = Number_Of_Device;
}

void SystemState::set(uint16_t dev_num, uint16_t pwm_num, uint16_t value, bool write)
{
    tlc5947_.setPWM((dev_num+1)*pwm_num, value);
    if (write)
    {
        tlc5947_.write();
    }
}

void SystemState::set_all(uint16_t value)
{
    for (int dev_num=0; dev_num<Number_Of_Device; dev_num++)
    {
        for (int pwm_num=0; pwm_num<Number_Of_Pwm; pwm_num++)
        {
            tlc5947_.setPWM((dev_num+1)*pwm_num, value);
        }
        tlc5947_.write();
    }
}


//for (auto kv : json_msg)
//{
//    Serial << (kv.key) << " ";
//    (kv.value).printTo(Serial); 
//    Serial << endl;
//}
