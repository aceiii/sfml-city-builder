//
// Created by Borin Ouch on 2016-03-30.
//

#ifndef SFML_TEST_ANIMATION_H
#define SFML_TEST_ANIMATION_H


class Animation {

public:
    unsigned int startFrame;
    unsigned int endFrame;
    float duration;

public:
    Animation(unsigned int startFrame, unsigned int endFrame, float duration);

public:
    unsigned int getLength();
};


#endif //SFML_TEST_ANIMATION_H
