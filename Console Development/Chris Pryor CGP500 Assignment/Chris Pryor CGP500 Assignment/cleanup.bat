
REM Remove Temp Visual Studio Files
REM *******************************

del	/s 	*.aps
del	/s 	*.clw
del	/s 	*.ncb
del	/s 	*.opt
del	/s 	*.plg
del	/s 	*.pdb
REM del	/s 	*.user
del	/s 	*.vsp
del	/s 	*.sdf
del /s  *.spv
del /s 	/A -H *.opensdf
del	/s 	/A -H *.suo





del	/Q Debug\\*.*
rmdir 	/Q Debug
del   	/Q Release\\*.*
rmdir 	/Q Release

FOR /F "tokens=*" %%G IN ('DIR /B /AD /S *ipch*') DO RMDIR /S /Q "%%G"

FOR /F "tokens=*" %%G IN ('DIR /B /AD /S *Debug*') DO RMDIR /S /Q "%%G"

FOR /F "tokens=*" %%G IN ('DIR /B /AD /S *bin*') DO RMDIR /S /Q "%%G"
