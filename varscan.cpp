#include <windows.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    DWORD pid;
    if (argc < 2) {
        std::cout << "PID: ";
        std::cin >> pid;
    } else {
        //pid = std::stoi(argv[1]);
         try
        {
            pid = static_cast<DWORD>(std::stoul(argv[1]));
        }
        catch (...)
        {
            std::cerr << "Invalid PID.\n";
            return 1;
        }
        std::cout << "PID accepted" << '\n';
    }

    //HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    HANDLE process = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
        FALSE,
        pid);

    if (!process)
    {
        std::cout << "Couldn't open process " << pid << ".\n";
        return 1;
    }
    char u1, u2, u3, u4;
    bool skipnf = false, skipwd = false, skippbs = false,skipacf = false;
    std::cout << "skip byte(hex) form(y/n)?";
    std::cin >> u1;
    if ((char)tolower(u1) == 'y') {
            skippbs = true;
    }
    std::cout << "skip number form?(y/n)";
    std::cin >> u2;
    if ((char)tolower(u2) == 'y') {
            skipnf = true;
    }
    std::cout << "show accumulated num?(y/n)";
    std::cin >> u2;
    if ((char)tolower(u2) == 'y') {
            skipacf = true;
    }
    std::cout << "skip word form?(y/n)";
    std::cin >> u3;
    if ((char)tolower(u3) == 'y') {
            skipwd = true;
    }

    MEMORY_BASIC_INFORMATION mbi{}; //mbi is the memory 'region'

    uintptr_t address = 0;

    bool readable = false;

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

        if (mbi.Protect & PAGE_READWRITE)
            std::cout << "  RW";

        if (mbi.Protect & PAGE_EXECUTE_READWRITE)
            std::cout << "  RWX";

        if (mbi.State == MEM_COMMIT) {
            std::cout << "  COMMITTED";
            //std::vector<unsigned char> buffer(mbi.RegionSize);
            std::vector<unsigned char> buffer(512 * 1024 * 1024);
            SIZE_T bytesRead;

            if (ReadProcessMemory(
                    process,
                    mbi.BaseAddress,
                    buffer.data(),
                    mbi.RegionSize,
                    &bytesRead))
            {
                std::cout << '\n' << "Read " << bytesRead << " bytes.\n";
                if (!skippbs) {
                    for (SIZE_T i = 0; i < bytesRead; i++) {
                        printf("%02X ", buffer[i]);
                    }
                }
                //int target = 123;
                std::cout << '\n' << '\n';
                long long accu = 0;
                for (SIZE_T i = 0; i + sizeof(int) <= bytesRead; i++) {
                    int value;
                    memcpy(&value, &buffer[i], sizeof(value));
                    accu += value;
                    if (!skipnf) {
                        std::cout << value << ' ';
                    }
                    /*
                    if (value == target) {
                        uintptr_t found =
                            (uintptr_t)mbi.BaseAddress + i;

                        std::cout << "Found at 0x"
                                  << std::hex << found << '\n';
                    }
                    */
                }
                if (!skipacf) {
                    std::cout << accu;
                }
                //int target = 123;
                std::cout << '\n' << '\n';
                if (!skipwd) {
                    for (SIZE_T i = 0; i + sizeof(int) <= bytesRead; i++) {
                        char c = buffer[i];
                        std::cout << c;

                        /*
                        int value;
                        memcpy(&value, &buffer[i], sizeof(value));

                        char c = value;
                        std::cout << c;
                        */
                        /*
                        if (value == target) {
                            uintptr_t found =
                                (uintptr_t)mbi.BaseAddress + i;

                            std::cout << "Found at 0x"
                                      << std::hex << found << '\n';
                        }
                        */
                    }
                }
                std::cout << '\n' << '\n';
                // add suff idk
            }
        }

        //std::cout << '\n';
        //selection
        /*
        if (readable) {

            uintptr_t address = mbi.BaseAddress;
            bool value;

            SIZE_T bytesreadbool;

            if (ReadProcessMemory(
                    process,
                    reinterpret_cast<LPCVOID>(address),
                    &value,
                    sizeof(value),
                    &bytesreadbool)) {

                cout << "value: " << boolalpha << value << '\n';
            } else {
                cout << "read failed, error: " << GetLastError() << '\n';
            }
            
            int number;

            if (ReadProcessMemory(
                    process,
                    reinterpret_cast<LPCVOID>(address),
                    &number,
                    sizeof(number),
                    nullptr)) {
                    cout << number << '\n';
                    char c = number;
                    cout << "possible char: " << c << '\n';
            }

            unsigned char buffer[64];

            SIZE_T bytesread;

            if (ReadProcessMemory(
                    process,
                    reinterpret_cast<LPCVOID>(address),
                    &buffer,
                    sizeof(buffer),
                    &bytesread)) {
                for (SIZE_T i = 0; i < bytesread; i++) {
                    printf("%02X ", buffer[i]);
                }
                for (SIZE_T i = 0; i < bytesread; i++) {
                    int j = stoi(buffer[i], 0, 16);
                    cout << j << ' ';
                }
                for (SIZE_T i = 0; i < bytesread; i++) {
                    char c = stoi(buffer[i], 0, 16);
                    cout << c << ' ';
                }
            }
        }
        */

        Sleep(20);
        address = (uintptr_t)mbi.BaseAddress + mbi.RegionSize;
    }

    CloseHandle(process);

}
