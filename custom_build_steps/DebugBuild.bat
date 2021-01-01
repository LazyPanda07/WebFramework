@ECHO OFF

CD ..

SET libsFolder=%CD%\bin\Debug-x64\

CD custom_build_steps
CD utility

CALL Setup.bat

MKDIR Debug

CD Debug

MKDIR libs
MKDIR dlls

SET debugFolder=%CD%\

CD ..
CD ..

CD custom_build_steps
CD utility

CALL CopyHeaders.bat %debugFolder%

CALL CopyLibs.bat %libsFolder% %debugFolder%libs %debugFolder%dlls

COPY /Y web_framework_settings.ini %debugFolder%
