#pragma once

#include "CoreMinimal.h"
#include "Async/Async.h"

class FFuturePromisePrimeCalculator
{
public:
	static void Calculate(int32 Start, int32 Range);

	static TFuture<TArray<int32>> DoPrimeCalculationTask(int32 Start, int32 Range);
   
};