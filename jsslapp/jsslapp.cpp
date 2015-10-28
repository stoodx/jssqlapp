//by http://duktape.org/guide.html#gettingstarted

//#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <fstream>

#include "CDukSyncNativeData.h"

int main(int argc, char* argv[])
{
	int nRes = 0;
	if (argc < 4 || !argv[1] || !argv[2] || !argv[3])
	{
		std::cout <<  "Error:" << std::endl << 
			"Need the pathes to js files" << std::endl;
		std::cout <<  "Usage:" << std::endl << 
			"jssqlapp.exe <path_to_start_js_module> <path_to_sql_js_module> <path_to_sql_database>" << std::endl; 
	}
	else
	{
		stoodx::CDukSyncNativeData* pDukeNative = new stoodx::CDukSyncNativeData(); //init duktape
		if (pDukeNative == NULL)
		{
			std::cout << "No memory" << std::endl;
			std::cout << "Fail duk_create_heap_default()" << std::endl; 
			return 1;
		}
		pDukeNative->InitNative("readFileNative"); //init of a native function
		//run js
		if (pDukeNative->LoadStartModule(argv[1])) //load a start module
		{
			pDukeNative->DefineStartFunction("main"); //define start function - function main(sqlModule, databaseName)
			pDukeNative->LoadArgumentForStartFunction(argv[2]); //load 1 argument
			pDukeNative->LoadArgumentForStartFunction(argv[3]); //load 2 argument 
			if (!pDukeNative->RunJS()) //run
			{
				//read error
				std::cout << "Error: " << pDukeNative->GetDukLastError() << std::endl;
				//stack dump
				std::cout << "Stack:" << std::endl << pDukeNative->ReadFullStack() << std::endl;
			}
			pDukeNative->DukPop(); //Pop off stack
		}
		else
		{
			std::cout << "Fail duk_peval_file():" << std::endl <<  pDukeNative->GetDukLastError() << std::endl;
			nRes = 1;
		}
		delete pDukeNative; //close
	}

	
	std::system("pause");
	return nRes;
}

