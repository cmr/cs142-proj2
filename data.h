#include <string>
#include <vector>
#include <istream>
#include <iostream>

namespace soccer {
	namespace data {
		enum class category {
			U6,
			U8,
			U10,
			U12,
			U14,
			U17,
			UUnknown;

			std::ostream& operator<<(std::ostream& oStrStream, const category val);
		};

		enum class status {
			Paid,
			Unpaid
			std::ostream& operator<<(std::ostream& oStrStream, const category val);
		};

		class student {
			public:
				std::string name;
				int year_of_birth;
				category category;
				status status;

/*				student();
				student(std::string name);
				student(std::string name, int year_of_birth); */
				student(std::string name, int year_of_birth, enum status stat);
				static student& from_istream(std::istream& file);
				void redetermine_category(int year);
		};

		class data_access {
			std::vector<student> students;
			std::fstream file;
			int year;

			public:
				/**
				 * Uses the default filename of "soccer.db"
				 */
				data_access();

				/**
				 * Open a file with the given name as a database.
				 */
				data_access(std::string filename);

				student& find_student(std::string);
				void add_student(student);
				void persist();

				// TODO: use an iterator, zero-copy
				std::vector<student> get_by_category(category cat);
		};
	}
}

