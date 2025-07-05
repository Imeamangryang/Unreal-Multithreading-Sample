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
				return false; // Found a divisor, not prime
			}
		}
		return true;
	}
};