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

#include "shellapi.h"
#include "iostream"
#include "algorithm"
#include "./Other/global.h"
#include "./Parsing/parsing.h"
#include "./Debugger/Debugger.h"
#include "./Threads/Threads.h"

//------------------------ END OF INCLUDES


// Setup for the dxgi Proxy
EXTERN_C IMAGE_DOS_HEADER __ImageBase; // We might need this some day. Don't remove.
HMODULE ourModule = 0; // Declare our "extern HMODULE ourModule" from proxy.cpp inside dllmain, so we can pass hModule to Proxy_Attach();

//Function Defs -------------------->
DWORD WINAPI RevertToStock(LPVOID lpParam);
void Hooks();
void Initiate_Hooks();
void Hook_Failure();
//------------------------ END OF Function Defs


/*

/////// Process Map For UE4-Librarian \\\\\\\

DLL Entry -> Initialize(); -> Set_Vars(); -> Thread_Creator(); -> 11 Threads [ AttachDLL0 - AttachDLL10 ]
dllmain.cpp  parsing.cpp      parsing.cpp    Threads.cpp          Threads.cpp

                  |               |
				 \|/             \|/
				  v               v
			  InitLog();        Hourglass = std::max [ Highest Timer ] + 1
			  Debugger.cpp



/////// Error Handler \\\\\\\

RevertToStock -> CreateThread(Hook_Failure) -> Sleep(Hourglass*1000);
                                                  
-_-_-_-_-_-_-_-_- dllmain.cpp -_-_-_-_-_-_-_-_-_-     |
		      All located within			         \|/
													  v
											   DidAnyLibrariesLoad(); -> Parses Successfully Loaded Libraries Into Wide String

*/

char* exe_base = nullptr;


//Start Routine
BOOL APIENTRY DllMain(HMODULE hModule, int ul_reason_for_call, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);
	Initialize();
	
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		ourModule = hModule;
		
		Proxy_Attach();

		Initiate_Hooks();

	}
	if (ul_reason_for_call == DLL_PROCESS_DETACH)
		Proxy_Detach();

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


//Show an error message and give the choice whether to start the game without hooking, close and retry, or even file a lawsui- I mean an issue request.
void Hook_Failure() {

	Sleep(Hourglass * 1000); // It waits as long as the highest timer value + an additional second to allow the boolean values to change.
	DidAnyLibrariesLoad(); // It checks to see if any additional DLLs managed to load anyways.

	std::wstring Message1 = L"'dxgi.dll' failed to hook the game, this is due to:\n\n1: You're not using a supported Unreal Engine (4.0-4.27.2)\n\n2: This game has Steam's DRM, try to bypass this using Steamless or Goldberg Emulator.\n(I personally use Steam-Auto-Cracker but I don't pirate much)\n\n3: nÑ£ç†®π∫∆œ≈∫˙√©¥Ωø√ç∂˙´¬åß∂ƒ¨ˆˇœ´∆†¨¥∫∂˙ø¬˙∂˚´µ±˙¢∂¨∑∆√¬\n\n";
	
	std::wstring OCD = L"this DLL has"; // OCD is a simple variable to decide whether or not multiple DLLs have been loaded and changes the message accordingly, and something my OCD bothered me to implement/fix.
	if (LibraryCt > 1) { // If there are more than 1 DLLs that have successfully loaded, the text output will change accordingly.
		OCD = L"these DLLs have";
	}

	std::wstring Message2 = L"\nHOWEVER, " + OCD + L" still been loaded into the game successfully:\n\n" + LoadedLibraries;
	std::wstring EndLine = L"\n\nClick YES to open the GitHub repository and file a lawsui- I mean file an issue request. \n\nClick NO if you want to play the game without the special 'Mod Unlocker' hook working. \n\nClick CANCEL to close the game.";

	std::wstring FullMessage = Message1 + EndLine; // Simple variable to decide how much information is output, if no DLLs successfully load then it will only output Message1. 
	if (LibraryCt > 0) { // But if any DLLs successfully manage to load, then it will output Message2, which includes OCD, LoadedLibraries, and the message describing the current options you have.
		FullMessage = Message1 + Message2 + EndLine;
	}

	if (HookWarning) {
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


