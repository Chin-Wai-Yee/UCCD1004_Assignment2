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
#include <windows.h>

using namespace std;

//========================================================================================================================//
// Global variables

const int MAX_STUDENTS = 500; // Maximum number of students that can be stored
const int MAX_COURSES = 10; // Maximum number of courses that can be stored
const int MAX_TRIMESTER = 15; // Maximum number of trimester that can be stored
const int MAX_LECTURERS = 50; // Maximum number of lecturers that can be stored
const int MAX_NAME_LENGTH = 60; // Maximum length of name
const int SCREEN_WIDTH = 150; // Width of the screen
const int MAX_ROW = 10; // Maximum number of name per page

int    getGpa(string);
bool   getNum (string, int&, int, int, bool&);
bool   getCourseCode (string, string&, bool&, bool accept_empty = false);
bool   getCourseName (string, string&, bool&, bool accept_empty = false);
bool   getGred (string, string&, bool&, bool accept_empty = false);
bool   getPassword (string, string&);
bool   confirm(string, bool&);
void   readBanner(string, string&);
int    roundUp(int, int);

//========================================================================================================================//
// For decoration purpose, banner in string format
string admin_manage, course_manage, lecturer_manage, search_result, student_manage, welcome, report;

//========================================================================================================================//
// Declaration of structs

// Gred list
struct {
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
        cout << code << "\t" << setw(60) << left << name << "\t" << gred << "\t" << right << credit_hour;
    }

    void printFile(string filename) {
        fstream file(filename, ios::app);
        file << code << " " << setw(60) << left << name << " " << setw(6) << gred << right << setw(6) << credit_hour;
        file.close();
    }

    void updateCreditHour () {
        credit_hour = stoi(code.substr(7, 1));
    }

    // update gpa
    void updateGpa () {
        gpa = gred_list[getGpa(gred)].gpa;
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
            cout << setw(4) << right << i+1 << ".  ";
            course[i].print();
            cout << endl;
        }
        cout << setfill('-') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
        cout << "  Total credit hour : " << setw(6) << right << total_credit_hour << endl;
        cout << "  Cgpa              : " << setprecision(4) << fixed << cgpa << endl;
    }

    void printFile(string filename) {
        fstream file(filename, ios::app);
        for (int i = 0; i < course_num; i++) {
        // Using flush to write to file immediately
        file << setw(4) << right << i+1 << ".  " << flush;
        course[i].printFile(filename);
        file << endl;
        }
        file << setfill('-') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
        file << "  Total credit hour : " << setw(6) << right << total_credit_hour << endl;
        file << "  Cgpa              : " << setprecision(4) << fixed << cgpa << endl;
        file.close();
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
    int course_num; // Number of course taken
    int total_credit_hour; // Total credit hour
    double cgpa; // CGPA of the student

    // constrcutor
    People () {
        // Initialize to empty string
        id = "";
        name = "";
        password = "";
        trimester_num = 0; // Initialize to 0
        course_num = 0; // Initialize to 0
        total_credit_hour = 0; // Initialize to 0
        cgpa = 0; // Initialize to 0
    }

    // print the people info
    void print(bool show_cgpa = false, bool show_password = false) {
        cout << setw(15) << left << id << setw(60) << left << name;
        if (show_cgpa) {
            cout << setw(10) << left << setprecision(4) << fixed << cgpa;
        }
        if (show_password) {
            cout << password;
        }
    }


    void printStudent(int page = 1) {
        cout << course_manage;
        cout << setfill('-') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
        cout << "  Student ID : " << id << endl;
        cout << "  Name       : " << name << endl;
        cout << setfill('-') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;

        int low = (page - 1) * 3;
        int high = page * 3;
        if (high > trimester_num) {
            high = trimester_num;
        }
        for (int i = low; i < high; i++) {
            cout << "  Year " << page << " Trimester " << i % 3 + 1 << endl;
            cout << setfill('=') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
            cout << "  No.  Code\t" << setw(60) << left << "Course name" << "\tGred" << "\tCredit Hour" << endl;
            cout << setfill('=') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
            trimester[i].print();
            cout << setfill('-') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
            cout << endl;
        }


        cout << "  Total number of courses taken : " << setw(6) << right << course_num << endl;
        cout << "  Total credit hour earned      : " << setw(6) << right << total_credit_hour << endl;
        cout << "  Cgpa                          : " << setprecision(4) << fixed << cgpa << endl;
        cout << endl;
    }

    void printFile(string filename) {
        fstream temp(filename, ios::out);
        temp.close();
        fstream file(filename, ios::app);
        file << course_manage;
        file << setfill('=') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
        file << "  Student ID : " << id << endl;
        file << "  Name       : " << name << endl;
        file << setfill('=') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
        file << endl;

        for (int i = 0; i < trimester_num; i++) {
            file << "  Year " << i/3 + 1 << " Trimester " << i % 3 + 1 << endl;
            file << setfill('=') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
            file << setw(15) << left << "  No.  Code" << setw(60) << " Course name" << " Gred " << " Credit Hour" << endl;
            file << setfill('=') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
            trimester[i].printFile(filename);
            file << setfill('-') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
            file << endl;
        }

        file << "  Total number of courses taken : " << setw(6) << right << course_num << endl;
        file << "  Total credit hour earned      : " << setw(6) << right << total_credit_hour << endl;
        file << "  Cgpa                          : " << setprecision(4) << fixed << cgpa << endl;
        file << endl;
        file.close();
    }

    string status () {
        if (cgpa >= 3.85) {
            return "President's List";
        }
        else if (cgpa >= 3.6) {
            return "Dean's List";
        }
        else if (cgpa < 2 && course_num != 0) {
            return "Probation";
        }
        else {
            return "Normal";
        }
    }

    // update the cgpa and total credit hour of the student
    void update() {
        total_credit_hour = 0;
        cgpa = 0;
        course_num = 0;
        trimester_num = 0;
        for (int i = 0; i < MAX_TRIMESTER; i++) {
            if (trimester[i].course_num == 0) {
                break;
            }
            trimester_num ++;
            course_num += trimester[i].course_num;
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

    void addCourseInfo(int page, bool update_info = false)
    {
        string prompt = " (0 - Exit";
        if (update_info)
        {
            prompt += " <Enter> - Keep original value";
        }
        prompt += ") : ";
        string header = "";

        bool loop = true;
        int newIndex, semester, year;
        int step = 1;
        string course_code, course_name, gred, temp;
        do
        {
            system("cls");
            printStudent(page);
            cout << header;
            switch (step)
            {
                case 1:
                    if (getCourseCode("  Enter course code (0 - Exit) : ", course_code, loop, update_info))
                    {
                        newIndex = getIndex(course_code);
                        if (update_info)
                        {
                            if (newIndex == -1)
                            {
                                cout << "  Course code not found!" << endl;
                                cout << "  Please enter again!" << endl;
                                cout << "  "; system("pause");
                            }
                            else
                            {
                                temp = course_code;
                                step++;
                            }
                        }
                        else 
                        {
                            if (newIndex != -1)
                            {
                                cout << "  Course code already exist!" << endl;
                                cout << "  Please enter again!" << endl;
                                cout << "  "; system("pause");
                            }
                            else
                            {
                                header += "  Course code : " + course_code + "\n";
                                step+=2;
                            }
                        }
                    }
                    break;

                case 2:
                    cout << "  Current course code : " << temp << endl;
                    if (getCourseCode("  Enter new course code" + prompt, course_code, loop, update_info))
                    {
                        if (getIndex(course_code) != -1)
                        {
                            cout << "  Course code already exist!" << endl;
                            cout << "  Please enter again!" << endl;
                            cout << "  "; system("pause");
                        }
                        else
                        {
                            semester = newIndex / MAX_COURSES;
                            newIndex %= MAX_COURSES;
                            if (course_code == "")
                            {
                                course_code = trimester[semester].course[newIndex].code;
                            }
                            header += "  Course code : " + course_code + "\n";
                            step++;
                        }
                    }
                    break;

                case 3:
                    if (getCourseName("  Enter course name" + prompt, course_name, loop, update_info))
                    {
                        if (course_name == "")
                        {
                            course_name = trimester[semester].course[newIndex].name;
                        }
                        header += "  Course name : " + course_name + "\n";
                        step++;
                    }
                    break;
                
                case 4:
                    if (getGred("  Enter gred" + prompt, gred, loop, update_info))
                    {
                        if (update_info)
                        {
                            step += 2; // Skip study year and trimester
                            if (gred == "")
                            {
                                gred = trimester[semester].course[newIndex].gred;
                            }
                        }
                        header += "  Course Gred : " + gred + "\n";
                        step++;
                    }
                    break;
                
                case 5:
                    if (getNum("  Enter study year" + prompt, year, 1, MAX_TRIMESTER/3, loop))
                    {
                        header += "  Study year  : " + to_string(year) + "\n";
                        step++;
                    }
                    break;

                case 6:
                    if(getNum("  Enter trimester number" + prompt, semester, 1, 3, loop))
                    {
                        semester = (year - 1) * 3 + semester - 1;
                        if (semester != 0 && trimester[semester - 1].course_num == 0)
                        {
                            header = header.substr(0, header.size() - 18);
                            cout << "  Please enter the course information for the previous trimester first." << endl;
                            cout << "  Please enter again..." << endl;
                            step--;
                            cout << "  "; system("pause");
                        }
                        else if (trimester[semester].course_num == MAX_COURSES)
                        {
                            header = header.substr(0, header.size() - 18);
                            cout << "  Maximum number of courses reached..." << endl;
                            cout << "  Please enter again..." << endl;
                            step--;
                            cout << "  "; system("pause");
                        }
                        else
                        {
                            newIndex = trimester[semester].course_num;
                            step++;
                        }
                    }
                    break;

                case 7:
                    if (!update_info)
                    {
                        if (semester > trimester_num)
                        {
                            trimester_num = semester;
                        }
                        trimester[semester].course_num++;
                    }
                    trimester[semester].course[newIndex].name = course_name;
                    trimester[semester].course[newIndex].code = course_code;
                    trimester[semester].course[newIndex].gred = gred;
                    trimester[semester].course[newIndex].updateAll();
                    trimester[semester].update();
                    update();
                    system("cls");
                    printStudent(page);
                    cout << "  Course information successfully updated !" << endl;
                    loop = false;
                    cout << "  "; system("pause");
                    break;
            }
        } while (loop);
    }

    void deleteCourse(int page)
    {
        int sem, i;
        bool loop = true;
        do
        {
            system("cls");
            printStudent(page);
            string input;
            if (getCourseCode("  Enter course code of course to be delete (0 - Exit): ", input, loop))
            {
                cout << endl;
                i = getIndex(input);
                sem = i / MAX_COURSES;
                if (i == -1)
                {
                    cout << "  Course not found!" << endl;
                    cout << "  Please enter agian!" << endl;
                    cout << "  "; system("pause");
                }
                else if (sem != trimester_num - 1 && trimester[sem + 1].course_num == 0 && trimester[sem].course_num == 1)
                {
                    cout << "  Please delete the course in the next trimester first!" << endl;
                    cout << "  "; system("pause");
                }
                else
                {
                    i %= MAX_COURSES;
                    // double comfirm
                    while (loop)
                    {
                        system("cls");
                        printStudent(page);
                        cout << "  \033[4mCourse Information\033[0m" << endl;
                        cout << "  Course code : " << trimester[sem].course[i].code << endl;
                        cout << "  Course name : " << trimester[sem].course[i].name << endl;
                        cout << "  Gred        : " << trimester[sem].course[i].gred << endl;
                        cout << "  GPA         : " << trimester[sem].course[i].gpa << endl;
                        if (confirm("  Are you sure to delete this course? (Y/N) : ", loop))
                        {
                            // delete course
                            for (; i < trimester[sem].course_num - 1; i++)
                            {
                                trimester[sem].course[i] = trimester[sem].course[i + 1];
                            }
                            trimester[sem].course_num--;
                            trimester[sem].course[i].updateAll();
                            trimester[sem].update();
                            update();
                            
                            system("cls");
                            printStudent(page);
                            cout << "  Course deleted!" << endl;
                            cout << "  "; system("pause");
                        }
                    }
                }
            }
        } while (loop);
    }

    void editPassword(int page)
    {
        int step = 1;
        if (password == "")
        {
            step ++;
        }
        bool loop = true;
        string input;
        while (loop)
        {
            system("cls");
            printStudent(page);
            if (step == 1)
            {
                cout << "  Please enter your original password : ";
                getline(cin, input);
                if (input == password)
                {
                    step++;
                }
                else
                {
                    loop = false;
                    cout << "  Wrong password, back to menu..." << endl;
                    cout << "  "; system("pause");
                }
            }
            else if (step == 2)
            {
                if(getPassword("  Please enter a new password : ", input))
                {
                    step++;
                }
            }
            else if (step == 3)
            {
                password = input;
                cout << "  Password successfully changed!" << endl;
                loop = false;
                cout << "  "; system("pause");
            }
        }

    }
};

//========================================================================================================================//
// Function declaration

void   readFile(string, People[], int&);
void   readCourseFile(string, People[], int);
void   writeFile(string, People[], int);
void   writeCourseFile(string, People[], int);
void   mainMenu(People[], People[], int&, int&, bool&);
void   courseMenu(People&, bool is_lecturer = false);
void   peopleMenu(People[], int&, string, bool admin = false);
void   adminMenu(People[], int&, People[], int&);
bool   testStudentId(string);
bool   testName(string, int max_size = MAX_NAME_LENGTH);
bool   testCourseCode(string);
string toUpper(string);
void   trim(string&);
int    roundUp(int, int);

int    searchEngine(People[], int, string, int[]);
int    getIndex(People[], int, string);
void   modifyPeopleInfo(People[], int&, string, int, bool admin = false, bool update = false);
void   deletePeople(People[], int&, string, int, bool admin = false);
bool   flip(int&, int, string);

void   searchPeople(People[], int, string, bool admin = false);
void   view(People[], int, string, int, bool show_cgpa = false, bool show_password = false);
void   view(People[], int[], int, string, int, bool show_cgpa = false, bool show_password = false);
bool   login(People[], int, int&);
void   list(People[], int);
void printList(People student[], int result[], int result_num, string status);


People students[MAX_STUDENTS]; // Array to store People data
People lecturers[MAX_LECTURERS]; // Array to store lecturer data

//========================================================================================================================//
// Main function
int main(void) {
    
    system("chcp 65001>nul");
    ShowWindow( GetConsoleWindow() , SW_MAXIMIZE);
    // Read banners
    readBanner("banner\\Admin Management System.txt", admin_manage);
    readBanner("banner\\Course Management System.txt", course_manage);
    readBanner("banner\\Lecturer Management System.txt", lecturer_manage);
    readBanner("banner\\Search Result.txt", search_result);
    readBanner("banner\\Student Management System.txt", student_manage);
    readBanner("banner\\Welcome to Student Management System.txt", welcome);
    readBanner("banner\\Report.txt", report);

    // Read file and get current number of students in the array
    int student_num, lecturer_num;
    readFile("students_list.txt", students, student_num);
    readFile("lecturer_list.txt", lecturers, lecturer_num);
    readCourseFile("course_list.txt", students, student_num);
    // Start the program

    bool exit = false;
    while (!exit) {
       mainMenu(students, lecturers, student_num, lecturer_num, exit);
    }

    writeFile("students_list.txt", students, student_num);
    writeFile("lecturer_list.txt", lecturers, lecturer_num);
    writeCourseFile("course_list.txt", students, student_num);
    cout << "  All changes are saved." << endl;
    cout << "  "; system("pause");
    return 0;
}

//========================================================================================================================//
// File handling related:

void readBanner (string filename, string& output) {
    fstream file(filename, ios::in);
    output = "";
    if (file.is_open()) {
        while (!file.eof()) {
            string temp;
            getline(file, temp);
            output += temp + "\n";
        }
    }
    else {
        cout << "  " << filename << " is not found, please check if all folder is exist" << endl;
        system("pause");
    }
    file.close();
}

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
        if (i != count - 1) {
            file << endl;
        }
    }
    file.close();
}

//========================================================================================================================//
// Menu related:

void mainMenu(People students[], People lecturers[], int& student_num, int& lecturer_num, bool& exit) {

    system("cls");
    cout << welcome << endl;
    cout << "  Please select your identity" << endl;
    cout << endl;
    cout << "  1. Student" << endl;
    cout << "  2. Lecturer" << endl;
    cout << "  3. Admin" << endl;
    cout << endl;
    cout << "  0. Exit" << endl;
    cout << endl;
    cout << "  Enter your option : ";
    string option;
    getline(cin, option);
    int i;
    if (option == "1") {
        if (login(students, student_num, i)) {
            courseMenu(students[i]);
        }
    }
    else if (option == "2") {
        if (login(lecturers, lecturer_num, i)) {
            peopleMenu(students, student_num, "Student");
        }
    }
    else if (option == "3") {
        cout << "  Enter admin password : ";
        string password;
        getline(cin, password);
        if (password == "admin") {
            adminMenu(students, student_num, lecturers, lecturer_num);
        }
        else {
            cout << "  Invalid password" << endl;
            cout << "  "; system("pause");
        }
    }
    else if (option == "0") {
        cout << "  Exiting..." << endl;
        exit = true;
        cout << "  "; system("pause");
    }
    else {
        cout << "  Invalid option" << endl;
        cout << "  "; system("pause");
    }
}

/**
 * @brief Show info of stuent's course
 * @param student The student to show course info
 * @param courses The array of courses
 * @param is_lecturer More features for lecturer, default to false
*/
void courseMenu(People& student, bool is_lecturer) {
    bool loop = true;
    int page = 1, max_page;
    while (loop) {
        max_page = roundUp(student.trimester_num, 3);
        system("cls");
        student.printStudent(page);
        
        cout << setfill('-') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
        cout << "    <P>revious page";
        cout << setw(SCREEN_WIDTH/2 - 16) << right << "Page " << page << " of " << setw(2) << left << max_page;
        cout << setw(SCREEN_WIDTH/2 - 16) << right << "<N>ext page" << endl;
        cout << endl;
        cout << "  1. Print course(s) information to file" << endl;
        if (is_lecturer) {
            cout << "  2. Add course" << endl;
            cout << "  3. Edit course" << endl;
            cout << "  4. Delete course" << endl;
        }
        else {
            cout << "  2. Edit password" << endl;
        }
        cout << endl;

        cout << "  0. Exit" << endl;
        cout << endl;

        cout << "  Please enter your option : ";
        string option;
        getline(cin, option);
        if (option == "0") {
            cout << "  Exiting..." << endl;
            loop = false;
            cout << "  "; system("pause");
        }
        else if (option == "1") {
            student.printFile(student.name + ".txt");
            cout << "  File saved to " << student.name << ".txt" << endl;
            cout << "  "; system("pause");
        }
        else if (flip(page, max_page, option)) {
            // Do nothing
        }
        else if (option == "2" && is_lecturer) {
            student.addCourseInfo(page);
        }
        else if (option == "3" && is_lecturer) {
            student.addCourseInfo(page, true);
        }
        else if (option == "4" && is_lecturer) {
            student.deleteCourse(page);
        }
        else if (!is_lecturer && option == "2") {
            student.editPassword(page);
        }
        else {
            cout << "  Invalid option" << endl;
            cout << "  "; system("pause");
        }
    }
}

void peopleMenu(People people[], int& total, string type, bool admin) {

    int page = 1;
    bool loop = true;
    while (loop) {
        system("cls");
        cout << endl;
        view(people, total, type, page, type == "Student", admin);
        cout << endl;
        cout << "  1. Add "    << type << endl;
        cout << "  2. Edit "   << type << endl;
        cout << "  3. Delete " << type << endl;
        cout << "  4. Search " << type << endl;
        if (type == "Student") {
            cout << "  5. View "   << type << " course" << endl;
            cout << "  6. View overall student performance" << endl;
        }
        cout << endl;
        cout << "  0. Exit" << endl;
        cout << endl;

        cout << "  Please enter your option : ";
        string option;
        getline(cin, option);
        if (flip(page, roundUp(total, MAX_ROW), option));
        else if (option == "1") {
            if (total < MAX_STUDENTS) {
                modifyPeopleInfo(people, total, type, page, admin);
            }
            else {
                cout << "  The system is full, please delete some " << type << " first." << endl;
                cout << "  "; system("pause");
            }
        }
        else if (option == "2") {
            modifyPeopleInfo(people, total, type, page, admin, true);
        }
        else if (option == "3") {
            deletePeople(people, total, type, page, admin);
        }
        else if (option == "4") {
            searchPeople(people, total, type, admin);
        }
        else if (option == "5" && type == "Student") {
            cout << "  Please enter the student ID : ";
            string id;
            getline(cin, id);
            int i = getIndex(people, total, id);
            if (i != -1) {
                courseMenu(people[i], true);
            }
            else {
                cout << "  Student not found, please try again." << endl;
                cout << "  "; system("pause");
            }
        }
        else if (option == "6" && type == "Student") {
            list(people, total);
        }
        else if (option == "0") {
            cout << "  Exiting..." << endl;
            loop = false;
            cout << "  "; system("pause");
        }
        else {
            cout << "  Invalid option. Please try again." << endl;
            cout << "  "; system("pause");
        }
    }
}

void adminMenu(People students[], int& student_num, People lecturers[], int& lecturer_num) {
    bool loop = true;
    while (loop) {
        system("cls");
        cout << admin_manage;
        cout << endl;
        cout << "  1. Student Management" << endl;
        cout << "  2. Lecturer Management" << endl;
        cout << endl;
        cout << "  0. Exit" << endl;
        cout << endl;

        cout << "  Please enter your option : ";
        string option;
        getline(cin, option);
        if (option == "1") {
            peopleMenu(students, student_num, "Student", true);
        }
        else if (option == "2") {
            peopleMenu(lecturers, lecturer_num, "Lecturer", true);
        }
        else if (option == "0") {
            cout << "  Exiting..." << endl;
            loop = false;
            cout << "  "; system("pause");
        }
        else {
            cout << "  Invalid option. Please try again." << endl;
            cout << "  "; system("pause");
        }
    }
}

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
bool testName(string input, int max_size)
{
    if (input == "" || input.size() > max_size || input[0] == ' ')
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
    if (input.back() == '0')
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

void trim(string& str) {
    // Using two pointers to remove the spaces
    int start = 0;
    int end = str.size() - 1;
    while (str[start] == ' ') {
        start++;
    }
    while (str[end] == ' ') {
        end--;
    }
    str = str.substr(start, end - start + 1);
}

int roundUp(int dividend, int divisor) {
    if (dividend == 0) {
        return 1;
    }
    else if (dividend % divisor == 0) {
        return dividend / divisor;
    }
    else {
        return dividend / divisor + 1;
    }
}

/**
 * @brief Search for a student or lecturer
 * @param people Array of students or lecturers
 * @param total Number of students or lecturers
 * @param type Type of people (Student or Lecturer)
 * @param result Array to store the index of the search result
 * @return Number of search results
*/
void searchPeople(People people[], int total, string type, bool admin) {
    bool loop = true;
    int page = 1;
    cout << "  Please enter " << type << " ID or " << type << " name : ";
    string search;
    getline(cin, search);
    search = toUpper(search);
    int result[MAX_STUDENTS];
    int search_result_num = searchEngine(people, total, search, result);
    while (loop)
    {
        system("cls");
        cout << search_result;
        view(people, result, search_result_num, type, page, type=="Student", admin);
        cout << endl;
        cout << "  0. Back" << endl;
        cout << endl;
        cout << "  Please enter your option : ";
        string option;
        getline(cin, option);
        if (option == "0") {
            loop = false;
        }
        else if (flip(page, roundUp(search_result_num, MAX_ROW), option)){
            // Do nothing, the page will update automatically
        }
        else {
            cout << "  Invalid option. Please try again." << endl;
            cout << "  "; system("pause");
        }
    }
}

int searchEngine(People people[], int total, string search, int result[])
{
    int search_result_num = 0;
    search = toUpper(search);
    for (int i = 0; i < total; i++)
    {
        if (toUpper(people[i].name).find(search) != string::npos ||
            toUpper(people[i].id).find(search) != string::npos)
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

void list(People student[], int size) {
    system("cls");
    int result[4][MAX_STUDENTS];
    int result_num[4] = { 0, 0, 0, 0 };
    /// result[0] = President's List
    /// result[1] = Dean's List
    /// result[2] = Probation
    /// result[3] = Normal
    for (int i = 0; i < size; i++) {
        if (student[i].status() == "President's List") {
            result[0][result_num[0]] = i;
            result_num[0]++;
        }
        else if (student[i].status() == "Dean's List") {
            result[1][result_num[1]] = i;
            result_num[1]++;
        }
        else if (student[i].status() == "Probation") {
            result[2][result_num[2]] = i;
            result_num[2]++;
        }
        else {
            result[3][result_num[3]] = i;
            result_num[3]++;
        }
    }
    cout << report;
    printList(student, result[0], result_num[0], "President's List (CGPA >= 3.85) ");
    printList(student, result[1], result_num[1], "Dean's List (3.60 <= CGPA < 3.85) ");
    printList(student, result[3], result_num[3], "Normal (2.00 <= CGPA < 3.60) ");
    printList(student, result[2], result_num[2], "Probation (CGPA < 2.00) ");
    cout << "  President's List : " << setw(4) << right << result_num[0] << endl;
    cout << "  Dean's List      : " << setw(4) << right << result_num[1] << endl;
    cout << "  Normal           : " << setw(4) << right << result_num[3] << endl;
    cout << "  Probation        : " << setw(4) << right << result_num[2] << endl;
    system("pause");
}

void printList(People student[], int result[], int result_num, string status) {
    cout << "  \033[1;4m" << status << "\033[0m" << endl;
    cout << "  \033[4m" << setw(6) << right << "No.";
    cout << "  " << left << setw(15) << "ID";
    cout << "  " << left << setw(60) << "Name";
    cout << "  " << "CGPA  " << "\033[0m" << endl;
    for (int i = 0; i < result_num; i++) {
        cout << "  " << setw(5) << right << i + 1 << ".";
        cout << "  " << left << setw(15) << student[result[i]].id;;
        cout << "  " << left << setw(60) << student[result[i]].name;
        cout << "  " << setprecision(4) << fixed << student[result[i]].cgpa << endl;
    }
    if (result_num == 0) {
        cout << "     No student in this category" << endl;
    }
    cout << endl;
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

bool getNum (string prompt, int& output, int size, int limit, bool& loop) {
    cout << prompt;
    string input;
    getline(cin, input);
    if (input == "0") {
        loop = false;
        return false;
    }
    else if (input.size() != size) {
        cout << "  Invalid input, please try again." << endl;
        cout << "  "; system("pause");
        return false;
    }
    else {
        for (int i = 0; i < input.size(); i++) {
            if (!isdigit(input[i])) {
                cout << "  Invalid input, please enter a number." << endl;
                cout << "  "; system("pause");
                return false;
            }
        }
        output = stoi(input);
        if (output < 1 || output > limit) {
            cout << "  Number is not within acceptable range, please try again." << endl;
            cout << "  "; system("pause");
            return false;
        }
    }
    return true;
}

bool getCourseCode (string prompt, string& code, bool& loop, bool accept_empty) {
    cout << prompt;
    getline(cin, code);
    code = toUpper(code);
    if (code == "0") {
        loop = false;
        return false;
    }
    else if (testCourseCode(code) || (accept_empty && code == "")) {
        return true;
    }
    else {
        cout << "  That is not the proper format of the course code." << endl;
        cout << "  Here is an example : LLLLNNNN" << endl;
        cout << "  (L = letters and N = numbers), the last digit cannot be 0" << endl;
        cout << "  "; system("pause");
        return false;
    }
}

bool getCourseName (string prompt, string& name, bool& loop, bool accept_empty) {
    cout << prompt;
    getline(cin, name);
    trim(name);
    if (name == "0") {
        loop = false;
        return false;
    }
    else if (testName(name) || (accept_empty && name == "")) {
        return true;
    }
    else {
        cout << "  That is not the proper format of the course name." << endl;
        cout << "  Here is an example : Introduction to Programming " << endl;
        cout << "  (Only alphabets and spaces are allowed)" << endl;
        cout << "  "; system("pause");
        return false;
    }
}

bool getGred (string prompt, string& gred, bool& loop, bool accept_empty) {
    cout << prompt;
    getline(cin, gred);
    gred = toUpper(gred);
    if (gred == "0") {
        loop = false;
        return false;
    }
    else if (getGpa(gred) != -1 || (accept_empty && gred == "")) {
        return true;
    }
    else {
        cout << "  That is not the proper format of the gred." << endl;
        cout << "  Here is an example : A+ " << endl;
        cout << "  Please enter again!" << endl;
        cout << "  "; system("pause");
        return false;
    }
}

bool getID (string prompt, string& id, bool& loop, bool is_student = true, bool accept_empty = false) {
    cout << prompt;
    getline(cin, id);
    if (id == "") {
        if (accept_empty) {
            return true;
        }
        cout << "  Invalid input, please try again." << endl;
        cout << "  The ID cannot be empty." << endl;
        cout << "  "; system("pause");
        return false;
    }
    else if (id == "0") {
        loop = false;
        return false;
    }
    else if (is_student) {
        if (testStudentId(id)) {
            return true;
        }
        else {
            cout << "  That is not the proper format of the student ID." << endl;
            cout << "  Here is an example : 1234567 " << endl;
            cout << "  (Only  7 digits number is allowed)" << endl;
            cout << "  "; system("pause");
            return false;
        }
    }
    else {
        for (int i = 0; i < id.size(); i++) {
            if (id[i] == '|') {
                cout << "  Invalid input, please try again." << endl;
                cout << "  Please do not use '|' in your ID." << endl;
                cout << "  "; system("pause");
                return false;
            }
        }
        return true;
    }
}

bool getName (string prompt, string& name, bool& loop, bool accept_empty = false) {
    cout << prompt;
    getline(cin, name);
    trim(name);
    if (name == "0") {
        loop = false;
        return false;
    }
    else if (testName(name) || (accept_empty && name == "")) {
        return true;
    }
    else {
        cout << "  That is not the proper format of the name." << endl;
        cout << "  Here is an example : John Doe " << endl;
        cout << "  (Only alphabets and spaces are allowed, maximum length shall be lower than " << MAX_NAME_LENGTH << ")" << endl;
        cout << "  "; system("pause");
        return false;
    }
}

bool getPassword(string prompt, string& password) {
    cout << prompt;
    getline(cin, password);
    bool dumb;
    if (password == "") {
        return confirm("  This will set no password for you, are you sure? (Y/N) : ", dumb);
    }
    for (int i = 0; i < password.size(); i++) {
        if (password[i] == ' ') {
            cout << "  Invalid input, please try again." << endl;
            cout << "  Please do not use space in your password." << endl;
            cout << "  "; system("pause");
            return false;
        }
    }
    return true;
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
        cout << "  Invalid input, please try again." << endl;
        cout << "  "; system("pause");
        loop = true;
        return false;
    }
}

bool flip(int& page, int max_page, string option) {
    option = toUpper(option);
    if (option == "P") {
        if (page > 1) {
            page--;
        }
        else {
            cout << "  There is no previous page." << endl;
            cout << "  "; system("pause");
        }
    }
    else if (option == "N") {
        if (page < max_page) {
            page++;
        }
        else {
            cout << "  This is the last page." << endl;
            cout << "  "; system("pause");
        }
    }
    else {
        return false;
    }
    return true;
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
void modifyPeopleInfo(People people[], int& newIndex, string type, int page, bool admin, bool update)
{
    bool loop = true;
	int step = 1, i = newIndex, size = newIndex;
    string prompt = " (<0> - exit";
    if (update)
    {
        prompt += " <Enter> - Keep the current value";    
    }
    prompt += ") : ";
    string header = "";

    string name, id, new_id, password;
	while (loop)
	{
        system("cls");
        view(people, size, type, page, type == "Student", admin);
        cout << endl;
		
        cout << header;
		if (step == 1)
		{
            if(getID("  Please enter ID (0 - Exit) : ", id, loop, type == "Student"))
            {
                int search = getIndex(people, newIndex, id);
                new_id = id;
                if (update)
                {
                    if (search == -1)
                    {
                        cout << "  ID not found!" << endl;
                        cout << "  Please enter again!" << endl;
                        cout << "  "; system("pause");
                    }
                    else
                    {
                        i = search;
                        step++;
                    }
                }
                else
                {
                    if (search == -1)
                    {
                        header += "  " + type + "  ID : " + id + "\n";
                        i = newIndex;
                        step+=2;
                    }
                    else
                    {
                        cout << "  ID already exists!" << endl;
                        cout << "  Please enter again!" << endl;
                        cout << "  "; system("pause");
                    }
                }
            }
        }
        else if (step == 2)
        {
            cout << "  Current ID : " << id << endl;
            if(getID("  Please enter new ID" + prompt, new_id, loop, type == "Student", true))
            {
                step++;
                if (new_id == "")
                {
                    new_id = people[i].id;
                    header += "  " + type + "  ID : " + new_id + "\n";
                }
                else if (getIndex(people, newIndex, new_id) == -1)
                {
                    header += "  " + type + "  ID : " + new_id + "\n";
                }
                else
                {
                    step--;
                    cout << "  ID already exists!" << endl;
                    cout << "  Please enter again!" << endl;
                    cout << "  "; system("pause");
                }
            }
        }
		else if (step == 3)
		{
            if(getName("  Please enter name" + prompt, name, loop, update))
            {
                if (name == "")
                {
                    name = people[i].name;
                }
                header += "  " + type + "  Name : " + name + "\n";
                step++;
            }
            if (!admin)
            {
                // Only admin can add & change password
                step++;
            }
		}
        
        else if (step == 4)
        {
            if(getPassword("  Please enter password (0 - Exit) : ", password))
            {
                step++;
            }
        }

        else if (step == 5)
        {
            people[i].name = name;
            people[i].id = new_id;
            people[i].password = password;
            if (!update)
            {
                newIndex++;
            }
            loop = false;
            system("cls");
            view(people, newIndex, type, page, type == "Student", admin);
            cout << endl;
            cout << "  " << type <<  " information updated !" << endl;
            cout << "  "; system("pause");
        }
	}
}

void deletePeople(People people[], int& newIndex, string type, int page, bool admin)
{
    bool loop = true;
    while (loop)
    {
        system("cls");
        view(people, newIndex, type, page, type == "Student", admin);
        cout << endl;
        string id;
        if(getID("  Please enter " + type + " ID to delete (0 - Exit) : ", id, loop, type == "Student"))
        {
            int i = getIndex(people, newIndex, id);
            if (i == -1)
            {
                cout << "  " << type << " ID not found!" << endl;
                cout << "  Please enter agian!" << endl;
                cout << "  "; system("pause");
            }
            else
            {
                while (loop)
                {
                    system("cls");
                    view(people, newIndex, type, page, type == "Student", admin);
                    cout << endl;
                    cout << "  Are you sure you want to delete this " << type << "?" << endl;
                    cout << endl;
                    cout << "  \033[4m" << type << " Information\033[0m" << endl;
                    cout << "  " << type << "  ID : " << people[i].id << endl;
                    cout << "  " << type << "  Name : " << people[i].name << endl;
                    if (confirm("  Confirm (Y/N) : ", loop))
                    {
                        for (; i < newIndex - 1; i++)
                        {
                            people[i] = people[i + 1];
                        }
                        newIndex--;
                        system("cls");
                        view(people, newIndex, type, page, type == "Student", admin);
                        cout << endl;
                        cout << "  " << type << " deleted!" << endl;
                        cout << "  "; system("pause");
                    }
                }
            }
        }
    }
}

//========================================================================================================================//
// Output related

/**
 * @brief View all people information
 * @param people Array of people information
 * @param size Size of the array
 * @param type Identity of the people (student/lecturer)
 * @param show_cgpa Show the cgpa, default is false
*/
void view (People people[], int size, string type, int page, bool show_cgpa, bool show_password)
{
    // header
    if (type == "Student") {
        cout << student_manage;
    }
    else {
        cout << lecturer_manage;
    }
    cout << setfill('=') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
    cout << "   No. \t";
    cout << setw(15) << left << type + " ID";
    cout << setw(60) << left << type + " name";
    if (show_cgpa)
    {
        cout << setw(10) << left << "CGPA";
    }
    if (show_password)
    {
        cout << "Password";
    }
    cout << endl;
    cout << setfill('=') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;


    // body
    int low = (page - 1) * MAX_ROW;
    int high = low + MAX_ROW;
    if (high > size)
    {
        high = size;
    }

    for (int i = low; i < high; i++)
    {
        cout << setw(5) << right << i + 1 << ". \t";
        people[i].print(show_cgpa, show_password);
        cout << endl;
    }

    if (size == 0)
    {
        cout << "  No data found." << endl;
    }
    
    cout << setfill('-') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
    cout << "    <P>revious page";
    cout << setw(SCREEN_WIDTH/2 - 16) << right << "Page " << page << " of " << setw(2) << left << roundUp(size, MAX_ROW);
    cout << setw(SCREEN_WIDTH/2 - 16) << right << "<N>ext page" << endl;
}

void view (People people[], int result[], int total, string type, int page, bool show_cgpa, bool show_password)
{
    // header
    cout << setfill('=') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
    cout << "   No. \t";
    cout << setw(15) << left << type + " ID";
    cout << setw(60) << left << type + " name";
    if (show_cgpa)
    {
        cout << setw(10) << left << "CGPA";
    }
    if (show_password)
    {
        cout << "Password";
    }
    cout << endl;
    cout << setfill('=') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;


    // body
    int low = (page - 1) * MAX_ROW;
    int high = low + MAX_ROW;
    if (high > total)
    {
        high = total;
    }

    for (int i = low; i < high; i++)
    {
        cout << setw(5) << right << i + 1 << ". \t";
        people[result[i]].print(show_cgpa, show_password);
        cout << endl;
    }

    if (total == 0)
    {
        cout << "   No data found...." << endl;
    }

    cout << setfill('-') << setw(SCREEN_WIDTH) << "" << setfill(' ') << endl;
    cout << "    <P>revious page";
    cout << setw(SCREEN_WIDTH/2 - 16) << right << "Page " << page << " of " << setw(2) << left << roundUp(total, MAX_ROW);
    cout << setw(SCREEN_WIDTH/2 - 16) << right << "<N>ext page" << endl;
}

//========================================================================================================================//
// Login related

/**
 * @brief Verify the login
 * @param password Correct password
 * @return True if the password is correct, false otherwise
*/
bool login(People people[], int size, int& index) {
    
    cout << "  Please enter your ID :";
    string id;
    getline(cin, id);
    int i = getIndex(people, size, id);
    if (i == -1) {
        cout << "  ID not found. Please try again." << endl;
        cout << "  "; system("pause");
        return false;
    }
    else {
        if (people[i].password == "") {
            return true;
        }
        cout << "  Please enter your password : ";
        string user_input;
        getline(cin, user_input);
        if (user_input != people[i].password) {
            cout << "  Wrong password. Please try again." << endl;
            system("pause");
            return false;
        }
    }
    index = i;
    return true;
}


