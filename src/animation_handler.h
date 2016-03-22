//
// Created by Borin Ouch on 2016-03-22.
//

#ifndef SFML_TEST_ANIMATION_HANDLER_H
#define SFML_TEST_ANIMATION_HANDLER_H


#include <SFML/Graphics.hpp>

#include <vector>


class Animation {

public:
    unsigned int startFrame;
    unsigned int endFrame;

    float duration;

public:
    Animation(unsigned int startFrame, unsigned int endFrame, float duration):
        startFrame(startFrame),endFrame(endFrame),duration(duration) {
    }

    unsigned int getLength() {
        return endFrame - startFrame + 1;
    }
};


class AnimationHandler {

private:
    std::vector<Animation> animations;

    float t;

    int currentAnim;

public:
    void addAnim(Animation& anim);

    void update(const float dt);

    void changeAnim(unsigned int animNum);

    sf::IntRect bounds;
    sf::IntRect frameSize;

public:
    AnimationHandler();
    AnimationHandler(const sf::IntRect& frameSize);
};


#endif //SFML_TEST_ANIMATION_HANDLER_H
