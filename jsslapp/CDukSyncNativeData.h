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

		enum DUKSYNC_STATUS
		{
			DUKSYNC_ERR = -1,
			DUKSYNC_INIT,
			DUKSYNC_NATIVE_READY
		};

		DUKSYNC_STATUS InitNative(const char* strFunctionName);
		const char* ReadFullStack();
		const char* GetDukLastError();
		bool DukPop( duk_idx_t nCount = 1);

	private:
		__int64 GetFileSize(const char* strFileName);
		char* m_pBuf;
		duk_context* m_ctx;
		DUKSYNC_STATUS m_status;
	};


}

