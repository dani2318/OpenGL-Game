"""
# lint.ps1
$ErrorActionPreference = "Continue"

$files = Get-ChildItem -Path "src","include" -Recurse -Include *.cpp,*.h,*.hpp

Write-Host "Found $($files.Count) files to check`n" -ForegroundColor Yellow

foreach ($file in $files) {
    Write-Host "Checking: $($file.FullName)" -ForegroundColor Cyan

    clang-tidy -fix-errors $file.FullName -- `
        -Ivendor/glad/include `
        -Ivendor/glfw/include `
        -Ivendor/glm/include `
        -Ivendor/stb/include `
        -Iinclude `
        -std=c++23

    if ($LASTEXITCODE -ne 0) {
        Write-Host "  Warning: Issues found in $($file.Name)" -ForegroundColor Yellow
    }
}

Write-Host "`nClang-tidy complete!" -ForegroundColor Green
"""

clang_tidy_flags = [
    "-Ivendor/glad/include",
    "-Ivendor/glfw/include",
    "-Ivendor/glm/include",
    "-Ivendor/stb/include",
    "-Iinclude",
    "-std=c++23"
]

from utils.file_search import file_search
import subprocess

files = file_search()

for file in files:
    print(f"Running clang-tidy on {file}")
    subprocess.run(["clang-tidy", "-fix-errors", str(file), "--", *clang_tidy_flags])
