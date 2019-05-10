class CBindCBHttpCallback : public IBindStatusCallback
{
public:

	CBindCBHttpCallback();
	~CBindCBHttpCallback();

	CLauncherDlg* m_pDlg;

	STDMETHOD(OnStartBinding)(DWORD dwReserved, IBinding __RPC_FAR *pib)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(GetPriority)(LONG __RPC_FAR *pnPriority)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnLowResource)(DWORD reserved)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnProgress)(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR wszStatusText);

	STDMETHOD(OnStopBinding)(HRESULT hresult, LPCWSTR szError)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(GetBindInfo)(DWORD __RPC_FAR *grfBINDF, BINDINFO __RPC_FAR *pbindinfo)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnDataAvailable)(DWORD grfBSCF, DWORD dwSize, FORMATETC __RPC_FAR *pformatetc, STGMEDIUM __RPC_FAR *pstgmed)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnObjectAvailable)(REFIID riid, IUnknown __RPC_FAR *punk)
	{
		return E_NOTIMPL;
	}

	STDMETHOD_(ULONG, AddRef)()
	{
		return 0;
	}

	STDMETHOD_(ULONG, Release)()
	{
		return 0;
	}

	STDMETHOD(QueryInterface)(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		return E_NOTIMPL;
	}
};

CBindCBHttpCallback::CBindCBHttpCallback(){}
CBindCBHttpCallback::~CBindCBHttpCallback(){}

//处理进度条

HRESULT  CBindCBHttpCallback::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR wszStatusText)
{
	//CProgressCtrl* m_prg = (CProgressCtrl*)m_pDlg->GetDlgItem(IDC_PROGRESS);
	//m_prg->SetRange32(0, 100);
	//if (m_prg&&IsWindow(m_prg->GetSafeHwnd()))
	//{
		//m_prg->SetPos((int)(ulProgress*100.0 / ulProgressMax));
	//}
	m_pDlg->setProgress(101, (int)(ulProgress*100.0 / ulProgressMax));
	CString szText;
	szText.Format(_T("%d%%"), (int)(ulProgress*100.0 / ulProgressMax));
	//(m_pDlg->GetDlgItem(IDC_PRO_INFO))->SetWindowText(szText);
	m_pDlg->setMsg(103, 0, szText,0);
	return S_OK;
}