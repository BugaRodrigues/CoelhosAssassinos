// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CoelhosAssassinos : ModuleRules
{
	public CoelhosAssassinos(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"Paper2D",
			"PaperZD",
            "UMG",
			"AudioMixer",
			"MetasoundEngine",
            "GameplayTags"
        });

		PrivateDependencyModuleNames.AddRange(new string[] 
		{  
			"Slate",
            "SlateCore"
        });
	}
}
