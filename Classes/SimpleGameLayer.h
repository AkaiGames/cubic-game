#ifndef __CUBIC_SIMPLE_GAME_LAYER_HPP__
#define __CUBIC_SIMPLE_GAME_LAYER_HPP__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Timer.h"
#include <sstream>
#include <list>
#include <stack>
#include <random>
#include <algorithm>

class SimpleGameLayer : public cocos2d::LayerColor
{
private:
    const int SPACING = 10;
    const int TO_NEXT_LEVEL = 10; // amounth of correct sets needed level up
    const int MAX_LIVES = 5;
    
    bool _isFirstTilesSet; // if the current set is the first
    
    std::list< std::string > _hiddenCorrectTilesLeft;
    std::stack< std::string > _lives;
    
    int _correctSets;

    int _currentTouchID; // register first touch id, used to prevent multitouch
    bool _running, _over;
    int _level, _lines, _columns, _cubics, _score;
    float _life_square_scale_ratio;
    int _totalTries, _totalCorrect;
    std::list<int> _targets;
    Timer _timer;
    int _secs, _mins;
    cocos2d::Label *_labelTime, *_scoreLabel, *_levelLabel;
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