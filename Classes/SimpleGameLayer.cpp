#include "SimpleGameLayer.h"
#include "MenuLayer.h"

USING_NS_CC;

SimpleGameLayer::SimpleGameLayer()
{
    
}

SimpleGameLayer::~SimpleGameLayer()
{
    labelTime->release();
    scoreLabel->release();
}

Scene* SimpleGameLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SimpleGameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool SimpleGameLayer::init()
{
    if( !LayerColor::initWithColor(Color4B(0xE9,0xE9,0xE9,255)) )
    {
        return false;
    }
    
    srand(time(NULL));
    
    setupGame();
    
    setupHUD();
    
    buildSquareBoard();
    
    scheduleUpdate();
    
    return true;
}

void SimpleGameLayer::setupHUD()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto clockSprite = Sprite::create("alarm-clock.png");
    clockSprite->setAnchorPoint(Vec2(0.5,0.5));
    clockSprite->setScale(0.6);
    clockSprite->setOpacity(120);
    clockSprite->setPosition(Vec2(clockSprite->getBoundingBox().size.width/2, 
                                visibleSize.height - clockSprite->getBoundingBox().size.height/1.5));
    addChild(clockSprite);
    
    TTFConfig ttfConfig("fonts/larabiefontrg.ttf", visibleSize.width/12);

    labelTime = Label::createWithTTF(ttfConfig,"00:00");
    labelTime->setColor(Color3B(0x74, 0x74, 0x74));
    labelTime->setAnchorPoint(Vec2(0.5,0.5));
    
    float v_spacing = visibleSize.height - labelTime->getContentSize().height/1.5;
    
    labelTime->setPosition(Vec2(labelTime->getContentSize().width / 1.5 + clockSprite->getBoundingBox().size.width/2 + 10, 
                                v_spacing));
    labelTime->retain();
    
    addChild(labelTime);
    
    int spacing = 0;
    
    scoreLabel = Label::createWithTTF(ttfConfig,"SCORE: 0");
    scoreLabel->setColor(Color3B(0x74, 0x74, 0x74));
    scoreLabel->setAnchorPoint(Vec2(0,1));
    scoreLabel->setPosition(Vec2(SPACING, 
                                v_spacing));
    spacing = scoreLabel->getBoundingBox().size.height/2 + visibleSize.height / 20.0;
    
    v_spacing = scoreLabel->getContentSize().height/1.5;
    
    scoreLabel->setPosition(Vec2(SPACING, 
                                 spacing));
    
    scoreLabel->retain();
    
    addChild(scoreLabel);
    
    spacing += scoreLabel->getContentSize().height;
    
    auto levelLabel = Label::createWithTTF(ttfConfig, "LEVEL: 01");
    levelLabel->setColor(Color3B(0x74, 0x74, 0x74));
    levelLabel->setAnchorPoint(Vec2(0,1));
    levelLabel->setPosition(Vec2(SPACING, 
                                spacing));
    
    setName("Layer");
    
    addChild(levelLabel);
}

void SimpleGameLayer::setupGame()
{
    _secs = _mins = 0;
    running = false;
    level = 1;
    lines = 4;
    columns = 3;
    cubics = 0;
    score = 0;
    _currentTouchID = -1;
    
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        log("Called func");
    
        // back to menu
        if ( keyCode == EventKeyboard::KeyCode::KEY_ESCAPE ) {
            auto scene = MenuLayer::createScene();
            Director::getInstance()->replaceScene(scene);
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
}


void SimpleGameLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    cocos2d::Layer::onKeyReleased(keyCode, event);
    
    
}


void SimpleGameLayer::buildSquareBoard()
{
    float v_spacing = Director::getInstance()->getVisibleSize().height - 
                            labelTime->getContentSize().height/1.5;
    int l=0, c=0;
    cubics = (lines*columns) / 2;
    totalTries = totalCorrect = 0;
    
    // [[-- Marca os tiles que o player deve lembrar e clicar depois
    
    targets.clear();
    
    for( int i = 0; i < cubics; i++ ) {
        while ( true ) {
            l = rand() % lines,
            c = rand() % columns;
            if ( std::find(targets.begin(), targets.end(), (l * lines + c) ) != targets.end() )
                continue;
            targets.push_back(l * lines + c);
            break;
        }
    }
    
    // --]]
    
    // [[-- De acordo com o número de colunas e linhas, pre-calcula o tamanho dos tiles
    
    v_spacing -= labelTime->getContentSize().height/1.5;
    int pre_calc_square_size = (Director::getInstance()->getVisibleSize().width - (columns+1)*SPACING) / columns;
    
    // --]]
    
    // [[-- Registra TouchListener
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches( true );
    touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
        if ( running == false ) return false;
        
        // we've already spent all our tries.
        if ( totalTries >= cubics ) {
            log("No more clicking!");
            return false;
        }
        
        auto loc = touch->getLocationInView();
        auto target = event->getCurrentTarget();
        auto rect = event->getCurrentTarget()->getBoundingBox();
        if ( rect.containsPoint(touch->getLocation()) ) {
            if ( target->getOpacity() > 0 ) {
                log("You already clicked this tile!!!");
                return false;
            }
            // prevent multi touch
            if ( _currentTouchID < 0 ) {
                _currentTouchID = touch->getID();
                log("First touch: %d", _currentTouchID);
            } else {
                log("Cannot touch again, cuz we already have: %d", _currentTouchID);
                return false;
            }
            return true;
        }
        return false;
    };
    touchListener->onTouchMoved = [=](Touch* touch, Event* event) {
        if ( _currentTouchID != touch->getID() ) {
            return;
        }
        
        log("Touch started: %d", _currentTouchID);  
    };
    touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
        
        if ( _currentTouchID == touch->getID() ) {
            log("Touch ended: %d", _currentTouchID);
            _currentTouchID = -1;
        } else {
            log("Cant end this touch!");
            return;
        }
        
        auto loc = touch->getLocationInView();
        auto target = event->getCurrentTarget();
        auto rect = event->getCurrentTarget()->getBoundingBox();
        if (rect.containsPoint(touch->getLocation())) {
            
            auto callback_before = CallFunc::create([=](){
                totalTries+=1;
                
                if ( target->getZOrder() == 3 ) { // touched correct tile
                    hiddenCorrectTilesLeft.remove( target->getName() ); // removes the correct tile
                    totalCorrect+=1;
                    score += (totalCorrect * level) / (_secs ? _secs : 1);
                } else { // touched wrong tile
                    score -= ((totalTries - totalCorrect) * level) * (_secs ? _secs : 1);
                }
                
                std::stringstream stream;
                stream << "SCORE: " << score;
                scoreLabel->setString(stream.str().c_str());
            });
            auto fadeIn = FadeIn::create(0.0f);
            auto callback_after = CallFunc::create([=](){
                
                if ( totalTries == cubics ) {
                    running = false;
                    timer.pause();
                    
                    Label* label = nullptr;
                    
                    if ( totalCorrect == cubics ) {
                        label = Label::createWithTTF("YOU WON", "fonts/monofonto.ttf", 
                                                      Director::getInstance()->getVisibleSize().width / 5);
                        label->setColor(Color3B(0x42, 0x80, 0xb8));
                        log("WON!");
                    } else {
                        label = Label::createWithTTF("YOU LOST", "fonts/monofonto.ttf", 
                                                      Director::getInstance()->getVisibleSize().width / 5);
                        label->setColor(Color3B(0xd1, 0x64, 0x00));
                        log("LOST!");
                    }
                    
                    label->setPosition( Vec2( Director::getInstance()->getVisibleSize().width / 2 + Director::getInstance()->getVisibleOrigin().x,
                                 Director::getInstance()->getVisibleSize().height / 2 + Director::getInstance()->getVisibleOrigin().y ) );
                    label->setOpacity(0);
                    
                    bool perfect = hiddenCorrectTilesLeft.empty() ? true : false;
                    
                    //auto fade = FadeIn::create(1.f);
                    //auto delay = DelayTime::create(1.0f);
                    auto revealCorrectsCallback = CallFunc::create([=](){
                        while ( hiddenCorrectTilesLeft.empty() == false ) {
                            std::string tileName = hiddenCorrectTilesLeft.front();
                            hiddenCorrectTilesLeft.pop_front();
                            auto fadeIn = FadeTo::create(0.5f, 128);
                            auto delay = DelayTime::create(2.0f);
                            auto fadeOut = FadeOut::create(0.5f);
                            auto seq = Sequence::create(fadeIn, delay, fadeOut, nullptr);
                            getChildByName( tileName )->runAction(seq);
                        }
                    });
                    auto waitPreviousDelay = DelayTime::create(3.f);
                    auto fade2 = FadeOut::create(1.0f);
                    auto nextSetCallback = CallFunc::create([=](){
                        log("Removing..");
                        label->removeFromParentAndCleanup(true);
                        log("Preparing..");
                        prepareForNextLevel();
                        log("Building..");
                        buildSquareBoard();
                    });
                    
                    Sequence* seq = nullptr;
                    
                    // wait only if we have some hidden tiles to show
                    if ( perfect == true )
                        //seq = Sequence::create(fade, delay, revealCorrectsCallback, fade2, nextSetCallback, nullptr);
                        seq = Sequence::create(revealCorrectsCallback, fade2, nextSetCallback, nullptr);
                    else
                        seq = Sequence::create(revealCorrectsCallback, waitPreviousDelay, fade2, nextSetCallback, nullptr);
                    
                    label->runAction(seq);
                    addChild(label, 777);
                }
            });
            auto seq = Sequence::create(callback_before, fadeIn, callback_after, nullptr);
            target->runAction(seq);
        }
    };
    touchListener->onTouchCancelled = [=](Touch* touch, Event* event) {
        log("Touch cancelled");
    };
    
    // --]]
    
    // [[-- Criação dos tiles
    
    for ( int i = 0; i < lines; i++ )
    {
        for( int j = 0; j < columns; j++ )
        {
            int zOrder = 0;
            Sprite* square = Sprite::create("tile-off.png");
            
            std::list<int>::iterator iter;
            
            square->setScale( (float( pre_calc_square_size) / square->getContentSize().width ) );
            square->setAnchorPoint( Vec2(0,1) );
            square->setPosition( ((j+1)*SPACING) + (j*pre_calc_square_size), 
                                  v_spacing); 
            
            iter = std::find(targets.begin(), targets.end(), (i * lines + j) );
            
            Sprite* new_tile = nullptr;
            
            if ( iter != targets.end() ) {
                targets.erase(iter);
                new_tile = Sprite::create("tile-on.png");
                zOrder = 3;
                new_tile->setOpacity(0);
                auto delay_before = DelayTime::create(1.5f);
                auto fadeIn = FadeIn::create(1.0f);
                auto delay_middle = DelayTime::create(1.5f);
                auto fadeOut = FadeOut::create(1.0f);
                auto startCallback = CallFunc::create([=](){
                    running = true;
                    timer.start();
                });
                auto seq = Sequence::create(delay_before, fadeIn, delay_middle, fadeOut, startCallback, nullptr);
                new_tile->runAction(seq);
            } else {
                new_tile = Sprite::create("tile-wrong.png");
                zOrder = 2;
                new_tile->setOpacity(0);
            }
            
            _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), new_tile);
            
            std::stringstream stream;
            stream << "over-" << i << "." << j;
            new_tile->setName(stream.str().c_str());
            
            if ( zOrder == 3 ) { // if it's a correct tile, adds it to the list of hidden correc tiles
                hiddenCorrectTilesLeft.push_back( stream.str() );
            }
            
            new_tile->setScale(square->getScale());
            new_tile->setAnchorPoint(square->getAnchorPoint());
            new_tile->setPosition(square->getPosition());
            
            addChild(new_tile, zOrder);
            
            stream.str("");
            stream.clear();
            
            stream << i << "." << j;
            square->setName(stream.str().c_str());
            
            addChild(square, 1);
        }
        
        v_spacing -= ((pre_calc_square_size) + (SPACING));
    }
}

void SimpleGameLayer::prepareForNextLevel()
{
    timer.stop();
    // reset time
    labelTime->setString("00:00");
    _secs = _mins = 0;
    
    for ( int i = 0; i < lines; i++ )
    {
        for( int j = 0; j < columns; j++ )
        {   
            std::stringstream stream;
            stream << "over-" << i << "." << j;
            auto sprite_over = getChildByName(stream.str().c_str());
            
            auto fadeOut = FadeOut::create(0.5f);
            auto delay = DelayTime::create(1.5f);
            auto removeCallback = CallFunc::create([=](){
                sprite_over->removeFromParentAndCleanup(true);
            });
            auto seq = Sequence::create(fadeOut, delay, removeCallback, nullptr);
            sprite_over->runAction(seq);
            
            
            stream.str("");
            stream.clear();
            stream << i << "." << j;
            getChildByName(stream.str().c_str())->removeFromParentAndCleanup(true);
        }
    }
}

void SimpleGameLayer::update(float dt)
{
    cocos2d::Node::update(dt);
    
    timer.tick(dt);
    
    if (timer.getSeconds() >= _secs || (timer.getSeconds() == 0 && _secs == 59)) {
        
        _secs = timer.getSeconds();
        _mins = timer.getMinutes();
        
        std::stringstream stream;
        
        if ( _mins < 10 ) stream << "0";
        stream << _mins;
        
        stream << ":";
        
        if ( _secs < 10 ) stream << "0";
        stream << _secs;
        
        labelTime->setString(stream.str().c_str());
    }
}