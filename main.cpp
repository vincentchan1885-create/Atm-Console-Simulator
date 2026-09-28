#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

// Configuration constants
const string    BANK_NAME            = "MMU DIGITAL BANK";
const string    ACCOUNT_HOLDER       = "LIM WEI JIE";
const string    ACCOUNT_NUMBER       = "1234567890";
const string    CORRECT_PIN          = "2580";
const int       MAX_PIN_ATTEMPTS     = 3;
const long long SEN_PER_RM           = 100;
const long long OPENING_BALANCE_SEN  = 125075;
const long long DAILY_LIMIT_RM       = 1500;
const long long MIN_AMOUNT_RM        = 10;
const long long NOTE_MULTIPLE_RM     = 10;
const long long MAX_DEPOSIT_RM       = 5000;
const int       SCREEN_WIDTH         = 52;

int main() {
    cout << "ATM System Initialized." << endl;
    return 0;
}