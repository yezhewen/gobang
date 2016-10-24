

// 
class chess
{
private:
	int data[15][15];
	int win;
	int last_x, last_y;

public:
	chess();
	bool set(int x, int y, int color);
	bool reset();
	bool undo();
	int iswin();
	int getdata(int x, int y);
	

};