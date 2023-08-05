#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;
#define MIN_BALANCE 500
class InsufficientFunds
{
};

class Account
{
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    static long NextAccountNumber;

public:
    Account() {}
    Account(string fname, string lname, float balance);
    long getAccNo() { return accountNumber; }
    string getFirstName() { return firstName; }
    string getLastName() { return lastName; }
    float getBalance() { return balance; }

    void Deposite(float amount);
    void Withdraw(float amount);
    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();
    friend ofstream &operator<<(ofstream &ofs, Account &acc);
    friend ifstream &operator>>(ifstream &ifs, Account &acc);
    friend ostream &operator<<(ostream &os, Account &acc);
};

long Account::NextAccountNumber = 0;

class Bank
{
private:
    map<long, Account> accounts;

public:
    Bank();
    Account OpenAccount(string fname, string lname, float balance);
    Account BalanceEnquiry(long accountNumber);
    Account Deposite(long accountNumber, float amount);
    Account Withdraw(long accountNumber, float amount);
    void closeAccount(long accountNumber);
    void showAllAccount();
    ~Bank();
};

int main()
{

    Bank b;
    Account acc;
    int choice;
    string fname, lname;
    long accountNumber;
    float balance;
    float amount;
    cout << "***Banking System***" << endl;
    do
    {
        cout << "\n\t select one option below ";
        cout << "\n\t 1.open an account ";
        cout << "\n\t 2.Balance Enquiry ";
        cout << "\n\t 3.Deposite ";
        cout << "\n\t 4.Withdraw ";
        cout << "\n\t 5.close an account ";
        cout << "\n\t 6.show all account ";
        cout << "\n\t 7.Quite ";
        cout << "\n\t 8.Enter your choice ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "Enter First name ";
            cin >> fname;
            cout << "Enter Last name ";
            cin >> lname;
            cout << "enter initial balance : ";
            cin >> balance;
            acc = b.OpenAccount(fname, lname, balance);
            cout << endl
                 << "Congratulation account is created " << endl;
            cout << acc;
            break;

        case 2:
            cout << "Enter AccountNumber : ";
            cin >> accountNumber;
            acc = b.BalanceEnquiry(accountNumber);
            cout << endl
                 << "your account details" << endl;
            cout << acc;
            break;

        case 3:
            cout << "Enter AccountNumber : ";
            cin >> accountNumber;
            cout << "enter balance ";
            cin >> amount;
            acc = b.Deposite(accountNumber, amount);
            cout << endl
                 << "amount is deposited " << endl;
            cout << acc;
            break;

                case 4:
            cout << "Enter AccountNumber : ";
            cin >> accountNumber;
            cout << "enter balance ";
            cin >> amount;
            acc = b.Withdraw(accountNumber, amount);
            cout << endl
                 << "Amount Withdraw " << endl;
            cout << acc;
            break;

        case 5:
            cout << "Enter AccountNumber : ";
            cin >> accountNumber;
            b.closeAccount(accountNumber);
            cout << endl
                 << "account is closed " << endl;
            cout << acc;
            break;

        case 6:
            b.showAllAccount();
            break;

        case 7:
            break;
        default:
            cout << "\n enter correct choice ";
            exit(0);
        }
    } while (choice != 7);

    return 0;
}

Account::Account(string fname, string lname, float balance)
{
    NextAccountNumber++;
    accountNumber = NextAccountNumber;
    firstName = fname;
    lastName = lname;
    this->balance = balance;
}

void Account::Deposite(float amount)
{
    balance += amount;
}

void Account::Withdraw(float amount)
{
    if (balance - amount < MIN_BALANCE)
        throw InsufficientFunds();
    balance -= amount;
}

void Account::setLastAccountNumber(long accountNumber)
{
    NextAccountNumber = accountNumber;
}

long Account ::getLastAccountNumber()
{
    return NextAccountNumber;
}

ofstream &operator<<(ofstream &ofs, Account &acc)
{
    ofs << acc.accountNumber << endl;
    ofs << acc.firstName << endl;
    ofs << acc.lastName << endl;
    ofs << acc.balance << endl;
    return ofs;
}

ifstream &operator>>(ifstream &ifs, Account &acc)
{
    ifs >> acc.accountNumber;
    ifs >> acc.firstName;
    ifs >> acc.lastName;
    ifs >> acc.balance;
    return ifs;
}

ostream &operator<<(ostream &os, Account &acc)
{
    os << "FirstName : " << acc.getFirstName() << endl;
    os << "LastName : " << acc.getLastName() << endl;
    os << "Account Number : " << acc.getAccNo() << endl;
    os << "Balance :" << acc.getBalance() << endl;
    return os;
}

Bank ::Bank()
{
    Account account;
    ifstream infile;
    infile.open("Bank.data");
    if (!infile)
    {
        cout << "error in openning : FileNotFound!!" << endl;
        return;
    }
    while (!infile.eof())
    {
        infile >> account;
        accounts.insert(pair<long, Account>(account.getAccNo(), account));
    }
    Account::setLastAccountNumber(account.getAccNo());
    infile.close();
}

Account Bank ::OpenAccount(string fname, string lname, float balance)
{
    ofstream outfile;
    Account account(fname, lname, balance);
    accounts.insert(pair<long, Account>(account.getAccNo(), account));

    outfile.open("Bank.data", ios::trunc);

    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        outfile << itr->second;
    }
    outfile.close();
    return account;
}

Account Bank::BalanceEnquiry(long accountNumber)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    return itr->second;
}
Account Bank::Deposite(long accountNumber, float amount)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    itr->second.Deposite(amount);
    return itr->second;
}

Account Bank::Withdraw(long accountNumber, float amount)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    itr->second.Withdraw(amount);
    return itr->second;
}

void Bank ::closeAccount(long accountNumber)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    cout << "Account deleted" << itr->second;
    accounts.erase(accountNumber);
}

void Bank::showAllAccount()
{
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        cout << "Account " << itr->first << endl
             << itr->second << endl;
    }
}

Bank::~Bank()
{
    ofstream outfile;
    outfile.open("Bank.data", ios::trunc);

    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        outfile << itr->second;
    }
    outfile.close();
}