#include "SimpleGameLayer.h"
#include "MenuLayer.h"

USING_NS_CC;

SimpleGameLayer::SimpleGameLayer()
{
    
}

SimpleGameLayer::~SimpleGameLayer()
{
    _labelTime->release();
    _scoreLabel->release();
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

    _labelTime = Label::createWithTTF(ttfConfig,"00:00");
    _labelTime->setColor(Color3B(0x74, 0x74, 0x74));
    _labelTime->setAnchorPoint(Vec2(0.5,0.5));
    
    float v_spacing = visibleSize.height - _labelTime->getContentSize().height/1.5;
    
    _labelTime->setPosition(Vec2(_labelTime->getContentSize().width / 1.5 + clockSprite->getBoundingBox().size.width/2 + 10, 
                                v_spacing));
    _labelTime->retain();
    
    addChild(_labelTime);
    
    int spacing = 0;
    
    _scoreLabel = Label::createWithTTF(ttfConfig,"SCORE: 0");
    _scoreLabel->setColor(Color3B(0x74, 0x74, 0x74));
    _scoreLabel->setAnchorPoint(Vec2(0,1));
    _scoreLabel->setPosition(Vec2(SPACING, 
                                v_spacing));
    spacing = _scoreLabel->getBoundingBox().size.height/2 + visibleSize.height / 20.0;
    
    v_spacing = _scoreLabel->getContentSize().height/1.5;
    
    _scoreLabel->setPosition(Vec2(SPACING, 
                                 spacing));
    
    _scoreLabel->retain();
    
    addChild(_scoreLabel);
    
    spacing += _scoreLabel->getContentSize().height;
    
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
    _running = false;
    _level = 1;
    _lines = 4;
    _columns = 3;
    _cubics = 0;
    _score = 0;
    _currentTouchID = -1;
    _correctSets = 0;
    
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
                            _labelTime->getContentSize().height/1.5;
    int l=0, c=0;
    _cubics = (_lines*_columns) / 2;
    _totalTries = _totalCorrect = 0;
    
    // [[-- Marca os tiles que o player deve lembrar e clicar depois
    
    _targets.clear();
    
    for( int i = 0; i < _cubics; i++ ) {
        while ( true ) {
            l = rand() % _lines,
            c = rand() % _columns;
            if ( std::find(_targets.begin(), _targets.end(), (l * _lines + c) ) != _targets.end() )
                continue;
            _targets.push_back(l * _lines + c);
            break;
        }
    }
    
    // --]]
    
    // [[-- De acordo com o número de colunas e linhas, pre-calcula o tamanho dos tiles
    
    v_spacing -= _labelTime->getContentSize().height/1.5;
    int pre_calc_square_size = (Director::getInstance()->getVisibleSize().width - (_columns+1)*SPACING) / _columns;
    
    // --]]
    
    // [[-- Registra TouchListener
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches( true );
    touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
        if ( _running == false ) return false;
        
        // we've already spent all our tries.
        if ( _totalTries >= _cubics ) {
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
                _totalTries+=1;
                
                if ( target->getZOrder() == 3 ) { // touched correct tile
                    _hiddenCorrectTilesLeft.remove( target->getName() ); // removes the correct tile
                    _totalCorrect+=1;
                    _score += (_totalCorrect * _level) / (_secs ? _secs : 1);
                } else { // touched wrong tile
                    _score -= ((_totalTries - _totalCorrect) * _level) * (_secs ? _secs : 1);
                }
                
                std::stringstream stream;
                stream << "SCORE: " << _score;
                _scoreLabel->setString(stream.str().c_str());
            });
            auto fadeIn = FadeIn::create(0.0f);
            auto callback_after = CallFunc::create([=](){
                
                if ( _totalTries == _cubics ) {
                    
                    if ( _totalCorrect == _totalTries ) { // perfect
                        _correctSets += 1;
                    }
                    
                    _running = false;
                    _timer.pause();
                    
                    Label* label = nullptr;
                    
                    if ( _totalCorrect == _cubics ) {
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
                    
                    bool perfect = _hiddenCorrectTilesLeft.empty() ? true : false;
                    
                    //auto fade = FadeIn::create(1.f);
                    //auto delay = DelayTime::create(1.0f);
                    auto revealCorrectsCallback = CallFunc::create([=](){
                        while ( _hiddenCorrectTilesLeft.empty() == false ) {
                            std::string tileName = _hiddenCorrectTilesLeft.front();
                            _hiddenCorrectTilesLeft.pop_front();
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
    
    for ( int i = 0; i < _lines; i++ )
    {
        for( int j = 0; j < _columns; j++ )
        {
            int zOrder = 0;
            Sprite* square = Sprite::create("tile-off.png");
            
            std::list<int>::iterator iter;
            
            square->setScale( (float( pre_calc_square_size) / square->getContentSize().width ) );
            square->setAnchorPoint( Vec2(0,1) );
            square->setPosition( ((j+1)*SPACING) + (j*pre_calc_square_size), 
                                  v_spacing); 
            
            square->setOpacity(0);
            auto delay_before_normal = DelayTime::create(0.2f + ((rand() % 30))/100.0 );
            auto fadeIn_normal = FadeIn::create(0.5f);
            auto seq_normal = Sequence::create(delay_before_normal, fadeIn_normal, nullptr);
            square->runAction(seq_normal);
            
            iter = std::find(_targets.begin(), _targets.end(), (i * _lines + j) );
            
            Sprite* new_tile = nullptr;
            
            if ( iter != _targets.end() ) {
                _targets.erase(iter);
                new_tile = Sprite::create("tile-on.png");
                zOrder = 3;
                new_tile->setOpacity(0);
                auto delay_before = DelayTime::create(1.5f);
                auto fadeIn = FadeIn::create(1.0f);
                auto delay_middle = DelayTime::create(1.5f);
                auto fadeOut = FadeOut::create(1.0f);
                auto startCallback = CallFunc::create([=](){
                    _running = true;
                    _timer.start();
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
                _hiddenCorrectTilesLeft.push_back( stream.str() );
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
    _timer.stop();
    // reset time
    _labelTime->setString("00:00");
    _secs = _mins = 0;
    
    for ( int i = 0; i < _lines; i++ )
    {
        for( int j = 0; j < _columns; j++ )
        {   
            std::stringstream stream;
            stream << "over-" << i << "." << j;
            auto sprite_over = getChildByName(stream.str().c_str());
            
            stream.str("");
            stream.clear();
            stream << i << "." << j;
            auto sprite_normal = getChildByName(stream.str().c_str());
            
            auto fadeOut = FadeOut::create(0.5f);
            auto delay = DelayTime::create(0.0f);
            auto removeCallback = CallFunc::create([=](){
                sprite_over->removeFromParentAndCleanup(true);
            });
            
            auto seq = Sequence::create(fadeOut, delay, removeCallback, nullptr);
            sprite_over->runAction(seq);
            
            auto fadeOut2 = FadeOut::create(0.5f);
            auto delay2 = DelayTime::create(0.0f);
            auto removeCallback2 = CallFunc::create([=](){
                sprite_normal->removeFromParentAndCleanup(true);
            });
            
            auto seq2 = Sequence::create(fadeOut2, delay2, removeCallback2, nullptr);
            sprite_normal->runAction(seq2);
        }
    }
    
    if ( _correctSets >= TO_NEXT_LEVEL ) {
        _level += 1;
        _lines += 1;
        _columns += 1;
        _correctSets = 0;
    }
}

void SimpleGameLayer::update(float dt)
{
    cocos2d::Node::update(dt);
    
    _timer.tick(dt);
    
    if (_timer.getSeconds() >= _secs || (_timer.getSeconds() == 0 && _secs == 59)) {
        
        _secs = _timer.getSeconds();
        _mins = _timer.getMinutes();
        
        std::stringstream stream;
        
        if ( _mins < 10 ) stream << "0";
        stream << _mins;
        
        stream << ":";
        
        if ( _secs < 10 ) stream << "0";
        stream << _secs;
        
        _labelTime->setString(stream.str().c_str());
    }
}