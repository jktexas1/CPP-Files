//Jacob Kapusta
//SID: 1181827
//COSC 4330
//Assignment 1

#include<stdlib.h>
#include<iostream>
#include<sstream>
#include<string>
#include<list>
#include<queue>

using namespace std;

//Process Model.
class Process
{
private:
	int procID;
	int elapsed;
	string status;
public:
	void setID(int ID){procID = ID;}
	void setElapsedTime(int E){elapsed = E;}
	void setStatus(string s){status = s;}
	int getID(){return procID;}
	int getElapsedTime(){return elapsed;}
	string getStatus(){return status;}
};

//Instruction Model.
class Instruction{
private:
	string type;
	int startTime;
	int timeRequest;
	int elapsed;
	int processID;
	bool lastInstruction;
public:
	Instruction(){lastInstruction = false; elapsed = 0;}
	void setType(string t){type = t;}
	void setStartTime(int t){startTime = t;}
	void setTimeRequest(int t){timeRequest = t;}
	void setElapsedTime(int t){elapsed = t;}
	void setProcessID(int id){processID = id;}
	void setLastInstruction(bool b){lastInstruction = b;}
	void incrementElapsedTime(){elapsed += 1;}
	string getType(){return type;}
	int getStartTime(){return startTime;}
	int getTimeRequest(){return timeRequest;}
	int getElapsedTime(){return elapsed;}
	int getProcID(){return processID;}
	bool isLastInstruction(){return lastInstruction;}
};

//CPU -4 cores- Model.
class CPU{
//Four cores with booleans to track if idle.
private:
	Instruction core1;
	bool idle1;
	Instruction core2;
	bool idle2;
	Instruction core3;
	bool idle3;
	Instruction core4;
	bool idle4;
public:
	CPU(){
		idle1 = true;
		idle2 = true;
		idle3 = true;
		idle4 = true;
	}
	void setCore1(Instruction I){core1 = I;}
	void setCore2(Instruction I){core2 = I;}
	void setCore3(Instruction I){core3 = I;}
	void setCore4(Instruction I){core4 = I;}
	void setFirstCore(bool b){idle1 = b;}
	void setSecondCore(bool b){idle2 = b;}
	void setThirdCore(bool b){idle3 = b;}
	void setFourthCore(bool b){idle4 = b;}
	Instruction getCore1(){return core1;}
	Instruction getCore2(){return core2;}
	Instruction getCore3(){return core3;}
	Instruction getCore4(){return core4;}
	bool firstCoreIdle(){return idle1;}
	bool secondCoreIdle(){return idle2;}
	bool thirdCoreIdle(){return idle3;}
	bool fourthCoreIdle(){return idle4;}
};

//Disk Model.
class Disk
{
private:
	Instruction instr;
	bool idle;
public:
	Disk(){idle = true;}
	void setInstruction(Instruction I){instr = I;}
	void setIdle(bool b){idle = b;}
	Instruction getInstruction(){return instr;}
	bool isIdle(){return idle;}
};

//Input Device Model.
class InputDevice
{
private:
	Instruction instr;
	bool idle;
public:
	InputDevice(){idle = true;}
	void setInstruction(Instruction I){instr = I;}
	void setIdle(bool b){idle = b;}
	Instruction getInstruction(){return instr;}
	bool isIdle(){return idle;}
};

void gatherProceses(); //Function to start processes.
void sortInstructions(); //Function to sort instructions by start time.
void simulate(); //Function to simulate process executions.
int idleCore(CPU cpu); //Function to find an empty core.

list<Instruction> instructions;
CPU cpu;
int core;
Disk disk;
InputDevice inputdevice;
queue<Instruction> readyqueue; //Queue for CPU usage.
queue<Instruction> diskqueue; //Queue for disk usage.
queue<Instruction> devicequeue; //Queue for input device usage.
int noInstructions = 0;

//Main Function.
int main()
{	
	//Fetch instructions from text.
	gatherProceses();
	//Sort instructions by start time.
	sortInstructions();
	//Run simulation.
	simulate();
	//Terminate.
	return 0;
}

void gatherProceses()
{
	int i = 0; 
	int ID; 
	int procTotal; 
	string input; 
	string data[2]; 
	Instruction instr;
	while(getline(cin, input))
	{
		//Next lines create a process to enlist.
		stringstream str(input);
		while(str >> data[i]) i++;
		if(data[0].compare("NEW") == 0)
		{
			if(!instructions.empty()){
				instructions.back().setLastInstruction(true);
			}
			ID = atoi(data[1].c_str());
		}
		if(data[0].compare("START") == 0)
		{
			procTotal = atoi(data[1].c_str());
		}
		if(data[0].compare("CPU") == 0 || data[0].compare("I/O") == 0 || data[0].compare("INPUT") == 0)
		{
			if(!(data[0].compare("I/O") == 0 && atoi(data[1].c_str()) == 0))
			{
				instr.setType(data[0]);
				instr.setStartTime(procTotal);
				instr.setTimeRequest(atoi(data[1].c_str()));
				instr.setProcessID(ID);
				instr.setElapsedTime(0);
				instructions.push_back(instr);
				procTotal += atoi(data[1].c_str());
				noInstructions++;
			}
		}
		//Set iterator to 0 for parsing of next input into data array.
		i = 0;
	}
	instructions.back().setLastInstruction(true);
}
void sortInstructions()
{
	Instruction first, second, temp;
	for(list<Instruction>::iterator it=instructions.begin(); it != instructions.end(); ++it)
	{
		for(list<Instruction>::iterator it2 = it; it2 != instructions.end(); ++it2)
		{
			first = *it;
			second = *it2;
			if(second.getStartTime() < first.getStartTime())
			{
				temp = first;
				*it = second;
				*it2 = temp;
			}
		}
	}
}

void simulate()
{
	//Current time variable.
	int clock = 0;
	int instructionsExe = 0;
	int avgCPU = 0;
	
	Instruction instr; //Temporary instruction.
	
	
	//Simulation of processes
	int totalSimTime = 0;
	noInstructions = instructions.size();
	while(instructionsExe != noInstructions)
	{
		//Sends off a new instruction.
		if(!instructions.empty())
		{
			while(instructions.front().getStartTime() == totalSimTime)
			{
				//instr = instructions.front();
				//instructions.pop_front();
				//cout << "instr from list: " << instr.getType() << " " << instr.getStartTime() << " " << instr.getTimeRequest() << " simtime " << totalSimTime << "Last Instruction " << instr.isLastInstruction() << " procid " << instr.getProcID() << endl;
				if(instructions.front().getType() == "CPU")
				{
					core = idleCore(cpu);
					if(core == 1)
					{
						cpu.setCore1(instructions.front());
						//cout << "Core1 " << cpu.getCore1().getType() << " " << cpu.getCore1().getTimeRequest() << " lastIns " << cpu.getCore1().isLastInstruction() << endl;
						cpu.setFirstCore(false);
					}
					if(core == 2)
					{
						cpu.setCore2(instructions.front());
						//cout << "Core2 " << cpu.getCore2().getType() << " " << cpu.getCore2().getTimeRequest() << " lastIns " << cpu.getCore1().isLastInstruction() << endl;
						cpu.setSecondCore(false);
					}
					if(core == 3)
					{
						cpu.setCore3(instructions.front());
						//cout << "Core3 " << cpu.getCore3().getType() << " " << cpu.getCore3().getTimeRequest() << endl;
						cpu.setThirdCore(false);
					}
					if(core == 4)
					{
						cpu.setCore4(instructions.front());
						//cout << "Core4 " << cpu.getCore4().getType() << " " << cpu.getCore4().getTimeRequest() << endl;
						cpu.setFourthCore(false);
					}
					if(core == 5)
					{
						readyqueue.push(instructions.front());
						//cout << "All cores busy. sending to readyqueue" << endl;
					}
				}
				if(instructions.front().getType().compare("I/O") == 0)
				{
					if(disk.isIdle() == false)
					{
						diskqueue.push(instructions.front());
						//cout << "disk busy. sending to diskqueue" << endl;
					}
					else
					{
						disk.setInstruction(instructions.front());
						//cout << "Disk " << disk.getInstruction().getType() << " " << disk.getInstruction().getTimeRequest() << endl;
						disk.setIdle(false);
					}
				}
				if(instructions.front().getType().compare("INPUT") == 0)
				{
					if(inputdevice.isIdle() == false)
					{
						devicequeue.push(instructions.front());
						//cout << "inputdevice busy. sending to devicequeue" << endl;
					}
					else
					{
						inputdevice.setInstruction(instructions.front());
						//cout << "InputDevice " << inputdevice.getInstruction().getType() << " " << inputdevice.getInstruction().getTimeRequest() << endl;
						inputdevice.setIdle(false);
					}
				}
				instructions.pop_front();
			}	
		}
		
		//Handle current instructions on hardware items.
		if(cpu.firstCoreIdle() == false)
		{
			if(cpu.getCore1().getElapsedTime() == cpu.getCore1().getTimeRequest())
			{
				cpu.setFirstCore(true);
				instructionsExe++;
				cout << "Process cpu 1" << endl;
				if(cpu.getCore1().isLastInstruction() == true)
				{
					//Print results to console if last instruction of a process.
					cout << "Process " << cpu.getCore1().getProcID() << " finished." << endl;
					
				}
			}
		}
		if(cpu.secondCoreIdle() == false)
		{
			if(cpu.getCore2().getElapsedTime() == cpu.getCore2().getTimeRequest())
			{
				cpu.setSecondCore(true);
				instructionsExe++;
				cout << "Process cpu 2" << endl;
				if(cpu.getCore2().isLastInstruction() == true)
				{
					//Print results to console.
					cout << "Process " << cpu.getCore2().getProcID() << " finished." << endl;
					
				}
			}
		}
		if(cpu.thirdCoreIdle() == false)
		{
			if(cpu.getCore3().getElapsedTime() == cpu.getCore3().getTimeRequest())
			{
				cpu.setThirdCore(true);
				instructionsExe++;
				cout << "Process cpu 3" << endl;
				if(cpu.getCore3().isLastInstruction() == true)
				{
					//Print results to console.
					cout << "Process " << cpu.getCore3().getProcID() << " finished." << endl;
					
				}
			}
		}
		if(cpu.fourthCoreIdle() == false)
		{
			if(cpu.getCore4().getElapsedTime() == cpu.getCore4().getTimeRequest())
			{
				cpu.setFourthCore(true);
				instructionsExe++;
				cout << "Process cpu 4" << endl;
				if(cpu.getCore4().isLastInstruction() == true)
				{
					//Print results to console.
					cout << "Process " << cpu.getCore4().getProcID() << " finished." << endl;
					
				}
			}
		}
		if(disk.isIdle() == false)
		{
			if(disk.getInstruction().getElapsedTime() == disk.getInstruction().getTimeRequest() && disk.isIdle() == false)
			{
				disk.setIdle(true);
				instructionsExe++;
				cout << "Process I/O" << endl;
				if(disk.getInstruction().isLastInstruction() == true)
				{
					//Print results to console.
					cout << "Process " << disk.getInstruction().getProcID() << " finished." << endl;
					
				}
			}
		}
		if(inputdevice.isIdle() == false)
		{
			if(inputdevice.getInstruction().getElapsedTime() == inputdevice.getInstruction().getTimeRequest())
			{
				inputdevice.setIdle(true);
				instructionsExe++;
				cout << "Process input" << endl;
				if(inputdevice.getInstruction().isLastInstruction() == true)
				{
					//Print results to console.
					cout << "Process " << inputdevice.getInstruction().getProcID() << " finished." << endl;
					
				}
			}
		}
		
		//Check to see if instructions in queues can start.
		
		if(cpu.firstCoreIdle() == true)
		{
			if(!readyqueue.empty())
			{
				cpu.setCore1(readyqueue.front());
				readyqueue.pop();
				cpu.setFirstCore(false);
			}
		}
		if(cpu.secondCoreIdle() == true)
		{
			if(!readyqueue.empty())
			{
				cpu.setCore2(readyqueue.front());
				readyqueue.pop();
				cpu.setSecondCore(false);
			}
		}
		if(cpu.thirdCoreIdle() == true)
		{
			if(!readyqueue.empty())
			{
				cpu.setCore3(readyqueue.front());
				readyqueue.pop();
				cpu.setThirdCore(false);
			}
		}
		if(cpu.fourthCoreIdle() == true)
		{
			if(!readyqueue.empty())
			{
				cpu.setCore4(readyqueue.front());
				readyqueue.pop();
				cpu.setFourthCore(false);
			}
		}
		if(disk.isIdle() == true)
		{
			if(!diskqueue.empty())
			{
				disk.setInstruction(diskqueue.front());
				diskqueue.pop();
				disk.setIdle(false);
			}
		}
		if(inputdevice.isIdle() == true)
		{
			if(!devicequeue.empty())
			{
				inputdevice.setInstruction(devicequeue.front());
				devicequeue.pop();
				inputdevice.setIdle(false);
			}
		}
		
		//Increment elapsed time on current hardware instructions.
		
		if(cpu.firstCoreIdle() == false)
		{
			instr = cpu.getCore1();
			instr.setElapsedTime(instr.getElapsedTime() + 1);
			cpu.setCore1(instr);
		}
		if(cpu.secondCoreIdle() == false)
		{
			instr = cpu.getCore2();
			instr.setElapsedTime(instr.getElapsedTime() + 1);
			cpu.setCore2(instr);
		}
		if(cpu.thirdCoreIdle() == false)
		{
			instr = cpu.getCore3();
			instr.setElapsedTime(instr.getElapsedTime() + 1);
			cpu.setCore3(instr);
		}
		if(cpu.fourthCoreIdle() == false)
		{
			instr = cpu.getCore4();
			instr.setElapsedTime(instr.getElapsedTime() + 1);
			cpu.setCore4(instr);
		}
		if(disk.isIdle() == false)
		{
			instr = disk.getInstruction();
			instr.setElapsedTime(instr.getElapsedTime() + 1);
			disk.setInstruction(instr);
		}
		if(inputdevice.isIdle() == false)
		{
			instr = inputdevice.getInstruction();
			instr.setElapsedTime(instr.getElapsedTime() + 1);
			inputdevice.setInstruction(instr);
		}
		
		//1 MS passes.
		totalSimTime += 1;
	}
}

int idleCore(CPU cpu)
{
	if(cpu.firstCoreIdle() == true) return 1;
	else if(cpu.secondCoreIdle() == true) return 2;
	else if(cpu.thirdCoreIdle() == true) return 3;
	else if(cpu.fourthCoreIdle() == true) return 4;
	else return 5;
}