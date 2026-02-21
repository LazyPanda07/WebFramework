@echo off

call Core.exe --server_config multi_threaded_redis_config.json --run_redis_tests --run_arguments Server.exe || exit 1
