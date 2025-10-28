#include <iostream>
#include <vector>
#include <conio.h>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <numeric>
#include <windows.h>
using namespace std;

//// TERMINAL ////
void clearBI() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void clear(){system("cls");}

void flushKeys(){while (_kbhit()) getch();}

void enableUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

int keyPress() {return _getch();}

//// LOGIC ////

struct URandConsis {
    float UR;
    float Consistency;
};

URandConsis findURandConsis(const vector<float> & times) {
    URandConsis ret;
    float sum = accumulate(times.begin(), times.end(), 0.0);
    float mean = sum / times.size();

    float variance = 0.0;
    for (float x : times) {
        variance += (x - mean) * (x - mean);
    }
    variance /= times.size();

    float stddev = sqrt(variance);
    ret.UR = stddev * 10000;
    ret.Consistency = 100 / (1+(stddev/mean));
    return ret;
}

int streamSkill(const float bpm, const float consistency, const int taps) {
    //normalize consistency
    const float c = clamp(consistency / 100.0f, 0.0f, 1.0f);

    const float bpmRef = 180.0f; //"medium" difficulty
    const float speedEmphasis = 1.2f; // >1 rewards speed
    const float consisEmphasis = 2.0f; //punish inconsistency

    const float len = sqrt(max(1, taps)); //prevents longer session domination
    const float cap = 25.0f; //caps score from taps
    const float l = min(len /sqrt(cap), 1.5f); // soft cap

    const float speed = pow(max(bpm,1.0f)/bpmRef, speedEmphasis);
    const float cons = pow(c, consisEmphasis);
    float score = 1000.0f * speed * cons * l;

    score = static_cast<int>(lround(score));
    if (score >= 2100.0f) {
        cout << "\n";
        cout << R"ASCII(
███████╗███████╗
██╔════╝██╔════╝
███████╗███████╗)ASCII" << "Stream Skill: " << score << R"ASCII( SR
╚════██║╚════██║
███████║███████║
╚══════╝╚══════╝
)ASCII";
    }
    else if (score >= 1700) {
        cout << "\n";
        cout << R"ASCII(
███████╗
██╔════╝
███████╗)ASCII" << "Stream Skill: " << score << R"ASCII( SR
╚════██║
███████║
╚══════╝
)ASCII";
    }
    else if (score >= 1400) {
        cout << "\n";
        cout << R"ASCII(
 █████╗
██╔══██╗
███████║)ASCII" << "Stream Skill: " << score << R"ASCII( SR
██╔══██║
██║  ██║
╚═╝  ╚═╝
)ASCII";
    }
    else if (score >= 1100) {
        cout << "\n";
        cout << R"ASCII(
██████╗
██╔══██╗
██████╔╝)ASCII" << "Stream Skill: " << score << R"ASCII( SR
██╔══██╗
██████╔╝
╚═════╝
)ASCII";
    }
    else if (score >= 800) {
        cout << "\n";
        cout << R"ASCII(
 ██████╗
██╔════╝
██║     )ASCII" << "Stream Skill: " << score << R"ASCII( SR
██║
╚██████╗
 ╚═════╝
)ASCII";
    }
    else if (score >= 500) {
        cout << "\n";
        cout << R"ASCII(
██████╗
██╔══██╗
██║  ██║)ASCII" << "Stream Skill: " << score << R"ASCII( SR
██║  ██║
██████╔╝
╚═════╝
)ASCII";
    }
    else if (score < 500) {
        cout << "\n";
        cout << R"ASCII(
███████╗
██╔════╝
█████╗  )ASCII" << "Stream Skill: " << score << R"ASCII( SR
██╔══╝
██║
╚═╝
)ASCII";
    }
    return score;

}

float findBPM(const vector<float> & times, const int TPB) {
    float total = 0.0;

    for (float f:times){total += f;}

    double avg = total / static_cast<float>(times.size());
    double TPM = 60 / avg;

    return static_cast<float>(TPM / static_cast<float>(TPB));
}

void displayGraph(const vector<float> & bpmData);

vector<float> stream(string mode, int tapInput, int & tapAmt, int seconds) {
    flushKeys();
    vector<float> times;
    vector<float> bpm;
    auto start = chrono::high_resolution_clock::now();
    auto last = start;
    while (true) {
        int tap = keyPress();
        if (tap == tolower('D') || tap == tolower('G')) {
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
        if (mode == "normal"){
            if (tapAmt >= tapInput) {
                break;
            }
        }
        else if (mode == "timed") {
            chrono::duration<float> timeEla = last - start;
            if (static_cast<int>(timeEla.count()) >= seconds) {
                break;
            }
        }


    }
    displayGraph(bpm);
    return times;
}

void displayGraph(const vector<float> & bpmData) {
    if (bpmData.empty()) return;

    std::ios::fmtflags f = cout.flags();
    std::streamsize p = cout.precision();

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
    for (int i = 2; i <= bpmData.size(); ++i) {
        cout << ((i % 5 == 0) ? to_string(i):" ");
    }
    cout << "\n";

    cout.flags(f);
    cout.precision(p);
}

//// APPLICATION ////

void startOption(int TPB) {
    int tapInput = 0;
    int tapAmt = 0;
    while (tapInput == 0) {
        cout << "       BPM TEST\n Enter desired amount of key presses (20 - 1000): ";
        cin >> tapInput;
        if (1000 >= tapInput && tapInput >= 20) {
            cout << "\n\n BPM test, up to " << tapInput << ". Key presses start in 3..."; Sleep(1000);
            cout << "2..."; Sleep(1000); cout << "1... "; Sleep(1000); cout << "GO!\n\n";

            const vector<float> times = stream("normal", tapInput, tapAmt, 0);

            float BPM = findBPM(times, TPB);
            URandConsis data= findURandConsis(times);

            cout << "\n\nBPM: " << BPM << " \nUR: " << data.UR <<"\nConsistency: " << data.Consistency << "%";
            int score = streamSkill(BPM, data.Consistency, tapInput);
            cout << "\n\n Press Enter to Exit...";
            flushKeys();
            clearBI();
            Sleep(1000);
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

void timedMode(int TPB) {
    int timeInput = 0;
    int tapAmt = 0;
    while (timeInput == 0) {
        cout << "       Timed Mode! Enter desired time (5 - 600s): ";
        cin >> timeInput;
        if (600 >= timeInput && timeInput >= 5) {
            cout << "\n\n " << time << " second BPM test starts in 3..."; Sleep(1000); cout << "2...";
            Sleep(1000); cout << "1... "; Sleep(1000); cout << "GO!\n\n";

            const vector<float> times = stream("timed", 0, tapAmt, timeInput);

            float BPM = findBPM(times, TPB);
            URandConsis data= findURandConsis(times);

            cout << "\n\nBPM: " << BPM << " \nUR: " << data.UR <<"\nConsistency: " << data.Consistency << "%";
            int score = streamSkill(BPM, data.Consistency, tapAmt);
            cout << "\n\nPress Enter to Exit...";
            flushKeys();
            clearBI();
            Sleep(1000);
            cin.get();
            break;
        }
        cout << "Not a valid option.";
        Sleep(500);
        clearBI();
        clear();
        timeInput = 0;
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
        cout << "\n\n1) Start\n2) Timed Mode\n3) Settings\n4) Exit\nChoose an option: ";
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
                timedMode(4);
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