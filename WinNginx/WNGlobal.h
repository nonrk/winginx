#include <string>
#include <vector>
#include <windows.h>
#include "psapi.h"
#include <tlhelp32.h> 

#pragma comment(lib,"version.lib")
using namespace std;
class WNGlobal
{
public:
	WNGlobal();
	~WNGlobal();
	//
	CString GetFileVersion(const CString& sTargetFileName);
	BOOL IsFileExist(const CString& csFile);
	CString GetAppPath();
	vector<CString> SplitCString(CString strSource, CString ch);
	void GetOSVersion(CString &strOSVersion, CString &strServerVersion);
	vector <CString> getFileContent(CString filePath);
	DWORD FindProcess(TCHAR *strProcessName);
	void KillProcess(TCHAR *strProcessName);
	CString Int2CString(int Num);
	vector<CString> getProcessAll();
	void writeLog(CListCtrl *list, CString str);
	void writeState(CListCtrl *list, CString str, CString state);
	CString GetDirectory();
	void writeList(CListCtrl *list, vector<CString> str, int len);
	CString getConfig(CString node, CString key, CString iniPath);
	BOOL setConfig(CString node, CString key, CString values, CString iniPath);
	void makeFolder(CString folderName);
	void copyFiles(CString srcDir, CString dstDir, CString fileExtension);
	void deleteDirectory(CString dirPath);
	BOOL copyDirectory(CString strSrcPath, CString strDesPath, BOOL bFailIfExists);
};

