#include<iostream>
#include<ctime>
using namespace std;

class Attendance{
int *date;
int *studentsid;
int noofstudents;
static int noofdays;
public:
Attendance(){
    noofdays++;
date = new int[noofdays];
time_t now = time(0);
tm *ltm = localtime(&now);
date[noofdays - 1] = (1900 + ltm->tm_year) * 10000 + (1 + ltm->tm_mon) * 100 + ltm->tm_mday;
}


};