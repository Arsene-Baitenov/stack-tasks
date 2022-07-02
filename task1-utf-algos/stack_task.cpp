#include <iostream>
#include <fstream>
#include<string>
#include<vector>

using namespace std;

class CSVRow {
    public:
        CSVRow() {
            row.clear();
            gaps.clear();
            gaps.push_back(-1);
        }
        CSVRow(string data) {
            setRow(data);
        }

        void setRow(string data) {
            row = data;
            gaps.clear();
            gaps.push_back(-1);

            int pos = 0;
            while ((pos = row.find(separator, pos)) != string::npos) {
                gaps.push_back(pos);
                ++pos;
            }
            gaps.push_back(row.length());
        }
        string getRow() {
            return row;
        }
        void clear() {
            row.clear();
            gaps.clear();
            gaps.push_back(-1);
        }

        string getField(int index) {
            if ((gaps.size() < 2) || (row.empty())) {
                throw exception("row is empty");
            }
            if ((index < 0) || (index > gaps.size() - 2)) {
                throw exception("index out of range");
            }
            return row.substr(gaps[index] + 1, gaps[index + 1] - gaps[index] - 1);
        }
        void addField(string data) {
            if (gaps.size() > 1) {
                row += separator;
            }
            row += data;
            gaps.push_back(row.length());
        }
    private:
        string row;
        vector<int> gaps;
        char separator = ';';
};

string dtos(double x) {
    string res = to_string(x).substr(0, 8);
    res[res.find('.')] = ',';
    return res;
}

int getHouseNumber(vector<CSVRow>& houses, vector<double>& charges, string street, string houseNumber) {
    int i = 0;
    while ((i < houses.size()) && !((houses[i].getField(1) == street) && (houses[i].getField(2) == houseNumber))) {
        ++i;
    }
    if (i == houses.size()) {
        CSVRow house;
        house.addField(to_string(i + 1));
        house.addField(street);
        house.addField(houseNumber);
        houses.push_back(house);
        charges.push_back(0);
    }
    return i;
}

int main()
{
    int Norm = 1;
    int Chet = 2;
    double stNorm = 301.26;
    double stChet = 1.52;

    ifstream in("абоненты.csv");
    ofstream out("Начисления_абоненты.csv");

    string s;
    CSVRow row;

    vector<CSVRow> houses;
    houses.clear();
    vector<double> charges;
    charges.clear();

    getline(in, s);
    row.setRow(s);
    row.addField("Accrued");
    out << row.getRow() << "\n";

    getline(in, s);

    while (!s.empty()) {
        row.setRow(s);

        int num = getHouseNumber(houses, charges, row.getField(2), row.getField(3));

        double acc;
        if (stoi(row.getField(5)) == Norm) {
            acc = stNorm;
        }
        else {
            int currData = stoi(row.getField(7));
            int prevData = stoi(row.getField(6));
            acc = stChet * (currData - prevData);
        }
        row.addField(dtos(acc));

        charges[num] += acc;

        out << row.getRow() << "\n";
        getline(in, s);
    }

    in.close();
    out.close();

    out.open("Начисления_дома.csv");

    row.clear();
    row.addField("Row number");
    row.addField("Street");
    row.addField("House number");
    row.addField("Accrued");
    out << row.getRow() << "\n";

    for (int i = 0; i < houses.size(); ++i) {
        houses[i].addField(dtos(charges[i]));
        out << houses[i].getRow() << "\n";
    }

    out.close();
}