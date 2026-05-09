#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdio>

using namespace std;
// ---------- EXCEPTION CLASSES ----------
class ComplaintException {
protected:
    string message;

public:
    ComplaintException(string msg) {
        message = msg;
    }

    string what() {
        return message;
    }
};

class FileException : public ComplaintException {
public:
    FileException(string msg)
        : ComplaintException(msg) {
    }
};

class LoginException : public ComplaintException {
public:
    LoginException(string msg)
        : ComplaintException(msg) {
    }
};

class ComplaintNotFoundException : public ComplaintException {
public:
    ComplaintNotFoundException(string msg)
        : ComplaintException(msg) {
    }
};

class InvalidPriorityException : public ComplaintException {
public:
    InvalidPriorityException(string msg)
        : ComplaintException(msg) {
    }
};

class InvalidRatingException : public ComplaintException {
public:
    InvalidRatingException(string msg)
        : ComplaintException(msg) {
    }
};

// ---------- USER ----------
class User {
protected:
    string username, password, email;

public:
    //DONE
    bool login() {
        cout << "Email: ";
        cin >> email;
        cout << "Password: "; 
        cin >> password;

        ifstream file("Users.txt");
        if (!file) {
            throw FileException("Users.txt could not be opened!");
        }

        string u, e, p;

        while (getline(file, u, '|') ){
            getline(file, e, '|');
            getline(file, p);
            if (email == e && password == p) {
                username = u;
                return true;
            }
        }

        return false;
    }
    //DONE
    void signup() {
        string newUsername, newEmail, newPassword;
        cout << "Username: ";
        cin.ignore();
        getline(cin, newUsername);
        cout << "Email: ";
        cin >> newEmail;
        cout << "Password: ";
        cin >> newPassword;

        // --- Check if Email already exists ---
        ifstream readFile("Users.txt");
        if (readFile) {
            string u, e, p;
            while (getline(readFile, u, '|')) {
                getline(readFile, e, '|');
                getline(readFile, p);

                if (e == newEmail) {
                    cout << "Error: An account with this email already exists!\n";
                    readFile.close();
                    return; 
                }
            }
            readFile.close();
        }

        // --- Proceed to save if email is unique ---
        ofstream file("Users.txt", ios::app);
        if (!file) {
            cout << "File error!\n";
            return;
        }

        username = newUsername;
        email = newEmail;
        password = newPassword;

        file << username << "|" << email << "|" << password << endl;
        cout << "Account created!\n";
    }
    //DONE
    string getUsername() { 
        return username;
    }
};

// ---------- COMPLAINT ----------
class Complaint : public User {
public:
    void submit() {
        string con, type, priority, desc, img, status = "Pending";
        string id = to_string(time(0));
        while (isIdExists(id)) {

            id = to_string(stoi(id) + 1);
        }

        cout << "Contact: "; 
        cin >> con;
        cout << "Type (Service/Technical/Billing): ";
        cin >> type;
        cout << "Priority (High/Medium/Low): "; 
        cin >> priority;

        if (priority != "High" &&
            priority != "Medium" &&
            priority != "Low") {

            throw InvalidPriorityException(
                "Priority must be High, Medium, or Low!"
            );
        }

        cin.ignore();
        cout << "Description: ";
        getline(cin, desc);

        cout << "Image path: ";
        getline(cin, img);

        ofstream file("Complaints.txt", ios::app);
        if (!file) {
            throw FileException(
                "Complaints.txt could not be opened!"
            );
        }

        file << id << "|" << getUsername() << "|" << con << "|"
            << type << "|" << priority << "|" << desc << "|"
            << img << "|" << status << endl;

        cout << "Complaint submitted! ID: " << id << endl;
    }

    bool isIdExists(string searchId) {
        ifstream file("Complaints.txt");
        if (!file)
            return false; 

        string id, line;
        while (getline(file, id, '|')) {
            getline(file, line);

            if (id == searchId) {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    void track() {
        string searchId;
        cout << "Enter ID: ";
        cin >> searchId;

        ifstream file("Complaints.txt");
        if (!file) {
            cout << "File error!\n";
            return;
        }

        string id, name, con, type, priority, desc, img, status;
        bool found = false;

        while (getline(file, id, '|')){
            getline(file, name, '|');
            getline(file, con, '|');
            getline(file, type, '|');
            getline(file, priority, '|');
            getline(file, desc, '|');
            getline(file, img, '|');
            getline(file, status);

            if (id == searchId) {
                cout << "\n--- Found ---\n";
                cout << "User: " << name << endl;
                cout << "Type: " << type << endl;
                cout << "Priority: " << priority << endl;
                cout << "Status: " << status << endl;
                found = true;
                break;
            }
        }

        if (!found) {
            throw ComplaintNotFoundException(
                "Complaint ID not found!"
            );
        }
      
    }

    void feedback() {
        string id, rating, comment;

        cout << "Complaint ID: "; 
        cin >> id;
        cout << "Rating (1-5): "; 
        cin >> rating;

        int r = stoi(rating);

        if (r < 1 || r > 5) {
            throw InvalidRatingException(
                "Rating must be between 1 and 5!"
            );
        }

        cin.ignore();
        cout << "Comment: ";
        getline(cin, comment);

        ofstream file("Feedback.txt", ios::app);
        if (!file) {
            cout << "Error!\n";
            return;
        }

        file << id << "|" << rating << "|" << comment << endl;
        cout << "Feedback saved!\n";
    }

};

// ---------- ADMIN ----------
class Admin {
    string user = "admin", pass = "admin123";

public:
    bool login() {
        string u, p;
        cout << "Admin Username: "; cin >> u;
        cout << "Password: "; cin >> p;
        return (u == user && p == pass);
    }

    void viewAll() {
        ifstream file("Complaints.txt");
        if (!file) {
            cout << "No data!\n";
            return;
        }

        string id, name, con, type, priority, desc, img, status;

        while (getline(file, id, '|') &&
            getline(file, name, '|') &&
            getline(file, con, '|') &&
            getline(file, type, '|') &&
            getline(file, priority, '|') &&
            getline(file, desc, '|') &&
            getline(file, img, '|') &&
            getline(file, status)) {

            cout << "\nID: " << id
                << "\nUser: " << name
                << "\nType: " << type
                << "\nPriority: " << priority
                << "\nStatus: " << status
                << "\n----------------\n";
        }
    }

    void updateStatus() {
        string searchId;
        cout << "Enter ID: ";
        cin >> searchId;

        ifstream in("Complaints.txt");
        ofstream temp("temp.txt");

        if (!in || !temp) {
            cout << "File error!\n";
            return;
        }

        string id, name, con, type, priority, desc, img, status;
        bool found = false;

        while (getline(in, id, '|') &&
            getline(in, name, '|') &&
            getline(in, con, '|') &&
            getline(in, type, '|') &&
            getline(in, priority, '|') &&
            getline(in, desc, '|') &&
            getline(in, img, '|') &&
            getline(in, status)) {

            if (id == searchId) {
                cout << "New status: ";
                cin.ignore();
                getline(cin, status);
                found = true;
            }

            temp << id << "|" << name << "|" << con << "|"
                << type << "|" << priority << "|" << desc << "|"
                << img << "|" << status << endl;
        }

        in.close();
        temp.close();

        remove("Complaints.txt");
        rename("temp.txt", "Complaints.txt");

        if (found) cout << "Updated!\n";
        else cout << "ID not found!\n";
    }

    void searchByUser() {
        string search;
        cout << "Enter username: ";
        cin >> search;

        ifstream file("Complaints.txt");

        string id, name, con, type, priority, desc, img, status;
        bool found = false;

        while (getline(file, id, '|') &&
            getline(file, name, '|') &&
            getline(file, con, '|') &&
            getline(file, type, '|') &&
            getline(file, priority, '|') &&
            getline(file, desc, '|') &&
            getline(file, img, '|') &&
            getline(file, status)) {

            if (name == search) {
                cout << "\nID: " << id
                    << "\nType: " << type
                    << "\nStatus: " << status
                    << "\n----------------\n";
                found = true;
            }
        }

        if (!found) cout << "No records!\n";
    }

    void filterStatus() {
        string f;
        cout << "Enter Status: ";
        cin >> f;

        ifstream file("Complaints.txt");

        string id, name, con, type, priority, desc, img, status;

        while (getline(file, id, '|') &&
            getline(file, name, '|') &&
            getline(file, con, '|') &&
            getline(file, type, '|') &&
            getline(file, priority, '|') &&
            getline(file, desc, '|') &&
            getline(file, img, '|') &&
            getline(file, status)) {

            if (status == f) {
                cout << id << " | " << name << " | " << status << endl;
            }
        }
    }

    void filterPriority() {
        string f;
        cout << "Enter Priority: ";
        cin >> f;

        ifstream file("Complaints.txt");

        string id, name, con, type, priority, desc, img, status;

        while (getline(file, id, '|') &&
            getline(file, name, '|') &&
            getline(file, con, '|') &&
            getline(file, type, '|') &&
            getline(file, priority, '|') &&
            getline(file, desc, '|') &&
            getline(file, img, '|') &&
            getline(file, status)) {

            if (priority == f) {
                cout << id << " | " << name << " | " << priority << endl;
            }
        }
    }

    void stats() {
        ifstream file("Complaints.txt");

        string id, name, con, type, priority, desc, img, status;

        int total = 0;
        int pending = 0, resolved = 0, inprogress = 0;
        int service = 0, technical = 0, billing = 0;
        int high = 0, medium = 0, low = 0;

        while (getline(file, id, '|') &&
            getline(file, name, '|') &&
            getline(file, con, '|') &&
            getline(file, type, '|') &&
            getline(file, priority, '|') &&
            getline(file, desc, '|') &&
            getline(file, img, '|') &&
            getline(file, status)) {

            total++;

            if (status == "Pending") pending++;
            else if (status == "Resolved") resolved++;
            else if (status == "InProgress") inprogress++;

            if (type == "Service") service++;
            else if (type == "Technical") technical++;
            else if (type == "Billing") billing++;

            if (priority == "High") high++;
            else if (priority == "Medium") medium++;
            else if (priority == "Low") low++;
        }

        cout << "\nTotal: " << total;
        cout << "\nPending: " << pending;
        cout << "\nResolved: " << resolved;
        cout << "\nInProgress: " << inprogress;

        cout << "\nService: " << service;
        cout << "\nTechnical: " << technical;
        cout << "\nBilling: " << billing;

        cout << "\nHigh: " << high;
        cout << "\nMedium: " << medium;
        cout << "\nLow: " << low << endl;
    }
};

// ---------- PANELS ----------
void userPanel(Complaint& c) {
    int ch;
    while (true) {
        cout << "\n1.Submit\n2.Track\n3.Feedback\n4.Logout\nChoice: ";
        cin >> ch;
        if (!(cin >> ch)) {
            cin.clear();
            cin.ignore();
            cout << "INvalid " << endl;
            continue;
        }

        if (ch == 1) 
            try {
                c.submit();
            }
        catch (ComplaintException e) {
            cout << e.what() << endl;
        }
        else if (ch == 2) 
            
            try {
            c.track();
        }
        catch (ComplaintException e) {
            cout << e.what() << endl;
        }
          
        else if (ch == 3) 
            c.feedback();
        else break;
    }
}

void adminPanel() {
    Admin a;
    if (!a.login()) 
        return;
    int ch;
    while (true) {
        cout << "\n1.View\n2.Update\n3.Search User\n4.Filter Status\n5.Filter Priority\n6.Stats\n7.Logout\nChoice: ";
        cin >> ch;
        if (!(cin >> ch)) {
            cin.clear();
            cin.ignore();
            cout << "Enter a valid number" << endl;
            continue;

        }
        if (ch == 1) 
            a.viewAll();
        else if (ch == 2) 
            a.updateStatus();
        else if (ch == 3) 
            a.searchByUser();
        else if (ch == 4) 
            a.filterStatus();
        else if (ch == 5) 
            a.filterPriority();
        else if (ch == 6) 
            a.stats();
        else break;
    }
}

// ---------- MAIN ----------
int main() {
    Complaint c;
    int ch;

    while (true) {
        cout << "\n1.Signup\n2.Login\n3.Admin\n4.Exit\nChoice: ";
        cin >> ch;

        if (ch == 1) {
            c.signup();
            userPanel(c);
        }
        else if (ch == 2) {
            try {
                if (c.login()) {
                    userPanel(c);
                }
                else {
                    throw LoginException("Invalid Email or Password!");
                }
            }
            catch (ComplaintException e) {
                cout << e.what() << endl;
            }
        }
        else if (ch == 3) {
            adminPanel();
        }
        else break;
    }

    return 0;
}