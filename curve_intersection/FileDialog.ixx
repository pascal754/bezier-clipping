// September 26, 2022
// functions from stackoverflow
// launch open and save file dialog

module;

#include "pch.h"
#define NOMINMAX
#include <Windows.h>
#include <ShObjidl.h>

export module FileDialog;

import std;

std::string wchar_to_UTF8(const wchar_t* in)
{
    std::string out;
    unsigned int codepoint = 0;
    for (; *in != 0; ++in)
    {
        if (*in >= 0xd800 && *in <= 0xdbff)
            codepoint = ((*in - 0xd800) << 10) + 0x10000;
        else
        {
            if (*in >= 0xdc00 && *in <= 0xdfff)
                codepoint |= *in - 0xdc00;
            else
                codepoint = *in;

            if (codepoint <= 0x7f)
                out.append(1, static_cast<char>(codepoint));
            else if (codepoint <= 0x7ff)
            {
                out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
                out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
            }
            else if (codepoint <= 0xffff)
            {
                out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
                out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
            }
            else
            {
                out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
                out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
                out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
            }
            codepoint = 0;
        }
    }
    return out;
}

export bool launchFileDialog(char mode, std::string& sFilePath, std::string& sSelectedFile)
{
    //  CREATE FILE OBJECT INSTANCE
    HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(f_SysHr))
        return false;

    // CREATE FileOpenDialog OBJECT
    IFileOpenDialog* f_FileSystem;
    if (mode == 'o')
        f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
    else if (mode == 's')
        f_SysHr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&f_FileSystem));
    else
        return false;

    if (FAILED(f_SysHr))
    {
        CoUninitialize();
        return false;
    }

    //  SHOW OPEN FILE DIALOG WINDOW
    f_SysHr = f_FileSystem->Show(NULL);
    if (FAILED(f_SysHr))
    {
        f_FileSystem->Release();
        CoUninitialize();
        return false;
    }

    //  RETRIEVE FILE NAME FROM THE SELECTED ITEM
    IShellItem* f_Files;
    f_SysHr = f_FileSystem->GetResult(&f_Files);
    if (FAILED(f_SysHr))
    {
        f_FileSystem->Release();
        CoUninitialize();
        return false;
    }

    //  STORE AND CONVERT THE FILE NAME
    PWSTR f_Path;
    f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
    if (FAILED(f_SysHr))
    {
        f_Files->Release();
        f_FileSystem->Release();
        CoUninitialize();
        return false;
    }

    //  FORMAT AND STORE THE FILE PATH
    //std::wstring path(f_Path);
    //std::string c(path.begin(), path.end());

    sFilePath = wchar_to_UTF8(f_Path);

    //  FORMAT STRING FOR EXECUTABLE NAME
    const size_t slash = sFilePath.find_last_of("/\\");
    sSelectedFile = sFilePath.substr(slash + 1);

    //  SUCCESS, CLEAN UP
    CoTaskMemFree(f_Path);
    f_Files->Release();
    f_FileSystem->Release();
    CoUninitialize();
    return true;
}
