#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class Patient {
private:
    int patientID;
    string name;
    int age;
    string disease;
    string appointmentDate;

public:
    Patient(int id, string pname, int page, string pdisease, string pdate) {
        patientID = id;
        name = pname;
        age = page;
        disease = pdisease;
        appointmentDate = pdate;
    }

    int getPatientID() const { return patientID; }
    string getName() const { return name; }
    int getAge() const { return age; }
    string getDisease() const { return disease; }
    string getAppointmentDate() const { return appointmentDate; }

    void updateMedicalRecord(string newDisease) {
        disease = newDisease;
    }

    void updateAppointment(string newDate) {
        appointmentDate = newDate;
    }

    void displayPatient() const {
        cout << "ID: " << patientID << " | Name: " << name << " | Age: " << age
             << " | Disease: " << disease << " | Appointment: " << appointmentDate << endl;
    }

    void saveToFile(ofstream &outfile) const {
        outfile << patientID << "," << name << "," << age << "," << disease << "," << appointmentDate << endl;
    }

    static Patient loadFromFile(ifstream &infile) {
        int id, age;
        string name, disease, date;
        infile >> id;
        infile.ignore();
        getline(infile, name, ',');
        infile >> age;
        infile.ignore();
        getline(infile, disease, ',');
        getline(infile, date);
        return Patient(id, name, age, disease, date);
    }
};

// Function to check if patient ID is unique
bool isPatientIDUnique(vector<Patient> &patients, int id) {
    for (const auto &p : patients) {
        if (p.getPatientID() == id) {
            return false;
        }
    }
    return true;
}

// Function to find a patient by ID
Patient* findPatient(vector<Patient> &patients, int id) {
    for (auto &p : patients) {
        if (p.getPatientID() == id) {
            return &p;
        }
    }
    return nullptr;
}

// Save patients to file
void savePatientsToFile(vector<Patient> &patients) {
    ofstream outfile("patients.txt", ios::trunc);
    for (const auto &p : patients) {
        p.saveToFile(outfile);
    }
    outfile.close();
}

// Load patients from file
void loadPatientsFromFile(vector<Patient> &patients) {
    ifstream infile("patients.txt");
    if (!infile) return;

    while (!infile.eof()) {
        int id, age;
        string name, disease, date;

        infile >> id;
        infile.ignore();
        if (getline(infile, name, ',') && infile >> age && infile.ignore() && getline(infile, disease, ',') && getline(infile, date)) {
            patients.push_back(Patient(id, name, age, disease, date));
        }
    }
    infile.close();
}

// Function to delete a patient record
void deletePatient(vector<Patient> &patients, int id) {
    for (auto it = patients.begin(); it != patients.end(); ++it) {
        if (it->getPatientID() == id) {
            patients.erase(it);
            cout << "Patient record deleted successfully!\n";
            savePatientsToFile(patients);
            return;
        }
    }
    cout << "Patient not found!\n";
}

int main() {
    vector<Patient> patients;
    loadPatientsFromFile(patients);

    int choice;
    while (true) {
        cout << "\n===== AI-Powered Smart Healthcare System =====\n";
        cout << "1. Register a Patient\n";
        cout << "2. View All Patients\n";
        cout << "3. Search Patient by ID\n";
        cout << "4. Update Medical Record\n";
        cout << "5. Schedule/Update Appointment\n";
        cout << "6. Delete a Patient Record\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int id, age;
            string name, disease, appointment;
            cout << "Enter Patient ID: ";
            cin >> id;
            cin.ignore();

            if (!isPatientIDUnique(patients, id)) {
                cout << "Error! Patient ID already exists.\n";
                continue;
            }

            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter Age: ";
            cin >> age;
            cin.ignore();
            cout << "Enter Disease: ";
            getline(cin, disease);
            cout << "Enter Appointment Date (DD-MM-YYYY): ";
            getline(cin, appointment);

            patients.push_back(Patient(id, name, age, disease, appointment));
            savePatientsToFile(patients);
            cout << "Patient Registered Successfully!\n";

        } else if (choice == 2) {
            cout << "\n===== Registered Patients =====\n";
            for (const auto &p : patients) {
                p.displayPatient();
            }

        } else if (choice == 3) {
            int id;
            cout << "Enter Patient ID to search: ";
            cin >> id;
            Patient* patient = findPatient(patients, id);

            if (patient) {
                patient->displayPatient();
            } else {
                cout << "Patient not found!\n";
            }

        } else if (choice == 4) {
            int id;
            string newDisease;
            cout << "Enter Patient ID to update medical record: ";
            cin >> id;
            cin.ignore();
            Patient* patient = findPatient(patients, id);

            if (patient) {
                cout << "Enter New Disease/Medical Condition: ";
                getline(cin, newDisease);
                patient->updateMedicalRecord(newDisease);
                savePatientsToFile(patients);
                cout << "Medical record updated successfully!\n";
            } else {
                cout << "Patient not found!\n";
            }

        } else if (choice == 5) {
            int id;
            string newDate;
            cout << "Enter Patient ID to update appointment: ";
            cin >> id;
            cin.ignore();
            Patient* patient = findPatient(patients, id);

            if (patient) {
                cout << "Enter New Appointment Date (DD-MM-YYYY): ";
                getline(cin, newDate);
                patient->updateAppointment(newDate);
                savePatientsToFile(patients);
                cout << "Appointment updated successfully!\n";
            } else {
                cout << "Patient not found!\n";
            }

        } else if (choice == 6) {
            int id;
            cout << "Enter Patient ID to delete: ";
            cin >> id;
            deletePatient(patients, id);

        } else if (choice == 7) {
            cout << "Exiting Smart Healthcare System. Thank you!\n";
            break;
        } else {
            cout << "Invalid choice! Try again.\n";
        }
    }
    return 0;
}
