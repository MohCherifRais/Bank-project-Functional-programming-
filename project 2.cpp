#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
struct sClient {
    string AcountNumber;
    string Pincode;
    string Name;
    string Phone;
    double Accountbalance;
};

struct sUser {
    string UserName;
    string Password;
    int Permissions;
};

sUser CurrentUser;
const string ClientsFileName = "Clients";
const string UsersFileName = "Users";

enum enMainMenueOptions
{
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6,
    eManageUsers = 7, eExit = 8
};
enum enTransactionsMenue
{
    eDeposit = 1, eWithdraw = 2,
    eTotalBalances = 3, eMainMenuefromT = 4
};
enum enManageUsersMenue
{
    eListUsers = 1, eAddUser = 2,
    eDeleteUser = 3, eUpdateUser = 4,
    eFindUser = 5, eMainMenuefromU = 6,
};
enum enMainMenuePermissions {
    eALl = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
    pUpdateClient = 8, pFindClient = 16, pTransactions = 32, pManageUsers = 64
};

void ShowMainMenue();
void GoBackToMainMenue();
void ShowTranscationsMenue();
void ShowManageUsersMenue();
void ShowAccessDeniedScreen();

bool CheckAccessPermisison(enMainMenuePermissions Permission)         // Check Permission of each operation
{
    if (CurrentUser.Permissions == enMainMenuePermissions::eALl)
        return true;
    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;
    else
        return false;
}

vector <string> SplitString(string S1, string delim)
{
    vector <string> vString;
    short pos = 0;
    string sWord;                  // define a string variable

    while ((pos = S1.find(delim)) != std::string::npos)      // use find() function to get the position of the delimeters
    {
        sWord = S1.substr(0, pos);                            // store the word 

        if (sWord != "")
        {
            vString.push_back(sWord);                         // push the word into vector
        }
        S1.erase(0, pos + delim.length());   // erase() until position and move to next word.
    }

    if (S1 != "")
    {
        vString.push_back(S1);   // this save the last word of the string. 
    }
    return vString;
}
sClient ConvertLineToRecord(string Line, string Seperator = "#//#")
{
    sClient client;
    vector <string> vClientData;

    vClientData = SplitString(Line, Seperator);
    client.AcountNumber = vClientData[0];
    client.Pincode = vClientData[1];
    client.Name = vClientData[2];
    client.Phone = vClientData[3];
    client.Accountbalance = stod(vClientData[4]);

    return client;
}
vector <sClient> LoadClientsDataFromFile(string Filename)
{
    vector <sClient> vClients;           // vector of structure  
    fstream MyFile;
    MyFile.open(Filename, ios::in);     // readMode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}
void PrintClientRecord(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AcountNumber;
    cout << "| " << setw(10) << left << Client.Pincode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.Accountbalance;

}
void ShowAllClientsScreen()
{
    if (!CheckAccessPermisison(enMainMenuePermissions::pListClients))
    {
        ShowAccessDeniedScreen();
        return;
    }

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\t Client List (" << vClients.size() << ") Client(s)";
    cout << "\n------------------------------------------------------------------------------------------------------\n" << endl;
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "PinCode";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Account Balance";
    cout << "\n\n------------------------------------------------------------------------------------------------------\n" << endl;

    for (sClient client : vClients)
    {
        PrintClientRecord(client);
        cout << endl;
    }
    cout << "\n------------------------------------------------------------------------------------------------------\n" << endl;
}


bool CheckClientByAccountNumber(string AccountNumber)
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    for (sClient& C : vClients)
    {
        if (C.AcountNumber == AccountNumber)
        {
            return true;
            break;
        }
    }
    return false;
}
string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AcountNumber + Seperator;
    stClientRecord += Client.Pincode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.Accountbalance);
    return stClientRecord;
}
void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}
sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AcountNumber);

    while (CheckClientByAccountNumber(Client.AcountNumber))
    {
        cout << "Client With [" << Client.AcountNumber << "] Already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AcountNumber);
    }
    cout << "\nEnter PinCode? ";
    getline(cin, Client.Pincode);
    cout << "Enter Name? ";
    getline(cin, Client.Name);
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? ";
    cin >> Client.Accountbalance;

    return Client;
}
void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}
void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        cout << "\nAdding New Client:\n";
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients ? Y / N ? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');

}
void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermisison(enMainMenuePermissions::pAddNewClient))
    {
        ShowAccessDeniedScreen();
        return;
    }
    system("cls");
    cout << "------------------------------------------" << endl;
    cout << "\t Add New Clients Screen" << endl;
    cout << "------------------------------------------" << endl;
    AddNewClients();

}


string ReadAccountNumber()
{
    string AccountNumber;
    cout << "Please Enter Account Number?  ";
    cin >> AccountNumber;
    return AccountNumber;

}
bool FindClientByAccountNumber(string AccountNumber, sClient& client)                                                   // change   
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    for (sClient& C : vClients)
    {
        if (C.AcountNumber == AccountNumber)
        {
            client = C;
            return true;
        }

    }
    return false;
}
void PrintClient(sClient client)
{
    cout << "\n\nThe following are the Client informations:\n";
    cout << "--------------------------------------------";
    cout << "\nAccount Number: " << client.AcountNumber;
    cout << "\nPin code: " << client.Pincode;
    cout << "\nName: " << client.Name;
    cout << "\nPhone: " << client.Phone;
    cout << "\nAccout Balance: " << client.Accountbalance;
    cout << "\n--------------------------------------------\n\n" << endl;

}
void ShowFindClientScreen()
{
    if (!CheckAccessPermisison(enMainMenuePermissions::pFindClient))
    {
        ShowAccessDeniedScreen();
        return;
    }
    cout << "------------------------------------" << endl;
    cout << "\tFind Client Screen" << endl;
    cout << "------------------------------------" << endl;
    sClient client;

    string AccountNumber = ReadAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, client))
    {
        PrintClient(client);
    }
    else
    {
        cout << "\n\nClient with Account Number " << AccountNumber << " Is Not Found!\n" << endl;
    }
}

void SaveClientsDataToFile(string Filename, vector <sClient> vClients)
{
    fstream Myfile;
    Myfile.open(Filename, ios::out);    // overwrite
    string DataLine;

    if (Myfile.is_open())
    {
        for (sClient& C : vClients)
        {
            {
                DataLine = ConvertRecordToLine(C);
                Myfile << DataLine << endl;
            }
        }
        Myfile.close();
    }

}
void AddClientsToDataFile(string AccountNumber, string Filename, vector <sClient>& vClients)

{
    fstream Myfile;
    Myfile.open(Filename, ios::out);    // overwrite
    string DataLine;

    if (Myfile.is_open())
    {
        for (sClient& C : vClients)
        {
            if (C.AcountNumber != AccountNumber)
            {
                DataLine = ConvertRecordToLine(C);
                Myfile << DataLine << endl;
            }
        }
        Myfile.close();
    }

}
void DeleteClient()
{
    string remove;
    sClient client;

    string AccountNumber = ReadAccountNumber();
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    if (FindClientByAccountNumber(AccountNumber, client))
    {
        PrintClient(client);
        cout << "\n\nAre You Sure you want to Delete This client? y/n  ";
        cin >> remove;

        if (remove == "y" || remove == "Y")
        {
            AddClientsToDataFile(AccountNumber, ClientsFileName, vClients);  // delete by not writng in file
            cout << "\nClient Deleted Succssefully " << endl;
        }
    }
    else
    {
        cout << "\n\nClient with Account Number " << AccountNumber << " Not Found" << endl;
    }

}
void ShowDeleteClientScreen()
{
    if (!CheckAccessPermisison(enMainMenuePermissions::pDeleteClient))
    {
        ShowAccessDeniedScreen();
        return;
    }
    cout << "------------------------------------" << endl;
    cout << "\tDelete Client Screen" << endl;
    cout << "------------------------------------" << endl;

    DeleteClient();

}


void UpdateClient(sClient& client)
{
    cout << "Enter Pincode?  ";
    getline(cin >> ws, client.Pincode);
    cout << "Enter Name?  ";
    getline(cin, client.Name);
    cout << "Enter Phone?  ";
    getline(cin, client.Phone);
    cout << "Enter Account Balance?  ";
    cin >> client.Accountbalance;

}
void UpdateClientInFile(string AccountNumber, string Filename, vector <sClient>& vClients)
{
    fstream Myfile;
    Myfile.open(Filename, ios::out);                 // overwrite
    string DataLine;

    if (Myfile.is_open())
    {
        for (sClient& C : vClients)
        {
            if (C.AcountNumber == AccountNumber)
            {
                UpdateClient(C);
                DataLine = ConvertRecordToLine(C);
                Myfile << DataLine << endl;
            }
            else
            {
                DataLine = ConvertRecordToLine(C);
                Myfile << DataLine << endl;
            }
        }
        Myfile.close();
    }

}
void ShowUpdateClientScreen()
{
    if (!CheckAccessPermisison(enMainMenuePermissions::pUpdateClient))
    {
        ShowAccessDeniedScreen();
        return;
    }

    cout << "------------------------------------" << endl;
    cout << "\tUpdate Client Screen" << endl;
    cout << "------------------------------------" << endl;

    string update;
    sClient client;
    string AccountNumber = ReadAccountNumber();
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    if (FindClientByAccountNumber(AccountNumber, client))
    {
        PrintClient(client);
        cout << "\n\nAre You Sure you Want to Update This client? y/n  ";
        cin >> update;

        if (update == "y" || update == "Y")
        {
            UpdateClientInFile(AccountNumber, ClientsFileName, vClients);
            cout << "\nClient Updated Succssefully " << endl;
        }
    }
    else
    {
        cout << "\n\nClient with Account Number " << AccountNumber << " Not Found" << endl;
    }

}

/// /// /// /// /// /// /// /// /// /// ///  /// First Extention

void PrintClientForBalance(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AcountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Accountbalance;

}
void ShowTotalBalancesScreen()
{
    double TotalBalance = 0;
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\t Balances List (" << vClients.size() << ") Client(s)";
    cout << "\n------------------------------------------------------------------------------------------------------\n" << endl;
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Account Balance";
    cout << "\n\n------------------------------------------------------------------------------------------------------\n" << endl;

    if (vClients.size() == 0)
        cout << "No Clients Available in The System!" << endl;

    else
        for (sClient client : vClients)
        {
            TotalBalance += client.Accountbalance;
            PrintClientForBalance(client);
            cout << endl;
        }
    cout << "\n------------------------------------------------------------------------------------------------------\n" << endl;
    cout << "\t\t\t\t\t    Total Balances = " << TotalBalance << endl;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{

    char answer = 'n';
    cout << "\n\nAre You Sure you Want to perform this Transaction? y/n  ";
    cin >> answer;

    if (answer == 'y' || answer == 'Y')
    {
        for (sClient& C : vClients)
        {
            if (C.AcountNumber == AccountNumber)
            {
                C.Accountbalance += Amount;
                SaveClientsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully.New Balance is:  " << C.Accountbalance;

                return true;
            }
        }
        return false;
    }
}
void ShowWithdrawScreen()
{
    cout << "----------------------------" << endl;
    cout << "\tWithdraw Screen" << endl;
    cout << "----------------------------" << endl;

    sClient Client;
    string AccountNumber = ReadAccountNumber();
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    while (!FindClientByAccountNumber(AccountNumber, Client))
    {
        cout << "Client With [" << AccountNumber << "] does not exists.\n";
        AccountNumber = ReadAccountNumber();
    }

    PrintClient(Client);
    double Amount = 0;
    cout << "Please Enter Withdraw Amount? ";
    cin >> Amount;

    while (Amount > Client.Accountbalance)
    {
        cout << "\nAmount Exceeds The Balance, You Can Withdrw Up To " << Client.Accountbalance << endl;
        cout << "Please Enter Another Amount ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}
void ShowDepositScreen()
{
    cout << "----------------------------" << endl;
    cout << "\tDeposit Screen" << endl;
    cout << "----------------------------" << endl;

    sClient Client;
    string AccountNumber = ReadAccountNumber();
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    while (!FindClientByAccountNumber(AccountNumber, Client))
    {
        cout << "Client With [" << AccountNumber << "] does not exists.\n";
        AccountNumber = ReadAccountNumber();
    }

    PrintClient(Client);
    double Amount = 0;
    cout << "Please Enter Deposit Amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);

}


void GoBackToTranscationsMenue()
{
    cout << "\n\nPress Any Key To Go Back To Transactions Menue... ";
    system("pause > 0");
    ShowTranscationsMenue();

}
short ReadTransactionsMenueOption()
{
    short choice;
    cout << "\nChoose What Do You Want To Do? [1 To 4]? ";
    cin >> choice;
    return choice;
}
void PerformTransactionsMenueOption(enTransactionsMenue TransactionsOptions)
{
    switch (TransactionsOptions)
    {
    case enTransactionsMenue::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTranscationsMenue();
        break;
    }
    case enTransactionsMenue::eWithdraw:
    {
        system("cls");
        ShowWithdrawScreen();
        GoBackToTranscationsMenue();
        break;
    }
    case enTransactionsMenue::eTotalBalances:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTranscationsMenue();
        break;
    }
    case enTransactionsMenue::eMainMenuefromT:
    {
        system("cls");
        ShowMainMenue();
    }
    }
}
void ShowTranscationsMenue()
{
    if (!CheckAccessPermisison(enMainMenuePermissions::pTransactions))
    {
        ShowAccessDeniedScreen();
        GoBackToMainMenue();
        return;
    }
    system("cls");
    cout << "====================================" << endl;
    cout << "\t  Transcations Menue Screen" << endl;
    cout << "====================================" << endl;
    cout << "\t[1] Deposit." << endl;
    cout << "\t[2] Withdraw." << endl;
    cout << "\t[3] Total Balances." << endl;
    cout << "\t[4] Main Menue." << endl;
    cout << "====================================" << endl;
    PerformTransactionsMenueOption((enTransactionsMenue)ReadTransactionsMenueOption());
}

/// /// /// /// /// /// /// /// /// /// ///  ///   Second Extention (----Manage Users----)


sUser ConvertLineUserToRecord(string Line, string Seperator = "#//#")
{
    sUser User;
    vector <string> vUserData;

    vUserData = SplitString(Line, Seperator);

    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);

    return User;
}
vector <sUser> LoadUsersDataFromFile(string Filename)
{
    vector <sUser> vUsers;           // vector of structure  
    fstream MyFile;
    MyFile.open(Filename, ios::in);     // readMode

    if (MyFile.is_open())
    {
        string Line;
        sUser User;

        while (getline(MyFile, Line))
        {
            User = ConvertLineUserToRecord(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return vUsers;
}
void PrintUserRecord(sUser User)
{
    cout << "| " << setw(15) << left << User.UserName;
    cout << "| " << setw(10) << left << User.Password;
    cout << "| " << setw(40) << left << User.Permissions;
}
void ShowUsersListScreen()

{
    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    cout << "\n\t\t\t\t\t Users List (" << vUsers.size() << ") Users(s)";
    cout << "\n------------------------------------------------------------------------------------------------------\n" << endl;
    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n\n------------------------------------------------------------------------------------------------------\n" << endl;

    for (sUser user : vUsers)
    {
        PrintUserRecord(user);
        cout << endl;
    }
    cout << "\n------------------------------------------------------------------------------------------------------\n" << endl;
}

string ConvertRecordToLine(sUser User, string Seperator = "#//#")
{
    string stUserRecord = "";

    stUserRecord += User.UserName + Seperator;
    stUserRecord += User.Password + Seperator;
    stUserRecord += to_string(User.Permissions);

    return stUserRecord;
}
bool CheckUserByName(string UserName, string Filename)
{
    vector <sUser> vUsers = LoadUsersDataFromFile(Filename);

    for (sUser& user : vUsers)
    {
        if (user.UserName == UserName)
        {
            return true;
            break;
        }
    }
    return false;
}
int ReadPermissions()
{
    int permissions = 0;
    char answer;

    cout << "Do you Want to Give Full access? y/n?  ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        return -1;
    }

    else
    {
        cout << "\nDo You Want to Give Access To: ";

        cout << "\nShow Clients List? y/n ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {
            permissions += enMainMenuePermissions::pListClients;    // readable than add 1
        }

        cout << "\nAdd New Client? y/n ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {
            permissions += enMainMenuePermissions::pAddNewClient;   // Readable than add 2
        }

        cout << "\nDelete Client? y/n ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {
            permissions += enMainMenuePermissions::pDeleteClient;
        }

        cout << "\nUpdate Client? y/n ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {
            permissions += enMainMenuePermissions::pUpdateClient;
        }

        cout << "\nFind Client? y/n ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {
            permissions += enMainMenuePermissions::pFindClient;
        }

        cout << "\nTransactions? y/n ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {
            permissions += enMainMenuePermissions::pTransactions;
        }

        cout << "\nManage Users? y/n ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {
            permissions += enMainMenuePermissions::pManageUsers;
        }

    }

    return permissions;
}
sUser ReadNewUser()
{
    sUser User;

    cout << "\nEnter UserName? ";
    getline(cin >> ws, User.UserName);

    while (CheckUserByName(User.UserName, UsersFileName))
    {
        cout << "\nUser With [" << User.UserName << "] Already exists, Enter another Username?  ";
        getline(cin >> ws, User.UserName);
    }
    cout << "Enter Password? ";
    getline(cin >> ws, User.Password);

    cout << "\nEnter Permissions? ";
    User.Permissions = ReadPermissions();

    return User;
}
void AddNewUser()
{
    sUser User;
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertRecordToLine(User));
}
void AddNewUsers()
{
    char AddMore = 'Y';
    do
    {
        cout << "Adding New User:\n";
        AddNewUser();
        cout << "\nUser Added Successfully, do you want to add more clients ? Y / N ? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');

}
void ShowAddNewUserScreen()
{
    system("cls");
    cout << "------------------------------------------" << endl;
    cout << "\t Add New User Screen" << endl;
    cout << "------------------------------------------" << endl;
    AddNewUsers();

}

string ReadUserName()
{
    string username;
    cout << "Please Enter UserName?  ";
    cin >> username;
    return username;

}
bool FindUserByUserName(string UserName, sUser& User)
{
    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (sUser& u : vUsers)
    {
        if (u.UserName == UserName)
        {
            User = u;
            return true;
        }

    }
    return false;
}
bool FindUserByUsernameAndPassword(string UserName, string Password, sUser& User)
{
    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (sUser& U : vUsers)
    {
        if (U.UserName == UserName && U.Password == Password)
        {
            User = U;
            return true;
        }

    }
    return false;
}
void PrintUser(sUser User)
{
    cout << "\n\nThe following are the User informations:\n";
    cout << "--------------------------------------------";
    cout << "\nAccount Number :  " << User.UserName;
    cout << "\nPassword       :  " << User.Password;
    cout << "\npermissions    :  " << User.Permissions;
    cout << "\n--------------------------------------------\n\n" << endl;
}
void ShowFindUserScreen()
{
    cout << "------------------------------------" << endl;
    cout << "\tFind User Screen" << endl;
    cout << "------------------------------------" << endl;

    sUser User;
    string UserName = ReadUserName();

    if (FindUserByUserName(UserName, User))
    {
        PrintUser(User);
    }
    else
    {
        cout << "\n\nUser with UserName " << UserName << " Is Not Found!\n" << endl;
    }
}

void AddUsersToDataFile(string UserName, string Filename, vector <sUser>& vUsers)

{
    fstream Myfile;
    Myfile.open(Filename, ios::out);    // overwrite
    string DataLine;

    if (Myfile.is_open())
    {
        for (sUser& U : vUsers)
        {
            if (U.UserName != UserName)
            {
                DataLine = ConvertRecordToLine(U);
                Myfile << DataLine << endl;
            }
        }
        Myfile.close();
    }

}
void DeleteUser()
{
    string remove;
    sUser User;
    string UserName = ReadUserName();

    if (UserName == "Admin")
    {
        cout << "\n\n You Can Not delete This User" << endl;
        return;
    }


    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    if (FindUserByUserName(UserName, User))
    {
        PrintUser(User);
        cout << "\n\nAre You Sure you want to Delete This User? y/n  ";
        cin >> remove;

        if (remove == "y" || remove == "Y")
        {
            AddUsersToDataFile(UserName, UsersFileName, vUsers);  // delete by not writng in file
            cout << "\nUser Deleted Succssefully " << endl;
        }
    }
    else
    {
        cout << "\n\nUser with Account Number " << UserName << " Not Found" << endl;
    }

}
void ShowDeleteUserScreen()
{
    cout << "------------------------------------" << endl;
    cout << "\tDelete User Screen" << endl;
    cout << "------------------------------------" << endl;

    DeleteUser();

}

void UpdateUser(sUser& user)
{
    cout << "Enter Password?  ";
    cin >> user.Password;

    cout << "Enter Permissions?  ";
    user.Permissions = ReadPermissions();

}
void UpdateUserInFile(string UserName, string Filename, vector <sUser>& vUsers)
{
    fstream Myfile;
    Myfile.open(Filename, ios::out);                 // overwrite
    string DataLine;

    if (Myfile.is_open())
    {
        for (sUser& U : vUsers)
        {
            if (U.UserName == UserName)
            {
                UpdateUser(U);
                DataLine = ConvertRecordToLine(U);
                Myfile << DataLine << endl;
            }
            else
            {
                DataLine = ConvertRecordToLine(U);
                Myfile << DataLine << endl;
            }
        }
        Myfile.close();
    }

}
void ShowUpdateUserScreen()
{
    cout << "------------------------------------" << endl;
    cout << "\tUpdate User Screen" << endl;
    cout << "------------------------------------" << endl;

    string update;
    sUser User;
    string UserName = ReadUserName();
    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    if (FindUserByUserName(UserName, User))
    {
        PrintUser(User);
        cout << "\n\nAre You Sure you Want to Update This User? y/n  ";
        cin >> update;

        if (update == "y" || update == "Y")
        {
            UpdateUserInFile(UserName, UsersFileName, vUsers);
            cout << "\nUser Updated Succssefully " << endl;
        }
    }
    else
    {
        cout << "\n\nUser with UserName " << UserName << " Not Found" << endl;
    }

}


void GoBackToManageUsersMenue()
{
    cout << "\n\nPress Any Key To Go Back To Manage Users Menue... ";
    system("pause > 0");
    ShowManageUsersMenue();

}
short ReadManageUsersOption()
{
    short choice;
    cout << "\nChoose What Do You Want To Do? [1 To 6]? ";
    cin >> choice;
    return choice;
}
void PerformManageUsersMenueOption(enManageUsersMenue ManageUsersOptions)
{
    switch (ManageUsersOptions)
    {

    case enManageUsersMenue::eListUsers:
    {
        system("cls");
        ShowUsersListScreen();
        GoBackToManageUsersMenue();
        break;
    }
    case enManageUsersMenue::eAddUser:
    {
        system("cls");
        ShowAddNewUserScreen();
        GoBackToManageUsersMenue();
        break;
    }
    case enManageUsersMenue::eDeleteUser:
    {
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenue();
        break;
    }
    case enManageUsersMenue::eUpdateUser:
    {
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenue::eFindUser:
    {
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenue::eMainMenuefromU:
    {
        system("cls");
        ShowMainMenue();
        break;
    }

    }
}

void ShowManageUsersMenue()
{
    if (!CheckAccessPermisison(enMainMenuePermissions::pManageUsers))
    {
        ShowAccessDeniedScreen();
        GoBackToMainMenue();
        return;
    }
    system("cls");
    cout << "====================================" << endl;
    cout << "\t  Manage Users Menue Screen" << endl;
    cout << "====================================" << endl;
    cout << "\t[1] List Users." << endl;
    cout << "\t[2] Add New User." << endl;
    cout << "\t[3] Delete User." << endl;
    cout << "\t[4] Update User." << endl;
    cout << "\t[5] Find User." << endl;
    cout << "\t[6] Main Menue." << endl;
    cout << "====================================" << endl;
    PerformManageUsersMenueOption((enManageUsersMenue)ReadManageUsersOption());
}


/// /// /// /// /// /// /// /// /// /// ///  ///  Main Program

bool LoadUserInfo(string Username, string Password)
{
    if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
        return true;
    else
        return false;

}
void Login()
{
    bool LoginFailed = false;

    string Username, Password;

    do
    {
        system("cls");
        cout << "-----------------------------------------" << endl;
        cout << "\tLogin Screen " << endl;
        cout << "-----------------------------------------" << endl;

        if (LoginFailed)
        {
            cout << "\nInvalid UserName/Password";
        }

        cout << "\nEnter Username? ";
        getline(cin >> ws, Username);
        cout << "Enter Password? ";
        cin >> Password;

        LoginFailed = !LoadUserInfo(Username, Password);

    } while (LoginFailed);

    ShowMainMenue();
}

void GoBackToMainMenue()
{
    cout << "Press Any Key To Go Back To Main Menue... ";
    system("pause > 0");
    ShowMainMenue();

}
void ShowAccessDeniedScreen()
{
    cout << "-----------------------------------------" << endl;
    cout << "Access Denied,"
        << "\nYou don't Have Permission To Do This,"
        << "\nPlease contact Your Admin," << endl;
    cout << "-----------------------------------------" << endl;

}
short ReadMainMenueOption()
{
    short option;
    cout << "\nChoose What Do You Want To Do? [1 To 8]? ";
    cin >> option;
    return option;
}
void PerformMainMenueOption(enMainMenueOptions MainMenueOptions)
{
    switch (MainMenueOptions)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
    {
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eDeleteClient:
    {
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eUpdateClient:
    {
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eFindClient:
    {
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;;
    }
    case enMainMenueOptions::eTransactions:
    {
        system("cls");
        ShowTranscationsMenue();
        break;
    }
    case enMainMenueOptions::eManageUsers:
    {
        system("cls");
        ShowManageUsersMenue();
        break;
    }

    case enMainMenueOptions::eExit:
    {
        system("cls");
        Login();
        break;
    }
    }
}
void ShowMainMenue()
{
    system("cls");

    cout << "===============================================" << endl;
    cout << "\t\tMain Menue Screen" << endl;
    cout << "===============================================" << endl;
    cout << "\t [1] Show Client List." << endl;
    cout << "\t [2] Show Add New Client." << endl;
    cout << "\t [3] Delete Client." << endl;
    cout << "\t [4] Update Client Info." << endl;
    cout << "\t [5] Find Client." << endl;
    cout << "\t [6] Transcations." << endl;
    cout << "\t [7] Manage Users." << endl;
    cout << "\t [8] Logout." << endl;
    cout << "===============================================" << endl;

    PerformMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}


int main()
{
    Login();
    system("pause>0");
    return 0;
}

