/*****************************************************************************
TITLE: Assembler                                                                                                                             
AUTHOR: ADITYA SHARMA
ROLL NUMBER: 2301AI01
Declaration of Authorship
This C++ file, asm.cpp, is part of the assignment of CS2102 at the 
department of Computer Science and Engineering, IIT Patna. 
*****************************************************************************/

#include <bits/stdc++.h>
#define map unordered_map
#define set unordered_set
using namespace std;

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"


// mnemonic_with_Opcode
map<string, int> opcodeTable = {
    {"ldc", 0}, {"adc", 1}, {"ldl", 2}, {"stl", 3}, {"ldnl", 4}, {"stnl", 5}, {"add", 6}, {"sub", 7}, {"shl", 8}, 
    {"shr", 9}, {"adj", 10}, {"a2sp", 11}, {"sp2a", 12}, {"call", 13}, {"return", 14}, {"brz", 15}, {"brlz", 16}, 
    {"br", 17}, {"HALT", 18},};

// Mnemonics with Operand --> offset or value
map<string, int> opcodeTableOperand = {
    {"ldc", 0}, {"adc", 1}, {"ldl", 2}, {"stl", 3}, {"ldnl", 4}, {"stnl", 5}, {"adj", 10}, {"call", 13}, 
    {"brz", 15}, {"brlz", 16}, {"br", 17}};

// Mnemonics without Operand
map<string, int> opcodeTableNoOperand = {
    {"add", 6}, {"sub", 7}, {"shl", 8}, {"shr", 9}, {"a2sp", 11}, {"sp2a", 12}, {"return", 14}, {"HALT", 18}};

map<string, int> label_with_pc;
set<string> labelStored;
set<string> usedLabels;
int programCounter = 0;
bool foundHalt = false;
ofstream errorfile("error.log");

string removeComments(const string &line)
{
    return line.substr(0, line.find(';'));
}

string removeSpaces(const string &str)
{
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


string decToHex(int num, bool isData = false)
{
    unsigned int value = static_cast<unsigned int>(num); // Handle negative numbers
    string hexStr = "";
    const string hexChars = "0123456789abcdef";

    do {
        hexStr = hexChars[value % 16] + hexStr;
        value /= 16;
    } while (value > 0);

    // Ensure the result is padded to 8 characters
    while (hexStr.length() < 8)
        hexStr = "0" + hexStr;

    return hexStr;
}

bool isOctal(string s){
	if(s.size() < 2)return false ;
	bool ok = true ;
	ok &= (s[0] == '0') ;
	for(int i = 1 ; i < s.size() ; i++){
		ok &= (s[i] >= '0' && s[i] <= '7') ;
	}
	return ok ;
}

bool isHex(string s){
	bool ok = true ;
	if(s.size() < 3)return false ;
	ok &= (s[0] == '0') ;
	ok &= (s[1] == 'x') ;
	for(int i = 2 ; i < s.size() ; i++){
		bool check = (s[i] >= '0' && s[i] <= '9') ;
		check |= (s[i] >= 'a' && s[i] <= 'f') ;
		ok &= check ;
	}
	return ok ;
}

string otaltoDec(string s){
	int ans = 0 , x = 1;
	for(int i = s.size() - 1 ; i > 0 ; i--){
		ans += (s[i] - '0') * x ;
		x *= 8 ; 
	}
	string anss=to_string(ans);
    return anss ;
}

string HextoDec(string s){
    int ans = 0 , x = 1;
    for(int i = s.size() - 1 ; i > 1 ; i--){
        if(s[i] >= '0' && s[i] <= '9'){
            ans += (s[i] - '0') * x ;
        }else{
            ans += (s[i] - 'a' + 10) * x ;
        }
        x *= 16 ; 
    }
    string anss=to_string(ans);
    return anss ;
}


string getSubstring(const string &str, int start_position)
{
    return str.substr(start_position);
}

bool isValidInteger( string s)
{
    if (s.empty())
        return false;

    if (s[0] == '-' || s[0] == '+')
    {
        if (s.size() == 1)  return false;
        s.erase(s.begin());
    }
    if(s[0]=='0')
    {
        return (s.size() == 1);
    }
    for (size_t i = 0; i < s.length(); i++)
    {
        if (s[i] < '0' || s[i] > '9')
            return false;
    }
    return true;
}

bool isValidLabel(const string &label)
{
    if (label.empty() || !isalpha(label[0]))
        return false;
    for (size_t i = 1; i < label.size(); ++i)
    {
        if (!isalnum(label[i]))
            return false;
    }
    return true;
}

bool checkExtraToken(string &line)
{
    istringstream ss(line);
    string a, b, c;
    ss >> a >> b >> c;
    return !c.empty();
}

void firstPass(const string &filename)
{
    freopen(filename.c_str(), "r", stdin);
    freopen("list1.txt", "w", stdout);
    string line;
    string value;
    int SETvalue;
    int line_number = 1;

    while (getline(cin, line))
    {
        line = removeSpaces(removeComments(line));
        if (line == "")
        {
            line_number++;
            continue;
        }

        size_t labelEnd = line.find(':');
        if (labelEnd != string::npos)
        {
            string label = line.substr(0, labelEnd);
            if (!isValidLabel(label))
            {
                errorfile << RED << "Error : Line : " << line_number << " Invalid label : " << label << " " << RESET  << endl;
                line_number++;
                continue;
            }
            if (labelStored.find(label) != labelStored.end())
            {
                errorfile << RED << "Error : Line : " << line_number << " Repeated label : " << label << " " << RESET << endl;
                line_number++;
                continue;
            }
            label_with_pc[label] = programCounter;
            labelStored.insert(label);
            line = getSubstring(line, labelEnd + 1);
            line = removeSpaces(line);

            bool set = false;
            if (line.substr(0, 3) == "SET")
            {
                set = true;
                value = line.substr(line.find(' ') + 1);
                line = line.substr(0, line.size() - value.size() - 1);
                try
                {
                    SETvalue = stoi(value);
                    label_with_pc[label] = SETvalue;
                    line = "";
                }
                catch (...)
                {
                    cerr << RED << "Error: value not integer" << RESET << endl;
                }
            }
            if (set == false)
                cout << decToHex(programCounter) << " " << label << endl;
            else
                cout << decToHex(label_with_pc[label]) << " " << label << endl;
        }

        line = removeSpaces(line);

        if (checkExtraToken(line))
        {
            string errorMsg = "Error : Extra Token in Line on " + decToHex(programCounter);
            errorfile << RED << errorMsg << RESET << endl;
        }

        string mnemonic = line.substr(0, line.find(' '));
        if (opcodeTable.count(mnemonic) > 0)
        {
            cout << decToHex(programCounter) << " " << line << endl;
            programCounter++;
        }
        else if (mnemonic == "data")
        {
            string value = line.substr(line.find(' ') + 1);

            if(isOctal(value))
            {
                value=otaltoDec(value);
            }
            else if(isHex(value))
            {
                value=HextoDec(value);
            }

            if (!isValidInteger(value))
            {
                cerr << RED << "Error : Line : " << line_number << " Invalid Operand Format " << programCounter << RESET << endl;
                line_number++;
                continue;
            }
            cout << decToHex(programCounter) << " data " << value << endl;
            programCounter++;
        }
        else if (!line.empty())
        {
            errorfile << RED << "Error : Line : " << line_number << " Unknown mnemonic found at : " << decToHex(programCounter) << RESET << endl;
        }
    }
    fclose(stdin);
}

void secondPass(const string &Infile)
{
    ifstream infile(Infile);
    ofstream listfile("list2.txt");
    ofstream objfile("obj.o");
    ofstream errorfile("error.log", ios::app);

    streambuf *original_cerr = cerr.rdbuf();
    cerr.rdbuf(errorfile.rdbuf());

    string line;
    while (getline(infile, line))
    {
        string program_Counter = line.substr(0, 8);
        string instruction = removeSpaces(getSubstring(line, 8));

        if (instruction.empty())
        {
            listfile << line << endl;
            continue;
        }

        if (label_with_pc.find(instruction) != label_with_pc.end())
        {
            listfile << line << endl;
            continue;
        }

        string mnemonic = instruction;
        string operand = "";
        size_t space_pos = instruction.find(' ');
        if (space_pos != string::npos)
        {
            mnemonic = instruction.substr(0, space_pos);
            operand = removeSpaces(instruction.substr(space_pos + 1));
        }

        if (mnemonic == "HALT")
        {
            foundHalt = true;
        }
        else if (foundHalt)
        {
            cerr << RED << "Error : Unreachable Code :  Code after HALT at " << program_Counter << RESET << endl;
        }

        if (mnemonic == "br" || mnemonic == "brz" || mnemonic == "brlz" || mnemonic == "call")
        {
            if (label_with_pc.find(operand) != label_with_pc.end())
            {
                if (label_with_pc[operand] == stoi(program_Counter, nullptr, 16))
                {
                    cerr << YELLOW << "Warn : infinite Loop Detected at " << program_Counter << RESET << endl;
                }
            }
            else if (isValidInteger(operand))
            {
                if (stoi(operand) == stoi(program_Counter, nullptr, 16))
                {
                    cerr << YELLOW << "Warn : infinite Loop Detected at" << program_Counter << RESET << endl;
                }
            }
            else if(isOctal(operand))
            {
                operand=otaltoDec(operand);
                if (stoi(operand) == stoi(program_Counter, nullptr, 16))
                {
                    cerr << YELLOW << "Warn : infinite Loop Detected at" << program_Counter << RESET << endl;
                }
            }
            else if(isHex(operand))
            {
                operand=HextoDec(operand);
                if (stoi(operand) == stoi(program_Counter, nullptr, 16))
                {
                    cerr << YELLOW << "Warn : infinite Loop Detected at" << program_Counter << RESET << endl;
                }
            }
        }

        string machine_code;

        if (mnemonic == "data")
        {
            if (operand.empty())
            {
                cerr << RED << "Error : Missing Operand : for data at " << program_Counter << RESET << endl;
                continue;
            }

            try
            {
                // int value = stoi(operand);
                // machine_code = decToHex(value);
                int value=stoi(operand);
                string hexi=decToHex(value);
                hexi= hexi.substr(2,6);
                hexi+="ff";
                machine_code=hexi;

                listfile << program_Counter << " " << machine_code << " data " << operand << endl;
                objfile << machine_code << endl;
                continue;
            }
            catch (...)
            {
                cerr << RED << "Error : Invalid Operand Format : Invalid data value at " << program_Counter << RESET << endl;
                continue;
            }
        }

        if (opcodeTable.find(mnemonic) != opcodeTable.end())
        {
            int opcode = opcodeTable[mnemonic];
            stringstream ss;
            ss << setfill('0') << setw(2) << nouppercase << hex << opcode;
            string opcode_hex = ss.str();
            // cout<<opcode_hex << endl;
            string operand_hex;
            if (operand.empty())
            {
                if (opcodeTableOperand.find(mnemonic) != opcodeTableOperand.end())
                {
                    cerr  << RED << "Error : Missing Operand Required operand missing at " << program_Counter << RESET << endl;
                    continue;
                }
                operand_hex = "000000";
            }
            else if (!operand.empty() && opcodeTableNoOperand.find(mnemonic) != opcodeTableNoOperand.end())
            {
                cerr  << RED  << "Error : Unexpected Operand : Unexpected operand at " << program_Counter   << RESET << endl;
                continue;
            }
            else if (isValidInteger(operand))
            {
                int value = stoi(operand);
                if (mnemonic == "ldc" || mnemonic == "adc" || mnemonic == "adj" ||
                    mnemonic == "ldl" || mnemonic == "stl" || mnemonic == "ldnl" ||
                    mnemonic == "stnl")
                {
                    machine_code = decToHex(value);
                    operand_hex = machine_code.substr(2, 6);
                }
                else
                {
                    // Calculate relative address for branch instructions
                    int relative_addr = value - stoi(program_Counter, nullptr, 16);
                    machine_code = decToHex(relative_addr);
                    operand_hex = machine_code.substr(2, 6);
                }
            }
            else if(isOctal(operand))
            {
                operand=otaltoDec(operand);
                int value = stoi(operand);
                if (mnemonic == "ldc" || mnemonic == "adc" || mnemonic == "adj" ||
                    mnemonic == "ldl" || mnemonic == "stl" || mnemonic == "ldnl" ||
                    mnemonic == "stnl")
                {
                    machine_code = decToHex(value);
                    operand_hex = machine_code.substr(2, 6);
                }
                else
                {
                    // Calculate relative address for branch instructions
                    int relative_addr = value - stoi(program_Counter, nullptr, 16);
                    machine_code = decToHex(relative_addr);
                    operand_hex = machine_code.substr(2, 6);
                }
            }
            else if(isHex(operand))
            {
                operand=HextoDec(operand);
                int value = stoi(operand);
                if (mnemonic == "ldc" || mnemonic == "adc" || mnemonic == "adj" ||
                    mnemonic == "ldl" || mnemonic == "stl" || mnemonic == "ldnl" ||
                    mnemonic == "stnl")
                {
                    machine_code = decToHex(value);
                    operand_hex = machine_code.substr(2, 6);
                }
                else
                {
                    // Calculate relative address for branch instructions
                    int relative_addr = value - stoi(program_Counter, nullptr, 16);
                    machine_code = decToHex(relative_addr);
                    operand_hex = machine_code.substr(2, 6);
                }
            }
            else if (label_with_pc.find(operand) != label_with_pc.end())
            {
                usedLabels.insert(operand);
                int addr = label_with_pc[operand];
                if (mnemonic == "ldc" || mnemonic == "adc" || mnemonic == "adj" ||
                    mnemonic == "ldl" || mnemonic == "stl" || mnemonic == "ldnl" ||
                    mnemonic == "stnl")
                {
                    machine_code = decToHex(addr);
                    operand_hex = machine_code.substr(2, 6);
                }
                else
                {
                    int relative_addr = addr - stoi(program_Counter, nullptr, 16);
                    machine_code = decToHex(relative_addr);
                    operand_hex = machine_code.substr(2, 6);
                }
            }
            else
            {
                cerr   << RED << "Error : Undefined Label : Label '" << operand << "' not found at " << program_Counter   << RESET << endl;
                continue;
            }

            machine_code = operand_hex + opcode_hex;
            listfile << program_Counter << " " << machine_code << " " << instruction << endl;
            objfile << machine_code << endl;
        }
        else
        {
            cerr   << YELLOW << "Error : Unknown Instruction : Invalid instruction '" << mnemonic << "' at " << program_Counter   << RESET << endl;
            continue;
        }
    }

    if (!foundHalt)
    {
        cerr   << YELLOW << "Warn : Halt Not Found : No HALT instruction in the program"   << RESET << endl;
    }

    for (const auto &label : label_with_pc)
    {
        if (usedLabels.find(label.first) == usedLabels.end())
        {
            cerr  << YELLOW << "Warn : Unused label : Label '" << label.first << "' is defined but never used"   << RESET << endl;
        }
    }

    cerr.rdbuf(original_cerr);

    infile.close();
    listfile.close();
    objfile.close();
    errorfile.close();
    ifstream check_errors("error.log");
    string error_content((istreambuf_iterator<char>(check_errors)), istreambuf_iterator<char>());
    check_errors.close();

    if (!error_content.empty())
    {
        cout   << GREEN << "Assembly completed with errors or warnings. Check error.log for details."   << RESET << endl;
    }
    else
    {
        cout   << GREEN << "Assembly completed successfully."   << RESET << endl;
    }
}

int main()
{
    ofstream logFile("error.log");
    string filename;
    // string filename="input" ;
    cout << GREEN << "\nEnter the name of the input asm file : "  << RESET ;
    cin >> filename;
    // filename+=".asm";
    cout<<GREEN<<"filename : " << filename<<RESET<<'\n'<<endl;
    firstPass(filename);
    secondPass("list1.txt");
    return 0;
    cerr.rdbuf(logFile.rdbuf());
}