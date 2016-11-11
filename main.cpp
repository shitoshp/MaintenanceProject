#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

//function to take in each line of string and break it into two strings
vector<string> breakdown(string x);

struct node {
	string module;
	node* next;
};

int main() {
	
	string relation;
	string first;
	string second;
	bool end_of_data = false;
	vector<node*> links;
	vector<string> thisline;
	map <string, int> modules;

	ifstream instream ("input.txt");

	if (instream.is_open()) {
		while (getline(instream, relation)) {
			if (end_of_data == false) {
				
				//if * * is encountered it signifies the end of data
				if (relation == "* *") {
					end_of_data = true;
				}
				else {
					thisline = breakdown(relation);

					//if module is in not in map yet and appears in front, put into map with value 1
					if (modules.find(thisline[0]) == modules.end()) {
						modules[thisline[0]] = 1;
					}
					
					//if module appears in second, add into map with value 2
					modules[thisline[1]] = 2;
					
					cout << thisline[0] << " and " << thisline[1] << endl;
				}
			}
			else {
				cout << "End of data: " << relation << endl;
			}

		}
		instream.close();
	}

	//printing unique modules from map with value 2
	cout << "The unique modules are: ";
	for (map<string, int>::iterator it = modules.begin(); it != modules.end(); ++it) {
		if (it->second == 2) {
			cout << it->first << " ";
		}
	}
	cout << endl;

	//printing transactions from map with value 1
	cout << "The transactions are: ";
	for (map<string, int>::iterator it = modules.begin(); it != modules.end(); ++it) {
		if (it->second == 1) {
			cout << it->first << " ";
		}
	}
	cout << endl;

	system("pause");

	return 0;
}

vector<string> breakdown(string x) {
	string first = "";
	string second = "";
	bool space = false;
	vector<string> arr;
	int my_size = (int)x.size();
	for (int i = 0; i < my_size; i++) {
		if (space) {
			second += x[i];
		}
		
		if (x[i] == ' ') {
			space = true;
		}

		if (!space) {
			first += x[i];
		}

	}
	arr.push_back(first);
	arr.push_back(second);
	return arr;
}