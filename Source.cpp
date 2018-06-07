/*The first AI home work, third cource
Writen by Yasamin Sadat Mirzababa
start time: May 24, 2018*/

#include<iostream>
#include<vector>
#include <fstream> //read from file
#include <string> //getline()
#include <algorithm>//find

using namespace std;

struct agent
{
	short int x, y;
};

struct box
{
	short int x, y;
};

struct state
{
	agent a;
	box b;
};

struct position {
	short int x, y;
};

struct tile
{
	bool box, agent, goal;
	position p;
};

struct value {
	state s;
	float v;
};

/*Explanation: Gaining the dimensions of the environment
input: --
output:The pair includes the length and width of the game environment */
pair<short int, short int> GetEnvironmentDimensions()
{
	pair<short int, short int> ED;
	fstream inputfile;
	short int line, column;
	inputfile.open("E:\\Course\\AI\\AI3\\project\\HW1\\Soko1.txt");
	if (inputfile.is_open())
	{
		inputfile >> line;
		inputfile >> column;
		ED.first = line;
		ED.second = column;
		inputfile.close();
	}
	else
	{
		cout << "\n\tCouldn't read file!\n\tPlease enter the correct path \n\n";
		exit(0);
	}
	return ED;
}

/*Explanation: Getting free houses to move the agent in the game environment
input: --
output:The vector includes the houses other than walls of the game environment*/
vector<tile> makegrid()
{
	vector<tile> MovementEnvironmet;
	tile temp;
	fstream inputfile;
	inputfile.open("E:\\Course\\AI\\AI3\\project\\HW1\\Soko1.txt");
	if (inputfile.is_open())
	{
		string dummyLine;
		getline(inputfile, dummyLine);//skip file's first line 

		short int line = GetEnvironmentDimensions().first;
		short int col = GetEnvironmentDimensions().second;
		
		char shape;

		for (short int i = 0; i < line; i++)
		{
			for (short int j = 0; j < col; j++) 
			{
				inputfile >> shape;

				if (shape != '#') 
				{
					if (shape == '@')
					{
						temp.box = true;
						temp.agent = false;
						temp.goal = false;
						temp.p.x = i;
						temp.p.y = j;

						MovementEnvironmet.push_back(temp);
					}
					if (shape == '.')
					{
						temp.box = false;
						temp.agent = false;
						temp.goal = false;
						temp.p.x = i;
						temp.p.y = j;

						MovementEnvironmet.push_back(temp);
					}
					if (shape == 'X')
					{
						temp.box = false;
						temp.agent = false;
						temp.goal = true;
						temp.p.x = i;
						temp.p.y = j;

						MovementEnvironmet.push_back(temp);
					}
					if (shape == 'S')
					{
						temp.box = false;
						temp.agent = true;
						temp.goal = false;
						temp.p.x = i;
						temp.p.y = j;

						MovementEnvironmet.push_back(temp);
					}
				}
			}
		}
	}
	else
	{
		cout << "\n\tCouldn't read file!\n\tPlease enter the correct path \n\n";
		exit(0);
	}
	return MovementEnvironmet;
}

vector<state>stategenerator(state s, vector<tile>grid , short int w) 
{
	state temp;
	vector<state>newstates;
	for (short int i = 0; i < grid.size(); i++) 
	{
		if (grid[i].agent == true)
		{
			//up
			if (w == 1)
			{
				if (s.a.x == grid[i].p.x - 1 && s.a.y == grid[i].p.y)
				{ /////////dummy//////////////!
					temp.a.x = s.a.x - 1;	temp.a.y = s.a.y;
					if (grid[i].p.x - 1 == s.b.x && grid[i].p.y == s.b.y && s.b.x == grid[i].p.x - 1 && s.b.y == grid[i].p.y) //hit the box
					{
						temp.b.x = s.b.x - 1;	temp.b.y = s.b.y;
					}
					temp.b.x = s.b.x;	temp.b.y = s.b.y;
					newstates.push_back(temp);
				}
			}
			//down
			if (w == 3)
			{
				if (s.a.x == grid[i].p.x + 1 && s.a.y == grid[i].p.y)
				{
					temp.a.x = s.a.x + 1;	temp.a.y = s.a.y;
					if (grid[i].p.x + 1 == s.b.x && grid[i].p.y == s.b.y && s.b.x == grid[i].p.x + 1 && s.b.y == grid[i].p.y)
					{
						temp.b.x = s.b.x + 1;	temp.b.y = s.b.y;
					}
					temp.b.x = s.b.x;	temp.b.y = s.b.y;
					newstates.push_back(temp);
				}
			}
			//left
			if (w == 4)
			{
				if (s.a.x == grid[i].p.x && s.a.y == grid[i].p.y - 1)
				{
					temp.a.x = s.a.x;	temp.a.y = s.a.y - 1;
					if (grid[i].p.x == s.b.x && grid[i].p.y - 1 == s.b.y && s.b.x == grid[i].p.x && s.b.y == grid[i].p.y - 1)
					{
						temp.b.x = s.b.x;	temp.b.y = s.b.y - 1;
					}
					temp.b.x = s.b.x;	temp.b.y = s.b.y;
					newstates.push_back(temp);
				}
			}
			//right
			if (w == 2)
			{
				if (s.a.x == grid[i].p.x && s.a.y == grid[i].p.y + 1)
				{
					temp.a.x = s.a.x;	temp.a.y = s.a.y + 1;
					if (grid[i].p.x == s.b.x && grid[i].p.y + 1 == s.b.y && s.b.x == grid[i].p.x && s.b.y == grid[i].p.y + 1)
					{
						temp.b.x = s.b.x;	temp.b.y = s.b.y + 1;
					}
					temp.b.x = s.b.x;	temp.b.y = s.b.y;
					newstates.push_back(temp);
				}
			}
		}
		break;
	}
	return newstates;
}


short int reward(state s, short int a , vector<tile>grid) 
{
	state newstate;
	
	if (a = 1) {
		newstate.a.x = s.a.x - 1;	newstate.a.y = s.a.y;
		if (newstate.a.x == s.b.x && newstate.a.y == s.b.y)//hit rhe box بالاش جعبه هست
		{
			newstate.b.x = s.b.x - 1;	newstate.b.y = s.b.y;
		}
		newstate.b.x = s.b.x;	newstate.b.y = s.b.y;
		for (short int i = 0; i < grid.size(); i++) {
			if (grid[i].goal == true 
				&& newstate.b.x == grid[i].p.x && newstate.b.y == grid[i].p.y && 
				newstate.a.x == newstate.b.x + 1 && newstate.a.y == newstate.b.y) {
				return 100;//reach to the goal
			}

			if (grid[i].p.x == newstate.a.x  && grid[i].p.y== newstate.a.y
				|| grid[i].p.x == newstate.b.x && grid[i].p.y == newstate.b.y) { return -10; }
		}
		if (newstate.a.x == s.b.x && newstate.a.y == s.b.y) { return 10; } //hit the box
	}

	if (a = 2) {
		newstate.a.x = s.a.x;	newstate.a.y = s.a.y + 1;
		if (newstate.a.x == s.b.x && newstate.a.y == s.b.y)//hit rhe box 
		{
			newstate.b.x = s.b.x;	newstate.b.y = s.b.y + 1;
		}
		newstate.b.x = s.b.x;	newstate.b.y = s.b.y;
		for (short int i = 0; i < grid.size(); i++) {
			if (grid[i].goal == true
				&& newstate.b.x == grid[i].p.x && newstate.b.y == grid[i].p.y &&
				newstate.a.x == newstate.b.x && newstate.a.y == newstate.b.y - 1) {
				return 100;//reach to the goal
			}

			if (grid[i].p.x == newstate.a.x  && grid[i].p.y == newstate.a.y
				|| grid[i].p.x == newstate.b.x && grid[i].p.y == newstate.b.y) {
				return -10;
			}
		}
		if (newstate.a.x == s.b.x && newstate.a.y == s.b.y) { return 10; } //hit the box
	}

	if (a = 3) {
		newstate.a.x = s.a.x + 1;	newstate.a.y = s.a.y;
		if (newstate.a.x == s.b.x && newstate.a.y == s.b.y)//hit rhe box بالاش جعبه هست
		{
			newstate.b.x = s.b.x + 1;	newstate.b.y = s.b.y;
		}
		newstate.b.x = s.b.x;	newstate.b.y = s.b.y;
		for (short int i = 0; i < grid.size(); i++) {
			if (grid[i].goal == true
				&& newstate.b.x == grid[i].p.x && newstate.b.y == grid[i].p.y &&
				newstate.a.x == newstate.b.x - 1 && newstate.a.y == newstate.b.y) {
				return 100;//reach to the goal
			}

			if (grid[i].p.x == newstate.a.x  && grid[i].p.y == newstate.a.y
				|| grid[i].p.x == newstate.b.x && grid[i].p.y == newstate.b.y) {
				return -10;
			}
		}
		if (newstate.a.x == s.b.x && newstate.a.y == s.b.y) { return 10; } //hit the box
	}

	if (a = 4) {
		newstate.a.x = s.a.x;	newstate.a.y = s.a.y - 1;
		if (newstate.a.x == s.b.x && newstate.a.y == s.b.y)//hit rhe box 
		{
			newstate.b.x = s.b.x;	newstate.b.y = s.b.y - 1;
		}
		newstate.b.x = s.b.x;	newstate.b.y = s.b.y;
		for (short int i = 0; i < grid.size(); i++) {
			if (grid[i].goal == true
				&& newstate.b.x == grid[i].p.x && newstate.b.y == grid[i].p.y &&
				newstate.a.x == newstate.b.x && newstate.a.y == newstate.b.y + 1) {
				return 100;//reach to the goal
			}

			if (grid[i].p.x == newstate.a.x  && grid[i].p.y == newstate.a.y
				|| grid[i].p.x == newstate.b.x && grid[i].p.y == newstate.b.y) {
				return -10;
			}
		}
		if (newstate.a.x == s.b.x && newstate.a.y == s.b.y) { return 10; } //hit the box
	}

	
}

vector<value> value_iteration() {

}

int main()
{
	vector<tile> temp = makegrid();

	return 0;
}
