#include <windows.h>
#include <iostream>

// Show help
void PrintHelp(char* executable) {
    std::cerr << "Usage: " << executable << " <file_path>" << std::endl;
}

int main(int argc, char* argv[]) {
   
    // Show help
    if (argc != 2) {
        PrintHelp(argv[0]);
        return 1;
    }

    const char* filePath = argv[1];

    HANDLE hFile;
    OVERLAPPED overlapped = { 0 };

    // Open a file for generic_read (other permissions are optional and can result with access denied error).
    hFile = CreateFileA(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        PrintHelp(argv[0]);
        std::cerr << "Error opening file: " << GetLastError() << std::endl;
        return 1;
    }

    // Lock the file exclusively
    if (!LockFileEx(hFile, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, MAXDWORD, MAXDWORD, &overlapped)) {
        std::cerr << "Error locking file: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    // Lock file until the user press the key
    std::cout << "File is locked. Press any key to unlock..." << std::endl;
    getchar();

    // Unlock the file
    if (!UnlockFileEx(hFile, 0, MAXDWORD, MAXDWORD, &overlapped)) {
        std::cerr << "Error unlocking file: " << GetLastError() << std::endl;
    }

    // Close the file handle
    CloseHandle(hFile);

    return 0;
}