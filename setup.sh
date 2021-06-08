#!/bin/bash
export WASI_SDK_PATH="$(pwd)/sdk/wasi-sdk-12.0"
export WASI_SYSROOT="${WASI_SDK_PATH}/share/wasi-sysroot"
export WASI_CC="${WASI_SDK_PATH}/bin/clang-11"
