//
// Created by Borin Ouch on 2016-03-22.
//


#include "animation_handler.h"


AnimationHandler::AnimationHandler() {
    _time = 0.0f;
    _currentAnimation = -1;
}

AnimationHandler::AnimationHandler(const sf::IntRect &frameSize) {
    this->frameSize = frameSize;

    _time = 0.0f;
    _currentAnimation = -1;
}

void AnimationHandler::update(const float dt) {
    if (_currentAnimation >= _animations.size() || _currentAnimation < 0) {
        return;
    }

    float duration = _animations[_currentAnimation].duration;

    if (int((_time + dt) / duration) > int(_time / duration)) {
        int frame = int((_time + dt) / duration);

        frame %= _animations[_currentAnimation].getLength();

        sf::IntRect rect = frameSize;
        rect.left = rect.width * frame;
        rect.top = rect.height * _currentAnimation;
        bounds = rect;
    }

    _time += dt;

    if (_time > duration * _animations[_currentAnimation].getLength()) {
        _time = 0.0f;
    }
}

void AnimationHandler::addAnimation(Animation &animation) {
    _animations.push_back(animation);
}

void AnimationHandler::changeAnimation(unsigned int animationId) {
    if (_currentAnimation == animationId || animationId >= _animations.size() || animationId < 0) {
        return;
    }

    this->_currentAnimation = animationId;

    sf::IntRect rect = frameSize;
    rect.top = rect.height * animationId;

    bounds = rect;
    _time = 0.0f;
}
