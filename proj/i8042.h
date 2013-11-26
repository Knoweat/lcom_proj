#ifndef __I8042_H
#define __I8042_H

#define KEYBOARD_IRQ 1
#define MOUSE_IRQ 12

#define OUT_BUF 0x60
#define IN_BUF 0x60
#define DATA_REG 0x60

#define MASK_BREAK_CODE 0x80
#define ESC_BREAK_CODE 0x81

#define STAT_REG 0x64
#define KBC_CMD_REG 0x64

#define M_PAR_ERR 0x80
#define M_TO_ERR 0x40
#define IBF 0x02
#define OBF 0x01

#define ACK 0xFA
#define NACK 0xFE
#define RESEND 0xFE
#define ERROR 0xFC

#define SWITCH_LEDS 0xED

#define ELEMS_MOUSE_PACKET 3

#define MOUSE_EVT 0x03
#define TIMER_EVT 0x01

#define LDOWN 0x01
#define RDOWN 0x02
#define MDOWN 0x04
#define YOVF 0x80
#define XOVF 0x40
#define YSIGN 0x20
#define XSIGN 0x10

#define ENABLE_MOUSE 0xA8
#define WRITE_B 0xD4
#define RESET 0xFF
#define MOUSE_OK 0xAA
#define DEVICE_ID 0x00
#define DISABLE_STREAM_M 0xF5
#define ENABLE_STREAM_M 0xEA
#define SET_SAMPLE_RATE 0xF3
#define DEFAULT_SAMPLE_RATE 0x64
#define SET_RESOLUTION 0xE8
#define DEFAULT_RESOLUTION 0x04
#define SET_SCALING 0xE7
#define READ_CMD_BYTE 0X20
#define WRITE_CMD_BYTE 0x60
#define EN_KBD_MOUSE 0xCF
#define EN_INT_KBD_MOUSE 0x03
#define EN_SEND_PACK 0xF4

#define STAT_REQUEST 0xE9

#define STAT_MODE BIT(6)
#define STAT_MOUSE_ENABLED BIT(5)
#define STAT_SCALING BIT(4)
#define STAT_LBUT BIT(2)
#define STAT_MBUT BIT(1)
#define STAT_RBUT BIT(0)

#define NO_ARG 0x80


#endif
