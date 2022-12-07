param (
    [Parameter()]
    [System.String]
    $InstallDir = [String]::Empty,

    [Parameter()]
    [switch]
    $Clean,

    [Parameter()]
    [switch]
    $Reset
)
    
Import-Module -Name "$(Get-Item "./Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoDarkGreen "████ Installing Emscripten"
Write-Host
Write-Host

& "./X-InsaneEmscripten-SetEmscriptenEnvVars.ps1" -InstallDir ($Reset.IsPresent ? "$(Get-UserHome)" : $InstallDir) 

$EMSDK = "https://github.com/emscripten-core/emsdk.git"
$InstallDir = "$($env:EMSCRIPTEN_SDK | Split-Path)"
$LocationStackName = "emsdk"
try {
    New-Item $InstallDir -ItemType Container -Force | Out-Null
    Push-Location $InstallDir -StackName $LocationStackName
    if ($Clean.IsPresent) {
        Write-InfoYellow "Removing SDK folder!."
        Remove-Item "./emsdk" -Force -Recurse -ErrorAction Ignore
    }
    if (!(Test-Path "./emsdk" -PathType Container)) {
        if(!(Test-Path "./emsdk" -PathType Leaf))
        {
            git clone "$EMSDK"; Test-LastExitCode
        }
        else
        {
            throw "`"$InstallDir/emsdk`" is not a directory"
        }
    }
    Push-Location "./emsdk" -StackName $LocationStackName
    

    git pull; Test-LastExitCode

    $EMSDK_EXE = Select-ValueByPlatform "emsdk.bat" "emsdk" "emsdk"
    $EMSDK_ENV = Select-ValueByPlatform "emsdk_env.bat" "emsdk_env.sh" "emsdk_env.sh"

    & "./$EMSDK_EXE" install latest; Test-LastExitCode
    & "./$EMSDK_EXE" activate latest; Test-LastExitCode
    if($IsLinux -or $IsMacOS)
    {
        chmod +x "./$EMSDK_ENV"
        & sh "./$EMSDK_ENV"; Test-LastExitCode
    }
    else {
        & "./$EMSDK_ENV"; Test-LastExitCode    
    }
}
finally {
    Pop-LocationStack -StackName $LocationStackName
}

Write-Host
Write-InfoDarkGreen "█ End install Emscripten"