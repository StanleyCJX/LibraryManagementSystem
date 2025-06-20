#define _CRT_SECURE_NO_WARNINGS 
#include	<iostream>
#include	<cstdlib>
#include	<cstdio>
#include	<iomanip>
#include    <fstream>
#include    <string>
#include	"List.hpp"
#include    "LibStudent.hpp"
#include    "LibBook.hpp"
using namespace std;

struct Course {
    char name[10];
    int totalStudent;
    int totalbook;
    int totaloverb;
    double totalfine;
    Course();
    void print();
};



bool ReadFile(string, List *);
bool DeleteRecord(List *, char *);
bool Display(List, int, int);
bool InsertBook(string, List *);
bool SearchStudent(List *, char *id, LibStudent &);
bool computeAndDisplayStatistics(List *);
bool printStuWithSameBook(List *, char *);
bool displayWarnedStudent(List *, List *, List *);
void underToSpace(char[]);
int numDueDays(Date, int, int, int);
int numDaysPerMonth(int, int);
bool leapYear(int);


int menu();


int main() {
	menu();
	cout << "\n\n";
	system("pause");
	return 0;;
}



bool ReadFile(string filename, List* list) {

	ifstream inFile(filename);
	LibStudent currentStudent;
	string line;
	int numField = 0, numRecord = 0;
	bool duplicate = false;

	//If the file can't be found, function terminates 
	if (!inFile.is_open())
		return false;
	//Extract data and store in node 
	while (getline(inFile, line)) {
		//Skip empty lines 
		if (line.empty())
			continue;
		//Extract data of current line 
		if (line.find("Student Id = ") != string::npos) {
			strncpy_s(currentStudent.id, line.substr(13).c_str(), 10);
			numField++;
		}
		else if (line.find("Name = ") != string::npos) {
			strncpy_s(currentStudent.name, line.substr(7).c_str(), 30);
			numField++;
		}
		else if (line.find("course = ") != string::npos) {
			strncpy_s(currentStudent.course, line.substr(9).c_str(), 3);
			numField++;
		}
		else if (line.find("Phone Number = ") != string::npos) {
			strncpy_s(currentStudent.phone_no, line.substr(15).c_str(), 10);
			numField++;
		}
		//If all four data are collected, a single student data is complete 
		if (numField == 4) {
			numField = 0;
			//Check for duplicates 
			for (int i = 1; i <= list->size(); i++) {
				if (strcmp((list->find(i))->item.id, currentStudent.id) == 0) {
					//If true: then its the same, hence a duplicate 
					duplicate = true;
					break;
				}
			}
			//Insert data into the list 
			if (!duplicate) {
				list->insert(currentStudent);
				numRecord++;
			}
		}
	}
	inFile.close();
	//Output number of records added into the list (skipped duplicates) 
	if (numRecord > 0)
		cout << numRecord << " records have been successfully read." << endl;
	else
		cout << "No new record has been successfully read.\n";
	return true;
}

bool DeleteRecord(List* list, char* id) {
	//Check if student exists in the list 
	for (int i = 1; i <= list->size(); i++) {
		if (strcmp(list->find(i)->item.id, id) == 0) {
			//If true: then remove this record 
			cout << "\nStudent " << list->find(i)->item.name << " with ID " << list->find(i)->item.id << "'s record has been deleted.\n";
			list->remove(i);
			return true;
		}
	}
	//Student does not exist in list 
	return false;
}

bool SearchStudent(List* list, char* id, LibStudent& stu)
{
    for (int i = 1; i <= list->size(); i++)
    {
        if (strcmp(list->find(i)->item.id, id) == 0)    //if the input id is same as in id in node
        {
            list->get(i, stu);  //get student information 
            stu.print(cout);
            return true;
        }
    }
    return false;
}

bool InsertBook(string filename, List* list)
{
    if (list->empty())  //if the list is empty 
    {
        cout << "Student record not found! Please insert student information.\n";
        return false;
    }

    ifstream inFile(filename);
    if (!inFile.is_open())  //if the file is invalid
    {
        cout << "Book record file not found!\n";
        return false;
    }

    LibBook* book = new LibBook();
    char idBook[8], authorBook[50], borrowBook[11], dueBook[11];

    //Read the file
    while (inFile >> idBook)
    {
        inFile >> authorBook;
        int numberAuthor = 0;
        char* tok = strtok(authorBook, "/");    //Split the input string separated by the delimiter "/" and store into tok pointer

        while (tok != NULL)
        {
            underToSpace(tok);  //pass tok (storing one author name) to the function  
            book->author[numberAuthor] = new char[10];
            strcpy(book->author[numberAuthor], tok);
            numberAuthor++;
            tok = strtok(NULL, "/");    //Continue to tokenize the string
        }

        inFile >> book->title;
        underToSpace(book->title);

        inFile >> book->publisher;
        underToSpace(book->publisher);

        inFile >> book->ISBN;
        inFile >> book->yearPublished;
        inFile >> book->callNum;


        inFile >> borrowBook;

        //store day, month, and year separately into LibBook's Date's object
        tok = strtok(borrowBook, "/");
        book->borrow.day = stoi(tok);   //convert from string to integer

        tok = strtok(NULL, "/");
        book->borrow.month = stoi(tok);

        tok = strtok(NULL, "/");
        book->borrow.year = stoi(tok);


        inFile >> dueBook;

        //store day, month, and year separately into LibBook's Date's object
        tok = strtok(dueBook, "/");
        book->due.day = stoi(tok);

        tok = strtok(NULL, "/");
        book->due.month = stoi(tok);

        tok = strtok(NULL, "/");
        book->due.year = stoi(tok);

        LibStudent* student = new LibStudent();

        book->fine = 0.50 * numDueDays(book->due, 29, 3, 2020);
        for (int i = 1; i <= list->size(); i++)
        {
            student = &list->find(i)->item;

            if (strcmp(student->id, idBook) == 0 && student->totalbook < 15)
            {
                bool duplicate = false;

                for (int j = 0; j < student->totalbook; j++)
                {
                    if (book->compareCallNum(student->book[j]) && (book->borrow.day == student->book[j].borrow.day && book->borrow.month == student->book[j].borrow.month && book->borrow.year == student->book[j].borrow.year))
                    {
                        cout << "\nBook borrowed by student " << student->name << " is duplicate.";
                        duplicate = true;
                        break;
                    }
                }
                if (!duplicate)
                {
                    student->book[student->totalbook] = *book;
                    student->totalbook++;
                    student->calculateTotalFine();
                }
            }
            else if (strcmp(student->id, idBook) == 0 && student->totalbook >= 15)
            {
                cout << "Student " << student->name << " has reached the maximum amount of books that can be borrowed. " << endl;
                cout << "Book info with call number : " << book->callNum << " will be ignored." << endl;
            }
        }
    }
    inFile.close();
    return true;
}




bool Display(List* list, int source, int detail)
{
    LibStudent student;
    if (source == 2 && detail == 1) //display on screen with book list of every student  
    {
        if (list->empty())
        {
            cout << "there are no any record" << endl;
            return false;
        }
        for (int i = 1; i <= list->size(); i++)
        {
            if (!list->get(i, student))
            {
                return false;
            }
            cout << "\nSTUDENT" << "  " << i;
            student.print(cout);
            cout << endl;
            cout << "BOOK LIST:" << endl << endl;
            for (int j = 0; j < student.totalbook; j++)
            {
                cout << "BOOK " << j + 1;
                student.book[j].print(cout);
                cout << endl;
            }
            cout << "*****************************************************************************\n\n";
        }

    }
    else if (source == 1 && detail == 1) //display into file with book list of every student  
    {
        if (list->empty())
        {
            cout << "there are no any record" << endl;
            return false;
        }
        ofstream outfile;
        outfile.open("student_booklist.txt");
        if (!outfile)
        {
            cout << "outfile cannot open" << endl;
            return false;
        }
        for (int i = 1; i <= list->size(); i++) {
            if (!list->get(i, student))
                return false;
            outfile << "\nSTUDENT" << "  " << i;
            student.print(outfile);
            outfile << endl;
            outfile << "BOOK LIST:" << endl << endl;
            for (int j = 0; j < student.totalbook; j++)
            {
                outfile << "BOOK " << j + 1;
                student.book[j].print(outfile);
                outfile << endl;
            }
            outfile << "*****************************************************************************\n\n";
        }
        outfile.close();
    }

    else if (source == 2 && detail == 2) //display on the screen without book list of every student)   
    {
        if (list->empty())
        {
            cout << "there are no record" << endl;
            return false;
        }

        for (int i = 1; i <= list->size(); i++)
        {
            if (!list->get(i, student))
                return false;
            cout << "\nSTUDENT" << "  " << i;
            student.print(cout);
            cout << endl;
            cout << "*****************************************************************************\n\n";
        }
    }

    else if (source == 1 && detail == 2) //display into file without book list of every student  
    {
        if (list->empty())
            return false;

        ofstream outfile;
        outfile.open("student_info.txt");
        for (int i = 1; i <= list->size(); i++)
        {
            if (!list->get(i, student))
                return false;
            outfile << "\nSTUDENT" << "  " << i;
            student.print(outfile);
            outfile << endl;
            outfile << "*****************************************************************************\n\n";
        }
        outfile.close();

    }

    else
    {
        cout << "wrong input" << endl;
        return false;
    }

    return true;
}

bool computeAndDisplayStatistics(List* a)
{
    Node* cur = a->head;
    Course course[5];
    strcpy(course[0].name, "CS");
    strcpy(course[1].name, "IA");
    strcpy(course[2].name, "IB");
    strcpy(course[3].name, "CN");
    strcpy(course[4].name, "CT");

    if (a->empty())
        return false;

    while (cur != NULL)
    {
        for (int i = 0; i < 5; i++)
        {
            if (strcmp(cur->item.course, course[i].name) == 0)
            {
                course[i].totalStudent++;
                course[i].totalbook += cur->item.totalbook;
                for (int j = 0; j < cur->item.totalbook; j++)
                {
                    if (cur->item.book[j].fine > 0)
                    {
                        course[i].totaloverb++;
                        course[i].totalfine += cur->item.book[j].fine;
                    }
                }
                break;

            }

        }
        cur = cur->next;
    }

    cout << "Course" << setw(28) << "Number of Students " << setw(28) << "Total Books Borrowed " << setw(28) << "Total Overdue Books " << setw(28) << "Total Overdue Fine (RM)" << endl;
    for (int i = 0; i < 5; i++)
    {
        course[i].print();
    }
    return true;

}



bool printStuWithSameBook(List* list, char* callNum) {
    int total = 0;
    Node* cur = list->head;

    //if the list empty, return false 
    if (list->empty()) {
        return false;
    }

    while (cur != NULL) {
        //Traverse total book of each student to search same book borrowed 
        for (int i = 0; i < cur->item.totalbook; i++) {
            if (strcmp(cur->item.book[i].callNum, callNum) == 0) {
                total++;
                break; // Stop checking this student if one book matches 
            }
        }
        cur = cur->next;
    }

    //Print message of no student borrow same book 
    if (total == 0)
        cout << "\nThere are no student that borrow the book with call number of " << callNum << ".\n";

    //Print all the students that borrow this book with borrow date and due date 
    else {
        cout << "There are " << total << " students that borrow the book with call number " << callNum << " as shown below:";

        cur = list->head;

        for (int j = 1; j <= list->size(); j++) {

            for (int k = 0; k < cur->item.totalbook; k++) {

                if (strcmp(cur->item.book[k].callNum, callNum) == 0) {

                    cout << "\nStudent Id = " << list->find(j)->item.id;
                    cout << "\nName = " << list->find(j)->item.name;
                    cout << "\nCourse = " << list->find(j)->item.course;
                    cout << "\nPhone Number = " << list->find(j)->item.phone_no;
                    cout << "\nBorrow Date: ";
                    cur->item.book[k].borrow.print(cout);
                    cout << "\nDue Date: ";
                    cur->item.book[k].due.print(cout);
                    cout << endl;

                    break;
                }
            }
            cur = cur->next;
        }
    }
    return true;
}

bool displayWarnedStudent(List* list, List* type1, List* type2) {

    //if the list empty, return false 
    if (list->empty()) {
        return false;
    }

    Node* cur = list->head;

    //traverse the list 
    while (cur != NULL)
    {
        int numOfBookOverdue10 = 0;
        int numOfBookOverdue = 0;

        //traverse book list of student 
        for (int i = 0; i < cur->item.totalbook; i++) {

            //define the overdueDays 
            int overdueDays = numDueDays(cur->item.book[i].due, 29, 3, 2020);

            //student has overdue the book 
            if (overdueDays > 0) {
                numOfBookOverdue++;
            }

            //student has overdue the book for 10 days or more than 10 days 
            if (overdueDays >= 10) {
                numOfBookOverdue10++;
            }
        }

        //insert total fine of student more than RM 50 to type2 
        if (cur->item.total_fine > 50 && cur->item.totalbook == numOfBookOverdue) {
            type2->insert(cur->item);
        }

        //insert student more than 2 book overdue for 10 days to type1 
        if (numOfBookOverdue10 > 2) {
            type1->insert(cur->item);
        }

        cur = cur->next;
    }

    //display type1 
    if (type1->empty())
        cout << "\nNo student have type1 offence.\n";

    else {
        cout << "\nType 1 offence (More than 2 books that are overdue for >= 10 days.)\n";
        for (int i = 1; i <= type1->size(); i++) {
            Node* cur1 = type1->find(i);
            cur1->item.print(cout);
            for (int j = 0; j < type1->find(i)->item.totalbook; j++) {
                type1->find(i)->item.book[j].print(cout);
            }
            cout << "\n*****************************************************************************\n";
        }
    }

    //display type2 
    if (type2->empty())
        cout << "\nNo student have type2 offence.\n";

    else {
        cout << "\nType 2 offence (Total fine is more than RM50.00 and every book is overdue.)\n";
        for (int i = 1; i <= type2->size(); i++) {
            Node* cur1 = type2->find(i);
            cur1->item.print(cout);
            for (int j = 0; j < type2->find(i)->item.totalbook; j++) {
                type2->find(i)->item.book[j].print(cout);
            }
            cout << "\n*****************************************************************************\n";
        }
    }

    return true;

}



int menu()
{
    List* list = new List();
    List* type1 = new List();
    List* type2 = new List();
    char id[10];
    char callNum[20];
    LibStudent temp;
    int choice;
    bool contRL = true;
    void emptyCinBuffer();

    // menu loops until user choose to exit 
    while (contRL == true) {
        // display menu 
        cout << "\n\nMenu\n";
        cout << "\n1. Read file\n";
        cout << "2. Delete record\n";
        cout << "3. Search student\n";
        cout << "4. Insert book\n";
        cout << "5. Display output\n";
        cout << "6. Compute and Display Statistics\n";
        cout << "7. Student with Same Book\n";
        cout << "8. Display Warned Student\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        emptyCinBuffer();

        switch (choice) {
        case 1:
            cout << "\nREAD FILE\n\n";
            //If function return false, file not found 
            if (!ReadFile("student.txt", list)) {
                cout << "Could not read file.\n";
            }
            break;
        case 2:
            cout << "\nDELETE RECORD\n\n";
            cout << "Enter the student's ID that you want to delete: ";
            cin >> id;
            emptyCinBuffer();
            //if function return false: student does not exist 
            if (!DeleteRecord(list, id)) {
                cout << "\nCould not find student.\n";
            }
            break;
        case 3:
            cout << "\nSEARCH STUDENT\n\n";
            cout << "Enter the student's ID that you want to search: ";
            cin >> id;
            emptyCinBuffer();
            //if function return false: student cannot be found in list 
            if (!SearchStudent(list, id, temp)) {
                cout << "\nCould not find student.\n";
            }
            break;
        case 4:
            cout << "\nINSERT BOOK\n\n";
            //if function return true: book is inserted successfully 
            if (InsertBook("book.txt", list)) {
                cout << "\n\nBooks inserted succesfully to student list.\n";
            }
            break;
        case 5:
            int source;
            int detail;
            cout << "\nDISPLAY OUTPUT\n\n";
            cout << "Where do you want to display the output (1 - File / 2 - Screen): ";
            cin >> source;
            emptyCinBuffer();
            cout << "\nDo you want to display book list for every student (1 - YES / 2 - NO): ";
            cin >> detail;
            cout << "\n";
            emptyCinBuffer();
            //if function return true: display the corresponding success message 
            if (Display(list, source, detail)) {
                if (source == 1) {
                    cout << "\nSuccessfully display output into file!\n";
                }
                else if (source == 2) {
                    cout << "\nSuccessfully display output on screen!\n";
                }
            }
            break;
        case 6:
            cout << "\nCOMPUTE AND DISPLAY STATISTICS\n\n";
            //if function return false: the list is empty 
            if (!computeAndDisplayStatistics(list)) {
                cout << "No information can be computed and display!\n";
            }
            break;
        case 7:
            cout << "\nSTUDENT WITH SAME BOOK\n\n";
            cout << "Enter the call number of the book you want to know: ";
            cin >> callNum;
            emptyCinBuffer();
            //if function return false: the list is empty 
            if (!printStuWithSameBook(list, callNum)) {
                cout << "\nList is empty!\n";
            }
            break;
        case 8:
            cout << "\nDISPLAY WARNED STUDENT\n\n";
            //if function return false: the list is empty 
            if (!displayWarnedStudent(list, type1, type2)) {
                cout << "List is empty!\n";
            }
            break;
        case 9:
            //user chooses to exit system 
            contRL = false;
            break;
        default:
            //user enters invalid choice 
            cout << "\nInvalid choice\n";
        }
    };
    return 0;
}


//empty the cin buffer 
void emptyCinBuffer() {
    cin.clear();
    cin.ignore(1000, '\n');
}

//Function to convert underscore to space 
void underToSpace(char word[])
{
    for (int i = 0; word[i] != NULL; i++)
    {
        if (word[i] == '_')
            word[i] = ' ';
    }
}

//Function to determine the number of due days 
int numDueDays(Date dueDate, int curentDay, int currentMonth, int currentYear)
{
    int numDueDays = 0;
    // Check if the book has passed its due date
    if (currentYear > dueDate.year || (currentYear == dueDate.year && currentMonth > dueDate.month) || (currentYear == dueDate.year && currentMonth == dueDate.month && curentDay > dueDate.day))
    {

        // Calculate the number of due days 
        while (currentYear > dueDate.year || currentMonth > dueDate.month || curentDay > dueDate.day)
        {
            numDueDays++;
            dueDate.day++;

            // If the current month reaches to the last day, move to the next month
            if (dueDate.day > numDaysPerMonth(dueDate.month, dueDate.year))
            {
                dueDate.day = 1;
                dueDate.month++;

                // Iff the current year reaches the last month, move to the next year
                if (dueDate.month > 12)
                {
                    dueDate.month = 1;
                    dueDate.year++;
                }
            }
        }
    }
    return numDueDays;
}


//Function to determine the number of days in a month
int numDaysPerMonth(int month, int year)
{
    if (month == 2)
    {
        if (leapYear(year))
        {
            return 29;
        }

        else
        {
            return 28;
        }
    }

    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
    {
        return 31;
    }

    else
    {
        return 30;
    }
}


//Function to determine if the year is a leap year
bool leapYear(int year)
{
    return ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0));
}

Course::Course()
{
    strcpy(name, "");
    totalStudent = 0;
    totalbook = 0;
    totaloverb = 0;
    totalfine = 0;
}

void Course::print()
{
    cout << setw(4) << name << setw(20) << totalStudent << setw(26) << totalbook << setw(28) << totaloverb << setw(28) << fixed << setprecision(2) << totalfine << endl;

}












