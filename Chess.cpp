#include"stdafx.h"
#include"Chess.h"	

chess::chess() //��������
{
	ZeroMemory(data, sizeof(data));
	//data[14][14] = { 0 };
	win = 0;
	last_x = last_y = -1;
}

bool chess::reset() //����
{
	ZeroMemory(data, sizeof(data));
	win = 0;
	last_x = last_y = -1;
	return true;

}

bool chess::set(int x, int y, int color) //���Ӳ�����Ƿ�ʤ��
{
	data[x][y] = color;
	last_x = x;
	last_y = y;

	int line,row,count,dis;
	//���� ���� ���� ���� �ĸ�������������������
	int move[8][2] = { {-1,0},{1,0},{0,-1},{0,1},{-1,-1},{1,1},{-1,1},{1,-1} };
	for (int i=0; i < 8; i++)
	{
		//ͬһ�����������������
		count = 1;//�����
		
		line =x+move[i][0], row = y+move[i][1];
		dis = 5;
		while (--dis&&line >= 0 && line <= 14 && row >= 0 && row <= 14&&count<6)
		{
			if (data[line][row] == color) count++;
			else break;
			line += move[i][0];
			row += move[i][1];
		}

		i++;
		line = x + move[i][0], row = y + move[i][1];
		dis = 5;

		while (--dis&&line >= 0 && line <= 14 && row >= 0 && row <= 14&&count<6)
		{
			if (data[line][row] == color) count++;
			else break;
			line += move[i][0];
			row += move[i][1];
		}
		if (count >= 5)//������������ �˳�
		{
			win = color;
			break;
		}

	}
	
	return true;
}

int chess::iswin() //�Ƿ�ʤ��
{
	return win;
}

bool chess::undo() //����
{
	if (last_x == -1||data[last_x][last_y]==0) return false;
	data[last_x][last_y] = 0;
}
int chess::getdata(int x,int y)//��ȡ����
{
	if (x < 0 || x>14 || y < 0 || y>14)
		return -1;
	return data[x][y];
}