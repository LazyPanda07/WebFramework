#!/bin/bash

set -e

export WEB_FRAMEWORK_SERVER_CONFIG=$1
export LD_LIBRARY_PATH=$(pwd):/usr/share/dotnet/host/fxr/10.0.2:${LD_LIBRARY_PATH}

chmod +x ./Core
chmod +x ./LoadBalancerCore
chmod +x ./ProxyCore
chmod +x ./DefaultHTTPSServer

# RUNTIMES variable contains list of all needed runtimes like this: --runtime python

echo "Current runtimes: ${RUNTIMES}"

dotnet test CSharpAPI.dll
./Core --server_config ${WEB_FRAMEWORK_SERVER_CONFIG} --run_arguments "dotnet CSharpServer.dll" ${RUNTIMES}
./LoadBalancerCore --port 9090 --load_balancer_run_arguments "dotnet CSharpLoadBalancerServer.dll" --config load_balancer_config.json ${RUNTIMES}
./LoadBalancerCore --port 9092 --load_balancer_run_arguments "dotnet CSharpLoadBalancerServer.dll" --config load_balancer_config_https.json --useHTTPS ${RUNTIMES} # needs to initialize runtimes for https config before using it
./LoadBalancerCore --port 9091 --load_balancer_run_arguments "dotnet CSharpLoadBalancerServer.dll" --config load_balancer_config.json --serversHTTPS ${RUNTIMES}
./LoadBalancerCore --port 9093 --load_balancer_run_arguments "dotnet CSharpLoadBalancerServer.dll" --config load_balancer_config_https.json --serversHTTPS --useHTTPS ${RUNTIMES}
./LoadBalancerCore --port 9094 --load_balancer_run_arguments "dotnet CSharpLoadBalancerServer.dll" --config load_balancer_config.json --custom_heuristic ${RUNTIMES}
./ProxyCore --port 15000 --load_balancer_run_arguments "dotnet CSharpLoadBalancerServer.dll" --proxy_run_arguments "dotnet CSharpProxyServer.dll" ${RUNTIMES}
./ProxyCore --port 15001 --load_balancer_run_arguments "dotnet CSharpLoadBalancerServer.dll" --proxy_run_arguments "dotnet CSharpProxyServer.dll" --useHTTPS ${RUNTIMES}
