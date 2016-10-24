
// gobangDlg.h : 头文件
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

// CgobangDlg 对话框
class CgobangDlg : public CDialogEx
{
// 构造
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
	}	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GOBANG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	void drawboard();       //画棋盘
	void fillbg(CDC *pdc, CRect &rect);//主界面背景填充
	void placepiece(int x, int y, int color);//落子
	bool undolast();//悔棋


	//测试
	in_addr gethostipn()
	{
		char hostname[20];
		int result;
		hostent *hostmsg;
		result = gethostname(hostname, sizeof(hostname));
		hostmsg = gethostbyname(hostname);
		return (*((in_addr*)hostmsg->h_addr_list[0]));
	}
	
	void setip();//服务器ip 端口设置函数
	
	int  connect_s();//连接服务器
	bool sendmsg(void);//发送信息
	static DWORD WINAPI recvmsg(LPVOID param);//接收信息
	static DWORD WINAPI chat(LPVOID param);//接收信息
	void process_msg();//处理消息
	void endconnect();//结束网络连接
	void newgame();//开始下一局

// 实现
protected:
	HICON m_hIcon;

	int m_width;//棋格宽度
	int board_size;//棋盘大小
	CImageList m_img; //棋子位图
	chess m_chess;//棋局对象
	int m_color;//己方颜色 1代表黑子 2代表白子
	bool m_turn;//1代表己方回合 可以落子
	int m_next;//对方是否同意开始下一局，1表示同意
	bool m_ready;//是否已准备好开始游戏
	int m_goal;
	DWORD recv_handle;

	//网络连接部分

	SOCKET m_socket;    //连接套接字
	sockaddr_in m_addr; //服务器的端口和ip信息
	char sendbuf[100];//发送缓冲区
	char recvbuf[100];//接收缓冲区


	// 生成的消息映射函数
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
