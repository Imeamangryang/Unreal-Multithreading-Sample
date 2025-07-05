#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ETaskType.h"
#include "MultiThreadPrimeSubSystem.generated.h"

UCLASS()
class MULTITHREADINGSAMPLE_API UMultiThreadPrimeSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "MultiThreadingSample")
	void CalculatePrimes(int32 Start, int32 Range, ETaskType TaskType);
};