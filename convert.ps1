$files = Get-Item SourceTexture/*.png

foreach($f in $files) {
    Start-Process -FilePath x64/Debug/TextureConverter.exe -ArgumentList $f,"-ml","3" -Wait
}
pause