param(
    [string]$ExtractPath,
    [string]$SDL2Version
)

# Function to download the file
function DownloadFile {
    param([string]$url, [string]$outputPath)
    
    $webClient = New-Object System.Net.WebClient
    $webClient.DownloadFile($url, $outputPath)
}

# Function to extract the ZIP file
function ExtractZipFile {
    param([string]$zipFilePath, [string]$ExtractPath)
    
    Add-Type -AssemblyName System.IO.Compression.FileSystem
    [System.IO.Compression.ZipFile]::ExtractToDirectory($zipFilePath, $ExtractPath)
}

# Parameters
$SDL2_ARCHIVE_NAME = "SDL2-devel-${SDL2Version}-VC.zip"
$SDL2_URL = "https://github.com/libsdl-org/SDL/releases/download/release-$SDL2Version/$SDL2_ARCHIVE_NAME" 
$SDL2_ZipFile = Join-Path $PSScriptRoot  $SDL2_ARCHIVE_NAME
$SDL2_PATH = Join-Path $ExtractPath "SDL2-$SDL2VERSION"

Write-Host "Removing existing extraction directory..."
# Remove the extraction directory if it exists
if (Test-Path $SDL2_PATH -PathType Container) {
    Write-Host "Removing existing extraction directory..."
    Remove-Item -Recurse -Force $SDL2_PATH
}

# Create the extraction directory if it doesn't exist
if (-Not (Test-Path $ExtractPath -PathType Container)) {
    Write-Host "Creating extraction directory..."
    New-Item -ItemType Directory -Path $ExtractPath
}

# Download SDL2 ZIP file
Write-Host "Downloading SDL2..."
DownloadFile -url $SDL2_URL -outputPath $SDL2_ZipFile
Write-Host "Download complete."


# Extract SDL2 ZIP file
Write-Host "Extracting SDL2..."
ExtractZipFile -zipFilePath $SDL2_ZipFile -ExtractPath $ExtractPath
Write-Host "Extraction complete."

# Clean up the ZIP file
Remove-Item -Path $SDL2_ZipFile -Force
Write-Host "Cleaned up ZIP file."
