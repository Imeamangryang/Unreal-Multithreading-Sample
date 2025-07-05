#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"


class FPrimeTask : public FNonAbandonableTask
{
public:
	FPrimeTask(int32 InStart, int32 InRange) : Start(InStart), Range(InRange)
	{
	}

	void DoWork();
	static void Calculate(int32 Start, int32 Range);

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FPrimeTask, STATGROUP_ThreadPoolAsyncTasks);
	}
private:
	int32 Start;
	int32 Range;
};