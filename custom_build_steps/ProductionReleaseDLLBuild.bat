@ECHO OFF

CD ..

SET libsFolder=%CD%\bin\ProductionReleaseDLL-x64\

CD custom_build_steps
CD utility

CALL Setup.bat

MKDIR ProductionReleaseDLL

CD ProductionReleaseDLL

MKDIR libs
MKDIR dlls

SET productionReleaseDLLFolder=%CD%\

CD ..
CD ..

CD custom_build_steps
CD utility

CALL CopyHeaders.bat %productionReleaseDLLFolder%

CALL CopyLibs.bat %libsFolder% %productionReleaseDLLFolder%libs %productionReleaseDLLFolder%dlls

COPY /Y web_framework_settings.ini %productionReleaseDLLFolder%
COPY /Y %libsFolder%WebFramework\WebFramework.dll %productionReleaseDLLFolder%\dlls
