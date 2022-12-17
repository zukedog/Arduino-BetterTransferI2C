#include "BetterTransferI2CMaster.h"




//Captures address and size of struct
void BetterTransferI2CMaster::begin(uint8_t * ptr, uint8_t length, TwoWire *theSerial){
	address = ptr;
	size = length;
	_serial = theSerial;

	//dynamic creation of rx parsing buffer in RAM
	rx_buffer = (uint8_t*) malloc(size);
}


//Sends out struct in binary, with header, length info and checksum
void BetterTransferI2CMaster::sendData(uint8_t i2c_address){
  uint8_t CS = size;
  _serial->beginTransmission(i2c_address);
  _serial->write(0x06);
  _serial->write(0x85);
  _serial->write(size);
  for(int i = 0; i<size; i++){
    CS^=*(address+i);
    _serial->write(*(address+i));
  }
  _serial->write(CS);
  _serial->endTransmission();
}


boolean BetterTransferI2CMaster::receiveData(uint8_t i2c_address){
  //Send header bits to show that we want info
  _serial->beginTransmission(i2c_address);
  _serial->write(0x06);
  _serial->write(0x85);
  _serial->write(size);//We might not use this on the slave but we are only looking at data if there is 3 bytes or more
  _serial->endTransmission();

  _serial->requestFrom(i2c_address, (uint8_t)(size+4));
  //this size check may be redundant due to the size check below, but for now I'll leave it the way it is.
  //this will block until a 0x06 is found or buffer size becomes less then 3.
  if (!_serial->available() || _serial->read() != 0x06){
    while (_serial->available()){
      _serial->read();
    }
    return false;
  }

    if(_serial->available() < 3){
        return false;
    }

    if (_serial->read() == 0x85){
      rx_len = _serial->read();
      //make sure the binary structs on both Arduinos are the same size.
      if(rx_len != size){
        return false;
      }
    }

  //we get here if we already found the header bytes, the struct size matched what we know, and now we are byte aligned.
  while(_serial->available() && rx_array_inx <= rx_len){
    rx_buffer[rx_array_inx++] = _serial->read();
  }

  if(rx_len == (rx_array_inx-1)){
    //seem to have got whole message
    //last uint8_t is CS
    calc_CS = rx_len;
    for (int i = 0; i<rx_len; i++){
      calc_CS^=rx_buffer[i];
    }

    if(calc_CS == rx_buffer[rx_array_inx-1]){//CS good
      memcpy(address,rx_buffer,size);
      rx_len = 0;
      rx_array_inx = 0;
      return true;
      }

    else{
    //failed checksum, need to clear this out anyway
      rx_len = 0;
      rx_array_inx = 0;
      return false;
    }

  }

  return false;
}
