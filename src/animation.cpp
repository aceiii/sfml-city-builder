//
// Created by Borin Ouch on 2016-03-30.
//

#include "animation.h"


Animation::Animation(unsigned int startFrame, unsigned int endFrame, float duration)
    :startFrame(startFrame),endFrame(endFrame),duration(duration) {
}

unsigned int Animation::getLength() {
    return endFrame - startFrame + 1;
}
