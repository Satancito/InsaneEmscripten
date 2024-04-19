[CmdletBinding()]
param (
    [Parameter(Mandatory = $true)]
    [string]
    $ProductName
)
$Error.Clear()
$ErrorActionPreference = "Stop"
Import-Module -Name "$PSScriptRoot/Z-InsaneEm.ps1" -Force -NoClobber

try {
    Write-Host
    Write-InfoBlue "████ Creating Insane Emscripten - New Project: " -NoNewLine
    Write-Host $ProductName
    Write-Host

    $projectName = $ProductName
    $distDir = "$PSScriptRoot/Dist"
    

    Write-Host "Copying files..."

    $destinationDir = "$distDir/$projectName"
    Remove-Item -Path "$destinationDir" -Force -Recurse -ErrorAction Ignore

    $filesToCopy = @(
        @{  Source = "$PSScriptRoot/Docs/*"; Destination = "$destinationDir"; DestinationItemType ="Directory"},
        @{  Source = "$PSScriptRoot/Server/*"; Destination = "$destinationDir/Server"; DestinationItemType ="Directory"},
        @{  Source = "$PSScriptRoot/Assets/*"; Destination = "$destinationDir/Assets"; DestinationItemType ="Directory"},
        @{  Source = "$PSScriptRoot/Src/*"; Destination = "$destinationDir/Source"; DestinationItemType ="Directory"},
        @{  Source = "$PSScriptRoot/Js/*"; Destination = "$destinationDir/Js"; DestinationItemType ="Directory"},
        @{  Source = "$PSScriptRoot/.clang-format"; Destination = "$destinationDir"; DestinationItemType ="Directory"}
        @{  Source = "$PSScriptRoot/T-UpdateSubmodules.ps1"; Destination = "$destinationDir"; DestinationItemType ="Directory"}
    )

    $filesToCopy | ForEach-Object {
        New-Item -Path $_.Destination -ItemType $_.DestinationItemType -Force | Out-Null
        Copy-Item -Path $_.Source -Destination $_.Destination -Force -Recurse    
    }
 
    Write-Host "Configuring product info..."
    $productInfoFilename = "$destinationDir/ProductInfo.json"
    $json = [System.IO.File]::ReadAllText($productInfoFilename)
    $productInfo = ConvertFrom-Json -InputObject $json
    $productInfo.Name = $ProductName
    $productInfo.Version = "0.0.0"
    $productInfo.InsaneVersion = $__INSANEEM_INSANE_VERSION
    [System.IO.File]::WriteAllText("$productInfoFilename", "$(ConvertTo-Json -InputObject $productInfo)", [System.Text.Encoding]::UTF8)

    Write-Host "Generating code..." 
    $indexHtmlFilename = "$destinationDir/index.html"
    $content = [System.IO.File]::ReadAllText("$indexHtmlFilename")

    $pattern = "<!-- _BEGIN_APP_TITLE_ -->[\s\S]*?<!-- _END___APP_TITLE_ -->"
    $replacement = "<title>$projectName - Emscripten - Tests</title>"
    $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)

    $pattern = "\/\*ModuleName\*\/[a-zA-Z_][a-zA-Z0-9_]*\."
    $replacement = "/*ModuleName*/$projectName."
    $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)
    [System.IO.File]::WriteAllText("$indexHtmlFilename", $content, [System.Text.Encoding]::UTF8)
    Write-Host "Configuring git repo..."
    try {
        Push-Location -Path "$destinationDir"
        $null = Test-ExternalCommand "git init" -ThrowOnFailure -NoAssertion
        $null = Test-ExternalCommand "git submodule add --force --name `"$__PSCOREFXS_REPO_NAME`"  `"$__PSCOREFXS_REPO_URL`" `"submodules/PsCoreFxs`"" -ThrowOnFailure -NoAssertion
        $null = Test-ExternalCommand "git submodule update --init --recursive --force" -ThrowOnFailure -NoAssertion
        $null = Test-ExternalCommand "git submodule update --remote --recursive --force" -ThrowOnFailure -NoAssertion   
    }
    catch {
        throw
    }
    finally {
        Pop-Location
    }
    exit
    
}
finally {
    Write-InfoBlue "█ End creating Insane Emscripten - New Project"
    Write-Host
}