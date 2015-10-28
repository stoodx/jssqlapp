#include <windows.h>
#include <string>
#include <fstream>

#include "CDukSyncNativeData.h"

stoodx::CDukSyncNativeData* stoodx::CDukSyncNativeData::m_pCDukSyncNativeData = NULL;

duk_ret_t stoodx::CDukSyncNativeData::get_sync_data_native(duk_context *ctx)
{
	std::string strFileName = duk_require_string(ctx, 0);
	if (strFileName.empty() || !m_pCDukSyncNativeData)
	{
		duk_push_null(ctx);
		return 1;
	}
	if (m_pCDukSyncNativeData->m_status == DUKSYNC_ERR)
		return 1;	

	int nSize = 0;  
	nSize = (int) m_pCDukSyncNativeData->GetFileSize(strFileName.c_str());
	if ( nSize > 0)
	{
		m_pCDukSyncNativeData->m_pBuf = (char*)duk_push_buffer(ctx, nSize, 0);	
		if (m_pCDukSyncNativeData->m_pBuf == NULL)
		{
			m_pCDukSyncNativeData->m_status = DUKSYNC_ERR;
			duk_push_null(ctx);
		}
		else
		{
			 std::ifstream is (strFileName, std::ifstream::binary);
			 if (is)
			 {
				 is.read (m_pCDukSyncNativeData->m_pBuf, nSize);
				 is.close();
			 }
			 else
			 {
				 m_pCDukSyncNativeData->m_status = DUKSYNC_ERR;
				 duk_push_null(ctx);
			 }
		}
	}
	else
	{
		m_pCDukSyncNativeData->m_status = DUKSYNC_ERR;
		duk_push_null(ctx);
	}
	return 1;
}


stoodx::CDukSyncNativeData::CDukSyncNativeData()
	: m_ctx(NULL)
	, m_pBuf(NULL)
	, m_status(DUKSYNC_ERR)
	, m_nArg(0)
{
	m_ctx = duk_create_heap_default();
	if (!m_ctx)
		return;
	m_status = DUKSYNC_INIT;
	m_pCDukSyncNativeData = this;

}

stoodx::CDukSyncNativeData::DUKSYNC_STATUS stoodx::CDukSyncNativeData::InitNative(const char* strFunctionName)
{
	if (m_status == DUKSYNC_ERR)
		return m_status;
	if (!strFunctionName)
	{
		m_status = DUKSYNC_ERR;
		return m_status;
	}

	//preparate a native function
	duk_push_global_object(m_ctx);
	duk_push_c_function(m_ctx, get_sync_data_native, 1);
	duk_put_prop_string(m_ctx, -2,  strFunctionName);
	m_status = DUKSYNC_NATIVE_READY;
	return m_status;
}

stoodx::CDukSyncNativeData::~CDukSyncNativeData(void)
{
	m_pCDukSyncNativeData = NULL;
	if (m_ctx)
		duk_destroy_heap(m_ctx);
}

bool stoodx::CDukSyncNativeData::LoadStartModule(const char* strFileName)
{
	if (m_status == DUKSYNC_ERR || !strFileName)
		return false;
	if (duk_peval_file(m_ctx, strFileName) == 0)
	{
		duk_pop(m_ctx);
		m_status = DUKSYNC_START_MODULE_LOADED;
		return true;
	}
	m_status = DUKSYNC_ERR;
	return false;
}

bool stoodx::CDukSyncNativeData::DefineStartFunction(const char* strFunc)
{
	if (m_status != DUKSYNC_START_MODULE_LOADED || !strFunc)
		return false;
	duk_get_prop_string(m_ctx, -1, strFunc);
	m_status = DUKSYNC_START_FUNC_DEFINED;
	return true;
}

bool stoodx::CDukSyncNativeData::LoadArgumentForStartFunction(const char* strArg)
{
	if (m_status != DUKSYNC_START_FUNC_DEFINED || !strArg)
		return false;
	duk_push_string(m_ctx, strArg);
	m_nArg++;
	return true;
}

bool stoodx::CDukSyncNativeData::RunJS()
{
	if (m_status != DUKSYNC_START_FUNC_DEFINED)
		return false;
	if (duk_pcall(m_ctx, m_nArg) != 0)
		return false;
	return true;
}

const char* stoodx::CDukSyncNativeData::ReadFullStack()
{
	if (!m_ctx)
		return NULL;
	duk_push_context_dump(m_ctx);
	//duk_get_prop_string(m_ctx, -1, "stack");
	return duk_safe_to_string(m_ctx, -1);
}

const char* stoodx::CDukSyncNativeData::GetDukLastError()
{
	if (!m_ctx)
		return NULL;
	return duk_safe_to_string(m_ctx, -1);
}

__int64 stoodx::CDukSyncNativeData::GetFileSize(const char* strFileName)
{
	if (!strFileName)
	{
		SetLastError(ERROR_INVALID_DATA);
		return -1;
	}
	HANDLE hFile = CreateFileA(strFileName, GENERIC_READ, 
								FILE_SHARE_READ, NULL, OPEN_EXISTING, 
								FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile==INVALID_HANDLE_VALUE)
		return -1; 
	LARGE_INTEGER liFileSize;
    if (!GetFileSizeEx(hFile, &liFileSize))
    {
        CloseHandle(hFile);
        return -1; // error condition, could call GetLastError to find out more
    }

    CloseHandle(hFile);
    return liFileSize.QuadPart;
}

bool stoodx::CDukSyncNativeData::DukPop( duk_idx_t nCount)
{
	if (m_status == DUKSYNC_ERR)
		return false;
	duk_pop_n(m_ctx, nCount);
	return true;
}