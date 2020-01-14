#ifndef TIMER_H
#define TIMER_H

#include "../../DivisionPCH.h"

class Timer
{
public:
	//Singleton design pattern
	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;

	Timer();
	~Timer();

	// SDL timer init
	void Start();

	// Update SDL Frame ticks
	void UpdateFrameTicks();

	// Getters for SDL Deltatime
	float GetDeltaTime() const;

	// SDL SleepTimes
	unsigned int GetSleepTime(const unsigned int fps_) const;
	
	// Determine getters for Current Ticks
	float GetCurrentTick() const;
private:
	unsigned int prevTicks, currentTicks = 0;
};
#endif // !TIMER_H
