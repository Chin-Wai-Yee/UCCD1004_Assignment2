#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_STUDENTS = 100; // Maximum number of students that can be stored
const int MAX_COURSES = 50; // Maximum number of courses that can be stored
const int MAX_LECTURERS = 50; // Maximum number of lecturers that can be stored

/// @brief Course that provided
struct Course {
    string code; // course code (etc: UCCD1004)
    string name; // course name (etc: Programming Practices)
    string gred; // course gred (etc: A+)
    int credit_hour; // course credit hour (etc: 4)
};

/// @brief Struct representing identity, including students and lecturers
struct People {
    string id; // Student id
    string name; // Student name
    string password; // Student password in hash
    int course_num; // number of course taken
    Course course[MAX_COURSES]; // Taken courses in array
    int total_credit_hour; // Total credit hour
};

// Function declaration

void readFile(string, People[], int&, bool);
void writeFile(string, People[], int, bool);
void displayStudentInfo(People[], int);
void mainMenu(bool&);
void studentMenu();


// Main function
int main() {
    People students[MAX_STUDENTS]; // Array to store People data
    People lecturers[MAX_LECTURERS]; // Array to store lecturer data
    // Read file and get current number of students in the array
    int student_num, lecturer_num;
    readFile("students_list.txt", students, student_num, true);
    readFile("lecturer_list.txt", lecturers, lecturer_num, false);
    for (int i = 0; i < student_num; i++) {
        cout << students[i].name << endl;
        cout << students[i].total_credit_hour << endl;
    }
    bool exit = false;
    // while (!exit) {
    //     mainMenu(exit);
    //     system("pause");
    // }
    writeFile("students_list_output.txt", students, student_num, true);
    writeFile("lecturer_list_output.txt", lecturers, lecturer_num, false);
    return 0;
}

/**
 * @brief Read people info from targeted file
 * @param file_name Input file to be read
 * @param people The array to store data
 * @param count The number of people in the array
 * @param isStudent Differentiate between student and other identity
*/
void readFile(string file_name, People people[], int& count, bool isStudent = false) {
    
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
            people[count].total_credit_hour = 0;
            // loop every course of student and read the info
            for (int j = 0; j < people[count].course_num ; j++) {
                getline(file, people[count].course[j].code, '|');
                getline(file, people[count].course[j].name, '|');
                getline(file, people[count].course[j].gred);
                cout << people[count].course[j].code << endl;
                people[count].course[j].credit_hour = stoi(people[count].course[j].code.substr(7, 1));
                cout << people[count].course[j].credit_hour << endl;
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
 * @param student_num Number of students
*/
void writeFile(string file_name, People people[], int count, bool isStudent = false) {

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
 * @brief Verify the login
 * @param password Correct password
 * @return True if the password is correct, false otherwise
*/
bool login(string password) {
    
    string input;
    cout << "Please enter your password : ";
    getline(cin, input);

    if (input == password) {
        return true;
    }
    else {
        cout << "Wrong password" << endl;
        return false;
    }

}