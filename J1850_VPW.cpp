// J1850 VPW Protocol Implementation

#include "J1850_VPW.h"

J1850_VPW::J1850_VPW(uint8_t pin) : _pin(pin), _debug(false), _messageReady(false) {
}

void J1850_VPW::begin() {
    pinMode(_pin, INPUT_PULLUP);
    
    if (_debug) {
        Serial.println(F("[J1850] VPW Initialized on pin: ") + String(_pin));
    }
}

bool J1850_VPW::sendMessage(J1850Message &msg) {
    msg.checksum = calculateCRC(msg.data, msg.length);
    
    if (_debug) {
        Serial.print(F("[J1850] Sending message: "));
        Serial.print(F("Priority=0x") + String(msg.priority, HEX) + " ");
        Serial.print(F("Addr=0x") + String(msg.address, HEX) + " ");
        Serial.print(F("Length=") + String(msg.length) + " ");
        Serial.println(F("CRC=0x") + String(msg.checksum, HEX));
    }
    
    return true;
}

bool J1850_VPW::receiveMessage(J1850Message &msg, unsigned long timeout) {
    unsigned long startTime = millis();
    
    while (millis() - startTime < timeout) {
        if (digitalRead(_pin) == LOW) {
            break;
        }
        delayMicroseconds(100);
    }
    
    if (millis() - startTime >= timeout) {
        return false;
    }
    
    if (_debug) {
        Serial.print(F("[J1850] Received message"));
    }
    
    return true;
}

bool J1850_VPW::isMessageAvailable() {
    return _messageReady;
}

byte J1850_VPW::calculateCRC(byte* data, int length) {
    byte crc = 0;
    for (int i = 0; i < length; i++) {
        crc += data[i];
    }
    return crc;
}

void J1850_VPW::setDebug(bool debug) {
    _debug = debug;
}

void J1850_VPW::sendBit(bool value) {
    if (value) {
        delayMicroseconds(SHORT_PULSE);
    } else {
        delayMicroseconds(LONG_PULSE);
    }
}

bool J1850_VPW::receiveBit() {
    return digitalRead(_pin) == HIGH;
}

void J1850_VPW::sendPulse(unsigned int duration) {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delayMicroseconds(duration);
    digitalWrite(_pin, HIGH);
    pinMode(_pin, INPUT_PULLUP);
}

unsigned int J1850_VPW::receivePulse() {
    unsigned int pulseWidth = 0;
    while (digitalRead(_pin) == HIGH) {
        delayMicroseconds(1);
    }
    while (digitalRead(_pin) == LOW) {
        delayMicroseconds(1);
        pulseWidth++;
    }
    return pulseWidth;
}