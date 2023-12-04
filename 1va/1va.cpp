//Menu

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <locale.h>
using namespace std;
struct Usuario {
    int rg;
    string nome;
    int idade;
    string email;
};

void menu();

//Tela de marcação de vacinação - quando há vagas

void telaMarcacao(vector<Usuario>& fila) {
    Usuario usuario;
    cout << "Digite o RG: ";
    cin >> usuario.rg;
    cout << "Digite o nome: ";
    cin.ignore();
    getline(cin, usuario.nome);
    cout << "Digite a idade: ";
    cin >> usuario.idade;
    cout << "Digite o e-mail: ";
    cin.ignore();
    getline(cin, usuario.email);
    fila.push_back(usuario);
    cout << "Usuário cadastrado com sucesso!" << endl;
}

//Tela de desmarcação

void telaDesmarcacao(vector<Usuario>& usuarios, vector<Usuario>& filaEspera) {
    int rg;
    cout << "Digite o RG: ";
    cin >> rg;
    for (int i = 0; i < usuarios.size(); i++) {
        if (usuarios[i].rg == rg) {
            usuarios.erase(usuarios.begin() + i);
            cout << "Usuário desmarcado com sucesso!" << endl;
            if (filaEspera.size() > 0) {
                usuarios.push_back(filaEspera[0]);
                filaEspera.erase(filaEspera.begin());
                cout << "Usuário da fila de espera foi marcado!" << endl;
            }
            return;
        }
    }
    cout << "Usuário não encontrado!" << endl;
}

//Tela de listagem dos pacientes marcados e na fila de espera

void telaListagem(vector<Usuario>& usuarios, vector<Usuario>& filaEspera) {
    cout << "Lista de pacientes marcados: " << endl;
    for (int i = 0; i < usuarios.size(); i++) {
        cout << "RG: " << usuarios[i].rg << endl;
        cout << "Nome: " << usuarios[i].nome << endl;
        cout << "Idade: " << usuarios[i].idade << endl;
        cout << "E-mail: " << usuarios[i].email << endl;
        cout << endl;
    }
    cout << "Lista de pacientes na fila de espera: " << endl;
    for (int i = 0; i < filaEspera.size(); i++) {
        cout << "RG: " << filaEspera[i].rg << endl;
        cout << "Nome: " << filaEspera[i].nome << endl;
        cout << "Idade: " << filaEspera[i].idade << endl;
        cout << "E-mail: " << filaEspera[i].email << endl;
        cout << endl;
    }
}

//Função principal

int main() {
    setlocale(LC_ALL, "Portuguese");
    vector<Usuario> usuarios;
    vector<Usuario> filaEspera;
    int opcao;
    do {
        menu();
        cin >> opcao;
        switch (opcao) {
        case 1:
            if (usuarios.size() < 10) {
                telaMarcacao(usuarios);
            }
            else {
                char opcao;
                cout << "Não há vagas, deseja ser colocado na fila de espera? (S/N) ";
                cin >> opcao;
                if (opcao == 'S') {
                    if (filaEspera.size() < 10) {
                        telaMarcacao(filaEspera);
                    }
                    else {
                        cout << "Não há vagas na fila de espera!" << endl;
                    }
                }
            }
            break;
        case 2:
            telaDesmarcacao(usuarios, filaEspera);
            break;
        case 3:
            telaListagem(usuarios, filaEspera);
            break;
        case 4:
            cout << "Saindo do programa..." << endl;
            break;
        default:
            cout << "Opção inválida!" << endl;
        }
    } while (opcao != 4);
    return 0;
}

//Menu

void menu() {
    cout << "1 - Marcar vacinação" << endl;
    cout << "2 - Cancelar marcação" << endl;
    cout << "3 - Listagem geral" << endl;
    cout << "4 - Sair" << endl;
    cout << "Digite a opção desejada: ";
}
