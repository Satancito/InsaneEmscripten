$XCoreValues = Get-Item "./Z-CoreValues*.ps1"
if (-not (Test-Path $XCoreValues -PathType Leaf)) {
    Write-InfoYellow "File `"$($PSScriptRoot)/Z-CoreValues*.ps1`" not found."
}
    
Import-Module -Name "$XCoreValues" -Force -NoClobber

function Write-TextColor {
    Param(
        [parameter(Position = 0, ValueFromPipeline = $true)]
        [Object]
        $Info,

        [parameter(Position = 1, ValueFromPipeline = $true)]
        [System.ConsoleColor]
        $ForegroundColor = [System.ConsoleColor]::White,
    
        [parameter(Position = 2, ValueFromPipeline = $true)]
        [Switch]
        $NoNewLine
    )

    Process {
        foreach ($value in $Info) {
            if ($NoNewLine) {
                Write-Host $value -ForegroundColor $ForegroundColor -NoNewline
            }
            else {
                Write-Host $value -ForegroundColor $ForegroundColor
            }
        }            
    }
}

function Write-PrettyKeyValue {
    [CmdletBinding()]
    Param(
        [parameter(Position = 0, ValueFromPipeline = $true)]
        [Object]
        $Key,

        [parameter(Position = 1, ValueFromPipeline = $true)]
        [Object]
        $Value,

        [parameter(Position = 2, ValueFromPipeline = $true)]
        [System.ConsoleColor]
        $LabelForegroudColor = [System.ConsoleColor]::Magenta,

        [parameter(Position = 3, ValueFromPipeline = $true)]
        [System.ConsoleColor]
        $InfoForegroundColor = [System.ConsoleColor]::White,

        [parameter(Position = 4, ValueFromPipeline = $true, Mandatory = $false)]
        [Switch]
        $NoNewLine,

        [Parameter(Position = 5, Mandatory = $false)]
        [System.Management.Automation.SwitchParameter]
        $IsDebug
    )
    
    Process {
        if ($IsDebug) {
            Write-InfoYellow "DEBUG: " -NoNewLine
        }
        Write-TextColor "$($Key): " $LabelForegroudColor -NoNewLine
        foreach ($value in $Value) {
            Write-TextColor $value $InfoForegroundColor -NoNewline:$NoNewLine
        }
    }
}

function Write-InfoRed {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $false)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information Red $NoNewLine
    }
}

function Write-InfoDarkRed {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $false)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information DarkRed $NoNewLine
    }
}

function Write-InfoYellow {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $true)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information Yellow $NoNewLine
    }
}

function Write-InfoDarkYellow {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $true)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information DarkYellow $NoNewLine
    }
}

function Write-InfoGray {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $true)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information Gray $NoNewLine
    }
}

function Write-InfoDarkGray {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $true)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information DarkGray $NoNewLine
    }
}

function Write-InfoGreen {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $false)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information Green $NoNewLine
    }
}

function Write-InfoDarkGreen {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $true)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information DarkGreen $NoNewLine
    }
}

function Write-InfoMagenta {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $true)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information Magenta $NoNewLine
    }
}

function Write-DarkMagenta {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $true)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information DarkMagenta $NoNewLine
    }
}

function Write-InfoWhite {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $true)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information White $NoNewLine
    }
}

function Write-InfoBlue {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $true)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information Blue $NoNewLine
    }
}

function Write-InfoDarkBlue {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $true)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information DarkBlue $NoNewLine
    }
}

function Write-InfoCyan {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $true)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information Cyan $NoNewLine
    }
}

function Write-InfoDarkCyan {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $true)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information Cyan $NoNewLine
    }
}

function Write-InfoBlack {
    Param(
        [parameter(Mandatory = $true, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [Object]
        $Information,
    
        [parameter(Position = 1, ValueFromPipeline = $true)]
        [Switch]
        $NoNewLine
    )

    Process {
        Write-TextColor $Information Black $NoNewLine
    }
}

function Set-GlobalConstant {
    param (
        [Parameter(Mandatory = $True, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [System.String]
        $Name,

        [Parameter(Mandatory = $True, Position = 1, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [System.String]
        $Value
    )
    Process {
        if (!(Get-Variable "$Name"  -ErrorAction 'Ignore')) {
            Set-Variable -Name "$Name" -Option Constant -Value "$Value" -Scope Global
        }
    }
}

function Set-GlobalVariable {
    param (
        [Parameter(Mandatory = $True, Position = 0, ValueFromPipeline = $true)]
        [ValidateNotNullOrEmpty()]
        [System.String]
        $Name,

        [Parameter(Mandatory = $false, Position = 1, ValueFromPipeline = $true)]
        [System.String]
        $Value = ""
    )
    Process {
        Set-Variable -Name "$Name" -Value "$Value" -Scope Global
    }
}

function Test-OnlyWindows {
    param (
    )
    process {
        if (!$IsWindows) {
            Write-Error "Windows Operating system is required for run this function."
            exit
        }
    }
}

function Test-OnlyLinux {
    param (
    )
    process {
        if (!$IsLinux) {
            Write-Error "Linux Operating system is required for run this function."
            exit
        }
    }
}

function Test-OnlyMacOS {
    param (
    )
    process {
        if (!$IsMacOS) {
            Write-Error "MacOS Operating system is required for run this function."
            exit
        }
    }
}

function Stop-WhenIsDbProviderName {
    param (
        [Parameter(Mandatory = $true, Position = 0)]
        [System.String]
        $Value
    )
    switch ($Value) {
        { $_ -in "SqlServer", "PostgreSql", "MySql", "All" } {
            Write-Error "Value cannot be a db Provider"
            exit
        }
        default {
            return;
        }       
    }
}

function Install-EfCoreTools {
    param (
        
    )
    Write-Host "█ Installing Entity Framework Core Tools" -ForegroundColor Magenta
    dotnet tool install --global dotnet-ef
    dotnet tool update --global dotnet-ef
    dotnet-ef   
}
function Add-Migration {
    param (
        [Parameter(Mandatory = $true)]
        [System.String]
        $Name,

        [Parameter(Mandatory = $true)]
        [System.String]
        [ValidateSet("SqlServer", "PostgreSql", "MySql", "All")]
        $Provider,

        [Parameter(Mandatory = $true)]
        [System.String]
        $Project,

        [Parameter(Mandatory = $true)]
        [System.String]
        $StartupProject,

        [Parameter(Mandatory = $false)]
        [System.String]
        $Context = ""
    )
    switch ($Provider) {
        ("SqlServer") {  
            $Context = "$($Context)SqlServerDbContext"
            $outputDir = "Migrations/SqlServer"
        }

        ("PostgreSql") {  
            $Context = "$($Context)PostgreSqlDbContext"
            $outputDir = "Migrations/PostgreSql"
        }

        ("MySql") {  
            $Context = "$($Context)MySqlDbContext"
            $outputDir = "Migrations/MySql"
        }

        ("All") {
            Add-Migration -Name $Name -Provider "SqlServer" -Project $project -StartupProject $startupProject -Context $Context
            Add-Migration -Name $Name -Provider "PostgreSql" -Project $project -StartupProject $startupProject -Context $Context
            Add-Migration -Name $Name -Provider "MySql" -Project $project -StartupProject $startupProject -Context $Context
            return
        } 

        Default {
            Write-Error "Invalid Provider"
            exit
        }

    }
    Write-Host "█ Add Migration - $context" -ForegroundColor Magenta
    dotnet-ef migrations add "Migration_$($context)_$Name" --startup-project "$StartupProject" --project "$Project" --context "$context" --output-dir "$outputDir" --verbose
}

function Remove-Migration {
    param ( 
        [Parameter(Mandatory = $false, Position = 0)]
        [ValidateSet("SqlServer", "PostgreSql", "MySql", "All")]
        [System.String]
        $Provider = "All",

        [Parameter(Mandatory = $true)]
        [System.String]
        $Project,
        
        [Parameter(Mandatory = $true)]
        [System.String]
        $StartupProject,

        [Parameter(Mandatory = $false)]
        [System.String]
        $Context = ""
    )

    switch ($Provider) {
        ("SqlServer") {  
            $Context = "$($Context)SqlServerDbContext"
        }

        ("PostgreSql") {  
            $Context = "$($Context)PostgreSqlDbContext"
        }

        ("MySql") {  
            $Context = "$($Context)MySqlDbContext"
        }

        ("All") {
            Remove-Migration -Provider "SqlServer" -Project $Project -StartupProject $StartupProject -Context $context
            Remove-Migration -Provider "PostgreSql" -Project $Project -StartupProject $StartupProject -Context $Context
            Remove-Migration -Provider "MySql" -Project $Project -StartupProject $StartupProject -Context $Context
            return
        } 

        Default {
            Write-Error "Invalid Provider"
            exit
        }
    }
    Write-Host "█ Remove Migration - $context" -ForegroundColor Magenta
    if ($Force.IsPresent) {
        #Con el parámetro --force Elimina la migración desde código y desde la base de datos.
        dotnet ef migrations remove --startup-project "$startupProject" --project "$project" --context "$context" --verbose --force
    }
    else {
        dotnet ef migrations remove --startup-project "$startupProject" --project "$project" --context "$context" --verbose
    }
    
}

function Remove-Database {
    param (
        [System.String]
        [ValidateSet("SqlServer", "PostgreSql", "MySql", "All")]
        $Provider = "All",
        [Parameter(Mandatory = $true)]
        [System.String]
        $Project,
        [Parameter(Mandatory = $true)]
        [System.String]
        $StartupProject,

        [Parameter(Mandatory = $false)]
        [System.String]
        $Context = ""
    )
    switch ($Provider) {
        ("SqlServer") {  
            $Context = "$($Context)SqlServerDbContext"
        }

        ("PostgreSql") {  
            $Context = "$($Context)PostgreSqlDbContext"
        }

        ("MySql") {  
            $Context = "$($Context)MySqlDbContext"
        }

        ("All") {
            Remove-Database -Provider "SqlServer" -Project $project -StartupProject $startupProject -Context $Context
            Remove-Database -Provider "PostgreSql" -Project $project -StartupProject $startupProject -Context $Context
            Remove-Database -Provider "MySql" -Project $project -StartupProject $startupProject -Context $Context
            return;
        } 

        Default {
            Write-Error "Invalid Provider"
            exit
        }

    }
    Write-Host "█ Remove Database - $context" -ForegroundColor Magenta
    dotnet-ef database drop --startup-project "$startupProject" --context "$context" --project "$project" --force --verbose
}

function Update-Database {
    param (
        [System.String]
        [ValidateSet("SqlServer", "PostgreSql", "MySql", "All")]
        $Provider = "All",
        [Parameter(Mandatory = $true)]
        [System.String]
        $Project,
        [Parameter(Mandatory = $true)]
        [System.String]
        $StartupProject,
        [Parameter(Mandatory = $false)]
        [System.String]
        $Context = ""
    )
    switch ($Provider) {
        ("SqlServer") {  
            $Context = "$($Context)SqlServerDbContext"
        }

        ("PostgreSql") {  
            $Context = "$($Context)PostgreSqlDbContext"
        }

        ("MySql") {  
            $Context = "$($Context)MySqlDbContext" 
        }

        ("All") {
            Update-Database -Provider "SqlServer" -Project $Project -StartupProject $StartupProject -Context $Context
            Update-Database -Provider "PostgreSql" -Project $Project -StartupProject $StartupProject -Context $Context
            Update-Database -Provider "MySql" -Project $Project -StartupProject $StartupProject -Context $Context
            return
        } 

        Default {
            Write-Error "Invalid Provider"
            exit
        }
    }
    Write-Host "█ Update database - $context" -ForegroundColor Magenta
    dotnet-ef database update --startup-project "$StartupProject" --context "$context" --project "$Project" --verbose
}

function New-MigrationScript {
    param (
        [System.String]
        [ValidateSet("SqlServer", "PostgreSql", "MySql", "All")]
        $Provider = "All",
        [Parameter(Mandatory = $true)]
        [System.String]
        $Project,
        [Parameter(Mandatory = $true)]
        [System.String]
        $StartupProject,
        [Parameter(Mandatory = $false)]
        [System.String]
        $Context = ""
    )
    switch ($Provider) {
        ("SqlServer") {  
            $Context = "$($Context)SqlServerDbContext"
            $outputFile = "$Project/SqlScripts/SqlServer/$($context)_$([DateTime]::Now.ToString("yyyyMMddHHmmssfff")).sql"
        }

        ("PostgreSql") {  
            $Context = "$($Context)PostgreSqlDbContext"
            $outputFile = "$Project/SqlScripts/PostgreSql/$($context)_$([DateTime]::Now.ToString("yyyyMMddHHmmssfff")).sql"
        }

        ("MySql") {  
            $Context = "$($Context)MySqlDbContext"
            $outputFile = "$Project/SqlScripts/MySql/$($context)_$([DateTime]::Now.ToString("yyyyMMddHHmmssfff")).sql"
        }

        ("All") {
            New-MigrationScript -Provider "SqlServer" -Project $project -StartupProject $startupProject -Context $Context
            New-MigrationScript -Provider "PostgreSql" -Project $project -StartupProject $startupProject -Context $Context
            New-MigrationScript -Provider "MySql" -Project $project -StartupProject $startupProject -Context $Context
            return
        }

        Default {
            Write-Error "Invalid Provider"
            exit
        }

    }
    Write-Host "█ Creating Sql Script - $context" -ForegroundColor Magenta
    dotnet ef migrations script --output "$outputFile" --context "$context" --project "$project" --startup-project "$startupProject" --verbose --idempotent
}

function Update-Version {
    param (
        [System.String]
        $ProjectFileName,

        [System.String]
        [ValidateSet("Major", "Minor", "Build", "Revision")]
        $ValueType
    )

    if (!(Test-Path $ProjectFileName -PathType Leaf) -and $ProjectFileName.EndsWith(".csproj")) {
        Write-Error "Invalid file `"$ProjectFileName`"."
    }

    [System.Xml.XmlDocument] $doc = [System.Xml.XmlDocument]::new()
    $doc.Load($ProjectFileName)
    #[System.Xml.XmlElement]
    $version = $doc.DocumentElement.SelectSingleNode("//Project/PropertyGroup/Version") 
    if ($null -eq $version) {
        [System.Xml.XmlElement]$v = $doc.CreateElement("Version")
        $v.InnerText = "1.0.0.0"
        $doc.DocumentElement.SelectSingleNode("//Project/PropertyGroup").AppendChild($v);
        $doc.Save($ProjectFileName)
        return
    }

    if ([String]::IsNullOrWhiteSpace($version.InnerText)) {
        $version.InnerText = "1.0.0.0" 
        $doc.Save($ProjectFileName)
        return
    }


    $avMajor, $avMinor, $avBuild, $avRevision = $version.InnerText.Split(".")
    $avMajor = [System.String]::IsNullOrWhiteSpace($avMajor) ? "0" : $avMajor
    $avMinor = [System.String]::IsNullOrWhiteSpace($avMinor) ? "0" : $avMinor
    $avBuild = [System.String]::IsNullOrWhiteSpace($avBuild) ? "0" : $avBuild
    $avRevision = [System.String]::IsNullOrWhiteSpace($avRevision) ? "0" : $avRevision
    switch ($ValueType) {
        ("Major") {  
            $avMajor = [Convert]::ToInt32($avMajor, 10) + 1;
            $avMinor = 0
            $avBuild = 0
            $avRevision = 0
        }
        ("Minor") {  
            $avMinor = [Convert]::ToInt32($avMinor, 10) + 1;
            $avBuild = 0
            $avRevision = 0
        }
        ("Build") {  
            $avBuild = [Convert]::ToInt32($avBuild, 10) + 1;
            $avRevision = 0
        }
        ("Revision") {  
            $avRevision = [Convert]::ToInt32($avRevision, 10) + 1;
        }
        Default {
            Write-Error "Unknown ValueType `"$ValueType`"."
        }
    }
    $result = "$avMajor.$avMinor.$avBuild.$avRevision"
    $version.InnerText = $result

    $doc.Save($ProjectFileName)

    return $result
}

function Get-NextVersion {
    param (
        [System.String]
        $Version,

        [System.String]
        [ValidateSet("Major", "Minor", "Build", "Revision")]
        $ValueType
    )

    

    $avMajor, $avMinor, $avBuild, $avRevision = $version.Split(".")
    $avMajor = [System.String]::IsNullOrWhiteSpace($avMajor) ? "0" : $avMajor
    $avMinor = [System.String]::IsNullOrWhiteSpace($avMinor) ? "0" : $avMinor
    $avBuild = [System.String]::IsNullOrWhiteSpace($avBuild) ? "0" : $avBuild
    $avRevision = [System.String]::IsNullOrWhiteSpace($avRevision) ? "0" : $avRevision
    switch ($ValueType) {
        ("Major") {  
            $avMajor = [Convert]::ToInt32($avMajor, 10) + 1;
            $avMinor = 0
            $avBuild = 0
            $avRevision = 0
        }
        ("Minor") {  
            $avMinor = [Convert]::ToInt32($avMinor, 10) + 1;
            $avBuild = 0
            $avRevision = 0
        }
        ("Build") {  
            $avBuild = [Convert]::ToInt32($avBuild, 10) + 1;
            $avRevision = 0
        }
        ("Revision") {  
            $avRevision = [Convert]::ToInt32($avRevision, 10) + 1;
        }
        Default {
            Write-Error "Unknown ValueType `"$ValueType`"."
        }
    }

    return "$avMajor.$avMinor.$avBuild.$avRevision"
}

function Read-Key {
    [CmdletBinding()]
    param (
        [Parameter()]
        [String]
        $Prompt = "Press any key to continue",
    
        [Parameter()]
        [bool]
        $Display = $false,

        [Parameter()]
        [bool]
        $Discard = $true
    )
    Write-Host "$Prompt " -NoNewline -ForegroundColor DarkGray
    $key = [Console]::ReadKey(!$Display)
    if (!(($key -eq 13) -or ($key -eq 10))) {
        [Console]::WriteLine()
    }

    if (!$Discard) {
        return $key
    }
}


function Get-VariableName {
    Param(
        [Parameter()]    
        [System.Object]
        $Variable
    )
    $Line = @(Get-PSCallStack)[1].Position.Text
        
    if ($Line -match '(.*)(Get-VariableName)([ ]+)(-Variable[ ]+)*\$(?<varName>([\w]+:)*[\w]*)(.*)') {
        #https://regex101.com/r/Uc6asf/1
        return $Matches['varName'] 
    }
} 
    
function Test-LastExitCode {
    if (($LASTEXITCODE -ne 0) -or (-not $?)) {
        Get-Error
        Write-Host "ERROR: When execute last command. Check and try again. ErrorCode = $($LASTEXITCODE)." -ForegroundColor Red
        exit
    }  
}
    
function Select-ValueByPlatform {
    param (
        [parameter(Mandatory = $true)]
        [System.Object]
        $WindowsValue,
        
        [parameter(Mandatory = $true)]
        [System.Object]
        $LinuxValue,
        
        [parameter(Mandatory = $true)]
        [System.Object]
        $MacOSValue
        
    )
    if ($IsWindows) {
        return $WindowsValue
    }
    if ($IsLinux) {
        return $LinuxValue
    }
    if ($IsMacOS) {
        return $MacOSValue
    }
        
    throw "Invalid Platform."
}
    
function Get-UserHome {
    return "$(Select-ValueByPlatform "$env:USERPROFILE" "$env:HOME" "$env:HOME")";
}

function Set-LocalEnvironmentVariable {
    param (
        [Parameter()]
        [System.String]
        $Name,

        [Parameter()]
        [System.String]
        $Value,

        [Parameter()]
        [Switch]
        $Append
    )
    if($Append.IsPresent)
    {
        if(Test-Path "env:$Name")
        {
            $Value = (Get-Item "env:$Name").Value + $Value
        }
    }
    Set-Item env:$Name -Value "$value" | Out-Null
}

function Set-PersistentEnvironmentVariable {
    param (
        [Parameter()]
        [System.String]
        $Name,
    
        [Parameter()]
        [System.String]
        $Value,
    
        [Parameter()]
        [Switch]
        $Append        
    )
    
    Set-LocalEnvironmentVariable -Name $Name -Value $Value -Append:$Append
    $pattern = "\s*export[ \t]+$Name=[\w]*[ \t]*>[ \t]*\/dev\/null[ \t]*;[ \t]*#[ \t]*$Name\s*"
    if ($Append.IsPresent) {
        $value = (Get-Item "env:$Name").Value
    }
    
    if ($IsWindows) {
        setx "$Name" "$Value" | Out-Null
        return
    }
    if ($IsLinux) {
        $content = Get-Content "~/.bashrc" -Raw
        $content = [System.Text.RegularExpressions.Regex]::Replace($content, $pattern, [String]::Empty);
        $content += [System.Environment]::NewLine + [System.Environment]::NewLine + "export $Name=$Value > /dev/null ;  # $Name"
        Set-Content "~/.bashrc" -Value $content -Force
        return
    }
    if ($IsMacOS) {
        $content = Get-Content "~/.bash_profile" -Raw
        $content = [System.Text.RegularExpressions.Regex]::Replace($content, $pattern, [String]::Empty);
        $content += [System.Environment]::NewLine + [System.Environment]::NewLine + "export $Name=$Value > /dev/null ; # $Name" 
        Set-Content "~/.bash_profile" -Value $content -Force
        return
    }
    throw "Invalid platform."
}