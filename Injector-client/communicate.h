#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>

#define COMMAND_KEY 0xDEADBEEF

namespace Driver
{
	extern HANDLE driver_handle;

	enum messages : __int64
	{
		NONE,
		START_THREAD,
		ALLOC_MEM,
		WRITE_MEM,
		MODULE_BASE,
		EXIT_CLEANUP,
		SET_NPT_HOOK,
		PROCESS_ID
	};

	struct Msg
	{
		int64_t command_key;
		int message_id;
	};

	struct GetProcessIdMsg : Msg
	{
		wchar_t process_name[50];
	};

	struct NptHookMsg : Msg
	{
		int32_t proc_id;
		size_t size;
		uintptr_t hook_address;
		uint8_t shellcode[0x1000];
	};

	struct AllocMemCmd : Msg
	{
		int32_t proc_id;
		size_t size;
		wchar_t	section_name[60];
	};

	struct InvokeRemoteFunctionCmd : Msg
	{
		int proc_id;
		uintptr_t map_base;
		uintptr_t address;
		uintptr_t image_size;
		uintptr_t RtlAddFunctionTable_address;
	};

	struct GetModuleMsg : Msg
	{
		uint32_t proc_id;
		wchar_t module[50];
	};

	struct WriteCmd : Msg
	{
		int proc_id;
		uintptr_t address;
		BYTE* buffer;
		int size;
	};

	struct InitMsg : Msg
	{
		int proc_id;
	};

	bool SetNptHook(int32_t proc_id, size_t size, uintptr_t hook_address, uint8_t* shellcode);

	uintptr_t AllocateMemory(DWORD proc_id, DWORD size);

	BOOL InvokeRemoteFunc(ULONG64 start_addr, int proc_id, uintptr_t params_addr, uintptr_t real_image_size);

	bool WriteMem(int process_id, ULONG64 address, BYTE* buffer, int size);

	void ExitDriver();

	uint64_t GetModuleBase(std::wstring module, int pid);

	int GetProcessId(const wchar_t* process_name);

	void Init();
}
