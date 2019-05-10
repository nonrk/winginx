
// LauncherDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CLauncherDlg 对话框
class CLauncherDlg : public CDialogEx
{
// 构造
public:
	CLauncherDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DEFAULT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
	CString  GetAppPath();
	CString httpGet(char* Url);
	CString httpPost(char* Url, char* params);
	static unsigned int __stdcall DownLoadProcess(void*);
	void setMsg(int type, int data, CString strData, int data_type);
	void setProgress(int type, int data);
	int getFileLength(CString filePath);
	void setStatus();
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnStart();
	CProgressCtrl m_progress;
	CStatic m_msg;
	CProgressCtrl m_process_total;
	CStatic m_msg_total;
	CStatic m_total;
	CButton m_start;
};
