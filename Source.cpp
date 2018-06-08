/*The first AI home work, third cource
Writen by Yasamin Sadat Mirzababa
start time: June 01, 2018*/

#include<iostream>
#include<vector>
#include <fstream> //read from file
#include <string> //getline()
#include <algorithm>    // std::max

using namespace std;

bool reward_10 = false; //if the reward_10 if false -> reward '-10' not assigned to any state 

/**
determine the agent position
x: the agent line in the environment
y: the agent column in the environment
*/
struct agent
{
	short int x, y;
};

/**
determine the box position
x: the box line in the environment
y: the agent column in the environment
*/
struct box
{
	short int x, y;
};

/*
to present each state. with each agent's move, a new state is created

each state contain "a box" and an "agent"
*/
struct state
{
	agent a;
	box b;
};

/*

*/
struct position {
	short int x, y;
};

/*
used to capture the environment rfom the file
*/
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

struct policy {
	state s;
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

/*Explanation: create next state
input: state s -> the state we want to find next state
vector<tile>grid -> game environment
output:The state in wich the agent has moved up*/
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

/*Explanation: create next state
input: state s -> the state we want to find next state
vector<tile>grid -> game environment
output:The state in wich the agent has moved right*/
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

/*Explanation: create next state
input: state s -> the state we want to find next state
vector<tile>grid -> game environment
output:The state in wich the agent has moved down*/
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

/*Explanation: create next state
input: state s -> the state we want to find next state
vector<tile>grid -> game environment
output:The state in wich the agent has moved left*/
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

/*Explanation: generate all state that will create
input: state s -> the starting state
vector<tile>grid -> game environment
output:a state contain all possible state that created from the start state*/
vector<state> allstates(state s, vector<tile>grid)  //adjency matrix
{
	vector<state> all;

	/*short int iterator = 1;
	vector<state> states;
	state temp;

	//	temp = stategenerator(s, grid);
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
	return states;*/
	return all;
}

/*Explanation: if the agent is in the current state and perform a definite move, what is the new state?
input: state s -> the current state
vector<tile>grid -> game environment
short int a -> a=1 (move up)
a=2 (move right)
a=3 (move down)
a=4 (move left)
output:an integer to present reward for new state*/
short int reward(state s, vector<tile>grid, short int a)
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

/*Explanation: the value of each state
input: vector<state>all -> output of allstates function
state s -> the current state
vector<tile>grid -> game environment
short int a -> a=1 (move up)
a=2 (move right)
a=3 (move down)
a=4 (move left)
output:a float number to present value for each state*/
float value_iteration(vector<state>all, state s, short int a, vector<tile>grid)
{
	float eta = 0.8;
	float val;
	vector<float>tempval;
	vector<state> states;
	state temp;
	/*
	temp = upstate(s, grid);
	if (temp.a.x != -90) {
	states.push_back(temp);

	}
	temp = rightstate(s, grid);
	if (temp.a.x != -90) {
	states.push_back(temp);

	}
	temp = downstate(s, grid);
	if (temp.a.x != -90) {
	states.push_back(temp);
	}
	temp = leftstate(s, grid);
	if (temp.a.x != -90) {
	states.push_back(temp);
	}
	*/
	for (short int i = 0; i < all.size(); i++)
	{
		s = all[i];
		for (short int j = 1; j <= 4; j++)
		{
			tempval[j] = reward(all[i], grid, j) + eta*(value_iteration(all, s, a, grid));
		}
		val = std::max_element(tempval.front(), tempval.back());
	}
	return val;


}

/*Explanation: map the value to the state and action
input: vector<state>all -> output of allstates function
state s -> the current state
vector<tile>grid -> game environment
short int a -> a=1 (move up)
a=2 (move right)
a=3 (move down)
a=4 (move left)
output:a list contain state,value and the action that reach this value for this function*/
vector<value> value_table(vector<state> all, state s, short int a, vector<tile>grid)
{
	float eta = 0.8;
	value init;
	int iterations = 0;
	state goal;
	for (short int i = 0; i < grid.size(); i++) {
		if (grid[i].goal == true)
		{
			goal.a.x = grid[i].p.x;		goal.a.y = grid[i].p.y;
		}
	}
	bool ifconvergence = false; //to find out if convergence accured or not
	vector<value> output;


	while (s.a.x != goal.a.x && s.a.y != goal.a.y) //not reach to goal
	{
		iterations += 1;

		init.v = value_iteration(all, s, a, grid);
		init.a = a;

	}
	init.v = 100.0;
	return output;

}

/*Explanation: create policy table
input: vector<state>all -> output of allstates function
vector<tile>grid -> game environment
output:a list contain state and the action. this action is best move for agent to create new state*/
vector<policy> policy_iteration(vector<state> all, vector<tile> grid)
{
	state s;
	bool noChange;
	int iteration;
	vector<policy> poutput;
	vector<value> voutput;
	state temp;
	policy p;
	float eta = 0.8;
	//initial policy table
	for (short int i = 0; i < all.size(); i++)
	{
		temp = upstate(all[i], grid);
		if (temp.a.x != -90)
		{
			p.a = 1;
		}
		else
		{
			temp = rightstate(all[i], grid);
			if (temp.a.x != -90)
			{
				p.a = 2;
			}
			else
			{
				temp = downstate(all[i], grid);
				if (temp.a.x != -90)
				{
					p.a = 3;
				}
				else
				{
					temp = leftstate(all[i], grid);
					if (temp.a.x != -90)
					{
						p.a = 4;
					}
				}
			}
		}
	}
	poutput.push_back(p);

	//////////////////////////////////////////////
	//initialize value arbitrarily
	vector<value> v(all.size());

	noChange = true;
	iteration = 0;
	while (noChange && iteration <= 100)
	{
		noChange = false;
		iteration += 1;

		for (short int i = 0; i < all.size(); i++)
		{
			voutput = value_table(all, all[i], poutput[i].a, grid);
		}

		for (short int i = 0; i < all.size(); i++)
		{
			if (voutput[i].a != poutput[i].a)
			{
				poutput[i].a = voutput[i].a;
			}
			noChange = false;
		}

	}
	return poutput;
}


int main()
{
	vector<tile> input = makegrid();
	state start;
	vector<state> news;

	start.a.x = start.b.x = -100;
	for (short int i = 0; i < input.size(); i++) {
		if (input[i].agent == true) {
			start.a.x = input[i].p.x;	start.a.y = input[i].p.y;
		}
		if (input[i].box == true) {
			start.b.x = input[i].p.x;	start.b.y = input[i].p.y;
		}
		if (start.a.x > 0 && start.b.x > 0) { break; }
	}

	vector<state> allstate;
	allstate = allstates(start, input);
	policy_iteration(allstate, input);
	return 0;
}
