@ECHO OFF

CD ..
CD ..

SET sourcePath=%~1
SET destinationLibsPath=%~2
SET destinationDLLsPath=%~3

FOR /R %sourcePath% %%i in (*.lib) DO COPY %%i %destinationLibsPath%
FOR /R %sourcePath% %%i in (*.dll) DO COPY %%i %destinationDLLsPath%

XCOPY /Y %sourcePath%WebFramework\WebFramework.pdb %destinationLibsPath%
