//
//  Obstacle.cpp
//  FlappyBird
//
//  Created by Yuki Kotani on 2016/05/12.
//
//

#include "Obstacle.hpp"

using namespace cocos2d;

bool Obstacle::init()
{
    if (!Node::init()){
        return false;
    }
    
    return true;
}

void Obstacle::moveLeft(float distance)
{
    this->setPosition(this->getPosition() + Vec2(-distance,0));
}

std::vector<cocos2d::Rect> Obstacle::getRects()
{
    std::vector<cocos2d::Rect> rects;
   
    auto pipeTop    = this->getChildByName<Sprite*>("pipe_top");
    auto pipeBottom = this->getChildByName<Sprite*>("pipe_bottom");
   
    auto pipeTopRect = pipeTop->getBoundingBox();
    auto pipeBottomRect = pipeBottom->getBoundingBox();
    pipeTopRect.origin += this->getPosition();
    pipeBottomRect.origin += this->getPosition();
    
    rects.push_back(pipeTopRect);
    rects.push_back(pipeBottomRect);
    return rects;
}