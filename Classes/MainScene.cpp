#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "CharacterReader.hpp"
#include "Character.hpp"
#include "ObstacleReader.hpp"
#include "Obstacle.hpp"

#include "Constants.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    CSLoader* instance = CSLoader::getInstance();
    instance->registReaderObject("CharacterReader", (ObjectFactory::Instance) CharacterReader::getInstance);
    instance->registReaderObject("ObstacleReader" , (ObjectFactory::Instance) ObstacleReader::getInstance);
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    Size size = Director::getInstance()->getVisibleSize();
    rootNode->setContentSize(size);
    ui::Helper::doLayout(rootNode);
    
    this->background = rootNode->getChildByName("back");
    this->character = background->getChildByName<Character*>("character");
    this->ground[0] = rootNode->getChildByName("ground0");
    this->ground[1] = rootNode->getChildByName("ground1");
    
    addChild(rootNode);

    return true;
}

void MainScene::onEnter()
{
    Layer::onEnter();
    this->setupTouchHandling();
    this->scheduleUpdate();
}

void MainScene::update(float dt)
{
    // 障害物の移動
    for (auto obstacle : this->obstacles) {
        obstacle->moveLeft(SCROLL_SPEED_X);
    }
    
    // 障害物とキャラの衝突判定
    Rect characterRect = character->getRect();
    // 土管との衝突判定
    for (auto obstacle : this->obstacles) {
        auto obstacleRects = obstacle->getRects();
        
        for (Rect obstacleRect : obstacle->getRects()) {
            bool hit = characterRect.intersectsRect(obstacleRect);
            if (hit){
                CCLOG("HIT");
                //this->unscheduleUpdate();
                //this->unscheduleAllCallbacks();
                triggerGameOver();
            } else {
                //CCLOG("NO HIT");
            }
        }
    }
    
    // 床にあたったとき
    auto groundRects = this->getGroundRects();
    for (Rect groundRect : groundRects) {
            bool hit = characterRect.intersectsRect(groundRect);
            if (hit){
                CCLOG("HIT");
                this->unscheduleUpdate();
                this->unscheduleAllCallbacks();
                triggerGameOver();
                character->stopFly();
            } else {
                //CCLOG("NO HIT");
            }
        }
    
    if (state == State::Playing){
        // 床の移動
        for (int i = 0; i < 2; i++) {
            ground[i]->setPosition(ground[i]->getPosition() + Vec2(-SCROLL_SPEED_X,0));
        }
    
        // 床のループ判定
        for (int i = 0; i < 2; i++) {
            if (ground[i]->getPositionX() < -288){
                ground[i]->setPosition(Vec2(288,110));
            }
        }
    }
}

void MainScene::setupTouchHandling()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    
    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        switch (this->state) {
            case State::Ready:
                this->character->jump();
                this->triggerPlaying();
                break;
            case State::Playing:
                this->character->jump();
                break;
            case State::GameOver:
                break;
            default:
                break;
        }
        
        return true;
    };
    
     this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void MainScene::createObstacle(float dt)
{
    if (state == State::Playing) {
        Obstacle* obstacle = dynamic_cast<Obstacle*>(CSLoader::createNode("Obstacle.csb"));
        this->obstacles.pushBack(obstacle);
        this->background->addChild(obstacle); // addChildしないと表示されないよ！
    
        float obstacleY = CCRANDOM_0_1() * (OBSTACLE_MAX_Y - OBSTACLE_MIN_Y) + OBSTACLE_MIN_Y;
    
        obstacle->setPosition(Vec2(OBSTACLE_INIT_X ,obstacleY));
    
        if (this->obstacles.size() > OBSTACLE_LIMIT) {
            this->obstacles.front()->removeFromParent();
            this->obstacles.erase(this->obstacles.begin());
        }
    }
}

void MainScene::triggerReady()
{
    this->state = State::Ready;
    this->character->stopFly();
}

void MainScene::triggerPlaying()
{
    this->state = State::Playing;
    this->character->startFly();
    
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::createObstacle),OBSTACLE_TIME_SPAN);
}

void MainScene::triggerGameOver()
{
    this->state = State::GameOver;
}

std::vector<cocos2d::Rect> MainScene::getGroundRects()
{
    std::vector<cocos2d::Rect> rects;
    
    auto ground0 = (Sprite*)ground[0];
    auto ground1 = (Sprite*)ground[1];
    
    auto ground0Rect = ground0->getBoundingBox();
    auto ground1Rect = ground1->getBoundingBox();
    ground0Rect.origin += this->getPosition();
    ground1Rect.origin += this->getPosition();
    
    rects.push_back(ground0Rect);
    rects.push_back(ground1Rect);
    
    return rects;
}

