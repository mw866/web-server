#!/usr/bin/env bash

OUTPUT="$(ps -ef | grep gdbserver | awk '/echo_server/ { print $2}')"
echo "gdbserver PID: $OUTPUT"
kill -9 "$OUTPUT"
OUTPUT="$(ps -aux | awk '/echo_server/' | awk 'NR==1{print $2}')"
echo "the running program $OUTPUT"
kill -9 "$OUTPUT"
sleep 5
make clean
make
gcc -g -o select_server select_server.c
gdbserver 127.0.0.1:7777 ./select_server &