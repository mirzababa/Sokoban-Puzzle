/*The first AI home work, third cource
Writen by Yasamin Sadat Mirzababa
start time: May 24, 2018*/

#include<iostream>
#include<vector>
#include <fstream> //read from file
#include <algorithm>//find

using namespace std;

/*Problem Entities*/

struct agent {//Agent position
	short int i, j;
};

struct tile
{
	std::pair<short int, short int> position;
	bool box, agent, wall, free, goal;
	char shape;
};

struct board
{
	vector <vector<tile> > Grid;
};

/*Explanation: Read file and Create Environment Matrix
input: void
output:Grid*/
std::pair<board,agent> Board() {
	pair <board, agent > result;
	fstream inputfile;
	short int line, column;
	inputfile.open("E:\\Course\\AI\\AI3\\project\\HW1\\Soko1.txt");
	if (inputfile.is_open()) {
		inputfile >> line;
		inputfile >> column;
		vector <vector<tile> > input(line, vector<tile>(column)); //create dynamic line*column vector
		for (short int i = 0; i < line; i++)
		{
			for (short int j = 0; j < column; j++) 
			{
				inputfile >> input[i][j].shape; 
				input[i][j].position.first = i;
				input[i][j].position.second = j;
				//Identify the position of the agent
				if (input[i][j].shape == 'S')
				{
					result.second.i = i;
					result.second.j = j;
				}
			}
		}
		
		result.first.Grid = input;

		return result;
	}
	else
	{
		cout << "\n\tCouldn't read file!\n\tPlease enter correct path \n\n";
		exit(0);
	}

}

/*Explanation: remove all wall and only save important tail
input: Board 
output:برد جدید با تعداد کمتر کاشی*/
std::pair<vector<tile>, agent> removewalls(pair<board, agent> board) {
	pair<vector<tile>, agent> avilable;
	for (short int i = 0; i < 5; i++) 
	{
		for (short int j = 0; j < 12; j++) 
		{
			if (board.first.Grid[i][j].shape != '#')
			{
				avilable.first.push_back(board.first.Grid[i][j]);
			}
		}
	}
	avilable.second = board.second;
	return avilable;
}

/*Explanation: Legal move
input: Board and current location of agent(x,y)
output:New board with new agent's location*/
board moveup(pair<vector<tile>, agent> en)
{

	if (std::find(en.first[0].position, en.first[en.first.size()].position, en.second.i-1 & en.second.j) != en.first[en.first.size()].position)//not wall
	{
		//box
		
			
		//free
		for (short int i = 0; i < en.first.size(); i++)
		{
			if (en.first[i].position.first == en.second.i) 
			{
				if (en.first[i].position.second == en.second.j)
				{

				}
			}
		}
	}
	
}

board movedown(board b, short int i, short int j) 
{
	b.Grid[i][j].agent = false;
	b.Grid[i][j].free = true;
	b.Grid[i + 1][j].agent = true;
	b.Grid[i + 1][j].free = false;
	return b;
}

board moveright(board b, short int i, short int j)
{
	b.Grid[i][j].agent = false;
	b.Grid[i][j].free = true;
	b.Grid[i][j + 1].agent = true;
	b.Grid[i][j + 1].free = false;
	return b;
}

board moveleft(board b, short int i, short int j) 
{
	b.Grid[i][j].agent = false;
	b.Grid[i][j].free = true;
	b.Grid[i][j - 1].agent = true;
	b.Grid[i][j - 1].free = false;
	return b;
}

/*Explanation: the set of all actions*/ 
vector<short int> action = { 1,2,3,4 };

/*Explanation: reward function R(s,a,s') 
input: Board and current location of agent(x,y)
output:New board with new agent's location*/
/*short int reward() {

}*/





int main() {
	pair<board,agent> input = Board(); //کل محیط و مکان عامل
	pair<vector<tile>, agent> environment = removewalls(input);//محیط قابل استفاده و مکان عامل
	/*test
	if (input.Grid[0][0].shape == '3') { cout << "T"; }
	else { cout << "F"; }*/
	return 0;
}