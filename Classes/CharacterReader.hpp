//
//  CharacterReader.hpp
//  FlappyBird
//
//  Created by Yuki Kotani on 2016/05/11.
//
//
#include "cocos2d.h"
#include "cocostudio/WidgetReader/NodeReader/NodeReader.h"

class CharacterReader : public cocostudio::NodeReader
{
public:
    static CharacterReader* getInstance();
    static void purge();
    cocos2d::Node* createNodeWithFlatBuffers(const flatbuffers::Table* nodeOptions);
};
