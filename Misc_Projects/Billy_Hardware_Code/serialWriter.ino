void serialWriter(double dataItem){
//This function converts double type data into a byte packet that can be sent to any programmable serial reader (like MATLAB)
//It converts AND WRITES the data, so it returns nothing. Simply call it when you have data to send.
//Interpreting the data packets requires specific formatting on the receiving side. Packet are of this form:

  //1st byte: sign flag. 1 for positive number, 0 for negative
  //2nd-5th bytes: 4 byte representation of uint32 (unsigned long) magnitude of number
    //convert these last 4 bytes from a uint8 array to uint32 in MATLAB, then just add the sign.

  
  //First, create sign flag (1 for positive, 0 for negative)
  byte sign = 1;
  if(dataItem <0){
    sign = 0;
  }
  //Next, cast magnitude of data item to unsigned long (4 byte, uint32 form)
  unsigned long longDataItem = (unsigned long)abs(dataItem);
  //Then, create a 'data buffer' to store the sign and 4 value bytes
  byte dataBuffer[5]; //1x5 byte array
  dataBuffer[0] = sign; //first element is the sign flag
  dataBuffer[1] = longDataItem & 255;//these 4 elements are standard byte packet representation of uint32
  dataBuffer[2] = (longDataItem >> 8)  & 255; //these are logical AND operations to convert to uint8 segments
  dataBuffer[3] = (longDataItem >> 16) & 255;
  dataBuffer[4] = (longDataItem >> 24) & 255;
  //Finally, write the data packet to the serial 
  Serial.write(dataBuffer, sizeof(dataBuffer));
}

long serialReader(){

unsigned long dataItem = 0;
byte idx1 = Serial.read();
byte idx2 = 3;
byte idx3 = 0;
byte idx4 = 0;

byte arr[4] = {idx1, idx2, idx3, idx4};

  num += arr[3] << 24;
  num += arr[2] << 16;
  num += arr[1] << 8;
  num += arr[0];
  Serial.println(num);
}
