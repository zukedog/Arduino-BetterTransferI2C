#include "BetterTransferI2CSlave.h"


void BetterTransferI2CSlave::begin(uint8_t * ptr, uint8_t length){
	address = ptr;
	size = length;

	//dynamic creation of rx parsing buffer in RAM
	rx_buffer = (uint8_t*) malloc(size);
    list.add(this);
    BetterTransferI2CSlave::nextIndex++;
}

void BetterTransferI2CSlave::sendData(){
  uint8_t CS = size;
  Wire.write(0x06);
  Wire.write(0x85);
  Wire.write(size);
  //say what it is
  for(int i = 0; i<size; i++){
    CS^=*(address+i);
    Wire.write(*(address+i));
  }
  Wire.write(CS);
}

void BetterTransferI2CSlave::onReceive(int numBytes){
  //start off by looking for the header bytes. If they were already found in a previous call, skip it.
  if(BetterTransferI2CSlave::rx_len == 0){
    //this size check may be redundant due to the size check below, but for now I'll leave it the way it is.
    if(Wire.available() >= 4){
      //this will block until a 0x06 is found or buffer size becomes less then 3.
      while(Wire.read() != 0x06) {
        //This will trash any preamble junk in the serial buffer
        //but we need to make sure there is enough in the buffer to process while we trash the rest
        //if the buffer becomes too empty, we will escape and try again on the next call
        if(Wire.available() < 4)
          return;
      }
      if (Wire.read() == 0x85){
        BetterTransferI2CSlave::rx_len = Wire.read();
        currentIndex = Wire.read();
        if (currentIndex >= nextIndex){
          BetterTransferI2CSlave::rx_len = 0;
          return;
        }
      }
    }
  }
  // run the receiveData for currentIndex
  list.get(currentIndex)->receiveData();
}

void BetterTransferI2CSlave::receiveData(){

  //make sure the binary structs on both Arduinos are the same size.
  if(BetterTransferI2CSlave::rx_len != size){
    BetterTransferI2CSlave::rx_len = 0;
    return;
  }

  //we get here if we already found the header bytes, the struct size matched what we know, and now we are byte aligned.
  if(BetterTransferI2CSlave::rx_len != 0){
    while(Wire.available() && rx_array_inx <= BetterTransferI2CSlave::rx_len){
      rx_buffer[rx_array_inx++] = Wire.read();
    }

    if(BetterTransferI2CSlave::rx_len == (rx_array_inx-1)){
      //seem to have got whole message
      //last uint8_t is CS
      calc_CS = BetterTransferI2CSlave::rx_len;
      for (int i = 0; i<BetterTransferI2CSlave::rx_len; i++){
        calc_CS^=rx_buffer[i];
      }

      if(calc_CS == rx_buffer[rx_array_inx-1]){//CS good
        memcpy(address,rx_buffer,size);
		BetterTransferI2CSlave::rx_len = 0;
		rx_array_inx = 0;
		return; //return true;
		}

	  else{
	  //failed checksum, need to clear this out anyway
		BetterTransferI2CSlave::rx_len = 0;
		rx_array_inx = 0;
		return; //return false;
	  }

    }
  }

  return; //return false;
}

boolean BetterTransferI2CSlave::updateData(){
  return true;
}
