#include "inmemorydatabase.h"
#include "inewsgroup.h"

using namespace main;

int main() {
	InMemoryDatabase db;
	shared_ptr<INewsgroup> db.add_newsgroup(1, "group1");
	shared_ptr<INewsgroup> db.get_newsgroup(1);


	
