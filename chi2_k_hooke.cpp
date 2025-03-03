#include<iostream>
#include<cmath>
#include<vector>
#include<string>
#include<fstream>

using namespace std;

struct data {
    double s_x=0;
    double a=0;
    double b=0;
    vector <double> F;
    vector <double> x;
};

struct info {
    double res_sum=0;
    double cf=0;
    double chi2_ref=0;
};

void chi2f(string&, data&, info&);
void routinef(string&, data&, info&);


int main() {

    string file;
    string& filer = file;

    data dati;
    data& datir = dati;

    info res;
    info& resr = res;

    cout << "alfa = ";
    cin >> resr.cf;

    if(resr.cf==0.1) resr.chi2_ref = 13.362;
    else if(resr.cf==0.05) resr.chi2_ref = 15.507;
    else if(resr.cf==0.025) resr.chi2_ref = 17.535;
    else if(resr.cf==0.01) resr.chi2_ref = 20.090;
    else if(resr.cf==0.005) resr.chi2_ref = 21.955;
    else if(resr.cf==0.90) resr.chi2_ref = 3.490;
    else if(resr.cf==0.95) resr.chi2_ref = 2.733;
    else if(resr.cf==0.975) resr.chi2_ref = 2.180;
    else if(resr.cf==0.99) resr.chi2_ref = 1.646;
    else if(resr.cf==0.995) resr.chi2_ref = 1.344;

    routinef(filer, datir, resr);

    return 0;
}


void chi2f(string& filer, data& datir, info& resr) {
    resr.res_sum=0;
    for(int i=0;i<datir.x.size();i++) resr.res_sum+=pow(((datir.F[i]*4-(datir.a+datir.b*datir.x[i]))/datir.s_x),2);
}

void routinef(string &filer, data& datir, info& resr) {

    string skip_line;

    ofstream ofile("chi2_k_results.txt",ios::app);
    if(!ofile.is_open()) cout << "out file failed";

    ifstream stddevfile("mean_stddev_deltax.txt");
    if(!stddevfile.is_open()) cout << "stddev file failed";
    for(int i=0;i<2;i++) getline(stddevfile,skip_line);

    ifstream parafile("results_linear_regression.txt");
    if(!parafile.is_open()) cout << "parameters file failed";
    for(int i=0;i<53;i++) getline(parafile,skip_line);          //METTERE 8 O 53 SKIP IN BASE AD ANDATA O RITORNO

    for(int i=1; i<=5; i++) {
        datir.F.clear();
        datir.x.clear();
//                                                                                          QUI PER MODIFICARE NOME FILE
        filer = "k_hooke_-" + to_string(i) + ".txt";

        ifstream ifile(filer);
        if(!ifile.is_open()) cout << "ifile failed";
        getline(ifile,skip_line);
        getline(ifile,skip_line);
        double b=0, c=0;
        while(ifile >> b >> c) {
            datir.F.push_back(b);
            datir.x.push_back(c);
        }

        double a=0, d=0;

        //                                                                             INCERTEZZA LEFAGATA A x DA CAPIRE
        stddevfile >> a >> d;
        datir.s_x=8.167;        //PRIMA QUESTA PARTE SERVIVA PER LEGGERE L'INCERTEZZA IN ALLUNGAMENTO

        parafile >> datir.a;
        getline(parafile,skip_line);
        parafile >> datir.b;

        getline(stddevfile,skip_line);
        for(int k=0; k<8; k++) getline(parafile,skip_line);

        ifile.close();

        chi2f(filer, datir, resr);

        ofile << "-" << filer << endl;
        if(resr.chi2_ref>resr.res_sum) ofile << "il test del chi2 e' superato con CL = " << resr.cf << endl;
        if(resr.chi2_ref<resr.res_sum) ofile << "il test del chi2 non e' superato con CL = " << resr.cf << endl;
        ofile << "chi2 sper = " << resr.res_sum << "\tchi2 ref = " << resr.chi2_ref << endl << endl;
    }
}