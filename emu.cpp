#include <bits/stdc++.h>
using namespace std;

//ANSI escape codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"
#define BG_BLACK "\033[40m"
#define BG_BLUE "\033[44m"
#define BG_CYAN "\033[46m"

int pc = 0, sp = 0, A = 0, B = 0;
int mem[100];
vector<string> hexCodes;

string formatNumber(long long num, int width, bool showSign = false) {
    stringstream ss;
    // Only show negative sign, remove the showSign parameter's effect for positive numbers
    if (num < 0) ss << num;
    else ss << abs(num);  // Use abs() to ensure positive numbers
    return string(max(0, width - (int)ss.str().length()), ' ') + ss.str();
}

void printSeparator(const string& title = "", int width = 70) {
    cout << YELLOW;
    if (title.empty()) {
        cout << "+" << string(width-2, '-') << "+";
    } else {
        int remaining = width - title.length() - 4;
        cout << "+" << string(remaining/2, '-') << " " 
             << WHITE << BOLD << title << RESET << YELLOW 
             << " " << string(remaining/2 + remaining%2, '-') << "+";
    }
    cout << RESET << "\n";
}

void printFooter(int width = 70) {
    cout << YELLOW << "+" << string(width-2, '-') << "+" << RESET << "\n";
}

// Utility functions remain the same
string removeSpaces(const string &str) {
    size_t first = 0;
    size_t last = str.length() - 1;
    while (first <= last && (str[first] == ' ' || str[first] == '\n' || str[first] == '\t' || str[first] == '\r'))
        first++;
    while (last >= first && (str[last] == ' ' || str[last] == '\n' || str[last] == '\t' || str[last] == '\r'))
        last--;
    if (first > last)
        return "";
    return str.substr(first, last - first + 1);
}

long long hexToDec(const string &hexValue) {
    string value = removeSpaces(hexValue);
    if (value.size() < 8 && value.front() == 'f') value.insert(0, "ff");
    unsigned int result = stoul(value, nullptr, 16);
    return result >= 0x80000000 ? static_cast<int>(result - 0x100000000) : static_cast<int>(result);
}

void loadHexCode(const string &path) {
    ifstream inFile(path);
    string line;
    int idx = 0;
    while (getline(inFile, line)) {
        hexCodes.push_back(line);
        if (line.substr(6, 2) == "ff") mem[idx++] = hexToDec(line.substr(0, 6));
    }
}

void printState(bool isFinal = false) {
    string prefix = isFinal ? "Final " : "";
    cout << BG_BLACK << WHITE << "| " << prefix 
         << GREEN << "PC: " << formatNumber(pc, 4) << WHITE << " | "
         << CYAN << "SP: " << formatNumber(sp, 4) << WHITE << " | "
         << MAGENTA << "A: " << formatNumber(A, 4) << WHITE << " | "
         << YELLOW << "B: " << formatNumber(B, 4) << WHITE << " |" 
         << RESET << "\n";
}

void logState(int step, int pc, int sp, int A, int B) {
    ofstream logFile("emu_log.txt", ios_base::app);
    logFile << "Step " << formatNumber(step, 4) << " | ";
    logFile << "PC: " << formatNumber(pc, 4) << " | ";
    logFile << "SP: " << formatNumber(sp, 4) << " | ";
    logFile << "A: " << formatNumber(A, 4) << " | ";
    logFile << "B: " << formatNumber(B, 4) << "\n";
    logFile.close();
}

void executeCode() {
    cout << "\n";
    printSeparator("Execution Trace");
    cout << BG_BLUE << WHITE << BOLD 
         << "   Step   |    Register States                              " 
         << RESET << "\n";
    
    int step = 0;
    
    while (true) {
        string opcodeHex = hexCodes[pc].substr(6, 2);
        string valueHex = hexCodes[pc].substr(0, 6);
        long long opcode = hexToDec(opcodeHex);
        long long value = hexToDec(valueHex);

        cout << BG_BLACK << WHITE << "   " 
             << formatNumber(step, 4) << "   ";
        printState();

        logState(step, pc, sp, A, B);
        step++;

        switch (opcode) {
            case 0: B = A; A = value; pc++; break;
            case 1: A += value; pc++; break;
            case 2: B = A; A = mem[sp + value]; pc++; break;
            case 3: mem[sp + value] = A; A = B; pc++; break;
            case 4: A = mem[A + value]; pc++; break;
            case 5: mem[A + value] = B; pc++; break;
            case 6: A += B; pc++; break;
            case 7: A = B - A; pc++; break;
            case 8: A = B << A; pc++; break;
            case 9: A = B >> A; pc++; break;
            case 10: sp += value; pc++; break;
            case 11: sp = A; A = B; pc++; break;
            case 12: B = A; A = sp; pc++; break;
            case 13: B = A; A = pc; pc += value; break;
            case 14: pc = A; A = B; pc++; break;
            case 15: if (A == 0) pc += value; else pc++; break;
            case 16: if (A < 0) pc += value; else pc++; break;
            case 17: pc += value; break;
            case 18: return;
            default: pc++; break;
        }

        if (pc >= hexCodes.size()) return;
    }
}

void printMemoryState() {
    cout << "\n";
    printSeparator("Memory State");
    cout << BG_CYAN << WHITE << BOLD 
         << "  Address  |    Value    |  Address  |    Value    " 
         << RESET << "\n";
    
    for (int i = 0; i < 16; i += 2) {
        cout << BG_BLACK << WHITE 
             << "  mem[" << formatNumber(i, 2) << "]  | " 
             << formatNumber(mem[i], 8) << "  |"
             << "  mem[" << formatNumber(i+1, 2) << "]  | " 
             << formatNumber(mem[i+1], 8) << "  " 
             << RESET << "\n";
    }
    
    printFooter();
    cout << "\n";
    printSeparator("Final Register States");
    printState(true);
    printFooter();
}

int main() {
    fill(begin(mem), end(mem), 0);
    
    cout << BOLD << WHITE << "\n+--- Emulator Execution ---+\n" << RESET;
    
    loadHexCode("obj.o");
    executeCode();
    printMemoryState();
    
    cout << BOLD << WHITE << "\n+--------------------------------------------------------------------+\n\n" << RESET;
    return 0;
    int n;
    cin >> n;
}