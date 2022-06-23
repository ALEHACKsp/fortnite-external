#include <basetsd.h>
#include <cstdint>
#include <random>
#include <windows.h>
#include <winternl.h>
#include <process.h>
#include <tlhelp32.h>
#include <inttypes.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <d3d9.h>
#include <dwmapi.h>
#include <map>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "Dwmapi.lib")

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx9.h"
#include "Imgui/imgui_impl_win32.h"
#include "offsets.h"
#include "urlmon.h"
#include "features.h"
#include "xor.hpp"
#include <mutex>
#include "driverutils.h"
#include <D3dx9tex.h>
#pragma comment(lib, "D3dx9")
#include "icon.h"

#pragma comment(lib, "urlmon.lib")

#define M_PI 3.14159265358979323846264338327950288419716939937510

int Width;
int Height;

ImFont* l_pFont;
ImFont* m_pFont;
ImFont* font1;
ImFont* font2;

DWORD ScreenCenterX;
DWORD ScreenCenterY;
//static HWND Window = NULL;
IDirect3D9Ex* p_Object = NULL;
HWND windowid = NULL;
static LPDIRECT3DDEVICE9 D3dDevice = NULL;
static LPDIRECT3DVERTEXBUFFER9 TriBuf = NULL;
RECT GameRect = { NULL };
D3DPRESENT_PARAMETERS d3dpp;
MSG Message = { NULL };
HWND win = NULL;

auto* bgtext = LPDIRECT3DTEXTURE9();

typedef struct _actors
{
	uint64_t ACTOR;
	int ID;
	uint64_t MESH;
	std::string name;
}actors;

std::vector<actors> allplayers;
std::vector<actors> actorlistids;
std::vector<actors> levelloot;
std::vector<actors> itemsloot;
std::vector<actors> weakspot;

class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f) {}

	Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

	~Vector3() {}

	double x;
	double y;
	double z;

	inline double Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline double Distance(Vector3 v)
	{
		return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(double number) const {
		return Vector3(x * number, y * number, z * number);
	}
};

struct FQuat
{
	double x;
	double y;
	double z;
	double w;
};

struct FTransform
{
	FQuat rot;
	Vector3 translation;
	char pad[4];
	Vector3 scale;
	char pad1[4];
	D3DMATRIX ToMatrixWithScale()
	{
		D3DMATRIX m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		double x2 = rot.x + rot.x;
		double y2 = rot.y + rot.y;
		double z2 = rot.z + rot.z;

		double xx2 = rot.x * x2;
		double yy2 = rot.y * y2;
		double zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		double yz2 = rot.y * z2;
		double wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		double xy2 = rot.x * y2;
		double wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		double xz2 = rot.x * z2;
		double wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

D3DMATRIX matrixx(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}

std::string string2utf8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

namespace Identifier {

	HANDLE DRIVERHANDLE;
	uint64_t ADDRESS;
	int PID;
}

std::wstring widen(const std::string& str)
{
	std::wostringstream wstm;
	const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(wstm.getloc());
	for (size_t i = 0; i < str.size(); ++i)
		wstm << ctfacet.widen(str[i]);
	return wstm.str();
}

static std::mutex mtx;

namespace driver
{
	__int64(__fastcall* NtUserUpdateDefaultDesktopThumbnail)(void* a1) = nullptr;

	bool setup()
	{
		LoadLibraryA("user32.dll");
		LoadLibraryA("win32u.dll");

		const HMODULE win32u = GetModuleHandleA("win32u.dll");
		if (!win32u)
			return false;

		*(void**)&NtUserUpdateDefaultDesktopThumbnail = GetProcAddress(win32u, "NtUserUpdateDefaultDesktopThumbnail");

		return NtUserUpdateDefaultDesktopThumbnail;
	}

	bool Rpm(uint64_t address, void* buffer, size_t size)
	{
		return ReadMemoryFromKernel(Identifier::PID, address, buffer, size);
	}

	bool Wpm(uint64_t address, void* buffer, size_t size)
	{
		return WriteMemoryFromKernel(Identifier::PID, address, buffer, size);
	}

	int get_process_id(const char* process_name)
	{
		PROCESSENTRY32 proc_info;
		proc_info.dwSize = sizeof(proc_info);

		const auto proc_snapshot =
			CreateToolhelp32Snapshot(
				TH32CS_SNAPPROCESS,
				NULL
			);

		if (proc_snapshot == INVALID_HANDLE_VALUE)
			return NULL;

		Process32First(proc_snapshot, &proc_info);
		if (!strcmp(proc_info.szExeFile, process_name)) {
			CloseHandle(proc_snapshot);
			return proc_info.th32ProcessID;
		}

		while (Process32Next(proc_snapshot, &proc_info)) {
			if (!strcmp(proc_info.szExeFile, process_name)) {
				CloseHandle(proc_snapshot);
				return proc_info.th32ProcessID;
			}
		}

		CloseHandle(proc_snapshot);
		return {};
	}

	bool GetModuleBaseAddressFromKernel(int processID, const char* moduleName, uint64_t* baseAddress)
	{
		comms request = {};
		SecureZeroMemory(&request, sizeof(comms));
		request.atype = type::getmbase;
		request.syscode = COMMUNICATION_KEY;
		request.pid = processID;
		request.modulen = moduleName;
		request.buf = 0;
		NtUserUpdateDefaultDesktopThumbnail(&request);

		*baseAddress = request.buf;
		return (request.buf != NULL);
	}


	bool ReadMemoryFromKernel(int processId, uint64_t address, void* buffer, size_t size)
	{
		size_t read = NULL;
		comms request = {};
		SecureZeroMemory(&request, sizeof(comms));

		request.atype = type::rpm;
		request.syscode = COMMUNICATION_KEY;
		request.address = address;
		request.buf = (uint64_t)buffer;
		request.size = size;
		request.pid = processId;
		request.read = read;

		NtUserUpdateDefaultDesktopThumbnail(&request);
		return (buffer != NULL);
	}

	bool WriteMemoryFromKernel(int processId, uint64_t address, void* buffer, size_t size)
	{
		size_t written = NULL;
		comms request = {};
		SecureZeroMemory(&request, sizeof(comms));

		request.atype = type::wpm;
		request.syscode = COMMUNICATION_KEY;
		request.address = address;
		request.buf = (uint64_t)buffer;
		request.size = size;
		request.pid = processId;
		request.read = written;

		NtUserUpdateDefaultDesktopThumbnail(&request);
		return (buffer != NULL);
	}

	bool SendCheckRequest()
	{
		comms request = {};
		SecureZeroMemory(&request, sizeof(comms));

		request.syscode = COMMUNICATION_KEY;
		request.atype = type::check;
		request.loaded = false;

		NtUserUpdateDefaultDesktopThumbnail(&request);
		return request.loaded;
	}
}


Vector3 localactorpos;

typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

void bsod()
{
	BOOLEAN bEnabled;
	ULONG uResp;
	LPVOID lpFuncAddress = GetProcAddress(LoadLibraryA(XorStr("ntdll.dll").c_str()), XorStr("RtlAdjustPrivilege").c_str());
	LPVOID lpFuncAddress2 = GetProcAddress(GetModuleHandle(XorStr("ntdll.dll").c_str()), XorStr("NtRaiseHardError").c_str());
	pdef_RtlAdjustPrivilege NtCall = (pdef_RtlAdjustPrivilege)lpFuncAddress;
	pdef_NtRaiseHardError NtCall2 = (pdef_NtRaiseHardError)lpFuncAddress2;
	NTSTATUS NtRet = NtCall(19, TRUE, FALSE, &bEnabled);
	NtCall2(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &uResp);
}

std::string random_string(const int len)
{
	const std::string alpha_numeric(XorStr("ABCDEFGHIJKLMNOPRSTUVZ01234567899874563210").c_str());
	std::default_random_engine generator{ std::random_device{}() };
	const std::uniform_int_distribution< std::string::size_type > distribution{ 0, alpha_numeric.size() - 1 };
	std::string str(len, 0);
	for (auto& it : str) {
		it = alpha_numeric[distribution(generator)];
	}
	return str;
}

std::string random_string_num(const int len)
{
	const std::string alpha_numeric(XorStr("01234567899874563210").c_str());
	std::default_random_engine generator{ std::random_device{}() };
	const std::uniform_int_distribution< std::string::size_type > distribution{ 0, alpha_numeric.size() - 1 };
	std::string str(len, 0);
	for (auto& it : str) {
		it = alpha_numeric[distribution(generator)];
	}
	return str;
}

int system_no_output(std::string command)
{
	command.insert(0, "/C ");

	SHELLEXECUTEINFOA ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = XorStr("cmd.exe").c_str();
	ShExecInfo.lpParameters = command.c_str();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;

	if (ShellExecuteExA(&ShExecInfo) == FALSE)
		return -1;

	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	DWORD rv;
	GetExitCodeProcess(ShExecInfo.hProcess, &rv);
	CloseHandle(ShExecInfo.hProcess);

	return rv;
}

DWORD inskey(LPVOID in) // loop that checks for INS and F8
{
	while (1)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			features::menu = !features::menu;
		}

		if (GetAsyncKeyState(VK_HOME) & 1) {
			features::allan = !features::allan;
		}

		if (GetAsyncKeyState(VK_F9) & 1)
		{
			features::debug = !features::debug;
		}

		Sleep(2);
	}
}

DWORD fnexit(LPVOID in) //close exe when fn winid not found
{
	while (1)
	{
		Sleep(2);
		if (FindWindowA(0, XorStr("Fortnite  ").c_str()) == 0)
		{
			system(XorStr("cls").c_str());
			system(XorStr("color 4").c_str());
			printf(XorStr("\n\n  LOG: INFO: Fortnite closed, exiting...").c_str());
			Sleep(2000);
			exit(0);
		}
	}
}

// ALL X86 ANTI DEBUG FUNCTIONS //

void isdebuggerpresent(void)
{
	BOOL found = FALSE;
	found = IsDebuggerPresent();

	if (found)
	{
		bsod();
		exit(0);
	}
}

void remotedebugpresent(void)
{
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	BOOL found = FALSE;

	hProcess = GetCurrentProcess();
	CheckRemoteDebuggerPresent(hProcess, &found);

	if (found)
	{
		bsod();
		exit(0);
	}
}

//void ntqueryinfoproc(void)
//{
//	HANDLE hProcess = INVALID_HANDLE_VALUE;
//	PROCESS_BASIC_INFORMATION pProcBasicInfo = { 0 };
//	ULONG returnLength = 0;
//
//	// Get a handle to ntdll.dll so we can import NtQueryInformationProcess
//	HMODULE hNtdll = LoadLibraryW(L"ntdll.dll");
//	if (hNtdll == INVALID_HANDLE_VALUE || hNtdll == NULL)
//	{
//		return;
//	}
//
//	// Dynamically acquire the addres of NtQueryInformationProcess
//	_NtQueryInformationProcess  NtQueryInformationProcess = NULL;
//	NtQueryInformationProcess = (_NtQueryInformationProcess)GetProcAddress(hNtdll, "NtQueryInformationProcess");
//
//	if (NtQueryInformationProcess == NULL)
//	{
//		return;
//	}
//
//	hProcess = GetCurrentProcess();
//
//	// Note: There are many options for the 2nd parameter NtQueryInformationProcess
//	// (ProcessInformationClass) many of them are opaque. While we use ProcessBasicInformation (0), 
//	// we could also use:
//	//      ProcessDebugPort (7)
//	//      ProcessDebugObjectHandle (30)
//	//      ProcessDebugFlags (31)
//	// There are likely others. You can find many other options for ProcessInformationClass over at PINVOKE:
//	//      https://www.pinvoke.net/default.aspx/ntdll/PROCESSINFOCLASS.html
//	// Keep in mind that NtQueryInformationProcess will return different things depending on the ProcessInformationClass used.
//	// Many online articles using NtQueryInformationProcess for anti-debugging will use DWORD types for NtQueryInformationProcess 
//	// paramters. This is fine for 32-builds with some ProcessInformationClass values, but it will cause some to fail on 64-bit builds.
//	// In the event of a failure NtQueryInformationProcess will likely return STATUS_INFO_LENGTH_MISMATCH (0xC0000004). 
//
//	// Query ProcessDebugPort
//	NTSTATUS status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pProcBasicInfo, sizeof(pProcBasicInfo), &returnLength);
//	if (NT_SUCCESS(status)) {
//		PPEB pPeb = pProcBasicInfo.PebBaseAddress;
//		if (pPeb)
//		{
//			if (pPeb->BeingDebugged)
//			{
//				bsod();
//				exit(0);
//			}
//		}
//	}
//}

//void ntinfothread(void)
//{
//	THREAD_INFORMATION_CLASS ThreadHideFromDebugger = (THREAD_INFORMATION_CLASS)0x11;
//
//	// Get a handle to ntdll.dll so we can import NtSetInformationThread
//	HMODULE hNtdll = LoadLibraryW(L"ntdll.dll");
//	if (hNtdll == INVALID_HANDLE_VALUE || hNtdll == NULL)
//	{
//		return;
//	}
//
//	// Dynamically acquire the addres of NtSetInformationThread and NtQueryInformationThread
//	_NtSetInformationThread NtSetInformationThread = NULL;
//	NtSetInformationThread = (_NtSetInformationThread)GetProcAddress(hNtdll, "NtSetInformationThread");
//
//	if (NtSetInformationThread == NULL)
//	{
//		return;
//	}
//
//	// There is nothing to check here after this call.
//	NtSetInformationThread(GetCurrentThread(), ThreadHideFromDebugger, 0, 0);
//}

void debugactiveproc(const char* cpid)
{
	BOOL found = FALSE;
	STARTUPINFOA si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof(si);
	TCHAR szPath[MAX_PATH];
	DWORD exitCode = 0;

	CreateMutex(NULL, FALSE, XorStr("antidbg").c_str());
	if (GetLastError() != ERROR_SUCCESS)
	{
		// If we get here we are in the child process
		if (DebugActiveProcess((DWORD)atoi(cpid)))
		{
			// No debugger found.
			return;
		}
		else
		{
			// Debugger found, exit child with a unique code we can check for.
			exit(555);
		}
	}

	// parent process
	DWORD pid = GetCurrentProcessId();
	GetModuleFileName(NULL, szPath, MAX_PATH);

	char cmdline[MAX_PATH + 1 + sizeof(int)];
	snprintf(cmdline, sizeof(cmdline), "%ws %d", szPath, pid);

	// Start the child process. 
	BOOL success = CreateProcessA(
		NULL,		// path (NULL means use cmdline instead)
		cmdline,	// Command line
		NULL,		// Process handle not inheritable
		NULL,		// Thread handle not inheritable
		FALSE,		// Set handle inheritance to FALSE
		0,			// No creation flags
		NULL,		// Use parent's environment block
		NULL,		// Use parent's starting directory 
		&si,		// Pointer to STARTUPINFO structure
		&pi);		// Pointer to PROCESS_INFORMATION structure

	// Wait until child process exits and get the code
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Check for our unique exit code
	GetExitCodeProcess(pi.hProcess, &exitCode);
	if (exitCode == 555)
	{
		found = TRUE;
	}

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (found)
	{
		bsod();
		exit(0);
	}
}

void hardwaredebugreg(void)
{
	BOOL found = FALSE;
	CONTEXT ctx = { 0 };
	HANDLE hThread = GetCurrentThread();

	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	if (GetThreadContext(hThread, &ctx))
	{
		if ((ctx.Dr0 != 0x00) || (ctx.Dr1 != 0x00) || (ctx.Dr2 != 0x00) || (ctx.Dr3 != 0x00) || (ctx.Dr6 != 0x00) || (ctx.Dr7 != 0x00))
		{
			found = TRUE;
		}
	}

	if (found)
	{
		bsod();
		exit(0);
	}
}

void closehandleexept(void)
{
	HANDLE hInvalid = (HANDLE)0xBEEF; // an invalid handle
	DWORD found = FALSE;

	__try
	{
		CloseHandle(hInvalid);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		found = TRUE;
	}

	if (found)
	{
		bsod();
		exit(0);
	}
}

DWORD antidbgshit(LPVOID in)
{
	while (1)
	{
		//adbg_BeingDebuggedPEBx64(); // adbg_BeingDebuggedPEB();
		//adbg_NtGlobalFlagPEBx64(); // adbg_NtGlobalFlagPEB();
		remotedebugpresent();
		/*ntqueryinfoproc();*/
		isdebuggerpresent();
		/*ntinfothread();*/
		hardwaredebugreg();
		/*adbg_MovSS();*/
	   /*adbg_GetTickCountx64();*/ //adbg_GetTickCount();
		closehandleexept();
		/*adbg_SingleStepExceptionx64();*/
		/*adbg_Int3x64();*/
		/*adbg_Int2Dx64();*/
		//adbg_PrefixHop();
		Sleep(2);
	}
}

static std::string ReadGetNameFromFName(int key) {
	uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
	uint16_t NameOffset = (uint16_t)key;

	uint64_t NamePoolChunk = ReadPointer<uint64_t>(Identifier::ADDRESS + 0xC42D200 + (8 * ChunkOffset) + 16) + (unsigned int)(4 * NameOffset);
	uint16_t nameEntry = ReadPointer<uint16_t>(NamePoolChunk);

	int nameLength = nameEntry >> 6;
	char buff[1024];
	if ((uint32_t)nameLength) {
		char* v2 = buff; // rbx
		unsigned int v4 = nameLength;
		unsigned int v5; // eax
		__int64 result; // rax
		int v7; // ecx
		unsigned int v8; // kr00_4
		__int64 v9; // ecx

		driver::ReadMemoryFromKernel(Identifier::PID, NamePoolChunk + 4, buff, v4);

		v5 = 0;
		result = 28i64;
		if (v4) {
			do {
				++v2;
				v7 = v5++ | 0xB000;
				v8 = v7 + result;
				BYTE(v7) = v8 ^ ~*(BYTE*)(v2 - 1);
				result = v8 >> 2;
				*(BYTE*)(v2 - 1) = v7;
			} while (v5 < v4);
		}
		buff[nameLength] = '\0';
		return std::string(buff);
	}
	else {
		return "";
	}
}

static std::string GetNameFromFName(int key)
{
	uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
	uint16_t NameOffset = (uint16_t)key;

	uint64_t NamePoolChunk = ReadPointer<uint64_t>(Identifier::ADDRESS + 0xc562740 + (8 * ChunkOffset) + 16) + (unsigned int)(4 * NameOffset); //((ChunkOffset + 2) * 8) ERROR_NAME_SIZE_EXCEEDED
	uint16_t nameEntry = ReadPointer<uint16_t>(NamePoolChunk);

	int nameLength = nameEntry >> 6;
	char buff[1024];
	if ((uint32_t)nameLength)
	{
		for (int x = 0; x < nameLength; ++x)
		{
			buff[x] = ReadPointer<char>(NamePoolChunk + 4 + x);
		}
		char* v2 = buff; // rbx
		unsigned int v4 = nameLength;
		unsigned int v5; // eax
		__int64 result; // rax
		int v7; // ecx
		unsigned int v8; // kr00_4
		__int64 v9; // ecx

		v5 = 0;
		result = ReadPointer<unsigned int>(Identifier::ADDRESS + 0xC42D698) >> 5;
		if (v4)
		{
			do
			{
				v7 = *v2++;
				v8 = result ^ (16 * v7) ^ (result ^ (v7 >> 4)) & 0xF;
				result = (unsigned int)(result + 4 * v5++);
				*(v2 - 1) = v8;
			} while (v5 < v4);
		}
		buff[nameLength] = '\0';
		return std::string(buff);
	}
	else
	{
		return "";
	}
}
