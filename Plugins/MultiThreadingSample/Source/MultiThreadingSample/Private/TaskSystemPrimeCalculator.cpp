#include "TaskSystemPrimeCalculator.h"
#include "PrimeHelper.h"
#include "Tasks/Task.h"
#include "Engine/Engine.h"

// 언리얼 엔진의 태스크 시스템(UE::Tasks)은 태스크 그래프를 개선한 현대적 멀티스레딩 솔루션입니다.
// - UE::Tasks::Launch는 람다를 비동기적으로 실행하며, 첫 번째 인자는 디버깅을 위한 UE_SOURCE_LOCATION, 두 번째 인자는 실행할 람다입니다.
// - 이 예제에서는 Start와 Range를 값으로 캡처하여, 지정된 범위 내의 숫자 각각에 대해 FPrimeHelper::IsPrime으로 소수 여부를 판별하고, 소수이면 UE_LOG로 출력합니다.
// - 작업은 백그라운드에서 실행되어 메인 스레드의 부하를 줄입니다.
// - 이 방식은 복잡한 설정 없이 병렬 처리가 필요한 간단한 작업에 적합합니다.
// - 태스크 시스템은 작업 종속성, 파이프, 중첩 작업 등 고급 기능도 지원하여 복잡한 워크플로와 확장성 있는 멀티스레드 처리가 가능합니다.

void FTaskSystemPrimeCalculator::Calcalate(int32 Start, int32 Range)
{
	double StartTime = FPlatformTime::Seconds();

	UE::Tasks::Launch(UE_SOURCE_LOCATION, [Start, Range]()
	{
		for (int32 Number = Start; Number < Start + Range; ++Number)
		{
			if (FPrimeHelper::IsPrime(Number))
			{
				UE_LOG(LogTemp, Log, TEXT("TaskSystem - Prime : %d"), Number);
			}
		}
	});

	double EndTime = FPlatformTime::Seconds();
	double Elapsed = EndTime - StartTime;
	UE_LOG(LogTemp, Log, TEXT("TaskSystem - Prime 계산 소요 시간: %.6f초"), Elapsed);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("TaskSystem - Prime 계산 소요 시간: %.6f초"), Elapsed));
}
