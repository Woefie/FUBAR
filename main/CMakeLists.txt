# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS 
    "main.c" 
    "Sensors/dirSensor.c" 
    "Sensors/dirController.c"
    "Sensors/speedSensor.c" 
    "Sensors/sensors.c" 
    "Peripherals/uart.c"
    "Peripherals/adc.c")
set(COMPONENT_ADD_INCLUDEDIRS "")

register_component()
