#include <iostream>
#include <string>
#include <sstream> // para converter string em inteiro

using namespace std;

class Aluno {
private:
    const string nome;   // nome não pode mudar depois de criado
    const float* notas;  // ponteiro para notas (memória dinâmica)
    const float mediaFinal; // média calculada uma vez e nunca muda

    // função auxiliar para calcular média
    static float calcularMedia(const float* n) {
        float soma = 0;
        for(int i = 0; i < 3; i++) {
            soma += n[i];
        }
        return soma / 3.0f;
    }

public:
    // Construtor: recebe nome e array de notas
    Aluno(const string& n, const float* nts)
        : nome(n), 
          notas(new float[3]),          // aloca memória para as notas
          mediaFinal(calcularMedia(nts)) // calcula a média
    {
        // copia as notas para a memória alocada
        for(int i = 0; i < 3; i++) {
            const_cast<float*>(notas)[i] = nts[i];
            // usamos const_cast apenas para inicializar
        }
    }

    // Destrutor: libera a memória das notas
    ~Aluno() {
        delete[] notas;
    }

    // Exibe os dados do aluno (somente leitura)
    void exibir() const {
        cout << "Nome: " << nome << endl;
        cout << "Notas: ";
        for(int i = 0; i < 3; i++) {
            cout << notas[i] << " ";
        }
        cout << "\nMedia final: " << mediaFinal << endl;
    }

    // Getters (aqui também será somente leitura)
    string getNome() const {
        return nome;
    }

    float getMedia() const {
        return mediaFinal;
    }
};

// Função para cadastrar um novo aluno
void cadastrarAluno(Aluno* alunos[], int& qtd) {
    if(qtd >= 20) {
        cout << "Limite de alunos atingido.\n";
        return;
    }

    string nome;
    float notas[3];

    // Limpa o buffer antes de ler o nome
    cin.ignore();
    cout << "Nome do aluno: ";
    getline(cin, nome);

    // Lê as três notas, com validação simples
    for(int i = 0; i < 3; i++) {
        cout << "Nota " << i+1 << ": ";
        cin >> notas[i];
        // Se a entrada falhar (ex: letra), pede novamente
        while(cin.fail()) {
            cin.clear(); // limpa o erro
            cin.ignore(10000, '\n'); // descarta a linha
            cout << "Valor invalido. Digite um numero: ";
            cin >> notas[i];
        }
    }

    // Cria o aluno dinamicamente e armazena no vetor
    alunos[qtd] = new Aluno(nome, notas);
    qtd++; // incrementa a quantidade
}

// Função para calcular a média da turma
void calcularMediaTurma(const Aluno* alunos[], int qtd, float& mediaTurma) {
    if(qtd == 0) {
        mediaTurma = 0;
        return;
    }
    float soma = 0;
    for(int i = 0; i < qtd; i++) {
        soma += alunos[i]->getMedia();
    }
    mediaTurma = soma / qtd;
}

// Classe para gerar relatórios
class RelatorioAlunos {
public:
    void listar(const Aluno* alunos[], int qtd) const {
        if(qtd == 0) {
            cout << "Nenhum aluno cadastrado.\n";
            return;
        }
        for(int i = 0; i < qtd; i++) {
            alunos[i]->exibir();
            cout << "-----------------\n";
        }
    }
};

int main() {
    Aluno* alunos[20]; // vetor de ponteiros para alunos
    int qtd = 0;
    string opcaoStr;   // opção lida como string para evitar problemas
    float mediaTurma;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1 - Cadastrar aluno\n";
        cout << "2 - Listar alunos\n";
        cout << "3 - Calcular media da turma\n";
        cout << "0 - Sair\n";
        cout << "Escolha uma opcao: ";
        
        // Lê a opção como string para não travar com entrada inválida
        getline(cin, opcaoStr);
        
        // Converte a string para inteiro
        int opcao;
        if(opcaoStr == "0") opcao = 0;
        else if(opcaoStr == "1") opcao = 1;
        else if(opcaoStr == "2") opcao = 2;
        else if(opcaoStr == "3") opcao = 3;
        else opcao = -1; // opção inválida

        switch(opcao) {
            case 1:
                cadastrarAluno(alunos, qtd);
                break;
            case 2: {
                RelatorioAlunos rel;
                // Usamos const_cast para compatibilidade com a função que exige const
                rel.listar(const_cast<const Aluno**>(alunos), qtd);
                break;
            }
            case 3:
                calcularMediaTurma(const_cast<const Aluno**>(alunos), qtd, mediaTurma);
                cout << "Media da turma: " << mediaTurma << endl;
                break;
            case 0:
                cout << "Encerrando programa.\n";
                break;
            default:
                cout << "Opcao invalida. Tente novamente.\n";
        }
    } while(opcaoStr != "0");

    // Libera a memória alocada para cada aluno
    for(int i = 0; i < qtd; i++) {
        delete alunos[i];
    }

    return 0;
}