#include <iostream>
#include <string>
#include <windows.h>
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[]) {
    HWND minesweeperWindow = FindWindow(NULL, "Minesweeper");

    if (minesweeperWindow == NULL) {
        std::cout << "Minesweeper window not found." << std::endl;
        return 1;
    }

    DWORD processId;
    GetWindowThreadProcessId(minesweeperWindow, &processId);

    DWORD address = 0x01005340;
    std::byte memoryBuffer[324];

    HANDLE processHandle = OpenProcess(PROCESS_VM_READ, false, processId);

    if (processHandle == NULL || !ReadProcessMemory(processHandle, (LPCVOID)address, memoryBuffer, 323, NULL)) {
        CloseHandle(processHandle);
        return 1;
    }

    bool write = false;

    for (int i = 32; i < 320; ++i) {
        auto n = static_cast<int>(memoryBuffer[i]);

        if (n == 16) {
            write = !write;
            if(write)
                std::cout << "\n";
        }

        if (!write) continue;
        if (n == 0x10) continue;

        std::string temp;

        if (n != 0x8f) temp = "0";
        else temp = "1";

        std::cout << temp << " ";
    }

    std::cout << std::endl;

    CloseHandle(processHandle);
    return 0;
}