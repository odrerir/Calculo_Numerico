#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Dados {
    double a, b, x0, delta;
    int n;
};

double funcao(double x) {
    return x * log10(x) - 1;
}

double derivada(double x) {
    return log10(x) + (1 / log(10));
}

// ======================= MÉTODOS =======================

// ----------- Bissecção -----------
void Bisseccao(Dados dados, ofstream &saida) {
    saida << "==================== METODO DA BISSECCAO ====================\n";
    saida << "Iter\t| a\t| b\t| x\t| f(x)\t| Erro\n";

    int k = 0;
    double a = dados.a, b = dados.b, x, erro;
    while (fabs(b - a) > dados.delta && k < dados.n) {
        x = (a + b) / 2.0;
        double fa = funcao(a);
        double fx = funcao(x);

        erro = fabs(b - a);
        saida << k + 1 << "\t| " << a << "\t| " << b << "\t| " << x << "\t| " << fx << "\t| " << erro << "\n";

        if (fa * fx < 0)
            b = x;
        else
            a = x;

        k++;
    }

    saida << "Raiz aproximada = " << x << "\n\n";
}

// ----------- Ponto Fixo -----------
double g(double x) {
    // x = 1 / log10(x) -> rearranjando f(x) = x*log10(x) - 1 = 0
    // Exemplo de função g(x) ajustada para convergência local
    return pow(10, 1 / x);
}

void PontoFixo(Dados dados, ofstream &saida) {
    saida << "==================== METODO DO PONTO FIXO ====================\n";
    saida << "Iter\t| x\t| g(x)\t| Erro\n";

    double x0 = dados.a;
    double x1, erro;
    int k = 0;

    do {
        x1 = g(x0);
        erro = fabs(x1 - x0);
        saida << k + 1 << "\t| " << x0 << "\t| " << x1 << "\t| " << erro << "\n";
        x0 = x1;
        k++;
    } while (erro > dados.delta && k < dados.n);

    saida << "Raiz aproximada = " << x1 << "\n\n";
}

// ----------- Newton-Raphson -----------
void NewtonRaphson(Dados dados, ofstream &saida) {
    saida << "==================== METODO DE NEWTON-RAPHSON ====================\n";
    saida << "Iter\t| x\t| f(x)\t| Erro\n";

    double x0 = (dados.a + dados.b) / 2.0;
    double x1, erro;
    int k = 0;

    do {
        x1 = x0 - funcao(x0) / derivada(x0);
        erro = fabs(x1 - x0);
        saida << k + 1 << "\t| " << x0 << "\t| " << funcao(x0) << "\t| " << erro << "\n";
        x0 = x1;
        k++;
    } while (erro > dados.delta && k < dados.n);

    saida << "Raiz aproximada = " << x1 << "\n\n";
}

// ----------- Secante -----------
void Secante(Dados dados, ofstream &saida) {
    saida << "==================== METODO DA SECANTE ====================\n";
    saida << "Iter\t| x_{k-1}\t| x_k\t| f(x_k)\t| Erro\n";

    double x0 = dados.a;
    double x1 = dados.b;
    double x2, erro;
    int k = 0;

    do {
        double f0 = funcao(x0);
        double f1 = funcao(x1);
        x2 = x1 - f1 * (x1 - x0) / (f1 - f0);
        erro = fabs(x2 - x1);

        saida << k + 1 << "\t| " << x0 << "\t| " << x1 << "\t| " << f1 << "\t| " << erro << "\n";

        x0 = x1;
        x1 = x2;
        k++;
    } while (erro > dados.delta && k < dados.n);

    saida << "Raiz aproximada = " << x2 << "\n\n";
}

// ----------- Falsa Posição (Regula Falsi) -----------
void RegulaFalsi(Dados dados, ofstream &saida) {
    saida << "==================== METODO DA FALSA POSICAO ====================\n";
    saida << "Iter\t| a\t| b\t| x\t| f(x)\t| Erro\n";

    double a = dados.a, b = dados.b, x, erro;
    int k = 0;

    do {
        double fa = funcao(a);
        double fb = funcao(b);
        x = (a * fb - b * fa) / (fb - fa);
        double fx = funcao(x);

        erro = fabs(fx);
        saida << k + 1 << "\t| " << a << "\t| " << b << "\t| " << x << "\t| " << fx << "\t| " << erro << "\n";

        if (fa * fx < 0)
            b = x;
        else
            a = x;

        k++;
    } while (erro > dados.delta && k < dados.n);

    saida << "Raiz aproximada = " << x << "\n\n";
}

// ======================= LEITURA DO ARQUIVO =======================
Dados lerDados(ifstream &arquivo) {
    Dados dados = {0, 0, 0, 0, 0};
    string linha;

    while (getline(arquivo, linha)) {
        // ignora comentários e linhas vazias
        if (linha.empty() || linha[0] == '#') continue;
        // remove espaços em branco
        linha.erase(remove(linha.begin(), linha.end(), ' '), linha.end());

        // pega chave e valor
        size_t pos = linha.find('=');
        if (pos == string::npos) continue;

        string chave = linha.substr(0, pos);
        string valor = linha.substr(pos + 1);

        // converte conforme a chave
        if (chave == "a") dados.a = stod(valor);
        else if (chave == "b") dados.b = stod(valor);
        else if (chave == "delta") dados.delta = stod(valor);
        else if (chave == "n") dados.n = stoi(valor);
    }
    return dados;
}

// ======================= MAIN =======================
int main() {
    ifstream entrada("C:\\Users\\DELL\\Documents\\GitHub\\Calculo_Numerico\\dados.txt");
    if (!entrada.is_open()) {
        cerr << "Erro: nao foi possivel abrir o arquivo de entrada.\n";
        return 1;
    }

    ofstream saida("C:\\Users\\DELL\\Documents\\GitHub\\Calculo_Numerico\\resultados.txt");
    if (!saida.is_open()) {
        cerr << "Erro: nao foi possivel criar o arquivo de saida.\n";
        return 1;
    }

    Dados dados = lerDados(entrada);
    cout << "A= " << dados.a << " B= " << dados.b << " Delta= " << dados.delta << " N= " << dados.n << "\n";

    Bisseccao(dados, saida);
    PontoFixo(dados, saida);
    NewtonRaphson(dados, saida);
    Secante(dados, saida);
    RegulaFalsi(dados, saida);

    entrada.close();
    saida.close();

    cout << "Execucao concluida. Resultados salvos em 'resultados.txt'.\n";
    return 0;
}
