// CMSC 341 - Spring 2021 - Project 3
// RQueue:

//Ify Jacob
//Project 3 - rqueue.cpp
//i23@umbc.edu
//4/20/2021

#include "rqueue.h"
using namespace std;
using std::cout;
using std::endl;
using std::domain_error;

RQueue::RQueue(prifn_t priFn)
{
  /************************
   * To be implemented
   * *********************/
  priority = priFn; //Initializes priority
  _heap = nullptr; //default for _heap
  _size = 0; //deafault size
}

RQueue::~RQueue()
{
  /************************
   * To be implemented
   * *********************/
  clear();
  _heap = nullptr;
  _size = 0;
  priority = nullptr;
}

RQueue::RQueue(const RQueue& rhs)
{
  /************************
   * To be implemented
   * *********************/
  _heap = nullptr; //default for _heap before copy
  priority = rhs.getPriorityFn();
  _size = rhs._size;
  _heap = AssignHelp(rhs._heap); //Helper function copys rhs values to _heap
}

RQueue& RQueue::operator=(const RQueue& rhs)
{
  /************************
   * To be implemented
   * *********************/

  //Avoid Self Assignment
  if(this != &rhs) {

    clear(); //clears to avoid deep copy

    _heap = nullptr; //default for _heap before assignment
    _size = rhs._size;
    priority = rhs.getPriorityFn();    
    _heap = AssignHelp(rhs._heap);//Helper function copys rhs values to _heap
  }
  return *this;
}

///////////////////////////////////////
Node* RQueue::AssignHelp(Node* root) {

  //Helper function used both in copy and assignment methods
  if(root == nullptr)
    return nullptr;

  Node* newNode = new Node(root->getStudent()); //creates a newNode to avoid shallow copies

  //Recursive calls that build the tree
  newNode->_left = AssignHelp(root->_left);
  newNode->_right = AssignHelp(root->_right);
  return newNode;
}


void RQueue::insertStudent(const Student& input) {
  /************************
   * To be implemented
   * *********************/
  insertHelp(_heap, input); //Insert helper that builds the Tree
}

////////////////////////////////////////////////////////////
void RQueue::insertHelp(Node *curr, const Student& input) {
  Node *newNode = new Node(input); 

  //Initial point in which the _heap is empty (could've used _size == 0)
  if(curr == nullptr) {
    _heap = newNode;
    _size++;
    return;
  }

  _heap = mergeHelp(_heap, newNode); //calls merge Helper that builds the heap
  _size++;
  return;
}
  
Student RQueue::getNextStudent() {
  /************************
   * To be implemented
   * *********************/

  //Exception thrown if the heap is empty
  if(_size ==  0) {
    throw std::domain_error("Heap is empty");
  }

  Student first = _heap->getStudent(); //stores value of the root student
  Node* temp = _heap; //sets temp equal to heap pointer that will be deleted

  _heap = mergeHelp(_heap->_left, _heap->_right); //calls merge Helper to rebuild heap using child values
  _size--; //decremenrs size since item of heap has been removed
  
  delete temp; //deallocates value of the original root
  return first;
}

void RQueue::mergeWithQueue(RQueue& rhs) {
  /************************
   * To be implemented
   * *********************/

  //Exception thrown if the heap priority does not match rhs heap priority
  if(getPriorityFn() != rhs.getPriorityFn()) {
    throw std::domain_error("Unmacthed Priority functions");
    return;
  }

  //Case 1: The heap is empty therefore the assignment operator can be used
  if(_heap == nullptr) {
    *this = rhs; //calls assignment operator
    return;
  }

  //Case 2: The rhs heap is empty so there is nothing that needs to be done
  if(rhs._heap == nullptr)
    return;
  
  //Case 3: Merge Heap with rhs Heap and the Heap has min priority
  if(priority(_heap->getStudent()) <=  priority(rhs._heap->getStudent())) {
    _heap = mergeHelp(_heap, rhs._heap);
    _size += rhs._size;
    rhs._heap = nullptr; //Has to be done or there will be errors/leaks
    rhs._size = 0; //Has to be done or there will be errors/leaks
    return;
  }

  //Case 4: Merge Heap with rhs Heap and the rhs Heap has min priority
  else {
    _heap = mergeHelp(rhs._heap, _heap);
    _size += rhs._size;
    rhs._heap = nullptr; //Has to be done or there will be errors/leaks
    rhs._size = 0; //Has to be done or there will be errors/leaks
    return;
  }  
}

/////////////////////////////////////////////
Node* RQueue::mergeHelp(Node* p1, Node* p2) {
  //Merge Helper function that takes two nodes and combines them

  //Case where p1 is nullptr then p2 becomes the heap
  if(p1 == nullptr) {
    return p2;
  }

  //Case where p2 is nullptr nothing else has to be done
  if(p2 == nullptr) {
    return p1;
  }


  //Swaps values if p2 has min priority
  if(priority(p1->getStudent()) > priority(p2->getStudent())) {
    Node* swap = p1;
    p1 = p2;
    p2 = swap;
  }

  //Swaps the values of p1's child nodes
  Node *temp = p1->_left;
  p1->_left = p1->_right;
  p1->_right = temp;
  
  p1->_left = mergeHelp(p1->_left, p2); //recursive call to p1 left for skew purposes
  return p1;
}

void RQueue::clear() {
  /************************
   * To be implemented
   * *********************/
  clearing(_heap); //calls clear Helper that traverses through entire tree

  //sets values back to default
  _heap = nullptr;
  _size = 0;
  priority = nullptr;
}

/////////////////////////////////
void RQueue::clearing(Node *curr) {

  //Helper function that deallocates values inside RQueue
  //Inorder traversal that deletes the root last
  if(curr == nullptr)
    return;

  clearing(curr->_left);
  clearing(curr->_right);
  delete curr; //deallocates current node
}

int RQueue::numStudents() const
{
  /************************
   * To be implemented
   * *********************/
  return _size; //returns size value
}

void RQueue::printStudentQueue() const {
  /************************
   * To be implemented
   * *********************/
  printHelp(_heap); //calls print Helper function
}

//////////////////////////////////////////
void RQueue::printHelp(Node *curr) const {
  //Recursive preorder traversal of heap tree
  if(curr == nullptr)
    return;

  cout << "[" << priority(curr->getStudent()) << "] "  << curr->getStudent() << endl;
  printHelp(curr->_left);
  printHelp(curr->_right);
}

prifn_t RQueue::getPriorityFn() const {
  /************************
   * To be implemented
   * *********************/
  return priority; //returns priority value
}

void RQueue::setPriorityFn(prifn_t priFn) {
  /************************
   * To be implemented
   * *********************/
  priority = priFn; //sets priority to anthoer priority

  Node** arr = new Node*[_size]; //creates a dynamic array that stores the students
  _size = 0; //resets size (array will reuse values of _size)
  traverse(_heap, arr); //calls traversal helper that builds dynamic array
  
  rebuild(arr); //calls rebuild function that recreates heap
  delete [] arr; //deallocates dynamic array

}

/////////////////////////////////////////////////
void RQueue::traverse(Node *root, Node** array) {

  //Preoder traversal Helper function that populates the dynamic array
  if(root == nullptr)
    return;

  array[_size] = root;
  _size++; //in the end equals the _size of the _heap

  traverse(root->_left, array);
  traverse(root->_right, array);
}

///////////////////////////////////
void RQueue::rebuild(Node** array) {

  //Rebuild Helper function that creates a new RQueue and uses the Assignment Operator to change current RQueue 
  
  RQueue newQueue(priority); //creates different RQueue with new priority
  Student prior = Student(); //initializes a Student value used later
  
  for(int i=0; i<numStudents(); i++) {
    if(priority(array[i]->getStudent()) < priority(prior))
      prior = array[i]->getStudent(); //prior is set to the highest priority value (makes reinserting simple)
  }

  newQueue.insertHelp(newQueue._heap ,prior); //creates the new Queue with the correct root
  
  for(int i=0; i<numStudents(); i++) {
    if(array[i]->getStudent().getName() != prior.getName()) {
      newQueue.insertHelp(newQueue._heap ,array[i]->getStudent()); //inserts the rest of the student values
    }
  }
  
  *this = newQueue; //calls the assignment operator that sets current Queue to new Queue
  newQueue.clear(); //clears the new Queue to avoid mem leaks
}

// for debugging
void RQueue::dump() const
{
  if (_size == 0) {
    cout << "Empty skew heap.\n" ;
  } else {
    dump(_heap);
    cout << endl;
  }
}

// for debugging
void RQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->_left);
    cout << priority(pos->_student) << ":" << pos->_student.getName();
    dump(pos->_right);
    cout << ")";
  }
}

// overloaded insertion operator for Student class
ostream& operator<<(ostream& sout, const Student& student) {
  sout << "Student: " << student.getName() << ", priority: " << student.getPriority()
       << ", year: " << student.getYear() << ", major: " << student.getMajorStr()
       << ", group: " << student.getGroupStr();
  return sout;
}

// overloaded insertion operator for Node class
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getStudent();
  return sout;
}
