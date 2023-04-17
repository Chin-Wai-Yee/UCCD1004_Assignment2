#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

/*******************************************************************************************************/
// Global variables

const int MAX_STUDENTS = 100; // Maximum number of students that can be stored
const int MAX_COURSES = 50; // Maximum number of courses that can be stored
const int MAX_LECTURERS = 20; // Maximum number of lecturers that can be stored
// parallel array for gred and gpa
string gred[] = {"A+", "A", "A-", "B+", "B", "B-", "C+", "C", "F"};
double gpa[] = {4.00, 4.00, 3.67, 3.33, 3.00, 2.67, 2.33, 2.00, 0.00};

/*******************************************************************************************************/
// Declaration of structs

/// @brief Course that provided
struct Course {
    string code; // course code (etc: UCCD1004)
    string name; // course name (etc: Programming Practices)
    string gred; // course gred (etc: A+)
    int credit_hour; // course credit hour (etc: 4)
    double gpa; // course gpa (etc: 4.00)
};

/// @brief Struct representing identity, including students and lecturers
struct People {
    string id; // Student id
    string name; // Student name
    string password; // Student password in hash
    int course_num; // number of course taken
    Course course[MAX_COURSES]; // Taken courses in array
    int total_credit_hour; // Total credit hour
    People () {
        course_num = 0;
        total_credit_hour = 0;
    }
};

/*******************************************************************************************************/
// Function declaration

void   readFile(string, People[], int&, bool = false);
void   writeFile(string, People[], int, bool = false);
void   mainMenu(People[], People[], int&, int&, bool&);
void   courseMenu(People, Course[], bool = false);
void   studentMenu(People[], int&);
void   lecturerMenu(People[], int&, bool&);
bool   testStudentId(string);
bool   testName(string);
bool   testCourseCode(string);
string toUpper(string);
int    searchEngine(string, People[], int, int[]);
int    getIndex(People[], string, int);
int    getIndex(Course[], string, int);
int    getGpa(string);
double cgpaCalc(Course[], int, int);
void   modifyPeopleInfo(People[], string, int&, bool = false);
void   deletePeople(People[], int&);
void   addCourseInfo(Course[], int&, string, bool = false);
void   deleteCourse(Course[], int&);
void   view(People, Course[]);
void   view(People[], int, string, bool = false);
bool   login(People[], int, int&);

/*******************************************************************************************************/
// Main function
int main() {
    system("echo off");
    system("chcp 437");
    People students[MAX_STUDENTS]; // Array to store People data
    People lecturers[MAX_LECTURERS]; // Array to store lecturer data
    // Read file and get current number of students in the array
    int student_num, lecturer_num;
    readFile("students_list.txt", students, student_num, true);
    readFile("lecturer_list.txt", lecturers, lecturer_num, false);
    // Start the program
    bool exit = false;
    while (!exit) {
        mainMenu(students, lecturers, student_num, lecturer_num, exit);
    }
    writeFile("students_list_output.txt", students, student_num, true);
    writeFile("lecturer_list_output.txt", lecturers, lecturer_num, false);
    return 0;
}

/*******************************************************************************************************/
// File handling related:

/**
 * @brief Read people info from targeted file
 * @param file_name Input file to be read
 * @param people The array to store data
 * @param count The number of people in the array
 * @param isStudent Differentiate between student and other identity, default to false
*/
void readFile(string file_name, People people[], int& count, bool isStudent) {
    
    // if the file does not exist create it, else work with it
    fstream file(file_name, ios::app | ios::in);

    // start reading
    // get the id, name and password
    for (count = 0; !file.eof(); count++) {

        getline(file, people[count].id, '|');
        getline(file, people[count].name, '|');

        if (isStudent) {
            // get the password
            getline(file, people[count].password, '|');

            // read course info
            file >> people[count].course_num;
            file.ignore();
            // loop every course of student and read the info
            for (int j = 0; j < people[count].course_num ; j++) {
                getline(file, people[count].course[j].code, '|');
                getline(file, people[count].course[j].name, '|');
                getline(file, people[count].course[j].gred);
                people[count].course[j].credit_hour = stoi(people[count].course[j].code.substr(7, 1));
                people[count].total_credit_hour += people[count].course[j].credit_hour;
            }
        }
        else {
            getline(file, people[count].password);
        }
    }
    // done reading people info
    file.close();
}

/**
 * @brief Write students info into targeted file
 * @param file_name Output file to be write
 * @param students Student array to write
 * @param student_num Number of students, default to 0
*/
void writeFile(string file_name, People people[], int count, bool isStudent) {

    fstream file(file_name, ios::out);

    for (int i = 0; i < count; i++) {
        // write people info
        file << people[i].id << '|';
        file << people[i].name << '|';
        
        // if student, need to write course info
        if (isStudent) {
            file << people[i].password << '|';
            file << people[i].course_num;
            //write course info
            for (int j = 0; j < people[i].course_num; j++) {
                file << endl;
                file << people[i].course[j].code << '|';
                file << people[i].course[j].name << '|';
                file << people[i].course[j].gred;
            }
        }
        else {
            file << people[i].password;
        }

        // if not last people, write new line
        if (i != count - 1){
            file << endl;
        }
    }
    file.close();
}

/*******************************************************************************************************/
// Menu related:

void mainMenu(People students[], People lecturers[], int& student_num, int& lecturer_num, bool& exit) {

    system("cls");
    cout << "Welcome to Student Management System" << endl;
    cout << "Please select your identity" << endl;
    cout << endl;
    cout << "1. Student" << endl;
    cout << "2. Lecturer" << endl;
    cout << "3. Admin" << endl;
    cout << endl;
    cout << "0. Exit" << endl;
    cout << endl;
    cout << "Enter your option : ";
    string option;
    getline(cin, option);
    int i;
    if (option == "1") {
        if (login(students, student_num, i)) {
            courseMenu(students[i], students[i].course);
        }
    }
    else if (option == "2") {
        if (login(lecturers, lecturer_num, i)) {
            studentMenu(students, student_num);
        }
    }
    else if (option == "3") {
        cout << "This feature is not available yet" << endl;
    }
    else if (option == "0") {
        cout << "Exiting..." << endl;
        exit = true;
    }
    else {
        cout << "Invalid option" << endl;
    }
    system("pause");
}

/**
 * @brief Show info of stuent's course
 * @param student The student to show course info
 * @param courses The array of courses
 * @param is_lecturer More features for lecturer, default to false
*/
void courseMenu(People student, Course courses[], bool is_lecturer) {

    int course_num = student.course_num;
    bool loop = true;
    while (loop) {
        system("cls");
        cout << "Course Management System" << endl;
        cout << endl;
        view(student, courses);
        cout << endl;

        cout << "1. Print course(s) information to file" << endl;
        if (is_lecturer) {
            cout << "2. Add course" << endl;
            cout << "3. Edit course" << endl;
            cout << "4. Delete course" << endl;
        }
        cout << endl;

        cout << "0. Exit" << endl;
        cout << endl;

        cout << "Please enter your option : ";
        string option;
        getline(cin, option);
        if (option == "1") {
            cout << "The function is not ready yet~";
        }
        else if (is_lecturer) {
            if (option == "2") {
                addCourseInfo(courses, course_num, "Do you want to add a new course");
            }
            else if (option == "3") {
                addCourseInfo(courses, course_num, "Do you want to edit a course", true);
            }
            else if (option == "4") {
                deleteCourse(courses, course_num);
            }
        }
        else if (option == "0") {
            cout << "Exiting..." << endl;
            loop = false;
        }
        else {
            cout << "Invalid option" << endl;
        }
        system("pause");
    }
}

void studentMenu(People students[], int& student_num) {

    bool loop = true;
    while (loop) {
        system("cls");
        cout << "Student Management System" << endl;
        cout << endl;
        view(students, student_num, "Student", true);
        cout << endl;
        cout << "1. Add student" << endl;
        cout << "2. Edit student" << endl;
        cout << "3. Delete student" << endl;
        cout << "4. View student" << endl;
        cout << endl;
        cout << "0. Exit" << endl;
        cout << endl;

        cout << "Please enter your option : ";
        string option;
        getline(cin, option);
        if (option == "1") {
            modifyPeopleInfo(students, "Do you want to add a new student", student_num);
        }
        else if (option == "2") {
            modifyPeopleInfo(students, "Do you want to edit a student", student_num, true);
        }
        else if (option == "3") {
            deletePeople(students, student_num);
        }
        else if (option == "4") {
            cout << "Please enter the student ID : ";
            string id;
            getline(cin, id);
            int i = getIndex(students, id, student_num);
            if (i != -1) {
                courseMenu(students[i], students[i].course, true);
            }
            else {
                cout << "Student not found, please try again." << endl;
            }
        }
        else if (option == "0") {
            cout << "Exiting..." << endl;
            loop = false;
        }
        else {
            cout << "Invalid option. Please try again." << endl;
        }
        system("pause");
    }
}

void lecturerMenu(People lecturers[], int& lecturer_num, bool& exit) {
    
    system("cls");
    cout << "Lecturer Management System" << endl;
    cout << "1. Add lecturer" << endl;
    cout << "2. Edit lecturer" << endl;
    cout << "3. Delete lecturer" << endl;
    cout << "4. View lecturer" << endl;
    cout << endl;
    cout << "0. Exit" << endl;
    cout << endl;

    cout << "This fucntion is not ready yet!" << endl;
    system("pause");
}

/*******************************************************************************************************/
// Validations and search related

/**
 * @brief Check if the input is a valid ID
 * @param input ID to be checked
 * @return true if valid, false if not
*/
bool testStudentId(string input)
{
	if (input.size() != 7)
	{
		return false;
	}
	for (int i = 0; i < input.length(); i++)
	{
		if (!isdigit(input[i]))
		{
			return false;
		}
	}
	return true;
}

/**
 * @brief Check if the input is a valid name (only alphabets and space)
 * @param input Name to be checked
 * @return true if valid, false if not
*/
bool testName(string input)
{
	for (int i = 0; i < input.length(); i++)
	{
		if (!isalpha(input[i]) && input[i] != ' ')
		{
			return false;
		}
	}
	return true;
}

/**
 * @brief Check if the input is a valid course code (first 4 characters are alphabets, last 4 characters are digits)
 * @param input Course code to be checked
 * @return true if valid, false if not
*/
bool testCourseCode(string input)
{
	int count;  // Loop counter

	if (input.size() != 8)
	{
		return false;
	}
	// accept the array arg and test the first three characters for alphabetic letters.
	for (count = 0; count < 4; count++)
	{
		if (!isalpha(input[count]))   //return true if it is alphabetic character. ! Is used
			return false;		      // to test char is not alphabetic.
	}

	// Test the remaining characters for numeric digits.
	for (count = 4; count < input.size() - 1; count++)
	{
		if (!isdigit(input[count]))
			return false;
	}
	return true;
}

/**
 * @brief Convert a string to uppercase
 * @param str String to be converted
 * @return Uppercase string
*/
string toUpper(string str) {
    for (size_t i = 0; i < str.size(); i++) {
        str[i] = toupper(str[i]);
    }
    return str;
}

int searchEngine(string prompt, People students[], int student_num, int result[])
{
    int search_result_num = 0;

    cout << "Please enter " << prompt << " : ";
    string search;
    getline(cin, search);
    search = toUpper(search);

    for (int i = 0; i < student_num; i++)
    {
        if (toUpper(students[i].name).find(search) != string::npos ||
            toUpper(students[i].id).find(search) != string::npos)
        {
            result[search_result_num] = i;
            search_result_num++;
        }
    }
    return search_result_num;
}

/**
 * @brief Get the index of a student
 * @param Students Array of students
 * @param id ID of the student
 * @param student_num Number of students
 * @return Index of the student
*/
int getIndex(People Students[], string id, int student_num) {
    for (int i = 0; i < student_num; i++) {
        if (Students[i].id == id) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Get the index of a course
 * @param courses Array of course
 * @param code Code of the course
 * @param course_num Number of course
 * @return Index of the course
*/
int getIndex(Course courses[], string code, int course_num) {
    for (int i = 0; i < course_num; i++) {
        if (courses[i].code == code) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Get the gpa of a gred
 * @param search_gred Gred to search
 * @return Gpa of the gred
*/
int getGpa(string search_gred) {
    for (int i = 0; i < 9; i++) {
        if (search_gred == gred[i]) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Calculate cgpa
 * @param course Array of course
 * @param course_num Number of course
 * @param total_credit_hour Total credit hour
 * @return cgpa
*/
double cgpaCalc(Course course[], int course_num, int total_credit_hour)
{
    double total_gpa = 0, cgpa = 0;
    if (course_num == 0)
    {
        return 0;
    }
    for (int j = 0; j < course_num; j++)
    {
        total_gpa += gpa[getGpa(course[j].gred)] * course[j].credit_hour;
    }
    cgpa = total_gpa / total_credit_hour;
    return cgpa;
}

/*******************************************************************************************************/

// CRUD related

// People related

/**
 * @brief Add / Update people
 * @param people Array of people
 * @param messages Message to be displayed
 * @param newIndex Index of the people
 * @param update If true, update the people, else add a new people, default is false
*/
void modifyPeopleInfo(People people[], string messages, int& newIndex, bool update)
{
	int loop = 0, i = newIndex;
	while (loop >= 0)
	{
		loop++;
		string input;
		do
		{   
			cout << messages << "? (1-yes, 2-no): ";
			getline(cin, input);
			if (input == "1")
			{
				loop++;
			}
			else if (input == "2")
			{
				cout << "Exiting...";
				loop = -1;
			}
			else
			{
				cout << "Invalid input! Please enter again!" << endl;
			}
			cout << endl;

		} while (input != "1" && input != "2");

		while (loop == 2)
		{
			cout << "Please enter ID: ";
			string id;
			getline(cin, id);
			if (update)
			{
				i = getIndex(people, id, newIndex);
				if (i == -1)
				{
					cout << "ID not found!" << endl;
					cout << "Please enter again!" << endl;
				}
				else
				{
					loop++;
				}
			}
			else if (testStudentId(id))
			{
				people[i].id = id;
				loop++;
			}
			else
			{
				cout << "That is not the proper format of the ID." << endl;
				cout << "Here is an example : 1234567 " << endl;
				cout << "Please enter again!" << endl;
			}
		}

		while (loop == 3)
		{
			cout << "Student's name: ";
			string name;
			getline(cin, name);
			if (testName(name))
			{
				people[i].name = name;
                if (!update)
                {
                    newIndex++;
                }
				loop = -1;
			}
			else
			{
				cout << "That is not the proper format of name." << endl;
				cout << "Here is an example : John Smith " << endl;
				cout << "Please enter again!" << endl;
			}
		}
	}
}

void deletePeople(People people[], int& newIndex)
{
	string id;
	cout << "Please enter ID: ";
	getline(cin, id);
	cout << endl;

    int i = getIndex(people, id, newIndex);
    if (i == -1)
    {
        cout << "Student ID not found!" << endl;
        cout << "Please enter agian!" << endl;
    }
    else
    {
        for (; i < newIndex - 1; i++)
        {
            people[i] = people[i + 1];
        }
        newIndex--;
    }
}

// Course related

/**
 * @brief Add / Update course
 * @param course Array of course
 * @param prompt Message to be displayed
 * @param newIndex Index of the course
 * @param update If true, update the course, else add a new course, default is false
*/
void addCourseInfo(Course course[], int& newIndex, string prompt, bool update)
{
	int loop = 0, i;
	do
	{
		while (loop == 0)
		{
			cout << prompt << "? (1-yes, 2-no): ";
			string input;
			getline(cin, input);
			if (input == "1")
			{
				loop++;
			}
			else if (input == "2")
			{
				cout << "Exiting...";
				loop = -1;
			}
			else
			{
				cout << "Invalid input! Please enter again!" << endl;
			}
		}
		while (loop == 1)
		{
			cout << "Course code: ";
			string course_code;
			getline(cin, course_code);
			if (update)
			{
				i = getIndex(course, course_code, newIndex);
				if (i == -1)
				{
					cout << "Course code not found!" << endl;
					cout << "Please enter again!" << endl;
				}
				else
				{
					loop++;
				}
			}
			else if (testCourseCode(course_code))
			{
				course[i].code = course_code;
				loop++;
			}
			else
			{
				cout << "That is not the proper format of the course code." << endl;
				cout << "Here is an example : LLLLNNNN " << endl;
				cout << "(L = letters and N = numbers)" << endl;
			}
		}
		while (loop == 2)
		{
			cout << "Course Name: ";
			string course_name;
			getline(cin, course_name);
			if (testName(course_name))
			{
				course[i].name = course_name;
				loop++;
			}
			else
			{
				cout << "That is not the proper format of the course name." << endl;
				cout << "Here is an example : Introduction to Programming " << endl;
				cout << "Please enter again!" << endl;
			}
		}
		while (loop == 3)
		{
			cout << "Gred: ";
			string gred;
			cin >> gred;
			if (getGpa(gred) != -1)
			{
				course[i].gred = gred;
				loop++;
                if (!update)
                {
                    newIndex++;
                }
			}
			else
			{
				cout << "That is not the proper format of the gred." << endl;
				cout << "Here is an example : A+ " << endl;
				cout << "Please enter again!" << endl;
			}
		}
	} while (loop >= 0);
}

void deleteCourse(Course course[], int& newIndex)
{
	string input;
	cout << "Enter course name or course code: ";
	getline(cin, input);
	cout << endl;

    int i = getIndex(course, input, newIndex);
    if (i == -1)
    {
        cout << "Course not found!" << endl;
        cout << "Please enter agian!" << endl;
    }
    else
    {
        for (; i < newIndex - 1; i++)
        {
            course[i] = course[i + 1];
        }
        newIndex--;
    }
}

/*******************************************************************************************************/
// Output related

/**
 * @brief View student information
 * @param student Student information
 * @param course Array of course
*/
void view(People student, Course course[])
{
    cout << "Student name: " << student.name << endl;
    cout << "Student ID: " << student.id << endl;

    // cout all the courses
    cout << "No. \t";
    cout << "Course code \t";
    cout << setw(60) << left << "Course name";
    cout << "Credit hour \t";
    cout << "Gred" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < student.course_num; i++)
    {
        cout << i + 1 << "\t";
        cout << course[i].code << "\t";
        cout << setw(60) << left << course[i].name;
        cout << course[i].credit_hour << "\t";
        cout << course[i].gred << endl;
    }
    cout << "CGPA: " << setw(4) << fixed << cgpaCalc(course, student.course_num, student.total_credit_hour) << endl;
}

/**
 * @brief View all people information
 * @param people Array of people information
 * @param size Size of the array
 * @param type Identity of the people (student/lecturer)
 * @param show_cgpa Show the cgpa, default is false
*/
void view (People people[], int size, string type, bool show_cgpa)
{
    cout << "No. \t";
    cout << setw(60) << left << type + " name";
    cout << type << " ID \t";
    if (show_cgpa)
    {
        cout << "CGPA";
    }
    cout << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < size; i++)
    {
        cout << i + 1 << "\t";
        cout << setw(60) << left << people[i].name;
        cout << people[i].id << "\t";
        if (show_cgpa)
        {
            cout << setw(4) << fixed << cgpaCalc(people[i].course, people[i].course_num, people[i].total_credit_hour);
        }
        cout << endl;
    }
}

/*******************************************************************************************************/
// Login related

/**
 * @brief Verify the login
 * @param password Correct password
 * @return True if the password is correct, false otherwise
*/
bool login(People people[], int size, int& index) {
    
    cout << "Please enter your ID :";
    string id;
    getline(cin, id);
    int i = getIndex(people, id, size);
    if (i == -1) {
        cout << "ID not found. Please try again." << endl;
        return false;
    }
    else {
        cout << "Please enter your password : ";
        string user_input;
        getline(cin, user_input);
        if (user_input != people[i].password) {
            cout << "Wrong password. Please try again." << endl;
            return false;
        }
    }
    index = i;
    return true;
}


