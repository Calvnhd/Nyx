// Copyright (C) 2024 - Calvin Davidson

using UnrealBuildTool;
using System.Collections.Generic;

public class NyxEditorTarget : TargetRules
{
    public NyxEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        ExtraModuleNames.AddRange(new string[] { "Nyx" });
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
    }
}
