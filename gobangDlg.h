
// gobangDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include"Chess.h"
#include"setaddress.h"
#include<Windows.h>
#include<WinSock2.h>	
#include<iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")


//DWORD WINAPI recvmsg(LPVOID param);

// CgobangDlg �Ի���
class CgobangDlg : public CDialogEx
{
// ����
public:
	CgobangDlg(CWnd* pParent = NULL)
		: CDialogEx(IDD_GOBANG_DIALOG, pParent)
		, m_words(_T(""))
	{
		m_width = 44;
		board_size = 15;
		m_color = 0;
		m_socket = INVALID_SOCKET;
		m_ready = 0;
		m_turn = 0;
		m_goal = 0;
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	}	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GOBANG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	void drawboard();       //������
	void fillbg(CDC *pdc, CRect &rect);//�����汳�����
	void placepiece(int x, int y, int color);//����
	bool undolast();//����


	//����
	in_addr gethostipn()
	{
		char hostname[20];
		int result;
		hostent *hostmsg;
		result = gethostname(hostname, sizeof(hostname));
		hostmsg = gethostbyname(hostname);
		return (*((in_addr*)hostmsg->h_addr_list[0]));
	}
	
	void setip();//������ip �˿����ú���
	
	int  connect_s();//���ӷ�����
	bool sendmsg(void);//������Ϣ
	static DWORD WINAPI recvmsg(LPVOID param);//������Ϣ
	static DWORD WINAPI chat(LPVOID param);//������Ϣ
	void process_msg();//������Ϣ
	void endconnect();//������������
	void newgame();//��ʼ��һ��

// ʵ��
protected:
	HICON m_hIcon;

	int m_width;//�����
	int board_size;//���̴�С
	CImageList m_img; //����λͼ
	chess m_chess;//��ֶ���
	int m_color;//������ɫ 1������� 2�������
	bool m_turn;//1�������غ� ��������
	int m_next;//�Է��Ƿ�ͬ�⿪ʼ��һ�֣�1��ʾͬ��
	bool m_ready;//�Ƿ���׼���ÿ�ʼ��Ϸ
	int m_goal;
	DWORD recv_handle;

	//�������Ӳ���

	SOCKET m_socket;    //�����׽���
	sockaddr_in m_addr; //�������Ķ˿ں�ip��Ϣ
	char sendbuf[100];//���ͻ�����
	char recvbuf[100];//���ջ�����


	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_board;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClickedButtonStart();
	afx_msg void OnClickedButtonUndo();

	//friend class gameover;
	friend class setaddress;
	CButton button_undo;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedOk();
	CButton button_start;
	CStatic notetext;
	afx_msg void OnStnClickednote();

	CString m_words;
	CEdit m_showwords;
	afx_msg void OnBnClickedChat();
	afx_msg void OnChangeEdit1();
};
