#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip> // Para manipulação de largura de campo
#include "read_arquivo.h"
#include "a_guloso.h"
#include "VND.h"
#include "solucao.h"
#include <string>


void print_capacidade_ocupacao(Read_Arquivo* entrada, Solucao* solucao){
    cout << "Capacidade - Ocupacao: ";
    for (int i = 0; i < entrada->get_n_servidores(); i++){
        cout << entrada->get_capacidade_servidor(i) - solucao->ocupacao[i] << " ";
    }
    cout << endl;
}

void main_execute(vector<Read_Arquivo*> arquivos, vector<int> otimos){
    int tam = arquivos.size();
    int Sguloso[tam];
    int SVND[tam];
    float GAP_guloso[tam];
    float GAP_VND[tam];
    float tempo_guloso[tam];
    float tempo_VND[tam];
    chrono::time_point<chrono::system_clock> inicio, fim;
    chrono::duration<float, milli> duracao;

    //dados.print_matriz();
    for (int i = 0; i < tam; ++i){
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
    for (int i = 0; i < tam; ++i){
        cout << otimos[i] <<  setw(5) << " | " << setw(8) << Sguloso[i] << setw(8) << " | " << setw(8) << tempo_guloso[i] << setw(5) << " | ";
        cout << setw(8) << GAP_guloso[i] << setw(5) << " | " << setw(8) << SVND[i] << setw(5) << " | " << setw(8) << tempo_VND[i] << setw(3) << " | ";
        cout << setw(8) << GAP_VND[i] << endl; 
    }
}

void entrar_com_arquivos(){
    char continuar = 's';
    string nome;
    int valor;
    vector<Read_Arquivo*> arquivos;
    vector<int> otimos;
    while(continuar == 's'){
        continuar = ' ';
        cout << "Digite o nome da instancia: " << endl;
        cin >> nome;
        Read_Arquivo arq(nome);
        cout << "Digite o valor otimo da instacia: " << endl;
        cin >> valor;

        arquivos.push_back(&arq);
        otimos.push_back(valor);

        // aqui o usuario insere o nome do arquivo que vai ser transformado em objeto e adcionado no vetor de arquivos

        main_execute(arquivos, otimos);
        
        while (!((continuar == 's') || (continuar == 'n'))){
            cout << endl;
            cin.clear();
            cout << "Deseja adicionar mais uma instancia? (s ou n)" << endl;
            cin >> continuar;
            if (!((continuar == 's') || (continuar == 'n'))){ cout << "Caractere invalido!" << endl; }
        }
        arquivos.clear();
        otimos.clear();

    }
    //cout << "executando" << endl;
    //main_execute(arquivo, otimos);
}

void arquivos_de_teste(){
    Read_Arquivo arq1("teste/n5m15A.txt");
    Read_Arquivo arq2("teste/n5m15B.txt");
    Read_Arquivo arq3("teste/n25m5A.txt");
    Read_Arquivo arq4("teste/n30m5A.txt");
    Read_Arquivo arq5("teste/n60m10.txt");
    Read_Arquivo arq6("teste/n60m10A.txt");

    
    vector<Read_Arquivo*> arquivos_teste = {&arq1, &arq2, &arq3, &arq4, &arq5, &arq6};
    vector<int> otimos_teste = {261, 269, 438, 423, 954, 973};

    main_execute(arquivos_teste, otimos_teste);
}

int main(){
    char close = 'f';
    int opcao;
    while (close == 'f'){
        cout << "digite 1 para passar enderecos de arquivos" << endl;
        cout << "digite 2 para usar as instancias de teste" << endl;
        cout << "digite 0 para fechar o programa" << endl;
        scanf("%i", &opcao);

        switch(opcao)
        {
        case 0:
            close = 'T';
            cout << "Programa terminado" << endl;
            break;
        case 1:
            cout << "entrou" << endl;
            entrar_com_arquivos();
            break;
        case 2:
            arquivos_de_teste();
            break;
        default:
            cout << "\nOpcao Invalida, tente novamente!" << endl;
            break;
        }
    }

    
    return 0;
}