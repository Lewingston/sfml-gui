#pragma once

#include <memory>
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>

namespace UI {

    class Widget;

    class Animation : public std::enable_shared_from_this<Animation> {

        public:

            using AnimationEndCallback = std::function<void(std::shared_ptr<Widget>)>;

            Animation(std::shared_ptr<Widget> widget) : widget(widget) {};
            virtual ~Animation() = default;

            virtual void doAnimationStep() = 0;

            virtual void onAnimationEnd(AnimationEndCallback callback);

            virtual void waitForAnimationToEnd();

            [[nodiscard]] bool hasEnded() const noexcept { return animationFinished; }

        protected:

            virtual void end();

            virtual void callAnimationEndCallback() const;

            std::weak_ptr<Widget> widget;

            AnimationEndCallback animationEndCallback;

            std::condition_variable animationEndCondition;
            std::mutex waitForAnimationEndMutex;

        private:

            class AnimationHandler {

                public:

                    AnimationHandler();
                    virtual ~AnimationHandler();

                    void addAnimationToQueue(std::shared_ptr<Animation> animation);

                private:

                    void startAnimationHandlerThread();
                    void stopAnimationHandlerThread();

                    std::thread animationHandlerThread;
                    bool animationThreadRunning = false;

                    void animationHandlerThreadFunction();

                    std::shared_ptr<Animation> waitForNextAnimationInQueue();

                    std::mutex animationQueueMutex;
                    std::condition_variable newAnimationInQueueCondition;

                    std::queue<std::shared_ptr<Animation>> animationQueue;

            };

            std::mutex animationEndMutex;
            bool animationFinished = false;

            static AnimationHandler animationHandler;

    };
}
