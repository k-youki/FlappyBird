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

bool MainScene::init()
{
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
    this->character = this->background->getChildByName<Character*>("character");
    this->ground[0] = rootNode->getChildByName("ground0");
    this->ground[1] = rootNode->getChildByName("ground1");
    this->scoreLabel = this->background->getChildByName<ui::TextBMFont*>("scoreLabel");
    score = 0;
    this->setScore(score);
    
    this->character->setLocalZOrder(1);
    this->scoreLabel->setLocalZOrder(1);
    
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
    switch (this->state) {
        case State::Ready:
            this->updateReady();
            break;
        case State::Playing:
            this->updatePlaying();
            break;
        case State::GameOver:
            this->updateGameOver();
            break;
    }
}

void MainScene::updateReady()
{
    // 床の移動
    for (int i = 0; i < GROUND_NUM; i++) {
        ground[i]->setPosition(ground[i]->getPosition() + Vec2(-SCROLL_SPEED_X,0));
    }
    
    // 床のループ判定
    for (int i = 0; i < GROUND_NUM; i++) {
        if (ground[i]->getPositionX() < -DISPLAY_WIDTH){
            ground[i]->setPosition(Vec2(DISPLAY_WIDTH,GROUND_Y));
        }
    }
}

void MainScene::updatePlaying()
{
    // 障害物とキャラの衝突判定
    Rect characterRect = character->getRect();
    
    // 床の移動
    for (int i = 0; i < GROUND_NUM; i++) {
        ground[i]->setPosition(ground[i]->getPosition() + Vec2(-SCROLL_SPEED_X,0));
    }
    
    // 床のループ判定
    for (int i = 0; i < GROUND_NUM; i++) {
        if (ground[i]->getPositionX() < -DISPLAY_WIDTH){
            ground[i]->setPosition(Vec2(DISPLAY_WIDTH,GROUND_Y));
        }
    }
    
    // 障害物の移動
    for (auto obstacle : this->obstacles) {
        obstacle->moveLeft(SCROLL_SPEED_X);
    }
    
    // 土管との衝突判定
    for (auto obstacle : this->obstacles) {
        auto obstacleRects = obstacle->getRects();
        
        for (Rect obstacleRect : obstacle->getRects()) {
            bool hit = characterRect.intersectsRect(obstacleRect);
            if (hit){
                //this->unscheduleUpdate();
                //this->unscheduleAllCallbacks();
                triggerGameOver();
            } else {
            }
        }
    }
    
    // 床にあたったとき
    auto groundRects = this->getGroundRects();
    for (Rect groundRect : groundRects) {
        bool hit = characterRect.intersectsRect(groundRect);
        if (hit){
            CCLOG("ground hit");
            this->unscheduleUpdate();
            this->unscheduleAllCallbacks();
            triggerGameOver();
            character->stopFly();
        } else {
        }
    }
    
    for (auto obstacle : this->obstacles) {
        float currentX = obstacle->getPositionX();
        float lastX = currentX - SCROLL_SPEED_X;
        if (lastX < this->character->getPositionX() && this->character->getPositionX() <= currentX) {
            this->setScore(++score);
        }
    }

}

void MainScene::updateGameOver()
{
    // 障害物とキャラの衝突判定
    Rect characterRect = character->getRect();
    
    // 床にあたったとき
    auto groundRects = this->getGroundRects();
    for (Rect groundRect : groundRects) {
        bool hit = characterRect.intersectsRect(groundRect);
        if (hit){
            character->stopFly();
        } else {
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
    this->setScore(0);
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

void MainScene::setScore(int score)
{
    this->score = score;
    this->scoreLabel->setString(std::to_string(score));
}

