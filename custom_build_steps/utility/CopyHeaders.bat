@ECHO OFF

CD ..
CD ..

SET destinationPath=%~1

MKDIR %destinationPath%WebFrameworkAssets

XCOPY /Y /S WebFramework\WebFrameworkAssets %destinationPath%WebFrameworkAssets\
XCOPY /Y /S WebFramework\src\*.h %destinationPath%

XCOPY /Y /S BaseTCPServer\src\*.h %destinationPath%\WebNetwork
XCOPY /Y /S HTTP\src\*.h %destinationPath%\WebNetwork
XCOPY /Y /S HTTP\JSON\src\*.h %destinationPath%\WebNetwork
XCOPY /Y /S SocketStreams\src\*.h %destinationPath%\WebNetwork
XCOPY /Y /S SHA256\src\*.h %destinationPath%\SQLite3

CD custom_build_steps
CD utility
