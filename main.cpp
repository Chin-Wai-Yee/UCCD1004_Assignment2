#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_STUDENTS = 100; // Maximum number of students that can be stored
const int MAX_COURSES = 50; // Maximum number of courses that can be stored

/// @brief Course that provided
struct Course {
    string code; // course code (etc: UCCD1004)
    string name; // course name (etc: Programming Practices)
    string gred; // course gred (etc: A+)
    int credit_hour; // course credit hour (etc: 4)
};

/// @brief Struct representing student
struct Student {
    string id; // Student id
    string name; // Student name
    string password; // Student password in hash
    int course_num; // number of course taken
    Course course[MAX_COURSES]; // Taken courses in array
    int total_credit_hour; // Total credit hour
};

struct Lecturer {
    string id; // Lecturer id
    string name; // Lecturer name
    string password; // Lecturer password in hash
};

// Function declaration

int split(string, char, string []);
void readFile(string, Student[] , int&);
void writeFile(string, Student[], int);
void displayStudentInfo(Student[], int);
void mainMenu(bool&);
void studentMenu();


// Main function
int main() {
    Student students[MAX_STUDENTS]; // Array to store student data
    int student_num;
    readFile("students_list.txt", students, student_num); // Current number of students in the array
    bool exit = false;
    // while (!exit) {
    //     mainMenu(exit);
    //     system("pause");
    // }
    writeFile("students_list_output.txt", students, student_num);

    return 0;
}

void mainMenu(bool& exit) {

    system("cls");
    cout << "Welcome to Student Management System" << endl;
    cout << "Please select your identity" << endl;
    cout << "1. Student" << endl;
    cout << "2. Lecturer" << endl;
    cout << "3. Admin" << endl;
    cout << endl;
    cout << "0. Exit" << endl;

    cout << "Enter your option : ";
    string option;
    getline(cin, option);
    if (option == "1") {
        studentMenu();
    }
    else if (option == "2") {
        // courseMenu();
    }
    else if (option == "3") {
        // adminMenu();
    }
    else if (option == "0") {
        cout << "Exiting..." << endl;
        exit = true;
    }
    else {
        cout << "Invalid option" << endl;
    }
}


void courseMenu(Course courses[], int& course_num, bool is_lecturer = false) {

    system("cls");
    cout << "Course Management System" << endl;
    cout << "Please enter your option" << endl;
    cout << "1. Print course(s) information to file" << endl;

    if (is_lecturer) {
        cout << "2. Add course" << endl;
        cout << "3. Edit course" << endl;
        cout << "4. Delete course" << endl;
    }

    cout << endl;
    cout << "0. Exit" << endl;

    string option;
    getline(cin, option);
    if (option == "1") {
        // printCourseInfo(courses, course_num);
    }
    else if (option == "2") {
        // addCourse(courses, course_num);
    }
    else if (option == "3") {
        // editCourse(courses, course_num);
    }
    else if (option == "0") {
        cout << "Exiting..." << endl;
    }
    else {
        cout << "Invalid option" << endl;
    }

}

void studentMenu() {
    system("cls");
    cout << "Student Management System" << endl;
    cout << "1. Add student" << endl;
    cout << "2. Edit student" << endl;
    cout << "3. Delete student" << endl;
    cout << "4. View student" << endl;
    cout << endl;
    cout << "0. Exit" << endl;
}

/**
 * @brief Split string into substring by delimiter
 * @param str Input string to be split
 * @param delim Delimiter between substring
 * @param substr_arr Output string array that contains the substrings
 * @return Numer of substring
*/
int split(string str, char delim, string substr_arr[]) {

    int n = 0; // number of substring
    int length = str.size(); // length of string
    if (length == 0) {
        return 0; // if the string is empty, return
    }
    else {
        int start = 0, end; // start and end position to get the substr
        do {
            end = str.find(delim, start);
            if (end != -1) {
                substr_arr[n] = str.substr(start, end - start);
            }
            else {
                substr_arr[n] = str.substr(start, length - start +1);
            }
            start = end + 1;
            n++;
        } while(end != -1);
    }

    return n;
}

bool login() {
    return false;
}

/**
 * @brief Read stuents info from students file
 * @param file_name Input file name
 * @param students Output array to store studdents' info
 * @param n Total number of students
 * @param i Total number of courses
 * 
*/
void readFile(string file_name, Student students[], int& n) {
    fstream file;
    n = 0; // number of student
    string courses;
    
    fstream(file_name, ios::app); // if the file does not exist, create it
    file.open(file_name);
    // read student info
    // while there are still student, get the id, name and password
    while (!file.eof()) {
        getline(file, students[n].id, '|');
        getline(file, students[n].name, '|');
        getline(file, students[n].password, '|');
        file >> students[n].course_num;

        // read course info
        getline(file, courses);
        string courses_arr[MAX_COURSES];
        int total_credit_hour = 0;
        for (int j = 0; j < students[n].course_num ; j++) { // loop every course of student
            getline(file, students[n].course[j].code, '|');
            getline(file, students[n].course[j].name, '|');
            getline(file, students[n].course[j].gred);
            students[n].course[j].credit_hour = int(students[n].course[j].code[8]);
            total_credit_hour += students[n].course[j].credit_hour;
        }
        students[n].total_credit_hour = total_credit_hour;
        n++;
    }

    file.close();
}

/**
 * @brief Write students info into targeted file
 * @param file_name Output file to be write
 * @param students Student array to write
 * @param student_num Number of students
*/
void writeFile(string file_name, Student students[], int student_num) {

    fstream file(file_name, ios::out);

    for (int i = 0; i < student_num; i++) {
        // write student info
        file << students[i].id << '|';
        file << students[i].name << '|';
        file << students[i].password << '|';
        file << students[i].course_num;
        //write course info
        for (int j = 0; j < students[i].course_num; j++) {
            file << endl;
            file << students[i].course[j].code << '|';
            file << students[i].course[j].name << '|';
            file << students[i].course[j].gred;
        }
        // end of the student & course info

        // if not last student, write new line char
        if (i != student_num -1){
            file << endl;
        }
    }
    file.close();
}
