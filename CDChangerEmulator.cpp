// CD Changer Emulator Implementation

#include "CDChangerEmulator.h"

CDChangerEmulator::CDChangerEmulator() 
    : _currentDisc(0), _currentTrack(1), _playbackState(STATE_STOPPED), _debug(false) {
    
    for (int i = 0; i < MAX_DISCS; i++) {
        _discs[i].isPresent = false;
        _discs[i].tracksAvailable = 0;
        memset(_discs[i].name, 0, DISC_NAME_LENGTH);
    }
}

void CDChangerEmulator::insertDisc(byte discNumber, const char* discName, byte trackCount) {
    if (!isValidDiscNumber(discNumber)) {
        return;
    }
    
    _discs[discNumber].isPresent = true;
    _discs[discNumber].tracksAvailable = trackCount;
    strncpy(_discs[discNumber].name, discName, DISC_NAME_LENGTH - 1);
    
    if (_debug) {
        Serial.print(F("[CD] Disc inserted: "));
        Serial.print(discNumber);
        Serial.print(F(" - "));
        Serial.println(discName);
    }
}

void CDChangerEmulator::removeDisc(byte discNumber) {
    if (!isValidDiscNumber(discNumber)) {
        return;
    }
    
    _discs[discNumber].isPresent = false;
    _discs[discNumber].tracksAvailable = 0;
    
    if (_currentDisc == discNumber) {
        stop();
        _currentDisc = 0;
    }
}

bool CDChangerEmulator::isDiscPresent(byte discNumber) {
    if (!isValidDiscNumber(discNumber)) {
        return false;
    }
    return _discs[discNumber].isPresent;
}

void CDChangerEmulator::selectDisc(byte discNumber) {
    if (!isValidDiscNumber(discNumber) || !isDiscPresent(discNumber)) {
        return;
    }
    
    _currentDisc = discNumber;
    _currentTrack = 1;
}

void CDChangerEmulator::selectTrack(byte trackNumber) {
    if (!isValidTrackNumber(trackNumber)) {
        return;
    }
    _currentTrack = trackNumber;
}

void CDChangerEmulator::play() {
    if (isDiscPresent(_currentDisc)) {
        _playbackState = STATE_PLAYING;
        _playStartTime = millis();
    }
}

void CDChangerEmulator::pause() {
    if (_playbackState == STATE_PLAYING) {
        _playbackState = STATE_PAUSED;
    }
}

void CDChangerEmulator::stop() {
    _playbackState = STATE_STOPPED;
    _currentTrack = 1;
}

void CDChangerEmulator::nextDisc() {
    byte nextDisc = (_currentDisc + 1) % MAX_DISCS;
    int attempts = 0;
    while (!isDiscPresent(nextDisc) && attempts < MAX_DISCS) {
        nextDisc = (nextDisc + 1) % MAX_DISCS;
        attempts++;
    }
    if (attempts < MAX_DISCS) {
        selectDisc(nextDisc);
        play();
    }
}

void CDChangerEmulator::previousDisc() {
    byte prevDisc = (_currentDisc - 1 + MAX_DISCS) % MAX_DISCS;
    int attempts = 0;
    while (!isDiscPresent(prevDisc) && attempts < MAX_DISCS) {
        prevDisc = (prevDisc - 1 + MAX_DISCS) % MAX_DISCS;
        attempts++;
    }
    if (attempts < MAX_DISCS) {
        selectDisc(prevDisc);
        play();
    }
}

void CDChangerEmulator::nextTrack() {
    if (_currentTrack < getDiscTrackCount(_currentDisc)) {
        _currentTrack++;
    }
}

void CDChangerEmulator::previousTrack() {
    if (_currentTrack > 1) {
        _currentTrack--;
    }
}

byte CDChangerEmulator::getDiscTrackCount(byte discNumber) {
    if (!isValidDiscNumber(discNumber)) {
        return 0;
    }
    return _discs[discNumber].tracksAvailable;
}

const char* CDChangerEmulator::getDiscName(byte discNumber) {
    if (!isValidDiscNumber(discNumber)) {
        return "";
    }
    return _discs[discNumber].name;
}

byte CDChangerEmulator::getStatusByte() {
    byte status = 0;
    
    if (_playbackState == STATE_PLAYING) {
        status |= STATUS_PLAYING;
    } else if (_playbackState == STATE_PAUSED) {
        status |= STATUS_PAUSED;
    } else {
        status |= STATUS_STOPPED;
    }
    
    if (isDiscPresent(_currentDisc)) {
        status |= STATUS_DISC_PRESENT;
    }
    
    status |= STATUS_POWER_ON;
    
    return status;
}

void CDChangerEmulator::initializeTestDiscs() {
    insertDisc(0, "Album 1", 12);
    insertDisc(1, "Album 2", 10);
    insertDisc(2, "Album 3", 15);
    insertDisc(3, "Album 4", 9);
    insertDisc(4, "Album 5", 11);
    insertDisc(5, "Album 6", 13);
}

void CDChangerEmulator::printStatus() {
    Serial.println(F("=== CD Changer Status ==="));
    Serial.print(F("Current Disc: "));
    Serial.print(_currentDisc);
    Serial.print(F(" - "));
    Serial.println(_discs[_currentDisc].name);
    Serial.print(F("Current Track: "));
    Serial.println(_currentTrack);
}

bool CDChangerEmulator::isValidDiscNumber(byte discNumber) {
    return discNumber < MAX_DISCS;
}

bool CDChangerEmulator::isValidTrackNumber(byte trackNumber) {
    return trackNumber > 0 && trackNumber <= getDiscTrackCount(_currentDisc);
}

void CDChangerEmulator::updatePlaybackTime() {
}