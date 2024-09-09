#include "Threads.h"
#include "../Other/global.h"
#include "../Debugger/Debugger.h"

typedef void(WINAPI* StartThread)();
HMODULE Bonkers = NULL;
WCHAR DLLPath[MAX_PATH] = { 0 };

std::wstring Failed = L" has failed to load:\n\n\n Press 'OK' to play the game without this DLL.\n\n Press 'CANCEL' to close the game.";

void Thread_Creator() {
	int Timers[] = { Timer0, Timer1, Timer2, Timer3, Timer4, Timer5, Timer6, Timer7, Timer8, Timer9, Timer10 };
	std::string DLLs[] = { DLL0, DLL1, DLL2, DLL3, DLL4, DLL5, DLL6, DLL7, DLL8, DLL9, DLL10 };
	bool Flags[] = { D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10 };
	for (int i = 0; i < 11; ++i) {

		if (Flags[i]) {
			DebugLog("DLL" + std::to_string(i), "Creating thread for "+ DLLs[i] + " | Timer: " + std::to_string(Timers[i]) + "s");
			LoadLibraries(i);
		}
		else {
			DebugLog("DLL" + std::to_string(i), "Skipping thread creation for " + DLLs[i] + " | Timer: " + std::to_string(Timers[i]) + "s");
		}
	}
}

void DidAnyLibrariesLoad() { // Loops through 11 iterations comparing a couple of arrays, parsing a string message and adding it into the LoadedLibraries string.
	std::string DLLs[] = { DLL0, DLL1, DLL2, DLL3, DLL4, DLL5, DLL6, DLL7, DLL8, DLL9, DLL10 };
	int Timers[] = { Timer0, Timer1, Timer2, Timer3, Timer4, Timer5, Timer6, Timer7, Timer8, Timer9, Timer10 };
	bool Libraries[] = { Library0, Library1, Library2, Library3, Library4, Library5, Library6, Library7, Library8, Library9, Library10 };

	for (int i = 0; i < 11; ++i) { // Loops through 11 times, the variable 'i' is initially defined as 0, and through each loop gains 1.
		if (Libraries[i]) { // Checks the libraries array for the boolean using the number of 'i', for example if 'i = 5' then it would be seeing if (Global::Library5) is true.
			std::wstring LoadLib = Convertion(DLLs[i]) + L" | DLL" + std::to_wstring(i) + L" | Timer: " + std::to_wstring(Timers[i]) + L"s\n"; // Parses a wide string, calling Convertion to convert the DLLs string into a wide string based on 'i', then it adds a string for format and adds the Timers integer converted to a wide string.  
			LoadedLibraries += LoadLib; // Adds the parsed wide string message to LoadedLibraries, which in itself is a string, this loop will add each message for however many DLLs are successfully loaded.
			LibraryCt += 1; // Adds 1 to LibraryCt which means Library Count.
		}
	}
}

DWORD WINAPI LoadLibraries(int Lib) {
	StartThread Threads[] = { AttachDLL0, AttachDLL1, AttachDLL2, AttachDLL3, AttachDLL4, AttachDLL5, AttachDLL6, AttachDLL7, AttachDLL8, AttachDLL9, AttachDLL10 };

	if (!(Lib >= 0 && Lib <= 10)) {
		return 1;
	}

	HANDLE hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Threads[Lib], nullptr, 0, 0);
	if (hThread != nullptr)
		CloseHandle(hThread);
	//DebugLog("LoadLibraries", "Creating new thread for DLL" + std::to_string(Lib) + " | Timer: " + std::to_string(Timers[Lib]) + "s");

	return 0;
}

// These are all separate threads to ensure that they are all properly timed and only work within themselves without affecting the other threads
void AttachDLL0() {
	std::wstring DLLName = Convertion(DLL0); //Converts the DLLName from a normal string to a wide string using the 'Convertion' function
	Sleep(Timer0 * 1000); // Sleeps for the amount of seconds you set in the timer via DList.ini, if you didn't set a timer then it will not sleep at all and will inject immediately, which is likely to cause a crash or fail entirely. Don't set a timer at 0, I'd recommend 4 seconds minimum.
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1); // Something something appends DLLName to the path
	Bonkers = LoadLibraryW(DLLPath); // Bonkers is a bool, and it will attempt to load the library (inject the custom DLL)
	if (!Bonkers) { // If the Bonkers bool ends up returning false, then it runs this error code, which pops up giving you the choice to continue without the DLL or to close the game.
		Attach_Failed(DLLName);
	}
	if (Bonkers) {
		Library0 = true;
		DebugLog("DLL0", Revertion(DLLName) + " has successfully loaded into the process.");
	}
}

void AttachDLL1() {
	std::wstring DLLName = Convertion(DLL1);
	Sleep(Timer1 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		Attach_Failed(DLLName);
	}
	if (Bonkers) {
		Library1 = true;
		DebugLog("DLL1", Revertion(DLLName) + " has successfully loaded into the process.");
	}
}

void AttachDLL2() {
	std::wstring DLLName = Convertion(DLL2);
	Sleep(Timer2 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		Attach_Failed(DLLName);
	}
	if (Bonkers) {
		Library2 = true;
		DebugLog("DLL2", Revertion(DLLName) + " has successfully loaded into the process.");
	}
}

void AttachDLL3() {
	std::wstring DLLName = Convertion(DLL3);
	Sleep(Timer3 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		Attach_Failed(DLLName);
	}
	if (Bonkers) {
		Library3 = true;
		DebugLog("DLL3", Revertion(DLLName) + " has successfully loaded into the process.");
	}
}

void AttachDLL4() {
	std::wstring DLLName = Convertion(DLL4);
	Sleep(Timer4 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		Attach_Failed(DLLName);
	}
	if (Bonkers) {
		Library4 = true;
		DebugLog("DLL4", Revertion(DLLName) + " has successfully loaded into the process.");
	}
}

void AttachDLL5() {
	std::wstring DLLName = Convertion(DLL5);
	Sleep(Timer5 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		Attach_Failed(DLLName);
	}
	if (Bonkers) {
		Library5 = true;
		DebugLog("DLL5", Revertion(DLLName) + " has successfully loaded into the process.");
	}
}

void AttachDLL6() {
	std::wstring DLLName = Convertion(DLL6);
	Sleep(Timer6 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		Attach_Failed(DLLName);
	}
	if (Bonkers) {
		Library6 = true;
		DebugLog("DLL6", Revertion(DLLName) + " has successfully loaded into the process.");
	}
}

void AttachDLL7() {
	std::wstring DLLName = Convertion(DLL7);
	Sleep(Timer7 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		Attach_Failed(DLLName);
	}
	if (Bonkers) {
		Library7 = true;
		DebugLog("DLL7", Revertion(DLLName) + " has successfully loaded into the process.");
	}
}

void AttachDLL8() {
	std::wstring DLLName = Convertion(DLL8);
	Sleep(Timer8 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		Attach_Failed(DLLName);
	}
	if (Bonkers) {
		Library8 = true;
		DebugLog("DLL8", Revertion(DLLName) + " has successfully loaded into the process.");
	}
}

void AttachDLL9() {
	std::wstring DLLName = Convertion(DLL9);
	Sleep(Timer9 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		Attach_Failed(DLLName);
	}
	if (Bonkers) {
		Library9 = true;
		DebugLog("DLL9", Revertion(DLLName) + " has successfully loaded into the process.");
	}
}

void AttachDLL10() {
	std::wstring DLLName = Convertion(DLL10);
	Sleep(Timer10 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		Attach_Failed(DLLName);
	}
	if (Bonkers) {
		Library10 = true;
		DebugLog("DLL10", Revertion(DLLName) + " has successfully loaded into the process.");
	}
}

void Attach_Failed(const std::wstring& DLLName) {
	std::wstring DoubleTime = L"'" + DLLName + L"'" + Failed;
	const int result = MessageBox(NULL, DoubleTime.c_str(), L"! ERROR !", MB_OKCANCEL | MB_SYSTEMMODAL);
	switch (result)
	{
	case IDOK:
		break;
	case IDCANCEL:
		abort();
		break;
	}
}

//int Timers[] = { Timer0, Timer1, Timer2, Timer3, Timer4, Timer5, Timer6, Timer7, Timer8, Timer9, Timer10 };
//bool Libraries[] = { Library0, Library1, Library2, Library3, Library4, Library5, Library6, Library7, Library8, Library9, Library10 };
//std::string DynamicLinkLibraries[] = { DLL0, DLL1, DLL2, DLL3, DLL4, DLL5, DLL6, DLL7, DLL8, DLL9, DLL10 };
/*template <typename T>
T* AttachDLL(int& i) {
	std::wstring DLLName = Convertion(i[DLL]);
	Sleep(Timers[i] * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);



bool LoadDLL(std::wstring& DLLPath) {
	Bonkers = LoadLibraryW(DLLPath.c_str());
	if (!Bonkers) {
		return false;
	}

	return true;
}



}

DWORD WINAPI LoadLibraries(int Lib) {
	//StartThread Threads[] = { AttachDLL0, AttachDLL1, AttachDLL2, AttachDLL3, AttachDLL4, AttachDLL5, AttachDLL6, AttachDLL7, AttachDLL8, AttachDLL9, AttachDLL10 };

	if (!(Lib >= 0 && Lib <= 10)) {
		return 1;
	}

	//HANDLE hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Threads[Lib], nullptr, 0, 0);
	//if (hThread != nullptr)
	//	CloseHandle(hThread);
	DebugLog("LoadLibraries", "Creating new thread for DLL" + std::to_string(Lib) + " | Timer: " + std::to_string(Timers[Lib]) + "s");

	return 0;
}


// These are all separate threads to ensure that they are all properly timed and only work within themselves without affecting the other threads
void AttachDLL0() {
	std::wstring DLLName = Convertion(DLL0); //Converts the DLLName from a normal string to a wide string using the 'Convertion' function
	Sleep(Timer0 * 1000); // Sleeps for the amount of seconds you set in the timer via DList.ini, if you didn't set a timer then it will not sleep at all and will inject immediately, which is likely to cause a crash or fail entirely. Don't set a timer at 0, I'd recommend 4 seconds minimum.
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1); // I'm going to be honest I have no clue what this does, I just know that I need it in order to work.
	Bonkers = LoadLibraryW(DLLPath); // Bonkers is a bool, and it will attempt to load the library (inject the custom DLL)
	if (!Bonkers) { // If the Bonkers bool ends up returning false, then it runs this error code, which pops up giving you the choice to continue without the DLL or to close the game.
		std::wstring DoubleTime = L"'" + DLLName + L"'" + Failed; // DoubleTime is the variable to make this entire message into just one variable, which is placed into the MessageBox, since MessageBoxes will not be able to do this themselves.
		const int result = MessageBox(NULL, DoubleTime.c_str(), L"! ERROR !", MB_OKCANCEL | MB_SYSTEMMODAL);
		switch (result)
		{
		case IDOK:
			break;
		case IDCANCEL:
			abort();
			break;
		}
	}
	if (Bonkers) {
		Library0 = true;

	}
}
void AttachDLL1() {
	std::wstring DLLName = Convertion(DLL1);
	Sleep(Timer1 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		std::wstring DoubleTime = L"'" + DLLName + L"'" + Failed;
		const int result = MessageBox(NULL, DoubleTime.c_str(), L"!ERROR!", MB_OKCANCEL | MB_SYSTEMMODAL);
		switch (result)
		{
		case IDOK:
			break;
		case IDCANCEL:
			abort();
			break;
		}
	}
	if (Bonkers) {
		Library1 = true;

	}
}
void AttachDLL2() {
	std::wstring DLLName = Convertion(DLL2);
	Sleep(Timer2 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		std::wstring DoubleTime = L"'" + DLLName + L"'" + Failed;
		const int result = MessageBox(NULL, DoubleTime.c_str(), L"!ERROR!", MB_OKCANCEL | MB_SYSTEMMODAL);
		switch (result)
		{
		case IDOK:
			break;
		case IDCANCEL:
			abort();
			break;
		}
	}
	if (Bonkers) {
		Library2 = true;

	}
}
void AttachDLL3() {
	std::wstring DLLName = Convertion(DLL3);
	Sleep(Timer3 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		std::wstring DoubleTime = L"'" + DLLName + L"'" + Failed;
		const int result = MessageBox(NULL, DoubleTime.c_str(), L"!ERROR!", MB_OKCANCEL | MB_SYSTEMMODAL);
		switch (result)
		{
		case IDOK:
			break;
		case IDCANCEL:
			abort();
			break;
		}
	}
	if (Bonkers) {
		Library3 = true;

	}
}
void AttachDLL4() {
	std::wstring DLLName = Convertion(DLL4);
	Sleep(Timer4 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		std::wstring DoubleTime = L"'" + DLLName + L"'" + Failed;
		const int result = MessageBox(NULL, DoubleTime.c_str(), L"!ERROR!", MB_OKCANCEL | MB_SYSTEMMODAL);
		switch (result)
		{
		case IDOK:
			break;
		case IDCANCEL:
			abort();
			break;
		}
	}
	if (Bonkers) {
		Library4 = true;

	}
}
void AttachDLL5() {
	std::wstring DLLName = Convertion(DLL5);
	Sleep(Timer5 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		std::wstring DoubleTime = L"'" + DLLName + L"'" + Failed;
		const int result = MessageBox(NULL, DoubleTime.c_str(), L"!ERROR!", MB_OKCANCEL | MB_SYSTEMMODAL);
		switch (result)
		{
		case IDOK:
			break;
		case IDCANCEL:
			abort();
			break;
		}
	}
	if (Bonkers) {
		Library5 = true;

	}
}
void AttachDLL6() {
	std::wstring DLLName = Convertion(DLL6);
	Sleep(Timer6 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		std::wstring DoubleTime = L"'" + DLLName + L"'" + Failed;
		const int result = MessageBox(NULL, DoubleTime.c_str(), L"!ERROR!", MB_OKCANCEL | MB_SYSTEMMODAL);
		switch (result)
		{
		case IDOK:
			break;
		case IDCANCEL:
			abort();
			break;
		}
	}
	if (Bonkers) {
		Library6 = true;

	}
}
void AttachDLL7() {
	std::wstring DLLName = Convertion(DLL7);
	Sleep(Timer7 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		std::wstring DoubleTime = L"'" + DLLName + L"'" + Failed;
		const int result = MessageBox(NULL, DoubleTime.c_str(), L"!ERROR!", MB_OKCANCEL | MB_SYSTEMMODAL);
		switch (result)
		{
		case IDOK:
			break;
		case IDCANCEL:
			abort();
			break;
		}
	}
	if (Bonkers) {
		Library7 = true;

	}
}
void AttachDLL8() {
	std::wstring DLLName = Convertion(DLL8);
	Sleep(Timer8 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		std::wstring DoubleTime = L"'" + DLLName + L"'" + Failed;
		const int result = MessageBox(NULL, DoubleTime.c_str(), L"!ERROR!", MB_OKCANCEL | MB_SYSTEMMODAL);
		switch (result)
		{
		case IDOK:
			break;
		case IDCANCEL:
			abort();
			break;
		}
	}
	if (Bonkers) {
		Library8 = true;

	}
}
void AttachDLL9() {
	std::wstring DLLName = Convertion(DLL9);
	Sleep(Timer9 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		std::wstring DoubleTime = L"'" + DLLName + L"'" + Failed;
		const int result = MessageBox(NULL, DoubleTime.c_str(), L"!ERROR!", MB_OKCANCEL | MB_SYSTEMMODAL);
		switch (result)
		{
		case IDOK:
			break;
		case IDCANCEL:
			abort();
			break;
		}
	}
	if (Bonkers) {
		Library9 = true;

	}
}
void AttachDLL10() {
	std::wstring DLLName = Convertion(DLL10);
	Sleep(Timer10 * 1000);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		std::wstring DoubleTime = L"'" + DLLName + L"'" + Failed;
		const int result = MessageBox(NULL, DoubleTime.c_str(), L"!ERROR!", MB_OKCANCEL | MB_SYSTEMMODAL);
		switch (result)
		{
		case IDOK:
			break;
		case IDCANCEL:
			abort();
			break;
		}
	}
	if (Bonkers) {
		Library10 = true;

	}
}










*/
/*
void Threads(int Lib) {
	if (!(Lib >= 0 && Lib <= 10)) {
		return;
	}

	std::thread(AttachDLL, Lib).detach();
	DebugLog("LoadLibraries", "Creating new thread for DLL" + std::to_string(Lib) + " | Timer: " + std::to_string(Timers[Lib]) + "s");
}
*/
/*
void AttachDLL(int Lib) {
	std::wstring DLLName = Convertion(DynamicLinkLibraries[Lib]);
	wcsncpy_s(DLLPath, DLLName.c_str(), _countof(DLLPath) - 1);
	Sleep(Timers[Lib] * 1000);
	Bonkers = LoadLibraryW(DLLPath);
	if (!Bonkers) {
		Attach_Failed(DLLName);
	}
	if (Bonkers) {
		Libraries[Lib] = true;
	}
}*/