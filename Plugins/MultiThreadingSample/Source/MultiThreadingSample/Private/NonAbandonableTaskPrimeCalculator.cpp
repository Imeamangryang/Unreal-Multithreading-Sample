#include "NonAbandonableTaskPrimeCalculator.h"
#include "PrimeHelper.h"

#include "Engine/Engine.h" // Ensure this include is present for GEngine  

// NonAbandonableTask란?
// Unreal Engine의 비동기 작업(Task) 시스템에서 사용되는 개념으로,
// 해당 Task가 실행을 시작하면 중간에 작업을 포기(Abandon)할 수 없는 작업 유형을 의미합니다.
// 즉, NonAbandonableTask는 반드시 끝까지 실행되어야 하며, 
// 작업이 완료될 때까지 시스템이 해당 Task의 리소스를 유지합니다.
// 주로 중요한 연산이나 중단되면 안 되는 작업에 사용됩니다.

void FPrimeTask::DoWork()
{

	double StartTime = FPlatformTime::Seconds();

	for (int32 Number = Start; Number < Start + Range; ++Number)
	{
		if (FPrimeHelper::IsPrime(Number))
		{
			UE_LOG(LogTemp, Log, TEXT("NonAbandonableTask - Prime : %d"), Number);
		}
	}

	double EndTime = FPlatformTime::Seconds();
	double Elapsed = EndTime - StartTime;
	UE_LOG(LogTemp, Log, TEXT("NonAbandonableTask - Prime 계산 소요 시간: %.6f초"), Elapsed);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("NonAbandonableTask - Prime 계산 소요 시간: %.6f초"), Elapsed));
}

void FPrimeTask::Calculate(int32 Start, int32 Range)
{
	(new FAutoDeleteAsyncTask<FPrimeTask>(Start, Range))->StartBackgroundTask();
}
