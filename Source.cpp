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
	short int a; //witch action
};

/*Explanation: Gaining the dimensions of the environment
input: --
output:The pair includes the length and width of the game environment */
pair<short int, short int> GetEnvironmentDimensions()
{
	pair<short int, short int> ED;
	fstream inputfile;
	short int line, column;
	inputfile.open("E:\\Course\\AI\\AI3\\project\\HW1\\Soko.txt");
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
	inputfile.open("E:\\Course\\AI\\AI3\\project\\HW1\\Soko.txt");
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

/*Explanation: Getting free houses to move the agent in the game environment
input: --
output:The vector includes the houses other than walls of the game environment*/
state upstate(state s, vector<tile>grid)
{
	state junk;
	junk.a.x = junk.a.y = -90;
	junk.b.x = junk.b.y = -90;

	state temp;
	for (short int i = 0; i < grid.size(); i++)
	{
		if (s.a.x == grid[i].p.x + 1 && s.a.y == grid[i].p.y)
		{
			temp.a.x = s.a.x - 1;	temp.a.y = s.a.y;
			temp.b.x = s.b.x;	temp.b.y = s.b.y;
			if (temp.a.x == temp.b.x && temp.a.y == temp.b.y) //???? ?? ???? ????? ?? ???? ???
			{
				for (short int j = 0; j < grid.size(); j++)
				{
					if (temp.a.x == grid[i].p.x + 1 && temp.a.y == grid[i].p.y)
					{
						temp.b.x = temp.b.x - 1;	temp.b.y = temp.b.y;
						return temp;
					}
				}
			}
			return temp;
		}
	}
	return junk;
}

state rightstate(state s, vector<tile>grid) {
	state junk;
	junk.a.x = junk.a.y = -90;
	junk.b.x = junk.b.y = -90;

	state temp2;
	for (short int i = 0; i < grid.size(); i++)
	{
		if (s.a.x == grid[i].p.x && s.a.y == grid[i].p.y - 1)
		{
			temp2.a.x = s.a.x;	temp2.a.y = s.a.y + 1;
			temp2.b.x = s.b.x;	temp2.b.y = s.b.y;
			if (temp2.a.x == temp2.b.x && temp2.a.y == temp2.b.y)
			{
				for (short int j = 0; j < grid.size(); j++)
				{
					if (temp2.a.x == grid[i].p.x && temp2.a.y == grid[i].p.y - 1)
					{
						temp2.b.x = temp2.b.x;	temp2.b.y = temp2.b.y + 1;
						return temp2;
					}
				}
			}
			return temp2;
		}
	}
	return junk;
}

state downstate(state s, vector<tile>grid) {
	state junk;
	junk.a.x = junk.a.y = -90;
	junk.b.x = junk.b.y = -90;

	state temp;
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
						return temp;
					}
				}
			}
			return temp;
		}
	}
	return junk;
}

state leftstate(state s, vector<tile>grid) {
	state junk;
	junk.a.x = junk.a.y = -90;
	junk.b.x = junk.b.y = -90;

	state temp;
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
						return temp;
					}
				}
			}
			return temp;
		}
	}
	return junk;
}


/*vector<state> allstates(state s, vector<tile>grid)
{
	 
	short int iterator = 1;
	vector<state> states;
	state temp;
	
		temp = stategenerator(s, grid);
		states.push_back(temp);
	
	/*while (!states.empty())
	{
		for (short int j = 0; j <= 4; j++) {//for each action
			temp = stategenerator(states[iterator], grid, j);
			for (short int k = 0; k < states.size(); k++) {
				if (temp.a.x == states[k].a.x && temp.a.y == states[k].a.y
					&& temp.b.x == states[k].b.x && temp.b.y == states[k].b.y) // skip repetitious state
				{
					continue;
				}
			}
			states.push_back(temp);
			iterator = iterator + 1;
		}
	}
	return states;
}*/

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
	//	state[i] = stategenerator(s, grid, i);
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
	vector<tile> sa = makegrid();
	//float v;
	state start;
	state starttemp1;
	state starttemp2;
	state starttemp3;
	state starttemp4;
	vector<state> news;
	start.a.x = start.b.x = -100;
	for (short int i = 0; i < sa.size(); i++) {
		if (sa[i].agent == true) {
			start.a.x = sa[i].p.x;	start.a.y = sa[i].p.y;
		}
		if (sa[i].box == true) {
			start.b.x = sa[i].p.x;	start.b.y = sa[i].p.y;
		}
		if (start.a.x > 0 && start.b.x > 0) { break; }
	}
	/*
	for (int i = 0; i < 10; i++) {
		reward_10 = false;
		v = value_iteration(start, 0.8, temp);
	}
	 */
	
	//allstates(start, temp);
	//stategenerator(start, temp, 1);
	/*state tempp;
	for (short int i = 0; i < temp.size(); i++)
	{
		if (start.a.x == temp[i].p.x + 1 && start.a.y == temp[i].p.y)
		{
			tempp.a.x = start.a.x - 1;	tempp.a.y = start.a.y;
			tempp.b.x = start.b.x;	tempp.b.y = start.b.y;
			if (tempp.a.x == tempp.b.x && tempp.a.y == tempp.b.y) //رفته رو خونه بغلیش که جعبه هست
			{
				for (short int j = 0; j < temp.size(); j++)
				{
					if (tempp.a.x == temp[i].p.x + 1 && tempp.a.y == temp[i].p.y)
					{
						tempp.b.x = tempp.b.x - 1;	tempp.b.y = tempp.b.y;
					}
					//cout << tempp.a.x << "," << tempp.a.y << "\n" << tempp.b.x << "," << tempp.b.y << "\n";
					news.push_back(tempp);
				}
			}
		}
	}
	cout << tempp.a.x << "," << tempp.a.y << "\n" << tempp.b.x << "," << tempp.b.y << "\n";*/
	//for (short int d = 1; d <= 4; d++) {
	/*starttemp1 = upstate(start, sa);
		news.push_back(starttemp1);

		starttemp2 = rightstate(start, sa);
		news.push_back(starttemp2);

		starttemp1 = stategenerator(start, sa, 3);
		news.push_back(starttemp3);

		starttemp1 = stategenerator(start, sa, 4);
		news.push_back(starttemp4);
*/
		
	
		starttemp1 = upstate(start, sa);
		if(starttemp1.a.x != -90)
		news.push_back(starttemp1);

		starttemp1 = rightstate(start, sa);
		if (starttemp1.a.x != -90)
			news.push_back(starttemp1);

		starttemp1 = downstate(start, sa);
		if (starttemp1.a.x != -90)
			news.push_back(starttemp1);

		starttemp1 = leftstate(start, sa);
		if (starttemp1.a.x != -90)
			news.push_back(starttemp1);
	

//	}
	 


	return 0;
}
