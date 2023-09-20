#include <Shlwapi.h>
#include <Windows.h>

using namespace System;
using namespace System::IO;
using namespace System::Diagnostics;

void ClearTempFiles(String^ folderPath) {
    try {
        // Удаление временных файлов
        array<String^>^ files = Directory::GetFiles(folderPath);

        for each (String ^ file in files) {
            try {
                File::Delete(file);
            }
            catch (...) {
            }
        }

        // Удаление подпапок
        array<String^>^ subdirectories = Directory::GetDirectories(folderPath);

        for each (String ^ subdirectory in subdirectories) {
            try {
                Directory::Delete(subdirectory, true);
            }
            catch (...) {
            }
        }

        // Удаление кэша браузера Chrome
        String^ chromeCachePath = Environment::GetFolderPath(Environment::SpecialFolder::LocalApplicationData) + "\\Google\\Chrome\\User Data\\Default\\Cache";
        if (Directory::Exists(chromeCachePath)) {
            array<String^>^ chromeCacheFiles = Directory::GetFiles(chromeCachePath);

            for each (String ^ cacheFile in chromeCacheFiles) {
                try {
                    File::Delete(cacheFile);
                }
                catch (...) {
                }
            }
        }

        // Удаление DNS кэша
        System::Diagnostics::Process::Start("ipconfig", "/flushdns");

    }
    catch (Exception^ e) {
    }
}


int ClearWindowsUpdateCache() {
    ProcessStartInfo^ psi = gcnew ProcessStartInfo("DISM.exe", "/Online /Cleanup-Image /StartComponentCleanup");
    psi->RedirectStandardOutput = true;
    psi->RedirectStandardError = true;
    psi->UseShellExecute = false;

    Process^ process = gcnew Process();
    process->StartInfo = psi;
    process->Start();

    process->WaitForExit();

    String^ output = process->StandardOutput->ReadToEnd();
    String^ error = process->StandardError->ReadToEnd();


    return 1;
}