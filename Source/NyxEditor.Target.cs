// Copyright (C) 2023 - Calvin Davidson

using UnrealBuildTool;
using System.Collections.Generic;

public class NyxEditorTarget : TargetRules
{
    public NyxEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange(new string[] { "Nyx" });
    }
}
