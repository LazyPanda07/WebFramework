@echo off

set WEB_FRAMEWORK_SERVER_CONFIG=%1
set "FXR_PATH="

for /d %%D in ("C:\Program Files\dotnet\host\fxr\10.*") do (
    if defined FXR_PATH (
        set "FXR_PATH=!FXR_PATH!;%%D"
    ) else (
        set "FXR_PATH=%%D"
    )
)

set PATH=%FXR_PATH%;%PATH%

REM RUNTIMES variable contains list of all needed runtimes like this: --runtimes python

echo "Current runtimes: %RUNTIMES%"

python api_test.py || exit 1
call Core.exe --server_config %WEB_FRAMEWORK_SERVER_CONFIG% --run_arguments "python -u server.py" %RUNTIMES% || exit 1
call LoadBalancerCore.exe --port 9090 --load_balancer_run_arguments "python -u load_balancer_server.py" --config load_balancer_config.json %RUNTIMES% || exit 1
call LoadBalancerCore.exe --port 9092 --load_balancer_run_arguments "python -u load_balancer_server.py" --config load_balancer_config_https.json --useHTTPS %RUNTIMES% || exit 1 REM needs to initialize runtimes for https config before using it
call LoadBalancerCore.exe --port 9091 --load_balancer_run_arguments "python -u load_balancer_server.py" --config load_balancer_config.json --serversHTTPS %RUNTIMES% || exit 1
call LoadBalancerCore.exe --port 9093 --load_balancer_run_arguments "python -u load_balancer_server.py" --config load_balancer_config_https.json --serversHTTPS --useHTTPS %RUNTIMES% || exit 1
call LoadBalancerCore.exe --port 9094 --load_balancer_run_arguments "python -u load_balancer_server.py" --config load_balancer_config.json --custom_heuristic %RUNTIMES% || exit 1
call ProxyCore.exe --port 15000 --load_balancer_run_arguments "python -u load_balancer_server.py" --proxy_run_arguments "python -u proxy_server.py" %RUNTIMES% || exit 1
call ProxyCore.exe --port 15001 --load_balancer_run_arguments "python -u load_balancer_server.py" --proxy_run_arguments "python -u proxy_server.py" --useHTTPS %RUNTIMES% || exit 1
