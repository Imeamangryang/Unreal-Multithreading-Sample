// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE5MultiThreadSample : ModuleRules
{
	public UE5MultiThreadSample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
