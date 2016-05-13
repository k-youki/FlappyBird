//
//  Character.cpp
//  FlappyBird
//
//  Created by Yuki Kotani on 2016/05/11.
//
//

#include "Character.hpp"
#include "Constants.h"

using namespace cocos2d;

bool Character::init()
{
    if (!Node::init()) {
        return false;
    }
    
    this->timeline = cocos2d::CSLoader::createTimeline("Character.csb");
    this->timeline->retain();
    
    this->velocity = 0;
    this->accel = GRAVITY_ACCEL;
    
    return true;
}

void Character::onEnter()
{
    Node::onEnter();

    this->scheduleUpdate();
}

void Character::update(float dt)
{
    if (isFlying) {
        this->velocity += accel * dt;
        this->setPosition(this->getPosition() + Vec2(0,this->velocity * dt));
    }
}

void Character::jump()
{
    this->velocity = JUMP_SPEED;
    
    
    // キャラクターに新しいアクションをスタートさせるには、実行中のアクションがある場合はまずそれを止める。
    this->stopAllActions();
    
    // フレームごとにアップデートを取得できるようにタイムラインを実行する
    this->runAction(this->timeline);
    
    // 叩き落しのアニメーションを再生するようタイムラインに伝え、falseのloopを設定する
    this->timeline->play("patapata", false);
    
}

cocos2d::Rect Character::getRect()
{
    auto bird = this->getChildByName<Sprite*>("bird");
    Size size = bird->Sprite::getContentSize() * 0.8f;
    return Rect(this->getPosition() - size/2, size);
}

void Character::startFly()
{
    this->isFlying = true;
}

void Character::stopFly()
{
    // キャラクターに新しいアクションをスタートさせるには、実行中のアクションがある場合はまずそれを止める。
    this->stopAllActions();
    this->isFlying = false;
}