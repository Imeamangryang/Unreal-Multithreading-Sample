#include "AsyncTaskPrimeCalculator.h"
#include "Async/IAsyncTask.h"
#include "PrimeHelper.h"

#include "Engine/Engine.h"

// AsyncTask는 언리얼 엔진에서 비동기 작업을 백그라운드 스레드에서 실행할 수 있도록 해주는 함수입니다.
// 주로 무거운 연산이나 게임의 메인 스레드를 블로킹하지 않고 처리해야 하는 작업에 사용됩니다.
// 아래 예제에서는 소수 계산 작업을 AnyBackgroundThreadNormalTask 우선순위의 스레드에서 실행합니다.

void FAsyncTaskPrimeCalculator::Calculate(int32 Start, int32 Range)
{
	double StartTime = FPlatformTime::Seconds();

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [Start, Range]()
		{
			for (int32 Number = Start; Number < Start + Range; ++Number)
			{
				if (FPrimeHelper::IsPrime(Number))
				{
					// Do something with the prime number, e.g., log it or store it
					UE_LOG(LogTemp, Log, TEXT("AsyncTask - Prime : %d"), Number);
				}
			}
			UE_LOG(LogTemp, Log, TEXT("AsyncTask prime Calculation Finished."));
		});

	double EndTime = FPlatformTime::Seconds();
	double Elapsed = EndTime - StartTime;
	UE_LOG(LogTemp, Log, TEXT("AsyncTask - Prime 계산 소요 시간: %.6f초"), Elapsed);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AsyncTask - Prime 계산 소요 시간: %.6f초"), Elapsed));
}
