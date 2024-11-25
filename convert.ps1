$files = Get-Item SourceTexture/*.png

foreach($f in $files) {
    -join ("Convert Start ... ", $f)
    Start-Process -FilePath x64/Debug/TextureConverter.exe -ArgumentList $f,"-ml","3" -Wait
}

Write-Output -InputObject "End ALL Process"