#include "RunnablePrimeCalculator.h"
#include "PrimeHelper.h"
#include "Engine/Engine.h"

// Runnable Thread란?
// Unreal Engine에서 Runnable Thread는 별도의 스레드에서 실행될 수 있는 작업 단위를 의미합니다.
// FRunnable 인터페이스를 상속받아 Init(), Run(), Stop(), Exit() 메서드를 구현하며,
// FRunnableThread::Create()를 통해 실제 OS 스레드로 실행됩니다.
// Init()에서 초기화, Run()에서 메인 작업, Stop()에서 중단 요청, Exit()에서 정리 작업을 수행합니다.
// 이를 통해 게임 로직과 별개로 비동기 작업(예: 연산, 파일 IO 등)을 안전하게 처리할 수 있습니다.

bool FRunnablePrimeCalculator::Init()
{
    return true;
}

uint32 FRunnablePrimeCalculator::Run()
{
	double StartTime = FPlatformTime::Seconds();

	for (int32 Number = Start; Number < Start + Range; ++Number)
	{
		if (StopTaskCounter.GetValue() == 1)
		{
			break;
		}
		if (FPrimeHelper::IsPrime(Number))
		{
			// Do something with the prime number, e.g., log it or store it
			UE_LOG(LogTemp, Log, TEXT("Runnable - Prime : %d"), Number);
		}
	}

	double EndTime = FPlatformTime::Seconds();
	double Elapsed = EndTime - StartTime;
	UE_LOG(LogTemp, Log, TEXT("Runnable - Prime 계산 소요 시간: %.6f초"), Elapsed);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Runnable - Prime 계산 소요 시간: %.6f초"), Elapsed));

	return 0;
}

void FRunnablePrimeCalculator::Stop()
{
	StopTaskCounter.Increment();
}

void FRunnablePrimeCalculator::Exit()
{
}

void FRunnablePrimeCalculator::Calculate(int32 Start, int32 Range)
{
	FRunnable* Runnable = new FRunnablePrimeCalculator(Start, Range);
	FRunnableThread::Create(Runnable, TEXT("PrimeRunnableThread"));
}
