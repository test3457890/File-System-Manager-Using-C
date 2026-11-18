# Publish repository docs into the separate GitHub Wiki repository.
# Prerequisite: Create the first "Home" page at the GitHub Wiki web interface.
# Requires Git and an authenticated GitHub account with wiki write permission.
# Run from anywhere: powershell -ExecutionPolicy Bypass -File scripts/publish-wiki.ps1
param(
    [switch]$Push
)
$ErrorActionPreference = "Stop"
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$wikiUrl = "https://github.com/test3457890/File-System-Manager-Using-C.wiki.git"
$temp = Join-Path ([System.IO.Path]::GetTempPath()) ("file-manager-wiki-" + [guid]::NewGuid().ToString("N"))
$mapping = [ordered]@{
    "docs/README.md" = "Home.md"
    "docs/Getting-Started.md" = "Getting-Started.md"
    "docs/Architecture.md" = "Architecture.md"
    "docs/Commands.md" = "Commands.md"
    "docs/Roadmap.md" = "Roadmap.md"
}
try {
    git clone $wikiUrl $temp
    if ($LASTEXITCODE -ne 0) {
        throw "Wiki clone failed. Create the initial Home page on GitHub, authenticate Git, then retry."
    }
    foreach ($source in $mapping.Keys) {
        $from = Join-Path $repoRoot $source
        $to = Join-Path $temp $mapping[$source]
        if (!(Test-Path $from)) { throw "Missing documentation: $source" }
        Copy-Item -LiteralPath $from -Destination $to -Force
    }
    # Wiki links are page titles, not repository-relative paths.
    $home = Join-Path $temp "Home.md"
    $text = Get-Content -LiteralPath $home -Raw
    $text = $text.Replace("(Getting-Started.md)", "(Getting-Started)")
    $text = $text.Replace("(Commands.md)", "(Commands)")
    $text = $text.Replace("(Architecture.md)", "(Architecture)")
    $text = $text.Replace("(Roadmap.md)", "(Roadmap)")
    Set-Content -LiteralPath $home -Value $text -Encoding UTF8
    $sidebar = @'
**File System Manager**
- [[Home]]
- [[Getting Started|Getting-Started]]
- [[Architecture]]
- [[Commands]]
- [[Roadmap]]

[Source code](https://github.com/test3457890/File-System-Manager-Using-C)
'@
    Set-Content -LiteralPath (Join-Path $temp "_Sidebar.md") -Value $sidebar -Encoding UTF8
    git -C $temp add Home.md Getting-Started.md Architecture.md Commands.md Roadmap.md _Sidebar.md
    if ($LASTEXITCODE -ne 0) { throw "Unable to stage Wiki pages." }
    git -C $temp diff --cached --stat
    if (-not $Push) {
        Write-Host "PREVIEW ONLY: No changes were pushed. To publish, rerun with -Push."
        return
    }
    git -C $temp diff --cached --quiet
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Wiki is already up to date."
        return
    }
    git -C $temp commit -m "Publish project documentation to GitHub Wiki"
    if ($LASTEXITCODE -ne 0) { throw "Commit failed. Check your Git author configuration." }
    git -C $temp push origin HEAD
    if ($LASTEXITCODE -ne 0) { throw "Push failed. Check authentication and Wiki permissions." }
    Write-Host "Wiki published: https://github.com/test3457890/File-System-Manager-Using-C/wiki"
}
finally {
    if (Test-Path $temp) { Remove-Item -LiteralPath $temp -Recurse -Force }
}
