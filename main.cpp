#include <iostream>
#include <vector>
#include "read_arquivo.h"
#include "a_guloso.h"
#include "VND.h"
#include "solucao.h"


int main(){
    Read_Arquivo arq1("teste/n5m15A.txt");
    Read_Arquivo arq2("teste/n5m15B.txt");
    Read_Arquivo arq3("teste/n25m5A.txt");
    Read_Arquivo arq4("teste/n30m5A.txt");
    Read_Arquivo arq5("teste/n60m10.txt");
    Read_Arquivo arq6("teste/n60m10A.txt");

    //dados.print_matriz();
    A_Guloso guloso(&arq1);
    guloso.executar();
    guloso.solucao.print_solucao();
    VND vnd(&guloso.solucao, &arq1, &guloso.jobs_ordenados);
    Solucao nova =  vnd.executar();
    nova.print_solucao();

    cout << endl;

    guloso = A_Guloso(&arq2);
    guloso.executar();
    guloso.solucao.print_solucao();
    vnd = VND(&guloso.solucao, &arq2, &guloso.jobs_ordenados);
    nova =  vnd.executar();
    nova.print_solucao();

    cout << endl;

    guloso = A_Guloso(&arq3);
    guloso.executar();
    guloso.solucao.print_solucao();
    vnd = VND(&guloso.solucao, &arq3, &guloso.jobs_ordenados);
    nova =  vnd.executar();
    nova.print_solucao();

    cout << endl;

    guloso = A_Guloso(&arq4);
    guloso.executar();
    guloso.solucao.print_solucao();
    vnd = VND(&guloso.solucao, &arq4, &guloso.jobs_ordenados);
    nova =  vnd.executar();
    nova.print_solucao();

    cout << endl;

    guloso = A_Guloso(&arq5);
    guloso.executar();
    guloso.solucao.print_solucao();
    vnd = VND(&guloso.solucao, &arq5, &guloso.jobs_ordenados);
    nova =  vnd.executar();
    nova.print_solucao();

    cout << endl;

    guloso = A_Guloso(&arq6);
    guloso.executar();
    guloso.solucao.print_solucao();
    vnd = VND(&guloso.solucao, &arq6, &guloso.jobs_ordenados);
    nova =  vnd.executar();
    nova.print_solucao();
    return 0;

}