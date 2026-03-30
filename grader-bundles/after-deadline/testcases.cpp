// run selected test cases

#include "level1.cpp"
#include "level2.cpp"
#include "level3.cpp"

int main() {
    cout << "========================================" << endl;
    cout << "The Last Ember" << endl;

    while (true) {
        cout << "\nSelect a level to run: 1 / 2 / 3" << endl;
        cout << "Enter q to quit" << endl;
        cout << "> ";

        string choice;
        if (!(cin >> choice)) {
            break;
        }

        if (choice == "1") {
            level1();
        }
        else if (choice == "2") {
            level2();
        }
        else if (choice == "3") {
            level3();
        }
        else if (choice == "q" || choice == "Q") {
            break;
        }
        else {
            cout << "Invalid input, please try again." << endl;
        }
    }

    cout << "\n========================================" << endl;
    cout << "Game Over!" << endl;
    cout << "\n========================================" << endl;

    return 0;
}

