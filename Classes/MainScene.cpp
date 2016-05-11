#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "CharacterReader.hpp"
#include "Character.hpp"

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

    auto rootNode = CSLoader::createNode("MainScene.csb");

    Size size = Director::getInstance()->getVisibleSize();
    rootNode->setContentSize(size);
    ui::Helper::doLayout(rootNode);
    
    auto back = rootNode->getChildByName("back");
    this->character = back->getChildByName<Character*>("character");
    
    
    addChild(rootNode);

    return true;
}

void MainScene::onEnter()
{
    Layer::onEnter();
    
    this->setupTouchHandling();
}

void MainScene::setupTouchHandling()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    
    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        this->character->jump();
        return true;
    };
    
     this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}