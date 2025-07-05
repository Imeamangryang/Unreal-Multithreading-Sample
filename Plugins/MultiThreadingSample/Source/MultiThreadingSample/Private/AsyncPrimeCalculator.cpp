#include "AsyncPrimeCalculator.h"  
#include "Async/Async.h"  
#include "PrimeHelper.h"  
#include "Engine/Engine.h"

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
