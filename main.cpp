#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

//"Hello"

struct Lecturer {
    string lecID;
    string lecName;
    string lecEmail;
    string lecPhone;
    Lecturer *nxtLec;
} *frontLec, *rearLec = NULL;
Lecturer* lecA = new Lecturer{"L01", "Alice Kee", "alice@uow.edu.my", "0112345678", NULL};
Lecturer* lecB = new Lecturer{"L02", "Bryan Law", "bryan@uow.edu.my", "0123456789", NULL};
Lecturer* lecC = new Lecturer{"L03", "Carol Foo", "carol@uow.edu.my", "0134567891", NULL};
Lecturer* lecD = new Lecturer{"L04", "Damien Ho", "damien@uow.edu.my", "0145678912", NULL};
Lecturer* lecE = new Lecturer{"L05", "Emily Tan", "emily@uow.edu.my", "0156789123", NULL};

struct Student {
    string studID;
    string studName;
    string studEmail;
    string studPhone;
    string studCourse;
    Student *nxtStud;
} *frontStud, *rearStud = NULL;
Student* studA = new Student{"S01", "Allison Lee", "allison@uow.edu.my", "0198765432", "USENG", NULL};
Student* studB = new Student{"S02", "Brook Yap", "brook@uow.edu.my", "0187654321", "UCOMMS", NULL};
Student* studC = new Student{"S03", "Cameron Tan", "cameron@uow.edu.my", "0176543212", "UEIM", NULL};
Student* studD = new Student{"S04", "Diana Chin", "diana@uow.edu.my", "0165432123", "USENG", NULL};
Student* studE = new Student{"S05", "Elliot Khoo", "elliot@uow.edu.my", "0154329876", "UBUSS", NULL};

// Struct for Schedule of Booked Consultations(schedType: B), Available Consultations (schedType: A), and Lecturer's Class Time (schedType: C)
// Lecturer's Class Time is when the lecturer is not available for consultations
struct Schedule {
    tm schedDateTime;
    string subject;
    string venue;
    string lecID;
    string studID;
    char schedType;
    Schedule *nxtSched;
} *frontSched, *rearSched = NULL;


// Function Declaration
int continueSession(int action);

Lecturer* searchLec(Lecturer* f, string inID);

Student* searchStud(Student* f, string inID);

void displaySchedule(Schedule* f, string inputID, char schedType);

bool validateDate(tm* timestruct);

int main() {
    lecA->nxtLec = lecB;
    lecB->nxtLec = lecC;
    lecC->nxtLec = lecD;
    lecD->nxtLec = lecE;
    Lecturer* lecFront = lecA;

    studA->nxtStud = studB;
    studB->nxtStud = studC;
    studC->nxtStud = studD;
    studD->nxtStud = studE;
    Student* studFront = studA;

    Schedule* weekSched = new Schedule;
    Schedule* consultSched = new Schedule;

    char user = 'X';

    do {
        cout << "\nWelcome to the UOWMKDU Appointment Booking System!" << endl;
        cout << "\nEnter S for Student and L for Lecturer: ";
        cin >> user;
        cin.ignore();
        user = toupper(user);
        
        if (user == 'L') {

            // Lecturer's Interface            
            Lecturer* currentLec = new Lecturer;

            // Verify if lecturer exists
            do {
                cout << "\nLecturer ID: ";
                string inID;
                getline(cin, inID, '\n');
                transform(inID.begin(), inID.end(), inID.begin(), ::toupper);

                // Initialize currentLec by searching linked list with inID user input
                currentLec = searchLec(lecFront, inID);

                if (currentLec == NULL) {
                    cout << "\nLecturer not found. Please try again." << endl;
                }
            } while (currentLec == NULL);

            // Continue session based on user's input - action
            int action;
            do {
                cout << "\nWelcome, " << currentLec->lecName << "!\n";

                cout << "\n1. Set Schedule for Consultation" << endl;
                cout << "2. View Consultation Schedule" << endl;
                cout << "3. View Students with Booked Consultation" << endl;
                cout << "4. View Full Schedule" << endl;
                cout << "5. Edit Consultation Slot Information" << endl;
                cout << "6. Delete Consultation Slot" << endl;

                cout << "\nPlease select your desired action by entering integers 1, 2, 3, 4, 5, or 6, based on the corresponding functions above." << endl;
                cout << "Action: ";
                cin >> action;

                char choice;
                if (action == 1) {
                    // 1. Set Schedule for Consultation

                    // TODO: This is hardcoded just for testing purposes
                    tm sampleDate = {0, 0, 8, 23, 3, 123};
                    Schedule* schedA = new Schedule {sampleDate, "Software", "COM-1", "L01", "S01", 'A', NULL};
                    frontSched = schedA;

                    displaySchedule(frontSched, currentLec->lecID, 'A');

                    action = continueSession(action);
                } else if (action == 2) {
                    //2. View Consultation Schedule" << endl;
                    action = continueSession(action);
                } else if (action == 3) {
                    // 3. View Students with Booked Consultation
                    action = continueSession(action);
                } else if (action == 4) {
                    // 4. View Full Schedule
                    action = continueSession(action);
                } else if (action == 5) {
                    // 5. Edit Consultation Slot Information
                    action = continueSession(action);
                } else if (action == 6) {
                    // 6. Delete Consultation Slot
                    action = continueSession(action);
                } else {
                    cout << "Please only enter the integers 1, 2, 3, 4, 5, or 6." << endl;
                    action = 0;
                }
            } while (action == 0);

        } else if (user == 'S') {

            // Student's Interface
            Student* currentStud = new Student;
            
            // Verify if student exists
            do {
                cout << "\nStudent ID: ";
                string inID;
                getline(cin, inID, '\n');
                transform(inID.begin(), inID.end(), inID.begin(), ::toupper);

                // Initialize currentStud by searching linked list with inID user input
                currentStud = searchStud(studFront, inID);

                if (currentStud == NULL) {
                    cout << "\nStudent not found. Please try again." << endl;
                }
            } while (currentStud == NULL);

            // Continue session based on user's input - action
            int action;
            do {
                cout << "\nWelcome, " << currentStud->studName << "!\n";

                cout << "\n1. View and Book Consulation Slots" << endl;
                cout << "2. View Booked Consultation Slots" << endl;
                cout << "3. View Booked Consultation History" << endl;
                cout << "4. Delete Booked Consultation Slots" << endl;

                cout << "\nPlease select your desired action by entering integers 1, 2, 3, or 4, based on the corresponding functions above." << endl;
                cout << "Action: ";
                cin >> action;

                char choice;
                if (action == 1) {
                    // 1. View and Book Consulation Slots
                    action = continueSession(action);
                } else if (action == 2) {
                    // 2. View Booked Consultation Slots
                    action = continueSession(action);
                } else if (action == 3) {
                    // 3. View Booked Consultation History
                    action = continueSession(action);
                } else if (action == 4) {
                    // 4. Delete Booked Consultation Slot
                    action = continueSession(action);
                } else {
                    cout << "Please only enter the integers 1, 2, 3, or 4." << endl;
                    action = 0;
                }
            } while (action == 0);

        } else {
            // Invalid User Role
            cout << "\nPlease only enter the letters S or L." << endl;
            user = 'N';
        }

    } while (user != 'X');

}

// Function Definition
int continueSession(int action) {
    cout << "\nEnter 'Y' if you wish to continue your session." << endl;
    cout << "Action: ";
    char choice;
    cin >> choice;
    choice = toupper(choice);

    if (choice == 'Y') {
        return 0;
    }

    return action;
}

Lecturer* searchLec(Lecturer* f, string inputID) {
    Lecturer* currentLec = f;
    
    while (currentLec != NULL) {
        if(currentLec->lecID == inputID) {
            return currentLec;
        }
        currentLec = currentLec->nxtLec;
    }

    return NULL;
}

Student* searchStud(Student* f, string inputID) {
    Student* currentStud = f;
    
    while (currentStud != NULL) {
        if(currentStud->studID == inputID) {
            return currentStud;
        }
        currentStud = currentStud->nxtStud;
    }

    return NULL;
}

void displaySchedule(Schedule* f, string inputID, char schedType) {
    Schedule* currentSched = f;

    cout << "+--------------+--------------+----------------------+----------------------+--------------+--------------+-----------+" << endl;
    cout << "| Date         | Time         | Subject              | Venue                | Lecturer ID  | Student ID   | Type      |" << endl;
    cout << "+--------------+--------------+----------------------+----------------------+--------------+--------------+-----------+" << endl;

    while (currentSched != NULL) {
        if (schedType == 'A') {
            if (inputID.at(0) == 'L') {
                if (currentSched->lecID == inputID) {
                    // TODO: Deal with this

                    //  cout << "| " << currentSched->schedDate.tm_mday <<          | Time         | Subject              | Venue                | Lecturer ID  | Student ID   | Type      |" << endl;

                    // cout << currentSched->schedDate.tm_hour << endl;
                }
            } else {

            }
        } else if (schedType == 'B') {

        } else {

        }
        currentSched = currentSched->nxtSched;
    }

    cout << "+--------------+--------------+----------------------+----------------------+--------------+--------------+-----------+" << endl;
}

// Date validation (Just format, not including time restraints)
// TODO: Deal with this later
// bool validateDate(tm* timestruct) {
// 	struct tm copy{};
// 	copy.tm_sec = timestruct->tm_sec;
// 	copy.tm_min = timestruct->tm_min;
// 	copy.tm_hour = timestruct->tm_hour;
// 	copy.tm_mday = timestruct->tm_mday;
// 	copy.tm_mon = timestruct->tm_mon;
// 	copy.tm_year = timestruct->tm_year;
// 	copy.tm_wday = timestruct->tm_wday;
// 	copy.tm_yday = timestruct->tm_yday;
// 	copy.tm_isdst = timestruct->tm_isdst;

// 	time_t res = mktime(&copy);
// 	if (res < 0)
// 	{
// 		return false;
// 	}
// 	if (copy.tm_mday != timestruct->tm_mday || copy.tm_mon != timestruct->tm_mon || copy.tm_year != timestruct->tm_year)
// 	{
// 		return false;
// 	}
// 	return true;
// }