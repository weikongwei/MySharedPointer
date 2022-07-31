// MySharedPointer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <list>
#include <string>
using namespace std;

class BankAccount {
	string owner;
	int accountNum;
	double balance;

public:
	BankAccount() {}
	BankAccount(const string & owner_val, double balance_val, int accountNum_val) {
		owner = owner_val;
		balance = balance_val;
		accountNum = accountNum_val;

		printf("Account added. \n");
		printf("Account number: %05d\n", accountNum);
		printf("Owner: %s \n", owner.c_str());
		printf("Balance: %0.2f\n\n", balance);
	}

	BankAccount(const BankAccount & source) {
		owner = source.owner;
		accountNum = source.accountNum;
		balance = source.balance;
		printf("A copy of account %05d created.\n", accountNum);
	}

	BankAccount(BankAccount && source) : owner(std::move(source.owner)), accountNum(source.accountNum), balance(source.balance)
	{
		printf("Account %05d moved.\n", accountNum);
	}

	~BankAccount() {
		printf("\n Destructor called for %05d.\n\n", accountNum);
	}

	BankAccount& operator = (const BankAccount &) = default;
	BankAccount& operator = (BankAccount &&) = default;

	//void operator = (const BankAccount & source)
	//{
	//	owner = source.owner;
	//	accountNum = source.accountNum;
	//	balance = source.balance;
	//	printf("A copy of account %05d created.\n", accountNum);
	//}

	//void operator = (BankAccount && source)
	//{
	//	owner = std::move(source.owner);
	//	accountNum = source.accountNum;
	//	balance = source.balance;
	//	printf("A copy of account %05d created.\n", accountNum);
	//}



	// Methods
	void setOwner(const string & name) {
		owner = name;
		printf("Owner name added: %s \n", owner.c_str());
	}

	void setAccountNumb(int accountNum_val) {
		accountNum = accountNum_val;
		printf("Account number set to %05d\n", accountNum);
	}

	void setBalance(double balance_val) {
		balance = balance_val;
	}

	void deposit(double depo) {
		balance += depo;
		printf("Deposited %.2f dollars\n", depo);
	}

	bool withdraw(double withdrw) {
		if (withdrw >= balance) {
			cout << "Insufficient fund. Withdraw faild. " << endl << endl;
			return false;
		}
		else
		{
			balance -= withdrw;
			cout << "Withdraw success. " << endl
				<< "New balance: " << balance << endl << endl;
			return true;
		}
	}

	const string & getOwner() const {
		return owner;
	}

	double getBalance() const {
		return balance;
	}

	int  getAccountNumb() const {
		return accountNum;
	}

	void printSelf() const {
		printf("Account number: %05d\n", accountNum);
		printf("Owner: %s \n", owner.c_str());
		printf("Balance: %0.2f\n\n", balance);
	}
};


template <typename T>
class MySharedPointer {
	T* p = nullptr;
	unsigned* counter = nullptr;
public:
	MySharedPointer()
		: counter(new unsigned(0)) {
	}
	~MySharedPointer() {
		this->reset();
	}
	MySharedPointer(T * sourceP)
		: p(sourceP), counter(new unsigned(1)) {
	}
	MySharedPointer(const MySharedPointer<T> & sourceMSP)
		: p(sourceMSP.p), counter(sourceMSP.counter) {
		if (sourceMSP.p)
			(*this->counter)++;
	}
	MySharedPointer(MySharedPointer<T> && sourceMSP) {
		p = sourceMSP.p;
		counter = sourceMSP.counter;
		sourceMSP.p = nullptr;
		sourceMSP.counter = nullptr;
	}


	MySharedPointer& operator=(T * sourceP) {
		p = sourceP;
		counter = 1;
	}

	// later change MySharedPointer& to void
	MySharedPointer& operator=(const MySharedPointer<T> & sourceMSP){
		p = sourceMSP.p;
		counter = sourceMSP.counter;
		if (sourceMSP.p)
			(*this->counter)++;
	}

	MySharedPointer& operator=(MySharedPointer<T> && sourceMSP) {
		p = sourceMSP.p;
		counter = sourceMSP.counter;
		sourceMSP.p = nullptr;
		sourceMSP.counter = nullptr;
	}

	T* get() {
		return this->p;
	}

	unsigned getCount() {
		return (*this->counter);
	}

	void reset() {
		(*this->counter)--;
		if ((*this->counter) == 0 ) {
			delete this->counter;
			delete this->p;
		}
	}
};


void testMSP() {
	BankAccount account("Wei", 999999999.9, 99999);
	//BankAccount* account = new BankAccount("Wei", 999999999.9, 9999);
	MySharedPointer<BankAccount> SP1(&account);
	cout << "SP1's count at creation: " << SP1.getCount() << "\n";  //1
	MySharedPointer<BankAccount> SP2(&account);
	cout << "SP2's count at creation: " << SP2.getCount() << "\n";  //1
	MySharedPointer<BankAccount> SP3(SP2);
	cout << "SP3's count at creation: " << SP3.getCount() << "\n";  //2
	MySharedPointer<BankAccount> SP4(move(SP2));
	cout << "SP4's count at creation: " << SP4.getCount() << "\n";  //2
	SP3.reset();
	cout << "SP4's count after SP3 reset: " << SP4.getCount() << "\n";  //1
	MySharedPointer<BankAccount> SP5 = SP4;
	cout << "SP5's count at creation: " << SP5.getCount() << "\n";  //2
	MySharedPointer<BankAccount> SP6 = move(SP5);
	cout << "SP6's count at creation: " << SP6.getCount() << "\n";  //2


	SP1.reset();
	//cout << "SP1's counter: " << SP1.getCount() << " \n";
	//if (SP1.get() == nullptr)
	//cout << "SP1 is freed.\n"; //<< (SP1.get()->getAccountNumb()) << " \n";
}

int main()
{	
	testMSP();

	return 0;
}





