#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
using namespace std;

vector<string> all_nodes;                       //vector containing all nodes
map <string, vector<string>> submodules;        //map where key is a node and value is a vector of nodes coming out of it

//function to take in each line of string and break it into two strings
vector<string> breakdown(string x);

//function to get all paths from transaction to defective module
void get_all_paths(string transaction, string defective, map<string, vector<string> > module_tree, vector<string> nodes);

//recursive helper function for get_all_paths
void get_all_paths_helper(string u, string d, map<string, bool> visited, string* path, int &path_index, map<string, vector<string> > module_tree);

//function to get explosion
void explosion(string transaction, map<string, vector<string> > module_tree, vector<string> nodes);

int main() {

    string relation;
    string first;
    string second;
    string transaction;
    string def_module;
    bool def_module_check = false;
    bool end_of_data = false;
    //vector<node*> links;
    vector<string> thisline;

    map <string, int> modules;

    vector<vector<string>> path;

    ifstream instream("input.txt");

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

                    submodules[thisline[0]].push_back(thisline[1]);


                    //cout << thisline[0] << " and " << thisline[1] << endl;
                }
            }
            else {
                //cout << "End of data: " << relation << endl;
                if (!def_module_check) {
                    def_module = relation;
                    def_module_check = true;
                }
                else {
                    transaction = relation;
                }
            }

        }
        instream.close();
    }

    //printing the user interface

    cout << "USER INTERFACE" << endl;
    cout << endl;
    for (map<string, vector<string>>::iterator it = submodules.begin(); it != submodules.end(); ++it) {
        cout << it->first << ": ";
        for (vector<string>::iterator vit = it->second.begin(); vit != it->second.end(); vit++) {
            cout << *vit << " ";
        }
        cout << endl;
    }
    cout << endl;

    //printing unique modules from map with value 2, they are the unique modules
    cout << "The unique modules are: ";
    for (map<string, int>::iterator it = modules.begin(); it != modules.end(); ++it) {
        if (it->second == 2) {
            all_nodes.push_back(it->first);
            cout << it->first << " ";
        }
    }
    cout << endl;

    //printing transactions from map with value 1, they are the transactions
    cout << "The transactions are: ";
    for (map<string, int>::iterator it = modules.begin(); it != modules.end(); ++it) {
        if (it->second == 1) {
            all_nodes.push_back(it->first);
            cout << it->first << " ";
        }
    }
    cout << endl;
    cout << endl;

    cout << "The given transaction is: " << transaction << endl;
    cout << "The defective module is: " << def_module << endl;

    cout << endl;


    cout << endl;




    cout << "Paths from given transaction to defective moudle:" << endl;
    get_all_paths(transaction, def_module, submodules, all_nodes); //printing all paths
    cout << endl;

    cout << "Explosion:" << endl;
    explosion(transaction, submodules, all_nodes); //printing the explosion
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

//Implementing graph theory's breadth first search to get all paths
void get_all_paths(string transaction, string module, map<string, vector<string> > module_tree, vector<string> nodes) {
    
    // Mark all the vertices as not visited
    map<string, bool> visited;

    // this string array stores all paths
    string* path = new string[nodes.size()];

    //used at the helper function to keep track of path
    int path_index = 0;
    
    // Call recursive helper function to print all paths
    get_all_paths_helper(transaction, module, visited, path, path_index, module_tree);
}

//helper function for print all paths
void get_all_paths_helper(string h_transaction, string h_module, map<string, bool> visited, string* path, int &path_index, map<string, vector<string> > node_map) {
    
    // Mark the current node and store it in path array
    visited[h_transaction] = true;
    path[path_index] = h_transaction;
    path_index++;

    // if you reach the module, print path
    if (h_transaction == h_module)
    {
        for (int i = 0; i < path_index; i++)
            cout << path[i] << " ";
        cout << endl;
    }
    // If current vertex is not destination
    else 
    {
        // look for adjacent vertices
        vector<string>::iterator i;
        for (i = node_map[h_transaction].begin(); i != node_map[h_transaction].end(); ++i)
            if (!visited[*i])
                get_all_paths_helper(*i, h_module, visited, path, path_index, node_map);
    }

    // Remove current vertex from path[] and mark it as unvisited
    path_index--;
    visited[h_transaction] = false;
}


void explosion(string transaction, map<string, vector<string> > node_map, vector<string> nodes) {
    // Mark all the vertices as not visited
    vector<string> visited;
    stack<map<string, int> > stack_modules;

   map<string, int> root;
    root[transaction] = 0;
    stack_modules.push(root);

    //creating a stack to store the modules to print 
    while (!stack_modules.empty()) {
        map<string, int> module = stack_modules.top();
        stack_modules.pop();
        string vertex;
        int space = 0;
        for (auto& x : module) {
            vertex = x.first;
            space = x.second;
        }
        // Print enough spaces before module
        for (int i = 0; i < space; i++) {
            cout << ' ';
        }
        cout << vertex << endl;

        if (find(visited.begin(), visited.end(), vertex) == visited.end()) {
            // if vertex not in visited
            visited.push_back(vertex);

            map<string, vector<string> >::const_iterator has_key = node_map.find(vertex);
            // If vertex is in node map
            if (has_key != node_map.end()) {
                for (auto it = node_map[vertex].rbegin(); it != node_map[vertex].rend(); ++it)
                {
                    if (find(visited.begin(), visited.end(), *it) == visited.end()) {
                        // if vertex not in visited
                        map<string, int> module;
                        module[*it] = space + 1;
                        stack_modules.push(module);
                    }
                    else {
                        // Printing session
                        for (int i = 0; i < space + 1; i++) {
                            cout << ' ';
                        }
                        cout << *it << '*' << endl;
                    }

                }
            }
        }
    }


}