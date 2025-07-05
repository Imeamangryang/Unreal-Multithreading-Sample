#include "ParallelForPrimeCalculator.h"
#include "Async/ParallelFor.h"

#include "PrimeHelper.h"

#include "Engine/Engine.h" // Ensure this include is present for GEngine  

// ParallelFor는 Unreal Engine에서 멀티스레딩을 활용하여 작업을 병렬로 수행할 수 있도록 해주는 함수입니다.
// 이 함수는 주어진 범위에 대해 병렬로 작업을 실행하며, 각 스레드가 독립적으로 작업을 수행할 수 있도록 합니다.
// ParallelFor를 사용하면 CPU의 멀티코어를 활용하여 작업을 분산 처리할 수 있어,
// 성능을 향상시킬 수 있습니다. 주로 대량의 데이터를 처리하거나 반복적인 작업을 병렬로 수행할 때 유용합니다.
// 순서가 중요한 작업에는 사용하지 않는 것이 좋습니다.

void FParallelForPrimeCalculator::Calculate(int32 Start, int32 Range)
{
	double StartTime = FPlatformTime::Seconds();

	ParallelFor(Range, [Start](int32 Index)
		{
			int32 Number = Start + Index;
			if (FPrimeHelper::IsPrime(Number))
			{
				// Do something with the prime number, e.g., log it or store it
				UE_LOG(LogTemp, Log, TEXT("ParallelFor - Prime : %d"), Number);
			}
		});
	double EndTime = FPlatformTime::Seconds();
	double Elapsed = EndTime - StartTime;
	UE_LOG(LogTemp, Log, TEXT("ParallelFor - Prime 계산 소요 시간: %.6f초"), Elapsed);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ParallelFor - Prime 계산 소요 시간: %.6f초"), Elapsed));
}
