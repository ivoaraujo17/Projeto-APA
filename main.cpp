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
    Read_Arquivo arq1("teste/n5m15A.txt");
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

    //dados.print_matriz();
    for (int i = 0; i < 6; ++i){
        A_Guloso guloso(arquivos[i]);
        guloso.executar();
        guloso.solucao.print_solucao();
        print_capacidade_ocupacao(arquivos[i], &guloso.solucao);
        Sguloso[i] = guloso.solucao.custo;
        GAP_guloso[i] = ((static_cast<float>(guloso.solucao.custo) - otimos[i]) / otimos[i]) * 100.0f;

        VND vnd(&guloso.solucao, arquivos[i], &guloso.jobs_ordenados);
        Solucao nova =  vnd.executar();
        nova.print_solucao();
        print_capacidade_ocupacao(arquivos[i], &nova);
        SVND[i] = nova.custo;
        GAP_VND[i] = ((static_cast<float>(nova.custo) - otimos[i]) / otimos[i]) * 100.0f;


        cout << endl;
    }

    cout << " Instancias " << " Otimo " << " Solucao Guloso " << " Tempo Guloso " << " GAP Guloso " << " Solucao VND " << " Tempo VND " << " GAP VND " << endl;   
    for (int i = 0; i < 6; ++i){
        cout << otimos[i]<< " " << Sguloso[i] << " tempo " << GAP_guloso[i] << " " << SVND[i] << " tempo " << GAP_VND[i] << endl; 
    }


    return 0;

}