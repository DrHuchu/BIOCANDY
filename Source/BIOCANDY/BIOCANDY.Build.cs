// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BIOCANDY : ModuleRules
{
	public BIOCANDY(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput","UMG","PhysicsCore", "AIModule", "NavigationSystem"});
	}
}
