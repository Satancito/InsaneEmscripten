function Test-Command {
    param (
        [string]$Command,

        [switch]
        $WriteOutput
    )

    try {
        $output = Invoke-Expression -Command $Command 2>&1
        $exitCode = $LASTEXITCODE

        if ($exitCode -eq 0) {
            if ($WriteOutput.IsPresent) {
                Write-Output "✅ Command: ""$Command"" $([Environment]::NewLine)"
                Write-Output "Output: $([Environment]::NewLine) $output"
                return
            }
            return $true
        }
        throw
    }
    catch {
        if ($WriteOutput.IsPresent) {
            Write-Output "❌ Command: ""$Command"""
            return
        }
        return $false
    }
}


$a = Test-Command -Command "java --version"
$b = Test-Command -Command "javax --version" 
$x = Test-Command -Command "java --version" -WriteOutput
$y = Test-Command -Command "javax --version" -WriteOutput


Write-Host "$a"
Write-Host "$b"
Write-Host "$x"
Write-Host "$y"