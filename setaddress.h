#pragma once
#include "afxcmn.h"


// setaddress �Ի���

class setaddress : public CDialogEx
{
	DECLARE_DYNAMIC(setaddress)

public:
	setaddress(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~setaddress();

	

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETADDRESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	//CString m_port;
	sockaddr_in addr;
	CIPAddressCtrl m_ip;
	long m_port;
	DWORD m_ipvalue;
	virtual BOOL OnInitDialog();
};
