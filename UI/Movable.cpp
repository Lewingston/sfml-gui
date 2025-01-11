
#include "Movable.h"

#include <cmath>

using namespace UI;

/******************************************************************************/
/*                             class MoveAnimation                            */
/******************************************************************************/

MoveAnimation::MoveAnimation(int32_t posX, int32_t posY, float speed, std::shared_ptr<Widget> widget) :
    Animation(widget),
    targetPosX(posX),
    targetPosY(posY),
    startPosX(widget->getPosX()),
    startPosY(widget->getPosY()),
    speed(speed) {}

void MoveAnimation::doAnimationStep() {

    if (hasEnded())
        return;

    std::shared_ptr<UI::Widget> widget = this->widget.lock();
    if (!widget)
        return;

    int32_t posX = widget->getPosX();
    int32_t posY = widget->getPosY();

    const int32_t diffX = targetPosX - posX;
    const int32_t diffY = targetPosY - posY;

    float speedScale = static_cast<float>(std::sqrt(diffX*diffX + diffY*diffY)) / speed;

    posX += static_cast<int32_t>(static_cast<float>(diffX) / speedScale);
    posY += static_cast<int32_t>(static_cast<float>(diffY) / speedScale);

    if (startPosX > targetPosX)
        posX = std::clamp(posX, targetPosX, startPosX);
    else
        posX = std::clamp(posX, startPosX, targetPosX);

    if (startPosY > targetPosY)
        posY = std::clamp(posY, targetPosY, startPosY);
    else
        posY = std::clamp(posY, startPosY, targetPosY);

    widget->setPosition(posX, posY);

    if (posX == targetPosX && posY == targetPosY) {
        end();
    }
}

/******************************************************************************/
/*                                 class Movable                              */
/******************************************************************************/

Movable::Movable(int32_t posX, int32_t posY, uint32_t width, uint32_t height) :
    Widget(posX, posY, width, height) {}

std::shared_ptr<MoveAnimation> Movable::moveTo(int32_t posX, int32_t posY, float speed) {

    moveAnimation = std::shared_ptr<MoveAnimation>(new MoveAnimation(posX, posY, speed, shared_from_this()));
    return moveAnimation;

}
