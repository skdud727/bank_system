#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class Account {
private:
   string name;
   int password;
   int account_number;
   int card_number;
   int balance;
   string bankname;
public:
   Account();
   Account(string name, int password, int account_number, int card_number, int balance, string bankname);
   string get_name();
   int get_password();
   int get_account_number();
   int get_card_number();
   int get_balance();
   string get_bank_name();
   int change_balance(int money);
};

class Bank {
private:
   vector<Account*> account_stl; // Vector type account information
   int bank_balance;
   string bankname;
public:
   string get_bankname();
   Bank(vector<Account*>, int, string);
   vector<Account*> get_account_stl();
   int get_bankbalance();
   int change_balance(int money);
};

class ATM{
public:
    virtual bool deposit()=0;
    virtual bool withdrawl()=0;
    virtual bool transfer()=0;
  };
  
class ATM_unilingual:public ATM{
private:
   int balance;
   int limit_check = 30;
   int limit_cash = 50;
   int error_count = 0;
   int admin_card;
   string trsac_info;
   int transaction_id;
   vector<Account*> Account_array[2];
   Account* newacc;
public:
   Bank* bank;
   int serialnumber;
   string atm_type;
   //int select_language;
   int max_transaction = 1000000;
   ATM_unilingual(Bank* primary_bank, vector<Account*> *Account_arrays, string type, int admin, int atm_balance);
   int check_account();     // 0 (admin) 1 (존재) 2(존재X)
   bool check_password();   //
   bool check_bank();       // true (사용 가능) false ( 사용 불가능 )
   int change_balance(int money);
   int get_atm_balance();
   int get_admin_card();
   bool deposit();
   bool withdrawl();
   bool transfer();
   bool stop(bool check);
   void use();
};

class ATM_bilingual:public ATM{
private:
   int balance;
   int limit_check = 30;
   int limit_cash = 50;
   int error_count = 0;
   int admin_card;
   string trsac_info;
   int transaction_id;
   vector<Account*> Account_array[2];
   Account* newacc;
public:
   Bank* bank;
   int serialnumber;
   string atm_type;
   int select_language;
   int max_transaction = 1000000;
   ATM_bilingual(Bank* primary_bank, vector<Account*> *Account_arrays, string type, int admin, int atm_balance);
   int check_account();     // 0 (admin) 1 (존재) 2(존재X)
   bool check_password();   //
   bool check_bank();       // true (사용 가능) false ( 사용 불가능 )
   int change_balance(int money);
   int get_atm_balance();
   int get_admin_card();
   bool deposit();
   bool withdrawl();
   bool transfer();
   bool stop(bool check);
   void use();
};
//Account
Account::Account(string name, int password, int account_number, int card_number, int balance, string bankname) {
   this->name = name;
   this->password = password;
   this->account_number = account_number;
   this->card_number = card_number;
   this->balance = balance;
   this->bankname = bankname;
}
Account::Account() {
}

int Account::change_balance(int money) {
   balance = balance + money;
   return balance;
}

string Account::get_name() {
   return name;
}
int Account::get_password() {
   return password;
}
int Account::get_account_number() {
   return account_number;
}
int Account::get_card_number() {
   return card_number;
}
int Account::get_balance() {
   return balance;
}
string Account::get_bank_name() {
   return bankname;
}

//Bank
Bank::Bank(vector<Account*> a, int b, string c) {
   for (int i = 0; i < a.size(); i++)
   {
      Account* a1 = a.at(i);
      this->account_stl.push_back(a1);
   }
   this->bank_balance = b;
   this->bankname = c;
}
vector<Account*> Bank::get_account_stl() {
   return account_stl;
}
int Bank::get_bankbalance() {
   return bank_balance;
}
string Bank::get_bankname() {
   return bankname;
}
int Bank::change_balance(int money) {
   bank_balance = bank_balance + money;
   return 0;
}

// ATM_unilingual
ATM_unilingual::ATM_unilingual(Bank* primary_bank1, vector<Account*> *Account_arrays, string type, int admin, int atm_balance){
    for( int i = 0; i < 2; i++)
    {
        Account_array[i] = Account_arrays[i];
    }
    bank = primary_bank1;
    atm_type = type;
    admin_card = admin;
    balance = atm_balance;
    serialnumber = random();
}

int ATM_unilingual::check_account(){
    int input_cardnumber;
    char select;
    cout << "Write the card number" << endl;
    cin >> input_cardnumber;
    if (input_cardnumber == admin_card) {
        cout << "Open Transaction history Y/N" << endl;
        cin >> select;
        if(select == 'Y'){
           ifstream readFile;
           readFile.open("transaction_info.txt");
            if (readFile.is_open())
            {
               while (!readFile.eof())
                  {
                     string str;
                     getline(readFile, str);
                     cout << str << endl;    //지금은 읽은 문자열 바로 출력.
                  }
               readFile.close();    //파일 닫아줍니다.
            }
        }
        else if(select == 'N'){
            cout << "exit" << endl;
        }
        else{
            cout << "invalid selection(select between Y/N)" << endl;
        }
        return 0;
    }
    for( int i = 0; i < 2; i++)
    {
        vector<Account*> vector_account = Account_array[i];
        for (int j = 0; j < 2 ; j++)
        {
            if (vector_account.at(j)->get_card_number() == input_cardnumber)
            {
                newacc = vector_account.at(j);
                return 1;
            }
        }
   }
   cout << "invalid card" << endl;
   return 2;
}
bool ATM_unilingual::check_password(){
    int input_password;
    while( error_count < 3){
        cout << "Write password" << endl;
        cin >> input_password;
        if (newacc->get_password() == input_password) {
            cout << "valid password" << endl;
            return true;
        }
        else {
            error_count = error_count + 1;
            cout << "invalid password" << endl;
        }
   }
   return false;
}
bool ATM_unilingual::check_bank(){
    if (newacc->get_bank_name() == bank->get_bankname()) {
        cout << "main bank" << endl;
        return true;
    }
    else {
        if (atm_type == "Single") {
            cout << "invalid atm use" << endl;
            return false;
        }
        else {
            return true;
        }
   }
}
int ATM_unilingual::change_balance(int money){
    balance = balance + money;
    return balance;
}
int ATM_unilingual::get_atm_balance(){
    return balance;
}
int ATM_unilingual::get_admin_card(){
    return admin_card;
}
bool ATM_unilingual::deposit(){
    string money_type;
    int money;
    int money_number;
    bool returnname = true;
    cout << "What type of money are you depoisting? \nHow much money are you depositing? \nHow many cash or checks are you depositing?" << endl;
    cin >> money_type;
    cin >> money;
    cin >> money_number;
    int transition_fee = 0;
    if (bank->get_bankname() == newacc->get_bank_name()) {
       transition_fee = 0;
    }
    else if (atm_type == "single") {
        cout << "This atm is single." << endl;
        returnname = false;
    }
    else {
        transition_fee = 500;
    }
    if( returnname = true)
    {
        if (money < transition_fee)
        {
        cout << "Money is too law." << endl;
        returnname = false;
        }
    }
    if( returnname = true)
    {
        if (money_type == "check"){
            if (money_number > limit_check)
            {
                cout << "There are too many checks" << endl;
                returnname = false;
            }
            else
            {
                newacc->change_balance(money-transition_fee);
            }
        }
        else{
            if (money_number > limit_cash)
            {
                cout << "There are too many cashs" << endl;
                returnname = false;
            }
            else
            {
                change_balance(money);//For ATM
                newacc->change_balance(money-transition_fee);
            }
        }
    }
    ofstream writeFile;
    if(transaction_id == 0){
        writeFile.open("transaction_info.txt",ios::out);
    }
    else{
        writeFile.open("transaction_info.txt",ios::app);
    }
    transaction_id++;
    trsac_info = "transsaction id : " + to_string(transaction_id) + ", card number : " + to_string(newacc -> get_card_number()) + ", Transaction Type : deposit, Amount : " + to_string(money) + '\n';
    
    writeFile.write(trsac_info.c_str(), trsac_info.size());
    writeFile.close();
    return returnname;
}

bool ATM_unilingual::withdrawl(){
    int money;
    bool returnname = true;
    cout << "How much money to withdrawl?" << endl;
    cin >> money;
    int transition_fee = 0;
    if (bank->get_bankname() == newacc->get_bank_name()) {
        transition_fee = 500;
    }
    else if (atm_type == "single") {
        cout << "This atm is single." << endl;
        returnname = false;
    }
    else {
        transition_fee = 1000;
    }
    if(returnname = true){
        if (money < transition_fee)
        {
            cout << "Money is too law." << endl;
            returnname = false;
        }
    }
    if(returnname = true){
        if (balance > money && newacc->get_balance() > money + transition_fee && max_transaction > money)
        {
            newacc->change_balance(-(money + transition_fee));
            change_balance(-money);//For ATM
        }
        else
        {
            cout << "error" << endl;
            returnname = false;
        }
    }
    ofstream writeFile;
    if(transaction_id == 0){
        writeFile.open("transaction_info.txt",ios::out);
    }
    else{
        writeFile.open("transaction_info.txt",ios::app);
    }
    transaction_id++;

    trsac_info = "transsaction id : " + to_string(transaction_id) + ", card number : " + to_string(newacc -> get_card_number()) + ", Transaction Type : withdrawl, Amount : " + to_string(money) + '\n';
    
    writeFile.write(trsac_info.c_str(), trsac_info.size());
    writeFile.close();
    
    return returnname;
}
bool ATM_unilingual::transfer(){
    Account* reciacc;
    bool returnname = true;
    int select_1;
    int transfer_money;
    int Account_Number;
    int Cash_number;
    char Check_transfer;
    string Bank_name;
    int Transition_fee = 0;
    cout << "1. Transfer from your account\n 2. Transfer with cash" << endl;
    cin >> select_1;
    cout << "How much money you will transfer?\n" << endl;
    cin >> transfer_money;
    cout << "Account Number of recipient" << endl;
    cin >> Account_Number;
    cout << "Bank Name of recipient" << endl;
    cin >> Bank_name;

    if(select_1 == 1){
        cout << "Check if Bank account and Amount of Money is right\n Bank name:"<<Bank_name<< "\nBank account:" << Account_Number <<"\nMoney:"<< transfer_money<<endl;
        cout << "Y/N" << endl;
        cin >> Check_transfer;
        if (Check_transfer == 'Y'){
            newacc ->change_balance(-transfer_money);
        }
        else{
            returnname = false;
        }
    }
    else if(select_1 == 2){
        cout << "How many number of cash are you transfering?" << endl;
        cin >> Cash_number;
        if( Cash_number > 50){
            cout << "Cash number error" << endl;
            returnname = false;
        }
        else{
            cout << "Check if Bank account and Amount of Money is right \n Bank name:"<<Bank_name<< "\nBank account:" << Account_Number <<"\nMoney:"<< transfer_money<<endl;
            cout << "Y/N" << endl;
            cin >> Check_transfer;
            if (Check_transfer == 'Y'){
            change_balance(transfer_money);
            }
            else{
                returnname = false;
            }
      }
    }
    else{
        cout << "key error" << endl;
        returnname = false;
    }
    for( int i = 0; i < 2; i++)
    {
        vector<Account*> vector_account = Account_array[i];
        for (int j = 0; j < 2; j++)
        {
            if (vector_account.at(j)->get_bank_name() == Bank_name && vector_account.at(j)->get_account_number() == Account_Number)
            {
                reciacc = vector_account.at(j);
            }
        }
    }
    reciacc -> change_balance(transfer_money);
    if(newacc -> get_bank_name() == reciacc ->get_bank_name()){
        if(check_bank()){
            Transition_fee = 1500;
        }
        else{
        Transition_fee = 2500;
        }
    }
    else{
        Transition_fee = 2000;
    }
    newacc ->change_balance(-Transition_fee);

    ofstream writeFile;
    if(transaction_id == 0){
        writeFile.open("transaction_info.txt",ios::out);
    }
    else{
        writeFile.open("transaction_info.txt",ios::app);
    }
    transaction_id++;

    trsac_info = "transsaction id : " + to_string(transaction_id) + ", card number : " + to_string(newacc -> get_card_number()) + ", Transaction Type : withdrawl, Amount : " + to_string(transfer_money) + " recipient account number : " + to_string(reciacc->get_account_number()) + '\n';
    
    writeFile.write(trsac_info.c_str(), trsac_info.size());
    writeFile.close();
    
    return returnname;
}
bool ATM_unilingual::stop(bool check)
{
    if(check) {
        return true;
    }
    else {
        exit(-1);
    }
}
void ATM_unilingual::use()
{
    /*
    cout << "Chose language : (0) Korea (1) English" << endl;
    cin >> select_language;
    */
    int select;
    bool check;
    cout << "Welcome" << endl;
    cout << "If you want to stop the session press Ctrl+C" << endl;
    cout << "To start, please inset your debit card" << endl;
    int a = check_account(); // 0  관리자  1 정상  2 비정상
    if (a == 1 ) { check = true; }
    else{ check = false; }
    stop(check);
    check = check_bank();
    stop(check);
    check = check_password();
    stop(check);
    while (true)
    {
        cout << " Select: (1)Deposit (2)Withdrawl (3) Transfer" << endl;
        cin >> select;
        if (select == 1)
        {
            check = deposit();
            stop(check);
        }
        else if (select == 2)
        {
            check = withdrawl();
            stop(check);
        }
        else
        {
            check = transfer();
            stop(check);
        }
    }
}

//ATM bilingual
// ATM
ATM_bilingual::ATM_bilingual(Bank* primary_bank1, vector<Account*> *Account_arrays, string type, int admin, int atm_balance){
    for( int i = 0; i < 2; i++)
    {
        Account_array[i] = Account_arrays[i];
    }
    bank = primary_bank1;
    atm_type = type;
    admin_card = admin;
    balance = atm_balance;
    serialnumber = random();
}

int ATM_bilingual::check_account(){
    int input_cardnumber;
    char select;
    if(select_language){
    cout << "Write the card number" << endl;
    }
    else{
        cout<<"카드 번호를 입력하세요"<<endl;
    }
    cin >> input_cardnumber;
    if (input_cardnumber == admin_card) {
        if(select_language){
        cout << "Open Transaction history Y/N" << endl;
        }
        else{
        cout <<"거래 내역을 여시겠습니까? Y/N"<<endl;
        }        cin >> select;
        if(select == 'Y'){
           ifstream readFile;
           readFile.open("transaction_info.txt");
            if (readFile.is_open())
            {
               while (!readFile.eof())
                  {
                     string str;
                     getline(readFile, str);
                     cout << str << endl;    //지금은 읽은 문자열 바로 출력.
                  }
               readFile.close();    //파일 닫아줍니다.
            }
        }
        else if(select == 'N'){
            cout << "exit" << endl;
        }
        else{
            if(select_language){
            cout << "invalid selection(select between Y/N)" << endl;
            }
            else{
                cout<<"허용되지 않은 선택입니다."<<endl;
            }
        }
        return 0;
    }
    for( int i = 0; i < 2; i++)
    {
        vector<Account*> vector_account = Account_array[i];
        for (int j = 0; j < 2 ; j++)
        {
            if (vector_account.at(j)->get_card_number() == input_cardnumber)
            {
                newacc = vector_account.at(j);
                return 1;
            }
        }
   }
   if(select_language){
     cout << "invalid card" << endl;
   }
   else{
       cout<<"허용되지 않은 카드입니다."<<endl;
   }
   return 2;
}
bool ATM_bilingual::check_password(){
    int input_password;
    while( error_count < 3){
        if(select_language){
        cout << "Write password" << endl;
        }
        else{
            cout<<"비밀번호를 입력하세요"<<endl;
        }
        cin >> input_password;
        if (newacc->get_password() == input_password) {
             if(select_language){
          cout << "valid password" << endl;
      }
      else{
          cout<<"옳은 비밀번호입니다."<<endl;
      }
            return true;
        }
        else {
            error_count = error_count + 1;
           if(select_language){
         cout << "invalid password" << endl;
     }
      else{
          cout<<"옳지 않은 비밀번호입니다."<<endl;
      }
        }
   }
   return false;
}
bool ATM_bilingual::check_bank(){
    if (newacc->get_bank_name() == bank->get_bankname()) {
        if(select_language){
          //cout << "main bank" << endl;
      }
      else{
          //cout<<"주거래 은행입니다."<<endl;
      }
        return true;
    }
    else {
        if (atm_type == "Single") {
            if(select_language){
            cout << "invalid atm use" << endl;
         }
         else{
             cout<<"허용되지 않은 atm 사용입니다."<<endl;
         }
            return false;
        }
        else {
            return true;
        }
   }
}
int ATM_bilingual::change_balance(int money){
    balance = balance + money;
    return balance;
}
int ATM_bilingual::get_atm_balance(){
    return balance;
}
int ATM_bilingual::get_admin_card(){
    return admin_card;
}
bool ATM_bilingual::deposit(){
    string money_type;
    int money;
    int money_number;
    bool returnname = true;
    if(select_language){
    cout << "What are you depoisting?cash/check \n How much money are you depositing? \n How many cash or checks are you depositing?" << endl;
   }
   else{
       cout << "어떤 화폐로 입금하나요? 현금(cash)/수표(check) \n 얼만큼 돈을 입금하나요 \n 몇장의 화폐를 입금하시나요?" << endl;
   }
    cin >> money_type;
    cin >> money;
    cin >> money_number;
    int transition_fee = 0;
    if (bank->get_bankname() == newacc->get_bank_name()) {
       transition_fee = 0;
    }
    else if (atm_type == "single") {
        if(select_language){
          cout<< "unallowed transaction" <<endl;
      }
      else{
          cout <<"허용되지 않은 거래입니다."<< endl;
      }
        returnname = false;
    }
    else {
        transition_fee = 500;
    }
    if( returnname == true)
    {
        if (money < transition_fee)
        {
            returnname = false;
            if(select_language){
                cout << "not enough money" << endl;
            }
            else{
                cout<<"돈이 충분하지 않습니다."<<endl;
            }
        }
    }
    if( returnname == true)
    {
        if (money_type == "check"){
            if (money_number > limit_check)
            {
                if(select_language){
         cout << "limit error" << endl;
         }
         else{
             cout << "화폐 수 초과 에러" << endl;
         }
                returnname = false;
            }
            else
            {
                newacc->change_balance(money-transition_fee);
            }
        }
        else{
            if (money_number > limit_cash)
            {
                if(select_language){
         cout << "limit error" << endl;
         }
         else{
             cout << "화폐 수 초과 에러" << endl;
         }
                returnname = false;
            }
            else
            {
                change_balance(money);//For ATM
                newacc->change_balance(money-transition_fee);
            }
        }
    }
    ofstream writeFile;
    if(transaction_id == 0){
        writeFile.open("transaction_info.txt",ios::out);
    }
    else{
        writeFile.open("transaction_info.txt",ios::app);
    }
    transaction_id++;
    trsac_info = "transsaction id : " + to_string(transaction_id) + ", card number : " + to_string(newacc -> get_card_number()) + ", Transaction Type : deposit, Amount : " + to_string(money) + '\n';
    
    writeFile.write(trsac_info.c_str(), trsac_info.size());
    writeFile.close();
    return returnname;
}

bool ATM_bilingual::withdrawl(){
    int money;
    bool returnname = true;
    if(select_language){
   cout << "How much money to withdrawl?" << endl;
   }
   else{
       cout<<"얼마나 돈을 출금하시나요?"<<endl;
   }
    cin >> money;
    int transition_fee = 0;
    if (bank->get_bankname() == newacc->get_bank_name()) {
        transition_fee = 500;
    }
    else if (atm_type == "single") {
         if(select_language){
            cout << "not allowed transcation error" << endl;
      }
      else{
          cout<<"허용되지 않는 거래입니다."<<endl;
      }
        returnname = false;
    }
    else {
        transition_fee = 1000;
    }
    if(returnname = true){
        if (money < transition_fee)
        {
            if(select_language){
      cout << "not enough money" << endl;
      }
      else{
          cout<<"돈이 충분하지 않습니다."<<endl;
      }
            returnname = false;
        }
    }
    if(returnname = true){
        if (balance > money && newacc->get_balance() > money + transition_fee && max_transaction > money)
        {
            newacc->change_balance(-(money + transition_fee));
            change_balance(-money);//For ATM
        }
        else
        {
            if(select_language){
      cout << "error" << endl;
      }
      else{
          cout<<"허용되지 않은 거래입니다."<<endl;
      }

            returnname = false;
        }
    }
    ofstream writeFile;
    if(transaction_id == 0){
        writeFile.open("transaction_info.txt",ios::out);
    }
    else{
        writeFile.open("transaction_info.txt",ios::app);
    }
    transaction_id++;

    trsac_info = "transsaction id : " + to_string(transaction_id) + ", card number : " + to_string(newacc -> get_card_number()) + ", Transaction Type : withdrawl, Amount : " + to_string(money) + '\n';
    
    writeFile.write(trsac_info.c_str(), trsac_info.size());
    writeFile.close();
    
    return returnname;
}

bool ATM_bilingual::transfer(){
    Account* reciacc;
    bool returnname = true;
    int select_1;
    int transfer_money;
    int Account_Number;
    int Cash_number;
    char Check_transfer;
    string Bank_name;
    int Transition_fee = 0;
if(select_language){
       cout << "1. Transfer from your account\n 2. Transfer with cash" << endl;
   }
   else{
       cout << "1. 계좌로부터 송금\n 2. 현금으로 송금" << endl;
   }
    cin >> select_1;
if(select_language){
       cout << "How much money you will transfer?\n" << endl;
   }
   else{
       cout << "얼마나 송금하실 건가요?\n" << endl;
   }
   cin >> transfer_money;
if(select_language){
    cout << "Account Number of recipient" << endl;
   }
   else{
       cout<<"송금하려는 상대의 계좌번호를 입력하세요?"<<endl;
   }

    cin >> Account_Number;
if(select_language){
       cout << "Bank Name of recipient" << endl;
   }
   else{
       cout<<"송금 상대의 은행을 입력하세요"<<endl;
   }
    cin >> Bank_name;

    if(select_1 == 1){
        if(select_language){
      cout << "Check if Bank account and Amount of Money is right\n Bank name:"<<Bank_name<< "\nBank account:" << Account_Number <<"\nMoney:"<< transfer_money<<endl;
      cout << "Y/N" << endl;
      }
      else{
          cout << "은행과 거래하려는 돈이 알맞은지 확인하세요\n 은행 이름:"<<Bank_name<< "\n 계좌번호:"<< Account_Number <<"\n 거래 금액:"<< transfer_money<<endl;
      cout << "Y/N" << endl;
      }
        cin >> Check_transfer;
        if (Check_transfer == 'Y'){
            newacc ->change_balance(-transfer_money);
        }
        else{
            returnname = false;
        }
    }
    else if(select_1 == 2){
        if(select_language){
      cout << "How many number of cash are you transfering?" << endl;
      }
      else{
          cout<<“몇장의 현금을 송금할 것입니까?"<<endl;
      }
        cin >> Cash_number;
        if( Cash_number > 50){
        if(select_language){
         cout << "Cash number error" << endl;
         }
         else{
             cout<<"현금 수 오류"<<endl;
         }
            returnname = false;
        }
        else{
            if(select_language){
      cout << "Check if Bank account and Amount of Money is right\n Bank name:"<<Bank_name<< "\nBank account:" << Account_Number <<"\nMoney:"<< transfer_money<<endl;
      cout << "Y/N" << endl;
      }
      else{
          cout << "은행과 거래하려는 돈이 알맞은지 확인하세요\n 은행 이름:"<<Bank_name<< "\n 계좌번호:"<< Account_Number <<"\n 거래 금액:"<< transfer_money<<endl;
      cout << "Y/N" << endl;
      }
            cin >> Check_transfer;
            if (Check_transfer == 'Y'){
            change_balance(transfer_money);
            }
            else{
                returnname = false;
            }
      }
    }
    else{
        if(select_language){
         cout << "key error" << endl;
       }
      else{
          cout<<"입력 오류"<<endl;
      }
        returnname = false;
    }
    for( int i = 0; i < 2; i++)
    {
        vector<Account*> vector_account = Account_array[i];
        for (int j = 0; j < 2; j++)
        {
            if (vector_account.at(j)->get_bank_name() == Bank_name && vector_account.at(j)->get_account_number() == Account_Number)
            {
                reciacc = vector_account.at(j);
            }
        }
    }
    reciacc -> change_balance(transfer_money);
    if(newacc -> get_bank_name() == reciacc ->get_bank_name()){
        if(check_bank()){
            Transition_fee = 1500;
        }
        else{
        Transition_fee = 2500;
        }
    }
    else{
        Transition_fee = 2000;
    }
    newacc ->change_balance(-Transition_fee);

    ofstream writeFile;
    if(transaction_id == 0){
        writeFile.open("transaction_info.txt",ios::out);
    }
    else{
        writeFile.open("transaction_info.txt",ios::app);
    }
    transaction_id++;

    trsac_info = "transsaction id : " + to_string(transaction_id) + ", card number : " + to_string(newacc -> get_card_number()) + ", Transaction Type : withdrawl, Amount : " + to_string(transfer_money) + " recipient account number : " + to_string(reciacc->get_account_number()) + '\n';
    
    writeFile.write(trsac_info.c_str(), trsac_info.size());
    writeFile.close();
    
    return returnname;
}
bool ATM_bilingual::stop(bool check)
{
    if(check) {
        return true;
    }
    else {
        exit(-1);
    }
}
void ATM_bilingual::use()
{
    cout << "Choose language : (0) Korea (1) English" << endl;
    cin >> select_language;
    int select;
    bool check;
    if(select_language){
    cout << "Welcome" << endl;
    cout << "If you want to stop the session press Ctrl+C" << endl;
    cout << "To start, please inset your debit card" << endl;
    }
    else{
    cout << "환영합니다" << endl;
    cout << "세션을 중단하고 싶다면 Ctrl+C를 누르세요" << endl;
    cout << "체크카드를 넣어서 세션을 시작하세요" << endl;
    }
    int a = check_account(); // 0  관리자  1 정상  2 비정상
    if (a == 1 ) { check = true; }
    else{ check = false; }
    stop(check);
    check = check_bank();
    stop(check);
    check = check_password();
    stop(check);
    while (true)
    {
       if(select_language){
     cout << " Select: (1)Deposit (2)Withdrawl (3) Transfer" << endl;
}
    else{
    cout<<“선택하세요:(1) 입금 (2) 출금 (3) 송금”<<endl;
}
        cin >> select;
        if (select == 1)
        {
            cout << newacc->get_balance() << endl;
            check = deposit();
            cout << newacc->get_balance() << endl;
            stop(check);
        }
        else if (select == 2)
        {
            check = withdrawl();
            stop(check);
        }
        else
        {
            check = transfer();
            stop(check);
        }
    }
}


int main()
{
   //Creating two Daegu account
   Account c1 = Account("Tom", 1, 1, 1, 100000, "Daegu");
   Account c2 = Account("Alex", 2, 2, 2, 50000, "Daegu");
   vector<Account*> Daegu_account = { &c1, &c2 };
   //Creating two Kookmin account
   Account c3 = Account("Amy", 3, 3, 3, 150000, "Kookmin");
   Account c4 = Account("Teddy", 4, 4, 4, 50000, "Kookmin");
   vector<Account*> Kookmin_account = { &c3, &c4 };
   //Creating Bank instance
   Bank Daegu(Daegu_account, 100000000, "Daegu");
   Bank Kookmin(Kookmin_account, 100000000, "Kookmin");
   vector<Account*> Account_array[]={Daegu_account, Kookmin_account};
   ATM_bilingual a1 = ATM_bilingual(&Kookmin, Account_array, "Single", 0, 1000000);
   ATM_bilingual a2 = ATM_bilingual(&Kookmin, Account_array, "Multi", 0, 1000000);
   a2.use();
}
