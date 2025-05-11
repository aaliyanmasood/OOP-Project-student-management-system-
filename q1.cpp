#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <ctime>

using namespace std;

class SMSException : public exception {
    string message;
public:
    SMSException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class Course {
public:
    string code;
    string name;

    Course() : code(""), name("") {}
    Course(string c, string n) : code(c), name(n) {}
};

class Student {
    int rollNo;
    string name;
    static const int MAX_COURSES = 5;
    Course courses[MAX_COURSES];
    int courseCount;
public:
    Student() : rollNo(0), name(""), courseCount(0) {}
    Student(int id, string nm) : rollNo(id), name(nm), courseCount(0) {}

    int getRollNo() const { return rollNo; }
    string getName() const { return name; }

    void registerCourse(const Course& course) {
        for (int i = 0; i < courseCount; ++i) {
            if (courses[i].code == course.code)
                throw SMSException("Course already registered");
        }
        if (courseCount >= MAX_COURSES)
            throw SMSException("Maximum course limit reached");
        courses[courseCount++] = course;
    }

    bool isRegisteredInCourse(const string& courseCode) const {
        for (int i = 0; i < courseCount; ++i) {
            if (courses[i].code == courseCode) return true;
        }
        return false;
    }

    void displayCourses() const {
        cout << "Courses for " << name << " (ID: " << rollNo << "):\n";
        for (int i = 0; i < courseCount; ++i)
            cout << "  " << courses[i].code << " - " << courses[i].name << endl;
    }

    void saveToFile() const {
        ofstream fout("Student.txt", ios::app);
        if (!fout) throw SMSException("Failed to open Student.txt");
        fout << rollNo << "," << name << "\n";
        fout.close();
    }
};

class Teacher {
    int teacherID;
    string name;
    Course course;
    static const int MAX_STUDENTS = 50;
    int assignedStudents[MAX_STUDENTS];
    int assignedCount;
public:
    Teacher() : teacherID(0), name(""), assignedCount(0) {}
    Teacher(int id, string nm, Course crs) : teacherID(id), name(nm), course(crs), assignedCount(0) {}

    int getTeacherID() const { return teacherID; }
    string getName() const { return name; }
    Course getCourse() const { return course; }

    void assignStudent(int studentID) {
        if (assignedCount >= MAX_STUDENTS) 
            throw SMSException("Teacher's assigned student limit reached");
        for (int i = 0; i < assignedCount; ++i)
            if (assignedStudents[i] == studentID)
                return; // already assigned
        assignedStudents[assignedCount++] = studentID;
    }

    bool isStudentAssigned(int studentID) const {
        for (int i = 0; i < assignedCount; ++i)
            if (assignedStudents[i] == studentID) return true;
        return false;
    }

    void saveToFile() const {
        ofstream fout("Teacher.txt", ios::app);
        if (!fout) throw SMSException("Failed to open Teacher.txt");
        fout << teacherID << "," << name << "," << course.code << "," << course.name << "\n";
        fout.close();
    }

    void markAttendance(int studentID, bool present) {
        string filename = to_string(studentID) + "_Attendance.txt";
        ofstream fout(filename, ios::app);
        if (!fout) throw SMSException("Cannot open attendance file for student");
        time_t now = time(nullptr);
        fout << course.code << "," << (present ? "Present" : "Absent") << "," << now << "\n";
        fout.close();
    }

    void assignMarks(int studentID, float marks) {
        string filename = to_string(studentID) + "_" + course.code + "_Marks.txt";
        ofstream fout(filename, ios::app);
        if (!fout) throw SMSException("Cannot open marks file for student");
        time_t now = time(nullptr);
        fout << marks << "," << now << "\n";
        fout.close();
    }

    void saveAssignments() const {
        string filename = to_string(teacherID) + "_Assignments.txt";
        ofstream fout(filename);
        if (!fout) throw SMSException("Cannot open assignments file");
        for (int i = 0; i < assignedCount; ++i)
            fout << assignedStudents[i] << "," << course.code << "\n";
        fout.close();
    }

    void overwriteAssignments() const {
        string filename = to_string(teacherID) + "_Assignments.txt";
        ofstream fout(filename);
        if (!fout) throw SMSException("Cannot open assignments file");
        for (int i = 0; i < assignedCount; ++i)
            fout << assignedStudents[i] << "," << course.code << "\n";
        fout.close();
    }
};

template <class T>
class AdminManager {
    static const int MAX_ENTRIES = 500;
    T* entries[MAX_ENTRIES];
    int count;
public:
    AdminManager() : count(0) {
        for (int i = 0; i < MAX_ENTRIES; ++i) entries[i] = nullptr;
    }
    ~AdminManager() {
        for (int i = 0; i < count; ++i) delete entries[i];
    }

    void add(T* entry) {
        if (count >= MAX_ENTRIES) 
            throw SMSException("Maximum entries reached");
        int id = getID(entry);
        if (findByID(id)) 
            throw SMSException("ID already exists");
        entries[count++] = entry;
        entry->saveToFile();
    }

    void removeByID(int id) {
        bool found = false;
        for (int i = 0; i < count; ++i) {
            if (getID(entries[i]) == id) {
                found = true;
                delete entries[i];
                for (int j = i; j < count - 1; ++j)
                    entries[j] = entries[j + 1];
                entries[count - 1] = nullptr;
                --count;
                break;
            }
        }
        if (!found)
            throw SMSException("Entry with given ID not found");
    }

    T* findByID(int id) const {
        for (int i = 0; i < count; ++i) 
            if (getID(entries[i]) == id) return entries[i];
        return nullptr;
    }

    void displayAll() const {
        for (int i = 0; i < count; ++i)
            display(entries[i]);
    }

private:
    int getID(Student* s) const { return s->getRollNo(); }
    int getID(Teacher* t) const { return t->getTeacherID(); }
    void display(Student* s) const { cout << "Student: " << s->getName() << ", ID: " << s->getRollNo() << endl; }
    void display(Teacher* t) const { cout << "Teacher: " << t->getName() << ", ID: " << t->getTeacherID() << ", Course: " << t->getCourse().code << endl; }
};

template<typename T>
void rewriteFileWithoutID(const string& filename, int id, int(T::*getId)() const) {
    ifstream fin(filename);
    if (!fin) return;
    ofstream temp("tempfile.txt");
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        size_t pos = line.find(',');
        if (pos == string::npos) continue;
        int currentID = stoi(line.substr(0, pos));
        if (currentID != id) {
            temp << line << '\n';
        }
    }
    fin.close();
    temp.close();
    remove(filename.c_str());
    rename("tempfile.txt", filename.c_str());
}

class Admin {
public:
    AdminManager<Student> studentManager;
    AdminManager<Teacher> teacherManager;

    void addStudent(int id, const string& name) {
        Student* s = new Student(id, name);
        studentManager.add(s);
        cout << "Added student " << name << " with ID " << id << endl;
    }

    void removeStudent(int id) {
        studentManager.removeByID(id);
        rewriteFileWithoutID<Student>("Student.txt", id, &Student::getRollNo);
        cout << "Removed student with ID " << id << endl;
    }

    void addTeacher(int id, const string& name, Course course) {
        Teacher* t = new Teacher(id, name, course);
        teacherManager.add(t);
        cout << "Added teacher " << name << " with ID " << id << endl;
    }

    void removeTeacher(int id) {
        teacherManager.removeByID(id);
        rewriteFileWithoutID<Teacher>("Teacher.txt", id, &Teacher::getTeacherID);
        string filename = to_string(id) + "_Assignments.txt";
        remove(filename.c_str());
        cout << "Removed teacher with ID " << id << endl;
    }

    void assignStudentToTeacher(int studentID, int teacherID) {
        Student* s = studentManager.findByID(studentID);
        Teacher* t = teacherManager.findByID(teacherID);
        if (!s) throw SMSException("Student not found");
        if (!t) throw SMSException("Teacher not found");
        if (!s->isRegisteredInCourse(t->getCourse().code))
            s->registerCourse(t->getCourse());
        t->assignStudent(studentID);
        t->overwriteAssignments();
        cout << "Assigned student " << studentID << " to teacher " << teacherID << " for course " << t->getCourse().code << endl;
    }

    Student* findStudent(int id) { return studentManager.findByID(id); }
    Teacher* findTeacher(int id) { return teacherManager.findByID(id); }
};

float calculateAttendancePercentage(const string& filename, const string& courseCode) {
    ifstream fin(filename.c_str());
    if (!fin) return 0.0f;

    int total = 0, present = 0;
    string line;
    while(getline(fin,line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1+1);
        if (pos1 == string::npos || pos2 == string::npos) continue;

        string code = line.substr(0, pos1);
        string status = line.substr(pos1+1, pos2-pos1-1);

        if (code != courseCode) continue;

        total++;
        if (status == "Present") present++;
    }
    fin.close();
    if(total == 0) return 0.0f;
    return (float)present*100.0f/(float)total;
}

void studentViewMarks(int studentID, const string& courseCode) {
    string filename = to_string(studentID) + "_" + courseCode + "_Marks.txt";
    ifstream fin(filename.c_str());
    if(!fin) {
        cout << "No marks available for course " << courseCode << endl;
        return;
    }
    cout << "Marks for course " << courseCode << ":" << endl;
    string line;
    int count = 0;
    while (getline(fin, line)) {
        size_t pos = line.find(',');
        if (pos == string::npos) continue;
        string markStr = line.substr(0,pos);
        cout << "Mark: " << markStr << endl;
        count++;
    }
    if(count == 0) cout << "No marks recorded." << endl;
    fin.close();
}

void adminMenu(Admin& admin) {
    while (true) {
        cout << "\nAdmin Menu:\n"
             << "1. Add Student\n"
             << "2. Remove Student\n"
             << "3. Add Teacher\n"
             << "4. Remove Teacher\n"
             << "5. Assign Student to Teacher\n"
             << "6. List Students\n"
             << "7. List Teachers\n"
             << "8. Exit Admin Menu\n"
             << "Choose: ";
        int choice; cin >> choice;
        cin.ignore();

        try {
            if (choice == 1) {
                cout << "Enter student ID: ";
                int id; cin >> id; cin.ignore();
                cout << "Enter student name: ";
                string nm; getline(cin,nm);
                admin.addStudent(id, nm);
            } else if (choice == 2) {
                cout << "Enter student ID to remove: ";
                int id; cin >> id; cin.ignore();
                admin.removeStudent(id);
            } else if (choice == 3) {
                cout << "Enter teacher ID: ";
                int id; cin >> id; cin.ignore();
                cout << "Enter teacher name: ";
                string nm; getline(cin,nm);
                cout << "Enter course code: ";
                string ccode; getline(cin,ccode);
                cout << "Enter course name: ";
                string cname; getline(cin,cname);
                admin.addTeacher(id, nm, Course(ccode,cname));
            } else if (choice == 4) {
                cout << "Enter teacher ID to remove: ";
                int id; cin >> id; cin.ignore();
                admin.removeTeacher(id);
            } else if (choice == 5) {
                cout << "Enter student ID to assign: ";
                int sid; cin >> sid; cin.ignore();
                cout << "Enter teacher ID to assign: ";
                int tid; cin >> tid; cin.ignore();
                admin.assignStudentToTeacher(sid, tid);
            } else if (choice == 6) {
                admin.studentManager.displayAll();
            } else if (choice == 7) {
                admin.teacherManager.displayAll();
            } else if (choice == 8) {
                break;
            } else {
                cout << "Invalid choice.\n";
            }
        } catch (SMSException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

void teacherMenu(Admin& admin, Teacher* teacher) {
    while (true) {
        cout << "\nTeacher Menu (" << teacher->getName() << " - " << teacher->getCourse().code << ")\n"
             << "1. Mark Attendance\n"
             << "2. Assign Marks\n"
             << "3. Exit Teacher Menu\n"
             << "Choose: ";
        int choice; cin >> choice; 
        cin.ignore();

        try {
            if (choice == 1) {
                cout << "Enter student ID: ";
                int sid; cin >> sid; cin.ignore();
                if (!teacher->isStudentAssigned(sid)) {
                    cout << "Student not assigned to you.\n";
                    continue;
                }
                cout << "Present? (1 for Yes / 0 for No): ";
                int present; cin >> present; cin.ignore();
                teacher->markAttendance(sid, present == 1);
                cout << "Attendance marked.\n";
            } else if (choice == 2) {
                cout << "Enter student ID: ";
                int sid; cin >> sid; cin.ignore();
                if (!teacher->isStudentAssigned(sid)) {
                    cout << "Student not assigned to you.\n";
                    continue;
                }
                cout << "Enter marks: ";
                float marks; cin >> marks; cin.ignore();
                teacher->assignMarks(sid, marks);
                cout << "Marks assigned.\n";
            } else if (choice == 3) {
                break;
            } else {
                cout << "Invalid choice.\n";
            }
        } catch (SMSException& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

void studentMenu(Admin& admin, Student* student) {
    while (true) {
        cout << "\nStudent Menu (" << student->getName() << ")\n"
             << "1. View Marks\n"
             << "2. View Attendance Percentage\n"
             << "3. Exit Student Menu\n"
             << "Choose: ";
        int choice; cin >> choice;
        cin.ignore();

        if (choice == 1) {
            cout << "Enter course code: ";
            string code; getline(cin, code);
            studentViewMarks(student->getRollNo(), code);
        } else if (choice == 2) {
            cout << "Enter course code: ";
            string code; getline(cin, code);
            string filename = to_string(student->getRollNo()) + "_Attendance.txt";
            float percent = calculateAttendancePercentage(filename, code);
            cout << "Attendance percentage for " << code << ": " << percent << "%\n";
        } else if (choice == 3) {
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

int main() {
    Admin admin;
    while (true) {
        cout << "\nMain Menu:\n1. Admin\n2. Teacher\n3. Student\n4. Exit\nChoose role: ";
        int role; cin >> role;
        cin.ignore();

        if (role == 1) {
            adminMenu(admin);
        } else if (role == 2) {
            cout << "Enter teacher ID: ";
            int id; cin >> id; cin.ignore();
            Teacher* t = admin.findTeacher(id);
            if (!t) cout << "Teacher not found.\n";
            else teacherMenu(admin, t);
        } else if (role == 3) {
            cout << "Enter student ID: ";
            int id; cin >> id; cin.ignore();
            Student* s = admin.findStudent(id);
            if (!s) cout << "Student not found.\n";
            else studentMenu(admin, s);
        } else if (role == 4) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}
