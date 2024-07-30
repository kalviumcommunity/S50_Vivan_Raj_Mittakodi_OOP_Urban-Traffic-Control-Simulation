#include <iostream>
#include <vector>
#include <string>

using namespace std;

class TrafficViolation {
public:
    string description;

    TrafficViolation(const string & desc) : description(desc) {}
};

class Penalty {
public:
    double amount;
    TrafficViolation violation;

    Penalty(double amt, const TrafficViolation& viol) : amount(amt), violation(viol) {}
};

class Driver {
public:
    string name;
    string licenseNumber;
    vector<Penalty> penalties;

    Driver(const string& n, const string& license) : name(n), licenseNumber(license) {}

    void addPenalty(const Penalty& penalty) {
        this->penalties.push_back(penalty);
    }

    double getTotalPenalties() const {
        double total = 0;
        for (const auto& penalty : this->penalties) {
            total += penalty.amount;
        }
        return total;
    }
};

class Car {
public:
    string regNumber;
    Driver& owner;
    vector<TrafficViolation> violations;

    Car(const string& reg_num, Driver& own) : regNumber(reg_num), owner(own) {}

    void recordViolation(const TrafficViolation& violation, double penalty_amount) {
        this->violations.push_back(violation);
        this->owner.addPenalty(Penalty(penalty_amount, violation));
    }
};

int main() {
    string driverName, licenseNumber, regNumber, violationDesc;
    double penaltyAmount;

    cout << "Enter driver's name: ";
    getline(cin, driverName);

    cout << "Enter driver's license number: ";
    getline(cin, licenseNumber);
    Driver driver(driverName, licenseNumber);

    cout << "Enter vehicle registration number: ";
    getline(cin, regNumber);
    Car car(regNumber, driver);

    cout << "Enter violation description: ";
    getline(cin, violationDesc);

    cout << "Enter penalty amount: ";
    cin >> penaltyAmount;
    cin.ignore();

    TrafficViolation violation(violationDesc);
    car.recordViolation(violation, penaltyAmount);

    cout << "Total penalties for " << driver.name << ": " << driver.getTotalPenalties() << endl;

    return 0;
}