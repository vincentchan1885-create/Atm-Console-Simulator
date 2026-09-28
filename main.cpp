#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
#include <cctype>
#include <cstdlib>

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

void printLine(char symbol = '=') {
    cout << string(SCREEN_WIDTH, symbol) << '\n';
}

void printCentered(const string& text) {
    int padding = (SCREEN_WIDTH - static_cast<int>(text.length())) / 2;
    if (padding < 0) padding = 0;
    cout << string(padding, ' ') << text << '\n';
}

void printHeader(const string& title) {
    cout << '\n';
    printLine('=');
    printCentered(title);
    printLine('=');
}

string maskAccountNumber(const string& accountNumber) {
    if (accountNumber.length() <= 4) return accountNumber;
    return string(accountNumber.length() - 4, '*') +
           accountNumber.substr(accountNumber.length() - 4);
}

void displayWelcomeScreen() {
    cout << '\n';
    printLine('*');
    printCentered(BANK_NAME);
    printCentered("AUTOMATED TELLER MACHINE");
    printLine('*');
    printCentered("Please insert your card...");
    printCentered("[ Card detected: " + maskAccountNumber(ACCOUNT_NUMBER) + " ]");
    printLine('-');
    cout << "  Shield the keypad when entering your PIN.\n";
}

void displayCardBlocked() {
    printHeader("CARD BLOCKED");
    cout << "  Too many incorrect PIN attempts (" << MAX_PIN_ATTEMPTS << "/"
         << MAX_PIN_ATTEMPTS << ").\n"
         << "  Your card has been retained for security.\n"
         << "  Please contact your bank branch to reactivate it.\n";
    printLine('=');
}

bool isFourDigitPin(const string& text) {
    if (text.length() != 4) return false;
    for (char ch : text) {
        if (!isdigit(static_cast<unsigned char>(ch))) return false;
    }
    return true;
}

bool authenticatePin() {
    int attemptsUsed = 0;
    string enteredPin;

    while (attemptsUsed < MAX_PIN_ATTEMPTS) {
        cout << "\n  Enter your 4-digit PIN: ";
        if (!(cin >> enteredPin)) return false;

        if (!isFourDigitPin(enteredPin)) {
            cout << "  [!] Invalid format. PIN must be exactly 4 digits (0-9).\n";
        } else if (enteredPin == CORRECT_PIN) {
            cout << "  [OK] PIN verified. Welcome, " << ACCOUNT_HOLDER << ".\n";
            return true;
        } else {
            attemptsUsed++;
            int remaining = MAX_PIN_ATTEMPTS - attemptsUsed;
            if (remaining > 0) {
                cout << "  [X] Incorrect PIN. " << remaining << " attempt"
                     << (remaining == 1 ? "" : "s") << " remaining.\n";
            }
        }
    }
    return false;
}

int main() {
    displayWelcomeScreen();

    if (!authenticatePin()) {
        displayCardBlocked();
        return 0;
    }

    cout << "\nAccess granted. Ready for transaction menu.\n";
    return 0;
}