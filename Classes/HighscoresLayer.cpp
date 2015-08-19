#include "HighscoresLayer.h"
#include "MenuLayer.h"
#include "CurlWrapper.h"
// rapidjson
#include "../external/rapidjson/document.h"
#include "../external/rapidjson/writer.h"
#include "../external/rapidjson/stringbuffer.h"

USING_NS_CC;

using namespace rapidjson;

cocos2d::Label* lblResult;

Scene* HighscoresLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HighscoresLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void clickCallback(Ref *sender )
{
    std::string response = CurlWrapper::post("http://httpbin.org/post", "name=foo&project=cubic");
    std::stringstream stream;
    stream << "Response: " << response;
    
    Document document;
    document.Parse(response.c_str());
    
    if ( document.IsObject() ) {
        lblResult->setString(response);
        log("Result: %s", response.c_str());
    }
    
}

// on "init" you need to initialize your instance
bool HighscoresLayer::init()
{
    if ( !LayerColor::initWithColor(Color4B(0xE9,0xE9,0xE9,255)) )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // [[-- BACK to MENU Event
    
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        // back to menu
        if ( keyCode == EventKeyboard::KeyCode::KEY_ESCAPE ) {
            auto scene = MenuLayer::createScene();
            Director::getInstance()->replaceScene(scene);
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
    
    // --]]
    
    // [[-- Menu items

    // fontconfig for the menu items
    TTFConfig ttfConfig("fonts/monofonto.ttf", visibleSize.height/30);
    
    auto lblItem1 = Label::createWithTTF(ttfConfig,"LIGHTS ON");
    //lblItem1->enableGlow(Color4B(100, 50, 100, 100));
    lblItem1->setColor(Color3B(0xE9, 0xE9, 0xE9));
    lblItem1->setAnchorPoint(Vec2(0.5,0.5));

    auto item1 = MenuItemLabel::create(lblItem1, clickCallback );
    item1->setAnchorPoint(Vec2(0.5,0.5));
    
    auto menu = Menu::create(item1, nullptr);
    int vspacing = origin.y + visibleSize.height/2 - lblItem1->getContentSize().height + 50;

    menu->setPosition( Vec2::ZERO );
    menu->setAnchorPoint( Vec2(0.5, 0.5) );
    
    item1->setPosition( Vec2(visibleSize.width/2 + origin.x,
                            vspacing) );
    
    auto bgBar = DrawNode::create();
    bgBar->setContentSize( Size(visibleSize.width, item1->getContentSize().height) );
    bgBar->setAnchorPoint( Vec2(0.5,0.5) );
    bgBar->setPosition( item1->getPositionX(), item1->getPositionY() ); 
    bgBar->clear();
    Vec2 verts[4];
    verts[0] = Vec2(0,0);
    verts[1] = Vec2(0,item1->getContentSize().height);
    verts[2] = Vec2(visibleSize.width, item1->getContentSize().height);
    verts[3] = Vec2(visibleSize.width,0);
    bgBar->drawSolidPoly(verts, 4, Color4F(0x74/255.0, 0x74/255.0, 0x74/255.0, 1.0f));
    addChild(bgBar, 0);

    addChild(menu, 2);
    
    // [[-- Label containing the result of curl
    
    lblResult = Label::createWithTTF(ttfConfig,"RESULT HERE");
    //lblItem1->enableGlow(Color4B(100, 50, 100, 100));
    lblResult->setColor(Color3B(0x74, 0x74, 0x74));
    lblResult->setAnchorPoint(Vec2(0.5,0.5));
    lblResult->setPosition( Vec2( lblItem1->getPositionX() + 100, lblItem1->getPositionY() + 100 ) );
    
    addChild(lblResult, 2);
    
    // --]]
    
    return true;
}

void HighscoresLayer::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}