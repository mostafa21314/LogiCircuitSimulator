#pragma once
#include<iostream>
#include<vector>;
#include<string>
#include<queue>
using namespace std;
struct Event
{
	int timeStamp;
	bool value;
	string Name;
	bool changed;
	// Overload the less than operator for priority_queue
	bool operator<(const Event& e) const
	{
		return timeStamp > e.timeStamp;
	}
};



struct LogicGate   //struct for the logic gates
{
	string name;
	string type;
	string logic;
	vector<string> inputNames;
	vector<bool> inputs;
	string OutputName;
	bool result = 0;
	int timeStamp=-1;
	int delay=-1;


};


struct LibStruct           //struct to store data from Lib file
{
	std::vector<std::string> GatesNames;    //vectors to store different data
	std::vector<int> InputNum;
	std::vector<std::string> Logic;
	std::vector<int> Delay;
	void addGateName(std::string s)   //function to add a gate name to the appropriate vector
	{
		GatesNames.push_back(s);
	}
	void addInputNum(int n)
	{
		InputNum.push_back(n);
	}
	void addLogic(std::string s)
	{
		Logic.push_back(s);
	}
	void addDelay(int x)
	{
		Delay.push_back(x);
	}

	void PrintLib()   //function to print out the contents of the lib struct
	{


		for (int i = 0; i < GatesNames.size(); i++)
		{
			cout << GatesNames[i] << "  Input variables = " << InputNum[i] << "   Logic = " << Logic[i] << "   Delay = " << Delay[i] << endl;
		}
		cout << GatesNames.size() << "  " << InputNum.size() << "  " << Logic.size() << "  " << Delay.size() << endl;


	}
};

struct CircuitStruct   //struct to store data obtained from the cir file into vectors
{
	vector<string> logic;
	vector<string> Inputs;
    vector<string> componentName;
	vector<string> type;
	vector<string> output;
	vector<vector<string>> GateInputs;
	void addInput(string c)
	{
		Inputs.push_back(c);
	}
	void addComponentName(string s)
	{
		componentName.push_back(s);
	}
	void addType(string s)
	{
		type.push_back(s);
	}
	void addOutput(string s)
	{
		output.push_back(s);
	}
	void addGateInput(vector<string> s)
	{
		GateInputs.push_back(s);
	}

	void PrintCir()   //function to print the contents of the struct
	{
		//cout << "HELLO  " << GateInputs.size() << "    " << componentName.size()<<endl;
		for (int i = 0; i < componentName.size(); i++)
		{
			cout << componentName[i] << "  " << type[i] << "   " << output[i] << "   ";
			for (int j = 0; j < GateInputs[i].size(); j++)
			{
				cout << GateInputs[i][j] << "  ";
			}
			cout << endl;
		}
		cout << this->type.size() <<"  "<< output.size() <<"  "<< componentName.size() <<"  "<< Inputs.size() <<"  "<<GateInputs.size()<<endl;
	}
	
};

struct StimStruct   //struct to store data from stim file into vectors 
{
	
	priority_queue<Event> pq;
	void addEvent(Event& x)
	{
		pq.push(x);
	}

	void PrintStim()
	{
		priority_queue<Event> temp=pq;

		while (!temp.empty())
		{
			Event l = temp.top();
			temp.pop();
			cout << l.timeStamp << ", " << l.Name << ", " << l.value;
			if (l.changed)
			{
				cout << "    Change happened here";
			}
			else
			{
				cout << "    No change happened";
			}
			cout << endl;
		}
	}

	
};