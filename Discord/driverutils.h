#pragma once
#include <Windows.h>
#include <vector>
#include <map>

namespace driver
{
	template<typename T>
	T ReadPointer(uint64_t address);
	bool Rpm(uint64_t address, void* buffer, size_t size);

	template<typename T>
	void WritePointer(uint64_t address, T value);
	bool Wpm(uint64_t address, void* buffer, size_t size);

	bool GetModuleBaseAddressFromKernel(int processID, const char* moduleName, uint64_t* baseAddress);
	bool ReadMemoryFromKernel(int processId, uint64_t address, void* buffer, size_t size);
	bool WriteMemoryFromKernel(int processId, uint64_t address, void* buffer, size_t size);
}

#define COMMUNICATION_KEY 0xABBDEE44487

enum type {
	getmbase = 0,
	rpm = 1,
	wpm = 2,
	check = 3
};
struct comms {
	uint64_t syscode;
	type atype;
	int pid;
	uint64_t address;
	const char* modulen;
	uint64_t buf;
	size_t size;
	union
	{
		SIZE_T written;
		SIZE_T read;
	};
	bool loaded;
};

template<typename T>
inline T ReadPointer(uint64_t address)
{
	T buffer;
	driver::Rpm(address, &buffer, sizeof(T));
	return buffer;
}

template<typename T>
inline void WritePointer(uint64_t address, T value)
{
	driver::Wpm(address, &value, sizeof(T));
}
