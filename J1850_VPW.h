// J1850 VPW protocol definitions and class declaration for Arduino Nano

#ifndef J1850_VPW_H
#define J1850_VPW_H

class J1850_VPW {
public:
    J1850_VPW();
    void begin();
    void sendData(byte data);
    byte receiveData();
    bool isMessageReceived();

private:
    // Private members for internal handling
};

#endif // J1850_VPW_H
