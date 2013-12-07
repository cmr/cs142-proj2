#include "data.h"

using namespace std;
using namespace soccer::data;

int main(void) {
	data_access d("testing.db", 2013);
	student s("Corey Richardson", 2000, soccer::data::status::Paid);
	d.add_student(s);
	d.persist();
}
