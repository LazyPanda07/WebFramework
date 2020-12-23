@ECHO OFF

CD ..
CD ..

set sourcePath=%~1
set destinationPath=%~2

FOR /R %sourcePath% %%i in (*.lib) DO COPY %%i %destinationPath%

XCOPY /Y %sourcePath%*.lib %destinationPath%
XCOPY /Y %sourcePath%WebFramework\WebFramework.pdb %destinationPath%
