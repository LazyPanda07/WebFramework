@ECHO OFF

CD ..

SET libsFolder=%CD%\bin\ProductionRelease-x64\

CD custom_build_steps
CD utility

CALL Setup.bat

MKDIR ProductionRelease

CD ProductionRelease

MKDIR libs

SET productionReleaseFolder=%CD%\

CD ..
CD ..

CD custom_build_steps
CD utility

CALL CopyHeaders.bat %productionReleaseFolder%

CALL CopyLibs.bat %libsFolder% %productionReleaseFolder%libs

COPY /Y web_framework_settings.ini %productionReleaseFolder%
