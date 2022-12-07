[CmdletBinding(DefaultParameterSetName = "seta")]
param (
    [Parameter(ParameterSetName = "seta")]
    [switch]
    $RemoveDeprecated,

    [Parameter(ParameterSetName = "seta")]
    [switch]
    $RemoveUnused,

    [Parameter(ParameterSetName = "setb")]
    [switch]
    $Reset,

    [switch]
    $Run
)

$ErrorActionPreference = "Stop"
$Z_CONFIG = "Z-PsCoreFxsConfig.json"
$Z_CONFIG_LAST = "Z-PsCoreFxsConfig.Last.json"
$Z_PSCOREFXS = "Z-PsCoreFxs.ps1"
$PsCoreFxs = "https://github.com/Satancito/PsCoreFxs.git"
$X_UPDATE = $PSCommandPath | Split-Path -Leaf

$ZCoreFxsUri = "https://raw.githubusercontent.com/Satancito/PsCoreFxs/main/Z-PsCoreFxs.ps1"
$XUpdatesUri = "https://raw.githubusercontent.com/Satancito/PsCoreFxs/main/X-UPDATE.ps1"
(New-Object System.Net.WebClient).DownloadFile($XUpdatesUri, $X_UPDATE)
if(!$Run.IsPresent)
{
    Write-Host "███ Update - PsCoreFxs Scripts " -ForegroundColor Magenta
    Remove-Item $X_UPDATE -Force -ErrorAction Ignore
    Remove-Item $Z_PSCOREFXS -Force -ErrorAction Ignore
    Write-Host "Downloading $XUpdatesUri"
    #(New-Object System.Net.WebClient).DownloadFile($XUpdatesUri, $X_UPDATE)
    Invoke-WebRequest -Uri "$XUpdatesUri" -OutFile "$X_UPDATE" -Headers @{"Cache-Control"="no-cache"} | Out-Null
    Write-Host "Downloading $ZCoreFxsUri" 
    #(New-Object System.Net.WebClient).DownloadFile($ZCoreFxsUri, $Z_PSCOREFXS)
    Invoke-WebRequest -Uri "$ZCoreFxsUri" -OutFile $Z_PSCOREFXS -Headers @{"Cache-Control"="no-cache"} | Out-Null
    pwsh -Command "./$X_UPDATE -Run $($RemoveDeprecated ? "-RemoveDeprecated" : [string]::Empty) $($RemoveUnused ? "-RemoveUnused" : [string]::Empty)"
    exit
}

Import-Module -Name "$(Get-Item "./Z-PsCoreFxs.ps1")" -Force -NoClobber
$Path = "$X_TEMP_DIR"

#RUN MODE
Set-GitRepository $PsCoreFxs $Path 

if ("$(Get-Location | Split-Path -Leaf)".Equals("$(Get-VariableName $PsCoreFxs)")) {
    Write-Warning -Message "WARNING. Cannot overwrite original directory of scripts."
    Write-InfoMagenta "███ End - Update - PsCoreFxs Scripts " 
    exit
}


# RUN MODE
if ($Reset.IsPresent) {
    Remove-Item $Z_CONFIG -Force -ErrorAction Ignore
    Remove-Item $Z_CONFIG_LAST -Force -ErrorAction Ignore
}

if (!(Test-Path $Z_CONFIG -PathType Leaf)) {
    Copy-Item -Path "$Path/$(Get-VariableName $PsCoreFxs)/$Z_CONFIG" $Z_CONFIG -Force
    Write-PrettyKeyValue "Creating" "$Z_CONFIG"
}

$localJsonObject = Get-JsonObject "./$Z_CONFIG"
$lastJsonObject = Get-JsonObject "$Path/$(Get-VariableName $PsCoreFxs)/$Z_CONFIG"

$localJsonObject.Files = ($null -eq $localJsonObject.Files ? ([System.Array]::Empty[System.String]()) : ([array]$localJsonObject.Files))
[array]$files = ($localJsonObject.Files | Where-Object { ($_ -notin $lastJsonObject.DeprecatedFiles) -and ($_ -in $lastJsonObject.Files) })
$localJsonObject.Files = $files
$localJsonObject.CoreFiles = $lastJsonObject.CoreFiles
Add-Member -MemberType NoteProperty -Name "LastSupportedFiles" -InputObject $localJsonObject -Value $lastJsonObject.Files -Force
#$localJsonObject.LastSupportedFiles = $lastJsonObject.Files
$localJsonObject.Build = $lastJsonObject.Build
$localJsonObject.DeprecatedFiles = $lastJsonObject.DeprecatedFiles

Set-JsonObject $localJsonObject $Z_CONFIG
Write-PrettyKeyValue "Updating" "$Z_CONFIG"

if ($RemoveDeprecated.IsPresent) {
    $localJsonObject.DeprecatedFiles | ForEach-Object {
        if (Test-Path $_ -PathType Leaf) {
            Remove-Item $_ -Force -ErrorAction Ignore
            Write-PrettyKeyValue "Removed deprecated" "$_"
        }
    }
}

if ($RemoveUnused.IsPresent) {
    $lastJsonObject.Files | Where-Object { $_ -notin $localJsonObject.Files } | ForEach-Object {
        if (Test-Path $_ -PathType Leaf) {
            Remove-Item $_ -Force  -ErrorAction Ignore
            Write-PrettyKeyValue "Removed unused" "$_"
        }
    }
}

($localJsonObject.Files + $localJsonObject.CoreFiles) | ForEach-Object {
    $file = "$Path/$(Get-VariableName $PsCoreFxs)/$_"
    if ("$_".Equals($Z_CONFIG)) {
        Copy-Item -Path $file $Z_CONFIG_LAST -Force 
        Write-PrettyKeyValue "Updating" "$Z_CONFIG_LAST"
    }
    else {
        if (Test-Path $file -PathType Leaf) {
            Copy-Item -Path $file -Destination $_ -Force 
            Write-PrettyKeyValue "Updating" "$_"
        }
        else {
            Write-PrettyKeyValue "Ignored" "$_"
        }
    }
}

Write-InfoMagenta "███ End - Update - PsCoreFxs Scripts " 