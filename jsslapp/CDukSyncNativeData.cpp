#include "stdafx.h"
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

	int nSize = 0;  
	nSize = (int) m_pCDukSyncNativeData->getFileSize(strFileName.c_str());
	if ( nSize > 0)
	{
		m_pCDukSyncNativeData->m_pBuf = (char*)duk_push_buffer(ctx, nSize, 0);	
		if (m_pCDukSyncNativeData->m_pBuf == NULL)
			duk_push_null(ctx);
		else
		{
			 std::ifstream is (strFileName, std::ifstream::binary);
			 if (is)
			 {
				 is.read (m_pCDukSyncNativeData->m_pBuf, nSize);
				 is.close();
			 }
			 else
				 duk_push_null(ctx);
		}
	}
	else
		duk_push_null(ctx);
	return 1;
}


stoodx::CDukSyncNativeData::CDukSyncNativeData(duk_context * ctx)
	: m_ctx(ctx)
	, m_pBuf(NULL)
{
	if (!m_ctx)
		return;
	m_pCDukSyncNativeData = this;
	//preparate a native function
	duk_push_global_object(m_ctx);
	duk_push_c_function(ctx, get_sync_data_native, 1);
	duk_put_prop_string(ctx, -2, "getSyncDataNative");
}


stoodx::CDukSyncNativeData::~CDukSyncNativeData(void)
{
	m_pCDukSyncNativeData = NULL;
}

__int64 stoodx::CDukSyncNativeData::getFileSize(const char* strFileName)
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