#include <iostream>
#include "authlib.h"
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <unordered_set>
#include "openssl/sha.h"
#include <vector>
#include <ctime>

using namespace std;
bool auth = false;
string encrypter(const string str) {
	SHA256_CTX alg;
	stringstream text;
    SHA256_Init(&alg);
    SHA256_Update(&alg, str.c_str(), str.size());
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &alg);
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        text << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return text.str();
}
void fileChecker(string username, string password) {
	ifstream file("pwdb.txt");
	if (file.is_open()) {
    	string line;
    	while (getline(file, line)) {
            string t;
            istringstream in(line.c_str());
            vector<string> v;
            while (getline(in,t,':')) {
                v.push_back(t);
            }
    		if(v.at(0) == username ) {
                if(v.at(1) == password) {
                    auth = true;
                }
    		}   
   		}
    file.close();
    }
}

int main() {
    string line;
    ifstream timefile ("time.txt");
    getline (timefile,line);
    time_t timer2 = stoi(line);
    time_t waittime = time(nullptr)- timer2;
    if (waittime>300) {
        for (int i = 0; i<3; i++) {
            if (!auth) {
                string username, password; 
                //Creates variables to store user/password details
                cout << "Please enter the username: " << endl;
                cin >> username;
                cout << "Please enter the password: " << endl;
                cin >> password;
                fileChecker(username,encrypter(password));
                if (auth) authenticated(username);
                else rejected(username);
            }
            if (i==2 && !auth) {
                cout << "you have entered the wrong password three times and are now locked out of your account for 5 minutes.";
                ofstream timefile;
                timefile.open ("time.txt");
                time_t timer = time(nullptr);
                timefile << timer;
                timefile.close();
            }
        }
    }
}