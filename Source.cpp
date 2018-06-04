/*The first AI home work, third cource
Writen by Yasamin Sadat Mirzababa
start time: May 24, 2018*/

#include<iostream>
#include<vector>
#include <fstream> //read from file
#include <string> //getline()
#include <algorithm>//find

using namespace std;

struct tile
{
	bool box, agent, wall, free, goal;
	char shape;
	pair<short int, short int> position;
};

struct board
{
	vector <vector<tile> > Grid;
};

/*available action for each state
*/
vector<short int> action = { 1,2,3,4 };

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

/*Explanation: Read file and Create Environment Matrix
input: void
output:Grid*/
board Board()
{
	fstream inputfile;
	inputfile.open("E:\\Course\\AI\\AI3\\project\\HW1\\Soko1.txt");
	if (inputfile.is_open()) 
	{
		string dummyLine;
		getline(inputfile, dummyLine);//skip file's first line 

		short int line = GetEnvironmentDimensions().first;
		short int col = GetEnvironmentDimensions().second;

		vector <vector<tile> > input(line, vector<tile>(col)); //create dynamic line*col vector
		for (short int i = 0; i < line; i++)
		{
			for (short int j = 0; j < col; j++)
			{
				inputfile >> input[i][j].shape;

				if (input[i][j].shape == '#') { input[i][j].wall = true; }
				if (input[i][j].shape == 'S') { input[i][j].agent = true; }
				if (input[i][j].shape == '.') { input[i][j].free = true; }
				if (input[i][j].shape == '@') { input[i][j].box = true; }
				if (input[i][j].shape == 'X') { input[i][j].goal = true; }

				input[i][j].position.first = i;
				input[i][j].position.second = j;
			}
		}
		board result;
		result.Grid = input;
		return result;
	}
	else
	{
		cout << "\n\tCouldn't read file!\n\tPlease enter correct path \n\n";
		exit(0);
	}

}

/*Explanation: Return all next states that are possible
input: game environmet and an integr to mention direction of move
1 -> up
2 -> right
3 -> down
4 -> left
output:The list include next states
*/
vector < board> StateGenerator(board temp , short int which)
{
	vector <board> output;
	board b = temp;
	//move up
	if (which == 1)
	{
		for (short int i = 0; i < GetEnvironmentDimensions().first; i++)
		{
			for (short int j = 0; j < GetEnvironmentDimensions().second; j++)
			{
				if (b.Grid[i][j].agent == true) //find agent
				{
					if (i >= 1)
					{
						if (b.Grid[i - 1][j].box == false) // will not hit the box
						{
							//Simulate motion
							b.Grid[i][j].agent = false;
							//b.Grid[i][j].free = true;

							b.Grid[i - 1][j].agent = true;

							output.push_back(b);
							break;
						}
						if (b.Grid[i - 1][j].box == true) // will hit the box
						{
							//Simulate motion
							b.Grid[i][j].agent = false;
							b.Grid[i][j].free = true;

							b.Grid[i - 1][j].agent = true;
							b.Grid[i - 1][j].box = false;

							b.Grid[i - 2][j].box = true;

							output.push_back(b);
							break;
						}
					}
				}
			}
		}
	}
	//move down
	if (which == 3)
	{
		for (short int i = 0; i < GetEnvironmentDimensions().first; i++)
		{
			for (short int j = 0; j < GetEnvironmentDimensions().second; j++)
			{
				if (b.Grid[i][j].agent == true) //find agent
				{
					if (i < GetEnvironmentDimensions().first - 1)
					{
						if (b.Grid[i + 1][j].box == false) //will not hit the box
						{
							//Simulate motion
							b.Grid[i][j].agent = false;
							
							b.Grid[i + 1][j].agent = true;

							output.push_back(b);
							break;
						}
						if (b.Grid[i + 1][j].box == true) // will hit the box
						{
							//Simulate motion
							b.Grid[i][j].agent = false;
							//b.Grid[i][j].free = true;

							b.Grid[i + 1][j].agent = true;
							b.Grid[i + 1][j].box = false;

							b.Grid[i + 2][j].box = true;
							//b.Grid[i + 2][j].free = false;

							output.push_back(b);
							break;
						}
					}
				}
			}
		}
	}
	//move left
	if (which == 4)
	{
		for (short int i = 0; i < GetEnvironmentDimensions().first; i++)
		{
			for (short int j = 0; j < GetEnvironmentDimensions().second; j++)
			{
				if (b.Grid[i][j].agent == true) //find agent
				{
					if (j > 0)
					{
						if (b.Grid[i][j - 1].box == false) //will not hit the box
						{
							//Simulate motion
							b.Grid[i][j].agent = false;
							//b.Grid[i][j].free = true;

							b.Grid[i][j - 1].agent = true;
							//b.Grid[i][j - 1].free = false;

							output.push_back(b);
							break;
						}
						if (b.Grid[i][j - 1].box == true) // will hit the box
						{
							//Simulate motion
							b.Grid[i][j].agent = false;
							//b.Grid[i][j].free = true;

							b.Grid[i][j - 1].agent = true;
							b.Grid[i][j - 1].box = false;

							b.Grid[i][j - 2].box = true;
							//b.Grid[i][j - 2].free = false;

							output.push_back(b);
							break;

						}
					}
				}
			}
		}
	}
	//move right
	if (which == 2)
	{
		for (short int i = 0; i < GetEnvironmentDimensions().first; i++)
		{
			for (short int j = 0; j < GetEnvironmentDimensions().second; j++)
			{
				if (b.Grid[i][j].agent == true) //find agent
				{
					if (j < GetEnvironmentDimensions().second - 1)
					{
						if (b.Grid[i][j + 1].box == false) //will not hit the box
						{
							//Simulate motion
							b.Grid[i][j].agent = false;
							//b.Grid[i][j].free = true;

							b.Grid[i][j + 1].agent = true;
							//b.Grid[i][j + 1].free = false;

							output.push_back(b);
							break;
						}
						if (b.Grid[i][j + 1].box == true) // will hit the box
						{
							//Simulate motion
							b.Grid[i][j].agent = false;
							//b.Grid[i][j].free = true;

							b.Grid[i][j + 1].agent = true;
							b.Grid[i][j + 1].box = false;

							b.Grid[i][j + 2].box = true;
							//b.Grid[i][j + 2].free = false;

							output.push_back(b);
							break;
						}
					}
				}
			}
		}
	}
	return output;
}

/*Explanation: get reward for state that the agent transfer to it
input : Board
output : an integer that point to reward for each state*/
short int reward(board b)
{
	bool flag = false;//flag change to true when the agent reach to the box firt time
	for (short int i = 0; i < GetEnvironmentDimensions().first; i++)
	{
		for (short int j = 0; j < GetEnvironmentDimensions().second; j++) 
		{
			//reward = 100 -> Collision with the goal 
			if (b.Grid[i][j].box == true && b.Grid[i][j].goal == true)
			{
				if (i > 0) {//up
					if (b.Grid[i - 1][j].agent == true) { return 100; }
				}
				if (i < GetEnvironmentDimensions().first - 1) {//down
					if (b.Grid[i + 1][j].agent == true) { return 100; }
				}
				if (j < GetEnvironmentDimensions().second - 1) {//right
					if (b.Grid[i][j + 1].agent == true) { return 100; }
				}
				if (j > 0) {//left
					if (b.Grid[i][j - 1].agent == true) { return 100; }
				}
			}

			//reward = -10 -> Collision with the wall after moving
			if (b.Grid[i][j].agent == b.Grid[i][j].wall == true) { return -10; }

			//reward = 10 -> Collision with the box(just in first time)
			if (flag != true)
			{
				if (i > 0) {//up
					if (b.Grid[i - 1][j].box == true) { return 10; }
				}
				if (i < GetEnvironmentDimensions().first - 1) {//down
					if (b.Grid[i + 1][j].box == true) { return 10; }
				}
				if (j < GetEnvironmentDimensions().second - 1) {//right
					if (b.Grid[i][j + 1].box == true) { return 10; }
				}
				if (j > 0) {//left
					if (b.Grid[i][j - 1].box == true) { return 10; }
				}
			}
		}
	}
}

int main() {
	board input = Board(); 
	//vector <board> BOARD = StateGenerator(input);
	return 0;
}