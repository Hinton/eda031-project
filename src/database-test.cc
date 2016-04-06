#include "idatabase.h"
#include "inewsgroup.h"
#include <iostream>

using namespace std;

class DatabaseMock : public IDatabase
{
public:
  DatabaseMock() {
    cout << "hejs" << endl;
  };

  virtual ~DatabaseMock() = default;
};

class NewsgroupMock : public INewsgroup 
{
public:
  NewsgroupMock() : INewsgroup(2, "hej") {
    cout << "hallå" << endl;
  };

  virtual ~NewsgroupMock() = default;
};

int main(int argc, char const *argv[])
{
  DatabaseMock db;
  NewsgroupMock newsgroup;
  //NewsgroupMock newsgroupp(2, "hej");
  return 0;
}