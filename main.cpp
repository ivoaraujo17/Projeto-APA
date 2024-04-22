#include <iostream>
#include <vector>
#include "read_arquivo.h"
#include "a_guloso.h"
#include "VND.h"
#include "solucao.h"


void print_capacidade_ocupacao(Read_Arquivo* entrada, Solucao* solucao){
    cout << "Capacidade: ";
    for (int i = 0; i < entrada->get_n_servidores(); i++){
        cout << entrada->get_capacidade_servidor(i) << " ";
    }
    cout << endl;
    cout << "Ocupacao:   ";
    for (int i = 0; i < entrada->get_n_servidores(); i++){
        cout << solucao->ocupacao[i] << " ";
    }
    cout << endl;
}



int main(){
    /*
    Read_Arquivo entrada("entrada.txt");
    A_Guloso guloso(&entrada);
    guloso.executar();
    guloso.solucao.print_solucao();
    cout << "Capacidade: ";
    for (int i = 0; i < entrada.get_n_servidores(); i++){
        cout << entrada.get_capacidade_servidor(i) << " ";
    }
    cout << endl;
    cout << "Ocupacao:   ";
    for (int i = 0; i < entrada.get_n_servidores(); i++){
        cout << guloso.solucao.ocupacao[i] << " ";
    }
    cout << endl;

    VND vnd(&guloso.solucao, &entrada, &guloso.jobs_ordenados);
    Solucao nova =  vnd.executar();
    nova.print_solucao();
    */
    
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
    print_capacidade_ocupacao(&arq1, &guloso.solucao);

    VND vnd(&guloso.solucao, &arq1, &guloso.jobs_ordenados);
    Solucao nova =  vnd.executar();
    nova.print_solucao();
    print_capacidade_ocupacao(&arq1, &nova);

    cout << endl;

    guloso = A_Guloso(&arq2);
    guloso.executar();
    guloso.solucao.print_solucao();
    print_capacidade_ocupacao(&arq2, &guloso.solucao);

    vnd = VND(&guloso.solucao, &arq2, &guloso.jobs_ordenados);
    nova =  vnd.executar();
    nova.print_solucao();
    print_capacidade_ocupacao(&arq2, &nova);

    cout << endl;

    guloso = A_Guloso(&arq3);
    guloso.executar();
    guloso.solucao.print_solucao();
    print_capacidade_ocupacao(&arq3, &guloso.solucao);

    vnd = VND(&guloso.solucao, &arq3, &guloso.jobs_ordenados);
    nova =  vnd.executar();
    nova.print_solucao();
    print_capacidade_ocupacao(&arq3, &nova);

    cout << endl;

    guloso = A_Guloso(&arq4);
    guloso.executar();
    guloso.solucao.print_solucao();
    print_capacidade_ocupacao(&arq4, &guloso.solucao);
    vnd = VND(&guloso.solucao, &arq4, &guloso.jobs_ordenados);
    nova =  vnd.executar();
    nova.print_solucao();
    print_capacidade_ocupacao(&arq4, &nova);

    cout << endl;

    guloso = A_Guloso(&arq5);
    guloso.executar();
    guloso.solucao.print_solucao();
    print_capacidade_ocupacao(&arq5, &guloso.solucao);
    vnd = VND(&guloso.solucao, &arq5, &guloso.jobs_ordenados);
    nova =  vnd.executar();
    nova.print_solucao();
    print_capacidade_ocupacao(&arq5, &nova);

    cout << endl;

    guloso = A_Guloso(&arq6);
    guloso.executar();
    guloso.solucao.print_solucao();
    print_capacidade_ocupacao(&arq6, &guloso.solucao);
    vnd = VND(&guloso.solucao, &arq6, &guloso.jobs_ordenados);
    nova =  vnd.executar();
    nova.print_solucao();
    print_capacidade_ocupacao(&arq6, &nova);

    return 0;

}