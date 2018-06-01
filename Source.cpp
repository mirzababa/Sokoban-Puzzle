/*The first AI home work, third cource
Writen by Yasamin Sadat Mirzababa
start time: May 24, 2018*/

#include<iostream>
#include<vector>
#include <fstream> //read from file
#include <string>

using namespace std;

struct tile
{
	bool box, agent, wall, free, goal;
	char shape;
	
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
		cout << "\n\tCouldn't read file!\n\tPlease enter correct path \n\n";
		exit(0);
	}
	return ED;
}

/*Explanation: Getting free houses to move the agent in the game environment
input: --
output:The vector includes the houses other than walls of the game environment */
vector<tile> readfile()
{
	vector<tile> MovementEnvironmet;
	fstream inputfile;
	inputfile.open("E:\\Course\\AI\\AI3\\project\\HW1\\Soko1.txt");
	if (inputfile.is_open()) {
		string dummyLine;
		getline(inputfile, dummyLine);//skip file's first line 

		short int line = GetEnvironmentDimensions().first;
		short int col = GetEnvironmentDimensions().second;
		tile temp;
		

		for (short int i = 0; i < line; i++)
		{
			for (short int j = 0; j < col; j++)
			{
				temp.agent = temp.box = temp.free = temp.goal = temp.wall = false;
				inputfile >> temp.shape;

				if (temp.shape != '#')
				{
					if (temp.shape == '.') {
						temp.free = true;
						MovementEnvironmet.push_back(temp);
					}
					if (temp.shape == '@') {
						temp.box = true;
						MovementEnvironmet.push_back(temp);
					}
					if (temp.shape == 'X') {
						temp.goal = true;
						MovementEnvironmet.push_back(temp);
					}
					if (temp.shape == 'S') {
						temp.agent = true;
						MovementEnvironmet.push_back(temp);
					}
				}
			}
		}
		
	}
	else
	{
		cout << "\n\tCouldn't read file!\n\tPlease enter correct path \n\n";
		exit(0);
	}
	return MovementEnvironmet;
}

int main() {
	vector<tile> Environmet = readfile();
	return 0;
}
