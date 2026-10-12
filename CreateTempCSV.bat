@echo off
setlocal enabledelayedexpansion
set "output=automod.csv"

echo [START] Mod.csv generalasa...
echo // Auto-generated mod.csv > "%output%"
echo. >> "%output%"

:: --- RAWFILES (maps, nektum, ui_mp, stb.) ---
:: Ebben a listában benne van minden mappa, amit rawfile-ként akarunk kezelni
for %%d in (maps nektum ui_mp scripts shocks vision xmodel mp localizedstrings) do (
    if exist "%%d" (
        echo [OK] Mappa feldolgozasa: %%d
        echo // %%d >> "%output%"
        
        pushd "%%d"
        for /r %%f in (*) do (
            set "fullpath=%%f"
            :: Megkeressük a mappa nevét az útvonalban és onnantól vágunk
            set "filename=%%f"
            set "relpath=%%d/!filename:*%%d\=!"
            set "relpath=!relpath:\=/!"
            
            if /i "%%~xf"==".menu" (
                echo menufile,!relpath! >> "..\!output!"
            ) else (
                echo rawfile,!relpath! >> "..\!output!"
            )
        )
        popd
        echo. >> "%output%"
    )
)

:: --- WEAPONS ---
if exist "weapons" (
    echo [OK] Fegyverek feldolgozasa...
    echo // Weapons >> "%output%"
    
    :: Vegigmegyunk a weapons mappaban levo osszes fajlon
    for /f "delims=" %%f in ('dir /b /s "weapons\"') do (
        :: Csak ha nem mappa
        if not exist "%%f\" (
            set "fname=%%~nf"
            echo weapon,mp/!fname! >> "%output%"
        )
    )
    echo. >> "%output%"
)

:: --- IMAGES ---
if exist "images" (
    echo [OK] Kepek feldolgozasa...
    echo // Images >> "%output%"
    pushd "images"
    for /r %%f in (*) do (
        echo image,%%~nf >> "..\!output!"
    )
    popd
)

echo.
echo ---------------------------------------
echo KESZ! A %output% elkeszult.
pause