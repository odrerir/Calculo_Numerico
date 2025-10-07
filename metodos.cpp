#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <iomanip>

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
    saida << "Iter\t a\t b\t x\t f(x)\t Erro\n";

    int k = 0;
    double a = dados.a, b = dados.b, x, erro;
    while (fabs(b - a) > dados.delta && k < dados.n) {
        x = (a + b) / 2.0;
        double fa = funcao(a);
        double fx = funcao(x);

        erro = fabs(b - a);
        saida << k + 1 << "\t" << a << "\t" << b << "\t" << x << "\t" << fx << "\t" << erro << "\n";

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
    saida << "Iter\t x\t g(x)\t Erro\n";

    double x0 = dados.a;
    double x1, erro;
    int k = 0;

    do {
        x1 = g(x0);
        erro = fabs(x1 - x0);
        saida << k + 1 << "\t" << x0 << "\t" << x1 << "\t" << erro << "\n";
        x0 = x1;
        k++;
    } while (erro > dados.delta && k < dados.n);

    saida << "Raiz aproximada = " << x1 << "\n\n";
}

// ----------- Newton-Raphson -----------
void NewtonRaphson(Dados dados, ofstream &saida) {
    saida << "==================== METODO DE NEWTON-RAPHSON ====================\n";
    saida << "Iter\t x\t f(x)\t Erro\n";

    double x0 = (dados.a + dados.b) / 2.0;
    double x1, erro;
    int k = 0;

    do {
        x1 = x0 - funcao(x0) / derivada(x0);
        erro = fabs(x1 - x0);
        saida << k + 1 << "\t" << x0 << "\t" << funcao(x0) << "\t" << erro << "\n";
        x0 = x1;
        k++;
    } while (erro > dados.delta && k < dados.n);

    saida << "Raiz aproximada = " << x1 << "\n\n";
}

// ----------- Secante -----------
void Secante(Dados dados, ofstream &saida) {
    saida << "==================== METODO DA SECANTE ====================\n";
    saida << "Iter\t x_{k-1}\t x_k\t f(x_k)\t Erro\n";

    double x0 = dados.a;
    double x1 = dados.b;
    double x2, erro;
    int k = 0;

    do {
        double f0 = funcao(x0);
        double f1 = funcao(x1);
        x2 = x1 - f1 * (x1 - x0) / (f1 - f0);
        erro = fabs(x2 - x1);

        saida << k + 1 << "\t" << x0 << "\t" << x1 << "\t" << f1 << "\t" << erro << "\n";

        x0 = x1;
        x1 = x2;
        k++;
    } while (erro > dados.delta && k < dados.n);

    saida << "Raiz aproximada = " << x2 << "\n\n";
}

// ----------- Falsa Posição (Regula Falsi) -----------
void RegulaFalsi(Dados dados, ofstream &saida) {
    saida << "==================== METODO DA FALSA POSICAO ====================\n";
    saida << "Iter\t a\t b\t x\t f(x)\t Erro\n";

    double a = dados.a, b = dados.b, x, erro;
    int k = 0;

    do {
        double fa = funcao(a);
        double fb = funcao(b);
        x = (a * fb - b * fa) / (fb - fa);
        double fx = funcao(x);

        erro = fabs(fx);
        saida << k + 1 << "\t" << a << "\t" << b << "\t" << x << "\t" << fx << "\t" << erro << "\n";

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
    Dados dados;
    string linha;
    while (getline(arquivo, linha)) {
        if (linha.empty() || linha[0] == '#') continue;

        if (linha.find("a =") != string::npos)
            dados.a = stod(linha.substr(linha.find('=') + 1));

        else if (linha.find("b =") != string::npos)
            dados.b = stod(linha.substr(linha.find('=') + 1));

        else if (linha.find("delta =") != string::npos)
            dados.delta = stod(linha.substr(linha.find('=') + 1));

        else if (linha.find("n =") != string::npos)
            dados.n = stoi(linha.substr(linha.find('=') + 1));
    }
    return dados;
}

// ======================= MAIN =======================
int main() {
    ifstream entrada("dados.txt");
    if (!entrada.is_open()) {
        cerr << "Erro: nao foi possivel abrir o arquivo de entrada.\n";
        return 1;
    }

    ofstream saida("resultados.txt");
    if (!saida.is_open()) {
        cerr << "Erro: nao foi possivel criar o arquivo de saida.\n";
        return 1;
    }

    Dados dados = lerDados(entrada);

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
