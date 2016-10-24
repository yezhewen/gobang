
// gobangDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "gobang.h"
#include "gobangDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CgobangDlg �Ի���



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


// CgobangDlg ��Ϣ�������

BOOL CgobangDlg::OnInitDialog()
{

		CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
   

	                            //��ʼ��λͼ���ӿؼ�m_img
	CBitmap bitmap_b, bitmap_w, bitmap_d;
	bitmap_d.LoadBitmapW(IDB_BITMAP_dot);
	bitmap_b.LoadBitmapW(IDB_BITMAP_black);
	bitmap_w.LoadBitmapW(IDB_BITMAP_white);
	m_img.Create(42, 42, ILC_COLOR24 | ILC_MASK, 0, 3);
	m_img.Add(&bitmap_d, RGB(255, 255, 255));
	m_img.Add(&bitmap_b, RGB(255,255,255));
	m_img.Add(&bitmap_w, RGB(0, 0, 0));
	               
                         //��winsocket��
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);


	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CgobangDlg::drawboard() //������
{
	CDC dc;
	CRect rect;
	CDC *pdc1 = m_board.GetDC();
	m_board.GetClientRect(&rect);
	CBitmap newbitmap, *oldbitmap;
	CPen newpen, *oldpen;
	CBrush newbrush, *oldbrush;

	int offset =m_width;//�������Ͻ����������ƫ����

	dc.CreateCompatibleDC(pdc1);
	newbitmap.CreateCompatibleBitmap(pdc1, rect.right, rect.bottom);
	oldbitmap = dc.SelectObject(&newbitmap);
	
	newbrush.CreateSolidBrush(RGB(255, 222, 173));//���̱���ɫ���
	oldbrush = dc.SelectObject(&newbrush);
	dc.Rectangle(rect);
	dc.SelectObject(oldbrush);
	newbrush.DeleteObject();

	newpen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	oldpen = dc.SelectObject(&newpen);
	
	for (int i = 0; i < board_size; i++)          //����
	{
		dc.MoveTo(m_width, (i+1)*m_width);
		dc.LineTo((board_size)*m_width, (i+1)*m_width);
		dc.MoveTo((i+1)*m_width,m_width);
		dc.LineTo((i+1)*m_width, (board_size)*m_width);
	}
	                                               //������5���ڵ�λ��
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
                          
	//����
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

void CgobangDlg::fillbg(CDC*pdc, CRect &rect)        //�������
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
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
        CRect rect;
		GetClientRect(rect);
		fillbg(&dc, rect); //�����汳�����

		//�ھ�̬ͼƬ�ռ�m_board�л�������
		//CDC *pdc = m_board.GetDC();
	  //  m_board.GetClientRect(&rect);
		drawboard();

		CDialogEx::OnPaint();
	}
}

void CgobangDlg::placepiece(int x, int y, int color)//����
{
	m_chess.set(x, y, color);
	CDC *dc;
	CRect rect;
	POINT pt; //����λ��
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
		notetext.SetWindowTextW(_T("���ֻغ�"));
	}
	else
		notetext.SetWindowTextW(_T("�ҵĻغ�"));


	if (m_chess.iswin() !=  0)//=1����ʤ��=2����ʤ
	{
		CString str, str1;
		int win = m_chess.iswin();
		if (win ==m_color)
		{
				str = "��Ӯ�ˣ�����";
				m_goal++;
		}
		else
		{
				str = "�����ˣ��ٽ�������";
		}
		int ret=MessageBox(str,NULL, MB_SYSTEMMODAL|MB_OK);
		
			//����һ�� ������̣��ٴε����ʼ��Ϸ��ʼ�µ�һ��
		str1.Format(_T("���ѻ�ʤ��%d"), m_goal);
		m_turn = 0;
		button_undo.EnableWindow(false);//��ֹ����
		button_start.EnableWindow(true);
		button_start.SetWindowTextW(_T("����һ��"));
		notetext.SetWindowTextW(str1);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CgobangDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CgobangDlg::OnLButtonUp(UINT nFlags, CPoint point)//������up
{
	
	 //TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			MessageBox(_T("���ӷ�����ʧ�ܣ�"));
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
			MessageBox(_T("�ȴ��Է���������"));
		}
	}
	else
		MessageBox(_T("������Ӳ������µģ�"));
	button_undo.EnableWindow(false);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//����
bool CgobangDlg::undolast()
{
	int ret = m_chess.undo();
	Invalidate();
	m_turn = ~m_turn;
	return ret;

}








//�������� 

int CgobangDlg::connect_s()//���ӷ�����
{

	/*char hostname[20];
	hostent *hostmsg;
	gethostname(hostname, sizeof(hostname));
	hostmsg = gethostbyname(hostname);

	m_addr.sin_addr=gethostipn();
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(8888);*/


	m_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (m_socket == INVALID_SOCKET) MessageBox(_T("socketʧЧ"));
	int ret = connect(m_socket, (sockaddr*)&m_addr, sizeof(m_addr));

	if (ret == -1)
	{
		//MessageBox(_T("���ӷ�����ʧ�ܣ�"));
		return ret;//ʧ�ܷ���-1
	}
	else MessageBox(_T("���ӷ������ɹ������ڵȴ�����"));

	button_start.EnableWindow(false);
	notetext.SetWindowTextW(_T("�ȴ������"));
	//�ɹ���ʼ���շ�������Ϣ
	HANDLE recvthread = CreateThread(NULL, 0, CgobangDlg::recvmsg, this, 0,NULL);
	//MessageBox(_T("���̴���"));
	
}

// ������Ϣ����
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
			//p->MessageBoxW(_T("�����ѽ��ܣ�ת������"));
			p->process_msg();
		}
		else
		{
			p->MessageBoxW(_T("������Ϣʧ�ܣ�����������Ͽ�����"));
			return -1;
		}
	}
}



//��Ϣ������
void CgobangDlg::process_msg()
{
	//MessageBox(_T("���ڴ�����Ϣ"));
	int *buffer = (int *)recvbuf;
	int *sendbuf1 = (int *)sendbuf;
	int color;
	int flag = buffer[0];
	switch (flag)
	{
	case 0:   //��ʼ����Ϣ �����������
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
			notetext.SetWindowTextW(_T("�ҷ�����"));
		}
		else
		{
			m_turn = 0;
			notetext.SetWindowTextW(_T("�Է�����"));
		}
		break;

	case 1:                 //������Ϣ 1
		m_color == 1 ? color = 2 : color = 1;
		m_turn = 1;
		placepiece(buffer[1], buffer[2], color);
		break;

	case 2:                //������Ϣ 2 0����2 1 ͬ�⣬2 2 �ܾ���
			if (buffer[1] == 0) //�������
			{
				int ret = MessageBox(_T("�Է�������壬�Ƿ�ͬ�⣿"), NULL, MB_YESNO | MB_SYSTEMMODAL);
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
					MessageBox(_T("�Է��ܾ�����Ļ�������!"));
				}
			}
			break;
	case 4:
		notetext.SetWindowTextW(_T("�������뿪��Ϸ������ƥ����"));
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
			int charLen = strlen(recvbuf + 4); //�����ַ�����С�����ֽ�Ϊ��λ��һ������ռ�����ֽ�  
		
			int len = MultiByteToWideChar(CP_ACP, 0, recvbuf + 4, charLen, NULL, 0); //������ֽ��ַ��Ĵ�С�����ַ�����  
			wchar_t *pWChar = new wchar_t[len + 1]; //Ϊ���ֽ��ַ�������ռ䣬  
			MultiByteToWideChar(CP_ACP, 0, recvbuf + 4, charLen, pWChar, len); //���ֽڱ���ת���ɿ��ֽڱ���  
			pWChar[len] = '\0';
			str.Append(pWChar);
			delete[] pWChar;
	
			str = _T("�Է���") + str + '\n';
			int nstart, nend;
			m_showwords.GetSel(nstart, nend);
			m_showwords.SetSel(nend, nend);
			m_showwords.ReplaceSel(str);
		}
		break;

	default:break;
	}

	ZeroMemory(recvbuf, sizeof(recvbuf));//��ս��ջ�����
}

//��ʼ��һ��	
void CgobangDlg:: newgame()
{
	m_chess.reset();
	Invalidate();//�������
	int *buffer = (int *)sendbuf;
	buffer[0] = 0;  
	buffer[1] = m_color;
	sendmsg();
	button_start.EnableWindow(false);
	button_undo.EnableWindow(false);
	notetext.SetWindowTextW(_T("�ȴ����ּ���"));

}


//������Ϣ
bool CgobangDlg::sendmsg(void)
{
	//MessageBox(_T("��������"));
	int ret = send(m_socket, sendbuf, sizeof(sendbuf), 0);
	if (ret <=0)	MessageBox(_T("������Ϣʧ�ܣ�����������Ͽ�����"));
	return ret;
}
//������������
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

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CgobangDlg::OnBnClickedOk()
{
	
		endconnect();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}



//�����߳�

DWORD WINAPI CgobangDlg::chat(LPVOID param)
{
	
	CgobangDlg* p = (CgobangDlg*)param;

	char buffer[100] = { '\0' };
	*(int *)buffer = 5;
	CString str = p->m_words;
	if (!(p->m_words.IsEmpty()))
	{ 
		int n = str.GetLength(); //��ȡstr���ַ���  
		int len = WideCharToMultiByte(CP_ACP, 0, str, n, NULL, 0, NULL, NULL); //��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����  

		WideCharToMultiByte(CP_ACP, 0, str, n,buffer+4, len, NULL, NULL); //���ֽڱ���ת���ɶ��ֽڱ���  
		
		if(p->m_socket==INVALID_SOCKET)
			str = _T("��Ϣ����ʧ��");
		else
		{
			int ret = send(p->m_socket, buffer, 100, 0);
			if (ret <= 0)
			{
				p->MessageBox(_T("����ʧ��"));

			}
		}
		str = _T("�ң� ") + str + '\n';
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
		m_showwords.ReplaceSel( _T("ϵͳ��δƥ�䵽����"));
		
		
	}
}


