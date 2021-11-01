#include <glut.h>
#include <iostream>
#include <fstream>
#include <string>
#define COUNT 8
using namespace std;

// SCREEN SIZE : (500 * 500)
// TYPE : 1(Square), 2(Triangle), 3(Circle)

// < Data Format >
// (TYPE=1)-> TYPE RED GREEN BLUE X1 X2 Y1 Y2
// (TYPE=2)-> TYPE RED GREEN BLUE X1 X2 X3 Y1 Y2 Y3
// (TYPE=3)-> TYPE RED GREEN BLUE X1 Y1 RADIUS

// 하나의 int형 배열에 모든 데이터 값을 입력 받은 뒤, TYPE값에 따라 다른 그리기 구현

static int data[COUNT][10];

struct information{
	double x1, x2;
	double y1, y2;
	double m;
};

void display()
{
	glClear( GL_COLOR_BUFFER_BIT );	
	glBegin( GL_POINTS );

	for( int m=0; m<COUNT; m++)
	{
		for( int i=0; i<=500; i++)
		for( int j=0; j<=500; j++)
		{	
			if(data[m][0] == 1) // 사각형 그리기
			{
				// (x2 > x1), (y2 > y1) 가정
				if((i>=data[m][4] && i<=data[m][5]) && (j>=data[m][6] && j<= data[m][7]))
				{
					glColor3d(data[m][1], data[m][2], data[m][3]);
					glVertex2i(i,j);
				}
			}

			else if(data[m][0] == 2) // 삼각형 그리기
			{
				information info[3];
				double m1 = ((double)data[m][8]-data[m][7])/(data[m][5]-data[m][4]); // (y2-y1)/(x2-x1)
				double m2 = ((double)data[m][9]-data[m][8])/(data[m][6]-data[m][5]); // (y3-y2)/(x3-x2)
				double m3 = ((double)data[m][9]-data[m][7])/(data[m][6]-data[m][4]); // (y3-y1)/(x3-x1)

				info[0].m = m1; // (x1,y1), (x2,y2)을 연결하는 직선
				info[1].m = m2; // (x3,y3), (x2,y2)을 연결하는 직선
				info[2].m = m3; // (x3,y3), (x1,y1)을 연결하는 직선

				info[0].x1 = data[m][4];
				info[0].x2 = data[m][5];
				info[0].y1 = data[m][7];
				info[0].y2 = data[m][8];

				info[1].x1 = data[m][6];
				info[1].x2 = data[m][5];
				info[1].y1 = data[m][9];
				info[1].y2 = data[m][8];

				info[2].x1 = data[m][6];
				info[2].x2 = data[m][4];
				info[2].y1 = data[m][9];
				info[2].y2 = data[m][7];

				for(int p=0; p<3; p++) // 기울기 내림차순 정렬
				{
					int index = p;
					for(int q=p+1; q<3; q++)
					{
						if(info[index].m < info[q].m)
							index = q;
					}
					if(p!=index)
					{
						information temp = info[p];
						info[p] = info[index];
						info[index] = temp;
					}
				}

				// 직선의 기울기를 통해 직선간의 위치를 결정해야한다.
				// 기울기 크기 순서를 info[0].m > info[1].m > info[2].m라 할때, 네 가지 유형에 대해 생각해볼 수 있다.
				// 1) (info[0].m > 0, info[1].m > 0, info[2].m > 0) (영역 범위 : 아래,위,아래)
				// 2) (info[0].m > 0, info[1].m > 0, info[2].m < 0) (영역 범위 : 아래,위,아래)
				// 3) (info[0].m > 0, info[1].m < 0, info[2].m < 0) (영역 범위 : 아래,위,위)
				// 4) (info[0].m < 0, info[1].m < 0, info[2].m < 0) (영역 범위 : 위,아래,위)
				
				if(info[0].m >= 0 && info[1].m >= 0 && info[2].m >= 0)
				{	// P1(200,200), P2(450,450), P3(250,400)
					if((j<=(((info[0].y2-info[0].y1)/(info[0].x2-info[0].x1))*(i-info[0].x1)+info[0].y1)) && (j>=((((info[1].y2-info[1].y1)/(info[1].x2-info[1].x1)))*(i-info[1].x1)+info[1].y1)) && (j<=(((info[2].y2-info[2].y1)/(info[2].x2-info[2].x1))*(i-info[2].x1)+info[2].y1))) // (영역 범위 : 아래,위,아래)
					{ 
						glColor3d(data[m][1], data[m][2], data[m][3]);
						glVertex2i(i,j);
					}
				}
				
				else if(info[0].m > 0 && info[1].m > 0 && info[2].m < 0)
				{	// P1(400,200), P2(200,0), P3(450,150)
					if((j<=(((info[0].y2-info[0].y1)/(info[0].x2-info[0].x1))*(i-info[0].x1)+info[0].y1)) && (j>=((((info[1].y2-info[1].y1)/(info[1].x2-info[1].x1)))*(i-info[1].x1)+info[1].y1)) && (j<=(((info[2].y2-info[2].y1)/(info[2].x2-info[2].x1))*(i-info[2].x1)+info[2].y1))) // (영역 범위 : 아래,위,아래)
					{
						glColor3d(data[m][1], data[m][2], data[m][3]);
						glVertex2i(i,j);
					}
				}

				else if(info[0].m > 0 && info[1].m < 0 && info[2].m < 0)
				{	// P1(0,300), P2(100,100), P3(200,0)
					if((j<=(((info[0].y2-info[0].y1)/(info[0].x2-info[0].x1))*(i-info[0].x1)+info[0].y1)) && (j>=((((info[1].y2-info[1].y1)/(info[1].x2-info[1].x1)))*(i-info[1].x1)+info[1].y1)) && (j<=(((info[2].y2-info[2].y1)/(info[2].x2-info[2].x1))*(i-info[2].x1)+info[2].y1))) // (영역 범위 : 아래,위,아래)
					{
						glColor3d(data[m][1], data[m][2], data[m][3]);
						glVertex2i(i,j);
					}
				}

				else if(info[0].m < 0 && info[1].m < 0 && info[2].m < 0)
				{	// P1(200,150), P2(150,100), P3(250,0)
					if((j>=(((info[0].y2-info[0].y1)/(info[0].x2-info[0].x1))*(i-info[0].x1)+info[0].y1)) && (j<=((((info[1].y2-info[1].y1)/(info[1].x2-info[1].x1)))*(i-info[1].x1)+info[1].y1)) && (j>=(((info[2].y2-info[2].y1)/(info[2].x2-info[2].x1))*(i-info[2].x1)+info[2].y1))) // (영역 범위 : 위,아래,위)
					{
						glColor3d(data[m][1], data[m][2], data[m][3]);
						glVertex2i(i,j);
					}
				}
			}

			else if(data[m][0] == 3) // 원 그리기
			{
				// 원의 방정식 : x^2 + y^2 = r^2
				if(((i-data[m][4])*(i-data[m][4]) + (j-data[m][5])*(j-data[m][5])) <= data[m][6]*data[m][6])
				{	
					glColor3d(data[m][1], data[m][2], data[m][3]);
					glVertex2i(i,j);
				}
			}
		}
	}
	glEnd();
	glFlush();
}

int main(int argc, char **argv)
{
	int i = 0;
	int j = 0;
	ifstream readFile;
	readFile.open("data.txt");

	if(readFile.is_open())
	{
		while(!readFile.eof())
		{	j=0;
			string str;
			getline(readFile, str);
			char input[100];
			strcpy(input, str.c_str());
			char *token = strtok(input," ");
			while(token!=NULL)
			{		
					// cout << token << endl;
					data[i][j] = atoi(token);
					j++;
					token = strtok(NULL," ");
			}
			// cout << endl << endl;
			i++;
		}
	}
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowPosition(250, 250);
	glutInitWindowSize( 501,501 );
	glutCreateWindow( "The Painting" );
	glClearColor( 1.0, 1.0, 0.0, 0.0 );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( 0.0, 500.0, 0.0, 500.0, -1.0, 1.0 );
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}