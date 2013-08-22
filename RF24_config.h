
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 ********************************************************************************
 Some modifications for use also with Teensy3x, DUE, but far for being optimized.
 Mods by Max MC Costa (sumotoy)
 */

#ifndef __RF24_CONFIG_H__
#define __RF24_CONFIG_H__

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include <stddef.h>



/*-----------------------------------------------------------------------------------
alternative printf that use only Serial.print commands. if PRINTFENABLED is not
defined it will not considered since all print features will be disabled
------------------------------------------------------------------------------------*/
//#define ALTPRINTF
/*-----------------------------------------------------------------------------------
turning OFF the following will save a lot of ram/eeprom, but you cannot debug anymore
------------------------------------------------------------------------------------*/
#define PRINTFENABLED //disable to turn off completely all sprintf functions in lib
/*----------------------------------------------------------------------------------*/


//DETECT Teensy3 and DUE
#if defined(__arm__) && defined(CORE_TEENSY)//teensy3
	#define TEENSY3X
	#ifdef PRINTFENABLED
		#include <stdarg.h>//needed by emulated sprintf
	#endif
#elif defined(ARDUINO) && defined(__arm__) && !defined(CORE_TEENSY)//due
	#define ARDUE
	#define _BV(x) (1<<(x))
	#ifdef PRINTFENABLED
		#include <stdarg.h>//needed by emulated sprintf
	#endif
#endif


// arduino,Teensy3 and DUE has standard SPI support
#ifdef ARDUINO || defined(TEENSY3X) || defined(ARDUE)
	#include <SPI.h>
#else//prolly maple?
	#include <stdint.h>
	#include <stdio.h>
	#include <string.h>
	extern HardwareSPI SPI;
	#define _BV(x) (1<<(x))
#endif


//handle serial debug when enabled
#undef SERIAL_DEBUG
#ifdef SERIAL_DEBUG
	#define IF_SERIAL_DEBUG(x) ({x;})
#else
	#define IF_SERIAL_DEBUG(x)
#endif


// Avoid spurious warnings
#if 1
	#if !defined(NATIVE) && defined(ARDUINO)//all arduinos and teensy fall in this category
		#if defined(TEENSY3X) || defined(ARDUE)//teensy3 and DUE
			#undef PSTR
		#else//arduinos
			#undef PROGMEM
			#define PROGMEM __attribute__(( section(".progmem.data") ))
			#undef PSTR
		#endif
		#ifdef PRINTFENABLED
			#if defined(TEENSY3X) || defined(ARDUE)//teensy3 and DUE
				#define PSTR(s) (__extension__({static const char __c[] = (s); &__c[0];}))
			#else//arduinos
				#define PSTR(s) (__extension__({static const char __c[] PROGMEM = (s); &__c[0];}))
			#endif
		#endif
	#endif
#endif

// Progmem is Arduino-specific
#if defined(TEENSY3X) ||  defined(ARDUE)//teensy3 and DUE
	typedef uint16_t prog_uint16_t;
	#ifdef PRINTFENABLED
		#define PSTR(x) (x)
		#define printf_P kprintf
		
		#define strlen_P strlen
		#define PRIPSTR "%s"
	#endif
	#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
	
#elif defined(ARDUINO) && !defined(__arm__)//all other arduinos
	#include <avr/pgmspace.h>
	#define PRIPSTR "%S"
#else//maple?
	typedef char const char;
	typedef uint16_t prog_uint16_t;
	#ifdef PRINTFENABLED
		#define PSTR(x) (x)
		#define printf_P printf
		#define strlen_P strlen
	#endif
	
	#define PROGMEM
	#define pgm_read_word(p) (*(p)) 
	#define PRIPSTR "%s"
#endif

#endif // __RF24_CONFIG_H__
// vim:ai:cin:sts=2 sw=2 ft=cpp
