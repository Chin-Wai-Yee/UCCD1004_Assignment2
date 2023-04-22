//========================================================================================================================//
// Created by: Ernest Tan
//           : Jean Kwan
//           : Chin
//========================================================================================================================//


// Include basic library
#include <iostream>
#include <fstream>
#include <string>

// Library for decoration purpose
#include <iomanip>

using namespace std;

//========================================================================================================================//
// Global variables

const int MAX_STUDENTS = 100; // Maximum number of students that can be stored
const int MAX_COURSES = 10; // Maximum number of courses that can be stored
const int MAX_TRIMESTER = 15; // Maximum number of trimester that can be stored
const int MAX_LECTURERS = 30; // Maximum number of lecturers that can be stored
const int MAX_NAME_LENGTH = 60; // Maximum length of name
const int SCREEN_WIDTH = 150; // Width of the screen

//========================================================================================================================//
// Declaration of structs

// Gred list
struct Gred {
    string gred;
    double gpa;
} gred_list[] = {
    {"A+", 4.00},
    {"A" , 4.00},
    {"A-", 3.67},
    {"B+", 3.33},
    {"B" , 3.00},
    {"B-", 2.67},
    {"C+", 2.33},
    {"C" , 2.00},
    {"F" , 0.00}
};

/// @brief Course that provided
struct Course {
    string code; // course code (etc: UCCD1004)
    string name; // course name (etc: Programming Practices)
    string gred; // course gred (etc: A+)
    int credit_hour; // course credit hour (etc: 4)
    double gpa; // course gpa (etc: 4.00)

    // constructor
    Course () {
        credit_hour = 0;
        gpa = 0; // Initialize to 0
    }

    void print() {
        cout << code << "\t" << setw(60) << left << name << "\t" << gred << "\t" << right << credit_hour << "\t";
    }

    void updateCreditHour () {
        credit_hour = stoi(code.substr(7, 1));
    }

    // update gpa
    void updateGpa () {
        gpa = gred_list[getGpa(gred)];
    }

    // just combine two function into one
    void updateAll () {
        updateCreditHour();
        updateGpa();
    }


};

struct Trimester {
    Course course[MAX_COURSES]; // Array to store course data
    int course_num; // Number of course taken in each trimester
    int total_credit_hour; // Total credit hour of the trimester
    double total_gpa; // total gpa of the trimester
    double cgpa; // CGPA of the trimester

    // constructor
    Trimester () {
        course_num = 0; // Initialize to 0
        total_gpa = 0; // Initialize to 0
        cgpa = 0; // Initialize to 0
        total_credit_hour = 0; // Initialize to 0
    }

    // print all the courses in the trimester
    void print() {
        for (int i = 0; i < course_num; i++) {
            cout << i+1 << "\t";
            course[i].print();
            cout << endl;
        }
        cout << setfill('-') << setw(SCREEN_WIDTH) << "" << endl;
        cout << "Total credit hour : " << setw(6) << right << total_credit_hour << endl;
        cout << "Cgpa              : " << setprecision(4) << fixed << cgpa << endl;
    }

    // update cgpa
    void update() {
        total_gpa = 0;
        total_credit_hour = 0;
        for (int i = 0; i < course_num; i++) {
            total_gpa += course[i].gpa * course[i].credit_hour;
            total_credit_hour += course[i].credit_hour;
        }
        if (course_num == 0) {
            cgpa = 0;
        }
        else {
            cgpa = total_gpa / total_credit_hour;
        }
    }

    int getIndex(string code) {
        for (int i = 0; i < course_num; i++) {
            if (course[i].code == code) {
                return i;
            }
        }
        return -1;
    }
};

/// @brief Struct representing identity, including students and lecturers
struct People {
    string id; // Student id
    string name; // Student name
    string password; // Student password in plain text
    Trimester trimester[MAX_TRIMESTER]; // Array to store trimester data
    int trimester_num; // Number of trimester taken
    int total_credit_hour; // Total credit hour
    double cgpa; // CGPA of the student

    // constrcutor
    People () {
        // Initialize to empty string
        id = "";
        name = "";
        password = "";
        trimester_num = 0; // Initialize to 0
        total_credit_hour = 0; // Initialize to 0
        cgpa = 0; // Initialize to 0
    }

    // print the people info
    void print(bool show_cgpa = false) {
        cout << id << " \t" << setw(60) << left << name;
        if (show_cgpa) {
            cout << "\t" << cgpa;
        }
    }

    // update the cgpa and total credit hour of the student
    void update() {
        total_credit_hour = 0;
        cgpa = 0;
        for (int i = 0; i < trimester_num; i++) {
            cgpa += trimester[i].total_gpa;
            total_credit_hour += trimester[i].total_credit_hour;
        }
        if (total_credit_hour == 0) {
            cgpa = 0;
        }
        else {
            cgpa /= total_credit_hour;
        }
    }

    int getIndex(string code) {
        int index;
        for (int i = 0; i < trimester_num; i++) {
            index = trimester[i].getIndex(code);
            if (index != -1) {
                return i * MAX_COURSES + index;
            }
        }
        return -1;
    }

    void addCourseInfo(string prompt, bool update = false)
    {
        string prompt = " (0 - Exit";
        if (update)
        {
            prompt += " <Enter> - Keep original value";
        }
        prompt += ") : ";
        string headear = "";

        bool loop = true;
        int newIndex, trimester, year;
        int step = 1;
        do
        {
            system("cls");
            cout << header;

            if (step == 1)
            {
                string course_code;
                bool valid = getCourseCode("Enter course code" + prompt, course_code, loop, update);
                newIndex = getIndex(course_code);
                if (valid && update)
                {
                    if (newIndex == -1)
                    {
                        cout << "Course code not found!" << endl;
                        cout << "Please enter again!" << endl;
                    }
                    else if (getCoruseCode("Enter new course code" + prompt, course_code, loop, update))
                    {
                        newIndex = getIndex(course_code);
                        if (newIndex != -1)
                        {
                            cout << "Course code already exist!" << endl;
                            cout << "Please enter again!" << endl;
                        }
                        else
                        {
                            trimester = newIndex % MAX_COURSES;
                            newIndex /= MAX_COURSES;
                            if (course_code == "")
                            {
                                course_code = trimester[trimester].course[newIndex].code;
                            }
                            header += "Course code : " + course_code + "\n";
                            step++;
                        }
                    }
                }
                else if (valid && newIndex != -1)
                {
                    cout << "Course code already exist!" << endl;
                    cout << "Please enter again!" << endl;
                }
                else if (valid)
                {
                    header += "Course code : " + course_code + "\n";
                    step++;
                }
            }
            else if (step == 2)
            {
                string course_name;
                if (getCourseName("Enter course name" + prompt, course_name, loop, update))
                {
                    if (course_name == "")
                    {
                        course_name = trimester[trimester].course[newIndex].name;
                    }
                    header += "Course name : " + course_name + "\n";
                    step++;
                }
            }
            else if (step == 3)
            {
                string gred;
                getGred("Enter gred" + prompt, gred, loop, update);
                if (getGpa(gred) != -1)
                {
                    if (update)
                    {
                        step += 2; // Skip study year and trimester
                        if (gred == "")
                        {
                            gred = trimester[trimester].course[newIndex].gred;
                        }
                    }
                    header += "Course Gred : " + gred + "\n";
                    step++;
                }
            }
            else if (step == 4)
            {
                if (getNum("Enter study year" + prompt, year, 1, MAX_TRIMESTER/3, loop))
                {
                    header += "Study year  : " + to_string(year) + "\n";
                    step++;
                }
            }
            else if (step == 5)
            {
                if(getNum("Enter trimester number" + prompt, trimester, 1, 3, loop))
                {
                    trimester = (year - 1) * 3 + trimester - 1;
                    if (trimester > trimester_num + 1)
                    {
                        cout << "Out of range..." << endl;
                        cout << "Please enter again..." << endl;
                    }
                    else
                    {
                        newIndex = trimester[trimester].course_num;
                        header += "Trimester   : " + to_string(trimester) + "\n";
                        trimester[trimester].course_num++;
                        if (trimester > trimester_num)
                        {
                            trimester_num = trimester;
                        }
                        step++;
                    }
                }
            }
            else if (step == 6)
            {
                trimester[trimester].course[newIndex] = {
                    course_code,
                    course_name,
                    gred,
                };
                trimester[trimester].course[newIndex].update();
                trimester[trimester].update();
                update();
                loop = false;
            }
        } while (loop);
    }

    void deleteCourse()
    {
        bool loop = true;
        do
        {
            system("cls");
            string input;
            if (getCoruseCode("Enter course code of course to be delete (0 - Exit): ", input, loop))
            {
                cout << endl;
                int i = getIndex(input);
                if (i == -1)
                {
                    cout << "Course not found!" << endl;
                    cout << "Please enter agian!" << endl;
                }
                else if () {
                    
                }
                else
                {
                    // double comfirm
                    cout << "Course code : " << course[i].code << endl;
                    cout << "Course name : " << course[i].name << endl;
                    cout << "Gred        : " << course[i].gred << endl;
                    cout << "GPA         : " << course[i].gpa << endl;
                    if (confirm("Are you sure to delete this course? (Y/N) : ", loop))
                    {
                        // delete course
                        for (; i < newIndex - 1; i++)
                        {
                            course[i] = course[i + 1];
                        }
                        newIndex--;
                        cout << "Course deleted!" << endl;
                    }
                }
            }
            system("pause");
        } while (loop);
    }

};

People students[MAX_STUDENTS]; // Array to store People data
People lecturers[MAX_LECTURERS]; // Array to store lecturer data

//========================================================================================================================//
// Function declaration

void   readFile(string, People[], int&);
void   readCourseFile(string, People[], int);
void   writeFile(string, People[], int);
void   writeCourseFile(string, People[], int);
void   mainMenu(People[], People[], int&, int&, bool&);
void   courseMenu(People, Course[], bool = false);
void   studentMenu(People[], int&);
void   lecturerMenu(People[], int&, bool&);
void   adminMenu(People[], int&, People[], int&);
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

//========================================================================================================================//
// Main function
int main() {
    system("chcp 65001>nul");
    // Read file and get current number of students in the array
    int student_num, lecturer_num;
    readFile("students_list.txt", students, student_num);
    readFile("lecturer_list.txt", lecturers, lecturer_num);
    readCourseFile("course_list.txt", students, student_num);
    // Start the program

    bool exit = false;
    // while (!exit) {
    //    mainMenu(students, lecturers, student_num, lecturer_num, exit);
    // }
    writeFile("students_list_output.txt", students, student_num);
    writeFile("lecturer_list_output.txt", lecturers, lecturer_num);
    writeCourseFile("course_list_output.txt", students, student_num);
    return 0;
}

//========================================================================================================================//
// File handling related:

/**
 * @brief Read people info from targeted file
 * @param file_name Input file to be read
 * @param people The array to store data
 * @param count The number of people in the array
 * @param isStudent Differentiate between student and other identity, default to false
 * @param student_file_name Student file name, default to empty string
*/
void readFile(string file_name, People people[], int& count) {
    
    // if the file does not exist create it, else work with it
    fstream file(file_name, ios::app | ios::in);

    // start reading
    // get the id, name and password
    for (count = 0; !file.eof(); count++) {
        getline(file, people[count].id, '|');
        getline(file, people[count].name, '|');
        getline(file, people[count].password);
    }
    // done reading people info
    file.close();
}

/**
 * @brief Read course info from targeted file
 * @param file_name Input file to be read
 * @param student The student to store data
 * @param course The array to store data
*/
void readCourseFile(string file_name, People student[], int stu_num) {

    fstream file(file_name, ios::app | ios::in); // open file if exist, else create it

    // loop every student
    for (int i = 0; i < stu_num; i++) {
        file >> student[i].trimester_num;
        for (int j = 0; j < student[i].trimester_num; j++) {
            file >> student[i].trimester[j].course_num;
            file.ignore();
            for (int k = 0; k < student[i].trimester[j].course_num; k++) {
                getline(file, student[i].trimester[j].course[k].code, '|');
                getline(file, student[i].trimester[j].course[k].name, '|');
                getline(file, student[i].trimester[j].course[k].gred);
                student[i].trimester[j].course[k].updateAll();
            }
            student[i].trimester[j].update();
        }
        student[i].update();
    }

    file.close();
}

/**
 * @brief Write people info to targeted file
 * @param file_name Output file to be written
 * @param people The array to store data
 * @param count The number of people in the array
 * @param isStudent Differentiate between student and other identity, default to false
 * @param student_file_name Student file name, default to empty string
*/
void writeFile(string file_name, People people[], int count) {

    fstream file(file_name, ios::out);

    for (int i = 0; i < count; i++) {
        // write people info
        file << people[i].id << '|';
        file << people[i].name << '|';
        file << people[i].password;
        // if not last people, write new line
        if (i != count - 1){
            file << endl;
        }
    }
    file.close();
}

void writeCourseFile(string file_name, People student[], int count){

    fstream file(file_name, ios::out);

    for (int i = 0; i < count; i++) {
        file << student[i].trimester_num;
        //write course info
        for (int j = 0; j < student[i].trimester_num; j++) {
            file << endl;
            file << student[i].trimester[j].course_num;
            for (int k = 0; k < student[i].trimester[j].course_num; k++) {
                file << endl;
                file << student[i].trimester[j].course[k].code << '|';
                file << student[i].trimester[j].course[k].name << '|';
                file << student[i].trimester[j].course[k].gred;
            }
        }
        if (i != student[i].trimester_num - 1) {
            file << endl;
        }
    }
    file.close();
}

//========================================================================================================================//
// // Menu related:

// void mainMenu(People students[], People lecturers[], int& student_num, int& lecturer_num, bool& exit) {

//     system("cls");
//     cout << "Welcome to Student Management System" << endl;
//     cout << "Please select your identity" << endl;
//     cout << endl;
//     cout << "1. Student" << endl;
//     cout << "2. Lecturer" << endl;
//     cout << "3. Admin" << endl;
//     cout << endl;
//     cout << "0. Exit" << endl;
//     cout << endl;
//     cout << "Enter your option : ";
//     string option;
//     getline(cin, option);
//     int i;
//     if (option == "1") {
//         if (login(students, student_num, i)) {
//             courseMenu(students[i], students[i].course);
//         }
//     }
//     else if (option == "2") {
//         if (login(lecturers, lecturer_num, i)) {
//             studentMenu(students, student_num);
//         }
//     }
//     else if (option == "3") {
//         cout << "Enter admin password : ";
//         string password;
//         getline(cin, password);
//         if (password == "admin") {
//             adminMenu(students, student_num, lecturers, lecturer_num);
//         }
//         else {
//             cout << "Invalid password" << endl;
//             system("pause");
//         }
//     }
//     else if (option == "0") {
//         cout << "Exiting..." << endl;
//         exit = true;
//     }
//     else {
//         cout << "Invalid option" << endl;
//     }
//     system("pause");
// }

// /**
//  * @brief Show info of stuent's course
//  * @param student The student to show course info
//  * @param courses The array of courses
//  * @param is_lecturer More features for lecturer, default to false
// */
// void courseMenu(People student, Course courses[][MAX_COURSES], bool is_lecturer) {
//     int course_num = student.course_num;
//     bool loop = true;
//     while (loop) {
//         system("cls");
//         cout << "Course Management System" << endl;
//         cout << endl;
//         view(student, courses);
//         cout << endl;

//         cout << "1. Print course(s) information to file" << endl;
//         if (is_lecturer) {
//             cout << "2. Add course" << endl;
//             cout << "3. Edit course" << endl;
//             cout << "4. Delete course" << endl;
//         }
//         cout << endl;

//         cout << "0. Exit" << endl;
//         cout << endl;

//         cout << "Please enter your option : ";
//         string option;
//         getline(cin, option);
//         if (option == "1") {
//             cout << "The function is not ready yet~";
//         }
//         else if (is_lecturer) {
//             if (option == "2") {
//                 addCourseInfo(courses, course_num, "Do you want to add a new course");
//             }
//             else if (option == "3") {
//                 addCourseInfo(courses, course_num, "Do you want to edit a course", true);
//             }
//             else if (option == "4") {
//                 deleteCourse(courses, course_num);
//             }
//         }
//         else if (option == "0") {
//             cout << "Exiting..." << endl;
//             loop = false;
//         }
//         else {
//             cout << "Invalid option" << endl;
//         }
//         system("pause");
//     }
// }

// void studentMenu(People students[], int& student_num) {

//     bool loop = true;
//     while (loop) {
//         system("cls");
//         cout << "Student Management System" << endl;
//         cout << endl;
//         view(students, student_num, "Student", true);
//         cout << endl;
//         cout << "1. Add student" << endl;
//         cout << "2. Edit student" << endl;
//         cout << "3. Delete student" << endl;
//         cout << "4. View student" << endl;
//         cout << endl;
//         cout << "0. Exit" << endl;
//         cout << endl;

//         cout << "Please enter your option : ";
//         string option;
//         getline(cin, option);
//         if (option == "1") {
//             modifyPeopleInfo(students, "Do you want to add a new student", student_num);
//         }
//         else if (option == "2") {
//             modifyPeopleInfo(students, "Do you want to edit a student", student_num, true);
//         }
//         else if (option == "3") {
//             deletePeople(students, student_num);
//         }
//         else if (option == "4") {
//             cout << "Please enter the student ID : ";
//             string id;
//             getline(cin, id);
//             int i = getIndex(students, id, student_num);
//             if (i != -1) {
//                 courseMenu(students[i], students[i].course, true);
//             }
//             else {
//                 cout << "Student not found, please try again." << endl;
//             }
//         }
//         else if (option == "0") {
//             cout << "Exiting..." << endl;
//             loop = false;
//         }
//         else {
//             cout << "Invalid option. Please try again." << endl;
//         }
//         system("pause");
//     }
// }

// void lecturerMenu(People lecturers[], int& lecturer_num, bool& exit) {
    
//     system("cls");
//     cout << "Lecturer Management System" << endl;
//     cout << endl;
//     cout << "1. Add lecturer" << endl;
//     cout << "2. Edit lecturer" << endl;
//     cout << "3. Delete lecturer" << endl;
//     cout << "4. View lecturer" << endl;
//     cout << endl;
//     cout << "0. Exit" << endl;
//     cout << endl;

//     cout << "This fucntion is not ready yet!" << endl;
//     system("pause");
// }

// void adminMenu(People students[], int& student_num, People lecturers[], int& lecturer_num) {
//     bool loop = true;
//     while (loop) {
//         system("cls");
//         cout << "Admin Management System" << endl;
//         cout << endl;
//         cout << "1. Student Management" << endl;
//         cout << "2. Lecturer Management" << endl;
//         cout << endl;
//         cout << "0. Exit" << endl;
//         cout << endl;

//         cout << "Please enter your option : ";
//         string option;
//         getline(cin, option);
//         if (option == "1") {
//             studentMenu(students, student_num);
//         }
//         else if (option == "2") {
//             lecturerMenu(lecturers, lecturer_num, loop);
//         }
//         else if (option == "0") {
//             cout << "Exiting..." << endl;
//             loop = false;
//         }
//         else {
//             cout << "Invalid option. Please try again." << endl;
//         }
//         system("pause");
//     }
// }

//========================================================================================================================//
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
bool testName(string input, int max_size = MAX_NAME_LENGTH)
{
    if (input == "" || input.size() > max_size)
    {
        return false;
    }
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

// int searchEngine(string prompt, People students[], int student_num, int result[])
// {
//     int search_result_num = 0;

//     cout << "Please enter " << prompt << " : ";
//     string search;
//     getline(cin, search);
//     search = toUpper(search);

//     for (int i = 0; i < student_num; i++)
//     {
//         if (toUpper(students[i].name).find(search) != string::npos ||
//             toUpper(students[i].id).find(search) != string::npos)
//         {
//             result[search_result_num] = i;
//             search_result_num++;
//         }
//     }
//     return search_result_num;
// }

/**
 * @brief Get the index of a student
 * @param Students Array of students
 * @param id ID of the student
 * @param student_num Number of students
 * @return Index of the student, -1 if not found
*/
int getIndex(People Students[], int student_num, string id) {
    for (int i = 0; i < student_num; i++) {
        if (Students[i].id == id) {
            return i;
        }
    }
    return -1;
}

int getGpa(string gred) {
    for (int i = 0; i < 9; i++) {
        if (gred == gred_list[i].gred) {
            return i;
        }
    }
    return -1;
}

//========================================================================================================================//

// // Input related

bool getNum (string prompt, int& output, int size, int limit, bool& loop = true) {
    cout << prompt;
    string input;
    getline(cin, input);
    if (input == "0") {
        loop = false;
        return false;
    }
    else if (input.size() > size) {
        cout << "Invalid input, please try again." << endl;
        return false;
    }
    else {
        for (int i = 0; i < input.size(); i++) {
            if (!isdigit(input[i])) {
                cout << "Invalid input, please try again." << endl;
                return false;
            }
        }
        output = stoi(input);
        if (output < 1 || output > limit) {
            cout << "Invalid input, please try again." << endl;
            return false;
        }
    }
    return true;
}

bool getCoruseCode (string prompt, string& output, bool& loop = true, bool accept_empty = false) {
    cout << prompt;
    getline(cin, code);
    if (code == "0") {
        loop = false;
        return false;
    }
    else if (testCourseCode(code) || (accept_empty && code == "")) {
        return true;
    }
    else {
        cout << "That is not the proper format of the course code." << endl;
        cout << "Here is an example : LLLLNNNN" << endl;
        cout << "(L = letters and N = numbers)" << endl;
        return false;
    }
}

bool getCourseName (string prompt, string& output, bool& loop = true, bool accept_empty = false) {
    cout << prompt;
    getline(cin, name);
    if (name == "0") {
        loop = false;
        return false;
    }
    else if (testName(name) || (accept_empty && name == "")) {
        return true;
    }
    else {
        cout << "That is not the proper format of the course name." << endl;
        cout << "Here is an example : Introduction to Programming " << endl;
        cout << "(Only alphabets and spaces are allowed)" << endl;
        return false;
    }
}

bool getGred (string prompt, string& output, bool& loop = true, bool accept_empty = false) {
    cout << prompt;
    getline(cin, gred);
    if (gred == "0") {
        loop = false;
        return false;
    }
    else if (getGpa(gred) != -1, || (accept_empty && gred == "")) {
        return true;
    }
    else {
        cout << "That is not the proper format of the gred." << endl;
        cout << "Here is an example : A+ " << endl;
        cout << "Please enter again!" << endl;
        return false;
    }
}

bool confirm(string prompt, bool& loop) {
    loop = false;
    cout << prompt;
    string input;
    getline(cin, input);
    input = toUpper(input);
    if (input == "Y") {
        return true;
    }
    else if (input == "N") {
        return false;
    }
    else {
        cout << "Invalid input, please try again." << endl;
        loop = true;
        return false;
    }
}


// ========================================================================================================================//

// CRUD related

// People related

/**
 * @brief Add / Update people
 * @param people Array of people
 * @param messages Message to be displayed
 * @param newIndex Index of the people
 * @param update If true, update the people, else add a new people, default is false
*/
void modifyPeopleInfo(People people[], int& newIndex, bool update)
{
	int loop = 0, i = newIndex;
	while (loop >= 0)
	{
		loop++;
		string input;
		while (loop == 1)
		{
			cout << "Please enter ID : ";
			string id;
			getline(cin, id);
			if (update)
			{
				i = getIndex(people, newIndex, id);
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

		while (loop == 2)
		{
			cout << "Student's name : ";
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
	cout << "Please enter ID : ";
	getline(cin, id);
	cout << endl;

    int i = getIndex(people, newIndex, id);
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


//========================================================================================================================//
// Output related

/**
 * @brief View student information
 * @param student Student information
 * @param course Array of course
*/
void view(Course course[], int size)
{

    // display all the courses
    for (int i = 0; i < size; i++)
    {
        cout << i + 1 << "\t";
        cout << course[i].code << "\t";
        cout << setw(60) << left << course[i].name;
        cout << course[i].credit_hour << "\t";
        cout << course[i].gred << endl;
    }
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
    // header
    cout << "No. \t";
    cout << type << " ID \t";
    cout << setw(60) << left << type + " name\t";
    if (show_cgpa)
    {
        cout << "CGPA";
    }
    cout << endl;
    cout << setfill('-') << setw(SCREEN_WIDTH) << "" << endl;

    // body
    for (int i = 0; i < size; i++)
    {
        cout << setw(3) << right << i + 1 << " \t";
        people[i].print(show_cgpa);
        cout << endl;
    }

    if (size == 0)
    {
        cout << "No data found." << endl;
    }
    
    cout << setfill('-') << setw(SCREEN_WIDTH) << "" << endl;
}

//========================================================================================================================//
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
    int i = getIndex(people, size, id);
    if (i == -1) {
        cout << "ID not found. Please try again." << endl;
        return false;
    }
    else {
        if (people[i].password == "") {
            return true;
        }
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


