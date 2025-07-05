#include "TaskGraphPrimeCalculator.h"
#include "Async/TaskGraphInterfaces.h"
#include "PrimeHelper.h"

#include "Engine/Engine.h"

// TaskGraph는 Unreal Engine에서 멀티스레딩을 관리하기 위한 시스템으로,
// 작업을 그래프 형태로 구성하여 의존성에 따라 실행 순서를 결정합니다.
// TaskGraph를 사용하면 작업 간의 의존성을 명확하게 정의하고,
// 병렬로 실행할 수 있는 작업들을 효율적으로 관리할 수 있습니다.

void FTaskGraphPrimeCalculator::Calculate(int32 Start, int32 Range)
{
	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Start, Range]()
		{
			double StartTime = FPlatformTime::Seconds();

			for (int32 i = Start; i < Start + Range; ++i)
			{
				if (FPrimeHelper::IsPrime(i))
				{
					UE_LOG(LogTemp, Log, TEXT("TaskGraph - Prime : %d"), i);
				}
			}

			double EndTime = FPlatformTime::Seconds();
			double Elapsed = EndTime - StartTime;
			UE_LOG(LogTemp, Log, TEXT("TaskGraph - Prime 계산 소요 시간: %.6f초"), Elapsed);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("TaskGraph - Prime 계산 소요 시간: %.6f초"), Elapsed));
		},
		TStatId(),
		nullptr,
		ENamedThreads::AnyThread // AnyThread는 엔진이 그래프 시스템에 사용 가능한 모든 작업자 스레드에서 작업을 예약하도록 합니다.
	);

	
}
