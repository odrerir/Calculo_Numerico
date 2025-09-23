#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

struct Bis{
    double a;
    double b;
    double delta;
    int n;
};

double funcao(double x) {
    return x * log10(x) - 1;
}

void Bisseccao( Bis &dados_iniciais) {
    int k = 0;
    double meio = 0;
    Bis dados = dados_iniciais;
    if (fabs(dados.b - dados.a) < dados.delta) {
        cout << "Raiz = " << dados.a << endl;
    } else {
        while (fabs(dados.b - dados.a) > dados.delta && k < dados.n) {
            k++;
            double finicio = funcao(dados.a);
            meio = (dados.a + dados.b) / 2.0;
            double fmeio = funcao(meio);

            if (finicio * fmeio < 0) {
                dados.b = meio;
            } else {
                dados.a = meio;
            }
        }
        cout << "k = " << k << endl;
        cout << "Raiz = " << meio << endl;
    }
}

Bis lerDados(ifstream &arquivo) {
    Bis dados;
    string linha;
    while (getline(arquivo, linha)) {
      if(linha.empty()) continue; // Pula linhas vazias
      if (linha[0] == '#') continue; // Pula comentários

      if(linha.find("a =") != string::npos) {
        size_t pos = linha.find('=');
        if (pos != string::npos) {
          string valor_str = linha.substr(pos + 1);
          dados.a = stof(valor_str);
          cout << "Valor de A lido: " << dados.a << endl;
        }
      }
      if (linha.find("b =") != string::npos) {
        size_t pos = linha.find('=');
        if (pos != string::npos) {
          string valor_str = linha.substr(pos + 1);
          dados.b = stof(valor_str);
          cout << "Valor de B lido: " << dados.b << endl;
        }
      }
      if (linha.find("delta =") != string::npos) {
        size_t pos = linha.find('=');
        if (pos != string::npos) {
          string valor_str = linha.substr(pos + 1);
          dados.delta = stof(valor_str);
          cout << "Valor de Delta lido: " << dados.delta << endl;
        }
      }
      if (linha.find("n =") != string::npos) {
        size_t pos = linha.find('=');
        if (pos != string::npos) {
          string valor_str = linha.substr(pos + 1);
          dados.n = stoi(valor_str);
          cout << "Valor de N lido: " << dados.n << endl;
        }

      };
    }
    return dados;
}

int main() {
    ifstream arquivo("C:\\Users\\DELL\\Documents\\GitHub\\Calculo_Numerico\\dados.txt");
    if (!arquivo.is_open()) {
      throw runtime_error("Erro: nao foi possivel abrir o arquivo.");
    }
    Bis dados = lerDados(arquivo);
    Bisseccao(dados);

    arquivo.close();
    return 0;
}