#include<stdio.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cstring>
#include<string>
using namespace std;

class CommandLineParser
{
private:
	char *input;
	char delimiter;
	char* data;  // to store input data
	int inputSize; //size of input data 

public:
        char* extract(char*); // Method to extract the variable value
        CommandLineParser(char*,char); //constructor
	 void read_inputData(char* fileName); // to read input data from file
};

CommandLineParser::CommandLineParser(char *argument,char delimit)
{
	input=argument;
	delimiter=delimit;
}

void CommandLineParser::read_inputData(char* fileName)
{
      ifstream input;
      input.open(fileName);

      // Get the input file size
	long begin = input.tellg();
	input.seekg(0,ios::end);
	long end = input.tellg();
	input.seekg(0,ios::beg);
	long size = end - begin;

	data = new char[size];
	
	int count = 0;
	while(input.get(data[count++])); // Store each character in the array and do error checking
	data[count-1] = '\n'; // Pad the input to help check validity

	inputSize = count;

       input.close();
}

char* CommandLineParser::extract(char *variableName)
{
       bool variableFound = false;
	bool variableNameFound = true;
	char *variable = new char[50]; // Char array to hold the value of the variable
	char *temp = new char[50]; // Temp array to hold a partial string to find the variable name
	int variableLength; // Length of the variable name
	int variableCount; // Counter for the variable array
	int inputLength; // Length of the command line argument
	int equalCount; // Index to find '='

	for(inputLength = 0; input[inputLength] != '\0'; inputLength++); // Get the length of the command line argument
	for(variableLength = 0; variableName[variableLength] != '\0'; variableLength++); // Get the length of the variable

	// Loop through the command line argument and find the variable name and get the variable value
	for(int i = 0; (input[i] != '\0' || input[i] != delimiter) && !variableFound && i < inputLength - variableLength + 1; i+=equalCount) {
		// Clear the temp array
		for(int j = 0; j < 50; j++) {
			temp[j] = '\0';
		}

		// Find the variable name
		for(equalCount = 0; equalCount < variableLength && variableNameFound; equalCount++) {
			temp[equalCount] = input[i+equalCount];
			
			// If the equalCount'th element of temp and variable name are not equal, we did not find the variable and we move on
			if(temp[equalCount] != variableName[equalCount]) {
				variableNameFound = false;
			}
		}

		// If we have a variable name match, we check the next char to see if it is '=', otherwise we have not found the variable
		if(variableNameFound && input[i+equalCount] != '=') {
			variableNameFound = false;
		}

		// If we found the variable name followed by '=', we get the value of the variable
		if(variableNameFound) {
			// Loop through the input to get the variable value
			for(variableCount = 0; input[i+equalCount+1+variableCount] != '\0' && input[i+equalCount+1+variableCount] != delimiter; variableCount++) {
				variable[variableCount] = input[i+equalCount+1+variableCount];
			}
			variable[variableCount] = '\0'; // Set the end line flag

			return variable;
		} else {
			variableNameFound = true; // Reset the variable name found flag to restart the search
		}
	}

	return NULL; // Return NULL if the variable name is not in the command line argument
}

void add(double a[110][110], double b[110][110], double c[110][110], int r, int col, int i, int j);

int main(int argc, char* argv[])
{
	double matrix1[110][110];
	double matrix2[110][110];
	double matrix3[110][110];
	string a[3];
	string b[3];
	int r, c, val;
	int rows1=0, rows2=0, columns1=0, columns2=0, i=0, j=0;
	string temp;
	string temp2;

	CommandLineParser clp(argv[1],';');

	ifstream textfile1(clp.extract("A"));
	ifstream textfile2(clp.extract("B"));
	ofstream textfile3(clp.extract("result"));
	string operation(clp.extract("operation"));

	for(int i=0; i<110; i++)
	{
		for(int j=0; j<110; j++)
		{
			matrix1[i][j]=0.0;
			matrix2[i][j]=0.0;
			matrix3[i][j]=0.0;
		}
	}

	//Opens file1 and inputs into matrix1, also skips bad lines.
	while(!textfile1.eof())
	{
		while(getline(textfile1, temp))
		{
			if(temp.find_first_not_of("0123456789")== std::string::npos)
			{
				continue;
			}
			else
			{
				break;
			}
		}
		
		stringstream str(temp);
		while(str >> a[i])
		{
			i++;
		}
		r = atoi(a[0].c_str());
		c = atoi(a[1].c_str());
		val = atof(a[2].c_str());
		matrix1[r][c] = val;
		if(r>=rows1) rows1=r;
		if(c>=columns1) columns1=c;
		i=0;
	}
	textfile1.close();
	
	//opens file2 and inputs into matrix2; also skips bad lines.
	while(!textfile2.eof())
	{
		
		while(getline(textfile2, temp2))
		{
			if(temp.find_first_not_of("0123456789")== std::string::npos)
			{
				continue;
			}
			else
			{
				break;
			}
		}
		
		stringstream str2(temp2);
		while(str2 >> b[i])
		{
			i++;
		}
		r = atoi(b[0].c_str());
		c = atoi(b[1].c_str());
		val = atof(b[2].c_str());
		matrix2[r][c] = val;
		i=0;
		if(r>=rows2) rows2=r;
		if(c>=columns2) columns2=c;
	}
	textfile2.close();
	
	if(rows1!=rows2 && columns1!=columns2)
	{
		cout << "The matrices are incompatible for addition.\n";
		system("pause");
		return 0;
	}
	
	rows1+=1;
	columns1+=1;
	i=1, j=1;

	add(matrix1, matrix2, matrix3, rows1, columns1, i, j);

	//Writes result matrix to file.
	for(int i=1; i<rows1; i++)
	{
		for(int j=1; j<columns1; j++)
		{
			textfile3 << i << " " << j << " " << matrix3[i][j] << endl;
		}
	}
	textfile3.close();

	cout << "Results stored in result file.\n";

	system("pause");
	return 0;
}

void add(double a[110][110], double b[110][110], double c[110][110], int r, int col, int i, int j)
{
	c[i][j]=a[i][j]+b[i][j];
	if(j<col && i<r)
	{
		if(j == col-1)
		{
			j=0;
			i = i+1;
		}
		add(a, b, c, r, col, i, j+1);
	}
}
	
