//
// Created by lin-k on 04.10.2016.
//

#include "../include/Controller.h"

Controller::Controller() {
}

Controller::~Controller() {
}

uint8_t Controller::Read() {
    uint8_t value = 0;
    if (index < 8 && buttons[index]) {
        value = 1;
    }
    index++;
    if (strobe & 1) {
        index = 0;
    }
    return value;
}

void Controller::Write(uint8_t arg) {
    strobe = arg;
    if (strobe & 1) {
        index = 0;
    }
}

void Controller::setButtons(bool state[]) {
    for (int i = 0; i < 8; i++) {
        buttons[i] = state[i];
    }
}