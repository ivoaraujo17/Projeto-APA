#include <iostream>
#include <vector>
#include "read_arquivo.h"
#include "a_guloso.h"


int main(){
    Read_Arquivo dados("entrada.txt");
    //dados.print_matriz();
    A_Guloso guloso(&dados);
    guloso.executar();

    return 0;

}