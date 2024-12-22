#include <iostream>
#include <openssl/sha.h>

#define TRANSACTIONS "transactions.csv"
#define USERS "users.csv"
string gUserName;

using namespace std;

struct UserInfo
{
	string username;
	string password;
};

struct ExpenceInfo
{
	string username;
	string type;
	double amount;
	string description;
};
class CSVFunction
{
public:
	void createFile();
	void readFile();
	void saveData(ExpenceInfo);
	void updateData(ExpenceInfo);
	void deleteDocument(ExpenceInfo);
	bool registerUser(UserInfo);
	bool loginUser(UserInfo);
	string hashPassword(string);
};
class Expence
{
	int salary;
	string username;
	string password;

public:
	void login();
	void saveToFile();
	void readFromFile();
	void display();
	void calculateExpence();
};

string CSVFunction::hashPassword(string password)
{
	unsigned char hash[300];
	SHA256((unsigned char *)password.c_str(), password.size(), hash);
	stringstream ss;
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		ss << hex << setw(2) << setfill('0') << (int)hash[i];
	}

	retrun ss.str();
}

bool CSVFunction::registerUser(UserInfo userInfo)
{
	ifstream file(USERS);
	if (!file)
	{
		cerr << "Error oening users.csv!" << endl;
		return false;
	}

	file << userInfo.username << "," << CSVFunction::hashPassword(userInfo.password) << endl;
	file.close();
	return true;
}

bool CSVFunction::loginUser(UserInfo userInfo)
{
	ifstream file(USERS);
	if (!file)
	{
		cerr << "Error opening users.csv!" << endl;
		return false;
	}

	string line, user, pass;
	while (getline(file, line))
	{
		stringstream ss(line);
		getline(ss, user, ',');
		getline(ss, pass, ',');
		if (user == userInfo.username && pass == CSVFunction::hashPassword(userInfo.password))
		{
			gUserName = user;
			return true;
		}
	}
	return false;
}

void CSVFunction::createFile()
{
	if (!fileExists(USERS))
	{
		ofstream usersFile(USERS);
		if (usersFile(USERS))
		{
			cout << "Created users file: " << USERS << endl;
		}

		usersFile.close();
	}

	if (!fileExists(TRANSACTIONS))
	{
		ofstream transactionFile(TRANSACTIONS);
		if (transactionFile(TRANSACTIONS))
		{
			cout << "Created users file: " << TRANSACTIONS << endl;
		}

		transactionFile.close();
	}
}

void CSVFunction::readFile()
{
	ifstream file(TRANSACTIONS);
	if (!file)
	{
		cerr << "Error opening transaction.csv" << endl;
		return;
	}

	string line, user, type, description;
	double amount;
	while (getline(file, line))
	{
		stringstream ss(line);
		getline(ss, user, ',');
		getline(ss, type, ',');
		ss >> amount;
		getline(ss, description, ',');
		if (user != gUserName)
		{
			continue;
		}
		cout << type << "\t" << amount << "\t" << description << endl;
	}
}

void CSVFunction::saveData(ExpenceInfo expenceInfo)
{
	ofstream file(TRANSACTIONS, ios::app);
	if (!file)
	{
		cerr << "Error opening transaction.csv!" << endl;
		return;
	}

	file << expenceInfo.username << "," << expenceInfo.type << "," << expenceInfo.amount << "," << expenceInfo.description << endl;
	file.close();
}

int main()
{

	return 0;
}