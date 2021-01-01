@ECHO OFF

CD ..

SET libsFolder=%CD%\bin\DebugDLL-x64\

CD custom_build_steps
CD utility

CALL Setup.bat

MKDIR DebugDLL

CD DebugDLL

MKDIR libs
MKDIR dlls

SET debugDLLFolder=%CD%\

CD ..
CD ..

CD custom_build_steps
CD utility

CALL CopyHeaders.bat %debugDLLFolder%

CALL CopyLibs.bat %libsFolder% %debugDLLFolder%libs %debugDLLFolder%dlls

COPY /Y web_framework_settings.ini %debugDLLFolder%
COPY /Y %libsFolder%WebFramework\WebFramework.dll %debugDLLFolder%\dlls
