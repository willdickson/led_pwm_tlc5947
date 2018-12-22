#include "system_state.h"

SystemState system_state;

void setup()
{
    system_state.initialize();
}

void loop()
{
    system_state.process_messages();
}

void serialEvent()
{
    system_state.update_on_serial_event();
}


