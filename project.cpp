#include<iostream>
using namespace std;

class Course{
protected:
string dep;
int id;
int credithours;
int noofstudentsenrolled;
public:
Course(string dep,int id,int credithours): dep(dep),id(id),credithours(credithours){}
string getdep(){
    return dep;
}
int getid(){
 return id;
}
int credithours(){
    return credithours;
}
};
class OOP:public Course{
public:
OOP():Course("CS",1009,3){}

};
class DLD:public Course{
public:
DLD():Course("EE",1010,3){}

};
class MVC:public Course{
public:
MVC():Course("CS",1011,3){}

};

