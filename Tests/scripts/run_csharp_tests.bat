@echo off

set WEB_FRAMEWORK_SERVER_CONFIG=%1

set PATH=C:\Program Files\dotnet\host\fxr\10.0.2\;%PATH%

REM RUNTIMES variable contains list of all needed runtimes like this: --runtime python

echo "Current runtimes: %RUNTIMES%"

call dotnet test CSharpAPI.dll || exit 1
call Core.exe --server_config %WEB_FRAMEWORK_SERVER_CONFIG% --run_arguments "dotnet CSharpServer.dll" %RUNTIMES% || exit 1
call LoadBalancerCore.exe --port 9090 --load_balancer_run_arguments "dotnet CSharpLoadBalancerServer.dll" --config load_balancer_config.json %RUNTIMES% || exit 1
call LoadBalancerCore.exe --port 9092 --load_balancer_run_arguments "dotnet CSharpLoadBalancerServer.dll" --config load_balancer_config_https.json --useHTTPS %RUNTIMES% || exit 1 REM needs to initialize runtimes for https config before using it
call LoadBalancerCore.exe --port 9091 --load_balancer_run_arguments "dotnet CSharpLoadBalancerServer.dll" --config load_balancer_config.json --serversHTTPS %RUNTIMES% || exit 1
call LoadBalancerCore.exe --port 9093 --load_balancer_run_arguments "dotnet CSharpLoadBalancerServer.dll" --config load_balancer_config_https.json --serversHTTPS --useHTTPS %RUNTIMES% || exit 1
call LoadBalancerCore.exe --port 9094 --load_balancer_run_arguments "dotnet CSharpLoadBalancerServer.dll" --config load_balancer_config.json --custom_heuristic %RUNTIMES% || exit 1
call ProxyCore.exe --port 15000 --load_balancer_run_arguments "dotnet CSharpLoadBalancerServer.dll" --proxy_run_arguments "dotnet CSharpProxyServer" %RUNTIMES% || exit 1
call ProxyCore.exe --port 15001 --load_balancer_run_arguments "dotnet CSharpLoadBalancerServer.dll" --proxy_run_arguments "dotnet CSharpProxyServer" --useHTTPS %RUNTIMES% || exit 1
