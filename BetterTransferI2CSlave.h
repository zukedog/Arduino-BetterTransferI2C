/******************************************************************
 *  BetterTransferI2CSlave Arduino Library
 *		details and example sketch:
 *			http://www.billporter.info/easytransfer-arduino-library/
 *
 *		Brought to you by:
 *              Mathieu Alorent
 *               Forked from
 *              Bill Porter
 *              www.billporter.info
 *
 *		See Readme for other info and version history
 *
 *
 *This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or(at your option) any later version.
 T his program i*s distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 <http://www.gnu.org/licenses/>
 *
 *This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
 *To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or
 *send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 ******************************************************************/
#ifndef BetterTransferI2CSlave_h
#define BetterTransferI2CSlave_h


//make it a little prettier on the front end. 
#define details(name) (byte*)&name,sizeof(name)

//Not neccessary, but just in case. 
#if ARDUINO > 22
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "HardwareSerial.h"
//#include <NewSoftSerial.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <Wire.h>
#include <LinkedList.h>

class BetterTransferI2CSlave {
public:
    void begin(uint8_t *, uint8_t);
    void sendData();
    static void onSend();
    static void onReceive(int);
    boolean newData();
private:
    bool updated;
    void receiveData();
    //NewSoftSerial *_serial;
    uint8_t * address;  //address of struct
    uint8_t size;       //size of struct
    uint8_t * rx_buffer; //address for temporary storage and parsing buffer
    uint8_t rx_array_inx;  //index for RX parsing buffer
    inline static uint8_t rx_len = 0;		//RX packet length according to the packet
    inline static uint8_t currentIndex = 0;
    inline static uint8_t nextIndex = 0;
    inline static uint8_t requestedIndex = 0;
    uint8_t tx_len;
    uint8_t tx_array_inx;  //index for tX parsing buffer
    uint8_t calc_CS;	   //calculated Chacksum
    inline static LinkedList<BetterTransferI2CSlave*> list = LinkedList<BetterTransferI2CSlave*>();

};

#endif
