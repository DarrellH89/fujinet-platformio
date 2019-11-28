#ifndef SIO_H
#define SIO_H

#include <Arduino.h>
#include "config.h"

#ifdef ESP_8266
#include <FS.h>
#define INPUT_PULLDOWN INPUT_PULLDOWN_16 // for motor pin
#elif defined(ESP_32)
#include <SPIFFS.h>
#endif


#define DELAY_T5 600
#define READ_CMD_TIMEOUT 12
#define CMD_TIMEOUT 50
#define STATUS_SKIP 8


/**
   ISR for falling COMMAND
*/
void ICACHE_RAM_ATTR sio_isr_cmd();

/**
   calculate 8-bit checksum.
*/
byte sio_checksum(byte *chunk, int length);

/**
   Get ID
*/
void sio_get_id();

/**
   Get Command
*/
void sio_get_command();

/**
   Get aux1
*/
void sio_get_aux1();

/**
   Get aux2
*/
void sio_get_aux2();

/**
   Read
*/
void sio_read();

/**
   Status
*/
void sio_status();

/**
   Process command
*/

void sio_process();

/**
   Send an acknowledgement
*/
void sio_ack();

/**
   Send a non-acknowledgement
*/
void sio_nak();

/**
   Get Checksum, and compare
*/
void sio_get_checksum();

void sio_incoming();

void setup_sio();

void handle_sio();

#endif // guard