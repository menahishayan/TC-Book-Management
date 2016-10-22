#include <iostream.h>
#include <conio.h>
#include <fstream.h>
#include <string.h>
#include <process.h>
#include <stdio.h>
#include <iomanip.h>
#include <stdlib.h>
#include <graphics.h>
#include <time.h>
#include <ctype.h>
#include "../bin/lib_mgmt/errno2.h"
#include <dos.h>

#define true 1
#define false 0

typedef int bool;

char bID[]="7783BN00", mID[]="7783MB00", tID[]="7783TR00";
char bLib_Dir[50]="../bin/lib_mgmt/books/second.bin", mLib_Dir[50]="../bin/lib_mgmt/borrow/second.bin", tLib_Dir[50]="../bin/lib_mgmt/trans/third.bin";

char Void[]="\0";

int midx, midy;

void header(char text[], bool type = false, bool escReq = false)
{
	clrscr();
	cleardevice();
	settextjustify(1,1);
	settextstyle(3,0,2);
	outtextxy(midx, 40, "LIBRARY MANAGEMENT SYSTEM");
	settextstyle(3,0,1);
	line(0, 80, midx*2, 80);
	outtextxy(midx, 90, text);
	line(0, 110, midx*2, 110);
	
	settextjustify(0,1);
	
	if(type)
	{
		outtextxy(midx/3, (midy*3/2)-4, "Choice: ");
		gotoxy(23,23);
	}
	
	line(0, midy*2-30, midx*2, midy*2-30);
	if(escReq)	outtextxy(20, midy*2-20, "Press (ESC) to return to previous menu");
	else outtextxy(20, midy*2-20, "Status: Ready");
	
	if(!type)	settextjustify(1,1);
	line(0, midy*2, midx*2, midy*2);
}

class colorScheme
{
	int f_col, col;
	fstream saveColor;

public:

	colorScheme()
	{
		saveColor.open("../bin/lib_mgmt/colors.bin", ios::in | ios::binary);
		if(saveColor.fail())
		{
			perror("Color scheme error");
			getch();
			abort();
		}
		saveColor>>f_col>>col;
		setcolor(f_col);
		setbkcolor(col);
		textcolor(f_col);
		textbackground(col);
		saveColor.close();
	}

	void picker()
	{
		char getcol='1';
		while(getcol!=13)
		{
			cleardevice();
			setcolor(f_col);
			setbkcolor(col);
			settextjustify(1,1);
			settextstyle(3,0,2);
			outtextxy(midx, midy-50, "Pick A Color Scheme");
			settextstyle(3,0,1);
			outtextxy(midx, midy, "Change Foreground (F)");
			outtextxy(midx, midy+20, "Change Background (B)");
			outtextxy(midx, midy+80, "Set Color (ENTER)");
			getcol = getch();
			if(getcol=='b' | getcol=='B')
			{
				if(col<7)	col++;
				else col=0;
				if(col == f_col) col++;
			}
			else if(getcol=='f' | getcol=='F')
			{
				if(f_col<15)	f_col++;
				else f_col=0;
				if(col == f_col) f_col++;
			}
		}
		saveColor.open("../bin/lib_mgmt/colors.bin", ios::out | ios::binary);
		saveColor<<f_col<<'\n'<<col;
		saveColor.close();
	}
	
	void reset()
	{
		setcolor(f_col);
		setbkcolor(col);
	}
	
	int bCol()
	{
		return col;
	}
	
	int fCol()
	{
		return f_col;
	}
};

struct errorHandler
{
	int loopCounter;
	
	clock_t start, end;
	float elapsed;
	
	void init()
	{
		loopCounter=0;
		start = clock();
	}
	
	void tick()
	{
		loopCounter++;
		
		end = clock();
		elapsed = (end - start) / CLK_TCK;
		
		if(elapsed >= 2.0)
		{
			header("Internal Fatal Error");
			perror(-99);
			closegraph();
			abort();
		}
	}
	
	void perror(int val = -10)
	{
		settextjustify(1,1);
		
		if(val != -10) errno = val;
		
		switch(errno)
		{
			case ENOENT:	outtextxy(midx, midy+90, "Error: No such file or directory");
							break;
							
			case EACCES:	outtextxy(midx, midy+90, "Error: Permission denied");
							break;
							
			case ENOTSAM:	outtextxy(midx, midy+90, "Error: Not same device");
							break;
							
			case ENOFNSH:	outtextxy(midx, midy, "Error: Unfinished part of program. Press any key to return");
							break;
							
			case ETIMOUT:	outtextxy(midx, midy, "Error: The operation timed out. Press any key to continue");
							break;
							
			case ENOLOAD:	outtextxy(midx, midy, "Error: No file loaded. Please load a file from the \'File\' menu first");
							break;
							
			case EEXIST:	outtextxy(midx, midy+90, "Error: Invalid file name. Press any key to re-enter");
							break;
							
			case ENODATE:	outtextxy(midx, midy+90, "Error: Invalid date. Press any key to re-enter");
							break;
							
			case ENOREC:	outtextxy(midx, midy+90, "Error: Record does not exist.");
							break;
							
			case EREXIST:	outtextxy(midx, midy+90, "Error: Transaction already completed. Press any key to return");
							break;
							
			case ELIDATE:	outtextxy(midx, midy+90, "Error: Impractical date. Press any key to re-enter");
							break;
							
			case EISSUED:	outtextxy(midx, midy+90, "Error: Modifying disabled. Please return all copies first");
							break;
							
			case ENOAVAL:	outtextxy(midx, midy+90, "Error: Out of stock. Press any key to cancel");
							break;
							
			default: 		char perr[100];
							sprintf(perr, "Error: Unknown error occurred (%d). Press any key to abort", errno);
							outtextxy(midx, midy+90, perr);
		}
		
		getch();
		
		colorScheme clrerr;
		setfillstyle(1, clrerr.bCol());
		if(errno == ENOFNSH || errno == ENOLOAD || errno == ETIMOUT)
			bar(0, midy-10, midx*2, midy+10);
		else
			bar(0, midy+80, midx*2, midy+100);
				
		errno = ENOFNSH;
	}
	
}globalErrorHandler;

void _cin(char *str, unsigned len, bool onlyAlpha = false, int posy = 195, int posx = 130)
{
	int currentPos = posx;
	
	settextstyle(3,0,1);
	settextjustify(1,1);
	
	colorScheme input;
	
	for(unsigned i=0; i<len-1; i++)
	{
		back: str[i] = getch();
		
		if(str[i] != 13 && str[i] != 27)
		{
			if(onlyAlpha)
			{
				if(!isalpha(str[i]) && str[i] != 8 && str[i] != '\'' && !isspace(str[i]))
				{
					i--;
					continue;
				}
			}
			
			if(str[i] == 8 && i > 0)
			{
				setfillstyle(1, input.bCol());
				str[i+1] = '\0';
				i--;
				bar(currentPos - textwidth(&str[i]) -5, posy -5, currentPos, posy + textheight(&str[i]));
				currentPos -= textwidth(&str[i]);
				
				goto back;
			}
			
			else if(str[i] == 8 && !(i > 0))
			{
				i--;
				continue;
			}
			
			else
			{
				if(str[i] != 8)
				{
					str[i+1] = '\0';
				
					if(i == 0)	str[i] = toupper(str[i]);
					
					if(str[i] == 'i' || str[i] == 'I' || str[i] == 'l' || str[i] == 'L' || str[i] == '\'')	outtextxy(currentPos-3, posy, &str[i]);
	//				else if(str[i-1] == 'i' || str[i-1] == 'I' || str[i-1] == 'l' || str[i-1] == 'L')	outtextxy(currentPos, posy, &str[i]);
					else outtextxy(currentPos, posy, &str[i]);
					
					currentPos += textwidth(&str[i]);
				}
			}
		}
		else break;
	}
	str[i] = '\0';
	//outtextxy(posx, posy+30, str);
	
	settextjustify(2,1);
}

void _cin(int &final, int posy = 195, int posx = 130)
{
	char str[6];
	
	int currentPos = posx;
	
	settextstyle(3,0,1);
	settextjustify(1,1);
	
	colorScheme input;
	
	for(int i=0; i<5; i++)
	{
		back: str[i] = getch();
		
		if(str[i] != 13 && str[i] != 27)
		{
			if(!isdigit(str[i]) && str[i] != 8)
			{
				i--;
				continue;
			}
			
			if(str[i] == 8)
			{
				setfillstyle(1, input.bCol());
				str[i+1] = '\0';
				i--;
				bar(currentPos - textwidth(&str[i]) -5, posy -5, currentPos, posy + textheight(&str[i]));
				currentPos -= textwidth(&str[i]);
				
				goto back;
			}
			
			else
			{
				str[i+1] = '\0';
				outtextxy(currentPos, posy, &str[i]);
				currentPos += textwidth(&str[i]);
			}
		}
		else break;
	}
	str[i] = '\0';
	//outtextxy(posx, posy+30, str);
	final = atoi(str);
	
	settextjustify(2,1);
}

void _cin(float &final, int posy = 195, int posx = 130)
{
	char str[8];
	
	int currentPos = posx;
	
	settextstyle(3,0,1);
	settextjustify(1,1);
	
	colorScheme input;
	
	for(int i=0; i<7; i++)
	{
		back: str[i] = getch();
		
		if(str[i] != 13 && str[i] != 27)
		{
			if(!isdigit(str[i]) && str[i] != 8)
			{
				i--;
				continue;
			}
			
			if(str[i] == 8)
			{
				setfillstyle(1, input.bCol());
				str[i+1] = '\0';
				i--;
				bar(currentPos - textwidth(&str[i]) -5, posy -5, currentPos, posy + textheight(&str[i]));
				currentPos -= textwidth(&str[i]);
				
				goto back;
			}
			
			else
			{
				str[i+1] = '\0';
				outtextxy(currentPos, posy, &str[i]);
				currentPos += textwidth(&str[i]);
			}
		}
		else break;
	}
	str[i] = '\0';
	//outtextxy(posx, posy+30, str);
	final = atof(str);
	
	settextjustify(2,1);
}

void makeDir(char Dir[], char name[], bool isCSV = false)
{
	char type = Dir[18];
	
	switch(type)
	{
		case 'o':	strcpy(Dir, "../bin/lib_mgmt/books/");
					break;
					
		case 'r':	strcpy(Dir, "../bin/lib_mgmt/borrow/");
					break;
					
		case 'a':	strcpy(Dir, "../bin/lib_mgmt/trans/");
					break;
	}
	
	strcat(Dir, name);
	if(isCSV)	strcat(Dir, ".csv");
	else	strcat(Dir, ".bin");
}

void makeDir(char Dir[])
{
	char type = Dir[18];
	
	switch(type)
	{
		case 'o':	strcpy(Dir, "../bin/lib_mgmt/books/");
					break;
					
		case 'r':	strcpy(Dir, "../bin/lib_mgmt/borrow/");
					break;
					
		case 'a':	strcpy(Dir, "../bin/lib_mgmt/trans/");
					break;
	}
}

char* breakDir(char Dir[])
{
	char type = Dir[18];
	int len = strlen(Dir);
	char fname[9];
	
	int i;
	
	switch(type)
	{
		case 'o':	i = 22;
					break;
					
		case 'r':	i = 23;
					break;
					
		case 'a':	i = 22;
					break;
	}
	
	int j=0;
	
	while(Dir[i]!='.')
	{
		fname[j] = Dir[i];
		i++;
		j++;
	}
	
	fname[j] = '\0';
	
	return fname;
}

class coreUtilities
{
protected:

	char ePath[50], type;
	
	bool univExportUtil(char fname[])
	{
		type = toupper(type);
		switch(type)
		{
			case 'B':	strcpy(ePath, "../bin/lib_mgmt/books/");
						break;
			
			case 'M':	strcpy(ePath, "../bin/lib_mgmt/borrow/");
						break;
			
			case 'T':	strcpy(ePath, "../bin/lib_mgmt/trans/");
						break;
		}
		
		makeDir(ePath, fname, true);
		const char *const_ePath = ePath;
		
		ifstream checkStat(const_ePath, ios::in | ios::binary);
		
		if(checkStat.fail())	return true;
		
		char eofCheck;
		checkStat >> eofCheck;
		if(checkStat.eof())	return true;	//if header required => true
		//checkStat.close();
		else return false;
	}
};

class Book : private coreUtilities
{
	char barcode[8], title[20], author[20], publisher[20];
	char p_title[20];
	int copies, issued;
	float price;

public:
	book()
	{
		copies = issued = price = 0;
	}

	char* ShareID()
	{
		return barcode;
	}
	
	char* ShareTitle()
	{
		return p_title;
	}
	
	bool isIssued()
	{
		if(!issued)	return false;
		else return true;
	}
	
	bool isAvailable()
	{
		if(issued == copies)	return false;
		else return true;
	}

//coreUtilities::getData;
//coreUtilities::Display;
//coreUtilities::univExport;

	void getData()
	{
		strcpy(barcode, bID);
		outtextxy(midx, 130, barcode);
		
		settextjustify(2,1);
		
		outtextxy(115, 195, "Title: ");
			_cin(title, sizeof(title));
			strcpy(p_title, title);
			
		outtextxy(115, 225, "Author: ");
			_cin(author, sizeof(author), true, 225);
			
		outtextxy(115, 258, "Publisher: ");
			_cin(publisher, sizeof(publisher), true, 258);
		
		outtextxy(115, 290, "Copies: ");
			_cin(copies, 290);
			
		outtextxy(115, 322, "Price: ");
			_cin(price, 322);
			
		strcpy(title, Void);
		
		settextjustify(1,1);
		outtextxy(midx, 380, "Book added successfully.");
	}

	void getData(bool select[])
	{
		header("Update Book");
		settextjustify(2,1);
		
		int padding[] = {195, 225, 258, 290, 322, 352, 380};
		int padVar=0;
		
		if(select[0])
		{
			outtextxy(115, padding[padVar], "Title: ");	_cin(title, sizeof(title), false, padding[padVar]);	strcpy(p_title, title);		strcpy(title, Void);
			padVar++;
		}
		if(select[1])
		{
			outtextxy(115, padding[padVar], "Author: "); _cin(author, sizeof(author), true, padding[padVar]);
			padVar++;
		}
		if(select[2])
		{
			outtextxy(115, padding[padVar], "Publisher: "); _cin(publisher, sizeof(publisher), true, padding[padVar]);
			padVar++;
		}
		if(select[3])
		{
			outtextxy(115, padding[padVar], "Copies: "); _cin(copies, padding[padVar]);
			padVar++;
		}
		if(select[4])
		{
			outtextxy(115, padding[padVar], "Price: "); _cin(price, padding[padVar]);
			padVar++;
		}
		
		settextjustify(1,1);
		padVar++;
		outtextxy(midx, padding[padVar], "Book modified.");
	}
	
	float SharePrice()
	{
		return price;
	}

	void transact(char arg)
	{
		if(arg == 'b')	issued++;
		if(arg == 'r')	issued--;
	}

	void Display()
	{
		char content[7][50];
		sprintf(content[0], "Barcode: %s", barcode);
		sprintf(content[1], "Title: %s", p_title);
		sprintf(content[2], "Author: %s", author);
		sprintf(content[3], "Publisher: %s", publisher);
		sprintf(content[4], "Total Stock: %d copies", copies);
		sprintf(content[5], "Remaining Stock: %d copies", copies-issued);
		sprintf(content[6], "Price: Rs.%f", price);
		
		settextjustify(1,1);
		outtextxy(midx, 180, content[0]);
		outtextxy(midx, 210, content[1]);
		settextjustify(0,1);
		outtextxy(90, 240, content[2]);
		outtextxy(midx+30, 240, content[3]);
		outtextxy(90, 270, content[4]);
		outtextxy(midx+30, 270, content[5]);
		settextjustify(1,1);
		outtextxy(midx, 300, content[6]);
	}
	
	void univExport(char fname[])
	{
		coreUtilities::type = barcode[4];
		bool requiresHead = coreUtilities::univExportUtil(fname);
		
		const char *const_ePath = coreUtilities::ePath;
		
		ofstream univExp_Dest(const_ePath, ios::app | ios::binary);
		
		if(requiresHead)	univExp_Dest<<"\"Barcode\",\"Title\",\"Author\",\"Publisher\",\"Copies\",\"Issued\",\"Price\"";
		univExp_Dest << "\n\"" << barcode << "\",\"" << p_title << "\",\"" << author << "\",\"" << publisher << "\",\"" << copies << "\",\"" << issued << "\",\"" << price << "\"";
		univExp_Dest.close();
	}
	
}b, book;

/*class coreUtilities : private Book
{
	char ePath[50], type;
	
	bool univExportUtil(char fname[])
	{
		switch(type)
		{
			case 'B':	strcpy(ePath, "../bin/lib_mgmt/books/");
						break;
			
			case 'M':	strcpy(ePath, "../bin/lib_mgmt/borrow/");
						break;
			
			case 'T':	strcpy(ePath, "../bin/lib_mgmt/trans/");
						break;
		}
		
		makeDir(ePath, fname, true);
		const char *const_ePath = ePath;
		
		ifstream checkStat(const_ePath, ios::in | ios::binary);
		
		if(checkStat.fail())	return true;
		
		char eofCheck;
		checkStat >> eofCheck;
		if(checkStat.eof())	return true;	//if header required => true
		//checkStat.close();
		else return false;
	}

public:

	coreUtilities(Book &bcopy) : Book(bcopy)
	{;}

	void univExport(char fname[])
	{
		type = barcode[4];
		bool requiresHead = univExportUtil(fname);
		
		const char *const_ePath = ePath;
		
		ofstream univExp_Dest(const_ePath, ios::app | ios::binary);
		
		if(requiresHead)	univExp_Dest<<"\"Barcode\",\"Title\",\"Author\",\"Publisher\",\"Copies\",\"Issued\",\"Price\"";
		univExp_Dest << "\n\"" << barcode << "\",\"" << p_title << "\",\"" << author << "\",\"" << publisher << "\",\"" << copies << "\",\"" << issued << "\",\"" << price << "\"";
		univExp_Dest.close();

	}
};*/

struct Date	
{
	unsigned day, month, year;
	void set(char _t[], int padding=0)
	{
		int d, m, y;
		char text[20];
		
		colorScheme patchCol;
		setfillstyle(1, patchCol.bCol());
		bar(200, 210 + (padding*30), 400, 240 + (padding*30));
		
		sprintf(text, "Enter Date %s: ", _t);
		outtextxy(215, 225 + (padding*30), text);
		outtextxy(215, 245 + (padding*30), "(DD MM YYYY)");
		
//		gotoxy(31,15 + (padding*2));	cin>>d;
//		gotoxy(34,15 + (padding*2));	cin>>m;
//		gotoxy(37,15 + (padding*2));	cin>>y;
		outtextxy(350, 225 + (padding*30), "   /    / ");
		_cin(d, 225 + (padding*30), 250);
		_cin(m, 225 + (padding*30), 300);
		_cin(y, 225 + (padding*30), 350);
		
		if(isValid(d,m,y))
		{
			day = d;
			month = m;
			year = y;
		}
		
		else
		{
			settextjustify(1,1);
			globalErrorHandler.perror(ENODATE);
			settextjustify(2,1);
			//getch();
			set(_t, padding);
		}
	}
	
	void sysdate()
	{
		date sysdate;
		getdate(&sysdate);
		
		day = sysdate.da_day;
		month = sysdate.da_mon;
		year = sysdate.da_year;
	}
	
	bool isValid(int &d, int &m, int &y)
	{
		if (!(1582<= y))
			return false;
			
		if (!(9000>= y))
			return false;
			
		if (!(1<= m && m<=12))
		 	return false;
			
		if (!(1<= d && d<=31))
		 	return false;
			
		if ((d==31) && (m==2 || m==4 || m==6 || m==9 || m==11))
		 	return false;
			
		if ((d==30) && (m==2))
		 	return false;
			
		if ((m==2) && (d==29) && (y%4!=0))
		 	return false;
			
		if ((m==2) && (d==29) && (y%400==0))
		 	return true;
			
		if ((m==2) && (d==29) && (y%100==0))
		 	return false;
			
		if ((m==2) && (d==29) && (y%4==0))
		 	return true;

		return true;
	}

	void display(char _t[], int posx = midx, int posy = 225)
	{
		static const char mName[12][5]={"Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sep", "Oct", "Nov", "Dec"};
		char date_str[30];
		sprintf(date_str, "Date %s: %s %d, %d", _t, mName[month-1], day, year);
		outtextxy(posx, posy, date_str);
	}
	
	static bool isGreater(Date &, Date &);
} today;

bool Date::isGreater(Date &d1, Date &d2)
{
	if(d1.year > d2.year)
		return 1;
		
	else if(d1.year < d2.year)
		return -1;
	
	else if(d1.month > d2.month)
		return 1;
		
	else if(d1.month < d2.month)
		return -1;
	
	else if(d1.day > d2.day)
		return 1;
		
	else if(d1.day < d2.day)
		return -1;
	
	else return 0;
}

void dialog(int posx, int posy, char label[], char xlen_t[], int ylen, int padx=0)
{
	colorScheme dilCol;
	settextstyle(0,0,0);
	
	if(ylen==0) ylen++;
	
	int xlen = textwidth(xlen_t);
	rectangle(posx-15, posy, posx + (xlen) + 15, posy + ((ylen+1)*20) - 5);
	
	setfillstyle(1, dilCol.bCol());
	settextjustify(0,1);
	//bar(padx/2 + ((posx) + xlen - textwidth(label))/2 - 10, posy - 10, padx/2 + ((posx) + xlen + textwidth(label))/2 + 10, posy + 10);
	bar(padx/8 + ((posx + ((xlen)*2) + 15)/2) - (textwidth(label)/2) -10, posy - 10, padx/8 + ((posx + ((xlen)*2) + 15)/2) + (textwidth(label)/2) +10, posy + 10);
	
	outtextxy((padx/8) + ((posx + (xlen*2) + 15)/2) - (textwidth(label)/2), posy, label);
}

class Borrower : private coreUtilities
{
	char id[8];
	char mName[20];
	char borrowed[20][9], returned[20][9]; //using barcode number
//	char p_borrowed[20][8], p_returned[20][8];	for debugging only. remove later and reset file data.
	char p_name[20];
	int bIndex, rIndex;
	Date joined;

public:

	Borrower()
	{
		bIndex = rIndex = 0;
	}

	char* ShareID()
	{
		return id;
	}
	
	char* ShareName()
	{
		return p_name;
	}

	void getData()
	{
		strcpy(id, mID);
		outtextxy(midx, 130, id);

		settextjustify(2,1);

		outtextxy(215, 195, "Name: ");
			_cin(mName, sizeof(mName), false, 195, 230);
			strcpy(p_name, mName);	strcpy(mName, Void);

	reIn1:	joined.set("Joined");

		if(Date::isGreater(today, joined) == -1)
		{
			globalErrorHandler.perror(ELIDATE);
			settextjustify(2,1);
			goto reIn1;
		}

		settextjustify(1,1);
		outtextxy(midx, 290, "Borrower registered.");
	}

	void getData(bool select[])
	{
		header("Update Borrower");
		settextjustify(2,1);

		if(select[0])
		{
			outtextxy(215, 195, "Name: ");	_cin(mName, sizeof(mName), false, 195, 230);	strcpy(p_name, mName);	strcpy(mName, Void);
		}
		if(select[1])
		{
		reIn2:	joined.set("Joined");
			
			if(Date::isGreater(today, joined) == -1)
			{
				globalErrorHandler.perror(ELIDATE);
				settextjustify(2,1);
				goto reIn2;
			}
		}
		
		settextjustify(1,1);
		
		outtextxy(midx, 290, "Borrower details updated.");
	}
	
	void transact(char arg, char val[])
	{
		if(arg == 'b')
		{
			strcpy(borrowed[bIndex], val);
			//strcpy(p_borrowed[bIndex], borrowed[bIndex]);
			borrowed[bIndex][8] = '\0';
			strupr(borrowed[bIndex]);
			
			//cout<<"val1: "<<p_borrowed[bIndex]<<"\nval2: "<<val<<endl;
			bIndex++;
		}
		if(arg == 'r')
		{
			strcpy(returned[rIndex], val);
			//strcpy(p_returned[rIndex], returned[rIndex]);
			returned[rIndex][8] = '\0';
			strupr(returned[rIndex]);
			rIndex++;
		}
//		cout<<id<<": Transacted.";
	}
	
	void ViewLog()
	{
		//if(arg == 'b')
		//{
			//cout<<"Borrow Log \n______________________\n\n";
			if(bIndex > 7)
				dialog(100+20, midy+50, "Borrow Log", "___________________", 6, -200);
				
			else
				dialog(100+20, midy+50, "Borrow Log", "___________________", bIndex, -200);
				
			char li[15];
			if(!bIndex)	outtextxy(115+20, midy+70, "Inventory Empty");
			else
			{
				int j = 0;
				for(int i=0; i<bIndex; i++)
				{
					//cout<<i+1<<". "<<borrowed[i]<<endl;
					if(bIndex > 7)
					{
						if(bIndex - i <= 7)
						{
							sprintf(li, "%d. %s", i+1, borrowed[i]);
							outtextxy(115+20, midy+70+(j*15), li);
							j++;
						}
					}
					else
					{
						sprintf(li, "%d. %s", i+1, borrowed[i]);
						outtextxy(115+20, midy+70+(i*15), li);
					}
				}
			}
			
			
		/*}
		if(arg == 'r')
		{*/
			//cout<<"Return Log \n______________________\n\n";
			if(rIndex > 7)
				dialog(midx+100-50, midy+50, "Return Log", "___________________", 6, midx*3-150);
				
			else
				dialog(midx+100-50, midy+50, "Return Log", "___________________", rIndex, midx*3-150);
				
			if(!rIndex)	outtextxy(midx+115-50, midy+70, "Inventory Empty");
			else
			{	
				int j = 0;
				for(int i=0; i<rIndex; i++)
				{
					//cout<<i+1<<". "<<returned[i]<<endl;
					if(rIndex > 7)
					{
						if(rIndex - i <= 7)
						{
							sprintf(li, "%d. %s", i+1, returned[i]);
							outtextxy(midx+115-50, midy+70+(j*15), li);
							j++;
						}
					}
					else
					{
						sprintf(li, "%d. %s", i+1, returned[i]);
						outtextxy(midx+115-50, midy+70+(i*15), li);
					}
				}
			}
			
		//}
		settextstyle(3,0,1);
	}

	void Display()
	{
		outtextxy(midx, 130, id);
		settextjustify(1,1);
		char _name[30];
		sprintf(_name, "Name: %s", p_name);
		outtextxy(midx, 195, _name);
		//outtextxy(midx/2, 225, "Date Joined: ");
		joined.display("Joined", midx, 225);
		
		ViewLog();
		
		settextjustify(1,1);
	}
	
	void univExport(char fname[])
	{
		coreUtilities::type = id[4];
		bool requiresHead = coreUtilities::univExportUtil(fname);
		
		const char *const_ePath = coreUtilities::ePath;
		
		ofstream univExp_Dest(const_ePath, ios::app | ios::binary);
		
		if(requiresHead)	univExp_Dest<<"\"ID\",\"Name\",\"Date Joined\",\"Books Borrowed\",\"Books Returned\"";
		univExp_Dest << "\n\"" << id << "\",\"" << p_name << "\",\"" << joined.day << "\/" << joined.month << "\/" << joined.year << "\",\"" << bIndex << "\",\"" << rIndex << "\"";
		univExp_Dest.close();
	}
	
}m, Borrower;

class transaction : private coreUtilities
{
	char ID[8], bID[9], mID[9];
	Date issued, returned;
	bool returnStatus;
	char p_bID[9];
	int condition;
	float charges;

public:
	transaction()
	{
		charges = 0;
		
	}
	
	char* ShareID()
	{
		return ID;
	}
	
	char* Share_bName()
	{
		const char *const_bLib_Dir = bLib_Dir;
		ifstream readBook(const_bLib_Dir, ios::in | ios::binary);
		
		globalErrorHandler.init();
		
		while(!readBook.eof())
		{
			readBook.read((char*) &book, sizeof(book));
			if(readBook.eof())	break;
			globalErrorHandler.tick();
			
			if(strcmpi(p_bID, book.ShareID()) == 0)
			{
				return book.ShareTitle();
			}
		}
		readBook.close();
	}
	
	char* Share_mName()
	{
		const char *const_mLib_Dir = mLib_Dir;
		ifstream readMem(const_mLib_Dir, ios::in | ios::binary);
		
		globalErrorHandler.init();
		
		while(!readMem.eof())
		{
			readMem.read((char*) &Borrower, sizeof(Borrower));
			if(readMem.eof())	break;
			
			globalErrorHandler.tick();
			
			if(strcmpi(mID, Borrower.ShareID()) == 0)
			{
				return Borrower.ShareName();
			}
			
		}
		readMem.close();
	}
	
	bool isReturned()
	{
		return returnStatus;
	}
	
	char* ShareID(char v)
	{
		if(v == 'b')
			return p_bID;

		else if(v == 'm')
			return mID;
	}
	
	void getData()
	{
		strcpy(ID, tID);
		
		outtextxy(midx, 130, ID);
		settextjustify(2,1);
		
		outtextxy(205, 195, "Book ID: ");
			_cin(bID, sizeof(bID), false, 195, 235);
			strcpy(p_bID, bID);	strcpy(bID, Void);	p_bID[8]='\0';
			
		outtextxy(205, 225, "Borrower ID: ");
			_cin(mID, sizeof(mID), false, 225, 235);
			mID[8]='\0';
			
	reIn3:	issued.set("Issued", 1);
		
		if(Date::isGreater(today, issued) == -1)
		{
			globalErrorHandler.perror(ELIDATE);
			settextjustify(2,1);
			goto reIn3;
		}
		
		returnStatus = false;
		
		condition = 0;
		
		settextjustify(1,1);
		
	}
	
	void getData(bool select[])
	{
		header("Borrow Book");
		settextjustify(2,1);
		
		int padding[] = {195, 225, 258, 290, 322, 352, 380};
		int padVar=0;
		
		if(select[0])
		{
			outtextxy(205, padding[padVar], "Book ID: ");	_cin(bID, sizeof(bID), false, padding[padVar], 235);	strcpy(p_bID, bID);	strcpy(bID, Void);
			padVar++;
		}
		if(select[1])
		{
			outtextxy(205, padding[padVar], "Borrower ID: "); _cin(mID, sizeof(mID), false, padding[padVar], 235);
			padVar++;
		}
		if(select[2])
		{
			//outtextxy(115, padding[padVar], "Publisher: "); gotoxy(16,17);	gets(publisher);
		reIn4:	issued.set("Issued", padVar-2);
			
			if(Date::isGreater(today, issued) == -1)
			{
				globalErrorHandler.perror(ELIDATE);
				settextjustify(2,1);
				goto reIn4;
			}
			
			padVar++;
		}
		
		settextjustify(1,1);
		//padVar++;
		//outtextxy(midx, padding[padVar], "Transaction complete.");
	}
	
	float getPrice()
	{
		const char *const_bLib_Dir = bLib_Dir;
		ifstream readBook(const_bLib_Dir, ios::in | ios::binary);
		
		globalErrorHandler.init();
		while(!readBook.eof())
		{
			readBook.read((char*) &book, sizeof(book));
			if(readBook.eof())	break;
			globalErrorHandler.tick();
			
			if(strcmpi(p_bID, book.ShareID()) == 0)
			{
				//cout<<"\nBook Found\n";
				return book.SharePrice();
				//break;
			}
			
		}
		readBook.close();
	}
	
	void Return()
	{
//		cout<<"Date Returned=";
		header("Return Book");
		settextjustify(2,1);
		returned.set("Returned");
		
		if(Date::isGreater(returned, issued) == -1)
		{
			globalErrorHandler.perror(ELIDATE);
			Return();
		}
		
		header("Return Book", true);
		settextjustify(0,1);
//		cout<<"Return Condition:\n1. Excelent\n2. Good\n3. Fair\n4. Poor\n5. Bad\nChoice: ";
		list("Return Condition:/(1) Excelent/(2) Good/(3) Fair/(4) Poor/(5) Bad/");
		//cin>>condition;
		_cin(condition, (midy*3/2)-4, midx/3 + 70);
		switch(condition)
		{
			case 1: charges=0;
					break;
					
			case 2: charges=getPrice()*0.3;
					break;
					
			case 3: charges=getPrice()*0.5;
					break;
					
			case 4: charges=getPrice()*0.8;
					break;
					
			case 5: charges=getPrice();
					break;
		}
		
reFee:		settextjustify(1,1);
		header("Return Book");
		
		cout.setf(ios::fixed);
		char setdam[100];
		sprintf(setdam, "Apply Rs.%f damage fee? (Y/N)", charges);
		
		outtextxy(midx, midy-90, setdam);
//		cout<<"Apply Rs."<<setprecision(2)<<charges<<" damage fee? (Y/N)";

		char feeChoice = getch();
		tolower(feeChoice);
		if(feeChoice == 'n')
		{
			outtextxy(midx, midy, "Manually Enter Damage Fee:");
			//cout<<"Manually Enter Damage Fee: Rs.";
			//gotoxy(40,18);
			_cin(charges, midy, midx);
		}
		else if(feeChoice != 'y')	goto reFee;
		
		outtextxy(midx, midy+90, "Applied");
		//cout<<"\nApplied.\n";
		returnStatus = true;
		//cout<<"Status="<<returnStatus<<endl;
		//cout<<"\nReturned\n";
		getch();
	}
	
	void Display()
	{
		outtextxy(midx, 130, ID);
		settextjustify(1,1);
		
		int padding[] = {195, 225, 258, 290, 322, 352, 380};
		int padVar=0;
		
		char _b[20], _m[20];
		
		strupr(p_bID);
		strupr(mID);
		
		sprintf(_b, "Book: %s", p_bID);
		outtextxy(midx, padding[padVar], _b);	padVar++;
		
		sprintf(_m, "Borrower: %s", mID);
		outtextxy(midx, padding[padVar], _m);	padVar++;
		
		//outtextxy(midx-65, padding[padVar], "Date Issued: ");
		issued.display("Issued", midx, padding[padVar]);	padVar++;
		
		if(!returnStatus)
		{
			settextjustify(1,1);
			padVar++;
			outtextxy(midx, padding[padVar], "Status: Not Returned.");
		}
		else returned.display("Returned", midx, padding[padVar]);
		
		settextjustify(1,1);
	}
	
	void univExport(char fname[])
	{
		coreUtilities::type = ID[4];
		bool requiresHead = coreUtilities::univExportUtil(fname);
		
		const char *const_ePath = coreUtilities::ePath;
		
		char Tcondition[15];
		
		switch(condition)
		{
			case '1': strcpy(Tcondition,"Excelent");
					break;
					
			case '2': strcpy(Tcondition,"Good");
					break;
					
			case '3': strcpy(Tcondition,"Fair");
					break;
					
			case '4': strcpy(Tcondition,"Poor");
					break;
					
			case '5': strcpy(Tcondition,"Bad");
					break;
		}
		
		ofstream univExp_Dest(const_ePath, ios::app | ios::binary);
		
		if(requiresHead)	univExp_Dest<<"\"ID\",\"Book ID\",\"Borrower ID\",\"Date Issued\",\"Date Returned\",\"Return Condition\",\"Damage Fee\"";
		if(!returnStatus) univExp_Dest << "\n\"" << ID << "\",\"" << p_bID << "\",\"" << mID << "\",\"" << issued.day << "\/" << issued.month << "\/" << issued.year << "\",\"\",\"N\/A\",\"\"";
		if(returnStatus) univExp_Dest << "\n\"" << ID << "\",\"" << p_bID << "\",\"" << mID << "\",\"" << issued.day << "\/" << issued.month << "\/" << issued.year << "\",\"" << returned.day << "\/" << returned.month << "\/" << returned.year << "\",\"" << Tcondition << "\",\"" << charges << "\"";
		
		univExp_Dest.close();
	}
}t, trans;

void _i(char ID[])
{
	if(ID[7]=='9')
	{
		ID[6]++;
		ID[7]='0';
	}
	else ID[7]++;
}

bool* multiSelect(int n)
{
	char *t_select = new char[2*n];
	int *t2_select = new int[n];
	bool *f_select = new bool[n];
	
	for(int i=0; i<n; i++)	f_select[i] = false;
	
	_cin(t_select, 2*n, false, (midy*3/2)-4, (midx/3)+75);
	
	int k=0;
	for(i=0; t_select[i]!='\0'; i++)
	{
		if(isdigit(t_select[i]) && (t_select[i]-49 >= 0 && t_select[i]-49 < n))
		{
			t2_select[k] = t_select[i]-49;
			k++;
		}
	}
	
	for(int j=0; j<k; j++)	f_select[t2_select[j]] = true;
	
	return f_select;
}

void list(char text[])
{
	int items=0, j=0;
	char label[10][30];
	
	for(int i=0; i<strlen(text); i++)
	{
		if(text[i] == '/')
		{
			label[items][j]='\0';
			items++;
			j=0;
		}
		else
		{
			label[items][j] = text[i];
			j++;
		}
	}
	
	int posy=115, posx=midx/3;
	for(i=0; i<items; i++)	outtextxy(posx, posy+=25, label[i]);
}

void showFiles()
{
	int padding[2] = {220,-100};
	//int padding[2] = {0,-100};
	
//	colorScheme dirCol;
//	settextstyle(0,0,0);
	
//	rectangle(padding[0]+(midx/3)-15, padding[1]+250, padding[0]+midx+80, padding[1]+325);
	
//	setfillstyle(1, dirCol.bCol());
//	bar(padding[0]+midx/2+5, padding[1]+240, padding[0]+midx+12, padding[1]+260);
	
//	outtextxy(padding[0]+midx/2 +15, padding[1]+250, "Current Directories");

	char varLib_Dir[50];
	
	if((strlen(bLib_Dir) >= strlen(mLib_Dir)) && (strlen(bLib_Dir) >= strlen(tLib_Dir)))	strcpy(varLib_Dir, bLib_Dir);
	else if((strlen(mLib_Dir) >= strlen(bLib_Dir)) && (strlen(mLib_Dir) >= strlen(tLib_Dir)))	strcpy(varLib_Dir, mLib_Dir);
	else strcpy(varLib_Dir, tLib_Dir);

	dialog(padding[0]+(midx/3), padding[1]+250, "Current Directories", varLib_Dir, 3, padding[0]+100);
	
	settextjustify(0,1);
	outtextxy((padding[0])+midx/3, padding[1]+270, bLib_Dir);
	outtextxy((padding[0])+midx/3, padding[1]+290, mLib_Dir);
	outtextxy((padding[0])+midx/3, padding[1]+310, tLib_Dir);
	
	settextstyle(3,0,1);
}

bool getLoadStat()
{
	bool loadStat;
	
	if(strcmpi(bLib_Dir,"../bin/lib_mgmt/books/") != 0)
	{
		if(strcmpi(mLib_Dir,"../bin/lib_mgmt/borrow/") != 0)
		{
			if(strcmpi(tLib_Dir,"../bin/lib_mgmt/trans/") != 0)	loadStat = true;
			else loadStat = false;
		}
		else loadStat = false;
	}
	else loadStat = false;
	
	return loadStat;
}

void loadIndex()
{
	ifstream indexes("../bin/lib_mgmt/index.bin", ios::in);
	
	if(indexes.fail() || indexes.eof())
	{
		closegraph();
		clrscr();
		perror("Database Error");
		cout<<"\nPress any key to exit..";
		getch();
		abort();
	}
	
	indexes>>bID>>mID>>tID;
	indexes.close();
}

void writeIndex()
{
	ofstream indexes("../bin/lib_mgmt/index.bin", ios::nocreate);
	
	if(indexes.fail() || indexes.eof())
	{
		closegraph();
		clrscr();
		perror("Database Error");
		cout<<"\nPress any key to exit..";
		getch();
		abort();
	}
										
	indexes<<bID<<endl<<mID<<endl<<tID;
	indexes.close();
}

void ListAllBooks()
{
	header("List All", false, true);
		
	const char *const_bLib_Dir_L = bLib_Dir;
	ifstream listBook(const_bLib_Dir_L, ios::in | ios::binary);
	
	globalErrorHandler.init();
	
	settextstyle(0,0,0);
	settextjustify(0,1);
	
	int elements = 0;
	
	while(!listBook.eof())
	{
		listBook.read((char*) &book, sizeof(book));
		if(listBook.eof())	break;
		globalErrorHandler.tick();
		
		char prID[15];
		sprintf(prID, "%d. %s", elements+1, book.ShareID());
		outtextxy(180, 160 + (elements*15), prID);
		outtextxy(midx, 160 + (elements*15), book.ShareTitle());
		elements++;
	}
	
	dialog(180, 140, "Barcode", "_____________", elements-1, 250);
	dialog(midx, 140, "Book Title", "_____________________", elements-1, 450);
	
	listBook.close();
}

void ListAllMem()
{
	header("List All", false, true);
		
	const char *const_mLib_Dir_L = mLib_Dir;
	ifstream listMem(const_mLib_Dir_L, ios::in | ios::binary);
	
	globalErrorHandler.init();
	
	settextstyle(0,0,0);
	settextjustify(0,1);
	
	int elements = 0;
	
	while(!listMem.eof())
	{
		listMem.read((char*) &m, sizeof(m));
		if(listMem.eof())	break;
		globalErrorHandler.tick();
		
		char prID[15];
		sprintf(prID, "%d. %s", elements+1, m.ShareID());
		outtextxy(180, 160 + (elements*15), prID);
		outtextxy(midx, 160 + (elements*15), m.ShareName());
		elements++;
	}
	
	dialog(180, 140, "Borrower ID", "_____________", elements, 250);
	dialog(midx, 140, "Name", "___________________", elements, 550);
	
	listMem.close();
}

void ListAllTrans()
{
	header("List All", false, true);
		
	const char *const_tLib_Dir_L = tLib_Dir;
	ifstream listTrans(const_tLib_Dir_L, ios::in | ios::binary);
	
	globalErrorHandler.init();
	
	settextstyle(0,0,0);
	settextjustify(0,1);
	
	int elements = 0;
	
	while(!listTrans.eof())
	{
		listTrans.read((char*) &trans, sizeof(trans));
		if(listTrans.eof())	break;
		globalErrorHandler.tick();
		
		char prID[15];
		sprintf(prID, "%d. %s", elements+1, trans.ShareID());
		outtextxy(80, 160 + (elements*15), prID);
		outtextxy(midx-95, 160 + (elements*15), trans.Share_bName());
		outtextxy(midx+80, 160 + (elements*15), trans.Share_mName());
		elements++;
	}
	
	dialog(80, 140, "Trans. ID", "_____________", elements, -50);
	dialog(midx-95, 140, "Book Title", "_________________", elements, 350);
	dialog(midx+80, 140, "Borrower Name", "_________________", elements, 1000);
	
	listTrans.close();
}

void main()
{
	char menu, i;
	highvideo();
	
	loadIndex();
	today.sysdate();

	int gdriver = DETECT, gmode, gerror;

	initgraph(&gdriver, &gmode, "../BGI/");

	gerror = graphresult();

	if (gerror != grOk)
	{
	   cout<<"Graphics error: "<<grapherrormsg(gerror)<<endl<<"Press any key to exit";
	   getch();
	   abort();
	}
	
	midx = getmaxx()/2;
	midy = getmaxy()/2;
	
	colorScheme col;
	
l1: header("Menu Bar (TAB)\n\n\n\n\n\n\nExit (ESC)");
	settextjustify(0,1);
//	outtextxy(midx/3, 130, "Borrow Book (B)");
//	outtextxy(midx/3, 150, "Return Book (R)");

	list("(B)orrow Book/(R)eturn Book///(L)ist All Records/");
	
	/*
	char test[20];
	_cin(test, sizeof(test), midx-50, midy, true);
	cout<<test;
	*/
	
	menu = getch();
	menu = tolower(menu);
	
	if((menu == 'b' || menu == 'r' || menu == 'l') && !getLoadStat())
	{
		//errno = ENOLOAD;
		header("Main Menu (TAB)\n\n\n\n\n\n\nExit (ESC)");
		globalErrorHandler.perror(ENOLOAD);
		//getch();
		goto l1;
	}
																
	switch(menu)
	{
l2:		case 9: header("(E)xtras\t(F)ile\t\t(S)earch\t(D)ocument", false, true);
				menu = getch();
				menu = tolower(menu);
				
				if((menu == 's' || menu == 'd') && !getLoadStat())
				{
					//errno = ENOLOAD;
					header("(E)xtras\t(F)ile\t\t(S)earch\t(D)ocument", false, true);
					globalErrorHandler.perror(ENOLOAD);
					//getch();
					goto l2;
				}
				
				switch(menu)
				{
					case 'e': header("Colors (C)\n\nAbout (A)\n\nHelp (Q)", false, true);
							menu = getch();
							menu = tolower(menu);
							
							switch(menu)
							{
								case 'c':	col.picker();
											break;
											
								case 'a':	header("About This Program", false, true);
											ifstream about("../bin/lib_mgmt/about.txt", ios::in);
											
											char aChar, aStr[200];
											int pos=0;
											
											settextjustify(1,1);
											//settextstyle(3,0,1);
											
											globalErrorHandler.init();

											while(!about.eof())
											{
												int aStrI = 0;
												
												errorHandler localErrorHandler;
												localErrorHandler.init();
												
												about.get(aChar);
												if(about.eof()) break;
												
												while(aChar!='\n')
												{
													localErrorHandler.tick();
													aStr[aStrI] = aChar;
													aStrI++;
													about.get(aChar);
												}
												
												aStr[aStrI] = '\0';
												
												globalErrorHandler.tick();
												//cout<<aChar;
												//if(pos>1)	settextstyle(0,0,0);
												outtextxy(midx, 140+(pos*30), aStr);
												//getch();
												pos++;
											}
											
											about.close();
											getch();
											break;
											
								case 'q':	header("Help Documentation", false, true);
											ifstream help("../bin/lib_mgmt/help.txt", ios::in);
											
											char hChar, hStr[200];
											int pos2=0;

											settextjustify(1,1);
											settextstyle(0,0,0);

											globalErrorHandler.init();

											while(!help.eof())
											{
												int hStrI = 0;

												errorHandler localErrorHandler;
												localErrorHandler.init();

												help.get(hChar);
												if(help.eof()) break;

												while(hChar!='\n')
												{
													localErrorHandler.tick();
													hStr[hStrI] = hChar;
													hStrI++;
													help.get(hChar);
												}

												hStr[hStrI] = '\0';

												globalErrorHandler.tick();
												//cout<<aChar;
												//if(pos>1)	settextstyle(0,0,0);
												outtextxy(midx, 160+(pos2*10), hStr);
												//getch();
												pos2++;
											}
											
											help.close();
											settextstyle(3,0,1);
											getch();
											break;
							}
							break;

					case 'f': header("File", true, true);
							list("(N)ew Library/(I)mport Library/(R)ename Library/(D)elete Library/(E)xport Database/");
							showFiles();
							
							menu = getch();
							menu = tolower(menu);
							char menu2;

							if(menu == 'n' || menu == 'i' || menu == 'r' || menu == 'd' || menu == 'e')
							{
								header("File", true, true);
								list("(B) Book Library/(M) Borrower Library/(T) Transaction Library/");
								menu2 = getch();
								menu2 = tolower(menu2);
							}

							char uLib[8], uLib_Name[8];
							int attempts;

							switch(menu)
							{
								case 'n': attempts = 0;
										l5: header("New Library");
										ofstream new_Lib;
										outtextxy(midx, midy, "Enter library name:         ");
//										gotoxy(40,20);
//										cin>>uLib_Name;

										_cin(uLib_Name, sizeof(uLib_Name), false, midy, midx+80);
										char uLib_Dir[50];

										switch(menu2)
										{
											case 'b': strcpy(uLib_Dir, bLib_Dir);
													break;
													
											case 'm': strcpy(uLib_Dir, mLib_Dir);
													break;
													
											case 't': strcpy(uLib_Dir, tLib_Dir);
													break;
										}
										
										makeDir(uLib_Dir, uLib_Name);
										const char *const_uLib_Dir = uLib_Dir;
										new_Lib.open( const_uLib_Dir, ios::noreplace | ios::binary );
										
										if(new_Lib.fail())
										{
											makeDir(uLib_Dir);
											globalErrorHandler.perror();
											//getch();
											
											attempts++;
											
											if(attempts<5)
												goto l5;
												
											else
											{
												//errno = ETIMOUT;
												header("New Library");
												globalErrorHandler.perror(ETIMOUT);
												//getch();
												goto l2;
											}
										}
										else outtextxy(midx, midy+50, "Created.");
										getch();
										
										new_Lib.close();
										break;
										
								case 'i':	header("Import Library");
										outtextxy(midx, midy, "Enter library name:         ");
										attempts=0;
										l4:; //gotoxy(40,20); cin>>uLib_Name;
										_cin(uLib_Name, sizeof(uLib_Name), false, midy, midx+80);
										
										ifstream checkFile;

										switch(menu2)
										{
											case 'b': makeDir(bLib_Dir, uLib_Name);
													const char *const_bLib_Dir = bLib_Dir;
													checkFile.open(const_bLib_Dir, ios::in | ios::binary);
													break;
													
											case 'm': makeDir(mLib_Dir, uLib_Name);
													const char *const_mLib_Dir = mLib_Dir;
													checkFile.open(const_mLib_Dir, ios::in | ios::binary);
													break;
													
											case 't': makeDir(tLib_Dir, uLib_Name);
													const char *const_tLib_Dir = tLib_Dir;
													checkFile.open(const_tLib_Dir, ios::in | ios::binary);
													break;
										}
										if(checkFile.fail())
										{
											switch(menu2)
											{
												case 'b': makeDir(bLib_Dir);
														break;
														
												case 'm': makeDir(mLib_Dir);
														break;
														
												case 't': makeDir(tLib_Dir);
														break;
											}
											globalErrorHandler.perror();
											//getch();
											header("Import Library");
											
											attempts++;
											
											if(attempts<5)
											{
												outtextxy(midx, midy, "Enter valid file name:         ");
												goto l4;
											}
											else
											{
												//errno = ETIMOUT;
												globalErrorHandler.perror(ETIMOUT);
												//getch();
												goto l2;
											}
										}
										else outtextxy(midx, midy+50, "Opened.");
										getch();
										checkFile.close();
										break;
										
								case 'r': header("Rename Library");
										outtextxy(midx, midy, "Enter library name:         ");
//										gotoxy(40,20);
//										cin>>uLib_Name;
										_cin(uLib_Name, sizeof(uLib_Name), false, midy, midx+80);
										
										char uLib_Name2[8];
										outtextxy(midx, midy+30, "Enter new name:         ");
//										gotoxy(40,20);
//										cin>>uLib_Name2;
										_cin(uLib_Name2, sizeof(uLib_Name2), false, midy, midx+80);
										
										switch(menu2)
										{
											case 'b': makeDir(bLib_Dir, uLib_Name);
													
													char bLib_Dir2[50]="../bin/lib_mgmt/books/";
													makeDir(bLib_Dir2, uLib_Name2);
													
													const char *const_bLib_Dir = bLib_Dir, *const_bLib_Dir2 = bLib_Dir2;
													if(!rename(const_bLib_Dir, const_bLib_Dir2))
													{
														outtextxy(midx, midy+90, "Renamed.");
														getch();
													}
													else globalErrorHandler.perror();

													makeDir(bLib_Dir);
													break;

											case 'm': makeDir(mLib_Dir, uLib_Name);

													char mLib_Dir2[50]="../bin/lib_mgmt/borrow/";
													makeDir(mLib_Dir2, uLib_Name2);

													const char *const_mLib_Dir = mLib_Dir, *const_mLib_Dir2 = mLib_Dir2;
													if(!rename(const_mLib_Dir, const_mLib_Dir2))
													{
														outtextxy(midx, midy+90, "Renamed.");
														getch();
													}
													else globalErrorHandler.perror();

													makeDir(mLib_Dir);
													break;

											case 't': makeDir(tLib_Dir, uLib_Name);

													char tLib_Dir2[50]="../bin/lib_mgmt/trans/";
													makeDir(tLib_Dir2, uLib_Name2);

													const char *const_tLib_Dir = tLib_Dir, *const_tLib_Dir2 = tLib_Dir2;
													if(!rename(const_tLib_Dir, const_tLib_Dir2))
													{
														outtextxy(midx, midy+90, "Renamed.");
														getch();
													}
													else globalErrorHandler.perror();
													
													makeDir(tLib_Dir);
													break;
										}
										//getch();
										break;
										
								case 'd': header("Delete Library");
										outtextxy(midx, midy, "Enter library name:         ");
//										gotoxy(40,20);
//										cin>>uLib_Name;
										_cin(uLib_Name, sizeof(uLib_Name), false, midy, midx+80);
										
										char bLib_Dir2[50]="../bin/lib_mgmt/books/", mLib_Dir2[50]="../bin/lib_mgmt/borrow/", tLib_Dir2[50]="../bin/lib_mgmt/trans/";
												
										switch(menu2)
										{
											case 'b': makeDir(bLib_Dir2, uLib_Name);
													break;
													
											case 'm': makeDir(mLib_Dir2, uLib_Name);
													break;
													
											case 't': makeDir(tLib_Dir2, uLib_Name);
													break;
										}
										
										outtextxy(midx, midy+50, "Confirm delete? (Y/N)");
										char confirm;	confirm = getch();	tolower(confirm);
										
										if (confirm == 'n')	break;
										
										outtextxy(midx, midy+80, "Reconfirm delete (Y/N)");
								reDel:		confirm = getch();	tolower(confirm);
										
										if (confirm == 'y')
										{
											switch(menu2)
											{
												case 'b': const char *const_bLib_Dir2 = bLib_Dir2;
														if(!remove(const_bLib_Dir2))
														{
															outtextxy(midx, midy+90, "Deleted.");
															getch();
														}
														else globalErrorHandler.perror();
														
														makeDir(bLib_Dir);
														break;
														
												case 'm': const char *const_mLib_Dir2 = mLib_Dir2;
														if(!remove(const_mLib_Dir2))
														{
															outtextxy(midx, midy+90, "Deleted.");
															getch();
														}
														else globalErrorHandler.perror();
														
														makeDir(mLib_Dir);
														break;
														
												case 't': const char *const_tLib_Dir2 = tLib_Dir2;
														if(!remove(const_tLib_Dir2))
														{
															outtextxy(midx, midy+90, "Deleted.");
															getch();
														}
														else globalErrorHandler.perror();
														
														makeDir(tLib_Dir);
														break;
											}
										}
										if (confirm != 'n')	goto reDel;
										//getch();
										break;
										
								case 'e': header("Export Database", false, true);
										
										ifstream expRead;
										
										char delDB_Dir[50];
										
										switch(menu2)
										{
											case 'b': const char *const_bLib_Dir = bLib_Dir;
													expRead.open(const_bLib_Dir, ios::in | ios::binary);
													strcpy(delDB_Dir,bLib_Dir);
													
													makeDir(delDB_Dir, breakDir(bLib_Dir), true);
													break;
													
											case 'm': const char *const_mLib_Dir = mLib_Dir;
													expRead.open(const_mLib_Dir, ios::in | ios::binary);
													strcpy(delDB_Dir,mLib_Dir);
													
													makeDir(delDB_Dir, breakDir(mLib_Dir), true);
													break;
													
											case 't': const char *const_tLib_Dir = tLib_Dir;
													expRead.open(const_tLib_Dir, ios::in | ios::binary);
													strcpy(delDB_Dir,tLib_Dir);
													
													makeDir(delDB_Dir, breakDir(tLib_Dir), true);
													break;
										}
										
										
										
										
										remove(delDB_Dir);
										
										globalErrorHandler.init();
										while(!expRead.eof())
										{
											switch(menu2)
											{
												case 'b':	expRead.read((char*) &b, sizeof(b));
															break;
															
												case 'm':	expRead.read((char*) &m, sizeof(m));
															break;
															
												case 't':	expRead.read((char*) &t, sizeof(t));
															break;
											}
											
											if(expRead.eof())	break;
											
											globalErrorHandler.tick();
											
											//coreUtilities _book(book);
											
											switch(menu2)
											{
												case 'b':	b.univExport(breakDir(bLib_Dir));
															break;
															
												case 'm':	m.univExport(breakDir(mLib_Dir));
															break;
															
												case 't':	t.univExport(breakDir(tLib_Dir));
															break;
											}
											
										}
										outtextxy(midx, midy+90, "Exported.");
										getch();
							}
							break;

					case 's': header("Search", true, true);
							list("(B) Book/(M) Borrower/(T) Transaction/");
							menu = getch();
							menu = tolower(menu);
							
							bool found = false;

							switch(menu)
							{
								case 'b': header("Book");
										outtextxy(midx, midy, "Enter barcode number:         ");
										char bCheck[9];
										//gotoxy(40,20);
										_cin(bCheck, sizeof(bCheck), false, midy, midx+80);
										
										const char *const_bLib_Dir = bLib_Dir;
										ifstream readBook(const_bLib_Dir, ios::in | ios::binary);
										
										globalErrorHandler.init();
										
										while(!readBook.eof())
										{
											readBook.read((char*) &book, sizeof(book));
											if(readBook.eof())	break;
											globalErrorHandler.tick();
											
											if(strcmpi(bCheck, book.ShareID()) == 0)
											{
												header("Book", false, true);
												book.Display();
												found = true;
												break;
											}
											
										}
										readBook.close();
										
										break;

								case 'm':	header("Borrower");
										outtextxy(midx, midy, "Enter Borrower ID:         ");
										char mCheck[9];
										//gotoxy(40,20);
										//cin>>mCheck;
										_cin(mCheck, sizeof(mCheck), false, midy, midx+80);

										const char *const_mLib_Dir = mLib_Dir;
										ifstream readMem(const_mLib_Dir, ios::in | ios::binary);
										
										globalErrorHandler.init();
										
										while(!readMem.eof())
										{
											readMem.read((char*) &Borrower, sizeof(Borrower));
											if(readMem.eof())	break;
											
											globalErrorHandler.tick();
											
											if(strcmpi(mCheck, Borrower.ShareID()) == 0)
											{
												header("Borrower", false, true);
												Borrower.Display();
												
												found = true;
												/*cout<<"\nView Borrow Log (B)\nView Return Log (R)\nBack (ESC)\n\nChoice: ";
												char menu3 = getch();
												menu3 = tolower(menu3);
												if(menu3 == 'b' || menu3 == 'r') Borrower.ViewLog(menu3); */
												//getch();
												break;
											}
											
										}
										readMem.close();
										break;

								case 't':	header("Transaction");
										outtextxy(midx, midy, "Enter transaction number: ");
										char tCheck[9];
										//gotoxy(40,20);
										//cin>>tCheck;
										_cin(tCheck, sizeof(tCheck), false, midy, midx+120);

										const char *const_tLib_Dir = tLib_Dir;
										ifstream findTrans(const_tLib_Dir, ios::in | ios::binary);
										
										globalErrorHandler.init();

										while(!findTrans.eof())
										{
											findTrans.read((char*) &trans, sizeof(trans));
											if(findTrans.eof())	break;
											
											globalErrorHandler.tick();
											
											if(strcmpi(tCheck, trans.ShareID()) == 0)
											{
												header("Transaction", false, true);
												trans.Display();
												found = true;
												break;
											}
											
										}
										findTrans.close();
										break;
							}
							if(menu == 'b' || menu == 'm' || menu == 't')
							{
								if(!found)	globalErrorHandler.perror(ENOREC);
								else	getch();
							}
							
							break;

					case 'd': header("Document", true, true);
							list("(B) New Book/(U) Update Book/(M) New Borrower/(R) Update Borrower/");	
							menu = getch();
							menu = tolower(menu);
							
							found = false;
							
							long pos;

							switch(menu)
							{
								case 'b':	header("New Book");
										const char *const_bLib_Dir = bLib_Dir;
										/*ifstream saveBook(const_bLib_Dir, ios::in | ios::binary);
										
										while(!saveBook.eof())
										{
											saveBook.read((char*)&book, sizeof(book));
											if(saveBook.eof())	break;
											_i(bID);
										}
										saveBook.close();*/
										
										ofstream writeBook(const_bLib_Dir, ios::app | ios::binary);
										b.getData();
										writeBook.write((char*)&b, sizeof(b));
										writeBook.close();
										
										_i(bID);
										writeIndex();
										
										break;
										
								case 'u': header("Update Book");
										outtextxy(midx, midy, "Enter barcode number:         ");
										char bCheck[9];
										//gotoxy(40,20);
										//cin>>bCheck;
										_cin(bCheck, sizeof(bCheck), false, midy, midx+80);
										//bCheck[8] = '\0';
										
										const char *const_bLib_Dir2 = bLib_Dir;
										fstream modBook(const_bLib_Dir2, ios::in | ios::out | ios::binary);
										
										globalErrorHandler.init();
										
										while(!modBook.eof())
										{
											pos = modBook.tellg();
											modBook.read((char*) &b, sizeof(b));
											if(modBook.eof())	break;
											
											globalErrorHandler.tick();
											
											if(strcmpi(bCheck, b.ShareID()) == 0)
											{
												header("Update Book");
												b.Display();
//												cout<<"Choose element(s) to edit\n";
//												cout<<"1. Title\n2. Author\n3. Publisher\n4. Total Stock\n5. Price\n\nChoice: ";
												outtextxy(midx, (midy*2)-90, "Press any key to continue");
												getch();
												
												if(b.isIssued())
												{
													header("Update Book");
													globalErrorHandler.perror(EISSUED);
													goto l2;
												}
												
												header("Update Book", true);
												list("Choose element(s) to edit:/(comma-separated choices)/(1) Title/(2) Author/(3) Publisher/(4) Total Stock/(5) Price/");
												b.getData(multiSelect(5));
												modBook.seekp(pos);
												modBook.write((char*) &b, sizeof(b));
												found = true;
												break;
											}
											
										}
										modBook.close();
										break;

								case 'm':	header("New Borrower");
										const char *const_mLib_Dir = mLib_Dir;
										/*fstream saveMem(const_mLib_Dir, ios::in | ios::app | ios::binary);
										
										while(!saveMem.eof())
										{
											saveMem.read((char*)&Borrower, sizeof(Borrower));
											if(saveMem.eof())	break;
											_i(mID);
										}
										saveMem.close();*/
										
										ofstream writeMem(const_mLib_Dir, ios::app | ios::binary);
										m.getData();
										writeMem.write((char*)&m, sizeof(m));
										writeMem.close();
										
										_i(mID);
										writeIndex();
										
										break;
										
								case 'r': header("Update Borrower");
										outtextxy(midx, midy, "Enter Borrower ID:         ");
										char mCheck[9];
										//gotoxy(40,20);
										//cin>>mCheck;
										_cin(mCheck, sizeof(mCheck), false, midy, midx+80);

										const char *const_mLib_Dir2 = mLib_Dir;
										fstream modMem(const_mLib_Dir2, ios::in | ios::out | ios::binary);
										
										globalErrorHandler.init();

										while(!modMem.eof())
										{
											pos = modMem.tellg();
											modMem.read((char*) &m, sizeof(m));
											if(modMem.eof())	break;
											
											globalErrorHandler.tick();
											
											if(strcmpi(mCheck, m.ShareID()) == 0)
											{
												header("Update Borrower");
												m.Display();
//												cout<<"Choose element(s) to edit\n";
//												cout<<"1. Name\n2. Date\n\nChoice: ";
												
												outtextxy(midx, (midy*2)-90, "Press any key to continue");
												getch();
												header("Update Borrower", true);
												list("Choose element(s) to edit:/(comma-separated choices)/(1) Name/(2) Date/");
												m.getData(multiSelect(2));
												modMem.seekg(pos);
												modMem.write((char*) &m, sizeof(m));
												found = true;
												break;
											}
											
										}
										modMem.close();
										break;
										
								default: goto l2;
						}
						
						if(menu == 'u' || menu == 'r')
						{
							if(!found)	globalErrorHandler.perror(ENOREC);
							else	getch();
						}
						
						break;
						
					case 27: goto l1;
				}
				goto l2;
				
		case 'b': header("Borrow Book");
				
				const char *const_tLib_Dir = tLib_Dir;
				
				bool found_b = false, found_m = false;
				
				t.getData();
reTrEdit:				header("Borrow Book");
				t.Display();
				outtextxy(midx, (midy*2)-90, "Do you want to make any changes to data? (Y\/N): ");
//				cout<<"Do you want to make any changes to data? (Y\\N): ";
				char tEdit;
				tEdit = getch();
				
				if(tEdit == 'Y' || tEdit == 'y')
				{
					header("Borrow Book", true);
					list("Choose element(s) to edit:/(comma-separated choices)/(1) Book ID/(2) Borrower ID/(3) Date Issued/");
					
//					cout<<"Choose element(s) to edit\n";
//					cout<<"1. Book ID\n2. Borrower ID\n3. Date Issued\n\nChoice: ";
					
					t.getData(multiSelect(3));
				}
				else if(tEdit != 'N' && tEdit != 'n') goto reTrEdit;
				
				bool isAvail = true;
				
				/* shorten code */
				
				
			
				/* shorten code */

				const char *const_bLib_Dir = bLib_Dir;
				fstream findBook(const_bLib_Dir, ios::in | ios::out | ios::binary);
				long pos;
				
				globalErrorHandler.init();

				while(!findBook.eof())
				{
					pos = findBook.tellg();
					findBook.read((char*) &b, sizeof(b));
					if(findBook.eof())	break;
					
					globalErrorHandler.tick();

					if(strcmpi(t.ShareID('b'), b.ShareID()) == 0)
					{
						if(!b.isAvailable())
						{
							header("Borrow Book");
							isAvail = false;
							globalErrorHandler.perror(ENOAVAL);
							break;
						}
						
						found_b = true;
						
						const char *const_mLib_Dir = mLib_Dir;
						fstream findMem(const_mLib_Dir, ios::in | ios::out | ios::binary);

						long pos2;
						
						errorHandler localErrorHandle;
						localErrorHandle.init();
						
						while(!findMem.eof())
						{
							pos2 = findMem.tellg();
							findMem.read((char*) &m, sizeof(m));
							if(findMem.eof())	break;
							
							localErrorHandle.tick();

							if(strcmpi(t.ShareID('m'), m.ShareID()) == 0)
							{
								found_m = true;
								
								m.transact('b', t.ShareID('b'));
								findMem.seekg(pos2);
								findMem.write((char*) &m, sizeof(m));
								
								break;
							}
						}

						findMem.close();
						
						if(found_m)
						{
							b.transact('b');
							findBook.seekg(pos);
							findBook.write((char*) &b, sizeof(b));
						}
						
						break;
					}
				}
				
				findBook.close();
				
				if(!found_m || !found_b)
				{
					header("Transaction Failed", false, true);
					
					if(isAvail)
					{
						if(!found_b)	outtextxy(midx, midy-20, "Book ID Error");
						if(!found_m)	outtextxy(midx, midy+10, "Borrower ID Error");
						
						globalErrorHandler.perror(ENOREC);
					}
					
					goto l1;
				}
				else
				{
					ofstream writeTrans(const_tLib_Dir, ios::app | ios::binary);
					writeTrans.write((char*)&t, sizeof(t));

					writeTrans.close();
				}

				

	/*			const char *const_mLib_Dir = mLib_Dir;
				fstream findMem(const_mLib_Dir, ios::in | ios::out | ios::binary);

				while(!findMem.eof())
				{
					pos = findMem.tellg();
					findMem.read((char*) &m, sizeof(m));
					if(findMem.eof())	break;

					if(strcmpi(t.ShareID('m'), m.ShareID()) == 0)
					{
						m.transact('b', t.ShareID('b'));
						findMem.seekg(pos);
						findMem.write((char*) &m, sizeof(m));
						found_m = true;
						break;
					}
				}

				findMem.close();*/
				
				_i(tID);
				writeIndex();
				
				header("Borrow Book");
				outtextxy(midx, midy, "Transaction complete.");

				//tVar++;
				//_i(tID);
				break;

		case 'r': header("Return Book");
				outtextxy(midx, midy, "Enter Transaction ID:        ");
				char tCheck[9];
//				gotoxy(40,20);
//				cin>>tCheck;
//				tCheck[8]='\0';
				_cin(tCheck, sizeof(tCheck), false, midy, midx+80);
				const char *const_tLib_Dir2 = tLib_Dir;
				fstream modTrans(const_tLib_Dir2, ios::in | ios::out | ios::binary);
				
				bool found = false;
				bool alreadyReturned = false;
				
				globalErrorHandler.init();
						
				while(!modTrans.eof())
				{
					//cout<<"\nLoop In\n";
					pos = modTrans.tellg();
					modTrans.read((char*) &trans, sizeof(trans));
					if(modTrans.eof())	break;
					
					globalErrorHandler.tick();
					
					//cout<<"\nElement read\n";
					//cout<<"Val1="<<tCheck<<"\nVal2="<<trans.ShareID();
					if(strcmpi(tCheck, trans.ShareID()) == 0)
					{
						//cout<<"\nMatched\n";
						found = true;
						
						errorHandler localErrorHandle;
						
						header("Return Book");
						
						if(trans.isReturned())
						{
							alreadyReturned = true;
							break;
						}
						
						trans.Display();
						outtextxy(midx, midy+160, "Press any key to continue..");
						getch();
						header("Return Book");
						trans.Return();
						
						const char *const_bLib_Dir = bLib_Dir;
						fstream findBook(const_bLib_Dir, ios::in | ios::out | ios::binary);
						long pos;
						
						
						localErrorHandle.init();
						
						while(!findBook.eof())
						{
							pos = findBook.tellg();
							findBook.read((char*) &b, sizeof(b));
							if(findBook.eof())	break;
							localErrorHandle.tick();
							
							if(strcmpi(trans.ShareID('b'), b.ShareID()) == 0)
							{
								//cout<<"\nMatched\n";
								b.transact('r');
								findBook.seekg(pos);
								findBook.write((char*) &b, sizeof(b));
								break;
							}
						}
						
						findBook.close();
						
						const char *const_mLib_Dir = mLib_Dir;
						fstream findMem(const_mLib_Dir, ios::in | ios::out | ios::binary);
						
						localErrorHandle.init();
						
						while(!findMem.eof())
						{
							pos = findMem.tellg();
							findMem.read((char*) &m, sizeof(m));
							if(findMem.eof())	break;
							
							localErrorHandle.tick();
							
							//cout<<"Val1="<<trans.ShareID('m')<<"\nVal2="<<m.ShareID();
							if(strcmpi(trans.ShareID('m'), m.ShareID()) == 0)
							{
								m.transact('r', trans.ShareID('b'));
								findMem.seekg(pos);
								findMem.write((char*) &m, sizeof(m));
								break;
							}
						}
						
						findMem.close();
						
						modTrans.seekg(pos);
						modTrans.write((char*) &trans, sizeof(trans));
						break;
					}
					//cout<<"\nNo Match\n";
					
				}
				//cout<<"\nLoop Out\n";
				
				modTrans.close();
				
				header("Return Book", false, true);
				
				if(!found)	globalErrorHandler.perror(ENOREC);
				else if(alreadyReturned)	globalErrorHandler.perror(EREXIST);
				else
				{
					outtextxy(midx, midy, "Book returned.");
					getch();
				}
				
				break;
				
		case 'l':	ListAllBooks();
				
				settextstyle(3,0,1);
				settextjustify(1,1);
				outtextxy(midx, (midy*2)-90, "Press (TAB) to cycle through entry types");
				
				char cycle;
				int cycleIndex = 0;
				
		reCyc:		cycle = getch();
				
				switch(cycle)
				{
					case 9:	switch(cycleIndex)
							{
								case 0:	ListAllMem();
										cycleIndex = 1;
										break;
								
								case 1:	ListAllTrans();
										cycleIndex = 2;
										break;
								
								case 2:	ListAllBooks();
										cycleIndex = 0;
										break;
							}
							break;
					
					case 27:goto l1;
				}
				goto reCyc;
				
				break;

		case 27:	clock_t start, end;
				float elapsed;
				start = clock();
				char text[150];
				int remaining;
				settextjustify(1,1);
				while(!kbhit())
				{
					end = clock();
					elapsed = (end - start) / CLK_TCK;
					cleardevice();
					remaining=3.0 - elapsed;
					if(remaining)
					{
						sprintf(text, "Program will exit in %d seconds..", remaining);
						outtextxy(midx, midy-20, text);
						outtextxy(midx, midy+5, "Press (ESC) to exit now. Press any key to cancel");
						delay(300);
					}
					if(elapsed>=3.0)	exit(0);
				};
				if(getch() == 27)	exit(0);
				goto l1;
	}
	goto l1;
}
