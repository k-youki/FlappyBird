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
    
protected:
    cocostudio::timeline::ActionTimeline* timeline;
    
};
#endif /* Character_hpp */
