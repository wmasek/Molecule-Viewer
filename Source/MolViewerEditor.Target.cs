// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MolViewerEditorTarget : TargetRules
{
	public MolViewerEditorTarget( TargetInfo Target) : base(Target)
	{
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        // Override build environment instead of setting it to unique
        bOverrideBuildEnvironment = true;
    }
}
