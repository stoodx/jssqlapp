#include "stdafx.h"
#include <string>
#include <fstream>

#include "—DukSyncNativeData.h"

stoodx::—DukSyncNativeData* stoodx::—DukSyncNativeData::m_p—DukSyncNativeData = NULL;

duk_ret_t stoodx::—DukSyncNativeData::get_sync_data_native(duk_context *ctx)
{
	std::string strFileName = duk_require_string(ctx, 0);
	if (strFileName.empty() || !m_p—DukSyncNativeData)
	{
		duk_push_null(ctx);
		return 1;
	}

	int nSize = 0;  
	nSize = (int) m_p—DukSyncNativeData->getFileSize(strFileName.c_str());
	if ( nSize > 0)
	{
		m_p—DukSyncNativeData->m_pBuf = (char*)duk_push_buffer(ctx, nSize, 0);	
		if (m_p—DukSyncNativeData->m_pBuf == NULL)
			duk_push_null(ctx);
		else
		{
			 std::ifstream is (strFileName, std::ifstream::binary);
			 if (is)
			 {
				 is.read (m_p—DukSyncNativeData->m_pBuf, nSize);
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


stoodx::—DukSyncNativeData::—DukSyncNativeData(duk_context * ctx)
	: m_ctx(ctx)
	, m_pBuf(NULL)
{
	if (!m_ctx)
		return;
	m_p—DukSyncNativeData = this;
	//preparate a native function
	duk_push_global_object(m_ctx);
	duk_push_c_function(ctx, get_sync_data_native, 1);
	duk_put_prop_string(ctx, -2, "getSyncDataNative");
}


stoodx::—DukSyncNativeData::~—DukSyncNativeData(void)
{
	m_p—DukSyncNativeData = NULL;
}

__int64 stoodx::—DukSyncNativeData::getFileSize(const char* strFileName)
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