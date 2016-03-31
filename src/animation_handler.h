//
// Created by Borin Ouch on 2016-03-22.
//

#ifndef SFML_TEST_ANIMATION_HANDLER_H
#define SFML_TEST_ANIMATION_HANDLER_H


#include <SFML/Graphics.hpp>
#include <vector>
#include "animation.h"


class AnimationHandler {

private:
    std::vector<Animation> _animations;
    float _time;
    int _currentAnimation;

public:
    sf::IntRect bounds;
    sf::IntRect frameSize;

public:
    void addAnimation(Animation &animation);
    void update(const float dt);
    void changeAnimation(unsigned int animationId);

public:
    AnimationHandler();
    AnimationHandler(const sf::IntRect& frameSize);
};


#endif //SFML_TEST_ANIMATION_HANDLER_H
