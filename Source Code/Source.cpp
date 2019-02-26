#include <iostream>
#include <fstream>
#include<iomanip>
#include <string>
#include <vector>
#include <conio.h>
#include <chrono>
#include "Employee.h" //Class that holds an employee's information: name, surname, age and number.

using namespace std;

void MenuMsg();
void CheckInput(int &); //Checks user's input if its a number from 1-7 for the menu choice.
void AddEmployee(vector <Employee> &); //Adds a new non duplicate employee to the employees vector.
void CheckIfWord(string &); //Checks if the input is a word (name/surname).
void CheckIfDob(string &); //Checks everything for the date of birth.
bool CheckIfDobFormat(string); //Checks if the inputted format of date of birth is correct.
bool CheckIfYear(int, bool &); //Checks if the inputted year is correct (employee might be too young/old to work) and if its a leap year.
bool CheckIfMonth(int, bool, int &); //Checks if the inputted month is correct. 
bool CheckIfDay(int, int); //Checks if the inputted date is correct depending on the month and leap year.
int CheckIfNmb(string &); //Checks if the input is a number.
bool CheckIfDuplicate(vector <Employee>, Employee); //Checks if there is already a employee with that information in the list.
void AlphabeticalSort(vector <Employee> &); //Sorts employee information (surname/name) by alphabetical order.
bool AlphabeticalCheck(Employee, Employee); //Checks which surname+name is alphabetically first.
void ShowEmployees(vector <Employee>); //Shows all employees from the employees vector.
void SaveToFile(vector <Employee>, int); //Loads the non duplicate employee's information from the "database.txt" file to the employees vector and then saves everything back to the file.
void LoadFromFile(vector <Employee> &, int); //Loads the non duplicate employee's information from the "database.txt" file to the employees vector.
void CheckDuplicatesFromFile(vector <Employee> & , vector <Employee> &); //Removes the duplicate employee information when it is loaded from the "database.txt" file.
void SearchEmployee(vector <Employee> &); //Searches for a specific employee from the employees vector based on their name and gives an option to remove them.
void PressToCnt();
void DeleteInformation(vector <Employee> &); //Asks the user if he wants to delete employee information in the vector and in the database file.
void Exit(bool &, vector <Employee>, int); //Before exiting asks the user if he wants to save employee information to the database.

int main()
{
		vector<Employee> employees; //Vector that holds all the employee's information.
		bool exit = true;
		int choice;
		do
		{
			MenuMsg();
			CheckInput(choice);
			switch (choice)
			{
			case(1) :
			{
				AddEmployee(employees);
				break;
			}

			case(2) :
			{
				ShowEmployees(employees);
				break;
			}

			case(3) :
			{
				SaveToFile(employees, choice);
				PressToCnt();
				break;
			}

			case(4) :
			{
				LoadFromFile(employees, choice);
				break;
			}
			case(5) :
			{
				SearchEmployee(employees);
				break;
			}
			case(6) :
			{
				DeleteInformation(employees);
				break;
			}
			case(7) :
			{
				Exit(exit, employees, choice);
				break;
			}
			default:
				break;
			}
	} while (exit);
}

void MenuMsg()
{
	system("cls");
	cout << "Database application." << endl << endl;
	cout << "What would you like to do: " << endl;
	cout << "(1) Add employee." << endl;
	cout << "(2) Show all employees." << endl;
	cout << "(3) Save employee data to database file." << endl;
	cout << "(4) Load employee data from database file." << endl;
	cout << "(5) Search for an employee." << endl;
	cout << "(6) Delete employee information." << endl;
	cout << "(7) Exit the program." << endl;
	cout << endl << "Your input: ";
}

void CheckInput(int & choice)
{
	string temp;
	getline(cin, temp);
	if (temp.size()==1 && (temp[0] >= '1' && temp[0] <= '7')) //Menu choice is from 1-7.
	{
		choice = stoi(temp);
	}
	else
	{
		cout << "Wrong input, please input a single number from 1 to 7: ";
		CheckInput(choice);
	}
}

void AddEmployee(vector <Employee> & employees)
{
	Employee employee;
	string surname, name, dob, temp;
	int number;
	cout << "Please input the Surname of the employee: ";
	getline(cin, surname);
	CheckIfWord(surname);
	cout << "Please input the Name of the employee: ";
	getline(cin, name);
	CheckIfWord(name);
	cout << "Please input the Date of birth of the employee in a format DD/MM/YYYY: " << endl;
	getline(cin, dob);
	CheckIfDob(dob);
	cout << "Please input the Number of the employee: ";
	getline(cin, temp);
	number = CheckIfNmb(temp);
	employee.addEmployee(surname, name, dob, number);
	if (CheckIfDuplicate(employees, employee))
		cout << "You have inputted duplicate information about an employee that is already on the list." << endl;
	else
	{
		cout << "Employee has been added to the list." << endl;
		employees.push_back(employee);
	}
	AlphabeticalSort(employees); 
	PressToCnt();
}

void CheckIfWord(string & temp)
{
	bool word = true;
	if (temp.size() == 0)
		word = false;
	else
	{
		for (int i = 0; i < temp.size(); i++)
		{
			if (!((temp[i] >= 'a' && temp[i] <= 'z') || (temp[i] >= 'A' && temp[i] <= 'Z')))
			{
				word = false;
				break;
			}
		}
	}
	if (word == false)
	{
		cout << "Wrong input, you must enter a name/surname with letters (a/A - z/Z): ";
		getline(cin, temp);
		CheckIfWord(temp);
	}
}

void CheckIfDob(string & temp)
{
	bool dob = true;
	bool leap = false;
	int daysInMonth;
	if (CheckIfDobFormat(temp)) //First check if the dob format was correctly inputted (DD/MM/YYYY).
	{
		string buffer; //Takes from the dob format specific numbers for year/month/day integer variables.
		int year, month, day;
		buffer.append(temp.begin() + 6, temp.end()); 
		year = stoi(buffer);
		buffer.clear();
		buffer.append(temp, 3, 2);
		month = stoi(buffer);
		buffer.clear();
		buffer.append(temp, 0, 2);
		day = stoi(buffer);
		if (CheckIfYear(year, leap)) //Checks if the year was correctly inputted and if its a leap year.
		{
			if (CheckIfMonth(month, leap, daysInMonth)) //Checks if the month was correctly inputted and depending on the month and leap year defines the maximum number of days for that month.
			{
				if (CheckIfDay(day, daysInMonth)) //Checks if the day was correctly inputted, max is daysInMonth.
					dob = true;
				else
					dob = false;
			}
			else
				dob = false;
		}
		else
			dob = false;
	}
	else
		dob = false;
	if (dob == false)
	{
		cout << "Wrong input, you must enter a correct date of birth with the format (DD/MM/YYYY): " << endl;
		getline(cin, temp);
		CheckIfDob(temp);
	}
}

bool CheckIfDobFormat(string temp)
{
	if (temp.size() != 10) //Date of birth format looks like DD/MM/YYYY.
		return false;
	else if (temp[2] != '/' || temp[5] != '/') 
		return false;
	else
	{
		for (int i = 0; i < temp.size(); i++)
		{
			if (i != 2 && i != 5)
			{
				if (!(temp[i] >= '0' && temp[i] <= '9')) //As long as it is a number, correct dates will be checked with other functions.
					return false;
			}
		}
	}
	return true;
}

bool CheckIfYear(int year, bool & leap)
{
	auto now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm ptm;
	localtime_s(&ptm, &now_c);
	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S ", &ptm);
	int currentYear;
	currentYear = 1000 * (buffer[0] - '0') + 100 * (buffer[1] - '0') + 10 * (buffer[2] - '0') + 1 * (buffer[3] - '0');
	if (year >= currentYear - 65 && year <= currentYear - 18) //Employees must be older than 18 and younger than 65 to work.
	{
		if ((year % 4 == 0) && (year % 100 != 0)) //Condition for a leap year.
			leap = true;
		else if ((year % 4 == 0) && (year % 100 == 0) && (year % 400 == 0)) //Condition for a leap year.
			leap = true;
		else
			leap = false;
		return true;
	}
	else
		return false;
}

bool CheckIfMonth(int month, bool leap, int & daysInMonth)
{
	if (month >= 1 && month <= 12)
	{
		if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) 
			daysInMonth = 31;
		else if (month == 2)
		{
			if (leap)
				daysInMonth = 29;
			else
				daysInMonth = 28;
		}
		else
			daysInMonth = 30;
		return true;
	}
	else
		return false;
}

bool CheckIfDay(int day, int daysInMonth)
{
	if (day >= 1 && day <= daysInMonth)
		return true;
	else
		return false;
}

int CheckIfNmb(string & temp)
{
	int temp_i;
	bool nmb = true;
	if (temp.size() == 0)
		nmb = false;
	else
	{
		for (int i = 0; i < temp.size(); i++)
		{
			if (!(temp[i] >= '0' && temp[i] <= '9'))
			{
				nmb = false;
				break;
			}
		}
	}
	if (nmb)
		return temp_i = stoi(temp);
	else
	{
		cout << "Wrong input, you must enter a number: ";
		getline(cin, temp);
		CheckIfNmb(temp);
	}
}

bool CheckIfDuplicate(vector <Employee> employees, Employee employee)
{
	if (employees.size() > 0)
	{
		for (int i = 0; i < employees.size(); i++)
		{
			if (employee.returnSurname() == employees[i].returnSurname() && employee.returnName() == employees[i].returnName() && employee.returnDob() == employees[i].returnDob() && employee.returnNumber() == employees[i].returnNumber())
				return true;
		}
	}
	return false;
}

void AlphabeticalSort(vector <Employee> & employees)
{
	
	Employee temp;
	if (employees.size() > 1)
	{
		for (int i = 0; i < employees.size() - 1; i++)
		{
			for (int j = 0; j < employees.size() - (1 + i); j++)
			{
				if (AlphabeticalCheck(employees[j], employees[j + 1])) //If employees[j] is alphabetically later than employees[j+1] switch places.
				{
					temp = employees[j];
					employees[j] = employees[j + 1];
					employees[j + 1] = temp;
				}
			}
		}
	}
}

bool AlphabeticalCheck(Employee previous, Employee next)
{
	int previousInt, nextInt;
	string previousStr, nextStr;
	previousStr = previous.returnSurname() + previous.returnName(); //Alphabetical sorting will be based upon surname and name.
	for (int i = 0; i < previousStr.size(); i++)
	{
		if (previousStr[i] >= 'A' && previousStr[i] <= 'Z') //If there are any uppercase letters turn them all into lowercase for alphabetical check.
			previousStr[i] += 32;
	}
	nextStr = next.returnSurname() + next.returnName();
	for (int i = 0; i < nextStr.size(); i++)
	{
		if (nextStr[i] >= 'A' && nextStr[i] <= 'Z')
			nextStr[i] += 32;
	}
	if (previousStr.size() < nextStr.size()) //If every letter the same the shorter string is alphabetically first.
	{
		for (int i = 0; i < previousStr.size(); i++) //Checks letter by letter.
		{
			previousInt = previousStr[i];
			nextInt = nextStr[i];
			if (previousInt < nextInt)
				return false;
			else if (previousInt > nextInt)
				return true;
		}
		return false;
	}
	else if (previousStr.size() > nextStr.size())
	{
		for (int i = 0; i < nextStr.size(); i++)
		{
			previousInt = previousStr[i];
			nextInt = nextStr[i];
			if (previousInt < nextInt)
				return false;
			else if (previousInt > nextInt)
				return true;
		}
		return true;
	}
	else
	{
		for (int i = 0; i < previousStr.size(); i++)
		{
			previousInt = previousStr[i];
			nextInt = nextStr[i];
			if (previousInt < nextInt)
				return false;
			else if (previousInt > nextInt)
				return true;
		}
		return false;
	}
}

void ShowEmployees(vector <Employee> employees)
{
	if (employees.size() > 0)
	{
		for (int i = 0; i < employees.size(); i++)
		{
			cout << i + 1 << ". employee: " << endl;
			employees[i].showEmployee();
		}
	}
	else
		cout << "No employees currently listed." << endl;
	PressToCnt();
}

void SaveToFile(vector <Employee> employees,int choice)
{
	if (employees.size() == 0)
		cout << "No employee's information was inputted to save to the database file." << endl;
	else
	{
		LoadFromFile(employees, choice);
		fstream file;
		file.open("database.txt", fstream::out | fstream::trunc);
		if (file.is_open())
		{
			file << left << setw(15) << "Surname:" << left << setw(15) << "Name:" << left << setw(15) << "DOB:" << "Number:" << endl;
			for (int i = 0; i < employees.size(); i++)
			{
				file << left << setw(15) << employees[i].returnSurname() << left << setw(15) << employees[i].returnName() << left << setw(15) << employees[i].returnDob() << employees[i].returnNumber();
				if (i != employees.size() - 1) //When the last employee's information has been saved to the file, don't go into a new line.
				{
					file << endl;
				}
			}
			cout << "Employee information has been saved to the text file \"database\"." << endl;
			file.close();
		}
		else
			cout << "Error. Text file \"database\" was not opened properly." << endl;
	}
}

void LoadFromFile(vector <Employee> & employees, int choice)
{
	vector <Employee> fileEmployees;
	Employee fileEmployee;
	string name, surname, dob;
	int number;
	fstream file;
	string line;
	int i = 0;
	file.open("database.txt", fstream::in);
	if (file.is_open())
	{
		
		file.seekg(0, file.end);
		int fileLength = file.tellg();
		file.seekg(0, file.beg);
		if (fileLength == 0)
		{
			if(choice == 4) //When SaveToFile calls LoadFromFile this message is not neccessary.
			cout << "Text file \"database\" has no employee information stored in it." << endl;
		}
		else
		{
			while (getline(file, line)) //First line of the file is skipped since getline() puts the pointer in the next row.
			{
				file >> name >> surname >> dob >> number;
				fileEmployee.addEmployee(name, surname, dob, number);
				fileEmployees.push_back(fileEmployee);
			}
			if(employees.size() > 0) 
				CheckDuplicatesFromFile(employees, fileEmployees); //If there is already employee information on the vector check if the incoming information from file is duplicate.
			if (fileEmployees.size() > 0) 
			for (int i = 0; i < fileEmployees.size(); i++) //After the duplicate information from the file has been removed add the rest to the vector.
			{
				employees.push_back(fileEmployees[i]);
			}
			if (choice == 4)//When SaveToFile calls LoadFromFile this message is not neccessary.
			cout << "Employee information has been loaded from the text file \"database\"." << endl;
		}
		file.close();
	}
	else
		cout << "Error. Text file \"database\" was not opened properly." << endl;
	AlphabeticalSort(employees);
	if (choice == 4)//When SaveToFile calls LoadFromFile this message is not neccessary.
	PressToCnt();
}

void CheckDuplicatesFromFile(vector <Employee> & employees, vector <Employee> & fileEmployees)
{
	vector <int> duplicates;
	for (int i = 0; i < fileEmployees.size(); i++)
	{
		if (CheckIfDuplicate(employees, fileEmployees[i]))
		{
			duplicates.insert(duplicates.begin(), i); //The last duplicate employee information will be the first to be deleted to avoid problems.
		}
	}
	for (int i = 0; i < duplicates.size(); i++) //Deletes the duplicate employee information based on the duplicates vector.
		fileEmployees.erase(fileEmployees.begin() + duplicates[i]);
}

void SearchEmployee(vector <Employee> & employees)
{
	string temp;
	vector <int> fired_emp;
	string name_search, surname_search;
	bool search = false;
	if (employees.size() == 0)
	{
		cout << "No employees to search from." << endl;
	}
	else
	{
		cout << "Please input surname of the employee you're searching: ";
		getline(cin, surname_search);
		cout << "Please input name of the employee you're searching: ";
		getline(cin, name_search);
		for (int i = 0; i < employees.size(); i++)
		{
			if (surname_search == employees[i].returnSurname() && name_search == employees[i].returnName())
			{
				cout << endl << "Employee found: " << endl;
				employees[i].showEmployee();
				cout << "Would you like to fire this employee (y/Y) or any other key for no: ";
				getline(cin, temp);
				if (temp[0] == 'y' || temp[0] == 'Y')
				{
					cout << "Employee was fired." << endl;
					fired_emp.insert(fired_emp.begin(), i); //The last employee to be fired is first to be erased from the list so it doesn't create problems with positioning.
				}
				else
					cout << "Employee was not fired." << endl;
				search = true;
			}
		}
		if (!search)
		{
			cout << "Employee with the name \"" << name_search << " " << surname_search << "\" is not employed." << endl;
		}
		else
		{
			if (fired_emp.size() > 0) //Erasing is done outside the for loop so the employees.size() stays the same in the for loop.
			{
				for (int i = 0; i < fired_emp.size(); i++)
				{
					employees.erase(employees.begin() + fired_emp[i]);
				}
			}
		}
	}
	PressToCnt();
}

void DeleteInformation(vector <Employee> & employees)
{
	string temp; 
	int i;
	cout << "Would you like to delete all employee information in the application? Y/y for yes or any other key for no: ";
	getline(cin, temp);
	if (temp[0] == 'Y' || temp[0] == 'y')
	{
		if (employees.size() == 0)
			cout << "No employee's information was inputted to delete from the application." << endl << endl;
		else
		{
			while (employees.size() > 0)
			{
				i = employees.size() - 1;
				employees.erase(employees.begin() + i);
			}
			cout << "All employee's information has been deleted from the application." << endl << endl;
		}
	}
	fstream file;
	int fileLength;
	cout << "Would you like to delete all employee information in the database file? Y/y for yes or any other key for no: ";
	getline(cin, temp);
	if (temp[0] == 'Y' || temp[0] == 'y')
	{
		file.open("database.txt", fstream::in);
		if (file.is_open())
		{

			file.seekg(0, file.end);
			fileLength = file.tellg();
			if (fileLength == 0) //If the database file is empty no need to delete anything from it.
				cout << "Text file \"database\" has no employee information stored in it." << endl;
			file.close();
		}
		else
			cout << "Error. Text file \"database\" was not opened properly." << endl;
		if(fileLength != 0) //Database file isnt empty with fstream::trunc delete everything.
		{
				file.open("database.txt", fstream::out | fstream::trunc);
				if (file.is_open())
				{
					cout << "All employee information in the text file \"database\" has been deleted." << endl;
					file.close();
				}
				else
					cout << "Error. Text file \"database\" was not opened properly." << endl;
		}
	}
	PressToCnt();
}

void Exit(bool & exit, vector <Employee> employees, int choice)
{
	string temp;
	cout << "Would you like to save the employee information to the database before exiting? Y/y for yes or any other key for no: ";
	getline(cin, temp);
	if (temp[0] == 'Y' || temp[0] == 'y')
		SaveToFile(employees, choice);
	exit = false;	
}

void PressToCnt()
{
	cout << "Press any key to continue.";
	_getch();
}
