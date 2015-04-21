//Jacob Kapusta SID:1181827
//Group 2 phase 2

/*Bijective Function: a function between the elements of two sets, where every element of one set is 
paired with exactly one element of the other set, and every element of the other set is paired with exactly one element of the first set.*/

//Problem: Edge{u, v} is in E IFF Edge{v, u} is in F

#include<iostream>
#include<fstream>
#include<string>
#include<list>

using namespace std;


bool bijective2 = true;

class Graphtype
{
private:
	int size;
	list<int> *graph;
public:
	Graphtype()
	{
		size = 0;
	}
	int getGraphSize()
	{
		return size;
	}
	bool isEmpty()
	{
		return (size == 0);
	}
	void createGraph(string file)
	{
		ifstream infile;
		string temp;

		int vertex;
		int adjacentVertex;

		if(size != 0) clearGraph();

		infile.open(file.c_str());
		
		if(!infile)
		{
			cout << "Cannot open input file." << endl;
			return;
		}
		
		infile >> size;
		graph = new list<int>[size];
		//cout << "Size: " << size << endl; 
		
		for(int i = 0; i<size; i++)
		{
			infile >> vertex;
			//cout << "Vertex: " << vertex << endl;
			infile >> adjacentVertex;
			
			while(adjacentVertex != -999)
			{
				//cout << "Adjacent Vertex: " << adjacentVertex << endl;
				graph[vertex].push_back(adjacentVertex);
				infile >> adjacentVertex;
			}
			
		}
		infile.close();
	}
	void clearGraph()
	{
		for(int i = 0; i<size; i++)
		{
			graph[i].clear();
		}
	}
	void getFront()
	{
		cout << "Graph[0] front is " << graph[0].front() << endl;
	}
	void printGraph()
	{
		for(int i=0; i<getGraphSize(); i++)
		{
			for(list<int>::iterator it=graph[i].begin(); it != graph[i].end(); ++it)
			{
				cout << ' ' << *it;
			}
			cout << endl;
		}
	}
	bool compareGraphs(Graphtype H)
	{
		bool bijective = true;
		for(int i=0; i<getGraphSize(); i++)
		{
			for(list<int>::iterator it=graph[i].begin(); it != graph[i].end(); ++it)
			{
				cout << "Inside G vertex " << i << " viewing AdjVertex " << *it << endl;
				bijective = H.search(*it, i);
				if(bijective == false)
				{
					return false;
				}
			}
		}
		return true;
	}
	void splitCompareGraphs(Graphtype H)
	{
		splitRecurse(H, getGraphSize()/2, 0, "Backwards");
		if(bijective2 == true)
			splitRecurse(H, getGraphSize()/2 + 1, getGraphSize() - 1, "Forwards");
	}
	void splitRecurse(Graphtype H, int start, int end, string direction)
	{
		if(start != end)
		{
			for(list<int>::iterator it=graph[start].begin(); it != graph[start].end(); ++it)
			{
				cout << "Inside G vertex " << start << " viewing AdjVertex " << *it << endl;
				bijective2 = H.search(*it, start);
				if(bijective2 == false)
				{
					return;
				}
				else
				{
					if(direction == "Backwards") splitRecurse(H, start-1, end, "Backwards");
					if(direction == "Forwards") splitRecurse(H, start+1, end, "Forwards");
				}
			}
		}
		if(start == end)
		{
			for(list<int>::iterator it=graph[start].begin(); it != graph[start].end(); ++it)
			{
				cout << "Inside G vertex " << start << " viewing AdjVertex " << *it << endl;
				bijective2 = H.search(*it, start);
			}
		}
	}
	void splitCompareGraphsTwo(Graphtype H, int start1, int start2)
	{
		for(list<int>::iterator it=graph[start1].begin(); it != graph[start1].end(); ++it)
		{
				cout << "Inside G vertex " << start1 << " viewing AdjVertex " << *it << endl;
				bijective2 = H.search(*it, start1);
		}
		if(bijective2 == false) return;
		
		else
		{
			splitRecurseTwo(H, H.getGraphSize()/2, 0, H.getGraphSize()/2 + 1, H.getGraphSize() - 1, "Forwards", "notcomplete", "notcomplete");
		}
		
		for(list<int>::iterator it=graph[0].begin(); it != graph[0].end(); ++it)
		{
				cout << "Inside G vertex " << 0 << " viewing AdjVertex " << *it << endl;
				bijective2 = H.search(*it, 0);
		}
	}
	
	void splitRecurseTwo(Graphtype H, int start1, int end1, int start2, int end2, string direction, string status1, string status2)
	{
		if(direction == "Forwards" && status2 == "notcomplete")
		{
			if(start2 != end2)
			{
				for(list<int>::iterator it=graph[start2].begin(); it != graph[start2].end(); ++it)
				{
					cout << "Inside G vertex " << start2 << " viewing AdjVertex " << *it << endl;
					bijective2 = H.search(*it, start2);
					if(bijective2 == false)
					{
						return;
					}
					else
					{
						splitRecurseTwo(H, start1 - 1, end1, start2, end2, "Backwards", "notcomplete", "notcomplete");
					}
				}
			}
			if(start2 == end2)
			{
				for(list<int>::iterator it=graph[start2].begin(); it != graph[start2].end(); ++it)
				{
						cout << "Inside G vertex " << start2 << " viewing AdjVertex " << *it << endl;
						bijective2 = H.search(*it, start2);
				}
				if(status2 != "complete")
				{
					splitRecurseTwo(H, start1, end1, start2 + 1, end2, "Forwards", "notcomplete", "complete");
				}
			}
		}
		
		if(direction == "Backwards" && status1 == "notcomplete")
		{
			if(start1 != end1)
			{
				for(list<int>::iterator it=graph[start1].begin(); it != graph[start1].end(); ++it)
				{
					cout << "Inside G vertex " << start1 << " viewing AdjVertex " << *it << endl;
					bijective2 = H.search(*it, start1);
					if(bijective2 == false)
					{
						return;
					}
					else
					{
						splitRecurseTwo(H, start1, end1, start2 + 1, end2, "Forwards", "notcomplete", "notcomplete");
					}
				}
			}
			if(start1 == end1)
			{
				for(list<int>::iterator it=graph[start1].begin(); it != graph[start1].end(); ++it)
				{
						cout << "Inside G vertex " << start1 << " viewing AdjVertex " << *it << endl;
						bijective2 = H.search(*it, start1);
				}
				if(status2 != "complete")
				{
					splitRecurseTwo(H, start1, end1, start2 + 1, end2, "Forwards", "notcomplete", "complete");
				}
			}
		}
	}
	
	bool search(int vert, int adj)
	{
		int j = -999;
		cout << "Inside H vertex " << vert << " Searching for AdjVertex " << adj << endl;
		for(list<int>::iterator it=graph[vert].begin(); it != graph[vert].end(); ++it)
		{
			if(*it == adj)
			{
				cout << "Found" << endl;
				j = *it;
			}
		}
		if(j == -999) return false;
		else return true;
	}
};


int main()
{
	Graphtype G;
	Graphtype H;
	G.createGraph("input1_5.txt");
	H.createGraph("input2_5.txt");

	bool exists = true;

	string choice = "split";
	
	if(choice == "split")
	{
		G.splitCompareGraphsTwo(H, H.getGraphSize()/2, H.getGraphSize()/2 + 1);
		if(bijective2 == true) cout << "Bijective Function Exists." << endl;
		else if(bijective2 == false) cout << "No Bijective Function Found." << endl;
	}
	if(choice == "Bruteforce")
	{
		exists = G.compareGraphs(H);
		if(exists == true) cout << "Bijective Function Exists." << endl;
		else if(exists == false) cout << "No Bijective Function Found." << endl;
	}
	return 0;
}