// UMU 2022 (An Unreal Engine 4 Third Party Content Creation Unlocker)
// Created By GHFear @ Illusory Software - // Feck- ..Forked By Aeyth8


// INCLUDES ------------------------>

#include "stdafx.h"
#include <stdint.h>
#include <stdio.h>

#include "MinHook/MinHook.h"
#include "DXGIProxy/proxy.h"
#include "UMUClasses/SigScanner/SigScanners/SigScanner_LooseFileLoader.h"
#include "UMUClasses/FunctionHooks/LooseFileLoadingHooks.h"

#include <shellapi.h>
#include <iostream>
#include <algorithm>
#include "./Other/global.h"


//------------------------ END OF INCLUDES


// Setup for the dxgi Proxy
EXTERN_C IMAGE_DOS_HEADER __ImageBase; // We might need this some day. Don't remove.
HMODULE ourModule = 0; // Declare our "extern HMODULE ourModule" from proxy.cpp inside dllmain, so we can pass hModule to Proxy_Attach();


//Function Defs -------------------->
DWORD WINAPI RevertToStock(LPVOID lpParam);
void BingleBangle();
void WSOD(std::string key, std::string value);
void Hooks();
void Initiate_Hooks();
void May_I_Take_Your_Hat_Sir();
void How_Many_Times_Do_We_Have_To_Teach_You_This_Lesson_Old_Man();
void DidAnyLibrariesLoad();
void Hook_Failure();
DWORD WINAPI MaybeSoMaybeNo(LPVOID lpParam);
//------------------------ END OF Function Defs



//Set our important Global Vars ------>
char* exe_base = nullptr;
bool D0 = false;
bool D1 = false;
bool D2 = false;
bool D3 = false;
bool D4 = false;
bool D5 = false;
bool D6 = false;
bool D7 = false;
bool D8 = false;
bool D9 = false;
bool D10 = false;
bool D11 = false;
std::wstring LoadedLibraries = L"";
int Hourglass = 1;
int LibraryCt = 0;
//------------------------ END OF Global Vars

//Start routine
BOOL APIENTRY DllMain(HMODULE hModule, int ul_reason_for_call, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);
	BingleBangle();
	
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		ourModule = hModule;
		
		Proxy_Attach();

		Initiate_Hooks();

	}
	if (ul_reason_for_call == DLL_PROCESS_DETACH)
		Proxy_Detach();
		
	How_Many_Times_Do_We_Have_To_Teach_You_This_Lesson_Old_Man();

	return TRUE;
}


DWORD WINAPI RevertToStock(LPVOID lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;
	HANDLE hThread = CreateThread(nullptr, 0,
		(LPTHREAD_START_ROUTINE)Hook_Failure, hModule, 0, 0);
	if (hThread != nullptr)
		CloseHandle(hThread);
	return 0;
}

void WSOD(std::string key, std::string value) {
	std::wstring DLLNum = Convertion(key);
	std::wstring DLLName = Convertion(value);
	std::wstring ErrorMessage = DLLNum + L"=" + DLLName + L" has failed to parse, please make sure you used proper syntax.\n\nAn example of proper formatting would be: " + DLLNum + L"=ExampleDLL.dll\n\n\nIf you would like to continue without loading this DLL, click 'OK', otherwise click 'CANCEL' to close the game.";
	const int result = MessageBox(NULL, ErrorMessage.c_str(), L"! Improper Syntax !", MB_OKCANCEL | MB_SYSTEMMODAL);
	switch (result)
	{
	case IDOK:
		break;
	case IDCANCEL:
		abort();
		break;
	}
}


bool IsValidDLL(const std::string& value, const std::string& key) {
	std::string DLL_Template_Name = "PutYourDLLHere.dll";
	std::string ProperExtension = "dll";
	auto File_Extension_Dot = value.rfind('.');

	if (value == DLL_Template_Name || value.empty() || std::all_of(value.begin(), value.end(), ::isspace)) {
		return false;
	}

	if (File_Extension_Dot == std::string::npos) {
		WSOD(key, value);
		return false;
	}
	std::string Extracted_Extension = value.substr(File_Extension_Dot + 1);

	if (Extracted_Extension != ProperExtension) {
		WSOD(key, value);
		return false;
	}
	
	return true;
}

bool IsValidTimer(const std::string& value) {
	std::string Timer_Template_Name = "JustPutANumberInSeconds";
	int Testing = 0;

	if (value == Timer_Template_Name || value.empty() || std::all_of(value.begin(), value.end(), ::isspace)) {
		return false;
	}

	try {
		Testing = std::stoi(value);
	}

	catch (const std::exception& e) {
		return false;
	}

	return true;
}

//It attempts to read a .ini file within the binaries directory named "DList.ini", if it is unable to find this file, it creates one.
void BingleBangle() {
	std::ifstream infile("DList.ini");
	if (!infile.good()) {
		std::ofstream outfile("DList.ini");
		outfile << "[DLLs]\n";
		outfile << "DLL0=PutYourDLLHere.dll\n";
		outfile << "DLL1=PutYourDLLHere.dll\n";
		outfile << "DLL2=PutYourDLLHere.dll\n";
		outfile << "DLL3=PutYourDLLHere.dll\n";
		outfile << "DLL4=PutYourDLLHere.dll\n";
		outfile << "DLL5=PutYourDLLHere.dll\n";
		outfile << "DLL6=PutYourDLLHere.dll\n";
		outfile << "DLL7=PutYourDLLHere.dll\n";
		outfile << "DLL8=PutYourDLLHere.dll\n";
		outfile << "DLL9=PutYourDLLHere.dll\n";
		outfile << "DLL10=PutYourDLLHere.dll\n\n";
		outfile << "[Timer]\n";
		outfile << "Timer0=JustPutANumberInSeconds\n";
		outfile << "Timer1=JustPutANumberInSeconds\n";
		outfile << "Timer2=JustPutANumberInSeconds\n";
		outfile << "Timer3=JustPutANumberInSeconds\n";
		outfile << "Timer4=JustPutANumberInSeconds\n";
		outfile << "Timer5=JustPutANumberInSeconds\n";
		outfile << "Timer6=JustPutANumberInSeconds\n";
		outfile << "Timer7=JustPutANumberInSeconds\n";
		outfile << "Timer8=JustPutANumberInSeconds\n";
		outfile << "Timer9=JustPutANumberInSeconds\n";
		outfile << "Timer10=JustPutANumberInSeconds\n\n";
		outfile.close();
		const int result = MessageBox(NULL, L"'DList.ini' was not found in the binaries, one has been created.", L"Relaunch The Game", MB_OK | MB_SYSTEMMODAL);
		switch (result)
		{
		case IDOK:
			abort();
			break;
		}
	}
	else {
		std::string line;
		std::string currentSection;

		while (std::getline(infile, line)) {
			// Trim whitespace from both ends of the line
			line.erase(line.find_last_not_of(" \t") + 1);
			line.erase(0, line.find_first_not_of(" \t"));

			// Skip empty lines and comments
			if (line.empty() || line[0] == ';') {
				continue;
			}

			// Check if line is a section header
			if (line[0] == '[' && line.back() == ']') {
				currentSection = line.substr(1, line.size() - 2);
				continue;
			}

			// Find the position of the equal sign
			auto pos = line.find('=');
			if (pos == std::string::npos) {
				continue; // Invalid line format
			}

			// Extract key and value
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
			

			// This is both inefficient, boring, and psychological torture ; PS - At the time of writing this I have been up for almost 24 hours straight :(
			if (currentSection == "DLLs") {
				if (key == "DLL0" && IsValidDLL(value, key)) {
					Global::DLL0 = value;
					D0 = true;
				}
				if (key == "DLL1" && IsValidDLL(value, key)) {
					Global::DLL1 = value;
					D1 = true;
				}
				if (key == "DLL2" && IsValidDLL(value, key)) {
					Global::DLL2 = value;
					D2 = true;
				}
				if (key == "DLL3" && IsValidDLL(value, key)) {
					Global::DLL3 = value;
					D3 = true;
				}
				if (key == "DLL4" && IsValidDLL(value, key)) {
					Global::DLL4 = value;
					D4 = true;
				}
				if (key == "DLL5" && IsValidDLL(value, key)) {
					Global::DLL5 = value;
					D5 = true;
				}
				if (key == "DLL6" && IsValidDLL(value, key)) {
					Global::DLL6 = value;
					D6 = true;
				}
				if (key == "DLL7" && IsValidDLL(value, key)) {
					Global::DLL7 = value;
					D7 = true;
				}
				if (key == "DLL8" && IsValidDLL(value, key)) {
					Global::DLL8 = value;
					D8 = true;
				}
				if (key == "DLL9" && IsValidDLL(value, key)) {
					Global::DLL9 = value;
					D9 = true;
				}
				if (key == "DLL10" && IsValidDLL(value, key)) {
					Global::DLL10 = value;
					D10 = true;
				}
			}
			if (currentSection == "Timer") {
				if (key == "Timer0" && D0 && IsValidTimer(value)) {
					Global::Timer0 = std::stoi(value);
				}
				if (key == "Timer1" && D1 && IsValidTimer(value)) {
					Global::Timer1 = std::stoi(value);
				}
				if (key == "Timer2" && D2 && IsValidTimer(value)) {
					Global::Timer2 = std::stoi(value);
				}
				if (key == "Timer3" && D3 && IsValidTimer(value)) {
					Global::Timer3 = std::stoi(value);
				}
				if (key == "Timer4" && D4 && IsValidTimer(value)) {
					Global::Timer4 = std::stoi(value);
				}
				if (key == "Timer5" && D5 && IsValidTimer(value)) {
					Global::Timer5 = std::stoi(value);
				}
				if (key == "Timer6" && D6 && IsValidTimer(value)) {
					Global::Timer6 = std::stoi(value);
				}
				if (key == "Timer7" && D7 && IsValidTimer(value)) {
					Global::Timer7 = std::stoi(value);
				}
				if (key == "Timer8" && D8 && IsValidTimer(value)) {
					Global::Timer8 = std::stoi(value);
				}
				if (key == "Timer9" && D9 && IsValidTimer(value)) {
					Global::Timer9 = std::stoi(value);
				}
				if (key == "Timer10" && D10 && IsValidTimer(value)) {
					Global::Timer10 = std::stoi(value);
				}

			}

		}
	}
	int Highest_Value = std::max({ Global::Timer0, Global::Timer1, Global::Timer1, Global::Timer2, Global::Timer3, Global::Timer4, Global::Timer5, Global::Timer6,  Global::Timer7, Global::Timer8, Global::Timer9, Global::Timer10 });
	Hourglass += Highest_Value;
}



void May_I_Take_Your_Hat_Sir() {
	Sleep(8 * 1000);
	//Prepare_DLLs();

}

DWORD WINAPI LoadLibrary0(LPVOID lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;
	HANDLE hThread = CreateThread(nullptr, 0,
		(LPTHREAD_START_ROUTINE)AttachDLL0, hModule, 0, 0);
	if (hThread != nullptr)
		CloseHandle(hThread);
	return 0;
}

DWORD WINAPI LoadLibrary1(LPVOID lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;
	HANDLE hThread = CreateThread(nullptr, 0,
		(LPTHREAD_START_ROUTINE)AttachDLL1, hModule, 0, 0);
	if (hThread != nullptr)
		CloseHandle(hThread);
	return 0;
}
DWORD WINAPI LoadLibrary2(LPVOID lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;
	HANDLE hThread = CreateThread(nullptr, 0,
		(LPTHREAD_START_ROUTINE)AttachDLL2, hModule, 0, 0);
	if (hThread != nullptr)
		CloseHandle(hThread);
	return 0;
}
DWORD WINAPI LoadLibrary3(LPVOID lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;
	HANDLE hThread = CreateThread(nullptr, 0,
		(LPTHREAD_START_ROUTINE)AttachDLL3, hModule, 0, 0);
	if (hThread != nullptr)
		CloseHandle(hThread);
	return 0;
}
DWORD WINAPI LoadLibrary4(LPVOID lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;
	HANDLE hThread = CreateThread(nullptr, 0,
		(LPTHREAD_START_ROUTINE)AttachDLL4, hModule, 0, 0);
	if (hThread != nullptr)
		CloseHandle(hThread);
	return 0;
}
DWORD WINAPI LoadLibrary5(LPVOID lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;
	HANDLE hThread = CreateThread(nullptr, 0,
		(LPTHREAD_START_ROUTINE)AttachDLL5, hModule, 0, 0);
	if (hThread != nullptr)
		CloseHandle(hThread);
	return 0;
}
DWORD WINAPI LoadLibrary6(LPVOID lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;
	HANDLE hThread = CreateThread(nullptr, 0,
		(LPTHREAD_START_ROUTINE)AttachDLL6, hModule, 0, 0);
	if (hThread != nullptr)
		CloseHandle(hThread);
	return 0;
}
DWORD WINAPI LoadLibrary7(LPVOID lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;
	HANDLE hThread = CreateThread(nullptr, 0,
		(LPTHREAD_START_ROUTINE)AttachDLL7, hModule, 0, 0);
	if (hThread != nullptr)
		CloseHandle(hThread);
	return 0;
}
DWORD WINAPI LoadLibrary8(LPVOID lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;
	HANDLE hThread = CreateThread(nullptr, 0,
		(LPTHREAD_START_ROUTINE)AttachDLL8, hModule, 0, 0);
	if (hThread != nullptr)
		CloseHandle(hThread);
	return 0;
}
DWORD WINAPI LoadLibrary9(LPVOID lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;
	HANDLE hThread = CreateThread(nullptr, 0,
		(LPTHREAD_START_ROUTINE)AttachDLL9, hModule, 0, 0);
	if (hThread != nullptr)
		CloseHandle(hThread);
	return 0;
}
DWORD WINAPI LoadLibrary10(LPVOID lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;
	HANDLE hThread = CreateThread(nullptr, 0,
		(LPTHREAD_START_ROUTINE)AttachDLL10, hModule, 0, 0);
	if (hThread != nullptr)
		CloseHandle(hThread);
	return 0;
}

DWORD WINAPI MaybeSoMaybeNo(LPVOID lpParam)
{
	HMODULE hModule = (HMODULE)lpParam;
	HANDLE hThread = CreateThread(nullptr, 0,
		(LPTHREAD_START_ROUTINE)Hook_Failure, hModule, 0, 0);
	if (hThread != nullptr)
		CloseHandle(hThread);
	return 0;
}


void How_Many_Times_Do_We_Have_To_Teach_You_This_Lesson_Old_Man() {
	if (D0) {
		LoadLibrary0(nullptr);
	}
	if (D1) {
		LoadLibrary1(nullptr);
	}
	if (D2) {
		LoadLibrary2(nullptr);
	}
	if (D3) {
		LoadLibrary3(nullptr);
	}
	if (D4) {
		LoadLibrary4(nullptr);
	}
	if (D5) {
		LoadLibrary5(nullptr);
	}
	if (D6) {
		LoadLibrary6(nullptr);
	}
	if (D7) {
		LoadLibrary7(nullptr);
	}
	if (D8) {
		LoadLibrary8(nullptr);
	}
	if (D9) {
		LoadLibrary9(nullptr);
	}
	if (D10) {
		LoadLibrary10(nullptr);
	}

}

void DidAnyLibrariesLoad() {
	if (Global::Library0) {
		std::wstring DLLName = Convertion(Global::DLL0);
		std::wstring LoadLib0 = DLLName + L" | Timer: " + std::to_wstring(Global::Timer0) + L"s\n";
		LoadedLibraries += LoadLib0;
		LibraryCt += 1;
	}
	if (Global::Library1) {
		std::wstring DLLName = Convertion(Global::DLL1);
		std::wstring LoadLib1 = DLLName + L" | Timer: " + std::to_wstring(Global::Timer1) + L"s\n";
		LoadedLibraries += LoadLib1;
		LibraryCt += 1;
	}
	if (Global::Library2) {
		std::wstring DLLName = Convertion(Global::DLL2);
		std::wstring LoadLib2 = DLLName + L" | Timer: " + std::to_wstring(Global::Timer2) + L"s\n";
		LoadedLibraries += LoadLib2;
		LibraryCt += 1;
	}
	if (Global::Library3) {
		std::wstring DLLName = Convertion(Global::DLL3);
		std::wstring LoadLib3 = DLLName + L" | Timer: " + std::to_wstring(Global::Timer3) + L"s\n";
		LoadedLibraries += LoadLib3;
		LibraryCt += 1;
	}
	if (Global::Library4) {
		std::wstring DLLName = Convertion(Global::DLL4);
		std::wstring LoadLib4 = DLLName + L" | Timer: " + std::to_wstring(Global::Timer4) + L"s\n";
		LoadedLibraries += LoadLib4;
		LibraryCt += 1;
	}
	if (Global::Library5) {
		std::wstring DLLName = Convertion(Global::DLL5);
		std::wstring LoadLib5 = DLLName + L" | Timer: " + std::to_wstring(Global::Timer5) + L"s\n";
		LoadedLibraries += LoadLib5;
		LibraryCt += 1;
	}
	if (Global::Library6) {
		std::wstring DLLName = Convertion(Global::DLL6);
		std::wstring LoadLib6 = DLLName + L" | Timer: " + std::to_wstring(Global::Timer6) + L"s\n";
		LoadedLibraries += LoadLib6;
		LibraryCt += 1;
	}
	if (Global::Library7) {
		std::wstring DLLName = Convertion(Global::DLL7);
		std::wstring LoadLib7 = DLLName + L" | Timer: " + std::to_wstring(Global::Timer7) + L"s\n";
		LoadedLibraries += LoadLib7;
		LibraryCt += 1;
	}
	if (Global::Library8) {
		std::wstring DLLName = Convertion(Global::DLL8);
		std::wstring LoadLib8 = DLLName + L" | Timer: " + std::to_wstring(Global::Timer8) + L"s\n";
		LoadedLibraries += LoadLib8;
		LibraryCt += 1;
	}
	if (Global::Library9) {
		std::wstring DLLName = Convertion(Global::DLL9);
		std::wstring LoadLib9 = DLLName + L" | Timer: " + std::to_wstring(Global::Timer9) + L"s\n";
		LoadedLibraries += LoadLib9;
		LibraryCt += 1;
	}
	if (Global::Library10) {
		std::wstring DLLName = Convertion(Global::DLL10);
		std::wstring LoadLib10 = DLLName + L" | Timer: " + std::to_wstring(Global::Timer10) + L"s\n";
		LoadedLibraries += LoadLib10;
		LibraryCt += 1;
	}
}

//Show an error message and give the choice whether to start the game without hooking, or close and retry, or even file a lawsui- I mean an issue request.
void Hook_Failure() {

	Sleep(Hourglass * 1000); // It waits as long as the highest timer value + an additional second to allow the boolean values to change.
	DidAnyLibrariesLoad(); // It checks to see if any additional DLLs managed to load anyways.

	std::wstring Message1 = L"'dxgi.dll' failed to hook the game, this is due to:\n\n1: You're not using a supported Unreal Engine (4.0-4.27.2)\n\n2: This game has Steam's DRM, try to bypass this using Steamless or Goldberg Emulator.\n(I personally use Steam-Auto-Cracker because I don't pirate much)\n\n3: nÑ£ç†®π∫∆œ≈∫˙√©¥Ωø√ç∂˙´¬åß∂ƒ¨ˆˇœ´∆†¨¥∫∂˙ø¬˙∂˚´µ±˙¢∂¨∑∆√¬\n\n";
	std::wstring OCD = L"\nHOWEVER, this DLL has still been loaded into the game successfully:\n\n"; // OCD is a simple variable to decide whether or not multiple DLLs have been loaded and changes the message accordingly, and something my OCD bothered me to implement/fix.

	if (LibraryCt > 1) { // If there are more than 1 DLLs that have successfully loaded, the text output will change accordingly.
		OCD = L"\nHOWEVER, these DLLs have still been loaded into the game successfully:\n\n";
	}

	std::wstring Message2 = OCD + LoadedLibraries + L"\n\nClick YES to open the GitHub repository and file a lawsui- I mean file a issue request. \n\nClick NO if you want to play the game without the DLL loader working. \n\nClick CANCEL to close the game.";

	std::wstring FullMessage = Message1; // Simple variable to decide how much information is output, if no DLLs successfully load then it will only output Message1. 
	if (LibraryCt > 0) { // But if any DLLs successfully manage to load, then it will output Message2, which includes OCD, LoadedLibraries, and the message describing the current options you have.
		FullMessage = Message1 + Message2;
	}
	const int result = MessageBox(NULL, FullMessage.c_str(), L"! Error !", MB_YESNOCANCEL | MB_SYSTEMMODAL);

	switch (result)
	{
	case IDYES:
		ShellExecute(0, 0, L"https://github.com/Aeyth8/UE4-Librarian/issues/new", 0, 0, SW_SHOW);
		break;
	case IDNO:
		break;
	case IDCANCEL:
		abort();
		break;
	}
	
}


void Hooks()
{

	SigScanner_LooseFileLoader sigScanner_LooseFileLoader; //Create sigscanner object for the loose file loader


	//PUBLIC LOOSE FILE HOOKS

	const size_t addr_FindFileInPakFiles = sigScanner_LooseFileLoader.Get_addr_Pakfile__Find(); // address of FindFileInPakFiles
	char* FindFileInPakFiles = exe_base + addr_FindFileInPakFiles;

	// FPakPlatformFile::IsNonPakFilenameAllowed(FString const &)
	const size_t addr_IsNonPakFilenameAllowed = sigScanner_LooseFileLoader.Get_IsNonPakFileNameAllowedAddr(); // address of IsNonPakFilenameAllowed;
	char* IsNonPakFilenameAllowed = exe_base + addr_IsNonPakFilenameAllowed; // exe_base + addr_IsNonPakFilenameAllowed;

	//Create a status profile for whatever is implemented in the sig scanning profile so we can select the proper function hook for what we want to do.
	LooseFileLoadingHooks::LOOSEFILE_STATUS LooseFileStatus = LooseFileLoadingHooks::CheckLooseFileStatus(
		addr_FindFileInPakFiles, 
		addr_IsNonPakFilenameAllowed, 
		sigScanner_LooseFileLoader.p_Find_File_In_PakFile_Version,
		sigScanner_LooseFileLoader.p_bUses_IsNonPakFileNameAllowed
	); //Get LOOSEFILE STATUS

	switch (LooseFileStatus) //DO WHATEVER THE STATUS TELLS US TO DO
	{
	case LooseFileLoadingHooks::LOOSEFILE_VER1_USESISALLOWEDTRUE:
		if (MH_CreateHook((void*)FindFileInPakFiles, 
			LooseFileLoadingHooks::FindFileInPakFiles_Hook, 
			(LPVOID*)&LooseFileLoadingHooks::FindFileInPakFiles_orig) == MH_OK
			)// Create our PakFile__Find_hook with minhook
		{
			MH_EnableHook((void*)FindFileInPakFiles); // Enable our PakFile__Find_hook
		}

		if (MH_CreateHook((void*)IsNonPakFilenameAllowed, 
			LooseFileLoadingHooks::IsNonPakFilenameAllowed_Hook, 
			(LPVOID*)&LooseFileLoadingHooks::PakFile__IsNonPakFilenameAllowed_orig) == MH_OK
			)// Create our IsNonPakFilenameAllowed_hook with minhook
		{
			MH_EnableHook((void*)IsNonPakFilenameAllowed); // Enable our IsNonPakFilenameAllowed_hook
		}
		break;
	case LooseFileLoadingHooks::LOOSEFILE_VER2_USESISALLOWEDTRUE:
		if (MH_CreateHook((void*)FindFileInPakFiles, 
			LooseFileLoadingHooks::FindFileInPakFiles_2_Hook, 
			(LPVOID*)&LooseFileLoadingHooks::FindFileInPakFiles_2_orig) == MH_OK
			)// Create our PakFile__Find_2_hook with minhook
		{
			MH_EnableHook((void*)FindFileInPakFiles); // Enable our PakFile__Find_2_hook
		}


		if (MH_CreateHook((void*)IsNonPakFilenameAllowed, 
			LooseFileLoadingHooks::IsNonPakFilenameAllowed_Hook, 
			(LPVOID*)&LooseFileLoadingHooks::PakFile__IsNonPakFilenameAllowed_orig) == MH_OK
			)// Create our IsNonPakFilenameAllowed_hook with minhook
		{
			MH_EnableHook((void*)IsNonPakFilenameAllowed); // Enable our IsNonPakFilenameAllowed_hook
		}
		break;
	case LooseFileLoadingHooks::LOOSEFILE_VER1_USESISALLOWEDFALSE:
		if (MH_CreateHook(
			(void*)FindFileInPakFiles, 
			LooseFileLoadingHooks::FindFileInPakFiles_Hook, 
			(LPVOID*)&LooseFileLoadingHooks::FindFileInPakFiles_orig) == MH_OK
			)// Create our PakFile__Find_hook with minhook
		{
			MH_EnableHook((void*)FindFileInPakFiles); // Enable our PakFile__Find_hook
		}
		break;
	case LooseFileLoadingHooks::LOOSEFILE_VER2_USESISALLOWEDFALSE:
		if (MH_CreateHook((void*)FindFileInPakFiles, 
			LooseFileLoadingHooks::FindFileInPakFiles_2_Hook, 
			(LPVOID*)&LooseFileLoadingHooks::FindFileInPakFiles_2_orig) == MH_OK)// Create our PakFile__Find_2_hook with minhook
		{
			MH_EnableHook((void*)FindFileInPakFiles); // Enable our PakFile__Find_2_hook
		}
		break;
	case LooseFileLoadingHooks::LOOSEFILE_UNKNOWN:
		CreateThread(NULL, 0, &RevertToStock, NULL, 0, NULL); // Show a revert to stock message and start the game without hooking anything.
		break;
	default:
		CreateThread(NULL, 0, &RevertToStock, NULL, 0, NULL); // Show a revert to stock message and start the game without hooking anything.
		break;
	}

}


void Initiate_Hooks()
{
	exe_base = (char*)GetModuleHandleA(NULL); //Get exe base

	if (!exe_base)
	{
		return;
	}

	MH_Initialize(); // Initialize MinHook
	Hooks(); //Run HookPakFile function
}


