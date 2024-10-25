#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

string generatePassword(int length) {
    const string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*()_+?:/;,";
    const int charSetSize = characters.size();

    string password;

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < length; ++i) {
        int randomIndex = rand() % charSetSize;
        password += characters[randomIndex];
    }
    return password;
}

void xorEncryptDecrypt(const string& filename, const string& key) {
    ifstream fileIn(filename, ios::binary);
    if (!fileIn) {
        cout << "File could not be opened!" << endl;
        return;
    }

    string content((istreambuf_iterator<char>(fileIn)), istreambuf_iterator<char>());
    fileIn.close();

    for (size_t i = 0; i < content.size(); ++i) {
        content[i] ^= key[i % key.size()];
    }

    ofstream fileOut(filename, ios::binary);
    fileOut.write(content.c_str(), content.size());
    fileOut.close();
}

void viewAndReEncryptFile(const string& filename) {
    string key;
    cout << "Enter the encryption key: ";
    cin >> key;

    // Decrypt the file
    xorEncryptDecrypt(filename, key);
    cout << "The " << filename << " file has been decrypted." << endl;

    // Open the file in a text editor
    cout << "Opening the " << filename << " file for viewing..." << endl;
    system("notepad password.txt");

    // Re-encrypt the file
    xorEncryptDecrypt(filename, key);
    cout << "The " << filename << " file has been re-encrypted." << endl;
}

void generatePasswordAndSave(const string& filename, const string& key) {
    int account;
    string name;
    do {
        cout << "For Facebook press: 1" << endl << "For Twitter press: 2" << endl << "For LinkedIn press: 3" << endl << "Other press: 4" << endl;
        cout << "Which account password do you want to generate: ";
        cin >> account;
    } while ((account == 0) || (account > 4));

    switch (account) {
        case 1:
            name = "Facebook";
            break;
        case 2:
            name = "Twitter";
            break;
        case 3:
            name = "LinkedIn";
            break;
        default:
            name = "Other";
    }

    int answer;
    do {
        cout << "For randomized length of password press 1" << endl << "To set your own randomized length press 2" << endl << "Make selection: ";
        cin >> answer;
    } while ((answer == 0) || (answer > 2));

    int passwordLength;
    string password;
    if (answer == 1) {
        passwordLength = 16;
        password = generatePassword(passwordLength);
        cout << "" << endl << name << " password is: " << password << endl;
    } else if (answer == 2) {
        cout << "Enter the desired length of your password:  ";
        cin >> passwordLength;
        if (passwordLength <= 0) {
            cout << "Invalid selection" << endl;
        } else {
            password = generatePassword(passwordLength);
            cout << "" << endl << name << " password is: " << password << endl;
        }
    }

    if (((answer == 1) || (answer == 2)) && (passwordLength > 0)) {
        char saveOption;
        cout << "Do you want to save this password to the file? (y/n): ";
        cin >> saveOption;
        if (saveOption == 'y' || saveOption == 'Y') {
            // Decrypt the file if it exists
            ifstream infile(filename);
            if (infile.good()) {
                xorEncryptDecrypt(filename, key);
            }
            infile.close();

            ofstream passwordFile(filename, ios::app);
            passwordFile << "" << endl << name << " password is: " << password << endl;
            passwordFile.close();

            // Re-encrypt the file
            xorEncryptDecrypt(filename, key);
            cout << "The " << filename << " file has been re-encrypted." << endl;
        }
    }
}

int main() {
    int choice;
    do {
        cout << "To generate a new password press: 1" << endl << "To decrypt and view the password file press: 2" << endl << "Make selection: ";
        cin >> choice;
    } while ((choice != 1) && (choice != 2));

    if (choice == 1) {
        string key = "mysecretkey";
        generatePasswordAndSave("password.txt", key);
    } else if (choice == 2) {
        viewAndReEncryptFile("password.txt");
    }

    return 0;
}
