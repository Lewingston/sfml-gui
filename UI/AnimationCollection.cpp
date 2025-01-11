
#include "AnimationCollection.h"

using namespace UI;

AnimationCollection::AnimationCollection(const std::vector<std::shared_ptr<Animation>>& animations) :
    Animation(nullptr),
    animations(animations) {

    for (auto animation : animations) {
        animation->onAnimationEnd([this](auto) {
            animationEndCondition.notify_one();
        });
    }
}

void AnimationCollection::doAnimationStep() {

    if (hasEnded())
        return;

    for (auto animation : animations) {
        if (!animation->hasEnded()) {
            return;
        }
    }

    end();
}

void AnimationCollection::waitForAnimationToEnd() {

    if (widget.lock()) {
        Animation::waitForAnimationToEnd();
    } else {
        waitForAnimations();
    }
}

void AnimationCollection::waitForAnimations() {

    std::unique_lock lock(waitForAnimationEndMutex);

    animationEndCondition.wait(lock, [this] {
        for (auto animation : animations) {
            if (!animation->hasEnded())
                return false;
        }
        return true;
    });

    lock.unlock();
}
