#ifndef __CUBIC_TIMER_H__
#define __CUBIC_TIMER_H__

#include "cocos2d.h"

class Timer {
private:
    float _miliseconds;
    int _secondsElapsed;
    int _minutesElapsed;
    int _hoursElapsed;
    bool _isPaused;
public:
    Timer();
    ~Timer();
    void start();
    void resume();
    void pause();
    void stop();
    void tick(float dt);
    int getMiliseconds() const; 
    int getSeconds() const;
    int getMinutes() const;
    int getHours() const;
};

#endif