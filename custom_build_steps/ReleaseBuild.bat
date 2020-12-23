@ECHO OFF

CD ..

SET libsFolder=%CD%\bin\Release-x64\

CD custom_build_steps
CD utility

CALL Setup.bat

MKDIR Release

CD Release

MKDIR libs

SET releaseFolder=%CD%\

CD ..
CD ..

CD custom_build_steps
CD utility

CALL CopyHeaders.bat %releaseFolder%

CALL CopyLibs.bat %libsFolder% %releaseFolder%libs

COPY /Y web_framework_settings.ini %releaseFolder%
