#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Student {
    long num;       //номер зачетки
    char name[10];  //имя студента
    int group;      //номер группы
    double grade;   //средний бал

    friend bool operator<(const Student& first, const Student& second) {
        return first.num < second.num;
    } 
    
    const Student operator=(const Student& other) {
        if (this == &other) {
            return *this;
        }
        grade = other.grade;
        num = other.num;
        group = other.group;
        strcpy(name, other.name);
        return *this;
    }
};

int Menu() {
    int choise;
    cout << "Select option" << endl;
    cout << "1.Enter the name of the files" << endl;
    cout << "2.Write data to file" << endl;
    cout << "3.Read from file" << endl;
    cout << "4.Union" << endl;
    cout << "5.Intersection" << endl;
    cout << "6.Difference" << endl;
    cout << "7.Print binary file" << endl;
    cout << "8.Sort by Num" << endl;
    cout << "9.Sort by Group and Name" << endl;
    cin >> choise;
    return choise;
}

bool CompNum(const Student lhs, const Student rhs) { 
    return lhs.num < rhs.num; 
}

bool CompGroupAndName(const Student lhs, const Student rhs) {
    if (lhs.group < rhs.group) {
        return true;
    } else {
        if ((lhs.group == rhs.group) && (strcmp(lhs.name, rhs.name) == -1)) {
            return true;
        }
    }
    return false;
}

void OutStud(const vector<Student>& first, const string fname) {
    ofstream out(fname, ofstream::binary);
    if (!out.is_open()) {
        cerr << "Open file failed." << endl;
    }
    for (int i = 0; i < size(first); i++) {
        out.write((char*)&first[i], sizeof(Student));
    }
    out.close();
}

void SortStudNum(vector<Student>& first, const string& fname) {
    sort(first.begin(), first.end(), CompNum);
    OutStud(first, fname);
}

void SortStudGroupAndName(vector<Student>& first, const string& fname) {
    sort(first.begin(), first.end(), CompGroupAndName);
    OutStud(first, fname);
}

void InFNames(string& first, string& second) {
    cout << "Input first file name" << endl;
    getline(cin, first);
    // cin.sync();
    // cerr << "#" << first << endl;
    getline(cin, first);
    // cerr << "#" << first << endl;
    cout << "Input second file name" << endl;
    getline(cin, second);
    // cerr << "#" << second << endl;
}

void WriteFiles(const string& first, const string& second) {
    ofstream out;
    Student temp;

    out.open(first, ofstream::binary);
    if (!out.is_open()) {
        cerr << "Open file failed." << endl;
    }
    cout << "First file: Input student number, name, group and grade" << endl;
    cin >> temp.num >> temp.name >> temp.group >> temp.grade;

    while (!cin.eof()) {
        out.write((const char*)&temp, sizeof(struct Student));
        cin >> temp.num >> temp.name >> temp.group >> temp.grade;
    }
    cin.clear();
    out.close();

    out.open(second, ofstream::binary);
    if (!out.is_open()) {
        cerr << "Open file failed." << endl;
    }
    cout << "Second file: Input student number, name, group and grade" << endl;
    cin >> temp.num >> temp.name >> temp.group >> temp.grade;
    while (!cin.eof()) {
        out.write((const char*)&temp, sizeof(struct Student));
        cin >> temp.num >> temp.name >> temp.group >> temp.grade;
    }
    out.close();
    cin.clear();
}

void ReadFiles(const string ffirst, const string fsecond,
               vector<Student>& sfirst, vector<Student>& ssecond) {
    ifstream fin(ffirst, ios::binary);
    Student templ;
    sfirst.clear();
    ssecond.clear();
    
    while (fin.read((char*)&templ, sizeof(Student))) {
        sfirst.push_back(templ);
    }
    fin.close();

    ifstream sin(fsecond, ios::binary);
    while (sin.read((char*)&templ, sizeof(Student))) {
        ssecond.push_back(templ);
    }
    sin.close();

    cout << "Reading completed" << endl;
}

void UnionDate(vector<Student> sfirst, vector<Student> ssecond) {
    vector<Student> result;
    sort(sfirst.begin(), sfirst.end(), CompNum);
    sort(ssecond.begin(), ssecond.end(), CompNum);
    int i = 0, j = 0;
    while (i < sfirst.size() || j < ssecond.size()) {
        if (j == ssecond.size() ||
            (i < sfirst.size() && CompNum(sfirst[i], ssecond[j]))) {
            if (result.size() > 0 && result[result.size() - 1].num == sfirst[i].num) {
                i++;
                continue;
            }
            result.push_back(sfirst[i]);
            i++;
        }
        else {
            if (result.size() > 0 &&
                result[result.size() - 1].num == ssecond[j].num) {
                j++;
                continue;
            }
            result.push_back(ssecond[j]);
            j++;
        }
    }
    OutStud(result, "Out.bin");
}

void Intersection(vector<Student> sfirst, vector<Student> ssecond) {
    vector<Student> result;
    int i = 0, j = 0;
    sort(sfirst.begin(), sfirst.end(), CompNum);
    sort(ssecond.begin(), ssecond.end(), CompNum);
    while (i < sfirst.size() && j < ssecond.size()) {
        if (CompNum(sfirst[i],ssecond[j]))
            i++;
        else if (CompNum(ssecond[j],sfirst[i]))
            j++;
        else {
            if (result.empty() || result.back().num != sfirst[i].num) {
            result.push_back(sfirst[i]);
            }
            i++;
            j++;
        }
    }
    OutStud(result, "Out.bin");
}

void Difference(vector<Student>& sfirst, vector<Student>& ssecond) {
    vector<Student> result(sfirst.size() + ssecond.size());
    vector<Student>::iterator it;
    sort(sfirst.begin(), sfirst.end(), CompNum);
    sort(ssecond.begin(), ssecond.end(), CompNum);
    it = set_difference(sfirst.begin(), sfirst.end(), ssecond.begin(),
        ssecond.end(), result.begin());
    result.resize(it - result.begin());
    OutStud(result, "Out.bin");
}

void PrintBinary() {
    string fname;
    ifstream in;
    Student temp;

    cout << "Input binary file name" << endl;
    getline(cin, fname);
    getline(cin, fname);  // ????????

    in.open(fname);

    while (in.read((char*)&temp, sizeof(Student))) {
        cout << temp.num << " " << temp.name << " " << temp.group << " "
            << temp.grade << endl;
    }
    in.close();
    cout << "Press any key to continue";
    getchar();
}

int main() {
    vector<Student> sfirst;
    vector<Student> ssecond;
    string ffirst;
    string fsecond;

    while (true) {
        switch (Menu()) {
        case 1:
            InFNames(ffirst, fsecond);
            break;
        case 2:
            WriteFiles(ffirst, fsecond);
            break;
        case 3:
            ReadFiles(ffirst, fsecond, sfirst, ssecond);
            break;
        case 4:
            UnionDate(sfirst, ssecond);
            break;
        case 5:
            Intersection(sfirst, ssecond);
            break;
        case 6:
            Difference(sfirst, ssecond);
            break;
        case 7:
            PrintBinary();
            break;
        case 8: {
            int numfile;
            cout << "Choise file 1 or 2?" << endl;
            cin >> numfile;
            if (numfile == 1)
                SortStudNum(sfirst, ffirst);
            else
                SortStudNum(ssecond, fsecond);
        }; break;
        case 9: {
            int numfile;
            cout << "Choise file 1 or 2?" << endl;
            cin >> numfile;
            if (numfile == 1)
                SortStudGroupAndName(sfirst, ffirst);
            else
                SortStudGroupAndName(ssecond, fsecond);
        }; break;
        default:
            "Select the correct item!";
        }
        system("cls");
    }
}