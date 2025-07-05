#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeCounter.h"

class FRunnablePrimeCalculator : public FRunnable
{
public:
	FRunnablePrimeCalculator(int32 InStart, int32 InRange) : Start(InStart), Range(InRange), StopTaskCounter() {}

	// FRunnable 인터페이스 구현
	virtual bool Init() override;

	virtual uint32 Run() override;
	
	virtual void Stop() override;
	virtual void Exit() override;

	static void Calculate(int32 Start, int32 Range);

private:
	int32 Start;
	int32 Range;
	FThreadSafeCounter StopTaskCounter;
};