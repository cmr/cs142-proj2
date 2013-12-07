#include <iostream>
#include <istream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <vector>

#include "data.h"

using namespace std;
using namespace soccer;

namespace soccer {
	namespace data {
		std::ostream& category::operator<<(std::ostream& oStrStream, const category val) {
			oStrStream << "TestEnum";
			return oStrStream;
		}

		std::ostream& status::operator<<(std::ostream& oStrStream, const category val) {
			oStrStream << "TestEnum";
			return oStrStream;
		}
		istream& status::operator>>(istream& out) {

		}

		student::student(string name, int yob, enum status stat) {
			this->name = name;
			this->year_of_birth = yob;
			this->category = category::UUnknown;
			this->status = stat;
		}

		void student::redetermine_category(int year) {
			int age = year - this->year_of_birth;
			if (age < 0) {
				throw new runtime_error("somehow this person was born after the current year");
			}

			enum category cat;

			if (age < 4) {
				throw new runtime_error("student too young");
			} else if (age < 6) {
				cat = category::U6;
			} else if (age < 8) {
				cat = category::U8;
			} else if (age < 10) {
				cat = category::U10;
			} else if (age < 12) {
				cat = category::U12;
			} else if (age < 14) {
				cat = category::U14;
			} else if (age < 17) {
				cat = category::U17;
			} else {
				throw new runtime_error("student too old");
			}

			this->category = cat;

		}

		data_access::data_access() : data_access(string("soccer.db")) { }

		data_access::data_access(string filename) {
			fstream file(filename);
			if (!file) {
				throw runtime_error("could not open database file");
			}

			// why
			//this->file = file;

			this->file >> this->year;
			while (true) {
				student x = student::from_istream(this->file);
				this->students.push_back(x);
			}
		}

		student& data_access::find_student(string studentname) {
			auto it = find_if(this->students.begin(), this->students.end(), [&](student& s) { return s.name == studentname; });
			if (it == this->students.end()) {
				throw runtime_error("could not find student");
			}
			return *it;
		}

		void data_access::add_student(student s) {
			this->students.emplace_back(s);
		}

		void data_access::persist() {
			this->file.seekp(std::ios_base::beg);

			auto end = this->students.end();
			for (auto it = this->students.begin(); it != end; it++) {
				this->file << it->name;
				this->file << it->year_of_birth;
				this->file << it->category;
				this->file << it->status;
			}
		}

		vector<student> data_access::get_by_category(category cat) {
			vector<student> out;
			out.reserve(this->students.size());
			remove_copy_if(this->students.begin(), this->students.end(), back_inserter(out),
						   [&](student &s) { return s.category != cat; } );
			return out;
		}
	}
}
