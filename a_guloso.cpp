#include <iostream>
#include <vector>
#include <fstream>
#include "a_guloso.h"
#include "read_arquivo.h"

using namespace std;

A_Guloso::A_Guloso(Read_Arquivo* dados){
    this->dados = dados;
}

void A_Guloso::executar(){
    // divide o custo de cada job pelo tempo de processamento
    vector<vector<float>> custo_tempo(this->dados->get_n_servidores(), vector<float>(this->dados->get_n_jobs(), 0));

    for (int i = 0; i < this->dados->get_n_servidores(); i++){
        for (int j = 0; j < this->dados->get_n_jobs(); j++){
            float eficiencia = static_cast<float>(this->dados->get_custo_job()[i][j]) / static_cast<float>(this->dados->get_t_proces_job()[i][j]);
            if (i == 0){
                custo_tempo[i][j] = eficiencia;
            }
            else{
                float valor_atual = static_cast<float>(custo_tempo[i][j]);
                if (eficiencia < valor_atual){
                    custo_tempo[i][j] = eficiencia;
                }
            }
        }
    }

    // ordena os jobs de acordo com a eficiencia
    
}