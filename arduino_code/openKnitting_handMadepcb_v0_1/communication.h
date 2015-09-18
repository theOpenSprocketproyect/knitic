#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include "arduino.h"

#include "encoders.h"
#include "solenoids.h"
#include "endLines.h"

class communication {
  private:
    encoders* myEncoders;
    endLines* myEndlines;
    solenoids* mysolenoids;
    unsigned long lastSendTimeStamp;
    int readCnt;
    unsigned int received;
    unsigned int lastReceived;
    unsigned int bitRegister16Solenoid[16];
    char receivedBin[201];
    int dataSize;
    boolean dataReplace;
    boolean startSend200pix;
    byte footer;
    char lf;
  public:
    String _status;
    communication() {
    }
    ~communication() {
    }

    void setup(encoders* _myEncoders, endLines* _myEndlines, solenoids* _mysolenoids) {
      myEncoders = _myEncoders;
      myEndlines = _myEndlines;
      mysolenoids = _mysolenoids;
      lastSendTimeStamp = millis();
      dataSize = 201;
      footer = 126;
      dataReplace = false;
      readCnt = 0;
      lf = '@';    // AT in ASCII
      // table - bit encoding
      unsigned int bitRegister16SolenoidTemp[16] =
      {
        32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1
      };
      for (int i = 0; i < 16; i++) {
        bitRegister16Solenoid[i] = bitRegister16SolenoidTemp[i];
      }
      Serial.setTimeout(10);
    }

    // send data to processing

    void sendSerialToComputer() {
      if ( (myEncoders->lastencoder1Pos != myEncoders->encoder1Pos)  || (millis() - lastSendTimeStamp) > 500 ) {
        lastSendTimeStamp = millis();
        Serial.print(",");
        Serial.print(myEncoders->stitch);
        Serial.print(",");
        Serial.print(myEncoders->headDirection);
        Serial.print(",");
        if (myEndlines->phase) {
          Serial.print("1");
        } else {
          Serial.print("0");
        }
        //myEncoders->last_8segmentEncoder
        //myEncoders->_8segmentEncoder
        /*
        if(myEncoders->last_8segmentEncoder){
          Serial.print("1");
        }else{
          Serial.print("0");
        }
        if(myEncoders->_8segmentEncoder){
          Serial.print("1");
        }else{
          Serial.print("0");
        }
        */
        /*
        Serial.print(",");
        if(myEndlines->started){
          Serial.print("1");
        }else{
          Serial.print("0");
        }
        */
        /*
        Serial.print(",");
        //Serial.print(_status);
        //Serial.print(",");

        for(int i=0;i<16;i++){
          if(mysolenoids->solenoidstateOn[i]){
            Serial.print("1");
          }else{
            Serial.print("0");
          }
        }
        
        /*
        Serial.print(",");
        Serial.print(myEncoders->encoder1Pos);
        Serial.print(",");
        Serial.print(mysolenoids->currentStitchSetup );
        
        Serial.print(",");
        Serial.print(mysolenoids->currentSolenoidIDSetup);
        Serial.print(",");
        */
        //Serial.print(mysolenoids->currentPixState);
        //Serial.print(",");

        /*
        Serial.print(myEndlines->valueEndLineLeft);
        Serial.print(",");
        Serial.print(myEndlines->valueEndLineRight);

        Serial.print(",");
        Serial.print(myEncoders->directionEncoders);
        Serial.print(",");
        Serial.print(myEncoders->lastDirectionEncoders);
        */

        Serial.println(lf);
        myEncoders->lastencoder1Pos = myEncoders->encoder1Pos;
      }
    }

    // recieve from processing
    void receiveAllLine() {
      while(Serial.available()) {
        if ( Serial.readBytesUntil(footer, receivedBin, dataSize)) {
          dataReplace = true;
        }
      }
      if (dataReplace) {
        sendCurrentPixelArray();
        dataReplace = false;
      }
    }
    // send to processing
    void sendCurrentPixelArray() {
      Serial.println(lf);
      for (int i = 0; i < 200; i++) {
        pixelBin[i] = receivedBin[i];
        Serial.print(pixelBin[i]);
      }
      Serial.println(lf);
    }
};

#endif





