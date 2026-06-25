#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
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

    HANDLE process = OpenProcess(PROCESS_VM_WRITE |
                                 PROCESS_VM_OPERATION,
                                 FALSE,
                                 pid);

    if (!process)
    {
        std::cout << "Couldn't open process.\n";
        return 1;
    }

    uintptr_t address;

    std::cout << "Enter variable address (hex): ";
    std::cin >> std::hex >> address;

    char u1;
    char e;
    int ed;
    std::string edi;
    bool te;
    std::cout << "enter what type it is(b/i/c); ";
    std::cin >> u1;
    if ((char)tolower(u1) == 'b') {
        //bool
        std::cout << "enter(t/f): ";
        std::cin >> e;
        bool value = (tolower(e) == 't');
        if (WriteProcessMemory(
            process,
            (LPVOID)address,
            &te,
            sizeof(te),
            nullptr)) {
            std::cout << "Memory edited!\n";
        } else {
            std::cout << "Write failed.\n";
        }
    } else if ((char)tolower(u1) == 'i') {
        //int
        std::cout << "enter: ";
        std::cin >> ed;
        if (WriteProcessMemory(
            process,
            (LPVOID)address,
            &ed,
            sizeof(ed),
            nullptr)) {
            std::cout << "Memory edited!\n";
        }
        else
        {
            std::cout << "Write failed.\n";
        }
    } else if ((char)tolower(u1) == 'c') {
        //string
        std::cout << "enter: ";
        std::cin >> edi;
        if (WriteProcessMemory(
            process,
            (LPVOID)address,
            edi.c_str(),
            edi.size(), // + 1,
            nullptr)) {
            std::cout << "Memory edited!\n";
        } else {
            std::cout << "Write failed.\n";
        }
    } else {
        std::cout << "idk";
        CloseHandle(process);
    }

    /*
    if (WriteProcessMemory(
            process,
            (LPVOID)address,
            &value,
            sizeof(value),
            nullptr))
    {
        std::cout << "Memory edited!\n";
    }
    else
    {
        std::cout << "Write failed.\n";
    }
    */

    CloseHandle(process);
}