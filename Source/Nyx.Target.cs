// Copyright (C) 2023 - Calvin Davidson

using UnrealBuildTool;
using System.Collections.Generic;

public class NyxTarget : TargetRules
{
    public NyxTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange(new string[] { "Nyx" });
    }
}
