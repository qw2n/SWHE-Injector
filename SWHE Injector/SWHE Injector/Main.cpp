#include <Windows.h>
#include <iostream>

// made by binarycrypt | Discord: https://discord.gg/pY6nDej76m

int main()
{
    const char* Dll_path = "DllWithHook.dll";
    const char* ProcessName = "notepad";

    HWND hwnd = FindWindowA(ProcessName, 0);
    if (!hwnd) {
        std::cout << "[-] Failed to find window!\n";
        
        return EXIT_FAILURE;
    }

    DWORD ProcessId = 0;
    DWORD ThreadId = GetWindowThreadProcessId(hwnd, &ProcessId);
    if (!ProcessId || !ThreadId) {
        std::cout << "[-] Failed to find process id or thread id!\n";
        
        return EXIT_FAILURE;
    }

    std::cout << "[+] ThreadId: " << ThreadId << std::endl;
    std::cout << "[+] ProcessId: " << ProcessId << std::endl;

    HMODULE Dll = LoadLibraryEx(Dll_path, 0, DONT_RESOLVE_DLL_REFERENCES);
    if (!Dll) {
        std::cout << "[-] Failed to load dll: '" << Dll_path << "' | " << GetLastError() << std::endl;
        
        return EXIT_FAILURE;
    }

    HOOKPROC HookAddress = (HOOKPROC)GetProcAddress(Dll, "NextHook");
    if (!HookAddress) {
        std::cout << "[-] Failed to find 'NextHook' in dll: '" << Dll_path << "' | " << GetLastError() << std::endl;
        
        return EXIT_FAILURE;
    }

    std::cout << "[+] HookAddress: 0x" << std::hex << HookAddress << std::endl;

    HHOOK HandleHook = SetWindowsHookEx(
        WH_GETMESSAGE,
        HookAddress,
        Dll,
        ThreadId
    );

    if (!HandleHook) {
        std::cout << "[-] Failed to SetWindowsHookEx!\n";

        return EXIT_FAILURE;
    }

    std::cout << "[+] Hook: 0x" << std::hex << HandleHook << std::endl;

    PostThreadMessage(ThreadId, WM_NULL, 0, 0);

    std::cout << "[+] Injected!\n";

    if (HandleHook) {
        if (UnhookWindowsHookEx(HandleHook))
            std::cout << "[+] Hook successfully removed.\n";
        else
            std::cout << "[-] Failed to remove hook! Error: " << GetLastError() << std::endl;
    }

    return EXIT_SUCCESS;
}
