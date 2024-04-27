#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip> // Para manipulação de largura de campo
#include "read_arquivo.h"
#include "a_guloso.h"
#include "VND.h"
#include "solucao.h"


void print_capacidade_ocupacao(Read_Arquivo* entrada, Solucao* solucao){
    cout << "Capacidade - Ocupacao: ";
    for (int i = 0; i < entrada->get_n_servidores(); i++){
        cout << entrada->get_capacidade_servidor(i) - solucao->ocupacao[i] << " ";
    }
    cout << endl;
}



int main(){

    Read_Arquivo arq1("teste/teste.txt");
    Read_Arquivo arq2("teste/n5m15B.txt");
    Read_Arquivo arq3("teste/n25m5A.txt");
    Read_Arquivo arq4("teste/n30m5A.txt");
    Read_Arquivo arq5("teste/n60m10.txt");
    Read_Arquivo arq6("teste/n60m10A.txt");

    
    vector<Read_Arquivo*> arquivos = {&arq1, &arq2, &arq3, &arq4, &arq5, &arq6};

    int otimos[6] = {261, 269, 438, 423, 954, 973};
    int Sguloso[6];
    int SVND[6];
    float GAP_guloso[6];
    float GAP_VND[6];
    float tempo_guloso[6];
    float tempo_VND[6];
    chrono::time_point<chrono::system_clock> inicio, fim;
    chrono::duration<float> duracao;

    //dados.print_matriz();
    for (int i = 0; i < 6; ++i){
        cout << arquivos[i]->nome << endl;
        A_Guloso guloso(arquivos[i]);
        inicio = chrono::system_clock::now();
        guloso.executar();
        fim = chrono::system_clock::now();
        duracao = fim - inicio;
        cout << "=====Guloso=====" << endl;
        guloso.solucao.print_solucao();
        print_capacidade_ocupacao(arquivos[i], &guloso.solucao);
        Sguloso[i] = guloso.solucao.custo;
        tempo_guloso[i] = duracao.count();
        GAP_guloso[i] = ((static_cast<float>(guloso.solucao.custo) - otimos[i]) / otimos[i]) * 100.0f;

        VND vnd(&guloso.solucao, arquivos[i]);
        inicio = chrono::system_clock::now();
        Solucao nova =  vnd.executar();
        fim = chrono::system_clock::now();
        duracao = fim - inicio;
        cout << "======VND======" << endl;
        nova.print_solucao();
        nova.criar_arquivo(arquivos[i]->nome);
        print_capacidade_ocupacao(arquivos[i], &nova);
        SVND[i] = nova.custo;
        tempo_VND[i] = duracao.count();
        GAP_VND[i] = ((static_cast<float>(nova.custo) - otimos[i]) / otimos[i]) * 100.0f;

        cout << endl;
    }

    cout << "Otimo " << setw(10) << " Solucao_Guloso " << setw(10) << " Tempo_Guloso " << setw(10) << " GAP_Guloso " << setw(10) << " Solucao_VND " << setw(10) << " Tempo_VND " << setw(10) <<  " GAP_VND " << endl;
    for (int i = 0; i < 6; ++i){
        cout << otimos[i] <<  setw(5) << " | " << setw(8) << Sguloso[i] << setw(8) << " | " << setw(8) << tempo_guloso[i] << setw(5) << " | ";
        cout << setw(8) << GAP_guloso[i] << setw(5) << " | " << setw(8) << SVND[i] << setw(5) << " | " << setw(8) << tempo_VND[i] << setw(3) << " | ";
        cout << setw(8) << GAP_VND[i] << endl; 
    }


    return 0;

}