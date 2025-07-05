#include "MultiThreadPrimeSubSystem.h"
#include "FuturePromisePrimeCalculator.h"
#include "AsyncPrimeCalculator.h"
#include "AsyncTaskPrimeCalculator.h"
#include "NonAbandonableTaskPrimeCalculator.h"
#include "ParallelForPrimeCalculator.h"
#include "RunnablePrimeCalculator.h"
#include "TaskGraphPrimeCalculator.h"
#include "TaskSystemPrimeCalculator.h"

void UMultiThreadPrimeSubSystem::CalculatePrimes(int32 Start, int32 Range, ETaskType TaskType)  
{  
   switch (TaskType)  
   {  
   case ETaskType::TaskGraph:  
   {  
       FTaskGraphPrimeCalculator Calculator;  
       Calculator.Calculate(Start, Range);  
       break;  
   }  
   case ETaskType::TaskSystem:  
       FTaskSystemPrimeCalculator::Calcalate(Start, Range);  
       break;  
   case ETaskType::Async:  
       FAsyncPrimeCalculator::Calculate(Start, Range);  
       break;  
   case ETaskType::AsyncTask:  
       FAsyncTaskPrimeCalculator::Calculate(Start, Range);  
       break;  
   case ETaskType::NonAbandonableTask:  
       FPrimeTask::Calculate(Start, Range);  
       break;  
   case ETaskType::ParallelFor:  
       FParallelForPrimeCalculator::Calculate(Start, Range);  
       break;  
   case ETaskType::Runnable:  
       FRunnablePrimeCalculator::Calculate(Start, Range);  
       break;  
   case ETaskType::FuturePromise:  
       FFuturePromisePrimeCalculator PrimeCalculator;  
       PrimeCalculator.Calculate(Start, Range);  
       break;  
   default:  
       UE_LOG(LogTemp, Warning, TEXT("Unknown Task Type"));  
       break;  
   }  
}
