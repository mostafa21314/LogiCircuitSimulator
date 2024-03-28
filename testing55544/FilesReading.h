#pragma once
#include<fstream>
#include<vector>
#include<string>
#include"Structs.h"
using namespace std;
class FilesReading
{
public:
	bool error = 0;
	vector<string> errorsHandling;
	LibStruct Lib;
	CircuitStruct cir;
	StimStruct stim;
	priority_queue<Event> finalEvents;
	vector<LogicGate> gates;
	void ReadLibFile(string s);
	void ReadCirFile(string s);
	void ReadStimFile(string s);
	void RunSimulation();
	void PrintLib();
	void PrintCir();
	void PrintStim();
	void PrintGates();
	void PrintFinal();
	void ProduceFile(string s);

};

