@ECHO OFF

CD ..

SET libsFolder=%CD%\bin\ReleaseDLL-x64\

CD custom_build_steps
CD utility

CALL Setup.bat

MKDIR ReleaseDLL

CD ReleaseDLL

MKDIR libs
MKDIR dlls

SET releaseDLLFolder=%CD%\

CD ..
CD ..

CD custom_build_steps
CD utility

CALL CopyHeaders.bat %releaseDLLFolder%

CALL CopyLibs.bat %libsFolder% %releaseDLLFolder%libs

COPY /Y web_framework_settings.ini %releaseDLLFolder%
COPY /Y %libsFolder%WebFramework\WebFramework.dll %releaseDLLFolder%\dlls
