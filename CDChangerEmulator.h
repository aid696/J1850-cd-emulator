#ifndef CDCHANGEREMULATOR_H
#define CDCHANGEREMULATOR_H

#include <vector>
#include <string>
#include <iostream>

class CDChangerEmulator {
public:
    CDChangerEmulator() : currentDisc(0) {
        discs.resize(6);
    }

    void loadDisc(int index, const std::string &discName) {
        if (index >= 0 && index < 6) {
            discs[index] = discName;
            std::cout << "Disc " << discName << " loaded in slot " << index << std::endl;
        } else {
            std::cout << "Invalid disc index!" << std::endl;
        }
    }

    void playDisc(int index) {
        if (index >= 0 && index < 6 && !discs[index].empty()) {
            currentDisc = index;
            std::cout << "Playing disc: " << discs[currentDisc] << std::endl;
        } else {
            std::cout << "No disc in slot " << index << " or invalid index!" << std::endl;
        }
    }

    void nextDisc() {
        currentDisc = (currentDisc + 1) % 6;
        if (!discs[currentDisc].empty()) {
            std::cout << "Playing next disc: " << discs[currentDisc] << std::endl;
        } else {
            std::cout << "No disc in the next slot!" << std::endl;
        }
    }

    void previousDisc() {
        currentDisc = (currentDisc - 1 + 6) % 6;
        if (!discs[currentDisc].empty()) {
            std::cout << "Playing previous disc: " << discs[currentDisc] << std::endl;
        } else {
            std::cout << "No disc in the previous slot!" << std::endl;
        }
    }

private:
    std::vector<std::string> discs;
    int currentDisc;
};

#endif // CDCHANGEREMULATOR_H
