
// LauncherDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CLauncherDlg �Ի���
class CLauncherDlg : public CDialogEx
{
// ����
public:
	CLauncherDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DEFAULT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
	CString  GetAppPath();
	CString httpGet(char* Url);
	CString httpPost(char* Url, char* params);
	static unsigned int __stdcall DownLoadProcess(void*);
	void setMsg(int type, int data, CString strData, int data_type);
	void setProgress(int type, int data);
	int getFileLength(CString filePath);
	void setStatus();
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
