#include <iostream>
#include <vector>
#include <conio.h>
#include <limits>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <windows.h>
using namespace std;

//// TERMINAL ////
void clearBI() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void clear(){system("cls");}

void enableUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

int keyPress() {return _getch();}

//// LOGIC ////

float findBPM(const vector<float> & times, const int TPB) {
    float total = 0.0;

    for (float f:times){total += f;}

    double avg = total / static_cast<float>(times.size());
    double TPM = 60 / avg;

    return static_cast<float>(TPM / static_cast<float>(TPB));
}

void displayGraph(const vector<float> & bpmData);

vector<float> stream(int tapInput, int tapAmt) {
    vector<float> times;
    vector<float> bpm;
    auto start = chrono::high_resolution_clock::now();
    auto last = start;
    cout << defaultfloat << setprecision(6);
    while (true) {
        int tap = keyPress();
        if (tap == tolower('Z') || tap == tolower('X')) {
            tapAmt++;
            auto now = chrono::high_resolution_clock::now();
            chrono::duration<float> diff = now - last;
            last = now;
            cout << static_cast<char>(tap) << " : " << diff.count() << "\n";

            if (tapAmt > 1) {
                times.push_back(diff.count());
                bpm.push_back(findBPM(times, 4));
            }
        }
        if (tapAmt >= tapInput) {
            break;
        }


    }
    displayGraph(bpm);
    return times;
}

void displayGraph(const vector<float> & bpmData) {
    if (bpmData.empty()) return;

    const int height = 10;
    float maxBPM = *max_element(bpmData.begin(), bpmData.end());
    float minBPM = *min_element(bpmData.begin(), bpmData.end());

    cout << "\n\nBPM Stability:\n";

    for (int row = height; row >= 0; --row) {
        float threshold = minBPM + (maxBPM - minBPM) * static_cast<float>(row)/height;
        cout << setw(4) << fixed << setprecision(0) << threshold << " | ";
        for (float bpm:bpmData) {
            cout << (bpm >= threshold ? "█": " ");
        }
        cout << "\n";
    }
    cout << "       "<<string(bpmData.size(), '-') << '\n';
    cout << "       1";
    for (int i = 2; i <= (int)bpmData.size(); ++i) {
        cout << ((i % 5 == 0) ? to_string(i):" ");
    }
    cout << "\n";
}

//// APPLICATION ////

void startOption(int TPB) {
    int tapInput = 0;
    int tapAmt = 0;
    while (tapInput == 0) {
        cout << "       BPM TEST\n Enter desired amount of key presses (20 - 1000): ";
        cin >> tapInput;
        if (1000 >= tapInput && tapInput >= 20) {
            cout << "\n\n BPM test up to " << tapInput << " key presses starts in 3..."; Sleep(1000);
            cout << "2..."; Sleep(1000); cout << "1... "; Sleep(1000); cout << "GO!\n\n";

            float BPM = findBPM(stream(tapInput, tapAmt), TPB);

            cout << "\n\nWell Done! Your average BPM was " << BPM << ". Press Enter to Exit...";
            Sleep(1000);
            clearBI();
            cin.get();
            break;
        }
        cout << "Not a valid option.";
        Sleep(500);
        clearBI();
        clear();
        tapInput = 0;
    }

}




int main() {
    enableUTF8();
    SetConsoleTitle("BEEPM");
    clear();
    bool loop = true;
    int option = 0;
    vector<int> options = {1,2,3,4};


    while (loop) {
        option = 0;
        clear();
        cout << "                  buhbz\n";
        cout << R"ASCII(
 ▄▄▄▄   ▓█████ ▓█████  ██▓███   ███▄ ▄███▓
▓█████▄ ▓█   ▀ ▓█   ▀ ▓██░  ██▒▓██▒▀█▀ ██▒
▒██▒ ▄██▒███   ▒███   ▓██░ ██▓▒▓██    ▓██░
▒██░█▀  ▒▓█  ▄ ▒▓█  ▄ ▒██▄█▓▒ ▒▒██    ▒██
░▓█  ▀█▓░▒████▒░▒████▒▒██▒ ░  ░▒██▒   ░██▒
░▒▓███▀▒░░ ▒░ ░░░ ▒░ ░▒▓▒░ ░  ░░ ▒░   ░  ░
▒░▒   ░  ░ ░  ░ ░ ░  ░░▒ ░     ░  ░      ░
 ░    ░    ░      ░   ░░       ░      ░
 ░         ░  ░   ░  ░                ░
      ░
)ASCII";
        cout << "\n\n1) Start\n2) Seconds Mode\n3) Settings\n4) Exit\nChoose an option: ";
        while (option == 0) {
            cin  >> option;
            auto check = find(options.begin(),options.end(), option);
            if (check != options.end()) {
                break;
            }
            cout << "Invalid option, please try again.\n";
            Sleep(500);
            clearBI();
            option = 0;
            clear();
            break;
        }
        if (option == 0) {continue;}

        switch (option) {

            case 1:
                clear();
                startOption(4);
                break;

            case 2:
                clear();
                cout << "yay now returning";
                Sleep(1000);
                break;

            case 3:
                clear();
                cout << "yay now returning";
                Sleep(1000);
                break;

            case 4:
                clear();
                cout << "Exiting...";
                loop = false;
                break;

        }
    }
    return 0;
}
