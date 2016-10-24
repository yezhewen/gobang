// setaddress.cpp : 实现文件
//

#include "stdafx.h"
#include "gobang.h"
#include "setaddress.h"
#include "afxdialogex.h"
#include"gobangDlg.h"

// setaddress 对话框

IMPLEMENT_DYNAMIC(setaddress, CDialogEx)

setaddress::setaddress(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETADDRESS, pParent)
	, m_port(8888)
	, m_ipvalue(0)
{
	
}

setaddress::~setaddress()
{
}

void setaddress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_port);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ip);
	DDX_Text(pDX, IDC_EDIT1, m_port);
	DDV_MinMaxLong(pDX, m_port, 0, 65535);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_ipvalue);
}


BEGIN_MESSAGE_MAP(setaddress, CDialogEx)
	ON_BN_CLICKED(IDOK, &setaddress::OnBnClickedOk)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &setaddress::OnIpnFieldchangedIpaddress1)
END_MESSAGE_MAP()


// setaddress 消息处理程序


void setaddress::OnBnClickedOk()
{
	UpdateData();

	BYTE nip[4];

	m_ip.GetAddress(nip[0], nip[1], nip[2], nip[3]);
	
	addr.sin_addr =*((in_addr*)nip);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);

	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void setaddress::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


BOOL setaddress::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	char hostname[20];
	hostent *hostmsg;
	gethostname(hostname, sizeof(hostname));
	hostmsg = gethostbyname(hostname);
	m_ip.SetAddress((hostmsg->h_addr_list[0][0]),(hostmsg->h_addr_list[0][1]),(hostmsg->h_addr_list[0][2]),(hostmsg->h_addr_list[0][3]));
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
