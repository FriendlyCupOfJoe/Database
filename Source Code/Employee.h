#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Employee //Class that holds an employee's information: name, surname, age and number.
{
private:
	string surname;
	string name;
	string dob;
	int number;
public:
	Employee() {};
	~Employee() {};
	void addEmployee(string surname, string name, string dob, int number)
	{
		this->surname = surname;
		this->name = name;
		this->dob = dob;
		this->number = number;
	};
	void showEmployee()
	{
		cout << "Surname of the Employee: " << returnSurname() << endl;
		cout << "Name of the Employee: " << returnName() << endl;
		cout << "Date of birth of the Employee: " << returnDob() << endl;
		cout << "Number of the Employee: " << returnNumber() << endl << endl;
	};
	string returnSurname() { return this->surname; };
	string returnName() { return this->name; };
	string returnDob() { return this->dob; };
	int returnNumber() { return this->number; };
};
