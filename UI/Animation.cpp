
#include "Animation.h"

using namespace UI;

Animation::AnimationHandler Animation::animationHandler{};

/******************************************************************************/
/*                              class Animation                               */
/******************************************************************************/

void Animation::onAnimationEnd(AnimationEndCallback callback) {

    const std::lock_guard<std::mutex> lock(animationEndMutex);

    animationEndCallback = callback;

    // If animation has already ended, imediatly call the callback functiona
    // via the animation handler
    if (hasEnded())
        animationHandler.addAnimationToQueue(shared_from_this());
}

void Animation::end() {

    const std::lock_guard<std::mutex> lock(animationEndMutex);

    {
    std::lock_guard waitEndLock(waitForAnimationEndMutex);
    animationFinished = true;
    }

    animationEndCondition.notify_all();

    // only add animation to animation hander if animation end callback ist set
    if (animationEndCallback)
        animationHandler.addAnimationToQueue(shared_from_this());

}

void Animation::callAnimationEndCallback() const {
    if (animationEndCallback)
        animationEndCallback(widget.lock());
}

void Animation::waitForAnimationToEnd() {

    std::unique_lock lock(waitForAnimationEndMutex);
    animationEndCondition.wait(lock, [this] { return animationFinished; });
    lock.unlock();
}

/******************************************************************************/
/*                      class Animation::AnimationHandler                     */
/******************************************************************************/

Animation::AnimationHandler::AnimationHandler() {
    startAnimationHandlerThread();
}

Animation::AnimationHandler::~AnimationHandler() {
    if (animationThreadRunning)
        stopAnimationHandlerThread();
}

void Animation::AnimationHandler::startAnimationHandlerThread() {
    animationThreadRunning = true;
    animationHandlerThread = std::thread(&Animation::AnimationHandler::animationHandlerThreadFunction, this);
}

void Animation::AnimationHandler::stopAnimationHandlerThread() {
    if (animationThreadRunning) {
        {
        const std::lock_guard<std::mutex> lock(animationQueueMutex);
        animationThreadRunning = false;
        }
        newAnimationInQueueCondition.notify_one();
        animationHandlerThread.join();
    }
}

void Animation::AnimationHandler::animationHandlerThreadFunction() {

    while (animationThreadRunning) {

        std::shared_ptr<Animation> animation = waitForNextAnimationInQueue();
        if (animation)
            animation->callAnimationEndCallback();

    }
}

void Animation::AnimationHandler::addAnimationToQueue(std::shared_ptr<Animation> animation) {

    {
    const std::lock_guard<std::mutex> lock(animationQueueMutex);
    animationQueue.push(animation);
    }

    newAnimationInQueueCondition.notify_one();
}

std::shared_ptr<Animation> Animation::AnimationHandler::waitForNextAnimationInQueue() {

    // wait until new animation is in queue or task is stopped
    std::unique_lock lock(animationQueueMutex);
    newAnimationInQueueCondition.wait(lock, [this]{ return !animationQueue.empty() || !animationThreadRunning; });

    if (animationQueue.empty())
        return nullptr;

    std::shared_ptr<Animation> animation = animationQueue.front();
    animationQueue.pop();
    return animation;
}
