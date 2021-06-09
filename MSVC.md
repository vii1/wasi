# How to use Wasmer (+WASI) from MSVC

1. [Download Wasmer](https://github.com/wasmerio/wasmer/releases) version 2.0+ and install it.
   * If you use the installer, the env var `WASMER_DIR` will be created automatically. If not, you have to set it up yourself.

2. Set up your project in MSVC:
   1. Add to include search path: `$(WASMER_DIR)\include`
   2. Add to library search path: `$(WASMER_DIR)\lib`
   3. Add to *Linker &rarr; Input &rarr; Additional Dependencies*: `wasmer.lib; ws2_32.lib; bcrypt.lib; userenv.lib`
   4. To remove a warning, add to *Linker &rarr; Input &rarr; Ignore Specific Default Libraries*: `libcmt`

That's it.