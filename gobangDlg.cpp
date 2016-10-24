
// gobangDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "gobang.h"
#include "gobangDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CgobangDlg 对话框



void CgobangDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BOARD, m_board);
	DDX_Control(pDX, IDC_BUTTON_undo, button_undo);
	DDX_Control(pDX, IDC_BUTTON_start, button_start);
	DDX_Control(pDX, IDC_note, notetext);
	DDX_Text(pDX, IDC_EDIT1, m_words);
	DDV_MaxChars(pDX, m_words, 40);
	DDX_Control(pDX, IDC_EDIT3, m_showwords);
}

BEGIN_MESSAGE_MAP(CgobangDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_start, &CgobangDlg::OnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_undo, &CgobangDlg::OnClickedButtonUndo)
	ON_BN_CLICKED(IDC_BUTTON3, &CgobangDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, &CgobangDlg::OnBnClickedOk)
	//ON_STN_CLICKED(IDC_note, &CgobangDlg::OnStnClickednote)

	ON_BN_CLICKED(IDC_CHAT, &CgobangDlg::OnBnClickedChat)
END_MESSAGE_MAP()


// CgobangDlg 消息处理程序

BOOL CgobangDlg::OnInitDialog()
{

		CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
   

	                            //初始化位图棋子控件m_img
	CBitmap bitmap_b, bitmap_w, bitmap_d;
	bitmap_d.LoadBitmapW(IDB_BITMAP_dot);
	bitmap_b.LoadBitmapW(IDB_BITMAP_black);
	bitmap_w.LoadBitmapW(IDB_BITMAP_white);
	m_img.Create(42, 42, ILC_COLOR24 | ILC_MASK, 0, 3);
	m_img.Add(&bitmap_d, RGB(255, 255, 255));
	m_img.Add(&bitmap_b, RGB(255,255,255));
	m_img.Add(&bitmap_w, RGB(0, 0, 0));
	               
                         //打开winsocket库
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);


	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CgobangDlg::drawboard() //画棋盘
{
	CDC dc;
	CRect rect;
	CDC *pdc1 = m_board.GetDC();
	m_board.GetClientRect(&rect);
	CBitmap newbitmap, *oldbitmap;
	CPen newpen, *oldpen;
	CBrush newbrush, *oldbrush;

	int offset =m_width;//棋盘左上角与主界面的偏移量

	dc.CreateCompatibleDC(pdc1);
	newbitmap.CreateCompatibleBitmap(pdc1, rect.right, rect.bottom);
	oldbitmap = dc.SelectObject(&newbitmap);
	
	newbrush.CreateSolidBrush(RGB(255, 222, 173));//棋盘背景色填充
	oldbrush = dc.SelectObject(&newbrush);
	dc.Rectangle(rect);
	dc.SelectObject(oldbrush);
	newbrush.DeleteObject();

	newpen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	oldpen = dc.SelectObject(&newpen);
	
	for (int i = 0; i < board_size; i++)          //棋盘
	{
		dc.MoveTo(m_width, (i+1)*m_width);
		dc.LineTo((board_size)*m_width, (i+1)*m_width);
		dc.MoveTo((i+1)*m_width,m_width);
		dc.LineTo((i+1)*m_width, (board_size)*m_width);
	}
	                                               //棋盘上5个黑点位置
	POINT pt[5] = { 
	{4*m_width-21,4 * m_width - 21 },
	{4 * m_width  - 21, 12 * m_width - 21 },
	{ 8 * m_width - 21, 8 * m_width  - 21 },
	{ 12 * m_width  - 21,4 * m_width  - 21 },
	{ 12* m_width  - 21, 12 * m_width  - 21 } 
	};
	for (int i = 0; i < 5; i++)
	{
		m_img.Draw(&dc, 0, pt[i], ILD_TRANSPARENT);
	}
                          
	//棋子
	POINT pt1;
	int color;
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
		{
			if ((color=m_chess.getdata(i, j))>0)
			{
				pt1.x = i * m_width + offset - 21, pt1.y = j * m_width + offset - 21;
				m_img.Draw(&dc, color, pt1, ILD_TRANSPARENT);
			}

		}

	dc.SelectObject(oldpen);
	newpen.DeleteObject();
    
	pdc1->BitBlt(0, 0, rect.right, rect.bottom, &dc, 0, 0, SRCCOPY);
	newbitmap.DeleteObject();
	//pdc1->DeleteDC();
	dc.DeleteDC();

}

void CgobangDlg::fillbg(CDC*pdc, CRect &rect)        //背景填充
{
	CDC dc;
	CBitmap newbitmap, *oldbitmap;
	CBrush newbrush, *oldbrush;

	dc.CreateCompatibleDC(pdc);
	newbitmap.CreateCompatibleBitmap(pdc, rect.right, rect.bottom);
	oldbitmap = dc.SelectObject(&newbitmap);

	newbrush.CreateSolidBrush(RGB(255 ,193, 37));
	oldbrush = dc.SelectObject(&newbrush);
	dc.Rectangle(rect);
	dc.SelectObject(oldbrush);

	pdc->BitBlt(0, 0, rect.right, rect.bottom, &dc, 0, 0, SRCCOPY);

	newbrush.DeleteObject();
	dc.DeleteDC();
}
void CgobangDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
        CRect rect;
		GetClientRect(rect);
		fillbg(&dc, rect); //主界面背景填充

		//在静态图片空间m_board中画出棋盘
		//CDC *pdc = m_board.GetDC();
	  //  m_board.GetClientRect(&rect);
		drawboard();

		CDialogEx::OnPaint();
	}
}

void CgobangDlg::placepiece(int x, int y, int color)//落子
{
	m_chess.set(x, y, color);
	CDC *dc;
	CRect rect;
	POINT pt; //落子位置
	pt.x = x*m_width + 21;
	pt.y = y*m_width + 21;

	m_board.GetClientRect(&rect);
	dc = m_board.GetDC();
	m_board.GetClientRect(&rect);

	m_img.Draw(dc, color, pt, ILD_TRANSPARENT);
	dc->DeleteDC();
	button_undo.EnableWindow(true);
	if (color == m_color)
	{
		notetext.SetWindowTextW(_T("对手回合"));
	}
	else
		notetext.SetWindowTextW(_T("我的回合"));


	if (m_chess.iswin() !=  0)//=1黑子胜，=2白子胜
	{
		CString str, str1;
		int win = m_chess.iswin();
		if (win ==m_color)
		{
				str = "你赢了，不错啊";
				m_goal++;
		}
		else
		{
				str = "你输了，再接再厉吧";
		}
		int ret=MessageBox(str,NULL, MB_SYSTEMMODAL|MB_OK);
		
			//再来一局 清空棋盘，再次点击开始游戏开始新的一局
		str1.Format(_T("你已获胜：%d"), m_goal);
		m_turn = 0;
		button_undo.EnableWindow(false);//禁止悔棋
		button_start.EnableWindow(true);
		button_start.SetWindowTextW(_T("再来一局"));
		notetext.SetWindowTextW(str1);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CgobangDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CgobangDlg::OnLButtonUp(UINT nFlags, CPoint point)//鼠标左键up
{
	
	 //TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_turn)
	{
		int x, y, win;
		x = (point.x -21) / m_width;
		y = (point.y - 21) / m_width;
		if (x>=0&&x<=14&&y>=0&&y<=14&&m_chess.getdata(x, y) == 0)
		{
			
            int *buffer = (int*)sendbuf;
			ZeroMemory(sendbuf, sizeof(sendbuf));
			buffer[0] = 1;
			buffer[1] = x;
			buffer[2] = y;
			sendmsg();
		
			placepiece(x, y, m_color);
			m_turn =0;
		}
		button_undo.EnableWindow(true);
	}

    CDialogEx::OnLButtonUp(nFlags, point);
	
}


void CgobangDlg::OnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_ready==1&&m_socket!=INVALID_SOCKET)
	{
		newgame();
	}
	else
	{
		 setip();
		int ret=connect_s();
		if (ret == -1)
		{
			MessageBox(_T("连接服务器失败！"));
		}
		else 
		m_ready = 1;
	}
}


void CgobangDlg::OnClickedButtonUndo()
{
	if (!m_turn)
	{
		ZeroMemory(sendbuf, sizeof(sendbuf));
		int*buffer = (int*)sendbuf;
		buffer[0] = 2;
		buffer[1] = 0;
		if (sendmsg()==0)
		{
			//undolast();
			MessageBox(_T("等待对方决定……"));
		}
	}
	else
		MessageBox(_T("这颗棋子不是你下的！"));
	button_undo.EnableWindow(false);
	// TODO: 在此添加控件通知处理程序代码
}

//悔棋
bool CgobangDlg::undolast()
{
	int ret = m_chess.undo();
	Invalidate();
	m_turn = ~m_turn;
	return ret;

}








//网络连接 

int CgobangDlg::connect_s()//连接服务器
{

	/*char hostname[20];
	hostent *hostmsg;
	gethostname(hostname, sizeof(hostname));
	hostmsg = gethostbyname(hostname);

	m_addr.sin_addr=gethostipn();
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(8888);*/


	m_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (m_socket == INVALID_SOCKET) MessageBox(_T("socket失效"));
	int ret = connect(m_socket, (sockaddr*)&m_addr, sizeof(m_addr));

	if (ret == -1)
	{
		//MessageBox(_T("连接服务器失败！"));
		return ret;//失败返回-1
	}
	else MessageBox(_T("连接服务器成功，正在等待对手"));

	button_start.EnableWindow(false);
	notetext.SetWindowTextW(_T("等待玩家中"));
	//成功则开始接收服务器信息
	HANDLE recvthread = CreateThread(NULL, 0, CgobangDlg::recvmsg, this, 0,NULL);
	//MessageBox(_T("进程创建"));
	
}

// 接收信息函数
DWORD WINAPI CgobangDlg::recvmsg(LPVOID param)
{
	CgobangDlg *p =(CgobangDlg *) param;

	if (p->m_socket == INVALID_SOCKET)
		return -1;
	int ret;
	while (1)
	{
		ret = recv(p->m_socket, p->recvbuf, sizeof(p->recvbuf), 0);
		if (ret > 0)
		{
			//p->MessageBoxW(_T("数据已接受，转处理函数"));
			p->process_msg();
		}
		else
		{
			p->MessageBoxW(_T("接收信息失败，已与服务器断开连接"));
			return -1;
		}
	}
}



//信息处理函数
void CgobangDlg::process_msg()
{
	//MessageBox(_T("正在处理信息"));
	int *buffer = (int *)recvbuf;
	int *sendbuf1 = (int *)sendbuf;
	int color;
	int flag = buffer[0];
	switch (flag)
	{
	case 0:   //初始化信息 分配先手玩家
		if (m_color == 0)
		{
			m_color = buffer[1];
		}
		else
		{
			m_color == 1 ? m_color = 2 : m_color = 1;
		}
		if (m_color == 1)
		{
			m_turn = 1;
			notetext.SetWindowTextW(_T("我方先手"));
		}
		else
		{
			m_turn = 0;
			notetext.SetWindowTextW(_T("对方先手"));
		}
		break;

	case 1:                 //落子信息 1
		m_color == 1 ? color = 2 : color = 1;
		m_turn = 1;
		placepiece(buffer[1], buffer[2], color);
		break;

	case 2:                //悔棋信息 2 0请求，2 1 同意，2 2 拒绝；
			if (buffer[1] == 0) //请求悔棋
			{
				int ret = MessageBox(_T("对方请求悔棋，是否同意？"), NULL, MB_YESNO | MB_SYSTEMMODAL);
				if (ret == IDYES)
				{
					undolast();
					sendbuf1[0] = 2; sendbuf1[1] = 1;
					send(m_socket, sendbuf, sizeof(sendbuf), 0);
				}
				else
				{
					sendbuf1[0] = 2; sendbuf1[1] = 2;
					send(m_socket, sendbuf, sizeof(sendbuf),0);
				}
			}
			else
			{
				if (buffer[1] == 1)
				{
					undolast();
				}
				else
				{
					MessageBox(_T("对方拒绝了你的悔棋请求!"));
				}
			}
			break;
	case 4:
		notetext.SetWindowTextW(_T("对手已离开游戏，重新匹配中"));
		m_chess.reset();
		m_goal = 0;
		m_color = 0;
		m_turn = 0;
		Invalidate();
		button_start.EnableWindow(false);
		button_undo.EnableWindow(false);
		break;

	case 5:
		{
			CString str;
			int charLen = strlen(recvbuf + 4); //计算字符串大小，以字节为单位，一个汉字占两个字节  
		
			int len = MultiByteToWideChar(CP_ACP, 0, recvbuf + 4, charLen, NULL, 0); //计算多字节字符的大小，按字符计算  
			wchar_t *pWChar = new wchar_t[len + 1]; //为宽字节字符数申请空间，  
			MultiByteToWideChar(CP_ACP, 0, recvbuf + 4, charLen, pWChar, len); //多字节编码转换成宽字节编码  
			pWChar[len] = '\0';
			str.Append(pWChar);
			delete[] pWChar;
	
			str = _T("对方：") + str + '\n';
			int nstart, nend;
			m_showwords.GetSel(nstart, nend);
			m_showwords.SetSel(nend, nend);
			m_showwords.ReplaceSel(str);
		}
		break;

	default:break;
	}

	ZeroMemory(recvbuf, sizeof(recvbuf));//清空接收缓冲区
}

//开始下一局	
void CgobangDlg:: newgame()
{
	m_chess.reset();
	Invalidate();//清空棋盘
	int *buffer = (int *)sendbuf;
	buffer[0] = 0;  
	buffer[1] = m_color;
	sendmsg();
	button_start.EnableWindow(false);
	button_undo.EnableWindow(false);
	notetext.SetWindowTextW(_T("等待对手加入"));

}


//发送信息
bool CgobangDlg::sendmsg(void)
{
	//MessageBox(_T("发送数据"));
	int ret = send(m_socket, sendbuf, sizeof(sendbuf), 0);
	if (ret <=0)	MessageBox(_T("发送信息失败，已与服务器断开连接"));
	return ret;
}
//结束网络连接
void CgobangDlg::endconnect()
{
	//CloseHandle(recv_handle);
	if (m_socket != INVALID_SOCKET)
	closesocket(m_socket);
	WSACleanup();

}

void CgobangDlg::setip()
{
	setaddress setaddr;
	setaddr.DoModal();
	m_addr = setaddr.addr;
}

void CgobangDlg::OnBnClickedButton3()
{
	setaddress setaddr;
	setaddr.DoModal();
	m_addr = setaddr.addr;

	// TODO: 在此添加控件通知处理程序代码
}


void CgobangDlg::OnBnClickedOk()
{
	
		endconnect();
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}



//聊天线程

DWORD WINAPI CgobangDlg::chat(LPVOID param)
{
	
	CgobangDlg* p = (CgobangDlg*)param;

	char buffer[100] = { '\0' };
	*(int *)buffer = 5;
	CString str = p->m_words;
	if (!(p->m_words.IsEmpty()))
	{ 
		int n = str.GetLength(); //获取str的字符数  
		int len = WideCharToMultiByte(CP_ACP, 0, str, n, NULL, 0, NULL, NULL); //获取宽字节字符的大小，大小是按字节计算的  

		WideCharToMultiByte(CP_ACP, 0, str, n,buffer+4, len, NULL, NULL); //宽字节编码转换成多字节编码  
		
		if(p->m_socket==INVALID_SOCKET)
			str = _T("消息发送失败");
		else
		{
			int ret = send(p->m_socket, buffer, 100, 0);
			if (ret <= 0)
			{
				p->MessageBox(_T("发送失败"));

			}
		}
		str = _T("我： ") + str + '\n';
		int nstart, nend;
		p->m_showwords.GetSel(nstart, nend);
		p->m_showwords.SetSel(nend, nend);
		p->m_showwords.ReplaceSel(str);
		p->m_words = _T("");
	}
	return 0;
}


void CgobangDlg::OnBnClickedChat()
{
	if (m_color)
	{
		UpdateData();
		HANDLE testthread = CreateThread(NULL, 0, CgobangDlg::chat, this, 0, NULL);
		UpdateData(false);
	}
	else
	{
		m_showwords.ReplaceSel( _T("系统：未匹配到对手"));
		
		
	}
}


