#include<iostream>
#include <fstream>
#include<string>
#include<exception>
#include"ProjectAbstract.h"
#include<ctime>
using namespace std;
class Attendance;
class Course{
   protected:
      string dep;
      int id;
      int credithours;
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
      virtual void display()=0;
    };
class OOP:public Course{
   public:
      OOP():Course("CS",1009,3){}
      void display(){cout<<"Registered OOP Successfully!\n"};
   };
class DLD:public Course{
   public:
      DLD():Course("EE",1010,3){}
      void display(){cout<<"Registered DLD Successfully!\n"};
   };
class MVC:public Course{
   public:
      MVC():Course("CS",1011,3){}
      void display(){cout<<"Registered MVC Successfully!\n"};
   };
class Student{
      int rollno;
      static int id;
      string name;
      Attendance record;
      Course *c[3];
   public:
      Student(string n){
         rollno = id;
         id++;
         name = n;
      }
      void callmark(){record.mark();};
      void callunmark(){record.unmark()};
      void callsearchmark(int a, int b){record.searchmark(a,b)};
      void callsearchunmark()(int a, int b){record.searchunmark(a,b)};
      void setcourse(int numberofcourses){
         if (numberofcourses==3){
            c[0]=new OOP();
            c[0].display();
            c[1]=new MVC();
            c[1].display();
            c[2]=new DLD();
            c[2].display();
         }
         else if (numberofcourses<3){
            int temp;
            for(int i=0;i<numberofcourses;i++){
            cout<<"press 1 for OOP\npress 2 for MVC\npress 3 for DLD\n= ";
            cin>>temp;
            if (temp==1) {
               c[i]=new OOP();
               c[i].display();
               };
            else if (temp==2) {
               c[i]=new MVC();
               c[i].display();
               };
            else if (temp==3) {
               c[i]=new DLD();
               c[i].display();
               };
         }
      }
      void setname(string n){
         name = n;
      }
      int getrollno(){
         return rollno;
      }
      string getname(){
         return name;
      }
   };
int Student::id=0;
class Attendance{
      char att[365];
      int year[365];
      int month[365];
      int day[365];
      int noofdays;
   public:
      Attendance(){
         for (int i=0;i<365;i++){
            att[i] = 'A';
            year[i] = 0;
            month[i]=0;
            day[i]=0;
         }
         noofdays = 0;
      }
      void mark(){
         att[noofdays]='P';
         time_t now = time(nullptr);
         tm* localTime = localtime(&now);
         year[noofdays] = 1900 + localTime->tm_year;
         month[noofdays] = 1 + localTime->tm_mon;
         day[noofdays] = localTime->tm_mday;
         noofdays++;
        };
      void unmark(){
         att[noofdays]='A';
         year[noofdays] = 0;
         month[noofdays] = 0;
         day[noofdays] = 0;
         noofdays--; 
        };
      void mark(int a, int b , int c, int d){
            att[d]='P';
            year[d] = c;
            month[d] = b;
            day[d] = a;
            noofdays++;
        };
      void unmark(int a, int b , int c, int d){
         att[d]='A';
         year[d] = c;
         month[d] = b;
         day[d] = a;
         noofdays--;
        };
      void searchmark(int a, int b){
         int c=0;
         for (int i=0;i<365;i++){
            if (day[i]==a && month[i]==b){
               cout<<"Enter Year: ";
               cin>>c;
               mark(a,b,c,i);
               i=370;
             }
         };
         if(c==0)cout<<"Date not found!";
         };
      void searchunmark(int a, int b){
         int c=0;
         for (int i=0;i<365;i++){
            if (day[i]==a && month[i]==b){
               cout<<"Enter Year: ";
               cin>>c;
               unmark(a,b,c,i);
               i=365;
             }}
            if(c==0) cout<<"date not found";
         };
   };
class Teacher{
   string name;
   int students[500];
   int count;
   Course *c;
   public:
   Teacher(string a,string b){
      count=0;
      name=a;
      try{
      if (b=="oop"){
         c=new OOP;
      };
      else if(b=="dld"){
         c=new DLD;
      };
      else if (b=="mvc"){
         c=new MVC;
      };
      else throw runtime_error("Invalid Course");
      }
      catch(const exception& e){
         cout<<"ERROR: "<<e.what<<endl;
      };
   }
   void assignstudent(Student& b){
      students[count]=b.getrollno();
      count++;
   }
   void removestudent(Student& b, int id){
      int temp;
      for (int i=0;i<500;i++){
      if (students[i]==id){
         temp=i;
         students[i]=0;
         count--;
      }
      }
      for(int i=temp;i<500;i++){
         if(i!=500){
         students[i]=students[i+1];
      }
      else students[0];
   }
   void setname(string n){name=n;};
};
