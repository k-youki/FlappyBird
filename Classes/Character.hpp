//
//  Character.hpp
//  FlappyBird
//
//  Created by Yuki Kotani on 2016/05/11.
//
//

#ifndef Character_hpp
#define Character_hpp

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class Character : public cocos2d::Node {
public:
    CREATE_FUNC(Character);
    bool init() override;
    void onEnter() override;
    void update(float dt) override;
    void jump();
    
protected:
    cocostudio::timeline::ActionTimeline* timeline;
    
    float velocity;
    float accel;
};
#endif /* Character_hpp */
