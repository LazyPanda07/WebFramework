#!/bin/bash

set -e

export WEB_FRAMEWORK_SERVER_CONFIG=$1
export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

if [ -z "${PREFIX_ARGS}" ]; then
    PREFIX_ARGS=./
fi

${PREFIX_ARGS}Server ${WEB_FRAMEWORK_SERVER_CONFIG} &
${PREFIX_ARGS}DefaultHTTPSServer &
${PREFIX_ARGS}ProxyServer --config proxy_config.json --port 15000 &
${PREFIX_ARGS}ProxyServer --config proxy_config.json --port 15001 --useHTTPS &
${PREFIX_ARGS}LoadBalancerServer --config load_balancer_config.json --port 9090 &
${PREFIX_ARGS}LoadBalancerServer --config load_balancer_config.json --port 9091 --serversHTTPS &
${PREFIX_ARGS}LoadBalancerServer --config load_balancer_config_https.json --port 9092 &
${PREFIX_ARGS}LoadBalancerServer --config load_balancer_config_https.json --port 9093 --serversHTTPS &
${PREFIX_ARGS}LoadBalancerServer --config load_balancer_config.json --port 9094 --custom_heuristic &

${PREFIX_ARGS}LoadBalancerServer --config load_balancer_config.json --type server --port 10000 &
${PREFIX_ARGS}LoadBalancerServer --config load_balancer_config.json --type server --port 10001 --serversHTTPS &
${PREFIX_ARGS}LoadBalancerServer --config load_balancer_config_https.json  --type server --port 10002 &
${PREFIX_ARGS}LoadBalancerServer --config load_balancer_config_https.json --type server --port 10003 --serversHTTPS &
sleep 1

${PREFIX_ARGS}Core ${WEB_FRAMEWORK_SERVER_CONFIG}
${PREFIX_ARGS}LoadBalancerCore --port 9090
${PREFIX_ARGS}LoadBalancerCore --port 9091
${PREFIX_ARGS}LoadBalancerCore --port 9092 --useHTTPS
${PREFIX_ARGS}LoadBalancerCore --port 9093 --useHTTPS
${PREFIX_ARGS}LoadBalancerCore --port 9094 --custom_heuristic
${PREFIX_ARGS}ProxyCore --port 15000
${PREFIX_ARGS}ProxyCore --port 15001 --useHTTPS
