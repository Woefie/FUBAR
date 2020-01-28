# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS 
    "main.c"
    "Sensors/controller.c"
    "Sensors/dirSensor.c" 
    "Sensors/yawController.c"
    "Sensors/speedSensor.c"
    "Sensors/pitchController.c" 
    "Sensors/sensors.c" 
    "Peripherals/uart.c"
    "Peripherals/adc.c"
    "Sensors/PID.c")
set(COMPONENT_ADD_INCLUDEDIRS "")

register_component()
