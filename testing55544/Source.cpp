#include<iostream>
#include"FilesReading.h"
#include"Structs.h"
#include"GateSimulator.h"
using namespace std;




void getLogic(LibStruct lib, LogicGate s)            //function that uses Lib file struct to match the logic of a gate with one of the logic expressions in the lib file

{
	for (int i = 0; i < lib.GatesNames.size(); i++)
	{
		if (s.type == lib.GatesNames[i])
		{
			s.logic = lib.Logic[i];
		}
	}
}

bool evaluateGate(string s, vector<bool> inp)    //functiont that uses the gate evaluator to evaluate outputs based on a vector of inputs
{
	LogicGateExpressionEvaluator evaluate;
	return evaluate.evaluateExpression(s, inp);
}



//int main(int argc, char *argv[])
//{
//    if(argc != 5) {
//        cout << "Usage: " << argv[0] << " <library file> <circuit file> <stimuli file> <output file>" << endl;
//        return 1;
//    }
//
//    string libFileName = argv[1];
//    string cirFileName = argv[2];
//    string stimFileName = argv[3];
//    string OutName = argv[4]; // Output file path is now a command line argument
//
//    FilesReading test;
//
//    test.ReadLibFile(libFileName);
//    test.ReadCirFile(cirFileName);
//    test.ReadStimFile(stimFileName);


	int main()
	{


		FilesReading test;
		string libFileName = "C:/Users/mosta/Desktop/AUC/DD1/Project/cells.lib";    //file locations, make sure you change them to work on your machine
		string cirFileName = "C:/Users/mosta/Desktop/AUC/DD1/Project/2.cir";
		string stimFileName = "C:/Users/mosta/Desktop/AUC/DD1/Project/1.stim";
		string OutName = "C:/Users/mosta/Desktop/AUC/DD1/Project/Circuit_Output.sim";      //the sim file that will be produced


		test.ReadLibFile(libFileName);
		test.ReadCirFile(cirFileName);    //here the readFile class starts reading the files and storing data
		test.ReadStimFile(stimFileName);



	test.PrintLib();
	cout << endl << endl;
	test.PrintCir();    //part of the debugging process, functions that displays whats stored in each file struct
	cout << endl << endl;
	test.PrintStim();
	cout << endl << endl;
	test.PrintGates();
	cout << endl << endl;
	test.RunSimulation();
	cout << endl << endl;
	//cout << test.stim.pq.size();
	test.PrintFinal();
	test.ProduceFile(OutName);
	


}