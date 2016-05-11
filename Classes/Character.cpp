//
//  Character.cpp
//  FlappyBird
//
//  Created by Yuki Kotani on 2016/05/11.
//
//

#include "Character.hpp"

bool Character::init()
{
    if (!Node::init()) {
        return false;
    }
    
    this->timeline = cocos2d::CSLoader::createTimeline("Character.csb");
    this->timeline->retain();
    
    return true;
}