#pragma once

extern "C" {
#include "duktape.h"
}

namespace stoodx
{
	class —DukSyncNativeData
	{
	public:
		—DukSyncNativeData(duk_context* ctx);
		~—DukSyncNativeData(void);
		static duk_ret_t get_sync_data_native(duk_context *ctx);
		static —DukSyncNativeData* m_p—DukSyncNativeData;
	private:
		__int64 getFileSize(const char* strFileName);
		char* m_pBuf;
		duk_context* m_ctx;
	};


}

