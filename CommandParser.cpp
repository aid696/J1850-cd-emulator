// Command Parser Implementation

#include "CommandParser.h"

CommandParser::CommandParser() : _debug(false) {
}

bool CommandParser::parseCommand(J1850Message &msg, CDCommand &cmd) {
    if (!validateMessage(msg)) {
        return false;
    }
    
    if (msg.length < 1) {
        return false;
    }
    
    cmd.commandID = msg.data[0];
    
    if (msg.length >= 2) {
        cmd.discNumber = msg.data[1];
    }
    
    if (msg.length >= 3) {
        cmd.trackNumber = msg.data[2];
    }
    
    if (_debug) {
        Serial.print(F("[Parser] Command: "));\n        Serial.println(getCommandName(cmd.commandID));
    }
    
    return true;
}

void CommandParser::createStatusResponse(J1850Message &response, CDStatus &status) {
    response.priority = 0x10;
    response.address = 0xEE;
    response.length = 5;
    
    response.data[0] = RESP_STATUS;
    response.data[1] = status.statusFlags;
    response.data[2] = status.currentDisc;
    response.data[3] = status.currentTrack;
    response.data[4] = status.maxTracks;
    
    response.checksum = calculateMessageCRC(response);
}

bool CommandParser::validateMessage(J1850Message &msg) {
    if (msg.length == 0 || msg.length > 7) {
        return false;
    }
    
    byte calculatedCRC = calculateMessageCRC(msg);
    if (calculatedCRC != msg.checksum) {
        if (_debug) {
            Serial.println(F("[Parser] Checksum mismatch!"));
        }
        return false;
    }
    
    return true;
}

const char* CommandParser::getCommandName(byte cmdID) {
    switch (cmdID) {
        case CMD_SELECT_DISC:
            return "SELECT_DISC";
        case CMD_NEXT_DISC:
            return "NEXT_DISC";
        case CMD_PREV_DISC:
            return "PREV_DISC";
        case CMD_PLAY:
            return "PLAY";
        case CMD_PAUSE:
            return "PAUSE";
        case CMD_STOP:
            return "STOP";
        case CMD_REQUEST_STATUS:
            return "REQUEST_STATUS";
        case CMD_NEXT_TRACK:
            return "NEXT_TRACK";
        case CMD_PREV_TRACK:
            return "PREV_TRACK";
        default:
            return "UNKNOWN";
    }
}

byte CommandParser::calculateMessageCRC(J1850Message &msg) {
    byte crc = msg.priority + msg.address;
    for (int i = 0; i < msg.length; i++) {
        crc += msg.data[i];
    }
    return crc;
}