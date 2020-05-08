REM Delete Garbage Files
del *.aps
del *.ilk
del *.map
del *.ncb
del *.opt
del *.plg
del *.pbi
del *.pbo
del *.pbt
del *.pdb
del *.tmp
del *.bak
del *.log
del *.user
del *.cfg
del ..\*.ncb

del Debug\*.obj
del Debug\*.sbr
del Debug\*.exe
del Debug\*.ilk
del Debug\*.res
del Debug\*.idb
del Debug\*.pdb
del Debug\*.pch
del Debug\*.bsc
del Debug\*.html
del Debug\*.htm
del Debug\*.dep
del Debug\*.manifest

rmdir Debug

del Release\*.obj
del Release\*.sbr
del Release\*.exe
del Release\*.ilk
del Release\*.res
del Release\*.idb
del Release\*.pdb
del Release\*.pch
del Release\*.bsc
del Release\*.html
del Release\*.htm
del Release\*.dep
del Release\*.manifest

rmdir Release

del ..\debug\*.exe
del ..\debug\*.ilk
del ..\debug\*.pdb

rmdir ..\debug

del ..\release\*.exe
del ..\release\*.ilk
del ..\release\*.pdb

rmdir ..\release