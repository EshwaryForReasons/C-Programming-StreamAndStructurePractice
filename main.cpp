
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct Account
{
    int ID;
    std::string email;
    double balance;
};

const int ACCOUNT_NUM = 10;

Account* id_to_account(Account* accounts[], int current_account_num, int ID)
{
    for(int i = 0; i < current_account_num; ++i)
    {
        if(accounts[i] && accounts[i]->ID == ID)
            return accounts[i];
    }

    return nullptr;
}

bool add_account(Account* accounts[], int& current_account_num, const Account& account)
{
    if(current_account_num >= ACCOUNT_NUM || id_to_account(accounts, current_account_num, account.ID))
        return false;
    
    accounts[current_account_num] = new Account();
    accounts[current_account_num]->ID = account.ID;
    accounts[current_account_num]->email = account.email;
    accounts[current_account_num]->balance = account.balance;
    ++current_account_num;
    return true;
}

bool read_data(Account* accounts[], int& current_account_num, const std::string& path)
{
    std::fstream file(path, std::ios_base::in);

    if(file.fail())
        return false;

    std::string line;
    std::string temp;

    int i = 0;
    while(std::getline(file, line))
    {
        if(current_account_num >= ACCOUNT_NUM)
            return false;

        Account account;
        std::stringstream ss(line);

        int j = 0;
        while(std::getline(ss, temp, ' '))
        {
            if(j == 0)
                account.ID = std::stoi(temp);
            else if(j == 1)
                account.email = temp;
            else if(j == 2)
                account.balance = std::stod(temp);

            ++j;
        }

        add_account(accounts, current_account_num, account);
        ++i;
    }

    for(int j = 0; j < current_account_num; ++j)
    {
        std::cout << accounts[j]->ID << " " << accounts[j]->email << " " << accounts[j]->balance << std::endl;
    }

    return true;
}

bool update_data(Account* accounts[], int current_account_num, const std::string& path)
{
    std::fstream file(path, std::ios_base::out);

    if(file.fail())
        return false;

    for(int i = 0; i < current_account_num; ++i)
        file << accounts[i]->ID << " " << accounts[i]->email << " " << accounts[i]->balance << "\n";
    
    return true;
}

//Read in accounts based on provided file path
bool initialize(Account* accounts[], int& current_account_num, std::string& file_path)
{
    std::cout << "Please enter a data file path relative to the execution directory: ";
    std::cin >> file_path;

    //Read in account data and store into array
    if(!read_data(accounts, current_account_num, file_path))
    {
        std::cout << "Could not find data file or data file contains more than the maximum allowed " << ACCOUNT_NUM << " accounts. Quitting." << std::endl;
        return false;
    }

    std::cout << "Successfully loaded user accounts!" << std::endl;
    return true;
}

//Create a new account based on user input
void create_account(Account* accounts[], int& current_account_num)
{
    Account account;

    std::string temp;
    std::cout << "Enter an ID for the new account: ";
    std::cin >> account.ID;
    std::cout << "Enter the email for the new account: ";
    std::cin >> account.email;
    std::cout << "Enter a balance for the new account: ";
    std::cin >> account.balance;

    if(!add_account(accounts, current_account_num, account))
        std::cout << "Could not add new account. Either max account limit " << ACCOUNT_NUM << " has been reached or a duplicate ID was provided." << std::endl;
}

//Update account based on provided ID
void update_account(Account* accounts[], int& current_account_num, std::string& file_path)
{
    double temp;
    
    std::cout << "Enter account ID to update: ";
    std::cin >> temp;

    if(Account* account = id_to_account(accounts, current_account_num, (int)temp))
    {
        std::cout << "Current balance: " << account->balance << "\n";
        std::cout << "How much money would you like to deposit into this account? Enter amount: ";
        std::cin >> temp;
        account->balance += temp;
        std::cout << "Done! New balance: " << account->balance << std::endl;

        if(!update_data(accounts, current_account_num, file_path))
            std::cout << "Could not update data file. Make sure it exists." << std::endl;
    }
    else
        std::cout << "No account found with that ID" << std::endl;
}

int main()
{
    std::string file_path;
    Account* accounts[ACCOUNT_NUM];
    int current_account_num = 0;

    if(!initialize(accounts, current_account_num, file_path))
        return 0;
    
    create_account(accounts, current_account_num);
    update_account(accounts, current_account_num, file_path);
    return 0;
}