//////////////////////////////////////////////////////////////
//
// Your #includes here; make sure you are allowed them ...
//
#include <fstream>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
using namespace std;
//////////////////////////////////////////////////////////////
//
// local #includes and function declarations; do not remove
//

#ifndef MARMOSET_TESTING
#include <iostream>
#endif

//////////////////////////////////////////////////////////////
//
// struct definitions
//
#ifndef MARMOSET_TESTING
struct Student{
	int studentID;
	int grade;
};

struct Dataset{
	int numStudents;
	Student* students;
};

struct Rejects{
	int numRejects;
	int* rejects;
};

struct Mode{
	int numModes;
	int* modes;
};

struct Statistics{
	int   minimum;
	float average;
	int   maximum;
	float popStdDev;
	float smplStdDev;
	Mode  mode;
	int   histogram[10];
};
#endif

//////////////////////////////////////////////////////////////
//
// Function declarations; do not remove
//

char * getProperFileNameCSV(const char filename[]){
	int filenamelen = 0;
	for (int i = 0; filename[i] != 0; i++){
		//if (filename[i] == '.' && filename[i+1] != 'c')
			//return NULL; if u uncomment this make the thing under else if
		if (filename[i] == '.'){
			filenamelen = i;
			break;
		}
		else if (filename[i+1] == 0){
			filenamelen = i + 1;
			break;
		}
	}

	char *ProperFileName = new char[filenamelen + 5];
	for (int i = 0; i < filenamelen; i++){
		ProperFileName[i] = filename[i];
	}
	ProperFileName[filenamelen] = '.';
	ProperFileName[filenamelen + 1] = 'c';
	ProperFileName[filenamelen + 2] = 's';
	ProperFileName[filenamelen + 3] = 'v';
	ProperFileName[filenamelen + 4] = '\0';

	return ProperFileName;
}

char * getProperFileName(const char filename[]){
	int filenamelen = 0;
	for (int i = 0; filename[i] != 0; i ++){
		//if (filename[i] == '.' && filename[i+1] != 's')
			//return NULL;
		if (filename[i] == '.'){
			filenamelen = i ;
			break;
		}
		else if (filename[i+1] == 0){
			filenamelen = i + 1;
			break;
		}
	}

	char *ProperFileName = new char[filenamelen + 6];
	for (int i = 0; i < filenamelen; i++){
		ProperFileName[i] = filename[i];
	}
	ProperFileName[filenamelen] = '.';
	ProperFileName[filenamelen + 1] = 's';
	ProperFileName[filenamelen + 2] = 't';
	ProperFileName[filenamelen + 3] = 'a';
	ProperFileName[filenamelen + 4] = 't';
	ProperFileName[filenamelen + 5] = '\0';

	return ProperFileName;
}



int mode(const int dataset[], const int size, Statistics& stats){
  if (size == 0){
    return -1;
  }
	stats.mode.modes = new int[100];
  int mostFreq = 1, counter = 1, modez = 0;
  stats.mode.modes[modez] = dataset[0];
  for (int i = 1; i <= size; i++){
    {
      if (dataset[i-1] == dataset[i]){
        counter++;
      }
      else{
        counter = 1;
      }
      if (counter > mostFreq){
        mostFreq = counter;
        modez=0;
        for (int i = 0; i < size - 1; i++){
          stats.mode.modes[i] = 0;
        }
        stats.mode.modes[modez] = dataset[i];
      }
      else if (counter == mostFreq){
        modez++;
        stats.mode.modes[modez] = dataset[i];
      }
    }
  }
	stats.mode.numModes = modez+1;
	return 0;
}

void quicksort(int arr[], int left, int right) {
      int i = left, j = right;
      int tmp;
      int pivot = arr[(left + right) / 2];
      while (i <= j) {
            while (arr[i] < pivot)
                  i++;
            while (arr[j] > pivot)
                  j--;
            if (i <= j) {
                  tmp = arr[i];
                  arr[i] = arr[j];
                  arr[j] = tmp;
                  i++;
                  j--;
            }
      };
      if (left < j)
            quicksort(arr, left, j);
      if (i < right)
            quicksort(arr, i, right);
}

int readCSV(const char filename[],const int minAcceptableID,
				const int maxAcceptableID,Dataset& data,
				Rejects& rejects);

int computeStatistics(Dataset& data, Statistics& stats);

int writeCSV(const char filename[], const Statistics& stats);

//////////////////////////////////////////////////////////////
//
// Function definitions. PLEASE USE HELPER FUNCTIONS
//
int readCSV(const char filename[],const int minAcceptableID,
				const int maxAcceptableID,Dataset& data,
				Rejects& rejects)
				{
					if (minAcceptableID>maxAcceptableID) return -1;

					data.students = new Student[10000];
					for (int i = 0; i <= 10000; i++){
						data.students[i].studentID = -2;
						data.students[i].grade = -2;
					}
					//Student* data.students = new Student[100];

					bool done = false;
					int fileLineNumber = 0;
					const int maxLineLength = 100;
					char line[maxLineLength];

					ifstream infile;
					infile.open(getProperFileNameCSV(filename));

					if (!infile.is_open()){
						return -1;
					}

					int numberOfRejects = 0;
					int rejectSize = 10;
					rejects.rejects = new int[rejectSize];
					for (int i = 0; i < rejectSize; i++){
						rejects.rejects[i] = 0;
					}

					while (!done){
						int tempID = 0;
						int tempGrade = 0;
				    ++fileLineNumber;
				    if (!infile.getline(line, maxLineLength)){
				      if (infile.eof()){
				        done = true;
				      }
				      else{
				        return -1;
				      }
				    }
						bool rejectBool = false;
						bool spaceBool = false;
						bool c = false;
						int i = 0;
						while (line[i] !=0 && line[i]!= '\r' && !rejectBool){
							if (line[0] == ','){
								rejects.rejects[numberOfRejects++] = fileLineNumber;
								if (numberOfRejects >= rejectSize) return -1;
								rejectBool = true;
							}
				     // if (rejectBool) break;
				      // if its whitespace:
				      if (line[i]==' ' || line[i]=='\t' && !rejectBool)
							{
								if (spaceBool && line[i+1] <='9' && line[i+1]>='0')
								{
									rejects.rejects[numberOfRejects++] = fileLineNumber;
									if (numberOfRejects >= rejectSize) return -1;
									rejectBool = true;
								}
							}
				      // if its a number:
				      else if (line[i]<='9' && line[i]>='0' && !rejectBool)
				      {
								if (!c)
			          {
									spaceBool = true;
			            // read studentID
			            while (line[i] <= '9' && line[i] >= '0' && (!rejectBool))
			            {
										tempID = 10*tempID + (line[i] - '0');
										i++;
									}
									i--;
									if (tempID < minAcceptableID || tempID > maxAcceptableID){
										rejects.rejects[numberOfRejects++] = fileLineNumber;
										if (numberOfRejects >= rejectSize) return -1;
										rejectBool = true;
									}
									else if (!rejectBool){
										data.students[fileLineNumber-1].studentID = tempID;
									}
								}
								else if (c && !rejectBool)
								{
									spaceBool = true;
									if (line[i] == ','){
										rejects.rejects[numberOfRejects++] = fileLineNumber;
										if (numberOfRejects >= rejectSize) return -1;
										rejectBool = true;
										}
									while (line[i]<= '9' && line[i] >= '0')
		              {
										tempGrade = (10*tempGrade + (line[i] - '0'));
										i++;
									}
									i--;
									if (tempGrade < 0 || tempGrade > 100){
										rejects.rejects[numberOfRejects++] = fileLineNumber;
										if (numberOfRejects >= rejectSize) return -1;
										rejectBool = true;
									}
									else if (!rejectBool){
										data.students[fileLineNumber-1].grade = tempGrade;
									}
								}
							}
							else if (line[i] == ',' && !rejectBool){
								if (c) {
									rejects.rejects[numberOfRejects++] = fileLineNumber;
									if (numberOfRejects >= rejectSize) return -1;
									rejectBool = true;
								}
								c = true;
								spaceBool = false;
							}
							else
							{
								rejects.rejects[numberOfRejects++] = fileLineNumber;
								if (numberOfRejects >= rejectSize) return -1;
								rejectBool = true;
							}
							if (line[i+1] == '\r' || line[i+1] == 0 && !rejectBool){
								if (data.students[fileLineNumber-1].studentID == -2 || data.students[fileLineNumber-1].grade == -2){
									rejects.rejects[numberOfRejects++] = fileLineNumber;
									if (numberOfRejects >= rejectSize) return -1;
									rejectBool = true;
								}
							}
							if (rejectBool){
								data.students[fileLineNumber-1].grade = -1;
								data.students[fileLineNumber-1].studentID = -1;
								break;
							}
							i++;
						}
					}
					rejects.numRejects = numberOfRejects;

					data.numStudents = fileLineNumber - numberOfRejects -1;

					if (numberOfRejects == 0){
						return 0;
					}

					else return numberOfRejects;
}

int computeStatistics(Dataset& data, Statistics& stats){
	stats.average=0;
	for (int i = 0; i < 10; i++){
		stats.histogram[i] = 0;
	}

	float sum = 0;
	float std = 0;
	int sortedArray[data.numStudents];
	int count = 0;
	for (int i = 0; i <= data.numStudents; i++){
		if (data.students[i].grade >= 0){
		sortedArray[count] = data.students[i].grade;
		}
		else count--;
		count++;
	}
	quicksort(sortedArray, 0, data.numStudents-1);
	stats.minimum = sortedArray[0];
	stats.maximum = sortedArray[data.numStudents-1];
	for (int i = 0; i < data.numStudents; i++){
		sum += sortedArray[i];
	}
	float avg = sum/(float)data.numStudents;
	stats.average = avg;
	// std::cout << "Minimum: " << sum/data.numStudents << std::endl;

	for (int k = 0; k < data.numStudents; k++){
    std += pow(sortedArray[k]-stats.average, 2);
  }
	stats.popStdDev = sqrt(std/data.numStudents);
	stats.smplStdDev = sqrt(std/(data.numStudents-1));
	mode(sortedArray, data.numStudents-1, stats);
	for (int i = 0; i < data.numStudents; i++){
		int grade = sortedArray[i];
		if (grade>=0 && grade <=9)
			stats.histogram[0]++;
		else if (grade>=10 && grade<=19)
			stats.histogram[1]++;
		else if (grade>=20 && grade<=29)
			stats.histogram[2]++;
		else if (grade>=30 && grade<=39)
			stats.histogram[3]++;
		else if (grade>=40 && grade<=49)
			stats.histogram[4]++;
		else if (grade>=50 && grade<=59)
			stats.histogram[5]++;
		else if (grade>=60 && grade<=69)
			stats.histogram[6]++;
		else if (grade>=70 && grade<=79)
			stats.histogram[7]++;
		else if (grade>=80 && grade<=89)
			stats.histogram[8]++;
		else if (grade>=90 && grade<=100)
			stats.histogram[9]++;
	}
	return 0;
}

int writeCSV(const char filename[], const Statistics& stats){


	ofstream outfile; // declare the file object
	outfile.open(getProperFileName(filename)); // open the file
	if (!outfile.is_open()) return -1; // Unable to open file
	//for (int i = 0; i ) HAS TO CHECK FILE EXTENSION

	outfile << "Minimum: " << stats.minimum << endl << "Average: " << stats.average << endl;
	outfile << "Maximum: " << stats.maximum << endl << "Population Standard Deviation: " << stats.popStdDev << endl;
	outfile << "Sample Standard Deviation: " << stats.smplStdDev << endl << "Modes: ";
	for (int i = 0; i < stats.mode.numModes; i++){
		outfile << stats.mode.modes[i];
		if (i +1< stats.mode.numModes){
			outfile << ", ";
		}
	}
	outfile << endl << "Histogram:" << endl << "[0-9]: " << stats.histogram[0] << endl << "[10-19]: " << stats.histogram[1] << endl;
	outfile << "[20-29]: " << stats.histogram[2] << endl << "[30-39]: " << stats.histogram[3] << endl << "[40-49]: " << stats.histogram[4] << endl;
	outfile << "[50-59]: " << stats.histogram[5] << endl << "[60-69]: " << stats.histogram[6] << endl << "[70-79]: " << stats.histogram[7] << endl;
	outfile << "[80-89]: " << stats.histogram[8] << endl << "[90-100]: " << stats.histogram[9];
	outfile.close();
		return 0;
}


//////////////////////////////////////////////////////////////
//
// Test Driver
//
// Do not remove the #ifndef and its associated #endif
//

#ifndef MARMOSET_TESTING

#define isNaN(X) (X != X)  // NaN is the only float that is not equal to itself

int main(const int argc, const char* const argv[]) {

	// Some test driver code here ....
	int minAcceptableID = 0;
	int maxAcceptableID = 2000000;
	/* Sample data:
	10, 20
	12123, 40
	1231, 93
	123123, 50
	-123, 64
	                          49492, 60
	100000000000, 600
	100, -12 */

	Dataset data = {0};
	Rejects rejects = {0};
	Statistics stats = {0};

	std::cout << std::endl << "Implement some more appropriate tests in main()" << std::endl << std::endl;
int read= readCSV("Sample.blahblahblah", minAcceptableID, maxAcceptableID, data, rejects);
	if( read< 0)
	{
		std::cout << ".csv file could not be read" << std::endl;
	}
	std::cout<<read<<" Read"<<std::endl;
	for(int i =0;i<read;i++){
		std::cout<<rejects.rejects[i]<<std::endl;
	}
	if (computeStatistics(data, stats) < 0)
	{
		std::cout << "Stats could not be computed" << std::endl;
	}
	std::cout<<"STATS DONE" <<std::endl;
	if (writeCSV("Sample.stat", stats) < 0)
	{
		std::cout << ".stat file could not be written" << std::endl;
	}
std::cout<<"WRITE DONE" <<std::endl;
	std::cout << "Average: " << stats.average << std::endl;
	std::cout << "Minimum: " << stats.minimum << std::endl;
	std::cout << "Maximum: " << stats.maximum << std::endl;
	std::cout << "Population standard deviation: " << stats.popStdDev << std::endl;
	std::cout << "Sample standard deviation: " << stats.smplStdDev << std::endl;


	for (unsigned char i = 0; i < stats.mode.numModes; i++)
	{
		std::cout << "Mode: " << stats.mode.modes[i] << std::endl;
	}

	for (unsigned char i = 0; i < 10; i++)
	{
		std::cout << "Histogram bin [" << (i*10) << ", " << ((((i+1)*10) - 1) + i/9) << "]: " << stats.histogram[i] << std::endl;
	}


	return 0;
}

#endif
