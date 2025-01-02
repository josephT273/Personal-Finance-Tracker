#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define TRANSACTIONS "transactions.csv"
#define USERS "users.csv"
string gUserName;

struct UserInfo
{
	string username;
	string password;
};

struct ExpenseInfo
{
	string username;
	string type;
	double amount;
	string description;
};

class PersonalExpenseTracker
{
public:
	void createFile();
	void readFile();
	void saveData(ExpenseInfo);
	void updateData(ExpenseInfo);
	void deleteDocument(ExpenseInfo);
	bool registerUser(UserInfo);
	bool loginUser(UserInfo);
	void run();

private:
	bool fileExists(const string &fileName);
};

bool PersonalExpenseTracker::fileExists(const string &fileName)
{
	ifstream file(fileName);
	return file.good();
}

bool PersonalExpenseTracker::registerUser(UserInfo userInfo)
{
	ofstream file(USERS, ios::app);
	if (!file)
	{
		cerr << "Error opening users.csv!" << endl;
		return false;
	}

	file << userInfo.username << "," << userInfo.password << endl;
	file.close();
	return true;
}

bool PersonalExpenseTracker::loginUser(UserInfo userInfo)
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
		if (user == userInfo.username && pass == userInfo.password)
		{
			gUserName = user;
			return true;
		}
	}
	return false;
}

void PersonalExpenseTracker::createFile()
{
	if (!fileExists(USERS))
	{
		ofstream usersFile(USERS);
		cout << "Created users file: " << USERS << endl;
		usersFile.close();
	}

	if (!fileExists(TRANSACTIONS))
	{
		ofstream transactionFile(TRANSACTIONS);
		cout << "Created transactions file: " << TRANSACTIONS << endl;
		transactionFile.close();
	}
}

void PersonalExpenseTracker::readFile()
{
	ifstream file(TRANSACTIONS);
	if (!file)
	{
		cerr << "Error opening transactions.csv" << endl;
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
		ss.ignore(); // Ignore comma
		getline(ss, description, ',');

		if (user != gUserName)
		{
			continue;
		}
		cout << type << "\t" << amount << "\t" << description << endl;
	}
}

void PersonalExpenseTracker::saveData(ExpenseInfo expenseInfo)
{
	ofstream file(TRANSACTIONS, ios::app);
	if (!file)
	{
		cerr << "Error opening transactions.csv!" << endl;
		return;
	}

	file << expenseInfo.username << "," << expenseInfo.type << "," << expenseInfo.amount << "," << expenseInfo.description << endl;
	file.close();
}

void PersonalExpenseTracker::updateData(ExpenseInfo expenseInfo)
{
	ifstream file(TRANSACTIONS);
	if (!file)
	{
		cerr << "Error opening transactions.csv!" << endl;
		return;
	}

	vector<string> lines;
	string line;
	while (getline(file, line))
	{
		lines.push_back(line);
	}
	file.close();

	ofstream outFile(TRANSACTIONS);
	if (!outFile)
	{
		cerr << "Error opening transactions.csv!" << endl;
		return;
	}

	for (const string &line : lines)
	{
		stringstream ss(line);
		string user, type, description;
		double amount;
		getline(ss, user, ',');
		getline(ss, type, ',');
		ss >> amount;
		ss.ignore(); // Ignore comma
		getline(ss, description, ',');

		if (user == expenseInfo.username && type == expenseInfo.type && amount == expenseInfo.amount && description == expenseInfo.description)
		{
			outFile << expenseInfo.username << "," << expenseInfo.type << "," << expenseInfo.amount << "," << expenseInfo.description << endl;
		}
		else
		{
			outFile << line << endl;
		}
	}
	outFile.close();
}

void PersonalExpenseTracker::deleteDocument(ExpenseInfo expenseInfo)
{
	ifstream file(TRANSACTIONS);
	if (!file)
	{
		cerr << "Error opening transactions.csv!" << endl;
		return;
	}

	vector<string> lines;
	string line;
	while (getline(file, line))
	{
		lines.push_back(line);
	}
	file.close();

	ofstream outFile(TRANSACTIONS);
	if (!outFile)
	{
		cerr << "Error opening transactions.csv!" << endl;
		return;
	}

	for (const string &line : lines)
	{
		stringstream ss(line);
		string user, type, description;
		double amount;
		getline(ss, user, ',');
		getline(ss, type, ',');
		ss >> amount;
		ss.ignore(); // Ignore comma
		getline(ss, description, ',');

		if (user == expenseInfo.username && type == expenseInfo.type && amount == expenseInfo.amount && description == expenseInfo.description)
		{
			continue;
		}
		outFile << line << endl;
	}
	outFile.close();
}

ExpenseInfo parseExpenseInfo()
{
	ExpenseInfo expenseInfo;

	cout << "Enter type of expense: ";
	getline(cin, expenseInfo.type);
	cout << "Enter amount: ";
	cin >> expenseInfo.amount;
	cin.ignore();
	cout << "Enter description: ";
	getline(cin, expenseInfo.description);
	expenseInfo.username = gUserName;

	return expenseInfo;
}

void PersonalExpenseTracker::run()
{
	createFile();
	int choice;
	do
	{
		cout << "\nWelcome to Personal Expense Tracker\n";
		cout << "1. Login\n";
		cout << "2. Register\n";
		cout << "3. Exit\n";
		cout << "Enter your choice: ";
		cin >> choice;

		UserInfo userInfo;

		switch (choice)
		{
		case 1:
            do
             {
             	cout << "Enter username: ";
				cin >> userInfo.username;
				cout << "Enter password: ";
				cin >> userInfo.password;

				if (loginUser(userInfo))
				{
					cout << "Login successful! Welcome, " << gUserName << "!\n";
					break;
				}
				else
				{
					cout << "Invalid username or password! Please try again.\n";
				}
             } while (true);
             break;

		case 2:
			cout << "Enter a new username: ";
			cin >> userInfo.username;
			cout << "Enter a new password: ";
			cin >> userInfo.password;

			if (registerUser(userInfo))
			{
				cout << "Registration successful! You can now log in.\n";
			}
			else
			{
				cout << "Error registering user. Please try again.\n";
			}
			continue; // Return to the main menu after registration.

		case 3:
			cout << "Exiting the application. Goodbye!\n";
			return;

		default:
			cout << "Invalid choice! Please try again.\n";
		}
	} while (choice != 1);
    ExpenseInfo expenseInfo;
	// Main application menu after successful login.
	do
	{
		cout << "\nMenu:\n";
		cout << "1. Add Expense\n";
		cout << "2. Edit Expense\n";
		cout << "3. Read Expenses\n";
		cout << "4. Delete Expense\n";
		cout << "5. Exit\n";
		cout << "Enter your choice: ";
		cin >> choice;
		cin.ignore();

		switch (choice)
		{
		case 1:
			expenseInfo = parseExpenseInfo();
			saveData(expenseInfo);
			break;

		case 2:
			expenseInfo = parseExpenseInfo();
			updateData(expenseInfo);
			break;

		case 3:
			readFile();
			cout << "\n\n";
			break;

		case 4:
			expenseInfo = parseExpenseInfo();
			deleteDocument(expenseInfo);
			break;

		case 5:
			cout << "Exiting...\n";
			break;

		default:
			cout << "Invalid choice! Please try again.\n";
		}
	} while (choice != 5);
}

int main()
{
	PersonalExpenseTracker pet;
	pet.run();
	return 0;
}
