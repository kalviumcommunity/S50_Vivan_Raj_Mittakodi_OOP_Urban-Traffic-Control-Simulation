#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Abstract class with pure virtual functions
class IViolation {
public:
    virtual string getDescription() const = 0;  // Pure virtual function
    virtual double getAmount() const = 0;       // Pure virtual function
    virtual ~IViolation() = default;            // Virtual destructor
};

// Base class for traffic violations
class TrafficViolation : public IViolation {
protected:
    string description;

public:
    // Liskov Substitution Principle (LSP): TrafficViolation can replace IViolation
    TrafficViolation(const string& desc) : description(desc) {}
    virtual ~TrafficViolation() {}

    string getDescription() const override {
        return description;
    }

    double getAmount() const override {
        return 0.0;  // Default penalty amount
    }
};

// Subclass for SpeedingViolation
class SpeedingViolation : public TrafficViolation {
private:
    double speedOverLimit;

public:
    // Liskov Substitution Principle (LSP): SpeedingViolation can replace TrafficViolation
    SpeedingViolation(const string& desc, double speed) 
        : TrafficViolation(desc), speedOverLimit(speed) {}

    double getAmount() const override {
        return 100.0 + (speedOverLimit * 10);  // Penalty for speeding
    }
};

// Subclass for ParkingViolation
class ParkingViolation : public TrafficViolation {
public:
    // Liskov Substitution Principle (LSP): ParkingViolation can replace TrafficViolation
    ParkingViolation(const string& desc) : TrafficViolation(desc) {}

    double getAmount() const override {
        return 50.0;  // Fixed penalty for parking violation
    }
};

class Penalty : public IViolation {
private:
    double amount;
    TrafficViolation& violation;

public:
    // Liskov Substitution Principle (LSP): Penalty can replace IViolation
    Penalty(double amt, TrafficViolation& viol) : amount(amt), violation(viol) {}

    string getDescription() const override {
        return violation.getDescription();  // Call base class method
    }

    double getAmount() const override {
        return amount;  // Return the penalty amount
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
            total += penalty.getAmount();  // Polymorphic call
        }
        return total;
    }
};

int Driver::totalDrivers = 0;

class Car {
private:
    string regNumber;
    Driver& owner;
    vector<IViolation*> violations;

public:
    static int totalViolations;

    Car(const string& reg_num, Driver& own) : regNumber(reg_num), owner(own) {}

    ~Car() {
        for (auto viol : violations) {
            delete viol;  // Polymorphic deletion of objects
        }
    }

    void recordViolation(IViolation* violation) {
        // Liskov Substitution Principle (LSP): Any subclass of IViolation can be used here
        violations.push_back(violation);
        if (Penalty* penalty = dynamic_cast<Penalty*>(violation)) {
            owner.addPenalty(*penalty);
        }
        totalViolations++;
    }

    string getRegNumber() const {
        return regNumber;
    }

    Driver& getOwner() const {
        return owner;
    }
};

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

    IViolation** violations = new IViolation*[numViolations];  // Liskov Substitution Principle (LSP): Array of IViolation can store derived classes
    double* penalties = new double[numViolations];

    for (int i = 0; i < numViolations; ++i) {
        string violationDesc;
        double penaltyAmount;

        cout << "Enter violation description " << i + 1 << ": ";
        getline(cin, violationDesc);

        // Liskov Substitution Principle (LSP): Creating objects of derived classes and storing them as base class type
        if (i % 2 == 0) {
            violations[i] = new SpeedingViolation(violationDesc, 15);  
        } else {
            violations[i] = new ParkingViolation(violationDesc);  
        }

        cout << "Enter penalty amount " << i + 1 << ": ";
        cin >> penaltyAmount;
        cin.ignore();
        penalties[i] = penaltyAmount;

        // Liskov Substitution Principle (LSP): Penalty stored as IViolation
        Penalty* penalty = new Penalty(penaltyAmount, *violations[i]);
        car.recordViolation(penalty);
    }

    cout << "Total penalties for " << driver.getName() << ": " << driver.getTotalPenalties() << endl;
    cout << "Total drivers created: " << Driver::totalDrivers << endl;
    cout << "Total violations recorded: " << Car::totalViolations << endl;

    delete[] violations;
    delete[] penalties;

    return 0;
}
