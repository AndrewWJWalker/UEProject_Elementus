// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectElementus : ModuleRules
{
    public ProjectElementus(ReadOnlyTargetRules target) : base(target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp17;

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "AIModule",
            "Niagara",
            "ModularGameplay",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "GameFeatures",
            "ModularFeatures_ExtraActions"
        });
    }
}