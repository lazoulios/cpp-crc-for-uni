#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

string generateMessage(int k) { //generation of random message
    string message;
    for (int i = 0; i < k; ++i) {
        message += rand() % 2 ? '1' : '0';
    }
    return message;
}

string xorStrings(const string &a, const string &b) { //xor function for the mod2 division
    string result;
    for (size_t i = 1; i < b.size(); ++i) {
        result += (a[i] == b[i]) ? '0' : '1';
    }
    return result;
}

string mod2div(string dividend, const string &divisor) { //mod2 division
    int pick = divisor.size();
    string tmp = dividend.substr(0, pick);

    while (pick < dividend.size()) {
        if (tmp[0] == '1') {
            tmp = xorStrings(divisor, tmp) + dividend[pick];
        } else {
            tmp = xorStrings(string(pick, '0'), tmp) + dividend[pick];
        }
        ++pick;
    }

    if (tmp[0] == '1') {
        tmp = xorStrings(divisor, tmp);
    } else {
        tmp = xorStrings(string(pick, '0'), tmp);
    }

    return tmp;
}

string calculateCRC(const string &message, const string &P) { //calculation of CRC
    string message_with_0 = message + string(P.size() - 1, '0');
    return mod2div(message_with_0, P);
}

string transmitMessage(const string &message, const string &crc, double ber) { //transmitting message through a noisy channel
    string transmitted_message = message + crc;
    string noisy_message;
    for (char bit : transmitted_message) {
        double random = (double)rand() / RAND_MAX;
        if (random <= ber) {
            noisy_message += (bit == '1') ? '0' : '1';
        } else {
            noisy_message += bit;
        }
    }
    return noisy_message;
}

bool checkCRC(const string &received_message, const string &divisor) { //checks crc at receiver end
    return mod2div(received_message, divisor).find('1') == string::npos;
}

int main() {
    srand(time(0)); //used for the random message generator

    int k = rand() % 15; //creation of random k vat for message length. upper bound 15 for simplicity, works without too.
    string P;
    cout << "Δώστε δυαδικό αριθμό για πρότυπο υπολογισμού του CRC:" << endl;
    cin >> P; //the divisor used for the CRC
    double BER = 0.001;

    string message = generateMessage(k); //generating the random message with max length k
    cout << "Αρχικό μήνυμα: " << message << endl;

    string crc = calculateCRC(message, P); //calculating the CRC of the message
    cout << "Υπολογισμένο CRC: " << crc << endl;

    string noisy_message = transmitMessage(message, crc, BER); //passing the message through a noisy channel (with BER=0.01)
    cout << "Ληφθέν μήνυμα: " << noisy_message << endl;

    bool is_valid = checkCRC(noisy_message, P); //checking if the message is valid
    cout << "Το μήνυμα είναι " << (is_valid ? "έγκυρο" : "μη έγκυρο") << endl;
    
    return 0;
}
