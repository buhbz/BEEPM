#include <algorithm>
#include <chrono>
#include <cmath>
#include <conio.h>
#include <iomanip>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <limits>
#include <numeric>
#include "toml.hpp"
#include <ctime>
#include <vector>
#include <windows.h>
using namespace std;

//// TERMINAL ////
void clearBI() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool isPrintable(const int k) {return k >= 32 && k <= 126;}

void clear(){system("cls");}

void flushKeys(){while (_kbhit()) getch();}

void enableUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

int keyPress() {return _getch();}

//// LOGIC ////

struct settings {
    char key1;
    char key2;

    int TPB;
};

struct URandConsis {
    float UR;
    float Consistency;
};

struct result {
    string mode;
    float bpm;
    float ur;
    float consistency;
    int score;
    int taps;
    float duration;
};

static string currentTimeStamp() {
    time_t t = time(nullptr);
    tm lt{};
    localtime_s(&lt, &t);
    char buf[32]{0};
    strftime(buf, sizeof(buf), "%m-%d-%Y %H:%M:%S", &lt);
    return buf;
}

static bool appendResult(const result &r, const string& path = "results.csv") {
    error_code ec;
    const bool exists = filesystem::exists(path, ec);
    ofstream ofs(path, ios::app);
    if (!ofs) return false;

    if (!exists) {
        ofs << "[timestamp]    [mode], [bpm], [ur], [consistency], [score], [taps], [duration]\n\n";
    }

    ios::fmtflags f = ofs.flags();
    streamsize p = ofs.precision();

    ofs << currentTimeStamp() << "  "
    << r.mode << ','
    << fixed << setprecision(2) << r.bpm << ','
    << fixed << setprecision(2) << r.ur << ','
    << fixed << setprecision(2) << r.consistency << ','
    << r.score << ','
    << r.taps << ','
    << fixed << setprecision(3) << r.duration << '\n';

    ofs.flags(f);
    ofs.precision(p);
    return true;
}

bool saveSettings(const settings& s, const string& path = "config.toml") {
    try {
        toml::table tbl{
            {"input", toml::table{
                {"keys", toml::array{
                    string(1, s.key1),
                        string(1, s.key2)
            }}
            }},
            {"advanced", toml::table{
                    {"TPB", s.TPB}
            }}
    };

        const string tmp = path+".tmp";
        {
            ofstream ofs(tmp, ios::trunc);
            if (!ofs) return false;
            ofs << tbl;
            ofs.flush();
            if (!ofs) return false;
        }

        error_code ec;
        filesystem::rename(tmp, path, ec);
        if (ec) {
            filesystem::remove(path, ec);
            filesystem::rename(tmp, path, ec);
            if (ec) return false;
        }
        return true;
    } catch (...) {
        return false;
    }
}

URandConsis findURandConsis(const vector<float> & times) {
    URandConsis ret;
    const float sum = accumulate(times.begin(), times.end(), 0.0);
    const float mean = sum / times.size();

    float variance = 0.0;
    for (float x : times) {
        variance += (x - mean) * (x - mean);
    }
    variance /= times.size();

    const float stddev = sqrt(variance);
    ret.UR = stddev * 10000;
    ret.Consistency = 100 / (1+(stddev/mean));
    return ret;
}

settings config;
void parseSettings() {
    try {
        toml::table tbl = toml::parse_file("config.toml");

        auto *keys = tbl["input"]["keys"].as_array();
        if (!keys || keys->size() < 2) {
            throw runtime_error("config.input.keys must have at least 2 entries");
        }

        auto k1 = (*keys)[0].value<string>().value_or("z");
        auto k2 = (*keys)[1].value<string>().value_or("x");
        if (k1.size() < 1 || k2.size() < 1) {
            throw runtime_error("config.input.keys must be a non empty char");
        }

        config.key1 = k1[0];
        config.key2 = k2[0];
        config.TPB = tbl["advanced"]["TPB"].value<int>().value_or(4);
    }
    catch (toml::parse_error & error) {
        cerr << "parsing Failed: " << error << "\n";
        exit(1);
    }

}

int streamSkill(const float bpm, const float consistency, const int taps) {
    //normalize consistency
    const float c = clamp(consistency / 100.0f, 0.0f, 1.0f);

    constexpr float bpmRef = 180.0f; //"medium" difficulty
    constexpr float speedEmphasis = 1.2f; // >1 rewards speed
    constexpr float consisEmphasis = 2.0f; //punish inconsistency

    const float len = sqrt(max(1, taps)); //prevents longer session domination
    constexpr float cap = 25.0f; //caps score from taps
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

vector<float> stream(string mode, int tapInput, int & tapAmt, int seconds, int BPM) {
    flushKeys();
    vector<float> times;
    vector<float> bpm;
    auto start = chrono::high_resolution_clock::now();
    auto last = start;
    while (true) {
        int tap = keyPress();
        if (tap == tolower(config.key1) || tap == tolower(config.key2)) {
            tapAmt++;
            auto now = chrono::high_resolution_clock::now();
            chrono::duration<float> diff = now - last;
            last = now;
            cout << static_cast<char>(tap) << " : " << diff.count() << "\n";
            chrono::duration<float> timeEla = last - start;

            if (tapAmt > 1) {
                times.push_back(diff.count());
                float currentBPM = findBPM(times, 4);
                bpm.push_back(currentBPM);
                if (mode == "endurance") {
                    if (currentBPM < BPM) {
                        if (static_cast<int>(timeEla.count()) > 3) {
                            cout << "\n You lasted " << static_cast<float>(timeEla.count()) << " seconds streaming at or above "<< BPM << "\n";
                            break;
                        }
                    }

                }
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

static char configPromptForKey(const char* label, char current, char other) {
    while (true) {
        cout << label << "press a key; Esc to cancel, current: '" << current << "': ";
        flushKeys();
        int k = keyPress();
        if (k == 27) return current;
        if (!isPrintable(k)) {
            cout << "Not a printable key. Try Again.\n";
            clearBI();
            Sleep(1000);
            clear();
            continue;
        }
        cout << static_cast<char>(k) << "\n";
        char c = static_cast<char>(tolower(k));
        if (c == tolower(other)) {
            cout << "Key is Already in Use. Choose a Different Binding.\n";
            clearBI();
            Sleep(500);
            clear();
            continue;

        }
        return c;
    }
}

static int configPromptForTPB(int current) {
    while (true) {
        cout << "Enter Taps-Per-Beat (TPB) [1-8] (current " << current << "): ";
        int t;
        if (cin >> t) {
            if (t >= 1 && t<= 8) return t;
        } else {
            clearBI();
        }
        cout << "Invalid Value. Enter value between 1-8\n";
        Sleep(1000);
        clear();
    }
}

void startOption(int TPB) {
    int tapInput = 0;
    int tapAmt = 0;
    float BPM = 0;
    int seconds = 0;
    while (tapInput == 0) {
        cout << "       BPM TEST\n Enter desired amount of key presses (20 - 1000): ";
        cin >> tapInput;
        if (1000 >= tapInput && tapInput >= 20) {
            cout << "\n\n BPM test, up to " << tapInput << ". Key presses start in 3..."; Sleep(1000);
            cout << "2..."; Sleep(1000); cout << "1... "; Sleep(1000); cout << "GO!\n\n";

            const vector<float> times = stream("normal", tapInput, tapAmt, seconds, BPM);

            BPM = findBPM(times, TPB);
            URandConsis data= findURandConsis(times);

            cout << "\n\nBPM: " << BPM << " \nUR: " << data.UR <<"\nConsistency: " << data.Consistency << "%";
            int score = streamSkill(BPM, data.Consistency, tapInput);
            float duration = accumulate(times.begin(), times.end(), 0.0);
            appendResult(result{"normal", BPM, data.UR, data.Consistency, score, tapInput, duration});
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
    int tapInput = 0;
    int tapAmt = 0;
    float BPM = 0;
    while (timeInput == 0) {
        cout << "       Timed Mode! Enter desired time (5 - 600s): ";
        cin >> timeInput;
        if (600 >= timeInput && timeInput >= 5) {
            cout << "\n\n" << timeInput << " second BPM test starts in 3..."; Sleep(1000); cout << "2...";
            Sleep(1000); cout << "1... "; Sleep(1000); cout << "GO!\n\n";

            const vector<float> times = stream("timed", tapInput, tapAmt, timeInput, BPM);

            BPM = findBPM(times, TPB);
            URandConsis data= findURandConsis(times);

            cout << "\n\nBPM: " << BPM << " \nUR: " << data.UR <<"\nConsistency: " << data.Consistency << "%";
            int score = streamSkill(BPM, data.Consistency, tapAmt);
            float duration = accumulate(times.begin(), times.end(), 0.0);
            appendResult(result{"timed",BPM, data.UR, data.Consistency, score, tapAmt, duration});
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

void enduranceMode(int TPB) {
    int tapAmt = 0;
    int bpmInput = 0;
    int tapInput = 0;
    int timeInput = 0;
    while (bpmInput == 0) {
        cout << "Endurance Mode! - Hold your desired BPM as Long as possible\nEnter BPM to hold (50 - 300 BPM): ";
        cin >> bpmInput;
        if (300 >= bpmInput && bpmInput >= 50) {
            cout << "\n\n" << bpmInput << " BPM Endurance test starts in 3..."; Sleep(1000); cout << "2...";
            Sleep(1000); cout << "1... "; Sleep(1000); cout << "GO!\n\n";

            const vector<float> times = stream("endurance", tapInput, tapAmt, timeInput, bpmInput);

            float BPM = findBPM(times, TPB);
            URandConsis data= findURandConsis(times);

            cout << "\n\nBPM: " << BPM << " \nUR: " << data.UR <<"\nConsistency: " << data.Consistency << "%";
            int score = streamSkill(BPM, data.Consistency, tapAmt);
            float duration = accumulate(times.begin(), times.end(), 0.0);
            appendResult(result{"endurance",BPM, data.UR, data.Consistency, score, tapAmt, duration});
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
        bpmInput = 0;
    }
}

void printRuns(const string& path = "results.csv") {
    ifstream file(path);

    if (!file.is_open()) {
        cout << "No runs captured. Use options 1-3 to view previous runs.\nExiting...";
        Sleep(1500);
        return;
    }

    string line;
    while (getline(file, line)) {
        cout << line << '\n';
    }
    file.close();
    cout << "\n\n Press Enter to Exit...";
    flushKeys();
    clearBI();
    Sleep(1000);
    cin.get();
}

void settingsEditor() {
    bool done = false;
    while (!done) {
        clear();
        cout << "======= Settings =======\n";
        cout << "Current key 1: '" << config.key1 << "'\n";
        cout << "Current key 2: '" << config.key2 << "'\n";
        cout << "Current TPB: '" << config.TPB << "'\n";
        cout << "1) Rebind Keys\n";
        cout << "2) Rebind TPB\n";
        cout << "3) Save & Return\n";
        cout << "4) Discard & Return\n";
        cout << "5) Restore Default\n";
        cout << "Choose an Option: ";

        int choice = 0;
        if (!(cin >> choice)){clearBI(); cout << "Invalid Option"; Sleep(1000); clear(); continue;}

        switch (choice) {
            case 1: {
                clear();
                cout << "Rebinding Keys...\n";
                char newk1 = configPromptForKey("New key1: ", config.key1, config.key2);
                char newk2 = configPromptForKey("New key2: ", config.key2, newk1);
                config.key1 = newk1;
                config.key2 = newk2;
                cout << "Keys set to '" << config.key1 << "' and '" << config.key2 << "'\n";
                Sleep(1000);
                break;
            }
            case 2: {
                clear();
                cout << "ADVANCED USERS ONLY!\nChanging TPB...\n";
                config.TPB = configPromptForTPB(config.TPB);
                cout << "TPB set to '" << config.TPB << "'\n";
                Sleep(1000);
                break;
            }
            case 3: {
                clear();
                cout << "Saving Settings...\n";
                if (saveSettings(config)) {
                    cout << "Saved Settings. Reloading Config...\n";
                    parseSettings();
                    cout << "Done. \n";
                } else {
                    cout << "Failed to Save Settings. (File permission or path issue?).\n";
                }
                Sleep(1000);
                done = true;
                break;
            }
            case 4: {
                clear();
                cout << "Discarding Settings...\n";
                parseSettings();
                Sleep(1000);
                done = true;
                break;
            }
            case 5: {
                clear();
                cout << "Restoring Default Settings...\n";
                config.key1 = 'z';
                config.key2 = 'x';
                config.TPB = 4;
                if (saveSettings(config)) {
                    cout << "Saved Settings. Reloading Settings...\n";
                    parseSettings();
                    cout << "Done. \n";
                } else { cout << "Failed to Save Settings... Verify config.toml integrity.\n";}

                Sleep(1000);
                done = true;
                break;
            }
            default:
                clear();
                cout << "Invalid option\n";
                Sleep(1000);
                clearBI();
                clear();
                break;
        }

    }
}




int main() {
    enableUTF8();
    SetConsoleTitle("BEEPM");
    parseSettings();
    clear();
    bool loop = true;
    int option = 0;
    vector<int> options = {1,2,3,4,5,6};


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
        cout << "\n\n1) Start\n2) Timed Mode\n3) Endurance Mode\n4) Previous Scores\n5) Settings\n6) Exit\nChoose an option: ";
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
            default:
                cout << "Invalid option, please try again.\n";
                Sleep(500);
                clearBI();
                option = 0;
                clear();
                break;

            case 1:
                clear();
                startOption(config.TPB);
                break;

            case 2:
                clear();
                timedMode(config.TPB);
                break;

            case 3:
                clear();
                enduranceMode(config.TPB);
                break;

            case 4:
                clear();
                printRuns();
                break;

            case 5:
                clear();
                settingsEditor();
                break;

            case 6:
                clear();
                cout << "Exiting...";
                loop = false;
                break;
        }
    }
    return 0;
}