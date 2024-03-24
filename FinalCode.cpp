// To perform common input/output operations include the input/output stream library.
#include <iostream>
// Use the string library to access functions for managing strings.
#include <string>
// Use the fstream library when doing file input/output tasks.
#include <fstream>
// Add the cstdlib library to the list of general utilities.
#include <cstdlib>
// To use functions related to time, include the ctime library.
#include <ctime>
// To utilize character categorization routines, include the cctype library.
#include <cctype>
// To prevent prefixing standard library items, use the standard namespace.
using namespace std;
// Define a constant, three-valued integer called SHIFT.
const int SHIFT = 3; 


// using a class named loginManager
class LoginManager {
     // public access 
public:
 // Constructor for loginManager class

    LoginManager() {
        // Initialize accessGranted to false
        accessGranted = false;
     
    }// the ending of the constructor

// Function to register to an acount 
void registerAccount() {
    // a chocie variable 
    string choice;
    // printing statement
    cout << "Enter your username: ";
    // a user enters a username input 
    cin >> username;
    // encrypt username
    encrypt(username);  
    // output statment
    cout << "Do you want to generate a password? (yes/no): ";
    cin >> choice;
    // check if the choice is yes
    if (choice == "yes") {
        // the password will be generated
        password = generatePassword();
        // printing the generated password
        cout << generatePassword() << endl;
        // encrypt password
        encryptPassword(); 
        cout << "Your password has been generated and encrypted." << endl;
    } else {
        
        cout << "Enter your password: ";
         // a user enters a password 
        cin >> password;
        // Loop until a strong password is entered
        while (!isStrongPassword(password)) {
            cout << "Your password is not strong enough." << endl;
            cout << "Make sure it contains at least 8 characters, including at least one uppercase letter, one special character, and one digit." << endl;
            cout << "Enter your password again: ";
            cin >> password;
        }
        cout << "Your password is strong." << endl;
        // Encrypt password
        encryptPassword(); 
    }
   
    cout << "Account successfully registered!" << endl;
    // Save account info to file
    saveToFile(username, password); 
    // call the login to let the user to log in 
    login();
}
// Continuously prompt the user to log in until access is granted
 void login() {
        while (!accessGranted) {
            bool usernameCorrect = false;
            bool passwordCorrect = false;

            // Prompt the user to enter the username
            while (!usernameCorrect) {
                cout << "Enter your username: ";
                // Read the username from the user
                cin >> username;
                // Encrypt username
                encrypt(username);  
                // Check if encrypted username exists in file
                if (checkUsernameInFile(username)) {
                    usernameCorrect = true;
                } else {
                    cout << "Username does not exist. Please try again." << endl;
                }
            }

            // Prompt the user to enter the password
            while (!passwordCorrect) {
                cout << "Enter your password: ";
                // Read the password from the user
                cin >> password;
                encryptPassword(); 

               
                // Check if encrypted username and password match in file
                if (checkPasswordInFile(username, password)) {
                    passwordCorrect = true;
                } else {
                    cout << "Incorrect password. Please try again." << endl;
                }
            }
            cout << "You have access" << endl;
            accessGranted = true;

            int option;
             // Provide options after successful login
            cout << "Choose an option:" << endl;
            cout << "1. Change Password" << endl;
            cout << "2. Delete Account" << endl;
            cout << "3. Logout" << endl;
            cin >> option;
             // Perform actions based on the user's choice
            if (option == 1) {
                string newPassword;
                cout << "Enter your new password: ";
                cin >> newPassword;
                encryptPassword();
                changePassword(username, newPassword);
                cout << "Password changed successfully!" << endl;
            } else if (option == 2) {
                deleteAccount(username);
                cout << "Account deleted successfully!" << endl;
                accessGranted = false; 
                break;
            } else if (option == 3) {
                accessGranted = false;
                cout << "Logged out successfully!" << endl;
                break;
            } else {
                cout << "Invalid option. Please choose again." << endl;
            }
        }
    }

    // Function to retrieve password based on username
    void retrievePassword() {
    cout << "Enter your username: ";
    cin >> username;
    encrypt(username); // Encrypt username for comparison

    // Open the accounts_info.txt file
    ifstream inFile("accounts_info.txt");
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            // Check if the line contains the encrypted username
            if (line.find("Username: " + username) != string::npos) {
                // Extract and decrypt the encrypted password
                size_t pos = line.find("Password: ");
                if (pos != string::npos) {
                    string encryptedPassword = line.substr(pos + 10); // Length of "Password: "
                    decrypt(encryptedPassword); // Decrypt the password
                    cout << "Your password is: " << encryptedPassword << endl;
                    inFile.close();
                    return;
                }
            }
        }
        inFile.close();
        cout << "Username not found." << endl;
    } else {
        cout << "Unable to open file!" << endl;
    }
}



   


// private section contains functions 
private:
// declare username variable
    string username;
//declaring password variable
    string password;
//Variable to track whether access is granted or not
    bool accessGranted;
// Function to generate a random password
    string generatePassword() {
// Variable to store the generated password
        string generatedPassword;
        //character set to generate the password from
        const string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890@!-/*-+?><!@#$";
        // Length of the generated password
        const int length = 12; 
        //Seed the random number generator
        srand(time(0));
       // Loop to generate characters for the password
        for (int i = 0; i < length; ++i) {
            // Append a random character from the character set to the generated password
            generatedPassword += charset[rand() % charset.length()];
        }
        // return generated password 
        return generatedPassword;
    }
// Function to encrypt the password
    void encryptPassword() {
        // Loop through each character in the password
        for (char &c : password) {
            //If the character is alphabetic
            if (isalpha(c)) {
                // Determine the base ASCII value for lowercase or uppercase letters
                char base = islower(c) ? 'a' : 'A';
                // Apply the Caesar cipher shift to encrypt the character
                c = base + (c - base + SHIFT) % 26;
            }
        }
    }
// Function to encrypt a string
    void encrypt(string &text) {
        for (char &c : text) {
            if (isalpha(c)) {
                char base = islower(c) ? 'a' : 'A';
                c = base + (c - base + SHIFT) % 26;
            }
        }
    }


    // Function to decrypt a string
    void decrypt(string &text) {
    for (char &c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = base + (c - base - SHIFT + 26) % 26; // Decrypt by shifting backwards
        }
    }
}

   // Function to save the account information to a file
    void saveToFile(string username, string password) {
        // Open file in append mode
        ofstream outFile("accounts_info.txt", ios::app); 
        // Check if the file is successfully opened
        if (outFile.is_open()) {
            // Write username and password to the file
            outFile << "Username: " << username << ", Password: " << password << endl;
            // Close the file
            outFile.close();
        } else {
            cout << "Unable to open file!" << endl;
        }
    }
// Function to check if a password is strong
    bool isStrongPassword(string s){
        // Flags to track if password contains uppercase letters, special characters, and digit
        bool containsUpper = false,containsSpecialChar = false, containsNumber = false;
        // Loop through each character of the password
        for(auto c:s)
        {
            // Check if the character is an uppercase letter
            if(isupper(c))
            // Set containsUpper flag to true
            containsUpper= true;

            // Check if the character is a special charact

            if(ispunct(c))
            // Set containsSpecialChar flag to 
            containsSpecialChar= true;



            // Check if the character is a digit
            if(isdigit(c))
            // Set containsNumber flag to true
            containsNumber= true;

        }
        // Check if password meets all criteria for being strong
        if(containsUpper && containsSpecialChar && containsNumber && s.size()>=8)
        // Return true if the password is strong
            return true;
            // Return false if the password is not strong
        return false;

    }

    

    // Helper function to check if encrypted username exists in file
    bool checkUsernameInFile(string encryptedUsername) {
         // Open the accounts_info.txt file
        ifstream inFile("accounts_info.txt");
         // Check if the file is successfully opened
        if (inFile.is_open()) {
            // Variable to store each line of the file
            string line;
             // Loop through each line of the file
            while (getline(inFile, line)) {
                  // Check if the line contains the encrypted username
                if (line.find("Username: " + encryptedUsername) != string::npos) {
                    // Close the file
                    inFile.close();
                    return true;// Return true if the encrypted username is found
                }
            }
            // Close the file
            inFile.close();
        } else {
            // Print an error message if the file cannot be opened
            cout << "Unable to open file!" << endl;
        }
        // Return false if the encrypted username is not found or if there's an error opening the file
        return false;
    }

    // Helper function to check if encrypted username and password match in file
    bool checkPasswordInFile(string encryptedUsername, string encryptedPassword) {
        // Open the accounts_info.txt file
        ifstream inFile("accounts_info.txt");
        // Check if the file is successfully opened
        if (inFile.is_open()) {
            // Variable to store each line of the file
            string line;
            // Loop through each line of the file
            while (getline(inFile, line)) {
                // Check if the line contains the encrypted username
                if (line.find("Username: " + encryptedUsername) != string::npos) {
                    // Found matching username, now check password
                    if (line.find("Password: " + encryptedPassword) != string::npos) {// Check if the line contains the encrypted password
                        inFile.close();
                        return true;
                    }
                }
            }
            inFile.close();
        } else {
            cout << "Unable to open file!" << endl;
        }
        return false;
    }

    // Prompt the user to enter a new password until a strong password is provided

     void changePassword(string username, string newPassword) {
    while (!isStrongPassword(newPassword)) {
        cout << "Your new password is not strong enough." << endl;
        cout << "Make sure it contains at least 8 characters, including at least one uppercase letter, one special character, and one digit." << endl;
        cout << "Enter your new password again: ";
        // Read the new password from the user
        cin >> newPassword;
    }

    // Encrypt the new password before updating the account information in the file
    encrypt(newPassword);
    // Open the accounts file for reading
    ifstream inFile("accounts_info.txt");
    // Create a temporary file for writing
    ofstream tempFile("temp_accounts_info.txt");

    // Check if both files are open and can be used
    if (inFile.is_open() && tempFile.is_open()) {
        string line;
        // Read each line from the accounts file
        while (getline(inFile, line)) {
            // Check if the current line contains the username to be updated
            if (line.find("Username: " + username) != string::npos) {
                // If yes, write the updated username and password to the temporary file
                tempFile << "Username: " << username << ", Password: " << newPassword << endl;
            } else {
                 // If not, write the line as it is to the temporary file
                tempFile << line << endl;
            }
        }
        inFile.close();
        tempFile.close();
        remove("accounts_info.txt");
        rename("temp_accounts_info.txt", "accounts_info.txt");
    } else {
        cout << "Unable to open file!" << endl;
    }
}

// Function to delete the account associated with the given username
void deleteAccount(string username) {
    // Open the accounts file for reading
    ifstream inFile("accounts_info.txt");
    // Create a temporary file for writing
    ofstream tempFile("temp_accounts_info.txt");

    // Check if both files are open and can be used
    if (inFile.is_open() && tempFile.is_open()) {
        string line;
        // Read each line from the accounts file
        while (getline(inFile, line)) {
             // Check if the current line does not contain the username to be deleted
            if (line.find("Username: " + username) == string::npos) {
                  // If not, write the line to the temporary file
                tempFile << line << endl;
            }
        }
        // Close both files
        inFile.close();
        tempFile.close();
        // Remove the original accounts file
        remove("accounts_info.txt");
        // Rename the temporary file to the original accounts file
        rename("temp_accounts_info.txt", "accounts_info.txt");
    } else {
        // If unable to open files, display an error message
        cout << "Unable to open file!" << endl;
    }
    }
};
// Main function to run the password manager application
int main() {
    // Create an object of the LoginManager class
    LoginManager loginManagerObj;
    // Variable to store the user's choice
    int choice;
    // Prompt the user to choose an option
    cout << "Choose an option:" << endl;
    // Option to register an account
    cout << "1. Register" << endl;
    // Option to login
    cout << "2. Login" << endl;
    cout << "3. Retrieve Password" << endl; // Added option for retrieving password
    cin >> choice;

    // If the user chooses to register
    if (choice == 1) {
        // Call the registerAccount method of the LoginManager object
        loginManagerObj.registerAccount();
        // Call the login method of the LoginManager object
        loginManagerObj.login();
        // If the user chooses to login 
    } else if (choice == 2) {
        // Call the login method of the LoginManager object
        loginManagerObj.login();
    } else if (choice == 3) {
        // Call the a method to retrieve password
        loginManagerObj.retrievePassword(); 
    } else {
        // Print an error message if the user enters an invalid choice
        cout << "Invalid choice. Please choose again." << endl;
    }
    // Return 0 to indicate successful completion of the program
    return 0;
}