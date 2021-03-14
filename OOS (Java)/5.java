/*
5) Write a class “BankAccount” with the following instance variables:
AccountNumber (an integer), balance a floating-point number), 
and “ownerName” (a String). Write proper constructor for this class. 
Also write methods balance, add (to deposit an amount), and subtract (to withdraw an amount) and implement them. 
Now create another class “AccountManager” that contains an array of BankAccount. 
Write methods create (to create an account), delete(to terminate an account), 
deposit (to deposit an amount to an account) and withdraw (to withdraw an amount from an account). 
Also write a class “Bank”, add main() function that creates an AccountManager and add 5 accounts. 
Now print the details of all accounts in this Bank
 */

class BankAccount {
    private int accountNumber;
    private double balance;
    private String ownerName;

    public BankAccount(String ownerName, int accountNumber) // new account => balance=0.0
    {
        this.ownerName = ownerName;
        this.accountNumber = accountNumber;
        this.balance = 0.0;
    }

    double getBalance() {
        return this.balance;
    }

    void add(double amt) {
        this.balance += amt;
    }

    void subtract(double amt) {
        if (amt > this.balance)
            System.out.println("Insufficient balance !");
        else
            this.balance -= amt;
    }
}

class AccountManager {
    private BankAccount bank_acc[];

    public AccountManager(int nos_acc) {
        this.bank_acc = new BankAccount[nos_acc];
    }
}