#include <iostream>
#include <string>
#include "class1.h"
#include "class2.h"

using namespace std;

int main()
{
  Class1 c1(3);
  Class2 c2("hi ");
  Class2 c3("no ");
  Class2 c4("hiiii ");
  cout << c1.getX() << endl;
  cout << c2.getS() << endl;
  cout << "Hello World" << endl;
}
