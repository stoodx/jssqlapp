//by http://duktape.org/guide.html#gettingstarted

//#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <fstream>

extern "C" {
#include "duktape.h"
}
#include "CDukSyncNativeData.h"

int main(int argc, char* argv[])
{
	int nRes = 0;
	duk_context* ctx = duk_create_heap_default();
	if(!ctx)
	{
		std::cout << "Fail duk_create_heap_default()" << std::endl; 
		return 1;
	}
	if (argc < 4 || !argv[1] || !argv[2] || !argv[3])
	{
		std::cout <<  "Error:" << std::endl << 
			"Nead a pathes to js files" << std::endl;
		std::cout <<  "Usage:" << std::endl << 
			"jssqlapp.exe <path_to_start_js_module> <path_to_sql_js_module> <path_to_sql_database>" << std::endl; 
	}
	else
	{
		stoodx::CDukSyncNativeData* pDukeNative = new stoodx::CDukSyncNativeData(ctx);
		if (pDukeNative == NULL)
		{
			duk_destroy_heap(ctx);
			return 1;
		}
		pDukeNative->InitNative("readFileNative");
		//run js
		if (duk_peval_file(ctx, argv[1]) == 0)
		{
			duk_pop(ctx);
			duk_get_prop_string(ctx, -1, "main");
			duk_push_string(ctx, argv[2]);
			duk_push_string(ctx, argv[3]);
			if (duk_pcall(ctx, 2) != 0)
			{
				std::cout << "Error: " << duk_safe_to_string(ctx, -1) << std::endl;
				//stack dump
				duk_push_context_dump(ctx);
				std::cout << "Stack:" << std::endl << pDukeNative->ReadFullStack() << std::endl;
			}
			duk_pop(ctx);
		}
		else
		{
			std::cout << "Fail duk_peval_file():" << std::endl <<  duk_safe_to_string(ctx, -1) << std::endl;
			nRes = 1;
		}
		delete pDukeNative;
	}

	duk_destroy_heap(ctx);
	std::system("pause");
	return nRes;
}

