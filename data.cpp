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
		ostream& operator<<(ostream& os, const enum category val) {
			switch (val) {
				case category::U6:
					os << "U6";
					break;
				case category::U8:
					os << "U8";
					break;
				case category::U10:
					os << "U10";
					break;
				case category::U12:
					os << "U12";
					break;
				case category::U14:
					os << "U14";
					break;
				case category::U17:
					os << "U17";
					break;
				default:
					throw runtime_error("invalid value for `category`");
			}

			return os;
		}

		ostream& operator<<(ostream& os, const enum status val) {
			switch (val) {
				case status::Paid:
					os << "Paid";
					break;
				case status::Unpaid:
					os << "Unpaid";
					break;
				default:
					throw runtime_error("invalid value for `status`");
			}

			return os;
		}

		istream& operator>>(istream& is, enum status& val) {
			string s;
			is >> s;
			if (s == "Paid") {
				val = status::Paid;
			} else if (s == "Unpaid") {
				val = status::Unpaid;
			} else {
				throw runtime_error("invalid file");
			}

			return is;
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
				throw runtime_error("somehow this person was born after the current year");
			}

			enum category cat;

			if (age < 4) {
				throw runtime_error("student too young");
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
				throw runtime_error("student too old");
			}

			this->category = cat;

		}

		student student::from_istream(std::istream& is) {
			string name, unused;
			int yob;
			enum status stat;

			is >> name >> yob >> unused >> stat;

			return student(name, yob, stat);
		}

		data_access::data_access() : data_access(string("soccer.db")) { }

		data_access::data_access(string filename) {
			// delegating constructor doesn't quite work here, so
			// unfortunately duplicate it :(
			fstream file(filename, ios_base::out | ios_base::in);
			if (!file) {
				throw runtime_error("could not open database file");
			}

			// why
			//this->file = file;

			this->file >> this->year;
			while (true) {
				student x = student::from_istream(this->file);
				x.redetermine_category(this->year);
				this->students.push_back(x);
			}
		}

		data_access::data_access(string filename, int date) {
			// delegating constructor doesn't quite work here, so
			// unfortunately duplicate it :(
			this->year = date;
			fstream file(filename, ios_base::in | ios_base::out);
			if (!file) {
				throw runtime_error("could not open database file");
			}

			// why
			//this->file = file;
		}

		student& data_access::find_student(string studentname) {
			auto it = find_if(this->students.begin(), this->students.end(), [&](student& s) { return s.name == studentname; });
			if (it == this->students.end()) {
				throw runtime_error("could not find student");
			}
			return *it;
		}

		void data_access::add_student(student s) {
			s.redetermine_category(this->year);
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
