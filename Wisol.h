#pragma once

#include <Arduino.h>


/**
 * Use Sigfox module with ease
 */
class Wisol {

    public:
        static const int SOFTWARE_RESET = 0;
        static const int SLEEP = 1;
        static const int DEEP_SLEEP = 2;

        /**
         * Construct Wisol from Stream (both Software or Hardware)
         */
        Wisol(Stream* stream) {
            _stream = stream;
        }

        String dumpInfo() {
            return String("ID :\t") + getID() + "\r\nPAC:\t" + getPAC();
        }

        /**
         * 
         */
        void wakeup() {
            _stream->print('\r');
        }

        /**
         * 
         */
        String getID() {
            return cmd("AT$I=10\r");
        }

        /**
         * 
         */
        String getPAC() {
            return cmd("AT$I=11\r");
        }

        /**
         *
         */
        uint16_t getTemp(){
            String temp = cmd("AT$T?\r");

            return temp.toInt();
        }

        /**
         * 
         */
        String setPowerMode(uint8_t mode) {
            return set("AT$P", mode);
        }

        /**
         * 
         */
        String setOutputPower(uint8_t power) {
            return set("ATS302", power);
        }

        /**
         * 
         */
        String enableSoftwareResetMode() {
            return setPowerMode(SOFTWARE_RESET);
        }

        /**
         * 
         */
        String enableSleepMode() {
            return setPowerMode(SLEEP);
        }

        /**
         * 
         */
        String enableDeepSleepMode() {
            return setPowerMode(DEEP_SLEEP);
        }

        /**
         * Send command and get response
         */
        String cmd(String cmd) {
            _stream->print(cmd);

            return read();
        }

        /**
         * Get response
         */
        String read(uint16_t timeout = 15000) {
            uint32_t start = millis();
            String data = "";

            while (millis() - start <= timeout && !_stream->available())
                delay(10);

            while(millis() - start <= timeout && _stream->available()){
                data += (char) _stream->read();
                delay(10);
            }

	        return data;
        }

        /**
         * Send data.
         */
        bool send(const void* data, uint8_t size, bool ack = false) {
            String status = "";
            char output;

            uint8_t* bytes = (uint8_t*) data;

            _stream->print("AT$SF=");

            for(uint8_t i= 0; i<size; ++i)
                _stream->print(bytes[i]);

            if (ack)
                _stream->print(",1");

            _stream->print("\r");

            return read().indexOf("OK") >= 0;
        }


    protected: 
        Stream *_stream;


        /**
         * Set key=value
         */
        String set(const char* key, uint8_t mode) {
            _stream->print(key);
            _stream->print('=');
            _stream->print(mode);
            _stream->print('\r');
            
            return read();
        }
};