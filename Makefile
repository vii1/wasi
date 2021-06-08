CC=$$WASI_SDK_PATH/bin/clang-11
SYSROOT=$$WASI_SYSROOT

.PHONY: all

all: main.wasm wasi-example

main.wasm: main.c
	$(CC) main.c --target=wasm32-wasi -o main.wasm --sysroot=$(SYSROOT)

# wasmer-example: wasmer-example.c
# 	clang wasmer-example.c -o wasmer-example -v -lpthread `wasmer config --libs` `wasmer config --cflags`

wasi-example: wasi-example.c
	clang wasi-example.c -o wasi-example -v -lpthread `wasmer config --libs` `wasmer config --cflags`

