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

// Function declaration

int split(string, char, string []);
void readFile(string, Student[] , int&);
void writeFile(string, Student[], int);
void displayStudentInfo(Student[], int);


// Main function
int main() {
    Student students[MAX_STUDENTS]; // Array to store student data
    int student_num;
    readFile("students_list.txt", students, student_num); // Current number of students in the array

    writeFile("students_list_output.txt", students, student_num);

    return 0;
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
        getline(file, students[n].password);

        // read course info
        getline(file, courses);
        string courses_arr[MAX_COURSES];
        int i = split(courses, '|', courses_arr);
        int total_credit_hour = 0;
        for (int j = 0; j < i; j++) { // loop every course of student
            string course_info[3];
            split(courses_arr[j], ',', course_info);
            students[n].course[j].code = course_info[0];
            students[n].course[j].name = course_info[1];
            students[n].course[j].gred = course_info[2];
            students[n].course[j].credit_hour = int(students[n].course[j].code[8]);
            total_credit_hour += students[n].course[j].credit_hour;
        }
        students[n].course_num = i;
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
    fstream file;
    file.open(file_name, ios::out);

    for (int i = 0; i < student_num; i++) {
        // write student info
        file << students[i].id << '|';
        file << students[i].name << '|';
        file << students[i].password << endl;
        //write course info
        for (int j = 0; j < students[i].course_num; j++) {
            file << students[i].course[j].code << ',';
            file << students[i].course[j].name << ',';
            file << students[i].course[j].gred;
            // if the course is not wirte delimiter char
            if (j != students[i].course_num - 1) {
                file << '|';
            }
        }
        // end of the student & course info

        // if not last student, write new line char
        if (i != student_num -1){
            file << endl;
        }
    }
    file.close();
}
