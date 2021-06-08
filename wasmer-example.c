#include <stdio.h>
#include "wasmer.h"

void print_wasmer_error()
{
	int error_length = wasmer_last_error_length();
	if( error_length <= 0 ) {
		printf( "No error message\n" );
		return;
	}
	char* error_message = malloc( error_length );
	wasmer_last_error_message( error_message, error_length );
	printf( "Error: %s\n", error_message );
	free( error_message );
}

int main( int argc, const char* argv[] )
{
	FILE* file = fopen( "main.wasm", "rb" );
	if( file == NULL ) {
		printf( "> Error loading main.wasm" );
		return 1;
	}
	fseek( file, 0, SEEK_END );
	size_t filelen = ftell( file );
	fseek( file, 0, SEEK_SET );
	wasm_byte_vec_t wasm_bytes;
	wasm_byte_vec_new_uninitialized( &wasm_bytes, filelen );
	fread( wasm_bytes.data, filelen, 1, file );
	fclose( file );

	printf( "Creating the store...\n" );
	wasm_engine_t* engine = wasm_engine_new();
	wasm_store_t*  store = wasm_store_new( engine );

	printf( "Compiling module...\n" );
	wasm_module_t* module = wasm_module_new( store, &wasm_bytes );

	if( !module ) {
		printf( "> Error compiling module!\n" );
		print_wasmer_error();
		return 1;
	}

	wasm_byte_vec_delete( &wasm_bytes );

	// printf("Creating imports...\n");
	// wasm_extern_vec_t import_object = WASM_EMPTY_VEC;

	printf( "Setting up WASI...\n" );
	wasi_config_t* config = wasi_config_new( "main" );
	// TODO: error checking
	// const char* js_string = "function greet(name) { return JSON.stringify('Hello, ' + name); };
	// print(greet('World'));"; wasi_config_arg( config, "--eval" ); wasi_config_arg( config, js_string );
	wasi_config_capture_stdout( config );

	wasi_env_t* wasi_env = wasi_env_new( config );
	if( !wasi_env ) {
		printf( "> Error building WASI env!\n" );
		print_wasmer_error();
		return 1;
	}

	printf( "Instantiating module...\n" );
	wasm_importtype_vec_t import_types;
	wasm_module_imports( module, &import_types );

	wasm_extern_vec_t imports;
	wasm_extern_vec_new_uninitialized( &imports, import_types.size );
	wasm_importtype_vec_delete( &import_types );

	bool get_imports_result = wasi_get_imports( store, module, wasi_env, &imports );

	if( !get_imports_result ) {
		printf( "> Error getting WASI imports!\n" );
		print_wasmer_error();
		return 1;
	}

	wasm_instance_t* instance = wasm_instance_new(store, module, &imports, NULL);

	if( !instance ) {
		printf( "> Error instantiating module!\n" );
		print_wasmer_error();
		return 1;
	}

	printf( "Retrieving exports...\n" );
	wasm_extern_vec_t exports;
	wasm_instance_exports( instance, &exports );

	if( exports.size == 0 ) {
		printf( "> Error accessing exports!\n" );
		print_wasmer_error();
		return 1;
	}

	printf( "Retrieving the `main` function...\n" );
	// wasm_func_t* main_func = wasm_extern_as_func( exports.data[0] );
	wasm_func_t* main_func = wasi_get_start_function(instance);

	if( main_func == NULL ) {
		printf( "> Failed to get the `main` function!\n" );
		print_wasmer_error();
		return 1;
	}

	printf( "Calling `main` function...\n" );
	wasm_val_t	   args_val[2] = { WASM_I32_VAL( 3 ), WASM_I32_VAL( 4 ) };
	wasm_val_t	   results_val[1] = { WASM_INIT_VAL };
	wasm_val_vec_t args = WASM_ARRAY_VEC( args_val );
	wasm_val_vec_t results = WASM_ARRAY_VEC( results_val );

	if( wasm_func_call( main_func, &args, &results ) ) {
		printf( "> Error calling the `main` function!\n" );
		print_wasmer_error();
		return 1;
	}

	wasm_func_delete( main_func );
	wasm_module_delete( module );
	wasm_extern_vec_delete( &exports );
	wasm_instance_delete( instance );
	wasm_store_delete( store );
	wasm_engine_delete( engine );
}
