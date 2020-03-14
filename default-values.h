#ifndef DEFAULTVALUES_H
#define DEFAULTVALUES_H

/* DEFUALT CONFIG FILE GROUPS, KEYS & VALUES */

/* Main group */
#define GROUP_MAIN                  "Main"
#define KEY_COMPONENT_NAME          "ComponentName"

/* Communication group */
#define GROUP_RS485                 "RS485"
#define KEY_SLAVE_ADDRESS           "SlaveAddress"
#define KEY_SERIAL_DEVICE           "SerialDevice"
#define KEY_BAUD_RATE               "BaudRate"
#define KEY_DATA_BITS               "DataBits"
#define KEY_PARITY                  "Parity"
#define KEY_STOP_BITS               "StopBits"
#define KEY_LOOP_DELAY_MS           "LoopDelayMS"
#define KEY_PROTOCOL_DELAY          "ProtocolDelay"
#define VALUE_SLAVE_ADDRESS         "1"
#define VALUE_SERIAL_DEVICE         "/dev/ttyS0"
#define VALUE_BAUD_RATE             "38400"
#define VALUE_DATA_BITS             "8"
#define VALUE_PARITY                "N"
#define VALUE_STOP_BITS             "1"
#define VALUE_LOOP_DELAY_MS         "100"
#define VALUE_PROTOCOL_DELAY        "4"

/* Spindle control group */
#define GROUP_CONTROL               "Control"
#define KEY_RUN_FWD                 "RunForwardValue"
#define KEY_RUN_REV                 "RunReverseValue"
#define KEY_STOP                    "StopValue"

/* Spindle RPM-in group */
#define GROUP_SPINDLE_IN            "SpindleRpmIn"
#define KEY_MAX_SPEED_RPM           "MaxSpeedRPM"
#define KEY_MIN_SPEED_RPM           "MinSpeedRPM"
//#define VALUE_MAX_SPEED_RPM       "24000"
//#define VALUE_MIN_SPEED_RPM       "3000"

/* Spindle RPM-out group */
#define GROUP_SPINDLE_OUT           "SpindleRpmOut"
#define KEY_AT_SPEED_THRESHOLD      "AtSpeedThreshold"
#define VALUE_AT_SPEED_THRESHOLD    "0.99"

/* User parameter group */
#define KEY_PIN_TYPE                "PinType"
#define KEY_PIN_NAME                "PinName"

/* Common */
#define KEY_ADDRESS                 "Address"
#define KEY_DIVIDER                 "Divider"
#define KEY_MULTIPLIER              "Multiplier"
#define VALUE_DIVIDER               "1"
#define VALUE_MULTIPLIER            "1"

#endif // DEFAULTVALUES_H
