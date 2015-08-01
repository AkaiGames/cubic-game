#include "MenuLayer.h"
#include "SimpleGameLayer.h"

USING_NS_CC;

Scene* MenuLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MenuLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void mode1_callback(Ref *sender )
{
    auto scene = SimpleGameLayer::createScene();
    Director::getInstance()->replaceScene(scene);
}

void mode2_callback(Ref *sender )
{
    log("Not implemented yet..");
}

void mode3_callback(Ref *sender )
{
    log("Not implemented yet..");
}

void credits_callback(Ref *sender )
{
    log("Continue selected");
}

// on "init" you need to initialize your instance
bool MenuLayer::init()
{
    if ( !LayerColor::initWithColor(Color4B(0xE9,0xE9,0xE9,255)) )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // [[-- Game Title

    auto label = Label::createWithTTF("CUBIC", "fonts/monofonto.ttf", visibleSize.width / 5);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    label->setColor(Color3B(0x74, 0x74, 0x74));
    //label->enableGlow(Color4B(100, 50, 100, 100));

    this->addChild(label, 1);
    
    // --]]
    
    // [[-- Menu items

    // add "HelloWorld" splash screen"
    /*auto sprite = Sprite::create("cubic-background.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);*/

    // fontconfig for the menu items
    TTFConfig ttfConfig("fonts/monofonto.ttf", visibleSize.height/15);
    
    auto lblItem1 = Label::createWithTTF(ttfConfig,"LIGHTS ON");
    //lblItem1->enableGlow(Color4B(100, 50, 100, 100));
    lblItem1->setColor(Color3B(0xE9, 0xE9, 0xE9));
    lblItem1->setAnchorPoint(Vec2(0.5,0.5));
    auto lblItem2 = Label::createWithTTF(ttfConfig,"BRIDGE");
    //lblItem2->enableGlow(Color4B(100, 50, 100, 100));
    lblItem2->setColor(Color3B(0xE9, 0xE9, 0xE9));
    lblItem2->setAnchorPoint(Vec2(0.5,0.5));
    auto lblItem3 = Label::createWithTTF(ttfConfig,"MINEFIELD");
    //lblItem3->enableGlow(Color4B(100, 50, 100, 100));
    lblItem3->setColor(Color3B(0xE9, 0xE9, 0xE9));
    lblItem3->setAnchorPoint(Vec2(0.5,0.5));

    auto item1 = MenuItemLabel::create(lblItem1, mode1_callback );
    item1->setAnchorPoint(Vec2(0.5,0.5));
    auto item2 = MenuItemLabel::create(lblItem2, mode2_callback );
    item2->setAnchorPoint(Vec2(0.5,0.5));
    auto item3 = MenuItemLabel::create(lblItem3, mode3_callback );
    item3->setAnchorPoint(Vec2(0.5,0.5));
    
    auto menu = Menu::create(item1, item2, item3, nullptr);
    int vspacing = origin.y + visibleSize.height/2 - lblItem1->getContentSize().height + 50;

    menu->setPosition( Vec2::ZERO );
    menu->setAnchorPoint( Vec2(0.5, 0.5) );
    
    item1->setPosition( Vec2(visibleSize.width/2 + origin.x,
                            vspacing) );
    
    vspacing -= lblItem2->getContentSize().height + 20;
    
    item2->setPosition( Vec2(visibleSize.width/2 + origin.x,
                            vspacing) );
    
    vspacing -= lblItem3->getContentSize().height + 20;
    
    item3->setPosition( Vec2(visibleSize.width/2 + origin.x,
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
    
    auto bgBar2 = DrawNode::create();
    bgBar2->setContentSize( Size(visibleSize.width, item2->getContentSize().height) );
    bgBar2->setAnchorPoint( Vec2(0.5,0.5) );
    bgBar2->setPosition( item2->getPositionX(), item2->getPositionY() ); 
    bgBar2->clear();
    verts[0] = Vec2(0,0);
    verts[1] = Vec2(0,item2->getContentSize().height);
    verts[2] = Vec2(visibleSize.width, item2->getContentSize().height);
    verts[3] = Vec2(visibleSize.width,0);
    bgBar2->drawSolidPoly(verts, 4, Color4F(0x74/255.0, 0x74/255.0, 0x74/255.0, 1.0f));
    addChild(bgBar2, 0);
    
    auto bgBar3 = DrawNode::create();
    bgBar3->setContentSize( Size(visibleSize.width, item3->getContentSize().height) );
    bgBar3->setAnchorPoint( Vec2(0.5,0.5) );
    bgBar3->setPosition( item3->getPositionX(), item3->getPositionY() ); 
    bgBar3->clear();
    verts[0] = Vec2(0,0);
    verts[1] = Vec2(0,item3->getContentSize().height);
    verts[2] = Vec2(visibleSize.width, item3->getContentSize().height);
    verts[3] = Vec2(visibleSize.width,0);
    bgBar3->drawSolidPoly(verts, 4, Color4F(0x74/255.0, 0x74/255.0, 0x74/255.0, 1.0f));
    addChild(bgBar3, 0);

    addChild(menu, 2);
    
    return true;
}

void MenuLayer::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
