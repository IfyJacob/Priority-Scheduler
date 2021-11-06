// CMSC 341 - Spring 2021 - Project 3
// mytest.cpp: driver for the RQueue class.

//Ify Jacob
//Project 3 - rqueue.cpp
//i23@umbc.edu
//4/20/2021

//Included ctime to be able to use rand
#include <ctime>
#include "rqueue.h"
using namespace std;
// Priority functions compute an integer priority for a student. Internal
// computations may be floating point, but must return an integer.
int priorityFn1(const Student &tudent);
int priorityFn2(const Student &student);

class Tester {
public:
  void InsertRemovePerformance(int numTrials, int N, RQueue& queue);
  bool testCopyConstructor(RQueue& queue);
  bool testAssignment(RQueue& queue1, RQueue& queue2);
  bool InvalidMerge(RQueue& queue1, RQueue& queue2);
  bool emptyDequeue(RQueue& queue);
};

int main() {

  Tester tester;
  
  RQueue queue1(priorityFn1);
  RQueue queue2(priorityFn1);
  RQueue queue4(priorityFn2);
  
  // Create some test students and insert them into the queue
  //Student(string name, int priority, int year, int major, int group)
  Student student1("Sam",1,1,1,1);
  Student student2("Liz",5,1,2,0);
  Student student3("Nick",2,0,0,0);
  Student student4("Eva",4,3,2,1);

  queue1.insertStudent(student3);
  queue1.insertStudent(student1);
  queue1.insertStudent(student2);
  queue1.insertStudent(student4);
 
  std::cout << "\nqueue1 dump after inserting 4 students (Nick, Sam, Liz, Eva):" << std::endl;
  queue1.dump();

  Student student5("John",6,3,2,1);
  Student student6("Mia",3,0,0,0);

  queue2.insertStudent(student5);
  queue2.insertStudent(student6);

  std::cout << "\nqueue2 dump after inserting 2 students (John, Mia):" << std::endl;
  queue2.dump();
 
  queue1.mergeWithQueue(queue2);

  RQueue queue3(queue1); //used for later tests
  
  // Print the queue, change priority function, print again, change
  // again, print again.  Changing the priority function should cause
  // the heap to be rebuilt.

  std::cout << std::endl << "Contents of queue1 after merging with queue2:\n";
  queue1.printStudentQueue();
  std::cout << "\nDump queue1 after merging with queue2:\n";
  queue1.dump();

  queue1.setPriorityFn(priorityFn2);

  RQueue queue6 = queue1; //used for later tests
  
  std::cout << std::endl << "Contents of queue1 after changing the priority function to priorityFn2:\n";
  queue1.printStudentQueue();
  std::cout << "\nDump queue1 after changing the priority function to priorityFn2:\n";
  queue1.dump();


  queue1.setPriorityFn(priorityFn1);

  std::cout << std::endl << "Contents of queue1 after changing the priority function back to priorityFn1:\n";
  queue1.printStudentQueue();
  std::cout << "\nDump queue1 after changing the priority function back to priorityFn1:\n";
  queue1.dump();

  // Remove and print students in priority order
  std::cout << "\nFinal queue1 in priority order:\n";
  while (queue1.numStudents() > 0) {
    Student student = queue1.getNextStudent();
    std::cout << "[" << priorityFn1(student) << "] " << student << std::endl;
  }
  
  queue1 = queue3; //queue1 was emptied so its values are restablished

  //Testing Merge With empty Heap
  RQueue none(priorityFn1);
  none.mergeWithQueue(queue1);

  cout << "\n\nTesting RQueue empty heap merge with popultaed RQueue" << endl;
  none.dump();
  queue1.dump();
  cout << "The values above should match!" << endl;
  
  
  //Testing Insert Performance
  int trials = 6;
  int amount = 10000;
  cout << "\n\nTesting Rqueue Insert time..." << endl;
  tester.InsertRemovePerformance(trials, amount, queue4);
  cout << endl;
  
  //Testing Copy Constructor
  cout << "Testing Copy Constructor (deep copy):" << endl;
  if(tester.testCopyConstructor(queue3)) cout << "Copy constructor passed!" << endl;
  else cout << "Copy constructor failed!" << endl;
  cout << endl;
  
  //Testing Copy Constructor size 0
  RQueue empty(priorityFn1);
  cout << "Testing Copy Constructor size 0:" << endl;
  if(tester.testCopyConstructor(empty)) cout << "Copy constructor passed!" << endl;
  else cout << "Copy constructor failed!" << endl;
  cout << endl;
  
  //Assignment Operator test
  RQueue q1(queue1);
  RQueue q2 = q1;;
  cout << "Testing Assignment Operator (deep copy):" << endl;
  if(tester.testAssignment(q1,q2)) cout << "Assignment operator passed!" << endl;
  else cout << "Assignment operator failed!" << endl;
  cout << endl;

  //Assignment Operator test size 0
  RQueue rqueue(priorityFn1);
  RQueue rqueue2 = rqueue;
  cout << "Testing Assignment Operator size 0:" << endl;
  if(tester.testAssignment(rqueue,rqueue2)) cout << "Assignment operator passed!" << endl;
  else cout << "Assignment operator failed!" << endl;
  cout << endl;
  
  //Testing Merge with different Priorities
  cout << "Testing Invalid Merge:" << endl;
  if(tester.InvalidMerge(queue1,queue6)) cout << "Invalid Merge Passed!" << endl;
  else cout << "Invalid Merge failed!" << endl;
  cout << endl;
  
  //Testing Empty Dequeue
  cout << "Testing Empty Dequeue:" << endl;
  queue3.dump();
  if(tester.emptyDequeue(queue3)) cout << "Empty Dequeue Passed!" << endl;
  else cout << "Empty Dequeue failed!" << endl;
  cout << endl;

  return 0;
}

int priorityFn1(const Student&student) {
  //priority value is determined based on some criteria
  //priority value falls in the range [0-6]
  //the smaller value means the higher priority
  int priority = student.getYear() + student.getMajor() + student.getGroup();
  return priority;
}

int priorityFn2(const Student&student) {
  //priority is determined by an officer in registrar office
  //priority value falls in the range [0-6]
  //the smaller value means the higher priority
  return student.getPriority();
}

void Tester::InsertRemovePerformance(int numTrials, int N, RQueue& queue) {
  const int a = 2;//scaling factor for input size
  double T = 0.0;//to store running times
  clock_t start, stop;//stores the clock ticks while running the program

  Student newStudent = Student();
  for (int k=0;k<numTrials-1;k++)
    {

      //Clock for insert
      start = clock();
      for(int i=0; i<N; i++) {
	int randNum = (rand() % (6 + 1)); //generates a random int value between 0-6
	newStudent._priority = randNum; //sets random number to student priority
	queue.insertStudent(newStudent);
      }
      stop = clock();
      
      T = stop - start;//number of clock ticks the algorithm took
      cout << "Inserting " << N << " members took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!" << endl;

      //Clock for remove using getNextStudent
      start = clock();
      for(int i=0; i<N; i++) {
	queue.getNextStudent();
      }
      stop = clock();

      T = stop - start;//number of clock ticks the algorithm took
      cout << "Extracting " << N << " members took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!" << endl;

      cout << endl;
      N = N * a;//increase the input size by the scaling factor
    }
}

bool Tester::testCopyConstructor(RQueue& queue){
  RQueue copy(queue);

  //the case of empty object
  if(queue._size == 0 && copy._size == 0) return true;

  if(queue._size != copy._size) return false;
  
  //the case that sizes are the same and the table pointers are not the same
  else if(queue._size == copy._size && queue._heap != copy._heap){

    //Proof that its a deep copy
    cout << "Heap Addresses " << queue._heap << " " << copy._heap << endl;
    
    cout << "Original Queue dump ";
    queue.dump();

    cout << "Copy Queue dump ";
    copy.dump();

    int size = queue._size;

    //Compares student for each queue (the order should be the same)
    for(int i=0;i<size;i++){
      
      Student student1 = queue.getNextStudent();
      Student student2 = copy.getNextStudent();

      if(student1.getName() != student2.getName()) { //check the value	
	  return false;	
      }
    }
    return true;
  }
  else return false;
}

bool Tester::testAssignment(RQueue& queue1, RQueue& queue2){
  //the case of empty object
  if(queue1._size == 0 && queue2._size == 0) return true;

  if(queue1._size != queue2._size) return false;
  
  //the case that sizes are the same and the table pointers are not the same
  else if (queue1._size == queue2._size && queue1._heap != queue2._heap){

    //Proof that its a deep copy
    cout << "Heap Addresses " << queue1._heap << " " << queue2._heap << endl;

    cout << "Queue1 dump ";
    queue1.dump();

    cout << "Queue2 dump ";
    queue2.dump();

    int size = queue1._size;

    //Compares student for each queue (the order should be the same)
    for (int i=0;i<size;i++){

      Student student1 = queue1.getNextStudent();
      Student student2 = queue2.getNextStudent();

      if(student1.getName() != student2.getName()) { //check the value
	return false;
      }
    }
    return true;
  }
  else return false;
}

bool Tester::InvalidMerge(RQueue& queue1, RQueue& queue2) {
  try {
    queue1.mergeWithQueue(queue2);
    queue1.dump();
  }
  catch(domain_error &e) {
    return true;
  }
  catch(...){
    //any other exception, this case is not acceptable
    return false;
  }
  //no exception thrown, this case is not acceptable
  return false;
}

bool Tester::emptyDequeue(RQueue& queue) {
  try {
    queue.getNextStudent();
  }
  catch(domain_error &e) {
    return true;
  }
  catch(...){
    //any other exception, this case is not acceptable
    return false;
  }
  //no exception thrown, this case is not acceptable
  return false;
}
