#include <iostream>
#include <time.h>
#include <windows.h>
#include <iomanip>
using namespace std;

void Menu();
int ChooseGrid();
void FillInitBoard(char** Board, int sizeOfGrid);
void DrawBoard(char** Board, int sizeOfGrid);
void GenerateRandomLocation(char** Board, int sizeOfGrid, char Piece, int& x, int& y);
bool IsKingMoveValid(char** Board, int coordinates[2], int x, int y, int sizeOfGrid);
bool IsRookMoveValid(char** Board, int coordinates[2], int x, int y, char Piece);
bool isCheckMated(int sizeOfGrid);
bool EKingMove(char** Board, int& x, int& y, int sizeOfGrid, int kingPLoc[2], int  rook1Loc[2], int rook2Loc[2], char* rookList, int& sizeOfRookList);
void EKingGenerateLock(int& x, int& y, int option);
void CutArray(char* optionsToChooseFrom, int index, int& sizeOfChar);
bool IsPieceValid(char Piece, char* rookList);
bool IsKingChecking(char** Board, int x, int y, char Piece, int sizeOfGrid);

void Menu()
{
	int sizeOfGrid = 8;
	char number = '\0';
	while (number != '3')
	{
		cout << "1. Start a new game \n2. Change the grid of the table \n3. Exit" << endl;
		cout << "---------------------------------------" << endl;
		do
		{
			cin >> number;
			cin.ignore(100, '\n');
		} while ((number <= '0') || (number >= '4'));
		if (number == '1')
		{
			system("cls");
			isCheckMated(sizeOfGrid);
			system("cls");
		}
		if (number == '2')
		{
			system("cls");
			sizeOfGrid = ChooseGrid();
			system("cls");
		}
	}
}

int ChooseGrid()
{
	cout << "Options for the size of the grid table \n1. 8 x 8\n2. 12 x 12\n3. 5 x 5\n";
	cout << "---------------------------------------" << endl;
	int sizeOfGrid = 8;
	char number;
	cin >> number;
	cin.ignore(100, '\n');
	switch(number)
	{
	case '1': 
	{
		sizeOfGrid = 8;
		break;
	}
	case '2':
	{
		sizeOfGrid = 12;
		break;
	}
	case '3':
	{
		sizeOfGrid = 5;
		break;
	}
	default:
	{
		cout << "Invalid input. The grid will be default.\n";
		system("pause");
		break;
	}
	}
	return sizeOfGrid;
}

void FillInitBoard(char** Board, int sizeOfGrid) // fill initial board, empty of pieces
{
	cout << "1 and 2 are rooks, K is player's king and P is computer's king." << endl;
	cout << " ";
	for (int i = 1; i < sizeOfGrid + 1; i++)
	{
		cout << setw(3) << i;
	}
	cout << endl;
	for (int i = 0; i < sizeOfGrid; i++)
	{
		cout << i + 1;
		for (int j = 0; j < sizeOfGrid; j++)
		{
			Board[i][j] = '_';
			cout << setw(3) << Board[i][j];
		}
		cout << endl;
		cout << endl;
	}
}

void DrawBoard(char** Board, int sizeOfGrid) // the board drawn after the moves are made
{
	cout << "1 and 2 are rooks, K is player's king and P is computer's king." << endl;
	cout << " ";
	for (int i = 1; i < sizeOfGrid + 1; i++)
	{
		cout << setw(3) << i;
	}
	cout << endl;
	for (int i = 0; i < sizeOfGrid; i++)
	{
		cout << i+1;
		for (int j = 0; j < sizeOfGrid; j++)
		{
			cout << setw(3) << Board[i][j];
		}
		cout << endl;
		cout << endl;
	}
}

void GenerateRandomLocation(char** Board, int sizeOfGrid, char Piece, int& x, int& y)  // the coordinates of the pieces
{
	bool isFree = false;
	while (isFree == false)
	{
		srand(time(0));
		x = rand() % sizeOfGrid;
		y = rand() % sizeOfGrid;
		if (Board[x][y] == '_')
		{
			isFree = true;
			Board[x][y] = Piece;
		}
	}
}

bool IsKingMoveValid(char** Board, int coordinates[2], int x, int y, int sizeOfGrid)  // for the players king, checks if the new coordinates that 
{                                                                                     // have been entered are correct and if the king will be saved
	if ((x >= coordinates[0] - 1) && (x <= coordinates[0] + 1) && (y >= coordinates[1] - 1) && (y <= coordinates[1] + 1))
	{
		if ((Board[x][y] == '_') && (IsKingChecking(Board, x, y, 'P', sizeOfGrid) == true))
		{
			Board[coordinates[0]][coordinates[1]] = '_';
			Board[x][y] = 'K';
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool IsRookMoveValid(char** Board, int coordinates[2], int x, int y, char Piece)  // for the players rooks, checks if the new coordinates that
{                                                                                // have been entered are correct and if the rook cn go there
	if ((coordinates[0] == x) && (coordinates[1] == y))
	{
		return false;
	}
	if ((coordinates[0] == x))
	{
		if ((y - coordinates[1]) < 0)
		{
			for (int i = coordinates[1] - 1; i >= y; i--)
			{
				if (i > y)
				{
					if (Board[coordinates[0]][i] == '_')
					{
						continue;
					}
					else
					{
						return false;
					}
				}
				else if (i == y)
				{
					if (Board[coordinates[0]][i] == '_')
					{
						Board[coordinates[0]][coordinates[1]] = '_';
						Board[coordinates[0]][i] = Piece;
						return true;
					}
					else
					{
						return false;
					}
				}
			}
		}
		else if ((y - coordinates[1]) > 0)
		{
			for (int i = coordinates[1] + 1; i <= y; i++)
			{
				if (i < y)
				{
					if (Board[coordinates[0]][i] == '_')
					{
						continue;
					}
					else
					{
						return false;
					}
				}
				else if (i == y)
				{
					if (Board[coordinates[0]][i] == '_')
					{
						Board[coordinates[0]][coordinates[1]] = '_';
						Board[coordinates[0]][i] = Piece;
						return true;
					}
					else
					{
						return false;
					}
				}
			}
		}
	}
	else if ((coordinates[1] == y))
	{
		if ((x - coordinates[0]) < 0)
		{
			for (int i = coordinates[0] - 1; i >= x; i--)
			{
				if (i > x)
				{
					if (Board[i][coordinates[1]] == '_')
					{
						continue;
					}
					else
					{
						return false;
					}
				}
				else if (i == x)
				{
					if (Board[i][coordinates[1]] == '_')
					{
						Board[coordinates[0]][coordinates[1]] = '_';
						Board[i][coordinates[1]] = Piece;
						return true;
					}
					else
					{
						return false;
					}
				}

			}
		}
		else if ((x - coordinates[0]) > 0)
		{
			for (int i = coordinates[0] + 1; i <= x; i++)
			{
				if (i < x)
				{
					if (Board[i][coordinates[1]] == '_')
					{
						continue;
					}
					else
					{
						return false;
					}
				}
				else if (i == x)
				{
					if (Board[i][coordinates[1]] == '_')
					{
						Board[coordinates[0]][coordinates[1]] = '_';
						Board[i][coordinates[1]] = Piece;
						return true;
					}
					else
					{
						return false;
					}
				}
			}
		}
	}
	else
	{
		return false;
	}
}

void EKingGenerateLock(int& x, int& y, int option)  // depending on the the option's value the computer's the king mooves to the set location
{
	switch (option)
	{
	case 0:
		x--;
		y--;
		break;
	case 1:
		x--;
		break;
	case 2:
		x--;
		y++;
		break;
	case 3:
		y++;
		break;
	case 4:
		x++;
		y++;
		break;
	case 5:
		x++;
		break;
	case 6:
		x++;
		y--;
		break;
	case 7:
		y--;
		break;
	}
}

void CutArray(char* optionsToChooseFrom, int index, int& sizeOfChar)  // shortens an array so that the element that stays on the given index is no more
{                                                                    // moves all elements on the index+1 location one place forward
	for (int i = index + 1; i < sizeOfChar; i++)
	{
		optionsToChooseFrom[i - 1] = optionsToChooseFrom[i];
	}
	optionsToChooseFrom[sizeOfChar - 1] = '\0';
	sizeOfChar--;
}

bool IsKingChecking(char** Board, int x, int y, char Piece, int sizeOfGrid)  // checks if a king is in check by the other king, because it will be an invalid move for both sides
{
	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			if ((i >= 0) && (j >= 0) && (i < sizeOfGrid) && (y < sizeOfGrid))
			{
				if(Board[i][j] == Piece)
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool EKingMove(char** Board, int& x, int& y, int sizeOfGrid, int kingPLoc[2], int  rook1Loc[2], int rook2Loc[2], char* rookList, int& sizeOfRookList)
{
	int generateCase = 0, option = 0, oldX = x, oldY = y, sizeOfChar = 8;  // covers the movement of the computer's king
	bool isSafe = false;
	char optionsToChooseFrom[] = "01234567"; // contains all the possible movements of the king
	for (int i = oldX - 1; i <= oldX + 1; i++)  // prioritises taking rooks if possible
	{
		for (int j = oldY - 1; j <= oldY + 1; j++)
		{
			if ((i == rook1Loc[0]) && (j == rook1Loc[1]) && (i != rook2Loc[0]) && (j != rook2Loc[1]) && (IsKingChecking(Board, i, j, 'K', sizeOfGrid) == true))
			{
				rook1Loc[0] = -2;
				rook1Loc[1] = -2;
				Board[oldX][oldY] = '_';
				Board[i][j] = 'P';
				x = i;
				y = j;
				CutArray(rookList, 0, sizeOfRookList);
				return false;
			}
			if ((i == rook2Loc[0]) && (j == rook2Loc[1]) && (i != rook1Loc[0]) && (j != rook1Loc[1]) && (IsKingChecking(Board, i, j, 'K', sizeOfGrid) == true))
			{
				rook2Loc[0] = -2;
				rook2Loc[1] = -2;
				Board[oldX][oldY] = '_';
				Board[i][j] = 'P';
				x = i;
				y = j;
				if (sizeOfRookList == 2)
				{
					CutArray(rookList, 1, sizeOfRookList);
				}
				else
				{
					CutArray(rookList, 0, sizeOfRookList);
				}
				return false;
			}
		}
	}
	while (isSafe == false)  // otherwise searches for a new safe loacation, as they are end number moves if all are concidered unsafe it is in checkmate
	{
		if (sizeOfChar == 0)
		{
			return true;
		}
		srand(time(0));
		generateCase = rand() % sizeOfChar;
		option = optionsToChooseFrom[generateCase]-'0';
		CutArray(optionsToChooseFrom, generateCase, sizeOfChar);
		EKingGenerateLock(x, y, option);
		if (x < 0 || y < 0 || x >= sizeOfGrid || y >= sizeOfGrid)
		{
			x = oldX;
			y = oldY;
			continue;
		}
		else if ((x == rook1Loc[0]) || (x == rook2Loc[0]))
		{
			x = oldX;
			y = oldY;
			continue;
		}
		else if ((y == rook1Loc[1]) || (y == rook2Loc[1]))
		{
			x = oldX;
			y = oldY;
			continue;
		}
		else if (((x == kingPLoc[0]) && (y == kingPLoc[1])) || (IsKingChecking(Board, x, y, 'K', sizeOfGrid) == false))
		{
			x = oldX;
			y = oldY;
			continue;
		}
		else if (Board[x][y] == '_')
		{
			Board[oldX][oldY] = '_';
			Board[x][y] = 'P';
			isSafe = true;
		}
	}
	return false;
}

bool IsPieceValid(char Piece, char* rookList)  // checks if the piece entered by the player is valid
{
	int index = 0;
	if ((Piece == 'K') || (Piece == 'k'))
	{
		return true;
	}
	while (rookList[index] != '\0')
	{
		if (rookList[index] == Piece)
		{
			return true;
		}
		index++;
	}
	return false;
}

bool isCheckMated(int sizeOfGrid)
{
	bool isMated = false, isValid = false;
	char Piece = '\0';
	char rookList[3] = "12"; // list of the rooks if one of them is taken the list is shortened
	int x = 0, y = 0, counter = 0, loops = 0, sizeOfRookList = 2;
	int kingPLoc[2], kingELoc[2], rook1Loc[2], rook2Loc[2];
	char** Board = new char*[sizeOfGrid];
	for (int i = 0; i < sizeOfGrid; i++)
	{
		Board[i] = new char[sizeOfGrid];
	}
	FillInitBoard(Board, sizeOfGrid);
	do    // generate the coordinates of all pieces until they are all correctly placed with no checks
	{ 
		if (loops != 0)
		{
			system("cls");
			FillInitBoard(Board, sizeOfGrid);
		}
		GenerateRandomLocation(Board, sizeOfGrid, 'K', kingPLoc[0], kingPLoc[1]);
		GenerateRandomLocation(Board, sizeOfGrid, 'P', kingELoc[0], kingELoc[1]);
		GenerateRandomLocation(Board, sizeOfGrid, '1', rook1Loc[0], rook1Loc[1]);
		GenerateRandomLocation(Board, sizeOfGrid, '2', rook2Loc[0], rook2Loc[1]);
		loops++;
	} while ((IsKingChecking(Board, kingPLoc[0], kingPLoc[1], 'P', sizeOfGrid) == false) || ((kingELoc[0] == rook1Loc[0]) ||
		(kingELoc[0] == rook2Loc[0])) || ((kingELoc[1] == rook1Loc[1]) || (kingELoc[1] == rook2Loc[1])));
	system("cls");
	DrawBoard(Board, sizeOfGrid);
	while (isMated == false) // untill the computer's king is mated
	{
		while (isValid == false)  // untill the player enters safe coordinates of a piece
		{
			cout << "Enter the piece you want to move: ";
			cin >> Piece;
			cin.clear();
			cin.ignore(100, '\n');
			if (IsPieceValid(Piece, rookList) == false)
			{
				cout << "Wrong input!" << endl;
				continue;
			}
			cout << "Enter the new coordinates of the piece: " << endl;
			cout << "Row: ";
			cin >> x;
			cin.clear();
			cin.ignore(100, '\n');
			cout << "Column: ";
			cin >> y;
			cin.clear();
			cin.ignore(100, '\n');
			if (((x > sizeOfGrid) || (x < 1)) || (y > sizeOfGrid) || (y < 1))
			{
				cout << "Wrong input!" << endl;
				continue;
			}
			if ((Piece == 'K') || (Piece == 'k'))  // checks the new coordinates for the king
			{
				isValid = IsKingMoveValid(Board, kingPLoc, x - 1, y - 1, sizeOfGrid);
				if ((isValid == true))
				{
					kingPLoc[0] = x - 1;
					kingPLoc[1] = y - 1;
				}
				else
				{
					cout << "Wrong input!" << endl;
				}
			}
			else if (Piece == '1')   // checks the new coordinates for the rook
			{
				isValid = IsRookMoveValid(Board, rook1Loc, x - 1, y - 1, '1');
				if (isValid == true)
				{
					rook1Loc[0] = x - 1;
					rook1Loc[1] = y - 1;
				}
				else if (isValid == false)
				{
					cout << "Wrong input!" << endl;
				}
			}
			else if (Piece == '2')  // checks the new coordinates for the rook
			{
				isValid = IsRookMoveValid(Board, rook2Loc, x - 1, y - 1, '2');
				if (isValid == true)
				{
					rook2Loc[0] = x - 1;
					rook2Loc[1] = y - 1;
				}
				else if (isValid == false)
				{
					cout << "Wrong input!" << endl;
				}
			}
		}
		counter++;
		isMated = EKingMove(Board, kingELoc[0], kingELoc[1], sizeOfGrid, kingPLoc, rook1Loc, rook2Loc, rookList, sizeOfRookList);
		system("cls");
		DrawBoard(Board, sizeOfGrid);
		isValid = false;
	}
	for (int i = 0; i < sizeOfGrid; i++)
	{
		delete[] Board[i];
	}
	delete[] Board;
	cout << "Checkmate" << endl;
	cout << "You succeeded in " << counter << " moves." << endl;
	system("pause");
	return isMated;
}

int main()
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 700, 600, TRUE);
	Menu();
	return 0;
}