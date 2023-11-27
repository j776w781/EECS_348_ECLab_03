#include <iostream>
#include <string>
using namespace std;


class Account {
    //Research and trial/error yielded that the protected keyword would be suitable for base attributes that derived classes can access.
    protected:
        string accountNumber;
        string accountHolder;
        double balance;
    public:
        Account (string acc_num, string holder, double blnc = 0) {
            accountNumber = acc_num;
            accountHolder = holder;
            balance = blnc;
        }

        //Virtual is used to ensure that overrided versions of displayDetails are called when the << operator is used.
        virtual void displayDetails() {
            cout << "Account Details for Account (ID: " << accountNumber << "):" << endl;
            cout << "\tHolder: " << accountHolder << endl;
            printf("\tBalance: $%.2lf\n", balance);
        }

        void deposit(double amount) {
            balance+=amount;
        }

        //The basic version of withdraw does not withdraw if a minimum balance of 0 is not maintained. It ensures, not enforces, a minimum balance.
        virtual void withdraw(double amount) {
            if (balance - amount >= 0)
                balance-=amount;
        }

        //The basic operator overload returns the updated account as the result of the addition, where the righthand account's money is transferred into the lefthand account.
        //Since a basic Account instance is returned, the + operator must be overloaded in the subclasses as well.
        //A try/catch block is used to prevent unauthorized overdrafts during transfer.
        //Research yielded the *this as a quasi-version of the self keyword from python. By returning *this, something like current = current + savings can be done.
        Account operator+(Account& RHS) {
            try {
                RHS.withdraw(300);
                deposit(300);
            } catch (const char* message) {
                cout << message;
            }
            return *this;
        }

        //The friend keyword is used so that the overload can be defined outside of the class definition. The ostream os will typically be cout. The account will be the account instance
        //that should be printed. The advantage of defining the overload outside of the class definition is that the account being printed can be included as a parameter of the overload.
        //I found that it was difficult to include the Account instance if the friend keyword was not included.
        friend ostream& operator<<(ostream& os, Account& account);
};

//When cout << acc is used in code, the displayDetails() method should simply be called. Since the operator overload returns an ostream (cout), the passed in os is simply returned.
ostream& operator<<(ostream& os, Account& account) {
    account.displayDetails();
    return os;
}



class SavingsAccount : public Account {
    private:
        double interestRate;
    public:
        //The same as the Account Constructor, but assigns an interest rate.
        SavingsAccount (string acc_num, string holder, double blnc, double intrst) : Account(acc_num, holder, blnc), interestRate(intrst) {}

        //This version converts the interest rate to a percentage and includes it in the presentation.
        void displayDetails() override {
            Account::displayDetails();
            double temp = interestRate * 100;
            printf("\tInterest Rate: %.2lf", temp);
            cout << "%" << endl;
        }

        //This version raises an error if the post-withdrawal balance is less than 0. Therefore, a minimum balance is aggressively enforced.
        void withdraw(double amount) override {
            if (balance - amount >= 0) {
                balance-=amount;
            } else {
                throw "Withdrawal is greater than current balance.";
            }
        }

        //This is used so that a SavingsAccount instance is returned.
        SavingsAccount operator+(Account& RHS) {
            try {
                RHS.withdraw(300);
                deposit(300);
            } catch (const char* message) {
                cout << message;
            }
            return *this;
        }

};

class CurrentAccount : public Account {
    private:
        double overdraftlimit;
    public:
        //The same as the Account constructor, but adds an overdraft limit.
        CurrentAccount (string acc_num, string holder, double blnc, double over) : Account(acc_num, holder, blnc), overdraftlimit(over) {}

        //This version simply adds the overdraft limit to the presentation.
        void displayDetails() override {
            Account::displayDetails();
            printf("\tOverdraft Limit: $%.2lf\n", overdraftlimit);
        }

        //This version throws an error if the post withdrawal balance is past the overdraft limit.
        void withdraw(double amount) override {
            if (balance - amount >= 0 - overdraftlimit) {
                balance-=amount;
            } else {
                throw "Overdraft limit exceeded.";
            }
        }

        //A CurrentAccount instance is returned for this operator overload.
        CurrentAccount operator+(Account& RHS) {
            try {
                RHS.withdraw(300);
                deposit(300);
            } catch (const char* message) {
                cout << message;
            }
            return *this;
        }

};



int main() {
    //Instance Creation.
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    cout << savings << endl;
    cout << current << endl;

    //Demonstration of depositing.
    savings.deposit(500);
    current.deposit(600);

    cout << savings << endl;
    cout << current << endl;

    //Demonstration of withdrawals.
    savings.withdraw(500);
    current.withdraw(600);

    cout << savings << endl;
    cout << current << endl;


    //Demonstration of displayDetails() method (balance inquiry).
    savings.displayDetails();
    cout << endl;
    current.displayDetails();
    cout << endl;

    //Transfer $300 from savings to current.
    current = current + savings;

    cout << savings << endl;
    cout << current << endl;

    //Demonstrating withdrawal that exceeds overdraft limit.
    try {
        current.withdraw(2801);
    } catch (const char* message) {
        cout << message << endl;
    }

    //Demonstrating withdrawal that fails minimum balance of 0.
    try {
        savings.withdraw(800);
    } catch (const char* message) {
        cout << message << endl;
    }

    cout << endl;


    return 0;
}