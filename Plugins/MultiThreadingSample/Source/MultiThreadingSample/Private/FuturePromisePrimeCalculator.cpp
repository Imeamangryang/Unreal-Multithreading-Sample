#include "FuturePromisePrimeCalculator.h"
#include "Async/Async.h"
#include "PrimeHelper.h"
#include "Templates/UnrealTemplate.h"

#include "Engine/Engine.h" // Ensure this include is present for GEngine  

// Future와 Promise는 비동기 프로그래밍에서 결과값을 안전하게 주고받기 위한 객체입니다.
// - Promise는 미래에 결과값을 제공할 수 있는 객체로, 비동기 작업이 완료되면 값을 설정합니다(SetValue).
// - Future는 Promise에 의해 설정될 값을 나중에 받아올 수 있는 객체로, 비동기 작업의 결과를 기다리거나(Wait), 결과를 가져올 수 있습니다(Get).
// Unreal Engine에서는 TPromise와 TFuture를 통해 멀티스레드 환경에서 안전하게 값을 전달할 수 있습니다.
// 예를 들어, 백그라운드 스레드에서 계산을 수행하고, 그 결과를 메인 스레드에서 받아 처리할 때 사용됩니다.

void FFuturePromisePrimeCalculator::Calculate(int32 Start, int32 Range)
{
	UE_LOG(LogTemp, Warning, TEXT("Prime calculation task started..."));

	double StartTime = FPlatformTime::Seconds();

	DoPrimeCalculationTask(Start, Range)
		.Next([Start, Range](const TArray<int32>& Primes)
			{
				AsyncTask(ENamedThreads::GameThread, [Primes]()
					{
						for (int32 Prime : Primes)
						{
							UE_LOG(LogTemp, Log, TEXT("FuturePromise - Prime : %d"), Prime);
						}
						UE_LOG(LogTemp, Warning, TEXT("Prime calculation task completed."));
					});
			});
	double EndTime = FPlatformTime::Seconds();
	double Elapsed = EndTime - StartTime;
	UE_LOG(LogTemp, Log, TEXT("FuturePromise - Prime 계산 소요 시간: %.6f초"), Elapsed);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("FuturePromise - Prime 계산 소요 시간: %.6f초"), Elapsed));
}

TFuture<TArray<int32>> FFuturePromisePrimeCalculator::DoPrimeCalculationTask(int32 Start, int32 Range)
{
	TSharedRef<TPromise<TArray<int32>>> Promise = MakeShared<TPromise<TArray<int32>>>();
	TFuture<TArray<int32>> Future = Promise->GetFuture();

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [Start, Range, Promise]()
		{
			TArray<int32> Primes;

			for (int32 Number = Start; Number < Start + Range; ++Number)
			{
				if (FPrimeHelper::IsPrime(Number))
				{
					Primes.Add(Number);
				}
			}

			Promise->SetValue(Primes);
		});

	return Future;
}
