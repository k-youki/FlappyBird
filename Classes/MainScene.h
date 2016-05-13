#ifndef __MainScene_SCENE_H__
#define __MainScene_SCENE_H__

#include "cocos2d.h"

class Character;
#include "Obstacle.hpp"

enum class State {
    Ready,
    Playing,
    GameOver,
};

class MainScene : public cocos2d::Layer
{
    
public:
    static cocos2d::Scene* createScene();

    CREATE_FUNC(MainScene);

    virtual bool init() override;
    void onEnter() override;
    void update(float dt) override;
 
    
private:
    Character* character;
    cocos2d::Vector<Obstacle*> obstacles;
    Node* background;
    Node* ground[2];
    Node* hoge;

    State state;
    
    void setupTouchHandling();
    void createObstacle(float dt);
    bool collisionDetection();
    void triggerReady();
    void triggerPlaying();
    void triggerGameOver();
    std::vector<cocos2d::Rect> getGroundRects();
};

#endif // __MainScene_SCENE_H__
