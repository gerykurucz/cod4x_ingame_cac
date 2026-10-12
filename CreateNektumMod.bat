del mod.ff


xcopy mp ..\..\raw\mp /SY
xcopy shock ..\..\raw\shock /SY
xcopy sound ..\..\raw\sound /SY
xcopy ui_mp ..\..\raw\ui_mp /SY
xcopy weapons ..\..\raw\weapons /SY
xcopy xmodel ..\..\raw\xmodel /SY
xcopy images ..\..\raw\images /SY
xcopy maps ..\..\raw\maps /SY
xcopy nektum ..\..\raw\nektum /SY
xcopy localizedstrings ..\..\raw\localizedstrings /SY

copy /Y mod.csv ..\..\zone_source

cd ..\..\bin

linker_pc.exe -language english -compress -cleanup mod

cd ..\mods\nektum

copy ..\..\zone\english\mod.ff



pause
close