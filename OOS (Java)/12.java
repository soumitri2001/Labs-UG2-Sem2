/* 
12) Write a class for “Account” containing data members ‘accountNumber’, ‘holderName’,
‘balance’ and add constructors and necessary accessor/modifier functions for these data
members. Now create two class “SavingsAccount” and “CurrentAccount” extending from this
class. SavingsAccount will have a member variable ‘interestRate’ and member function
‘calculateYearlyInterest’. Write another class “Manager” that contains a list Account. Also write
a main() function to create an instance of Manager class. Add two SavingsAccount and three
CurrentAccount to Manager. Calculate interest of each SavingsAccount. Print the details of all
accounts. 
*/

import java.util.HashMap;

class Account {

    int accountNumber;
    String holderName;
    double balance;

    static int count = 0;

    public Account(String holderName, double balance) {
        this.accountNumber = generateAccountNumber();
        this.holderName = holderName;
        this.balance = balance;
    }

    public void display() {
        System.out.print("\nAccount Number : " + accountNumber);
        System.out.print("\nHolder Name : " + holderName);
        System.out.print("\nBalance : " + balance + " rupees only");
    }

    private static int generateAccountNumber() {
        return count++;
    }
}

class SavingsAccount extends Account {

    double interestRate;

    public SavingsAccount(String holderName, double balance, double interestRate) {
        super(holderName, balance);
        this.interestRate = interestRate;
    }

    public double yearlyInterest() {
        return balance * interestRate / 100;
    }

    @Override
    public void display() {
        super.display();
        System.out.print("\nInterest Rate : " + interestRate + "%  ONLY !!!");
        System.out.print("\nYearly Interest : " + this.yearlyInterest() + " rupees only");
    }
}

class CurrentAccount extends Account {

    public CurrentAccount(String holderName, double balance) {
        super(holderName, balance);
    }

}

class Manager {

    HashMap<Integer, Account> hm;

    public Manager() {
        hm = new HashMap<>();
    }

    public void addAccount(Account acc) {
        hm.put(acc.accountNumber, acc);
    }

    public void display() {
        int i = 0;

        for (int accountNumber : hm.keySet()) {
            System.out.print("\n\nAccount:" + (++i));
            System.out.print("\n________________________");

            hm.get(accountNumber).display();
        }
    }

}

class Main_Q12 {
    public static void main(String arg[]) {

        Manager man = new Manager();
        Account acc;

        acc = new Account("Charles Boyle", 100000);
        man.addAccount(acc);

        acc = new Account("Jack Peralta", 100);
        man.addAccount(acc);

        acc = new Account("Rosa Diaz", 1234567);
        man.addAccount(acc);

        acc = new SavingsAccount("Raymond Holt", 12345678, 25);
        man.addAccount(acc);

        acc = new SavingsAccount("Gina Linetti", 100, 10);
        man.addAccount(acc);

        System.out.print("\n\n\nDetails of all accounts :\n____________________________________\n");

        man.display();
    }
}