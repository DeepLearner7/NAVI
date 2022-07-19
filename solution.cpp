#include<bits/stdc++.h>
using namespace std;

/*
*   Class containg Balance Output 
*   
*/
class BalanceOutput{
    public:
        BalanceOutput(){}
        string bank_name;
        string name;
        float amount;
        int emis;
};


/*
*   Class Object to store the Loan Information
*   
*/
class LoanDetails{
    public:
        LoanDetails(){}
        string bank_name;
        string name;
        float total_amount;
        int duration;
        float emi;
        vector<pair<int, float>> lump_sum;
};


/*
*   Class Handling storage of loans and updating the lump sums
*   
*/
class StorageService{
    public:
        StorageService(){}
        // Store Loans Corresponding to Bank Name and User Name 
        map<string, map<string, LoanDetails*>> loans;

        void addLoan(string bank_name, string name, float amount, float percentage, int years){
            LoanDetails* new_loan = new LoanDetails();
            new_loan->bank_name = bank_name;
            new_loan->name = name;
            new_loan->duration = years * 12;

            float interest = (amount * percentage * years) / 100;
            float total = amount + interest;
            
            new_loan->emi = ceil(total / new_loan->duration);
            new_loan->total_amount = total;

            loans[bank_name][name] = new_loan;
        }

        void addLumpSum(string bank_name, string name, float amount, int emi_no){
            LoanDetails* loan = loans[bank_name][name];
            loan->lump_sum.push_back({emi_no, amount});
        }

        pair<float, int> getBalance(string bank_name, string name, int emi_no){
            LoanDetails* loan = loans[bank_name][name];
            pair<float, int> balance;

            float lump_sum_paid = 0;
            vector<pair<int, float>> lump_sums = loan->lump_sum;
            for(int i=0;i<lump_sums.size();i++){
                if(lump_sums[i].first <= emi_no){
                    lump_sum_paid += lump_sums[i].second;
                }
            }

            float total_paid = (loan->emi * emi_no) + lump_sum_paid;
            balance.first = total_paid;

            float total_left = loan->total_amount - total_paid;
            balance.second = ceil(total_left / loan->emi);

            return balance;
        }
};

/*
*   Ledger Company exposing the services LOAN, PAYMENT, BALANCE
*   
*/
class LedgerCompany{
    StorageService storageService;
    public:
        LedgerCompany(){}
        
        void getLoan(string bank_name, string name, float amount, int years, float percentage){
            storageService.addLoan(bank_name, name, amount, percentage, years);
        }


        void payment(string bank_name, string name, float amount, int emi_no){
            storageService.addLumpSum(bank_name, name, amount, emi_no);
        }

        BalanceOutput balance(string bank_name, string name, int emi_no){
            BalanceOutput result;
            result.bank_name = bank_name;
            result.name = name;
            pair<float, int> balance = storageService.getBalance(bank_name, name, emi_no);
            result.emis = ceil(balance.second);
            result.amount = ceil(balance.first);
            return result;
        }
};

int main()
{   
    LedgerCompany* ledger = new LedgerCompany();
    BalanceOutput output;
    string bank_name, name;
    float amount, percentage;
    int years, emi_no;


    /*
    *   FILE HANDLING
    *
    *   Reading the File and processing the input   
    */
    fstream inputfile;
    inputfile.open("input.txt",ios::in);
    string line;
    while(getline(inputfile, line)){
        int i = 0;
        string curr = "";
        vector<string> input;
        while(i < line.size()){
            if(line[i] != ' '){
                curr += line[i];
            }else{
                input.push_back(curr);
                curr = "";
            }
            i+=1;
        }
        input.push_back(curr);


        if(line[0] == 'L'){
            // Creating a loan
            ledger->getLoan(input[1], input[2], stof(input[3]), stoi(input[4]), stof(input[5]));
        }else if(line[0] == 'P'){
            // Adding Lump Sum Amount
            ledger->payment(input[1], input[2], stof(input[3]), stoi(input[4]));
        }else{
            // Fetching Balances
            output = ledger->balance(input[1], input[2], stoi(input[3]));
            cout<<output.bank_name<<" "<<output.name<<" "<<output.amount<<" "<<output.emis<<endl;
        }
    }
    inputfile.close();

    return 0;
}
