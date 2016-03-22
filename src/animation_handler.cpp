//
// Created by Borin Ouch on 2016-03-22.
//

#include "animation_handler.h"


AnimationHandler::AnimationHandler() {
    t = 0.0f;
    currentAnim = -1;
}

AnimationHandler::AnimationHandler(const sf::IntRect &frameSize) {
    this->frameSize = frameSize;

    t = 0.0f;
    currentAnim = -1;
}

void AnimationHandler::update(const float dt) {
    if (currentAnim >= animations.size() || currentAnim < 0) {
        return;
    }

    float duration = animations[currentAnim].duration;

    if (int((t + dt) / duration) > int(t / duration)) {
        int frame = int((t + dt) / duration);

        frame %= animations[currentAnim].getLength();

        sf::IntRect rect = frameSize;
        rect.left = rect.width * frame;
        rect.top = rect.height * currentAnim;
        bounds = rect;
    }

    t += dt;

    if (t > duration * animations[currentAnim].getLength()) {
        t = 0.0f;
    }
}

void AnimationHandler::addAnim(Animation& anim) {
    animations.push_back(anim);
}

void AnimationHandler::changeAnim(unsigned int animId) {
    if (currentAnim == animId || animId >= animations.size() || animId < 0) {
        return;
    }

    this->currentAnim = animId;

    sf::IntRect rect = frameSize;
    rect.top = rect.height * animId;

    bounds = rect;
    t = 0.0f;
}
