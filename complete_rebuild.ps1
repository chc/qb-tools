Set-Location -Path test_files
#New-Item -Name "pak_out" -ItemType Directory
Write-Host "test"
unchunk QB.PAB.PS3 QB.PAB

Set-Location -Path pak_out
unpak ../QB.PAK.PS3 ../QB.PAB

$cwd = Get-Location
Get-Location | Select-Object
$cwdLen = $cwd.Path.Length + 1
Write-Host "loc: $cwd - $cwdLen"

Remove-Item Env:QBTOOLS_DBGINFO_PATH

Get-ChildItem -File -Recurse | ForEach-Object {
    # Perform actions on each file
    $file = $_.FullName
    $relativePath = $file.Substring($cwdLen)
    Write-Host "Processing file: $relativePath"
    qdeopt $file "$file.d"
    if (-not $?) {
        Write-Host "qdeopt failled - $file"
        exit 1
    }
    qdump "$file.d" > "$file.q"
    if (-not $?) {
        Write-Host "qdump failled - $file"
        exit 1
    }
    qcomp "$file.q" "$file.c"
    if (-not $?) {
        Write-Host "qcomp failled - $file"
        exit 1
    }
    qopt "$file.c" "$relativePath"
    if (-not $?) {
        Write-Host "qopt failled - $file"
        exit 1
    }

    Remove-Item "$file.d"
    if (-not $?) {
        Write-Host "Remove-Item - $file.d"
        exit 1
    }

    Remove-Item "$file.q"
    if (-not $?) {
        Write-Host "Remove-Item - $file.q"
        exit 1
    }

    Remove-Item "$file.c"
    if (-not $?) {
        Write-Host "Remove-Item - $file.c"
        exit 1
    }

    # Remove-Item "$file"
    # if (-not $?) {
    #     Write-Host "Remove-Item - $file"
    #     exit 1
    # }
    # Move-Item -Path "$($_.FullName).final" -Destination $file
    
}



Set-Location -Path ../..