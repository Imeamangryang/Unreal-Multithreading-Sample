# UE5MultiThreadSample

## 프로젝트 소개

UE5MultiThreadSample 프로젝트는 Unreal Engine 5에서 멀티 스레딩을 활용하는 방법을 예시로 보여주는 샘플입니다. 
이 프로젝트는 게임 개발 시 멀티 스레드 환경에서의 효율적인 작업 분배와 성능 향상 기법을 학습하는 데 목적이 있습니다.

- **엔진 버전:** Unreal Engine 5.4.4

| 주요 기능 | 설명 |
| --- | --- |
| TaskGraph | UE5의 TaskGraph 시스템을 활용한 병렬 처리 예제 |
| TaskSystem | 커스텀 태스크 시스템을 통한 작업 분배 및 실행 예제 |
| Runnable | FRunnable 인터페이스를 이용한 스레드 직접 제어 예제 |
| Async | `Async()` 함수를 활용한 비동기 작업 처리 예제 |
| AsyncTask | `AsyncTask`를 이용한 게임 스레드 및 백그라운드 스레드 작업 예제 |
| NonAbandonable | NonAbandonable 태스크 구현 및 사용 예제 |
| Future/Promise | Future와 Promise를 활용한 결과 반환 및 동기화 예제 |

아래에서 `소수 찾기 예제`를 통해 멀티 스레드에 대해 자세히 설명합니다.

## DoWork

멀티 스레드 예제에서 사용된 소수 찾기 코드는 주어진 정수가 소수인지 판별하는 함수(IsPrime)를 제공합니다. 

이 함수는 여러 스레드에서 동시에 호출하여, 대량의 숫자에 대해 소수 여부를 빠르게 판별하는 데 활용할 수 있습니다.

```
#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"


class FPrimeHelper
{
public:
	UFUNCTION(BlueprintCallable, Category = "Prime Helper")
	static bool IsPrime(int32 Number)
	{
		if (Number <= 1) return false;
		for (int32 i = 2; i <= FMath::Sqrt(static_cast<float>(Number)); ++i)
		{
			if (Number % i == 0)
			{
				return false;
			}
		}
		return true;
	}
};
```

## Runnable

```
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
```

## Async

```
// 이 클래스는 Async() 함수를 사용하여 별도의 스레드에서 작업을 실행할 수 있도록 합니다.
// 비동기 작업은 게임의 메인 스레드와 분리되어 실행되므로, CPU 집약적인 작업(예: 소수 계산)을 메인 스레드에서 분리하여
// 게임의 프레임률을 유지하면서 수행할 수 있습니다.

void FAsyncPrimeCalculator::Calculate(int32 Start, int32 Range)  
{  
   double StartTime = FPlatformTime::Seconds();  

   Async(EAsyncExecution::ThreadPool, [Start, Range]()  
   {  
       for (int32 Number = Start; Number < Start + Range; ++Number)  
       {  
           if (FPrimeHelper::IsPrime(Number))  
           {  
               UE_LOG(LogTemp, Log, TEXT("Async - Prime : %d"), Number);  
           }  
       }  
   });  

   double EndTime = FPlatformTime::Seconds();  
   double Elapsed = EndTime - StartTime;  
   UE_LOG(LogTemp, Log, TEXT("Async - Prime 계산 소요 시간: %.6f초"), Elapsed);  


   GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Async - Prime 계산 소요 시간: %.6f초"), Elapsed));  
}
```

## AsyncTask

```
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
```

## NonAbandonableTask

```
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
```

## ParallelFor

```
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
```

## Future/Promise

```
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
```

## TaskGraph

```
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
```

## TaskSystem

```
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
```
