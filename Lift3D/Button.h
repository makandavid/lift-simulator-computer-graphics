#pragma once
#include <glm/glm.hpp>

struct ElevatorButton
{
    glm::vec3 position;
    glm::vec3 size;
    int action;   // what button does
};

enum ButtonAction
{
    FLOOR_SU = -1,
    FLOOR_PR = 0,
    FLOOR_1 = 1,
    FLOOR_2 = 2,
    FLOOR_3 = 3,
    FLOOR_4 = 4,
    FLOOR_5 = 5,
    FLOOR_6 = 6,

    DOOR_OPEN,
    DOOR_CLOSE,
    STOP,
    FAN,
    CALL_ELEVATOR
};
