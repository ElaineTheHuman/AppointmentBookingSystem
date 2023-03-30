#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

using namespace std;

struct Lecturer {
    string lecID;
    string lecName;
    string lecEmail;
    string lecPhone;
    Lecturer *nxtLec;
} *frontLec, *rearLec = NULL;
// Existing Lecturers
Lecturer *lecA = new Lecturer{"L01", "Alice Kee", "alice@uow.edu.my", "0112345678", NULL};
Lecturer *lecB = new Lecturer{"L02", "Bryan Law", "bryan@uow.edu.my", "0123456789", NULL};
Lecturer *lecC = new Lecturer{"L03", "Carol Foo", "carol@uow.edu.my", "0134567891", NULL};
Lecturer *lecD = new Lecturer{"L04", "Damien Ho", "damien@uow.edu.my", "0145678912", NULL};
Lecturer *lecE = new Lecturer{"L05", "Emily Tan", "emily@uow.edu.my", "0156789123", NULL};

struct Student {
    string studID;
    string studName;
    string studEmail;
    string studPhone;
    string studCourse;
    Student *nxtStud;
} *frontStud, *rearStud = NULL;
// Existing Students
Student *studA = new Student{"S01", "Allison Lee", "allison@uow.edu.my", "0198765432", "USENG", NULL};
Student *studB = new Student{"S02", "Brook Yap", "brook@uow.edu.my", "0187654321", "UCOMMS", NULL};
Student *studC = new Student{"S03", "Cameron Tan", "cameron@uow.edu.my", "0176543212", "UEIM", NULL};
Student *studD = new Student{"S04", "Diana Chin", "diana@uow.edu.my", "0165432123", "USENG", NULL};
Student *studE = new Student{"S05", "Elliot Khoo", "elliot@uow.edu.my", "0154329876", "UBUSS", NULL};

// Struct for Schedule of Booked Consultations(schedType: B) and Available Consultations (schedType: A)
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

char promptUser(string msg);

bool isSameDate(tm date1, tm date2);

bool isDateWithin7DaysFromNow(tm inputDate);

Lecturer *searchLec(string inID);

Student *searchStud(string inID);

bool validateDate(tm *timestruct);

void deleteSchedule(tm schedDateTime);

void addSchedule(tm schedDateTime, string subject, string venue, string lecID, string studID, char schedType);

void viewSchedulesForLecturer(Lecturer *lecturer);

void viewAvailableConsultation(Lecturer *lecturer);

void viewSchedulesForStudent(Student *student);

void viewBookingHistory(Student *student);

void viewStudentsWithBookedConsultation(Lecturer *lecturer);

Schedule *searchScheduleByDateTime(tm schedDateTime);


int main() {
    lecA->nxtLec = lecB;
    lecB->nxtLec = lecC;
    lecC->nxtLec = lecD;
    lecD->nxtLec = lecE;
    frontLec = lecA;

    studA->nxtStud = studB;
    studB->nxtStud = studC;
    studC->nxtStud = studD;
    studD->nxtStud = studE;
    frontStud = studA;

    // Existing schedules already added into the system
    tm schedDateTime1 = {0, 0, 10, 1, 3, 123}; // 1st April, 10am
    tm schedDateTime2 = {0, 0, 15, 2, 3, 123}; // 2nd April, 3pm
    tm schedDateTime3 = {0, 0, 8, 3, 3, 123}; // 3rd April, 8am
    tm schedDateTime4 = {0, 0, 14, 4, 3, 123}; // 4th April, 2pm
    tm schedDateTime5 = {0, 0, 12, 5, 3, 123}; // 5th April, 12pm
    tm schedDateTime6 = {0, 0, 12, 5, 2, 123}; // 5th March, 12pm
    addSchedule(schedDateTime1, "", "MS Teams", "L01", "", 'A');
    addSchedule(schedDateTime3, "IT Project Management", "03-CRM-13", "L01", "S01", 'B');
    addSchedule(schedDateTime2, "", "MS Teams", "L02", "", 'A');
    addSchedule(schedDateTime4, "Web Development", "03-CRM-10", "L02", "S01", 'B');
    addSchedule(schedDateTime5, "Computer Ethics", "MS Teams", "L03", "S01", 'B');
    addSchedule(schedDateTime1, "", "MS Teams", "L03", "", 'A');
    addSchedule(schedDateTime4, "Data Structures", "03-CRM-08", "L04", "S02", 'B');
    // addSchedule(schedDateTime6, "FYP", "03-CRM-12", "L05", "S01", 'B');

    char user = 'X';

    do {
        cout << "\nWelcome to the UOWMKDU Appointment Booking System!" << endl;
        cout << "\nEnter S for Student and L for Lecturer: ";
        cin >> user;
        cin.ignore();
        user = toupper(user);

        if (user == 'L') {

            // Lecturer's Interface            
            Lecturer *currentLec = new Lecturer;

            // Verify if lecturer exists
            do {
                cout << "\nLecturer ID: ";
                string inID;
                getline(cin, inID, '\n');
                transform(inID.begin(), inID.end(), inID.begin(), ::toupper);

                // Initialize currentLec by searching linked list with inID user input
                currentLec = searchLec(inID);

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
                cout << "4. Edit Consultation Slot Information" << endl;
                cout << "5. Delete Consultation Slot" << endl;

                cout << "\nPlease select your desired action by entering integers 1, 2, 3, 4, or 5, based on the corresponding functions above." << endl;
                cout << "Action: ";
                cin >> action;

                char choice;
                if (action == 1) {
                    // 1. Set Schedule for Consultation
                    // Show lecturer's current schedule
                    viewSchedulesForLecturer(currentLec);

                    // Prompt to set schedule for consultation
                    char prompt = promptUser("\nDo you wish to set a new consultation slot? (Y/N): ");

                    if (prompt == 'Y') {
                        tm schedDateTime;
                        string subject, venue, lecID, studID;
                        char schedType;

                        // Get date and time
                        do {
                            cout << "\nDate and Time of Consultation (DD/MM/YYYY HH:MM): ";
                            string inDateTime;
                            cin.ignore();
                            getline(cin, inDateTime, '\n');

                            // Convert string to tm struct
                            istringstream ss(inDateTime);
                            ss >> get_time(&schedDateTime, "%d/%m/%Y %H:%M");

                            if (!validateDate(&schedDateTime)) {
                                cout << "\nInvalid date and time. Please try again." << endl;
                            }
                        } while (!validateDate(&schedDateTime));

                        // Get venue
                        cout << "Venue: ";
                        getline(cin, venue, '\n');

                        // Get lecturer ID
                        lecID = currentLec->lecID;

                        // Call set schedule function
                        addSchedule(schedDateTime, "", venue, lecID, "", 'A');

                        // View updated schedule
                        viewSchedulesForLecturer(currentLec);
                    }

                    action = continueSession(action);
                } else if (action == 2) {
                    //2. View Consultation Schedule" << endl;
                    viewSchedulesForLecturer(currentLec);
                    action = continueSession(action);
                } else if (action == 3) {
                    // 3. View Students with Booked Consultation
                    viewStudentsWithBookedConsultation(currentLec);
                    action = continueSession(action);
                } else if (action == 4) {
                    /// 4. Edit Consultation Slot Information

                    // Help user to find the schedule
                    viewSchedulesForLecturer(currentLec);

                    // Prompt to edit consultation schedule
                    char prompt = promptUser("\nDo you wish to edit a consultation slot? (Y/N): ");

                    if (prompt == 'Y') {
                        // Get date from user to search for schedule
                        tm schedDateTime;
                        do {
                            cout << "\nDate and Time of Consultation (DD/MM/YYYY HH:MM): ";
                            string inDateTime;
                            cin.ignore();
                            getline(cin, inDateTime, '\n');

                            // Convert string to tm struct
                            istringstream ss(inDateTime);
                            ss >> get_time(&schedDateTime, "%d/%m/%Y %H:%M");

                            // Validate date and time
                            if (!validateDate(&schedDateTime)) {
                                cout << "\nInvalid date and time. Please try again." << endl;
                            }
                        } while (!validateDate(&schedDateTime));

                        // Search for schedule
                        Schedule *currentSched = searchScheduleByDateTime(schedDateTime);

                        // If schedule is found
                        if (currentSched != NULL) {
                            // Get new details from user
                            string venue;

                            // Get venue
                            cout << "Venue: ";
                            getline(cin, venue, '\n');

                            // Update schedule
                            currentSched->venue = venue;

                            // View updated schedule
                            viewSchedulesForLecturer(currentLec);
                        } else {
                            cout << "\nSchedule not found. Please try again." << endl;
                        }

                    }

                    action = continueSession(action);
                } else if (action == 5) {
                    // 5. Delete Consultation Slot
                    // Help user to find the schedule
                    viewSchedulesForLecturer(currentLec);

                    // Prompt to delete consultation slot
                    char prompt = promptUser("\nDo you wish to delete a consultation slot? (Y/N): ");

                    if (prompt == 'Y') {
                        // Get date from user to search for schedule
                        tm schedDateTime;
                        do {
                            cout << "\nDate and Time of Consultation (DD/MM/YYYY HH:MM): ";
                            string inDateTime;
                            cin.ignore();
                            getline(cin, inDateTime, '\n');

                            // Convert string to tm struct
                            istringstream ss(inDateTime);
                            ss >> get_time(&schedDateTime, "%d/%m/%Y %H:%M");

                            // Validate date and time
                            if (!validateDate(&schedDateTime)) {
                                cout << "\nInvalid date and time. Please try again." << endl;
                            }

                            
                        } while (!validateDate(&schedDateTime));

                        deleteSchedule(schedDateTime);

                        viewSchedulesForLecturer(currentLec);
                    }

                    action = continueSession(action);
                } else {
                    cout << "Please only enter the integers 1, 2, 3, 4, or 5." << endl;
                    action = 0;
                }
            } while (action == 0);

        } else if (user == 'S') {

            // Student's Interface
            Student *currentStud = new Student;

            // Verify if student exists
            do {
                cout << "\nStudent ID: ";
                string inID;
                getline(cin, inID, '\n');
                transform(inID.begin(), inID.end(), inID.begin(), ::toupper);

                // Initialize currentStud by searching linked list with inID user input
                currentStud = searchStud(inID);

                if (currentStud == NULL) {
                    cout << "\nStudent not found. Please try again." << endl;
                }
            } while (currentStud == NULL);

            // Continue session based on user's input - action
            int action;
            do {
                cout << "\nWelcome, " << currentStud->studName << "!\n";

                cout << "\n1. View and Book Consultation Slots" << endl;
                cout << "2. View Booked Consultation Slots" << endl;
                cout << "3. View Booked Consultation History" << endl;
                cout << "4. Delete Booked Consultation Slots" << endl;

                cout << "\nPlease select your desired action by entering integers 1, 2, 3, or 4, based on the corresponding functions above." << endl;
                cout << "Action: ";
                cin >> action;

                char choice;
                if (action == 1) {
                    // 1. View and Book Consulation Slots
                    cout << "\nWhich lecturer's consultation slots would you like to book?" << endl;
                    cout << "Lecturer ID: ";
                    string searchID;
                    cin.ignore();
                    getline(cin, searchID, '\n');
                    transform(searchID.begin(), searchID.end(), searchID.begin(), ::toupper);

                    Lecturer *searchedLec = new Lecturer;
                    searchedLec = searchLec(searchID);

                    if (searchedLec != NULL) {
                        viewAvailableConsultation(searchedLec);

                        // Prompt to book a consultation slot
                        char prompt = promptUser("\nDo you wish to book a consultation slot? (Y/N): ");

                        if (prompt == 'Y') {
                            // Get date from user to search for schedule
                            tm schedDateTime;
                            do {
                                cout << "\nDate and Time of Consultation (DD/MM/YYYY HH:MM): ";
                                string inDateTime;
                                cin.ignore();
                                getline(cin, inDateTime, '\n');

                                // Convert string to tm struct
                                istringstream ss(inDateTime);
                                ss >> get_time(&schedDateTime, "%d/%m/%Y %H:%M");

                                // Validate date and time
                                if (!validateDate(&schedDateTime)) {
                                    cout << "\nInvalid date and time. Please try again." << endl;
                                }
                            } while (!validateDate(&schedDateTime));

                            // Search for schedule
                            Schedule *currentSched = searchScheduleByDateTime(schedDateTime);

                            // If schedule is found
                            if (currentSched != NULL) {
                                // Get new details from user
                                string inSubject;

                                // Get subject
                                cout << "Subject: ";
                                getline(cin, inSubject, '\n');

                                // Update schedule
                                currentSched->subject = inSubject;
                                currentSched->studID = currentStud->studID;
                                currentSched->schedType = 'B';

                                cout << "Consultation slot is successfully booked!" << endl;
                            } else {
                                cout << "\nConsultation slot not found. Please try again." << endl;
                            }
                        }
                    } else {
                        cout << "Lecturer not found. Please try again." << endl;
                    }

                    action = continueSession(action);
                } else if (action == 2) {
                    // 2. View Booked Consultation Slots
                    viewSchedulesForStudent(currentStud);
                    action = continueSession(action);
                } else if (action == 3) {
                    // 3. View Booked Consultation History
                    viewBookingHistory(currentStud);
                    action = continueSession(action);
                } else if (action == 4) {
                    // 4. Delete Booked Consultation Slot
                    viewSchedulesForStudent(currentStud);

                    // Prompt to delete a consultation slot
                    char prompt = promptUser("\nDo you wish to delete a consultation slot? (Y/N): ");

                    if (prompt == 'Y') {
                        // Get date from user to search for schedule
                        tm schedDateTime;
                        do {
                            cout << "\nDate and Time of Consultation (DD/MM/YYYY HH:MM): ";
                            string inDateTime;
                            cin.ignore();
                            getline(cin, inDateTime, '\n');

                            // Convert string to tm struct
                            istringstream ss(inDateTime);
                            ss >> get_time(&schedDateTime, "%d/%m/%Y %H:%M");

                            // Validate date and time
                            if (!validateDate(&schedDateTime)) {
                                cout << "\nInvalid date and time. Please try again." << endl;
                            }
                        } while (!validateDate(&schedDateTime));

                        // Search for schedule
                        Schedule *currentSched = searchScheduleByDateTime(schedDateTime);

                        // If schedule is found
                        if (currentSched != NULL) {
                            // Update schedule
                            currentSched->subject = "";
                            currentSched->studID = "";
                            currentSched->schedType = 'A';

                            cout << "Consultation slot is successfully deleted!" << endl;

                            // View updated schedule
                            viewSchedulesForStudent(currentStud);
                        }
                    }

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

char promptUser(string msg) {
    char prompt;
    do {
        cout << msg;
        cin >> prompt;
        prompt = toupper(prompt);

        if ((prompt != 'Y') && (prompt != 'N')) {
            cout << "Please enter Y for Yes, and N for No." << endl;
        }

    } while ((prompt != 'Y') && (prompt != 'N'));

    return prompt;
}

Lecturer *searchLec(string inputID) {
    Lecturer *temp = frontLec;

    while (temp != NULL) {
        if (temp->lecID == inputID) {
            return temp;
        }
        temp = temp->nxtLec;
    }

    return NULL;
}

Student *searchStud(string inputID) {
    Student *temp = frontStud;

    while (temp != NULL) {
        if (temp->studID == inputID) {
            return temp;
        }
        temp = temp->nxtStud;
    }

    return NULL;
}

bool validateDate(tm *timestruct) {
    time_t now = time(0);
    tm *currentDate = localtime(&now);
    if (timestruct->tm_year < currentDate->tm_year) {
        return false;
    } else if (timestruct->tm_year == currentDate->tm_year) {
        if (timestruct->tm_mon < currentDate->tm_mon) {
            return false;
        } else if (timestruct->tm_mon == currentDate->tm_mon) {
            if (timestruct->tm_mday < currentDate->tm_mday) {
                return false;
            }
        }
    }
    return true;
}

bool isSameDate(tm date1, tm date2) {
    return (date1.tm_year == date2.tm_year) && (date1.tm_mon == date2.tm_mon) && (date1.tm_mday == date2.tm_mday);
}

// Returns true if the given date is within 7 days from the current date, false otherwise.
bool isDateWithin7DaysFromNow(std::tm inputDate) {
    // Convert the input date to a time_t value.
    std::time_t inputTime = std::mktime(&inputDate);

    // Get the current time as a system_clock time_point, and convert it to a time_t value.
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

    // Calculate the time 7 days from now by adding 7 days' worth of seconds to the current time.
    std::time_t weekFromNowTime = nowTime + 7 * 24 * 60 * 60;

    // Return true if the input time is within 7 days from now (between now and 7 days from now),
    // false otherwise.
    return (inputTime >= nowTime && inputTime <= weekFromNowTime);
}

void addSchedule(tm schedDateTime, string subject, string venue, string lecID, string studID, char schedType) {
    // Validation for adding only for the current week
    // Check if the consultation is within the current week
    time_t now = time(0);
    tm *currentDate = localtime(&now);
    
    if (!isDateWithin7DaysFromNow(schedDateTime)) {
        cout << "\nConsultation slot is not within the current week. Please try again." << endl;
        return;
    }

    // Check if a schedule with the same datetime and lecturer ID already exists
    Schedule *temp = frontSched;
    while (temp != NULL) {
        if (temp->lecID == lecID && temp->schedDateTime.tm_year == schedDateTime.tm_year &&
            temp->schedDateTime.tm_mon == schedDateTime.tm_mon &&
            temp->schedDateTime.tm_mday == schedDateTime.tm_mday &&
            temp->schedDateTime.tm_hour == schedDateTime.tm_hour &&
            temp->schedDateTime.tm_min == schedDateTime.tm_min) {
            cout << "\nConsultation slot with existing time and date already exists. Please try again." << endl;
            return;
        }
        temp = temp->nxtSched;
    }

    // Create a new schedule and add it to the linked list
    Schedule *newSched = new Schedule;

    newSched->schedDateTime = schedDateTime;
    newSched->subject = subject;
    newSched->venue = venue;
    newSched->lecID = lecID;
    newSched->studID = studID;
    newSched->schedType = schedType;
    newSched->nxtSched = NULL;

    if (frontSched == NULL) {
        // If schedule LL is empty
        frontSched = newSched;
    } else {
        temp = frontSched;
        while (temp->nxtSched != NULL) {
            temp = temp->nxtSched;
        }
        temp->nxtSched = newSched;
    }
}

void viewSchedulesForLecturer(Lecturer *lecturer) {
    if (lecturer == NULL) {
        cout << "Invalid lecturer." << endl;
        return;
    }

    Schedule *temp = frontSched;
    time_t now = time(0);
    tm *currentDate = localtime(&now);

    cout << "\nConsultations of the Week for " << lecturer->lecID << " - " << lecturer->lecName << ":" << endl;

    while (temp != NULL) {
        if ((temp->lecID == lecturer->lecID)) {
            if (isDateWithin7DaysFromNow(temp->schedDateTime)) {
                cout << "----------------------------------------------" << endl;
                tm dateTime = temp->schedDateTime;
                auto dateStr = to_string(dateTime.tm_mday) + "/" + to_string(dateTime.tm_mon + 1) + "/" +
                               to_string(dateTime.tm_year + 1900);
                auto timeStr = to_string(dateTime.tm_hour) + ":" + to_string(dateTime.tm_min);

                Student *student = searchStud(temp->studID);
                if (student != NULL) {
                    cout << "Date: " << dateStr << endl;
                    cout << "Time: " << timeStr << endl;
                    cout << "Subject: " << temp->subject << endl;
                    cout << "Venue: " << temp->venue << endl;
                    cout << "Lecturer: " << lecturer->lecID << " - " << lecturer->lecName << endl;
                    cout << "Student: " << student->studID << " - " << student->studName << endl;
                    cout << "Consultation: " << "Booked" << endl;
                } else {
                    // if the student does not exist, display -- instead
                    cout << "Date: " << dateStr << endl;
                    cout << "Time: " << timeStr << endl;
                    cout << "Subject: " << "--" << endl;
                    cout << "Venue: " << temp->venue << endl;
                    cout << "Lecturer: " << lecturer->lecID << " - " << lecturer->lecName << endl;
                    cout << "Student: " << "--" << endl;
                    cout << "Consultation: " << "Available" << endl;
                }
                cout << "----------------------------------------------" << endl;
            }
        }
        temp = temp->nxtSched;
    }
}

void viewAvailableConsultation(Lecturer *lecturer) {
    Schedule *temp = frontSched;
    time_t now = time(0);
    tm *currentDate = localtime(&now);

    cout << "\nConsultations of the Week for " << lecturer->lecID << " - " << lecturer->lecName << ":" << endl;

    while (temp != NULL) {
        if ((temp->lecID == lecturer->lecID) && (temp->schedType== 'A') && (isDateWithin7DaysFromNow(temp->schedDateTime))) {
            cout << "----------------------------------------------" << endl;
            tm dateTime = temp->schedDateTime;
            auto dateStr = to_string(dateTime.tm_mday) + "/" + to_string(dateTime.tm_mon + 1) + "/" +
                            to_string(dateTime.tm_year + 1900);
            auto timeStr = to_string(dateTime.tm_hour) + ":" + to_string(dateTime.tm_min);

            cout << "Date: " << dateStr << endl;
            cout << "Time: " << timeStr << endl;
            cout << "Subject: " << "--" << endl;
            cout << "Venue: " << temp->venue << endl;
            cout << "Lecturer: " << lecturer->lecID << " - " << lecturer->lecName << endl;
            cout << "Student: " << "--" << endl;
            cout << "Consultation: " << "Available" << endl;

            cout << "----------------------------------------------" << endl;
        }
           temp = temp->nxtSched;     
    }
}

void viewSchedulesForStudent(Student *student) {
    Schedule *temp = frontSched;
    time_t now = time(0);
    tm *currentDate = localtime(&now);

    cout << "\nConsultations of the Week for " << student->studID << " - " << student->studName << ":" << endl;

    while (temp != NULL) {
        if ((temp->studID == student->studID)) {
            if (isDateWithin7DaysFromNow(temp->schedDateTime)) {
                cout << "----------------------------------------------" << endl;
                tm dateTime = temp->schedDateTime;
                auto dateStr = to_string(dateTime.tm_mday) + "/" + to_string(dateTime.tm_mon + 1) + "/" +
                               to_string(dateTime.tm_year + 1900);
                auto timeStr = to_string(dateTime.tm_hour) + ":" + to_string(dateTime.tm_min);

                Lecturer *lecturer = searchLec(temp->lecID);

                cout << "Date: " << dateStr << endl;
                cout << "Time: " << timeStr << endl;
                cout << "Subject: " << temp->subject << endl;
                cout << "Venue: " << temp->venue << endl;
                cout << "Lecturer: " << lecturer->lecID << " - " << lecturer->lecName << endl;
                cout << "Student: " << student->studID << " - " << student->studName << endl;
                cout << "Consultation: " << "Booked" << endl;
                cout << "----------------------------------------------" << endl;
            }
        }
        temp = temp->nxtSched;
    }
}

void viewBookingHistory(Student * student) {
    Schedule *temp = frontSched;
    time_t now = time(0);
    tm *currentDate = localtime(&now);

    cout << "\nAll Booked Consultations of " << student->studID << " - " << student->studName << ":" << endl;

    while (temp != NULL) {
        if ((temp->studID == student->studID)) {
            cout << "----------------------------------------------" << endl;
            tm dateTime = temp->schedDateTime;
            auto dateStr = to_string(dateTime.tm_mday) + "/" + to_string(dateTime.tm_mon + 1) + "/" +
                            to_string(dateTime.tm_year + 1900);
            auto timeStr = to_string(dateTime.tm_hour) + ":" + to_string(dateTime.tm_min);

            Lecturer *lecturer = searchLec(temp->lecID);

            cout << "Date: " << dateStr << endl;
            cout << "Time: " << timeStr << endl;
            cout << "Subject: " << temp->subject << endl;
            cout << "Venue: " << temp->venue << endl;
            cout << "Lecturer: " << lecturer->lecID << " - " << lecturer->lecName << endl;
            cout << "Student: " << student->studID << " - " << student->studName << endl;
            cout << "Consultation: " << "Booked" << endl;
            cout << "----------------------------------------------" << endl;
        }
        temp = temp->nxtSched;
    }
}

void viewStudentsWithBookedConsultation(Lecturer *lecturer) {
    // Get all booked consultation slots for lecturer
    Schedule *currentSched = frontSched;

    while (currentSched != NULL) {
        // only show current week's consultations
        time_t now = time(0);
        tm *currentDate = localtime(&now);
       
        if (!isDateWithin7DaysFromNow(currentSched->schedDateTime)) {
            currentSched = currentSched->nxtSched;
            continue;
        }
        if (currentSched->lecID == lecturer->lecID && currentSched->schedType == 'B') {

            // Get student details
            Student *currentStud = searchStud(currentSched->studID);
            // if student does not exist
            if (currentStud == NULL) {
                currentSched = currentSched->nxtSched;
                continue;
            }
            cout << "\n----------------------------------------------" << endl;
            // Display student details
            cout << "\nStudent Details" << endl;
            cout << "ID: " << currentStud->studID << endl;
            cout << "Name: " << currentStud->studName << endl;
            cout << "Email: " << currentStud->studEmail << endl;
            cout << "Phone: " << currentStud->studPhone << endl;
            cout << "Course: " << currentStud->studCourse << endl;

            // Get all booked consultation slots for student
            // Display consultation details
            cout << "\nConsultation Details" << endl;
            cout << "Date: " << currentSched->schedDateTime.tm_mday << "/"
                 << currentSched->schedDateTime.tm_mon + 1 << "/" << currentSched->schedDateTime.tm_year + 1900 << endl;
            cout << "Time: " << currentSched->schedDateTime.tm_hour << ":"
                 << currentSched->schedDateTime.tm_min << endl;
            cout << "Venue: " << currentSched->venue << endl;
            cout << "Subject: " << currentSched->subject << endl;

            cout << "\n----------------------------------------------" << endl;

        }


        currentSched = currentSched->nxtSched;
    }

}

Schedule *searchScheduleByDateTime(tm schedDateTime) {
    Schedule *temp = frontSched;
    while (temp != NULL) {
        if (isSameDate(temp->schedDateTime, schedDateTime) && temp->schedDateTime.tm_hour == schedDateTime.tm_hour &&
            temp->schedDateTime.tm_min == schedDateTime.tm_min) {
            return temp;
        }
        temp = temp->nxtSched;
    }
    return NULL;
}

void deleteSchedule(tm schedDateTime) {
    Schedule *temp = frontSched;
    Schedule *prev = NULL;
    while (temp != NULL) {
        if (isSameDate(temp->schedDateTime, schedDateTime) && temp->schedDateTime.tm_hour == schedDateTime.tm_hour &&
            temp->schedDateTime.tm_min == schedDateTime.tm_min) {
            if (prev == NULL) {
                frontSched = temp->nxtSched;
            } else {
                prev->nxtSched = temp->nxtSched;
            }
            delete temp;
            cout << "Consultation slot successfully deleted!" << endl;

            return;
        }
        prev = temp;
        temp = temp->nxtSched;
    }
}