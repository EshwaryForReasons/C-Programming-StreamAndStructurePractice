
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

const char* FILE_PATH = "../Data.txt";
const int ACCOUNT_NUM = 10;
int current_account_num = 0;
Account* accounts[ACCOUNT_NUM];

Account* id_to_account(int ID)
{
    for(int i = 0; i < current_account_num; ++i)
    {
        if(accounts[i] && accounts[i]->ID == ID)
            return accounts[i];
    }

    return nullptr;
}

bool read_data(const char* path)
{
    std::fstream file(path, std::ios_base::in);

    if(file.fail())
        return false;

    std::string line;
    std::string temp;

    int i = 0;
    while(std::getline(file, line))
    {
        Account* account = new Account;
        std::stringstream ss(line);

        int j = 0;
        while(std::getline(ss, temp, ' '))
        {
            if(j == 0)
                account->ID = std::stoi(temp);
            else if(j == 1)
                account->email = temp;
            else if(j == 2)
                account->balance = std::stod(temp);

            ++j;
        }

        accounts[i] = account;
        ++current_account_num;
        ++i;
    }

    return true;
}

bool update_data(const char* path)
{
    std::fstream file(path, std::ios_base::out);

    if(file.fail())
        return false;

    for(int i = 0; i < current_account_num; ++i)
        file << accounts[i]->ID << " " << accounts[i]->email << " " << accounts[i]->balance << "\n";
    
    return true;
}

int main()
{
    if(!read_data(FILE_PATH))
    {
        std::cout << "Could not find data file. Quitting." << std::endl;
        return 0;
    }

    double temp;
    
    std::cout << "Enter account ID to update: ";
    std::cin >> temp;

    if(Account* account = id_to_account((int)temp))
    {
        std::cout << "Currnet balance: " << account->balance << "\n";
        std::cout << "How much money would you like to deposit into this account? Enter amount: ";
        std::cin >> temp;
        account->balance += temp;
        std::cout << "Done! New balance: " << account->balance << std::endl;

        if(!update_data(FILE_PATH))
            std::cout << "Could not update data file. Make sure it exists." << std::endl;
    }
    else
        std::cout << "No account found with that ID" << std::endl;

    return 0;
}