#include "FilesReading.h"
#include <iostream>
#include"GateSimulator.h"

void FilesReading::ReadLibFile(string s)  //function to read contents from a lib file and store it in vectors
{
	ifstream LibFile;
	LibFile.open(s);

	if (!LibFile.is_open())   //making sure the file is opened correctly
	{
		error = true;
		string ssss= "error opening Lib file, please recheck the name ";
		cout << "error opening Lib file, please recheck the name ";
		errorsHandling.push_back(ssss);
		return;
	}

	while (!LibFile.eof())  //while we didnt reach the end of the file
	{
		string temp;
		LibFile >> temp;
		if (temp == "")  //if the file became empty(only have spaces or new lines
		{
			break;
		}
		//cout << temp << endl;

		temp.erase(remove(temp.begin(), temp.end(), ','), temp.end());  //we remove any commas
		FilesReading::Lib.addGateName(temp);  //we add the gat name to its vector
		LibFile >> temp;
		if (temp == "")  //if the file became empty(only have spaces or new lines
		{
			break;
		}
		//cout << temp << endl;

		temp.erase(remove(temp.begin(), temp.end(), ','), temp.end());  //we remove any commas
		Lib.addInputNum(stoi(temp));  //we add the input size of the specified gate
		getline(LibFile, temp, ',');
		if (temp == "")  //if the file became empty(only have spaces or new lines
		{
			break;
		}
		//cout << temp << endl;

		temp.erase(remove(temp.begin(), temp.end(), ','), temp.end());  //we remove any commas
		Lib.addLogic(temp);  //we add the logic definition of the specified gate
		
		LibFile >> temp;
		if (temp == "")  //if the file became empty(only have spaces or new lines
		{
			break;
		}
		//cout << temp << endl;

		temp.erase(remove(temp.begin(), temp.end(), ','), temp.end());   //we remove any commas
		Lib.addDelay(stoi(temp));
		if (temp == "")
		{
			break;
		}

	}

	LibFile.close();
}

void FilesReading::ReadCirFile(string s)   //function to read contents from the cir file
{
	ifstream CirFile;
	CirFile.open(s);

	if (!CirFile.is_open())   //handling the case when the file name is wrong
	{
		error = true;
		string ssss = "error opening Cir file, please recheck the name ";
		cout << "error opening Cir file, please recheck the name ";
		errorsHandling.push_back(ssss);
		return;
	}

	string temp;
	getline(CirFile, temp);
	//cout << temp << endl;
	
	 
	while (temp != "COMPONENTS:")   //removing the unimportant line from the file
	{
		if (!((CirFile >> temp)))
		{
			break;
		}
		if (temp == "")
		{
			break;
		}

	//	cout << temp << endl;
		FilesReading::cir.addInput(temp);  //adding the input variables
	}
	cir.Inputs.pop_back();

	while (!CirFile.eof())    //while we are reading from the file and it didnt reach the end
	{


		if (temp == "")
		{
			break;
		}
		LogicGate x;    //we create a logic gate and feed it its data from the cir file
		int inputCounter = 1;
		if (!((CirFile >> temp)))
		{
			break;
		}
		if (temp == "")
		{
			break;
		}

		//cout << temp << endl;
		temp.erase(remove(temp.begin(), temp.end(), ','), temp.end()); //we remove any commas

		x.name = temp;
		cir.addComponentName(temp);    


		CirFile >> temp;
		if (temp == "")
		{
			break;
		}

		//cout << temp << endl;
		temp.erase(remove(temp.begin(), temp.end(), ','), temp.end()); //we remove any commas

		if ((temp[temp.size() - 1] <= 57) && (temp[temp.size() - 1] >= 48))
			inputCounter = (temp[temp.size() - 1]) - '0';


		x.type = temp;


		for (int i = 0; i < Lib.GatesNames.size(); i++)
		{
			if (Lib.GatesNames[i] == temp)
			{
				cir.logic.push_back(Lib.Logic[i]);
				x.logic = Lib.Logic[i];     //getting the logic and delay from the lib file struct
				x.delay = Lib.Delay[i];
				break;
			}
			if (i == Lib.GatesNames.size() - 1)
			{
				error = true;
				string sss = "There was an error because the gate " + temp + " doesnt exist in the lib file";
				errorsHandling.push_back(sss);
				break;
			}
		
		}

		cir.addType(temp);
		CirFile >> temp;
		if (temp == "")
		{
			break;
		}

		//cout << temp << endl;
		temp.erase(remove(temp.begin(), temp.end(), ','), temp.end()); //we remove any commas

		x.OutputName = temp;
		cir.addOutput(temp);

		vector<string> soy;
		while (inputCounter != 0)
		{
			CirFile >> temp;
			if (temp == "")
			{
				break;
			}

			temp.erase(remove(temp.begin(), temp.end(), ','), temp.end());
			inputCounter--;
			soy.push_back(temp);
		}
		cir.addGateInput(soy);
		x.inputNames = soy;
		for (int i = 0; i < soy.size(); i++)
		{
			x.inputs.push_back(0);
		}
		LogicGateExpressionEvaluator evaluate;
		x.result = evaluate.evaluateExpression(x.logic, x.inputs);
		gates.push_back(x);   //we finally push the gate to its struct
		Event xx;
		xx.Name = x.OutputName;
		xx.value = x.result;
		xx.changed = false;
		xx.timeStamp = 0;

		for (int i = 0; i < cir.Inputs.size(); i++)
		{
			cout << x.OutputName << "   " << cir.Inputs[i] << endl;
			if (x.OutputName == cir.Inputs[i])
			{
				error = true;

				string sss;
				sss = "There was an error in gate " + x.name + " where one of circuit inputs is its output which should be impossible ";
				errorsHandling.push_back(sss);
			}
		}
		
		if(x.result)
		{
			xx.changed = true;
			xx.timeStamp = x.delay;
			finalEvents.push(xx);
		}
		stim.pq.push(xx);
		if (temp == "")
		{
			break;
		}

		/*for (int i = 0; i < gates.size(); i++)
		{
			for (int j = 0; j < gates[i].inputNames.size(); j++)
			{
				cout << gates[i].inputNames[j] << "   " << x.OutputName << endl;
				if (gates[i].inputNames[j] == x.OutputName)
				{
					cout << "idaiaisjndaisljabdkjlakljdnsakjldnalasd;nasl;nd";
					
					bool old = gates[i].result;
					gates[i].inputs[j] = x.result;
					gates[i].result = evaluate.evaluateExpression(gates[i].logic, gates[i].inputs);
					Event kk;
					kk.changed = false;
					if (gates[i].result != old)
					{
						kk.changed = true;
					}

						kk.Name = gates[i].OutputName;
						kk.value = gates[i].result;
						kk.timeStamp = gates[i].delay ;
						stim.pq.push(kk);
						if(kk.changed)
						{
							finalEvents.push(kk);
					    }
				}
			}
		}*/

	}


	CirFile.close();
}

void FilesReading::ReadStimFile(string s)  //function to read the contents of a stim file
{
	ifstream FileOpen;
	FileOpen.open(s);
	if (!FileOpen.is_open())  //handling an error instance
	{
		string s = "error opening stim file, please recheck the name ";
		cout << "error opening stim file, please recheck the name ";
		error = true;
		errorsHandling.push_back(s);
		return;
	}
	string temp;
	while (!FileOpen.eof())   //filling the appropriate vectors with the data needed
	{
		Event x;
		FileOpen >> temp;
		temp.erase(remove(temp.begin(), temp.end(), ','), temp.end());
		x.timeStamp=(stoi(temp));

		FileOpen >> temp;
		temp.erase(remove(temp.begin(), temp.end(), ','), temp.end());
		x.Name = temp;
		FileOpen >> temp;
		temp.erase(remove(temp.begin(), temp.end(), ','), temp.end());
		x.value = false;
		x.changed = false;
		if (temp != "0")
		{
			x.value = true;
			x.changed = true;
		}
	      if (temp != "1"&&temp!="0")
		{
			  Event error;
			  error.changed = false;
			  error.timeStamp = INT_MAX;
			  error.Name = "There was an error with the input "+x.Name+ " where its truth value wasnt 0 or 1. It was considered to be 1";
			  error.value = false;
			  stim.addEvent(error);
		}
		stim.addEvent(x);
		//if (x.Name != "0")
		finalEvents.push(x);

	}





	

	FileOpen.close();
}

void FilesReading::PrintLib()//function to print contents of the lib file
{
	Lib.PrintLib();
}


void FilesReading::PrintCir()//function to print contents of the cir file
{
	cir.PrintCir();
}

void FilesReading::PrintStim()  //function to print contents of the stim file
{
	stim.PrintStim();
}
void FilesReading::PrintFinal()
{

	priority_queue<Event> temp=finalEvents;
	if(!error)
	while (!temp.empty())
	{
		Event x = temp.top();
		temp.pop();
		cout << x.timeStamp << ", " << x.Name << ", " << x.value;
		cout << endl;
	}
	else
	{
		for (int i = 0; i < errorsHandling.size(); i++)
		{
			cout << errorsHandling[i] << endl;
		}
	}
}

void FilesReading::ProduceFile(string s)
{

	ofstream fileOutput;
	fileOutput.open(s);

	if(!error)
	while (!finalEvents.empty())
	{
		Event x = finalEvents.top();
		finalEvents.pop();
		fileOutput << x.timeStamp << ", " << x.Name << ", " << x.value<<endl;
	}
	else
	{
	
			for (int i = 0; i < errorsHandling.size(); i++)
			{
				fileOutput << errorsHandling[i] << endl;
		    }
	
	}

}

void FilesReading::PrintGates()//function to print contents of the gates struct
{
	for (int i = 0; i < gates.size(); i++)
	{
		cout << gates[i].name << "  " << gates[i].type << "  " << gates[i].logic << "  " << gates[i].OutputName << "  ";
		for (int j = 0;j < gates[i].inputNames.size(); j++)
		{
			cout << gates[i].inputNames[j]<<"  ";
		}
		cout << "Output : " << gates[i].result<<"  "<< "Gate Delay : "<<gates[i].delay<<"  Time Stamp : "<<gates[i].timeStamp; //prints it in a semi tabular form
		cout << endl<<endl;
	}


}



void FilesReading::RunSimulation()
{

	while (!stim.pq.empty())
	{
		Event star = stim.pq.top();
		stim.pq.pop();
		for (int i = 0; i < gates.size(); i++)
		{
			for (int j = 0; j < gates[i].inputNames.size(); j++)
			{
				if (gates[i].inputNames[j] == star.Name)
				{
					gates[i].inputs[j] = star.value;
					bool oldResult = gates[i].result;
					LogicGateExpressionEvaluator evaluate;
					gates[i].result = evaluate.evaluateExpression(gates[i].logic, gates[i].inputs);
					if (gates[i].result != oldResult)
					{
						Event croissant;
						croissant.value = gates[i].result;
						croissant.changed = true;
						croissant.Name = gates[i].OutputName;
						croissant.timeStamp = star.timeStamp + gates[i].delay;
						finalEvents.push(croissant);
						stim.pq.push(croissant);
					}

				}
			}
		}
	}
	//for (int i = 0; i < test.gates.size(); i++)        //the main part of the program. This is used to evaluate each logic gate and decide its output and time stamp and stores them in their appropriate vectors
	//{
	//	int counter = 0;
	//	int maxDelay = INT_MIN;   //integer to store the maximum delay of all the inputs

	//	for (int j = 0; j < test.gates[i].inputNames.size(); j++)    //loop to go over all the gates
	//	{


	//		for (int k = 0; k < test.stim.inputVariables.size(); k++)    //loop that goes over all the known variable 
	//		{

	//			cout << test.gates[i].inputNames[j] << "    " << test.stim.inputVariables[k] << "  " << test.stim.status[k] << endl;     //part of the debugging process
	//			if (test.gates[i].inputNames[j] == test.stim.inputVariables[k]) //if we find a matching input for the logic gate
	//			{
	//				if (test.stim.Delay[k] > maxDelay)
	//				{
	//					maxDelay = test.stim.Delay[k];        //we record the max delay
	//				}
	//				test.gates[i].inputs[j] = test.stim.status[k];  //we change the status of the logic gate input value
	//				counter++;




	//				bool temporayStore = test.gates[i].result;
	//				/*for (int u = 0; u < test.gates[i].inputs.size(); u++)
	//				{
	//					cout << test.gates[i].inputs[u] << "  ";
	//				}*/
	//				int StoreTime;                                                    //here we calculate the time stamp, evaluate the output and pushes them to their appropriate vectors
	//				StoreTime = test.gates[i].timeStamp;
	//				test.gates[i].timeStamp = maxDelay + test.gates[i].delay;
	//				test.gates[i].result = evaluateGate(test.gates[i].logic, test.gates[i].inputs);
	//				test.stim.addInputVariable(test.gates[i].OutputName);
	//				test.stim.addStatus(test.gates[i].result);
	//				test.stim.addDelay(test.gates[i].timeStamp);
	//				if (test.gates[i].result == temporayStore)     //if the output didnt change, the corresponding change variable in the change vetor is set to false so we dont include it in sim file
	//				{
	//					test.stim.changed.push_back(false);
	//				}
	//				else
	//					test.stim.changed.push_back(true);

	//				if (((test.gates[i].timeStamp - test.gates[i].delay) < StoreTime) && (StoreTime != -1) && (test.gates[i].result != temporayStore))  //this is part of the second implementation, if the current
	//				{                                                                                                         //interferes with an old output thats still processing the old one will be deleted
	//					for (int l = test.stim.inputVariables.size() - 2; l >= 0; l--)
	//					{
	//						if (test.stim.inputVariables[l] == test.gates[i].OutputName)
	//						{
	//							//test.stim.changed[l] = false; //this is the implementation using the second way, remve the comment and it will work    
	//						}
	//					}
	//				}
	//				if (counter == test.gates[i].inputNames.size())
	//					break;

	//			}
	//		}

	//	}
	//}


	////test.PrintGates();




	//for (int i = 0; i < test.stim.Delay.size(); i++)  //part of the debugging process
	//{
	//	cout << test.stim.Delay[i] << ", " << test.stim.inputVariables[i] << ", " << test.stim.status[i] << endl;
	//}


	//ofstream fileOutput;
	//fileOutput.open(OutName);

	//for (int i = 0; i < test.stim.Delay.size(); i++)
	//{


	//	if (test.stim.changed[i])
	//	{
	//		if (i < test.stim.Delay.size() - 1)
	//		{                                                                                                //if there 2 outputs at exactly the same time, take the one that was evaluated later
	//			if ((test.stim.Delay[i] == test.stim.Delay[i + 1]) && (test.stim.inputVariables[i] == test.stim.inputVariables[i + 1]) && (test.stim.changed[i + 1]))
	//			{
	//				i++;
	//			}
	//		}


	//		string store = "";
	//		store += to_string(test.stim.Delay[i]) + ", " + test.stim.inputVariables[i] + ", " + to_string(test.stim.status[i]);
	//		fileOutput << store << endl;
	//	}
	//}


}

