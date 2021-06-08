CC=$$WASI_SDK_PATH/bin/clang-11
SYSROOT=$$WASI_SYSROOT

main.wasm:
	$(CC) main.c --target=wasm32-wasi -o main.wasm --sysroot=$(SYSROOT)

wasmer-example:
	clang wasmer-example.c -o wasmer-example -v -static -lpthread `wasmer config --libs` `wasmer config --cflags`


