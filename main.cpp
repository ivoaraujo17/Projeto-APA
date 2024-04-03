#include <iostream>
#include <vector>
#include "read_arquivo.h"
#include "a_guloso.h"


int main(){
    Read_Arquivo dados("entrada.txt");
    A_Guloso guloso(&dados);
    guloso.executar();
    /*for (int i = 0; i < int(dados.get_n_servidores()); i++){
        cout << "Capacidade Sevidor " << i << " : " << dados.get_capacidade_servidores(i) << endl;
    }


    for (int i = 0; i < int(dados.get_n_servidores()); i++){
        for (int j = 0; j < int(dados.get_n_jobs()); j++){
            cout << "Tempo Job " << j << " no servidor " << i << " : "<< dados.get_t_proces_job()[i][j] << endl;
        }
        cout << endl;
    }

    for (int i = 0; i < int(dados.get_n_servidores()); i++){
        for (int j = 0; j < int(dados.get_n_jobs()); j++){
            cout << "Custo Job " << j << " no servidor " << i << " : " << dados.get_custo_job()[i][j] << endl;
        }
        cout << endl;
    }*/

    return 0;

}