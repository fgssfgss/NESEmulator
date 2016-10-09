//
// Created by lin-k on 04.10.2016.
//

#ifndef NESEMULATOR_CONTROLLER_H
#define NESEMULATOR_CONTROLLER_H

#include "Common.h"

class Controller {
public:
    Controller();

    ~Controller();

    uint8_t Read();

    void Write(uint8_t arg);

    void setButtons(bool state[8]);

private:
    bool buttons[8] = {false, false, false, false, false, false, false, false};
    uint8_t index = 0;
    uint8_t strobe = 0;
};


#endif //NESEMULATOR_CONTROLLER_H
