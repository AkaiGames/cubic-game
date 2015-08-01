#include "Timer.h"

Timer::Timer()
{
    stop();
}

Timer::~Timer()
{

}

void Timer::start()
{
    _isPaused = false;
}

void Timer::pause()
{
    _isPaused = true;
}

void Timer::resume()
{
    _isPaused = false;
}

void Timer::stop()
{
    _isPaused = true;
    _miliseconds = 0;
    _secondsElapsed = 0;
    _minutesElapsed = 0;
    _hoursElapsed = 0;
}

void Timer::tick(float dt)
{
    // if not paused
    if ( _isPaused == false ) {
        _miliseconds += dt;
        
        if (_miliseconds >= 1.000) // 1.000 equals to 1 second 
        {
            _secondsElapsed += int(_miliseconds);
            _miliseconds -= int(_miliseconds);
            
            if ( _secondsElapsed == 60 ) 
            {
                _minutesElapsed += 1;
                _secondsElapsed = 00;
            }
            
            if ( _minutesElapsed == 60 ) 
            {
                _hoursElapsed += 1;
                _minutesElapsed = 00;
            }
        }
    }
}

int Timer::getMiliseconds() const 
{
    return _miliseconds;
}

int Timer::getSeconds() const
{
    return _secondsElapsed;
}

int Timer::getMinutes() const 
{
    return _minutesElapsed;
}

int Timer::getHours() const
{
    return _hoursElapsed;
}


