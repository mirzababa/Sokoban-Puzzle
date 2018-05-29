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

/*Explanation: the set of all actions*/
vector<short int> action = { 1,2,3,4 };
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

				if (input[i][j].shape == '#') { input[i][j].wall = true; }
				if (input[i][j].shape == 'S') { input[i][j].agent = true; }
				if (input[i][j].shape == '.') { input[i][j].free = true; }
				if (input[i][j].shape == '@') { input[i][j].box = true; }
				if (input[i][j].shape == 'X') { input[i][j].goal = true; }

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
pair<vector<tile>, agent> moveup(pair<vector<tile>, agent> en)
{
	bool flag = false;	//used in reward function. if flag is true -> hit the wall
	short int n;	//index of the upper tile of the agent in vector
	short int a;	// index of the agent in vector
	short int th;	//index of the two higher tile in vector
	//If the upper tile is free
	if (std::find(en.first[0].position, en.first[en.first.size()].position, en.second.i-1 && en.second.j) != en.first[en.first.size()].position)//not wall
	{
		for (short int i = 0; i < en.first.size(); i++)
		{
				if (en.first[i].position.first == en.second.i)
				{
					if (en.first[i].position.second == en.second.j) { a = i; }	
				}
				if (i > 0)
				{
					if (en.first[i].position.first == en.second.i - 1)
					{
						if (en.first[i].position.second == en.second.j) { n = i; }

					}
				}
		}
		//Simulate motion
		if (en.first[n].free == true) 
		{
			en.first[n].free = false;
			en.first[n].agent = true;
			en.first[n].shape = 'S';

			en.first[a].free = true;
			en.first[a].agent = false;
			en.first[n].shape = '.';
			en.second.i = en.first[n].position.first;
			en.second.j = en.first[n].position.second;
		}
		//return new environmet after the move
		return en;
	}

	//If the upper tile is box
	if (std::find(en.first[0].position, en.first[en.first.size()].position, en.second.i - 1 && en.second.j) != en.first[en.first.size()].position)//not wall
	{
		if (std::find(en.first[0].position, en.first[en.first.size()].position, en.second.i - 2 && en.second.j) != en.first[en.first.size()].position)
		{
			for (short int i = 0; i < en.first.size(); i++)
			{
				if (en.first[i].position.first == en.second.i)
				{
					if (en.first[i].position.second == en.second.j) { a = i; }
				}
				if (i > 0)
				{
					if (en.first[i].position.first == en.second.i - 1)
					{
						if (en.first[i].position.second == en.second.j) { n = i; }

					}
				}
				if (i > 1)
				{
					if (en.first[i].position.first == en.second.i - 2)
					{
						if (en.first[i].position.second == en.second.j) { th = i; }

					}
				}

			}
			//Simulate motion for box and agent
			if (en.first[n].box == true)
			{
				//move the box
				en.first[n].box = false;
				en.first[n].free = true;
				en.first[n].shape = '.';
				en.first[th].box = true;
				en.first[th].free = false;
				en.first[n].shape = '@';

				en.first[a].free = true;
				en.first[a].agent = false;
				en.first[n].shape = '.';
				en.first[n].free = false;
				en.first[n].agent = true;
				en.first[n].shape = 'S';
				en.second.i = en.first[n].position.first;
				en.second.j = en.first[n].position.second;
			}
			//return new environmet after the move
			return en;
		}
	}
	flag = true;
}

pair<vector<tile>, agent> movedown(pair<vector<tile>, agent> en)
{
	short int n;	//index of the upper tile of the agent in vector
	short int a;	// index of the agent in vector
	short int th;	//index of the two higher tile in vector
					//If the upper tile is free
	if (std::find(en.first[0].position, en.first[en.first.size()].position, en.second.i - 1 && en.second.j) != en.first[en.first.size()].position)//not wall
	{
		for (short int i = 0; i < en.first.size(); i++)
		{
			if (en.first[i].position.first == en.second.i)
			{
				if (en.first[i].position.second == en.second.j) { a = i; }
			}
			if (i > 0)
			{
				if (en.first[i].position.first == en.second.i - 1)
				{
					if (en.first[i].position.second == en.second.j) { n = i; }

				}
			}
		}
		//Simulate motion
		if (en.first[n].free == true)
		{
			en.first[n].free = false;
			en.first[n].agent = true;

			en.first[a].free = true;
			en.first[a].agent = false;
			en.second.i = en.first[n].position.first;
			en.second.j = en.first[n].position.second;
		}
		//return new environmet after the move
		return en;
	}

	//If the upper tile is box
	if (std::find(en.first[0].position, en.first[en.first.size()].position, en.second.i - 1 && en.second.j) != en.first[en.first.size()].position)//not wall
	{
		if (std::find(en.first[0].position, en.first[en.first.size()].position, en.second.i - 2 && en.second.j) != en.first[en.first.size()].position)
		{
			for (short int i = 0; i < en.first.size(); i++)
			{
				if (en.first[i].position.first == en.second.i)
				{
					if (en.first[i].position.second == en.second.j) { a = i; }
				}
				if (i > 0)
				{
					if (en.first[i].position.first == en.second.i - 1)
					{
						if (en.first[i].position.second == en.second.j) { n = i; }

					}
				}
				if (i > 1)
				{
					if (en.first[i].position.first == en.second.i - 2)
					{
						if (en.first[i].position.second == en.second.j) { th = i; }

					}
				}

			}
			//Simulate motion for box and agent
			if (en.first[n].box == true)
			{
				//move the box
				en.first[n].box = false;
				en.first[n].free = true;
				en.first[th].box = true;
				en.first[th].free = false;

				en.first[a].free = true;
				en.first[a].agent = false;
				en.first[n].free = false;
				en.first[n].agent = true;
				en.second.i = en.first[n].position.first;
				en.second.j = en.first[n].position.second;
			}
			//return new environmet after the move
			return en;
		}
	}

}

pair<vector<tile>, agent> moveleft(pair<vector<tile>, agent> en)
{
	short int n;	//index of the upper tile of the agent in vector
	short int a;	// index of the agent in vector
	short int th;	//index of the two higher tile in vector
					//If the upper tile is free
	if (std::find(en.first[0].position, en.first[en.first.size()].position, en.second.i - 1 && en.second.j) != en.first[en.first.size()].position)//not wall
	{
		for (short int i = 0; i < en.first.size(); i++)
		{
			if (en.first[i].position.first == en.second.i)
			{
				if (en.first[i].position.second == en.second.j) { a = i; }
			}
			if (i > 0)
			{
				if (en.first[i].position.first == en.second.i - 1)
				{
					if (en.first[i].position.second == en.second.j) { n = i; }

				}
			}
		}
		//Simulate motion
		if (en.first[n].free == true)
		{
			en.first[n].free = false;
			en.first[n].agent = true;

			en.first[a].free = true;
			en.first[a].agent = false;
			en.second.i = en.first[n].position.first;
			en.second.j = en.first[n].position.second;
		}
		//return new environmet after the move
		return en;
	}

	//If the upper tile is box
	if (std::find(en.first[0].position, en.first[en.first.size()].position, en.second.i - 1 && en.second.j) != en.first[en.first.size()].position)//not wall
	{
		if (std::find(en.first[0].position, en.first[en.first.size()].position, en.second.i - 2 && en.second.j) != en.first[en.first.size()].position)
		{
			for (short int i = 0; i < en.first.size(); i++)
			{
				if (en.first[i].position.first == en.second.i)
				{
					if (en.first[i].position.second == en.second.j) { a = i; }
				}
				if (i > 0)
				{
					if (en.first[i].position.first == en.second.i - 1)
					{
						if (en.first[i].position.second == en.second.j) { n = i; }

					}
				}
				if (i > 1)
				{
					if (en.first[i].position.first == en.second.i - 2)
					{
						if (en.first[i].position.second == en.second.j) { th = i; }

					}
				}

			}
			//Simulate motion for box and agent
			if (en.first[n].box == true)
			{
				//move the box
				en.first[n].box = false;
				en.first[n].free = true;
				en.first[th].box = true;
				en.first[th].free = false;

				en.first[a].free = true;
				en.first[a].agent = false;
				en.first[n].free = false;
				en.first[n].agent = true;
				en.second.i = en.first[n].position.first;
				en.second.j = en.first[n].position.second;
			}
			//return new environmet after the move
			return en;
		}
	}

}

pair<vector<tile>, agent> moveright(pair<vector<tile>, agent> en)
{
	short int n;	//index of the upper tile of the agent in vector
	short int a;	// index of the agent in vector
	short int th;	//index of the two higher tile in vector
					//If the upper tile is free
	if (std::find(en.first[0].position, en.first[en.first.size()].position, en.second.i - 1 && en.second.j) != en.first[en.first.size()].position)//not wall
	{
		for (short int i = 0; i < en.first.size(); i++)
		{
			if (en.first[i].position.first == en.second.i)
			{
				if (en.first[i].position.second == en.second.j) { a = i; }
			}
			if (i > 0)
			{
				if (en.first[i].position.first == en.second.i - 1)
				{
					if (en.first[i].position.second == en.second.j) { n = i; }

				}
			}
		}
		//Simulate motion
		if (en.first[n].free == true)
		{
			en.first[n].free = false;
			en.first[n].agent = true;

			en.first[a].free = true;
			en.first[a].agent = false;
			en.second.i = en.first[n].position.first;
			en.second.j = en.first[n].position.second;
		}
		//return new environmet after the move
		return en;
	}

	//If the upper tile is box
	if (std::find(en.first[0].position, en.first[en.first.size()].position, en.second.i - 1 && en.second.j) != en.first[en.first.size()].position)//not wall
	{
		if (std::find(en.first[0].position, en.first[en.first.size()].position, en.second.i - 2 && en.second.j) != en.first[en.first.size()].position)
		{
			for (short int i = 0; i < en.first.size(); i++)
			{
				if (en.first[i].position.first == en.second.i)
				{
					if (en.first[i].position.second == en.second.j) { a = i; }
				}
				if (i > 0)
				{
					if (en.first[i].position.first == en.second.i - 1)
					{
						if (en.first[i].position.second == en.second.j) { n = i; }

					}
				}
				if (i > 1)
				{
					if (en.first[i].position.first == en.second.i - 2)
					{
						if (en.first[i].position.second == en.second.j) { th = i; }

					}
				}

			}
			//Simulate motion for box and agent
			if (en.first[n].box == true)
			{
				//move the box
				en.first[n].box = false;
				en.first[n].free = true;
				en.first[th].box = true;
				en.first[th].free = false;

				en.first[a].free = true;
				en.first[a].agent = false;
				en.first[n].free = false;
				en.first[n].agent = true;
				en.second.i = en.first[n].position.first;
				en.second.j = en.first[n].position.second;
			}
			//return new environmet after the move
			return en;
		}
	}

}



short int Reward(pair<vector<tile>, agent> en , short int whichaction, bool f)
{
	pair<vector<tile>, agent> ren;
	if (whichaction == 1) { ren = moveup(en); }
	if (whichaction == 2) { ren = moveright(en); }
	if (whichaction == 3) { ren = movedown(en); }
	if (whichaction == 4) { ren = moveleft(en); }

	//Collision with the wall
	if (ren.first == en.first) {
		return -10;
	}

	//Collision with the box
	if (f == false) {
		for (short int i = 0; i < ren.first.size(); i++)
		{
			if (ren.first[i].box == true)
			{
				if (en.first[i].free == true) {
					return 10;
				}
			}
		}
	}

	//Reach the goal along with the box
	for (short int i = 0; i < ren.first.size(); i++)
	{
		if (ren.first[i].box == true && ren.first[i].goal == true)
		{
			if (en.first[i].goal == true) {
				return 100;
			}
		}
	}
	return 0;
}





int main() {
	pair<board,agent> input = Board(); //کل محیط و مکان عامل
	pair<vector<tile>, agent> environment = removewalls(input);//محیط قابل استفاده و مکان عامل
	bool flag = false;	////used in reward function. if flag is true ->  agent once received a -10 reward and should change in value iteration
	/*test
	if (input.Grid[0][0].shape == '3') { cout << "T"; }
	else { cout << "F"; }*/
	return 0;
}