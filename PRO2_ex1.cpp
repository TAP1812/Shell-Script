#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unistd.h> // Required for sleep()

using namespace std;

const string USERS_FILE_PATH = "/etc/passwd";
const string GROUPS_FILE_PATH = "/etc/group"; // Corrected typo

vector<string> splitString(const string& s, char delimiter = ':') {
    vector<string> res;
    stringstream ss(s);
    string word;
    while (getline(ss, word, delimiter)) {
        res.push_back(word);
    }
    return res;
}

int main() {
    bool isFound = false;

    // Get username
    cout << "Enter your username:" << endl;
    cout << "Checking ......" << endl;
    sleep(1); // Reduced sleep time. 5 seconds is excessive

    string username;
    cin >> username;

    // Open file /etc/passwd to find username
    ifstream usersFile(USERS_FILE_PATH);
    if (!usersFile.is_open()) {
        cerr << "Error opening " << USERS_FILE_PATH << endl;
        return 1; // Indicate an error
    }

    string line;
    while (getline(usersFile, line)) {
        vector<string> userInfo = splitString(line);
        if (!userInfo.empty() && username == userInfo[0]) {
            cout << "User: " << username << endl;
            cout << "Id: " << userInfo[2] << endl;
            cout << "Home directory: " << userInfo[5] << endl;
            cout << "Groups: ";
            usersFile.close();
            isFound = true;
            break;
        }
    }
    usersFile.close();


    if (!isFound) {
        cout << "Not Found!!" << endl;
        return 0; // Exit early if user not found in passwd
    }

    // Open file /etc/group to find groups
    ifstream groupsFile(GROUPS_FILE_PATH);
    if (!groupsFile.is_open()) {
        cerr << "Error opening " << GROUPS_FILE_PATH << endl;
        return 1; // Indicate an error
    }

    vector<string> groups; // Store found groups
    while (getline(groupsFile, line)) {
        vector<string> groupInfo = splitString(line);
        if (groupInfo.size() > 3) { // Check if the vector has at least 4 elements
            vector<string> users = splitString(groupInfo[3], ',');
            if (find(users.begin(), users.end(), username) != users.end()) {
                groups.push_back(groupInfo[0]); // Store group name
            }
        }
    }
    groupsFile.close();

    // Print the groups
    for (size_t i = 0; i < groups.size(); ++i) {
        cout << groups[i];
        if (i < groups.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;


    return 0;
}
