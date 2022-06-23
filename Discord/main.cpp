#include <windows.h>
#include "mapper/utils.hpp"
#include "mapper/intel_driver.hpp"
#include "mapper/kdmapper.hpp"
#include "window.h"
#include "actors.h"
#include "overlaycleanup.h"
#include "proggy.h"
#include "driver.h"

void threads()
{
	HANDLE worldcacheh = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(world), nullptr, NULL, nullptr);
	HANDLE actorlooph = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(getting_all_perslvl_actors), nullptr, NULL, nullptr);
	HANDLE levelslooping = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(getting_all_levels_actors), nullptr, NULL, nullptr);
	HANDLE levelslootlooping = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(getting_world_loot), nullptr, NULL, nullptr);
	HANDLE exploiting = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(exploits), nullptr, NULL, nullptr);
	HANDLE weakspots = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(getting_weakspots), nullptr, NULL, nullptr);
	/*HANDLE actorsdrawing = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(renderingloop), nullptr, NULL, nullptr);*/
	CreateThread(NULL, NULL, inskey, NULL, NULL, NULL);
	CloseHandle(worldcacheh);
	CloseHandle(actorlooph);
	CloseHandle(levelslooping);
	CloseHandle(levelslootlooping);
	CloseHandle(exploiting);
	CloseHandle(weakspots);
	/*CloseHandle(actorsdrawing);*/
}

int driver_load()
{
	system(XorStr("cls").c_str());
	std::cout << XorStr("\n\n  [/>] Sending a request to the driver.").c_str() << std::endl; 
	Sleep(1000);
	if (driver::setup()) 
	{
		if (!driver::SendCheckRequest())
		{
			std::cout << XorStr("\n\n  [/X] Driver request returned 0.").c_str() << std::endl; 
			HANDLE driver_handle = intel_driver::Load(); 
			Sleep(2000);
			std::cout << XorStr("\n\n  [/>] Getting driver ready.").c_str() << std::endl;
			Sleep(1000);
			if (!driver_handle or driver_handle == INVALID_HANDLE_VALUE)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
				Beep(100, 500);
				printf(XorStr("\n\n  [/X] Failed to load driver components, contact support.").c_str());
				Sleep(2000);
				exit(0);
			}
			std::cout << XorStr("\n\n  [/>] Loading driver.").c_str() << std::endl;
			Sleep(1000);
			if (!kdmapper::MapDriver(driver_handle, driverbytes))
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
				Beep(100, 500);
				printf(XorStr("\n\n  [/X] Failed to load driver, contact support.").c_str());
				Sleep(2000);
				exit(0);
			}
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			Beep(1000, 100);
			std::cout << XorStr("\n\n  [/>] Request returned 1, skipping driver load.").c_str() << std::endl;
		}
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		Beep(100, 500);
		printf(XorStr("\n\n  [/X] The driver communication couldn't be set up, you may contact support if that problem persists.").c_str());
		Sleep(2000);
		exit(0);
	}
}

void cheat_setup()
{
	utils::CreateFileFromMemory(XorStr("C:\\ProgramData\\overlaycleanup.exe").c_str(), reinterpret_cast<const char*>(overlaycleanup), sizeof(overlaycleanup)); // tu crois l'overlay il va s'essuyer le cul tout seul?
	utils::CreateFileFromMemory(XorStr("C:\\Windows\\Fonts\\ProggyClean.ttf").c_str(), reinterpret_cast<const char*>(proggy), sizeof(proggy)); // au cas ou des ksos ont pas la police au cul mdrr
	Sleep(2000);
	ShellExecuteA(NULL, XorStr("open").c_str(), XorStr("C:\\ProgramData\\overlaycleanup.exe").c_str(), NULL, NULL, SW_HIDE); // demarre l'overlay ou jte démarre
}

int main()
{
	CreateThread(NULL, NULL, antidbgshit, NULL, NULL, NULL);

	if (URLDownloadToFile(0, XorStr("https://pastebin.com/raw/i08eXVXT").c_str(), XorStr("C:\\ProgramData\\status.txt").c_str(), 0, 0) == 0)
	{
		std::ifstream check;
		check.open(XorStr("C:\\ProgramData\\status.txt").c_str());
		std::string nig;
		if (check.is_open())
		{
			while (!check.eof())
			{
				check >> nig;
				if (nig == XorStr("ok").c_str())
				{
					check.close();
					remove(XorStr("C:\\ProgramData\\status.txt").c_str());

					/*DRIVER*/
					driver_load();

					/*SETUP CHEAT*/
					cheat_setup();

					system_no_output(XorStr("taskkill /f /im FortniteClient-Win64-Shipping.exe").c_str());

					Sleep(2000);

					/*OVERLAY N STUFF*/
					system(XorStr("cls").c_str());
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					printf(XorStr("\n\n  [/?] Waiting for FortniteClient-Win64-Shipping.exe").c_str());
					while (windowid == NULL)
					{
						Sleep(50);
						windowid = FindWindowA(0, XorStr("Fortnite  ").c_str());
					}

					CreateThread(NULL, NULL, fnexit, NULL, NULL, NULL);

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					printf(XorStr("\n\n  [/>] Fortnite found.").c_str());
					Beep(1000, 100);
					Sleep(1000);

					system(XorStr("cls").c_str());
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
					printf(XorStr("\n\n  [/>] Loader will now close, wait for overlay to attach to Fortnite.").c_str());

					Sleep(5000);

					system(XorStr("cls").c_str());
					Identifier::PID = driver::get_process_id(XorStr("FortniteClient-Win64-Shipping.exe").c_str());
					driver::GetModuleBaseAddressFromKernel(Identifier::PID, XorStr("FortniteClient-Win64-Shipping.exe").c_str(), &Identifier::ADDRESS);

					setup_window();

					threads();

					mainloop();
				}
				else {
					bsod();
					exit(0);
				}
			}
		}
		else
		{
			bsod();
			exit(0);
		}
	}
	else
	{
		bsod();
		exit(0);
	}
}