@ECHO OFF

CD ..

SET libsFolder=%CD%\bin\ProductionRelease-x64\

CD custom_build_steps
CD utility

CALL Setup.bat

MKDIR ProductionRelease

CD ProductionRelease

MKDIR libs
MKDIR dlls

SET productionReleaseFolder=%CD%\

CD ..
CD ..

CD custom_build_steps
CD utility

CALL CopyHeaders.bat %productionReleaseFolder%

CALL CopyLibs.bat %libsFolder% %productionReleaseFolder%libs %productionReleaseFolder%dlls

COPY /Y web_framework_settings.ini %productionReleaseFolder%
COPY /Y load_balancer_settings.ini %productionReleaseFolder%
