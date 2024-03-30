#include <iostream>
#include <vector>
#include <fstream>

using namespace std;


class read_arquivo{
    private:
        ifstream arquivo;
        string linha;
        int n_jobs;
        int n_servidores;
        int custo_fixo;
        vector<vector<int>> t_proces_job;
        vector<vector<int>> custo_job;

    public:
        read_arquivo(string nome_arquivo){
            arquivo.open(nome_arquivo);
            if (arquivo.is_open()){
                getline(arquivo, linha);
                this->n_jobs = stoi(linha);
                getline(arquivo, linha);
                this->n_servidores = stoi(linha);
                getline(arquivo, linha);
                this->custo_fixo = stoi(linha);
                // atualiza o tamanho da matriz t process job para o tamanho n_jobs x n_servidores
                this->t_proces_job.resize(this->n_servidores, vector<int>(this->n_jobs));
                this->custo_job.resize(this->n_servidores, vector<int>(this->n_jobs));

                getline(arquivo, linha); // pega a linha em branco
                // leitura da matriz de tempo de processamento
                for (int i = 0; i < this->n_servidores; i++){
                    getline(arquivo, linha);
                    string aux = "";
                    int coluna = 0;
                    for (int c=0; c < int(linha.size()); c++){
                        if (linha[c] == ' '){
                            this->t_proces_job[i][coluna] = stoi(aux);
                            coluna++;
                            aux = "";
                        }
                        else{
                            aux += linha[c];
                        }
                    }
                    this->t_proces_job[i][coluna] = stoi(aux); 
                }

                getline(arquivo, linha); // pega a linha em branco

                // leitura da matriz de custo
                for (int i = 0; i < this->n_servidores; i++){
                    getline(arquivo, linha);
                    string aux = "";
                    int coluna = 0;
                    for (int c=0; c < int(linha.size()); c++){
                        if (linha[c] == ' '){
                            this->custo_job[i][coluna] = stoi(aux);
                            coluna++;
                            aux = "";
                        }
                        else{
                            aux += linha[c];
                        }
                    }
                    this->custo_job[i][coluna] = stoi(aux);
                }
            }
        }

        int get_n_jobs(){
            return this->n_jobs;
        }

        int get_n_servidores(){
            return this->n_servidores;
        }

        int get_custo_fixo(){
            return this->custo_fixo;
        }

        vector<vector<int>> get_t_proces_job(){
            return this->t_proces_job;
        }

        vector<vector<int>> get_custo_job(){
            return this->custo_job;
        }

        ~read_arquivo(){
            arquivo.close();
        }
};


int main(){
    read_arquivo r("entrada.txt");
    for (int i = 0; i < int(r.get_n_servidores()); i++){
        for (int j = 0; j < int(r.get_n_jobs()); j++){
            cout << r.get_t_proces_job()[i][j] << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < int(r.get_n_servidores()); i++){
        for (int j = 0; j < int(r.get_n_jobs()); j++){
            cout << r.get_custo_job()[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
