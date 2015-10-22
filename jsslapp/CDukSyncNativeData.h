#pragma once

extern "C" {
#include "duktape.h"
}

namespace stoodx
{
	class CDukSyncNativeData
	{
	public:
		CDukSyncNativeData(duk_context* ctx);
		~CDukSyncNativeData(void);
		static duk_ret_t get_sync_data_native(duk_context *ctx);
		static CDukSyncNativeData* m_pCDukSyncNativeData;
	private:
		__int64 getFileSize(const char* strFileName);
		char* m_pBuf;
		duk_context* m_ctx;
	};


}

