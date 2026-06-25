#include <windows.h>
#include <iostream>
#include <iomanip>

int main()
{
    DWORD pid;

    std::cout << "PID: ";
    std::cin >> pid;

    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);

    if (!process)
    {
        std::cout << "Couldn't open process.\n";
        return 1;
    }

    MEMORY_BASIC_INFORMATION mbi{};

    uintptr_t address = 0;

    while (VirtualQueryEx(
        process,
        (LPCVOID)address,
        &mbi,
        sizeof(mbi)))
    {
        std::cout
            << std::hex
            << mbi.BaseAddress
            << "  Size: 0x"
            << mbi.RegionSize;

        if (mbi.State == MEM_COMMIT)
            std::cout << "  COMMITTED";

        if (/*mbi.Protect &*/ PAGE_READWRITE)
            std::cout << "  RW";

        if (/*mbi.Protect &*/ PAGE_EXECUTE_READWRITE)
            std::cout << "  RWX";

        if (PAGE_READONLY) 
            std::cout << "  R";

        if (PAGE_EXECUTE_READ)
            std::cout << "  RX";

        if (PAGE_WRITECOPY)
            std::cout << "  WC";

        if (PAGE_EXECUTE_WRITECOPY)
            std::cout << "  WCX";
        
        std::cout << '\n';

        address = (uintptr_t)mbi.BaseAddress + mbi.RegionSize;
    }
    CloseHandle(process);

}
