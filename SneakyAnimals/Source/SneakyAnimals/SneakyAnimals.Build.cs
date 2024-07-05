// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SneakyAnimals : ModuleRules
{
	public SneakyAnimals(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Slate", "SlateCore", "UMG", "OnlineSubsystem", "OnlineSubsystemSteam", "NetCore", "AIModule", "GameplayTasks" });
	}
}
