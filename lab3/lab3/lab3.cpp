#include <fstream>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

class Proces {
public:
    vector<int> CzasyNaMaszynie;
    int Index;
    int TotalLength;
};

char str[3];

//---------------------------------------------------------------------------------------------------------------------------

int procTime(Proces proc) {
    int ret = 0;
    for (int i = 0; i < proc.CzasyNaMaszynie.size(); i++)
    {
        ret += proc.CzasyNaMaszynie[i];
    }
    return ret;
}
void readData(int number, std::vector<Proces>& processes, int& Cmax, vector<int>& order) {
    int L_proc;
    int L_maszyn;

    processes.clear();
    std::ifstream infile("C:\\Users\\pkubo\\OneDrive\\Dokumenty\\GitHub\\Politechnika\\S.P.D.\\lab3\\neh.data.txt");

    snprintf(str, 4, "%03d", number);
    string str_2 = string(str);
    std::string data_selector = "data." + str_2 + ":";
    std::string tmp;
    while (tmp != data_selector) {
        infile >> tmp;
    }
    
    infile >> L_proc;
    infile >> L_maszyn;
    
    for (int i = 0; i < L_proc; i++) {
        Proces p;
        for (int i = 0; i < L_maszyn; i++) {
            int tmp = -1;
            infile >> tmp;
            p.CzasyNaMaszynie.push_back(tmp);
        }
        p.Index = i+1;                              // +1 -> dla kolejnosci jak w txt
        p.TotalLength = procTime(p);
        processes.push_back(p);
    }

    data_selector = "neh:";
    while (tmp != data_selector) {
        infile >> tmp;
    }
    infile >> Cmax;
    for (int i = 0; i < L_proc; i++) {
        int tmp = -1;
        infile >> tmp;
        order.push_back(tmp);
    }
    infile.close();
}
void showData(vector<Proces> processes) {
    for (int i = 0; i < processes.size(); i++)
    {
        for (int j = 0; j < processes[i].CzasyNaMaszynie.size(); j++)
        {
            std::cout << processes[i].CzasyNaMaszynie[j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
}

long calcCmax(vector<Proces> processes) {
    int L_procesow = processes.size();
    int L_maszyn = processes[0].CzasyNaMaszynie.size();
    vector<vector<long>> Cmax;

    for (int i = 0; i < L_procesow; i++)
    {
        vector<long> tmp;
        for (int j = 0; j < L_maszyn; j++)
        {
            tmp.push_back(-1);
        }
        Cmax.push_back(tmp);
    }

    for (int i = 0; i < L_procesow; i++)
    {
        for (int j = 0; j < L_maszyn; j++)
        {
            long p_int_int = processes[i].CzasyNaMaszynie[j];

            if (i == 0 && j == 0){
                Cmax[i][j] = p_int_int;
            }
            else if (j == 0) {
                Cmax[i][j] = p_int_int + Cmax[i-1][j];
            }
            else if (i == 0) {
                Cmax[i][j] = p_int_int + Cmax[i][j-1];
            }
            else {
                Cmax[i][j] = p_int_int + max(Cmax[i][j - 1], Cmax[i - 1][j]);
            }
            //cout << Cmax[i][j] << " " ;
        }
        //cout << endl;
    }
    //cout << endl;
    //cout << Cmax[L_procesow - 1][Cmax[L_procesow - 1].size() - 1] << " " << endl;

    return Cmax[L_procesow-1][Cmax[L_procesow-1].size()-1];
}
bool isSameProc(Proces vec1, Proces vec2) {
    for (int i = 0; i < vec1.CzasyNaMaszynie.size(); i++)
    {
        if (vec1.CzasyNaMaszynie[i] != vec2.CzasyNaMaszynie[i])
        {
            return false;
        }
    }
    return true;
}

vector<int> getOrder(vector<Proces> goodProcesses) {
    vector<int> res;
    for (int i = 0; i < goodProcesses.size(); i++)
    {
        res.push_back(goodProcesses[i].Index);
    }
    return res;
}

void printVec(vector<int> res) {
    for (int i = 0; i < res.size(); i++)
    {
        snprintf(str, 3, "%02d", res[i]);
        string str_2 = string(str);
        cout << str_2 << " ";
    }
}
void printVec(vector<Proces> processes) {
    for (int i = 0; i < processes.size(); i++)
    {
        //cout << "Idx: " << processes[i].Index << " -- Total time: " << processes[i].TotalLength << "";
        for (int j = 0; j < processes[i].CzasyNaMaszynie.size(); j++)
        {
            std::cout << processes[i].CzasyNaMaszynie[j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
}
vector<Proces> sortProcByLength(vector<Proces> invec) {
    vector<Proces> ret;
    sort(invec.begin(), invec.end(), [](const Proces& lhs, const Proces& rhs)
    {
        return lhs.TotalLength > rhs.TotalLength;
    });
    return invec;
}





void CalcNEH(vector<Proces> processes, vector<int>& resOrder, vector<Proces>& res, int& finCmax) {
    processes = sortProcByLength(processes);

    for (int i = 0; i < processes.size(); i++)
    {
        long p_max = 0;
        long c_max = 999999;
        vector<Proces> tmp_vec(res);
        tmp_vec.push_back(processes[i]);
        //tmp_vec.insert(tmp_vec.begin() + i, processes[i]);

        for (int j = i; j >= 0; j--) {
            long curr = calcCmax(tmp_vec);
            if (c_max >= curr) {
                c_max = curr;
                p_max = j;
                res = tmp_vec;
            }
            if (j > 0) {
                swap(tmp_vec[j], tmp_vec[j - 1]);
            }
        }
    }
    finCmax = calcCmax(res);
    resOrder = getOrder(res);
}







void CalcNEH_origin(vector<Proces> processes, vector<int>& resOrder, vector<Proces>& res, int& finCmax) {
    finCmax = 99999;
    processes = sortProcByLength(processes);
    res.push_back(processes[0]);

    int ccc = 0;

    for (int i = 1; i < processes.size(); i++)
    {
        int tmp_index = 999999;
        long tmp_Cmax = 999999;
        Proces proc = processes[i];

        for (int j = 0; j < res.size() + 1; j++)                      // +1 zeby dodal tez na koniec
        {
            vector<Proces> tmp_vec(res);
            tmp_vec.insert(tmp_vec.begin() + j, proc);

            int ttmpCmax = calcCmax(tmp_vec);

            if (ttmpCmax < tmp_Cmax)
            {
                tmp_Cmax = ttmpCmax;
                finCmax = ttmpCmax;
                tmp_index = j;
            }

            if (ccc++ > 100)
            {
                //                return;
            }
        }
        res.insert(res.begin() + tmp_index, proc);
    }
    resOrder = getOrder(res);
}

bool testCmax(int myCmax, int corCmax, vector<int> corOrder, vector<int> myOrder) {
    return (myOrder == corOrder) && (corCmax == myCmax);
}

vector<Proces> testCmax_makeVec(vector<Proces> proc, vector<int> ord) {
    vector<Proces> res;
    for (int i = 0; i < ord.size(); i++)
    {
        res.push_back(proc[ord[i]-1]);
    }
    return res;
}

void test_Cmax() {
    for (int i = 0; i < 121; i++)
    {
        int cor_Cmax;
        vector<int> cor_order;
        vector<Proces> processes;

        readData(i, processes, cor_Cmax, cor_order);
        
        vector<Proces> resssss = testCmax_makeVec(processes, cor_order);

        int Cmax = calcCmax(resssss);


        if (Cmax != cor_Cmax)
        {
            cout << "corCmax: " << cor_Cmax << "\r\n";
            cout << "my Cmax: " << Cmax << "\r\n";
            cout << "\r\n------------------------------\r\n";
        }
        else {
            cout << "" << i << " - Cmax : ok\r\n";
        }
    }
}

int main()
{ 
    int number = 0;             // moj -> 17  // 1 -> 1448 // 0 -> 40

    //test_Cmax(); return 1;        //pass [O] - ok


    for (int i =43; i < 44; i++)
    {
        int Cmax;
        int cor_Cmax;
        vector<int> resOrder;
        vector<int> cor_order;
        vector<Proces> processes;
        vector<Proces> res;
        
        readData(i, processes, cor_Cmax, cor_order);

        CalcNEH(processes, resOrder, res, Cmax);

        if (!testCmax(Cmax,cor_Cmax, cor_order, resOrder))
        {
            cout << "corCmax: " << cor_Cmax << "\r\n";
            cout << "my Cmax: " << Cmax     << "\r\n";
            cout << "corVecOrd: ";
            printVec(cor_order);
            cout << "\r\n";
            cout << "my VecOrd: ";
            printVec(resOrder);
            cout << "\r\n------------------------------\r\n";
        }
        else {
            cout << ": " << i << " - Cmax : " << Cmax << "\r\n";
        }

        //return 1;
    }

    //std::cout << "Hello World!\n";
}
