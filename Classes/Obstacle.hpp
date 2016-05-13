//
//  Obstacle.hpp
//  FlappyBird
//
//  Created by Yuki Kotani on 2016/05/12.
//
//

#ifndef Obstacle_hpp
#define Obstacle_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class Obstacle : public cocos2d::Node{
    
public:
    CREATE_FUNC(Obstacle);
    
    bool init() override;
    void moveLeft(float distance);
    std::vector<cocos2d::Rect> getRects();
};

#endif /* Obstacle_hpp */
