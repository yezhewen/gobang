#include"stdafx.h"
#include"Chess.h"	

chess::chess() //析构函数
{
	ZeroMemory(data, sizeof(data));
	//data[14][14] = { 0 };
	win = 0;
	last_x = last_y = -1;
}

bool chess::reset() //重置
{
	ZeroMemory(data, sizeof(data));
	win = 0;
	last_x = last_y = -1;
	return true;

}

bool chess::set(int x, int y, int color) //落子并检查是否胜利
{
	data[x][y] = color;
	last_x = x;
	last_y = y;

	int line,row,count,dis;
	//横向 竖向 左上 右上 四个方向检查有无五子连珠
	int move[8][2] = { {-1,0},{1,0},{0,-1},{0,1},{-1,-1},{1,1},{-1,1},{1,-1} };
	for (int i=0; i < 8; i++)
	{
		//同一条线往两个方向查找
		count = 1;//最长连珠
		
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
		if (count >= 5)//出现五子连珠 退出
		{
			win = color;
			break;
		}

	}
	
	return true;
}

int chess::iswin() //是否胜利
{
	return win;
}

bool chess::undo() //悔棋
{
	if (last_x == -1||data[last_x][last_y]==0) return false;
	data[last_x][last_y] = 0;
}
int chess::getdata(int x,int y)//获取数据
{
	if (x < 0 || x>14 || y < 0 || y>14)
		return -1;
	return data[x][y];
}