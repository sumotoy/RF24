/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 This version has been modified for compatibility with Teensy3.
 Should work as is with arduino and due.
 */

/**
 * @file printfile.h
 * Setup necessary to direct stdout to the Arduino Serial library, which enables 'printf'
 */

#ifndef __PRINTF_H__
#define __PRINTF_H__

#if defined(TEENSY3X) || defined(ARDUE)

void printf_begin(void){
}

#elif defined ARDUINO

int serial_putc(char c, FILE *){
	#ifdef PRINTFENABLED
		Serial.write(c);
		return c;
	#else
		return 0;
	#endif
} 

void printf_begin(void){
	#ifdef PRINTFENABLED
		fdevopen(&serial_putc,0);
	#endif
}

#else
#error This example is only for use on Arduino and Teensy3.
#endif // ARDUINO

#endif // __PRINTF_H__
