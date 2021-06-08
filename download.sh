#!/bin/bash
set -e
mkdir -p sdk
wget -P sdk https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/libclang_rt.builtins-wasm32-wasi-12.0.tar.gz 
wget -P sdk https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/wasi-sdk-12.0-linux.tar.gz 
wget -P sdk https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/wasi-sysroot-12.0.tar.gz 
cd sdk
tar xvf wasi-sdk-12.0-linux.tar.gz
cd ..
