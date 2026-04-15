#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <string>
using namespace std;

// ---------------- Berth Type Enum ----------------
enum BerthType { LB, MB, UB, SL, SU, GN }; // GN for General Seating
string berthToString(BerthType b) {
    switch (b) {
        case LB: return "Lower Berth";
        case MB: return "Middle Berth";
        case UB: return "Upper Berth";
        case SL: return "Side Lower";
        case SU: return "Side Upper";
        case GN: return "General Seat";
        default: return "Unknown";
    }
}
// ---------------- Berth Struct ----------------
struct Berth {
    int seatNumber;
    BerthType type;
    bool isBooked;
};
// ---------------- Coach Class ----------------
class Coach {
private:
    string coachID;
    string coachType;
    vector<Berth> seats;
public:
    Coach(string id, string type) : coachID(id), coachType(type) {
        int totalSeats = 0;
        if (coachType == "General") {
            totalSeats = 90; // General coaches have general seating only
            for (int i = 1; i <= totalSeats; ++i) {
                seats.push_back({i, GN, false});
            }
        } else {
            totalSeats = 67; // Sleeper/AC coaches
            for (int i = 1; i <= totalSeats; ++i) {
                BerthType bt = assignBerthType(i);
                seats.push_back({i, bt, false});
            }
        }
    }
    BerthType assignBerthType(int seatNo) {
        int mod = seatNo % 8;
        switch (mod) {
            case 1: case 4: return LB;
            case 2: case 5: return MB;
            case 3: case 6: return UB;
            case 7: return SL;
            case 0: return SU;
        }
        return LB;
    }

    string getCoachType() const { return coachType; }
    string getCoachID() const { return coachID; }

    bool bookSeat(int& seatNo, BerthType& type, int passengerAge) {
        if (coachType == "General") {
            for (auto& seat : seats) {
                if (!seat.isBooked) {
                    seat.isBooked = true;
                    seatNo = seat.seatNumber;
                    type = seat.type;
                    return true;
                }
            }
            return false;
        } else {
            vector<BerthType> preferredBerths;
            if (passengerAge <= 19) {
                preferredBerths = { UB, SU };
            } else if (passengerAge > 40) {
                preferredBerths = { LB, SL };
            } else {
                preferredBerths = { MB, LB, SL, UB, SU };
            }
            for (BerthType pref : preferredBerths) {
                for (auto& seat : seats) {
                    if (!seat.isBooked && seat.type == pref) {
                        seat.isBooked = true;
                        seatNo = seat.seatNumber;
                        type = seat.type;
                        return true;
                    }
                }
            }
            for (auto& seat : seats) {
                if (!seat.isBooked) {
                    seat.isBooked = true;
                    seatNo = seat.seatNumber;
                    type = seat.type;
                    return true;
                }
            }
            return false;
        }
    }

    void cancelSeat(int seatNo) {
        for (auto& seat : seats) {
            if (seat.seatNumber == seatNo) {
                seat.isBooked = false;
                break;
            }
        }
    }
};
// ---------------- Train Class ----------------
class Train {
private:
    int trainNumber;
    string trainName;
    string source;
    string destination;
    string departureTime;
    vector<Coach> coaches;
public:
    Train(int num, string name, string src, string dest, string time) :
        trainNumber(num), trainName(name), source(src), destination(dest), departureTime(time) {
        for (int i = 1; i <= 10; ++i)
            coaches.push_back(Coach("S" + to_string(i), "Sleeper"));
        for (int i = 1; i <= 3; ++i)
            coaches.push_back(Coach("B" + to_string(i), "AC 3-Tier"));
        coaches.push_back(Coach("A1", "AC 2-Tier"));
        coaches.push_back(Coach("H1", "AC 1-Tier"));
        coaches.push_back(Coach("G1", "General"));
        coaches.push_back(Coach("G2", "General"));
    }
    int getTrainNumber() const { return trainNumber; }
    string getTrainName() const { return trainName; }
    string getRoute() const { return source + " → " + destination; }
    string getTime() const { return departureTime; }

    bool bookSeat(const string& coachType, string& coachID, int& seatNo, BerthType& berthType, int passengerAge) {
        for (auto& coach : coaches) {
            if (coach.getCoachType() == coachType) {
                if (coach.bookSeat(seatNo, berthType, passengerAge)) {
                    coachID = coach.getCoachID();
                    return true;
                }
            }
        }
        return false;
    }

    void cancelSeat(string coachID, int seatNo) {
        for (auto& coach : coaches) {
            if (coach.getCoachID() == coachID) {
                coach.cancelSeat(seatNo);
                break;
            }
        }
    }

    void displayDetails() const {
        cout << left << setw(10) << trainNumber
             << setw(20) << trainName
             << setw(20) << source
             << setw(20) << destination
             << setw(10) << departureTime << endl;
    }

    void displayCoachTypes() const {
        cout << "Available Coach Types:\n";
        cout << "1. Sleeper\n2. AC 3-Tier\n3. AC 2-Tier\n4. AC 1-Tier\n5. General\n";
    }
};
// ---------------- Passenger Class ----------------
class Passenger {
private:
    string name;
    int age;
    string gender;
    string journeyDate;
    double ticketPrice;
    int trainNumber;
    string coachID;
    string coachType;
    int seatNumber;
    BerthType berthType;
public:
    Passenger(string n, int a, string g, string date, double price,
              int tNum, string cID, string ctype, int sNum, BerthType bType)
        : name(n), age(a), gender(g), journeyDate(date), ticketPrice(price),
          trainNumber(tNum), coachID(cID), coachType(ctype), seatNumber(sNum), berthType(bType) {}
    void printTicket(const Train& train) const {
        cout << "\n=====================================================\n";
        cout << "                  INDIAN RAILWAYS                    \n";
        cout << "-----------------------------------------------------\n";
        cout << "Passenger Name : " << name << " (" << gender << ")\n";
        cout << "Age            : " << age << "\n";
        cout << "Train No.      : " << train.getTrainNumber() << "\n";
        cout << "Train Name     : " << train.getTrainName() << "\n";
        cout << "Route          : " << train.getRoute() << "\n";
        cout << "Departure Time : " << train.getTime() << "\n";
        cout << "Journey Date   : " << journeyDate << "\n";
        cout << "Coach          : " << coachID << " (" << coachType << ", " << berthToString(berthType) << ")\n";
        cout << "Seat Number    : " << seatNumber << "\n";
        cout << "Ticket Price   : ₹" << fixed << setprecision(2) << ticketPrice << "\n";
        cout << "-----------------------------------------------------\n";
        cout << "         Thank you for choosing Indian Railways      \n";
        cout << "=====================================================\n";
    }
    int getTrainNumber() const { return trainNumber; }
    string getCoachID() const { return coachID; }
    string getCoachType() const { return coachType; }
    int getSeatNumber() const { return seatNumber; }
    double getTicketPrice() const { return ticketPrice; }
};
// ---------------- ReservationSystem Class ----------------
class ReservationSystem {
private:
    vector<Train> trains;
    vector<Passenger> passengers;

    double getTicketPrice(const string& coachType) {
        if (coachType == "General") return 150.00;
        else if (coachType == "Sleeper") return 450.00;
        else if (coachType == "AC 3-Tier") return 1200.00;
        else if (coachType == "AC 2-Tier") return 1800.00;
        else if (coachType == "AC 1-Tier") return 2500.00;
        return 0.0;
    }

    double getCancellationFine(const string& coachType) {
        if (coachType == "General" || coachType == "Sleeper") return 50.00;
        else if (coachType == "AC 3-Tier") return 150.00;
        else if (coachType == "AC 2-Tier") return 250.00;
        else if (coachType == "AC 1-Tier") return 400.00;
        return 50.00;
    }

    string getCoachTypeByChoice(int choice) {
        switch (choice) {
            case 1: return "Sleeper";
            case 2: return "AC 3-Tier";
            case 3: return "AC 2-Tier";
            case 4: return "AC 1-Tier";
            case 5: return "General";
            default: return "";
        }
    }
public:
    ReservationSystem() {
        trains.push_back(Train(101, "Express Line", "Amaravati", "Hyderabad", "08:00"));
        trains.push_back(Train(202, "Coastal Rider", "Vijayawada", "Vizag", "14:30"));
    }

    void showTrains() const {
        cout << "\nAvailable Trains:\n";
        cout << left << setw(10) << "Train No"
             << setw(20) << "Train Name"
             << setw(20) << "Source"
             << setw(20) << "Destination"
             << setw(10) << "Time" << endl;
        for (const auto& train : trains) {
            train.displayDetails();
        }
    }

    void bookTicket() {
        string name, gender, journeyDate;
        int age, trainNum, coachChoice;
        showTrains();

        cout << "\nEnter Train Number to Book: ";
        cin >> trainNum;

        Train* selectedTrain = nullptr;
        for (auto& t : trains) {
            if (t.getTrainNumber() == trainNum) {
                selectedTrain = &t;
                break;
            }
        }
        if (!selectedTrain) {
            cout << "Invalid Train Number.\n";
            return;
        }

        selectedTrain->displayCoachTypes();

        cout << "Select Coach Type (Enter number 1-5): ";
        cin >> coachChoice;
        string coachType = getCoachTypeByChoice(coachChoice);
        if (coachType == "") {
            cout << "Invalid coach type selection.\n";
            return;
        }

        cout << "Enter Passenger Name: ";
        cin.ignore();
        getline(cin, name);

        cout << "Enter Age: ";
        cin >> age;

        cout << "Enter Gender (M/F): ";
        cin >> gender;

        cout << "Enter Journey Date (DD-MM-YYYY): ";
        cin >> journeyDate;

        string coachID;
        int seatNo;
        BerthType berthType;

        if (selectedTrain->bookSeat(coachType, coachID, seatNo, berthType, age)) {
            double price = getTicketPrice(coachType);
            Passenger p(name, age, gender, journeyDate, price,
                        trainNum, coachID, coachType, seatNo, berthType);
            passengers.push_back(p);
            p.printTicket(*selectedTrain);
        } else {
            cout << "No seats available in selected coach type.\n";
        }
    }

    void cancelTicket() {
        int trainNum, seatNum;
        string coachID;

        cout << "Enter Train Number: ";
        cin >> trainNum;

        cout << "Enter Coach ID (e.g., S1): ";
        cin >> coachID;

        cout << "Enter Seat Number to Cancel: ";
        cin >> seatNum;

        for (size_t i = 0; i < passengers.size(); ++i) {
            if (passengers[i].getTrainNumber() == trainNum &&
                passengers[i].getCoachID() == coachID &&
                passengers[i].getSeatNumber() == seatNum) {

                string coachType = passengers[i].getCoachType();
                double originalPrice = passengers[i].getTicketPrice();
                double cancellationFine = getCancellationFine(coachType);
                double refund = originalPrice - cancellationFine;

                passengers.erase(passengers.begin() + i);

                for (auto& train : trains) {
                    if (train.getTrainNumber() == trainNum) {
                        train.cancelSeat(coachID, seatNum);
                        break;
                    }
                }
                cout << "\n========== CANCELLATION RECEIPT ==========\n";
                cout << "Train Number     : " << trainNum << endl;
                cout << "Coach ID         : " << coachID << endl;
                cout << "Seat Number      : " << seatNum << endl;
                cout << "Original Price   : ₹" << fixed << setprecision(2) << originalPrice << endl;
                cout << "Cancellation Fine: ₹" << cancellationFine << endl;
                cout << "Refund Amount    : ₹" << refund << endl;
                cout << "===========================================\n";
                return;
            }
        }
        cout << "Ticket not found.\n";
    }
};

int main() {
    ReservationSystem system;
    int choice;
    do {
        cout << "                ===========================================\n";
        cout << "                         RAILWAY RESERVATION SYSTEM\n";
        cout << "                ===========================================\n";
        cout << "1. View Train Schedule\n";
        cout << "2. Book Ticket\n";
        cout << "3. Cancel Ticket\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: system.showTrains(); break;
            case 2: system.bookTicket(); break;
            case 3: system.cancelTicket(); break;
            case 4: cout << "Exiting system...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 4);
    return 0;
}
