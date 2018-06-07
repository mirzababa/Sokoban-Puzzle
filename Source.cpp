/*The first AI home work, third cource
Writen by Yasamin Sadat Mirzababa
start time: May 24, 2018*/

#include<iostream>
#include<vector>
#include <fstream> //read from file
#include <string> //getline()
#include <algorithm>    // std::max

using namespace std;

bool reward_10 = false;

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

/*Explanation: Getting free houses to move the agent in the game environment
input: --
output:The vector includes the houses other than walls of the game environment*/
state stategenerator(state s, vector<tile>grid, short int w)
{
	state temp;
	//up
	if (w == 1)
	{
		for (short int i = 0; i < grid.size(); i++)
		{
			if (s.a.x == grid[i].p.x + 1 && s.a.y == grid[i].p.y)
			{
				temp.a.x = s.a.x - 1;	temp.a.y = s.a.y;
				temp.b.x = s.b.x;	temp.b.y = s.b.y;
				if (temp.a.x == temp.b.x && temp.a.y == temp.b.y) //رفته رو خونه بغلیش که جعبه هست
				{
					for (short int j = 0; j < grid.size(); j++)
					{
						if (temp.a.x == grid[i].p.x + 1 && temp.a.y == grid[i].p.y)
						{
							temp.b.x = temp.b.x - 1;	temp.b.y = temp.b.y;
						}
						return temp;
					}
				}
			}
		}
	}

	//right
	if (w == 2)
	{
		for (short int i = 0; i < grid.size(); i++)
		{
			if (s.a.x == grid[i].p.x && s.a.y == grid[i].p.y - 1)
			{
				temp.a.x = s.a.x;	temp.a.y = s.a.y + 1;
				temp.b.x = s.b.x;	temp.b.y = s.b.y;
				if (temp.a.x == temp.b.x && temp.a.y == temp.b.y) 
				{
					for (short int j = 0; j < grid.size(); j++)
					{
						if (temp.a.x == grid[i].p.x && temp.a.y == grid[i].p.y - 1)
						{
							temp.b.x = temp.b.x;	temp.b.y = temp.b.y + 1;
						}
						return temp;
					}
				}
			}
		}
	}

	//down
	if (w == 3)
	{
		for (short int i = 0; i < grid.size(); i++)
		{
			if (s.a.x == grid[i].p.x - 1 && s.a.y == grid[i].p.y)
			{
				temp.a.x = s.a.x + 1;	temp.a.y = s.a.y;
				temp.b.x = s.b.x;	temp.b.y = s.b.y;
				if (temp.a.x == temp.b.x && temp.a.y == temp.b.y) 
				{
					for (short int j = 0; j < grid.size(); j++)
					{
						if (temp.a.x == grid[i].p.x - 1 && temp.a.y == grid[i].p.y)
						{
							temp.b.x = temp.b.x + 1;	temp.b.y = temp.b.y;
						}
						return temp;
					}
				}
			}
		}
	}

	//left
	if (w == 4)
	{
		for (short int i = 0; i < grid.size(); i++)
		{
			if (s.a.x == grid[i].p.x && s.a.y == grid[i].p.y + 1)
			{
				temp.a.x = s.a.x;	temp.a.y = s.a.y - 1;
				temp.b.x = s.b.x;	temp.b.y = s.b.y;
				if (temp.a.x == temp.b.x && temp.a.y == temp.b.y) 
				{
					for (short int j = 0; j < grid.size(); j++)
					{
						if (temp.a.x == grid[i].p.x && temp.a.y == grid[i].p.y + 1)
						{
							temp.b.x = temp.b.x;	temp.b.y = temp.b.y - 1;
						}
						return temp;
					}
				}
			}
		}
	}
}

/*Explanation: Getting free houses to move the agent in the game environment
input: --
output:The vector includes the houses other than walls of the game environment*/
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

			if (reward_10 == false && grid[i].p.x == newstate.a.x  && grid[i].p.y == newstate.a.y
				|| grid[i].p.x == newstate.b.x && grid[i].p.y == newstate.b.y) {
				reward_10 = true;
				return -10;
			}
		}
		if (newstate.a.x == s.b.x && newstate.a.y == s.b.y) { return 10; } //hit the box

		else { return 0; }
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

			if (reward_10 == false && grid[i].p.x == newstate.a.x  && grid[i].p.y == newstate.a.y
				|| grid[i].p.x == newstate.b.x && grid[i].p.y == newstate.b.y) {
				reward_10 = true;
				return -10;
			}
		}
		if (newstate.a.x == s.b.x && newstate.a.y == s.b.y) { return 10; } //hit the box

		else { return 0; }
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

			if (reward_10 == false && grid[i].p.x == newstate.a.x  && grid[i].p.y == newstate.a.y
				|| grid[i].p.x == newstate.b.x && grid[i].p.y == newstate.b.y) {
				reward_10 = true;
				return -10;
			}
		}
		if (newstate.a.x == s.b.x && newstate.a.y == s.b.y) { return 10; } //hit the box

		else { return 0; }
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

			if (reward_10 == false && grid[i].p.x == newstate.a.x  && grid[i].p.y == newstate.a.y
				|| grid[i].p.x == newstate.b.x && grid[i].p.y == newstate.b.y) {
				reward_10 = true;
				return -10;
			}
		}
		if (newstate.a.x == s.b.x && newstate.a.y == s.b.y) { return 10; } //hit the box

		else { return 0; }
	}

	
}

/*Explanation: Getting free houses to move the agent in the game environment
input: --
output:The vector includes the houses other than walls of the game environment*/
//a -> eta
float value_iteration(state s, float a, vector<tile>grid) 
{
	bool ifconvergence = false; //to find out if convergence accured or not
	float val;
	vector <float> max;
	short int iterations = 0;
	vector<value> v;
	value init; 
	state temp;
	vector<state> state; // output of state generator

	state.push_back(s);
	//initial value table
	init.s = s;	init.v = 0.0;
	v.push_back(init);
	temp = s;
	state.erase(state.begin());
	for (short int i = 1; i <= 4; i++) {
		state[i] = stategenerator(s, grid, i);
		init.s = state[i];	init.v = 0.0;
		v.push_back(init);
	}
	
	while (ifconvergence == false)
	{
		iterations += 1;

		for (short int i = 0; i < state.size(); i++) {  //for each state
			for (short int j = 1; j <= 4; j++) { //for each action

				max[j] = reward(temp, j, grid) + a * (value_iteration(state[i], a, grid));
			}
			val = *max_element(max.begin(), max.end());
		}
	}
	return val;
}

int main()
{
	vector<tile> temp = makegrid();
	float v;
	state start;
	start.a.x = start.b.x = -100;
	for (short int i = 0; i < temp.size(); i++) {
		if (temp[i].agent == true) {
			start.a.x = temp[i].p.x;	start.a.y = temp[i].p.y;
		}
		if (temp[i].box == true) {
			start.b.x = temp[i].p.x;	start.b.y = temp[i].p.y;
		}
		if (start.a.x > 0 && start.b.x > 0) { break; }
	}

	for (int i = 0; i < 10; i++) {
		reward_10 = false;
		v = value_iteration(start, 0.8, temp);
	}
	 
	 
	return 0;
}
