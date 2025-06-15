#!/bin/bash

set -e

export WEB_FRAMEWORK_SERVER_CONFIG=$1
export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

if [ -z "${ADDITIONAL_ARGS+x}" ]; then
    ADDITIONAL_ARGS=""
fi

${ADDITIONAL_ARGS} ./Server ${WEB_FRAMEWORK_SERVER_CONFIG} &
${ADDITIONAL_ARGS} ./DefaultHTTPSServer &
${ADDITIONAL_ARGS} ./ProxyServer --config proxy_config.json --port 15000 &
${ADDITIONAL_ARGS} ./ProxyServer --config proxy_config.json --port 15001 --useHTTPS &
${ADDITIONAL_ARGS} ./LoadBalancerServer --config load_balancer_config.json --port 9090 &
${ADDITIONAL_ARGS} ./LoadBalancerServer --config load_balancer_config.json --port 9091 --serversHTTPS &
${ADDITIONAL_ARGS} ./LoadBalancerServer --config load_balancer_config_https.json --port 9092 &
${ADDITIONAL_ARGS} ./LoadBalancerServer --config load_balancer_config_https.json --port 9093 --serversHTTPS &
${ADDITIONAL_ARGS} ./LoadBalancerServer --config load_balancer_config.json --port 9094 --custom_heuristic &

${ADDITIONAL_ARGS} ./LoadBalancerServer --config load_balancer_config.json --type server --port 10000 &
${ADDITIONAL_ARGS} ./LoadBalancerServer --config load_balancer_config.json --type server --port 10001 --serversHTTPS &
${ADDITIONAL_ARGS} ./LoadBalancerServer --config load_balancer_config_https.json  --type server --port 10002 &
${ADDITIONAL_ARGS} ./LoadBalancerServer --config load_balancer_config_https.json --type server --port 10003 --serversHTTPS &
sleep 1

${ADDITIONAL_ARGS} ./Core ${WEB_FRAMEWORK_SERVER_CONFIG}
${ADDITIONAL_ARGS} ./LoadBalancerCore --port 9090
${ADDITIONAL_ARGS} ./LoadBalancerCore --port 9091
${ADDITIONAL_ARGS} ./LoadBalancerCore --port 9092 --useHTTPS
${ADDITIONAL_ARGS} ./LoadBalancerCore --port 9093 --useHTTPS
${ADDITIONAL_ARGS} ./LoadBalancerCore --port 9094 --custom_heuristic
${ADDITIONAL_ARGS} ./ProxyCore --port 15000
${ADDITIONAL_ARGS} ./ProxyCore --port 15001 --useHTTPS
