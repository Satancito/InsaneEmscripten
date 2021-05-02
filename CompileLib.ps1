[CmdletBinding()]
param (
    [Parameter()]
    [switch]
    $Clean
)

$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber

Clear-Host
Write-Host
Write-Host "████ Compiling Insane LLVM Bitcode" -ForegroundColor Blue
Write-Host

if($Clean.IsPresent)
{
    Write-Host "Removing Obj files."
    Remove-Item ./build -Force -Recurse -ErrorAction Ignore
    Remove-Item Insane.bc -Force -Recurse -ErrorAction Ignore
}

emmake make -j8 CXX="$(Select-ValueByPlatform "em++.bat" "em++" "em++")"
Write-Host "█ End Compiling Insane LLVM Bitcode - Finished" -ForegroundColor Blue
Write-Host
