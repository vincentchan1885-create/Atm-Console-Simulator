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

void printRow(const string& label, const string& value) {
    cout << "  " << left << setw(22) << label << ": " << value << '\n';
}

string formatRM(long long sen) {
    bool isNegative = sen < 0;
    if (isNegative) sen = -sen;
    long long ringgit = sen / SEN_PER_RM;
    long long cents   = sen % SEN_PER_RM;

    string digits = to_string(ringgit);
    string withCommas;
    int digitCount = 0;
    for (int i = static_cast<int>(digits.length()) - 1; i >= 0; --i) {
        withCommas.insert(withCommas.begin(), digits[i]);
        digitCount++;
        if (digitCount % 3 == 0 && i > 0) {
            withCommas.insert(withCommas.begin(), ',');
        }
    }

    string centsText = (cents < 10 ? "0" : "") + to_string(cents);
    return string(isNegative ? "-" : "") + "RM " + withCommas + "." + centsText;
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

void displayMenu() {
    printHeader("MAIN MENU");
    cout << "  [1] Balance Inquiry\n"
         << "  [2] Cash Withdrawal\n"
         << "  [3] Cash Deposit\n"
         << "  [4] Exit & Return Card\n"
         << "  [5] About ATM Technology\n";
    printLine('-');
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

void showBalance(long long balanceSen, long long withdrawnTodayRM) {
    printHeader("BALANCE INQUIRY");
    printRow("Account Holder", ACCOUNT_HOLDER);
    printRow("Account Number", maskAccountNumber(ACCOUNT_NUMBER));
    printRow("Available Balance", formatRM(balanceSen));
    printRow("Withdrawn Today", formatRM(withdrawnTodayRM * SEN_PER_RM));
    printRow("Daily Limit Left", formatRM((DAILY_LIMIT_RM - withdrawnTodayRM) * SEN_PER_RM));
}

void processWithdrawal(long long& balanceSen, long long& withdrawnTodayRM) {
    printHeader("CASH WITHDRAWAL");
    long long amountRM = 0;
    cout << "\n  Enter amount to withdraw (RM): ";
    cin >> amountRM;

    if (amountRM <= 0) {
        cout << "  [X] Amount must be greater than zero.\n";
    } else if (amountRM > balanceSen / SEN_PER_RM) {
        cout << "  [X] Insufficient funds. Available balance: " << formatRM(balanceSen) << '\n';
    } else {
        balanceSen -= amountRM * SEN_PER_RM;
        withdrawnTodayRM += amountRM;
        cout << "  [OK] Withdrawal successful. Please collect your cash.\n";
        cout << "  Remaining balance: " << formatRM(balanceSen) << '\n';
    }
}

void processDeposit(long long& balanceSen) {
    printHeader("CASH DEPOSIT");
    long long amountRM = 0;
    cout << "\n  Enter amount to deposit (RM): ";
    cin >> amountRM;

    if (amountRM <= 0) {
        cout << "  [X] Invalid deposit amount.\n";
    } else {
        balanceSen += amountRM * SEN_PER_RM;
        cout << "  [OK] Deposit successful. New balance: " << formatRM(balanceSen) << '\n';
    }
}

int main() {
    long long balanceSen       = OPENING_BALANCE_SEN;
    long long withdrawnTodayRM = 0;

    displayWelcomeScreen();

    if (!authenticatePin()) {
        displayCardBlocked();
        return 0;
    }

    bool sessionActive = true;
    int choice = 0;

    do {
        displayMenu();
        cout << "  Select an option (1-5): ";
        cin >> choice;

        switch (choice) {
            case 1:
                showBalance(balanceSen, withdrawnTodayRM);
                break;
            case 2:
                processWithdrawal(balanceSen, withdrawnTodayRM);
                break;
            case 3:
                processDeposit(balanceSen);
                break;
            case 4:
                cout << "\n  Exiting session. Thank you for using our ATM.\n";
                sessionActive = false;
                break;
            case 5:
                cout << "\n  [Stub] Historical overview of ATM innovation.\n";
                break;
            default:
                cout << "  [!] Invalid option. Please choose a number from 1 to 5.\n";
                continue;
        }

        if (sessionActive) {
            char cont;
            cout << "\n  Perform another transaction? (Y/N): ";
            cin >> cont;
            if (cont == 'N' || cont == 'n') {
                sessionActive = false;
            }
        }
    } while (sessionActive);

    return 0;
}