
#pragma once

#include "Widget.h"
#include "Event.h"

#include <memory>

namespace UI {

    class RootWidget;

    class Selectable : public virtual Widget {

        public:

            virtual ~Selectable() = default;

            virtual void reset() = 0;

            virtual void handleKeyboardEvent(const KeyBoardEvent&) {}

        protected:

            Selectable() = default;

            bool registerAsSelected();
            std::shared_ptr<RootWidget> getRootWidget() const;

        private:

    };

}
