#include <iostream>
#include <windows.h>

using namespace std;

int main() {

	DWORD pid;
	cout << "PID: ";
	cin >> pid;

	//HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	HANDLE process = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE,
		pid);

	if (!process) {
		cout << "couldn't open process \n";
		return 1;
	}
	
	//MEMORY_BASIC_INFORMATION mbi{};

	uintptr_t address = 0;

	cout << "enter address (hex): 0x";
	cin >> hex >> address;
	cout << "address entered: 0x" << hex << address << '\n';

	//here
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
		cout << '\n' << "bytes in hex" << '\n';
		for (SIZE_T i = 0; i < bytesread; i++) {
			printf("%02X ", buffer[i]);
		}
		cout << '\n' << "bytes to int" << '\n';
		long long accu = 0;
		for (SIZE_T i = 0; i < bytesread; i++) {
			int j = buffer[i]; //stoi(buffer[i], 0, 16);
			cout << j << ' ';
			accu += j;
		}
		cout << "summed int: " << accu;
		cout << '\n' << "bytes to char" << '\n';
		for (SIZE_T i = 0; i < bytesread; i++) {
			int j = buffer[i]; //stoi(buffer[i], 0, 16);
			char c = j; //stoi(buffer[i], 0, 16);
			cout << c << ' ';
		}
	}


	CloseHandle(process);
	
	return 0;
}
