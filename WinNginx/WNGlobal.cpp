#include "stdafx.h"
#include "WNGlobal.h"
WNGlobal::WNGlobal()
{
}


WNGlobal::~WNGlobal()
{
}
CString WNGlobal::GetFileVersion(const CString& sTargetFileName)
{
	DWORD nInfoSize = 0, dwHandle = 0;
	nInfoSize = GetFileVersionInfoSize(sTargetFileName, &dwHandle);
	if (nInfoSize == 0)
	{
		return _T("");
	}

	char* pInfoBuf = new char[nInfoSize];
	GetFileVersionInfo(sTargetFileName, 0, nInfoSize, pInfoBuf);

	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	}*pTranslate;

	UINT cbTranslate = 0;
	VerQueryValue(pInfoBuf, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&pTranslate, &cbTranslate);


	CString version;
	// Read the file description for each language and code page.
	for (size_t i = 0; i < (cbTranslate / sizeof(struct LANGANDCODEPAGE)); i++)
	{
		WCHAR SubBlock[256] = { 0 };
		wsprintf(SubBlock,
			TEXT("\\StringFileInfo\\%04x%04x\\FileVersion"),
			pTranslate[i].wLanguage,
			pTranslate[i].wCodePage);

		WCHAR* pVersion = NULL;
		UINT nBytes = 0;
		VerQueryValue(pInfoBuf, SubBlock, (LPVOID*)&pVersion, &nBytes);
		version = pVersion;
		break;
	}

	delete pInfoBuf;

	return version;

}
BOOL WNGlobal::IsFileExist(const CString& csFile) 
{ 
	DWORD dwAttrib = GetFileAttributes(csFile);
	//if file is exist return true;
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY); 
}
CString WNGlobal::GetAppPath()
{
	TCHAR tszBuf[MAX_PATH] = { '\0' };
	GetModuleFileName(NULL, tszBuf, MAX_PATH);
	CString strDir, tmpDir;
	tmpDir = tszBuf;
	strDir = tmpDir.Left(tmpDir.ReverseFind('\\'));
	return strDir;
}
vector <CString> WNGlobal::SplitCString(CString strSource, CString ch)
{
	vector <CString> vecString;
	int iPos = 0;
	CString strTmp;
	strTmp = strSource.Tokenize(ch, iPos);
	while (strTmp.Trim() != _T(""))
	{
		vecString.push_back(strTmp);
		strTmp = strSource.Tokenize(ch, iPos);
	}
	return vecString;
}
void WNGlobal::GetOSVersion(CString &strOSVersion, CString &strServerVersion)
{
	CString str;
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	BOOL bOSVersionInfoEx;

	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (!(bOSVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi)))
	{
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx((OSVERSIONINFO *)&osvi);
	}

	GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");
	CString ppp;
	GetSystemInfo(&si);
	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
		{
			if (osvi.wProductType == VER_NT_WORKSTATION)
			{
				str.Format(_T("Windows 10 "));
			}
			else
			{
				str.Format(_T("Windows Server \"Longhorn\" "));
			}
		}
		if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
		{
			if (osvi.wProductType == VER_NT_WORKSTATION)
			{
				str.Format(_T("Windows 7 "));
			}
			else
			{
				str.Format(_T("Windows Server \"Longhorn\" "));
			}
		}
		if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
		{
			
			if (osvi.wProductType == VER_NT_WORKSTATION)
			{
				str.Format(_T("Windows Vista"));
			}
			else
			{
				str.Format(_T("Windows Server \"Longhorn\" "));
			}
		}
		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
		{
			if (GetSystemMetrics(SM_SERVERR2))
			{
				str.Format(_T("Microsoft Windows Server 2003 \"R2\" "));
			}
			else if (osvi.wProductType == VER_NT_WORKSTATION &&
				si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
			{
				str.Format(_T("Microsoft Windows XP Professional x64 Edition "));
			}
			else
			{
				str.Format(_T("Microsoft Windows Server 2003, "));
			}
		}

		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
		{
			str.Format(_T("Microsoft Windows XP "));
		}

		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
		{
			str.Format(_T("Microsoft Windows 2000 "));
		}

		if (osvi.dwMajorVersion <= 4)
		{
			str.Format(_T("Microsoft Windows NT "));
		}

		if (bOSVersionInfoEx)
		{
			//将Server Pack版本保存
			strServerVersion.Format(_T("Service Pack %d"), osvi.wServicePackMajor);

			//Test for the workstation type.
			if (osvi.wProductType == VER_NT_WORKSTATION &&
				si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_AMD64)
			{
				if (osvi.dwMajorVersion == 4)
					str = str + _T("Workstation 4.0");
				else if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
					str = str + _T("Home Edition");
				else
					str = str + _T("Professional");
			}
			else if (osvi.wProductType == VER_NT_SERVER ||
				osvi.wProductType == VER_NT_DOMAIN_CONTROLLER)//Test for the server type.
			{
				if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
				{
					if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
					{
						if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
							str = str + _T("Datacenter Edition for Itanium-based Systems");
						else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
							str = str + _T("Enterprise Edition for Itanium-based Systems");
					}
					else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
					{
						if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
							str = str + _T("Datacenter x64 Edition ");
						else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
							str = str + _T("Enterprise x64 Edition ");
						else
							str = str + _T("Standard x64 Edition ");
					}
					else
					{
						if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
							str = str + _T("Datacenter Edition ");
						else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
							str = str + _T("Enterprise Edition ");
						else if (osvi.wSuiteMask & VER_SUITE_BLADE)
							str = str + _T("Web Edition ");
						else str = str + _T("Standard Edition ");
					}
				}
				else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
				{
					if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
						str = str + _T("Datacenter Server ");
					else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
						str = str + _T("Advanced Server ");
					else str = str + _T("Server ");
				}
				else
				{
					if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
						str = str + _T("Server 4.0, Enterprise Edition ");
					else str = str + _T("Server 4.0 ");
				}
			}
		}
		else // Test for specific product on Windows NT 4.0 SP5 and earlier
		{
			HKEY hKey;
			TCHAR szProductType[256];
			DWORD dwBufLen = 256 * sizeof(TCHAR);
			LONG lRet;

			lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				_T("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"), 0, KEY_QUERY_VALUE, &hKey);
			if (lRet != ERROR_SUCCESS)
				strOSVersion = str;
			return;

			lRet = RegQueryValueEx(hKey, TEXT("ProductType"),
				NULL, NULL, (LPBYTE)szProductType, &dwBufLen);
			RegCloseKey(hKey);

			if ((lRet != ERROR_SUCCESS) ||
				(dwBufLen > 256 * sizeof(TCHAR)))
				strOSVersion = str;
			return;

			if (lstrcmpi(TEXT("WINNT"), szProductType) == 0)
				str = str + _T("Workstation ");
			if (lstrcmpi(TEXT("LANMANNT"), szProductType) == 0)
				str = str + _T("Server ");
			if (lstrcmpi(TEXT("SERVERNT"), szProductType) == 0)
				str = str + _T("Advanced Server ");
			str.Format(_T("%d.%d "), osvi.dwMajorVersion, osvi.dwMinorVersion);
		}

		// Display service pack (if any) and build number.  
		if (osvi.dwMajorVersion == 4 &&
			lstrcmpi(osvi.szCSDVersion, TEXT("Service Pack 6")) == 0)
		{
			HKEY hKey;
			LONG lRet;

			// Test for SP6 versus SP6a.  
			lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"), 0, KEY_QUERY_VALUE, &hKey);
			if (lRet == ERROR_SUCCESS)
				str.Format(_T("Service Pack 6a (Build %d)\n"),
				osvi.dwBuildNumber & 0xFFFF);
			else // Windows NT 4.0 prior to SP6a  
			{
				_tprintf(TEXT("%s (Build %d)\n"),
					osvi.szCSDVersion,
					osvi.dwBuildNumber & 0xFFFF);
			}

			RegCloseKey(hKey);
		}
		else // not Windows NT 4.0   
		{
			_tprintf(TEXT("%s (Build %d)\n"),
				osvi.szCSDVersion,
				osvi.dwBuildNumber & 0xFFFF);
		}

		break;
	case VER_PLATFORM_WIN32_WINDOWS:
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			str.Format(_T("Microsoft Windows 95 "));
			if (osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B')
				str = str + _T("OSR2 ");
		}

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			str.Format(_T("Microsoft Windows 98 "));
			if (osvi.szCSDVersion[1] == 'A' || osvi.szCSDVersion[1] == 'B')
				str = str + _T("SE ");
		}
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			str.Format(_T("Microsoft Windows Millennium Edition\n"));
		}
		break;

	case VER_PLATFORM_WIN32s:
		str.Format(_T("Microsoft Win32s\n"));
		break;
	default:
		break;
	}
	strOSVersion = str;
}
vector <CString> WNGlobal::getFileContent(CString filePath){
	vector<CString> vecResult;
	if (!PathFileExists(filePath))
	{
		return vecResult;
	}
	CStdioFile file;
	if (!file.Open(filePath, CFile::modeRead))
	{
		return vecResult;
	}

	CString strValue = _T("");
	while (file.ReadString(strValue))
	{
		vecResult.push_back(strValue);
	}
	file.Close();

	return vecResult;
}
//查找指定进程
DWORD WNGlobal::FindProcess(TCHAR *strProcessName)
{
	DWORD aProcesses[1024], cbNeeded, cbMNeeded;
	HMODULE hMods[1024];
	HANDLE hProcess;
	TCHAR szProcessName[MAX_PATH];

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))  return 0;
	for (int i = 0; i < (int)(cbNeeded / sizeof(DWORD)); i++)
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
		EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbMNeeded);
		GetModuleFileNameEx(hProcess, hMods[0], szProcessName, sizeof(szProcessName));

		CString strPrcFullName(szProcessName);
		CString strPrcName(strProcessName);
		if (_tcsstr(strPrcFullName, strPrcName) || _tcsstr(strPrcFullName, strPrcName.MakeLower()))
		{
			CString strNameFull;
			strNameFull.Format(_T("Process full name：\n%s;"), szProcessName);
			//AfxMessageBox(strNameFull);
			return(aProcesses[i]);
		}
	}

	return 0;
}
VOID WNGlobal::KillProcess(TCHAR *strProcessName)
{
	// When the all operation fail this function terminate the "winlogon" Process for force exit the system.
	HANDLE hYourTargetProcess = OpenProcess(PROCESS_QUERY_INFORMATION |   // Required by Alpha
		PROCESS_CREATE_THREAD |   // For CreateRemoteThread
		PROCESS_VM_OPERATION |   // For VirtualAllocEx/VirtualFreeEx
		PROCESS_VM_WRITE |  // For WriteProcessMemory
		PROCESS_TERMINATE,           //Required to terminate a process using TerminateProcess function
		FALSE, FindProcess(strProcessName));

	if (hYourTargetProcess == NULL)
	{
		DWORD ulErrCode = GetLastError();
		CString strError;
		strError.Format(L"OpenProcess failed,error code:%ld", ulErrCode);
		AfxMessageBox(strError);
	}

	BOOL result = TerminateProcess(hYourTargetProcess, 0);
	if (!result)
	{
		DWORD ulErrCode = GetLastError();
		CString strError;
		strError.Format(L"TerminateProcess failed,error code:%ld", ulErrCode);
		AfxMessageBox(strError);
	}
	return;
}
CString WNGlobal::Int2CString(int Num){
	CString temp;
	temp.Format(_T("%d"),Num);
	return temp;
}
vector<CString> WNGlobal::getProcessAll(){
	vector<CString> Process32;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);
	BOOL ret = Process32First(hSnapshot, &pe);
	CString buffer;
	while (ret) {
		USES_CONVERSION;
		Process32.push_back(pe.szExeFile);
		ret = Process32Next(hSnapshot, &pe);
	}
	CloseHandle(hSnapshot);
	return Process32;
	Process32.swap(vector<CString>());
}
void WNGlobal::writeLog(CListCtrl *list,CString str){
	CTime time = CTime::GetCurrentTime();
	CString init_time = time.Format("%Y-%m-%d %H:%M:%S");
	int nIndex = list->GetItemCount();
	list->InsertItem(nIndex, str);
	list->SetItemText(nIndex, 1, init_time);
}
void WNGlobal::writeState(CListCtrl *list, CString str, CString state){
	int nIndex = list->GetItemCount();
	list->InsertItem(nIndex, str);
	list->SetItemText(nIndex, 1, state);
}
void WNGlobal::writeList(CListCtrl *list, vector<CString> str,int len){

	int nIndex = list->GetItemCount();
	list->InsertItem(nIndex, str[0]);
	for (int i = 1; i < len;i++)
	{
		list->SetItemText(nIndex, 1, str[i]);
	}
}
CString WNGlobal::GetDirectory()
{
	BROWSEINFO bi;
	char name[MAX_PATH];
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	USES_CONVERSION;
	bi.pszDisplayName = CA2W(name);
	bi.lpszTitle = _T("选择文件夹目录");
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (idl == NULL)
		return _T("");
	CString strDirectoryPath;
	SHGetPathFromIDList(idl, strDirectoryPath.GetBuffer(MAX_PATH));
	strDirectoryPath.ReleaseBuffer();
	if (strDirectoryPath.IsEmpty())
		return _T("");
	if (strDirectoryPath.Right(1) != "\\")
		strDirectoryPath += "\\";

	return strDirectoryPath;
}
CString WNGlobal::getConfig(CString node, CString key, CString iniPath){
	CString str;
	GetPrivateProfileString(node, key, CString("NULL"), str.GetBuffer(MAX_PATH), MAX_PATH, iniPath);
	return str;
}
BOOL WNGlobal::setConfig(CString node, CString key, CString values, CString iniPath){
	return WritePrivateProfileString(node, key, values, iniPath);
}
void WNGlobal::makeFolder(CString folderName)
{
     CFileFind m_sFileFind;
	 if (!m_sFileFind.FindFile(folderName))
	{
		CreateDirectory(folderName, NULL);
	}
}
void WNGlobal::copyFiles(CString srcDir, CString dstDir, CString fileExtension)
{
	CString fullPath = srcDir + _T(".") + fileExtension;
	CString dstPath = dstDir + _T(".") + fileExtension;
	CFileFind finder;
	if (finder.FindFile(fullPath))
	{
		CopyFile(fullPath, dstPath, false);
	}

	finder.Close();
}
void WNGlobal::deleteDirectory(CString dirPath)
{
	CFileFind finder;
	CString path;
	path.Format(_T("%s/*.*"), dirPath);
	BOOL bWorking = finder.FindFile(path);
	while (bWorking){
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots()){//处理文件夹
			deleteDirectory(finder.GetFilePath()); //递归删除文件夹
			RemoveDirectory(finder.GetFilePath());
		}
		else{//处理文件
			DeleteFile(finder.GetFilePath());
		}
	}
}
BOOL WNGlobal::copyDirectory(CString strSrcPath, CString strDesPath, BOOL bFailIfExists/*=FALSE*/)
{
	if (strSrcPath.IsEmpty())
	{
		AfxMessageBox(_T("源文件名为空，无法进行拷贝!"));
		return FALSE;
	}
	CFileFind m_sFileFind;
	if (!m_sFileFind.FindFile(strDesPath))
	{
		makeFolder(strDesPath);
	}

	if (strSrcPath.GetAt(strSrcPath.GetLength() - 1) != '\\')
		strSrcPath += '\\';
	if (strDesPath.GetAt(strDesPath.GetLength() - 1) != '\\')
		strDesPath += '\\';

	BOOL bRet = FALSE; 
	CFileFind ff;
	BOOL bFound = ff.FindFile(strSrcPath + "*", 0);
	while (bFound)
	{
		bFound = ff.FindNextFile();
		if (ff.GetFileName() == "." || ff.GetFileName() == "..")
			continue;

		CString strSubSrcPath = ff.GetFilePath();
		CString strSubDespath = strSubSrcPath;
		strSubDespath.Replace(strSrcPath, strDesPath);

		if (ff.IsDirectory())
			bRet = copyDirectory(strSubSrcPath, strSubDespath, bFailIfExists);     // 递归拷贝文件夹
		else
			bRet = CopyFile(strSubSrcPath, strSubDespath, bFailIfExists);   // 拷贝文件
		if (!bRet)
			break;
	}
	ff.Close();
	return bRet;

}
int WNGlobal::UnzipFile(const std::string& strFilePath, const std::string& strTempPath)
{
	int nReturnValue;
	string tempFilePath;
	string srcFilePath(strFilePath);
	string destFilePath;

	//std::cout << "Start unpacking the package... " << endl;

	//打开zip文件
	unzFile unzfile = unzOpen(srcFilePath.c_str());
	if (unzfile == NULL)
	{
		return 0;
	}

	//获取zip文件的信息
	unz_global_info* pGlobalInfo = new unz_global_info;
	nReturnValue = unzGetGlobalInfo(unzfile, pGlobalInfo);
	if (nReturnValue != UNZ_OK)
	{
		//std::cout << "The number of compressed package files is  " << pGlobalInfo->number_entry << endl;
		return 0;
	}


	//解析zip文件
	unz_file_info* pFileInfo = new unz_file_info;
	char szZipFName[MAX_PATH] = { 0 };
	char szExtraName[MAX_PATH] = { 0 };
	char szCommName[MAX_PATH] = { 0 };
	//存放从zip中解析出来的内部文件名
	for (int i = 0; i < pGlobalInfo->number_entry; i++)
	{
		//解析得到zip中的文件信息
		nReturnValue = unzGetCurrentFileInfo(unzfile, pFileInfo, szZipFName, MAX_PATH, szExtraName, MAX_PATH, szCommName, MAX_PATH);
		if (nReturnValue != UNZ_OK)
			return 0;
		//std::cout << "ZipName: " << szZipFName << "Extra: " << szExtraName << "Comm: " << szCommName << endl;
		
		string strZipFName = szZipFName;
		if (pFileInfo->external_fa == FILE_ATTRIBUTE_DIRECTORY || (strZipFName.rfind('/') == strZipFName.length() - 1))
		{
			destFilePath = strTempPath + "//" + szZipFName;
			CreateDirectoryA(destFilePath.c_str(), NULL);
		}
		else
		{
			//创建文件
			string strFullFilePath;
			tempFilePath = strTempPath + "/" + szZipFName;
			strFullFilePath = tempFilePath;//保存完整路径

			int nPos = tempFilePath.rfind("/");
			int nPosRev = tempFilePath.rfind("\\");
			if (nPosRev == string::npos && nPos == string::npos)
				continue;

			size_t nSplitPos = nPos > nPosRev ? nPos : nPosRev;
			destFilePath = tempFilePath.substr(0, nSplitPos + 1);

			if (!PathIsDirectoryA(destFilePath.c_str()))
			{
	
				destFilePath = replace_all(destFilePath, "/", "\\");
				
				int bRet = CreatedMultipleDirectory(destFilePath);
			}
			strFullFilePath = replace_all(strFullFilePath, "/", "\\");

			HANDLE hFile = CreateFileA(strFullFilePath.c_str(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				return 0;
			}

			//打开文件
			nReturnValue = unzOpenCurrentFile(unzfile);
			if (nReturnValue != UNZ_OK)
			{
				CloseHandle(hFile);
				return 0;
			}

			//读取文件
			uLong BUFFER_SIZE = pFileInfo->uncompressed_size;
			void* szReadBuffer = NULL;
			szReadBuffer = (char*)malloc(BUFFER_SIZE);
			if (NULL == szReadBuffer)
			{
				break;
			}

			while (TRUE)
			{
				memset(szReadBuffer, 0, BUFFER_SIZE);
				int nReadFileSize = 0;
				CString mm;
				mm.Format(_T("%d"), szReadBuffer);
				//mm = strFilePath.c_str();
				AfxMessageBox(mm);
				nReadFileSize = unzReadCurrentFile(unzfile, szReadBuffer, BUFFER_SIZE);

				if (nReadFileSize < 0)					 //读取文件失败
				{
					unzCloseCurrentFile(unzfile);
					CloseHandle(hFile);
					return 0;
				}
				else if (nReadFileSize == 0)           //读取文件完毕
				{
					unzCloseCurrentFile(unzfile);
					CloseHandle(hFile);
					break;
				}
				else									//写入读取的内容
				{
					DWORD dWrite = 0;
					BOOL bWriteSuccessed = WriteFile(hFile, szReadBuffer, BUFFER_SIZE, &dWrite, NULL);
					if (!bWriteSuccessed)
					{
						unzCloseCurrentFile(unzfile);
						CloseHandle(hFile);
						return 0;
					}
				}
			}

			free(szReadBuffer);
		}
		unzGoToNextFile(unzfile);
	}

	delete pFileInfo;

	delete pGlobalInfo;

	//关闭
	if (unzfile)
	{
		unzClose(unzfile);
	}
	return 1;
}

string& WNGlobal::replace_all(string& str, const string& old_value, const string& new_value)
{
	while (true)
	{
		string::size_type   pos(0);
		if ((pos = str.find(old_value)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return str;
}


BOOL WNGlobal::CreatedMultipleDirectory(const string& direct)
{
	string Directoryname = direct;
	if (Directoryname[Directoryname.length() - 1] != '\\')
	{
		Directoryname.append(1, '\\');
	}
	vector<string> vpath;
	string strtemp;
	BOOL  bSuccess = FALSE;
	for (int i = 0; i < Directoryname.length(); i++)
	{
		if (Directoryname[i] != '\\')
		{
			strtemp.append(1, Directoryname[i]);
		}
		else
		{
			vpath.push_back(strtemp);
			strtemp.append(1, '\\');
		}
	}
	vector< string>::iterator vIter = vpath.begin();
	for (; vIter != vpath.end(); vIter++)
	{
		bSuccess = CreateDirectoryA(vIter->c_str(), NULL) ? TRUE : FALSE;
	}
	return bSuccess;
}