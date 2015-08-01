#ifndef __CUBIC_SIMPLE_GAME_LAYER_HPP__
#define __CUBIC_SIMPLE_GAME_LAYER_HPP__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Timer.h"
#include <sstream>
#include <list>
#include <random>
#include <algorithm>

class SimpleGameLayer : public cocos2d::LayerColor
{
private:
    const int SPACING = 10;
    const int TO_NEXT_LEVEL = 10; // amounth of correct sets needed level up
    std::list< std::string > hiddenCorrectTilesLeft;

    int _currentTouchID; // register first touch id, used to prevent multitouch
    bool running = false;
    int level, lines, columns, cubics, score;
    int totalTries, totalCorrect;
    std::list<int> targets;
    Timer timer;
    int _secs, _mins;
    cocos2d::Label *labelTime, *scoreLabel;
public: 
    SimpleGameLayer();
    virtual ~SimpleGameLayer();
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    virtual void update(float dt) override;
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void buildSquareBoard();
    void prepareForNextLevel();
    void setupGame();
    void setupHUD();
    CREATE_FUNC(SimpleGameLayer);
};

#endif