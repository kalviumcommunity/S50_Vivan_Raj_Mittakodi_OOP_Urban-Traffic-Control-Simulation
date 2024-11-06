#include <iostream>
#include <vector>
#include <string>

using namespace std;

class TrafficViolation {
private:
    string description;

public:
    TrafficViolation(const string& desc) : description(desc) {}

    string getDescription() const {
        return description;
    }
};

class Penalty {
private:
    double amount;
    TrafficViolation violation;

public:
    Penalty(double amt, const TrafficViolation& viol) : amount(amt), violation(viol) {}

    double getAmount() const {
        return amount;
    }

    TrafficViolation getViolation() const {
        return violation;
    }
};

class Driver {
private:
    string name;
    string licenseNumber;
    vector<Penalty> penalties;

public:
    static int totalDrivers;

    Driver(const string& n, const string& license) : name(n), licenseNumber(license) {
        totalDrivers++;
    }

    string getName() const {
        return name;
    }

    string getLicenseNumber() const {
        return licenseNumber;
    }

    void addPenalty(const Penalty& penalty) {
        penalties.push_back(penalty);
    }

    double getTotalPenalties() const {
        double total = 0;
        for (const auto& penalty : penalties) {
            total += penalty.getAmount();
        }
        return total;
    }
};

// Initialize static member variable
int Driver::totalDrivers = 0;

class Car {
private:
    string regNumber;
    Driver& owner;
    vector<TrafficViolation> violations;

public:
    static int totalViolations;

    Car(const string& reg_num, Driver& own) : regNumber(reg_num), owner(own) {}

    void recordViolation(const TrafficViolation& violation, double penalty_amount) {
        violations.push_back(violation);
        owner.addPenalty(Penalty(penalty_amount, violation));
        totalViolations++;
    }

    string getRegNumber() const {
        return regNumber;
    }

    Driver& getOwner() const {
        return owner;
    }
};

// Initialize static member variable
int Car::totalViolations = 0;

int main() {
    string driverName, licenseNumber, regNumber;
    int numViolations;

    cout << "Enter driver's name: ";
    getline(cin, driverName);

    cout << "Enter driver's license number: ";
    getline(cin, licenseNumber);
    Driver driver(driverName, licenseNumber);

    cout << "Enter vehicle registration number: ";
    getline(cin, regNumber);
    Car car(regNumber, driver);

    cout << "Enter number of violations: ";
    cin >> numViolations;
    cin.ignore();

    TrafficViolation* violations = new TrafficViolation[numViolations];
    double* penalties = new double[numViolations];

    for (int i = 0; i < numViolations; ++i) {
        string violationDesc;
        double penaltyAmount;

        cout << "Enter violation description " << i + 1 << ": ";
        getline(cin, violationDesc);
        violations[i] = TrafficViolation(violationDesc);

        cout << "Enter penalty amount " << i + 1 << ": ";
        cin >> penaltyAmount;
        cin.ignore();
        penalties[i] = penaltyAmount;

        car.recordViolation(violations[i], penalties[i]);
    }

    cout << "Total penalties for " << driver.getName() << ": " << driver.getTotalPenalties() << endl;

    cout << "Total drivers created: " << Driver::totalDrivers << endl;
    cout << "Total violations recorded: " << Car::totalViolations << endl;

    delete[] violations;
    delete[] penalties;

    return 0;
}
