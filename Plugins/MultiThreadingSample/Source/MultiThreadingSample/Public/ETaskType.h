#pragma once

#include "CoreMinimal.h"
#include "ETaskType.generated.h"

UENUM(BlueprintType)
enum class ETaskType : uint8
{
	TaskGraph				UMETA(DisplayName = "Task Graph"),
	TaskSystem				UMETA(DisplayName = "Task System"),
	NonAbandonableTask		UMETA(DisplayName = "FNonAbandonableTask"),
	Runnable				UMETA(DisplayName = "FRunnable"),
	AsyncTask				UMETA(DisplayName = "FAsyncTask"),
	Async					UMETA(DisplayName = "Async"),
	FuturePromise		    UMETA(DisplayName = "Future/Promise"),
	ParallelFor				UMETA(DisplayName = "ParallelFor"),
};