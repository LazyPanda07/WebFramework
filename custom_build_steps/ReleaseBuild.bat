@ECHO OFF

CD ..

SET libsFolder=%CD%\bin\Release-x64\

CD custom_build_steps
CD utility

CALL Setup.bat

MKDIR Release

CD Release

MKDIR libs
MKDIR dlls

SET releaseFolder=%CD%\

CD ..
CD ..

CD custom_build_steps
CD utility

CALL CopyHeaders.bat %releaseFolder%

CALL CopyLibs.bat %libsFolder% %releaseFolder%libs %releaseFolder%dlls

COPY /Y web_framework_settings.ini %releaseFolder%
COPY /Y load_balancer_settings.ini %releaseFolder%
