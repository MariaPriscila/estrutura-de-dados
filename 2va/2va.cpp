#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <locale.h>
#include <limits>

enum Tipo {
    PRIORIDADE = 1,
    GERAL = 2
};

struct Cliente {
    std::string nome;
    Tipo tipo;
};

struct Caixa {
    int numero;
    bool aberto;
    std::string tipo;
    std::shared_ptr<Cliente> cliente;
    std::vector<std::shared_ptr<Cliente>> clientesAtendidos;
};

struct Fila {
    std::shared_ptr<Cliente> cliente;
    std::shared_ptr<Fila> proximo;

    bool vazia() {
        return (cliente == nullptr);
    }
};


void menu();
void chegadaCliente(std::vector<Caixa>& caixas, std::shared_ptr<Fila>& filaPrioridade, std::shared_ptr<Fila>& filaGeral);
void situacaoCaixas(const std::vector<Caixa>& caixas);
void liberacaoCaixa(std::vector<Caixa>& caixas, std::shared_ptr<Fila>& filaPrioridade, std::shared_ptr<Fila>& filaGeral);
void relatorioCaixa(const std::vector<Caixa>& caixas);
void relatorioFila(const std::shared_ptr<Fila>& fila, const std::string tipoFila);

int main() {
    setlocale(LC_ALL, "Portuguese");
    std::vector<Caixa> caixas;
    caixas.push_back({1, true, "Prioridade", nullptr});
    caixas.push_back({2, true, "Geral", nullptr});
    std::shared_ptr<Fila> filaPrioridade = nullptr;
    std::shared_ptr<Fila> filaGeral = nullptr;
    int opcao;

    do {
        menu();
        std::cin >> opcao;

        switch (opcao) {
            case 1:
                chegadaCliente(caixas, filaPrioridade, filaGeral);
                break;
            case 2:
                situacaoCaixas(caixas);
                break;
            case 3:
                liberacaoCaixa(caixas, filaPrioridade, filaGeral);
                break;
            case 4:
                relatorioCaixa(caixas);
                break;
            case 5:
                relatorioFila(filaPrioridade , "Prioridade");
                relatorioFila(filaGeral, "Geral");
                break;
            case 6:
                std::cout << "Saindo..." << std::endl;
                break;
            default:
                std::cout << "Opção inválida!" << std::endl;
        }
    } while (opcao != 6);

    return 0;
}

void menu() {
    std::cout << "Banco Poupe bem" << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << "Opções:" << std::endl;
    std::cout << "\n1 - Chegada de cliente" << std::endl;
    std::cout << "2 - Situação dos caixas" << std::endl;
    std::cout << "3 - Liberação de caixa" << std::endl;
    std::cout << "4 - Relatório diário dos caixas" << std::endl;
    std::cout << "5 - Situação das filas" << std::endl;
    std::cout << "6 - Sair" << std::endl;
    std::cout << "\n---------------------------------------------------" << std::endl;
    std::cout << "Digite a opção desejada: ";
}


void chegadaCliente(std::vector<Caixa>& caixas, std::shared_ptr<Fila>& filaPrioridade, std::shared_ptr<Fila>& filaGeral) {
    std::shared_ptr<Cliente> cliente = std::make_shared<Cliente>();
    std::cout << "\nBanco Poupe bem" << std::endl;
    std::cout << "\n---------------------------------------------------" << std::endl;
    std::cout << "Digite o nome do cliente: ";
    std::cin.ignore();
    std::getline(std::cin, cliente->nome);
    int opcao;
    do{
        std::cout << "Digite o tipo do cliente (1-Prioridade ou 2-Geral): ";
        std::cin >> opcao;
        if(opcao != 1 && opcao != 2){
            std::cout << "Opção inválida!" << std::endl;
        }
    }
    while(opcao != 1 && opcao != 2);

    cliente->tipo = static_cast<Tipo>(opcao);

    for (auto& caixa : caixas) {
        if (caixa.aberto) {
            caixa.cliente = cliente;
            caixa.aberto = false;
            std::cout << "O cliente " << cliente->nome << " foi chamado para o caixa " << caixa.tipo << std::endl;
            std::cout << "\n---------------------------------------------------" << std::endl;
            return;
        }
    
    }

    std::shared_ptr<Fila>& fila = (cliente->tipo == PRIORIDADE) ? filaPrioridade : filaGeral;
    
    if (fila == nullptr) {
        fila = std::make_shared<Fila>();
        fila->cliente = cliente;
    } else {
        std::shared_ptr<Fila> aux = fila;
        while (aux->proximo != nullptr) {
            aux = aux->proximo;
        }
        aux->proximo = std::make_shared<Fila>();
        aux->proximo->cliente = cliente;
    }

    std::cout << "O cliente " << cliente->nome << " entrou na fila " << (cliente->tipo == PRIORIDADE ? "de prioridade" : "geral") << std::endl;
    
    int resposta;
    std::cout << "Deseja cadastrar outra entrada de cliente? (1-Sim ou 2-Não): ";
    std::cin >> resposta;
    if (resposta == 1) {
        chegadaCliente(caixas, filaPrioridade, filaGeral);
    }
}


void situacaoCaixas(const std::vector<Caixa>& caixas) {
    std::cout << "\nSituação dos caixas:\n";

    for (const auto& caixa : caixas) {
        if (caixa.aberto) {
            std::cout << "Caixa " << caixa.numero << " (" << caixa.tipo << ") está desocupado.\n";
        } else {
            std::cout << "Caixa " << caixa.numero << " (" << caixa.tipo << ") está atendendo o cliente " << caixa.cliente->nome << ".\n";
        }
    }
    std::cout << "Pressione Enter para voltar ao menu...";

    // Limpar o buffer de entrada
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Aguarda até que o usuário pressione Enter
    std::cin.get();
}

void liberacaoCaixa(std::vector<Caixa>& caixas, std::shared_ptr<Fila>& filaPrioridade, std::shared_ptr<Fila>& filaGeral) {
    int numero;
    std::cout << "Digite o número do caixa que deseja liberar: ";
    std::cin >> numero;

    for (auto& caixa : caixas) {
        if (caixa.numero == numero) {
            if (caixa.aberto) {
                std::cout << "Caixa " << caixa.numero << " está desocupado!" << std::endl;
                return;
            }

            std::cout << "O cliente " << caixa.cliente->nome << " terminou de ser atendido pelo caixa " << caixa.numero << " - " << caixa.tipo << std::endl;
            caixa.clientesAtendidos.push_back(caixa.cliente);
            caixa.cliente = nullptr;
            caixa.aberto = true;

            std::shared_ptr<Fila>& fila = (caixa.tipo == "Prioridade") ? filaPrioridade : filaGeral;

            if (fila != nullptr && !fila->vazia()) {
                    caixa.cliente = fila->cliente;
                    caixa.aberto = false;
                    fila = fila->proximo;
                    std::cout << "O cliente " << caixa.cliente->nome << " foi chamado para o caixa " << caixa.numero << std::endl;
            } else {
                std::shared_ptr<Fila>& filaInversa = (caixa.tipo != "Prioridade") ? filaPrioridade : filaGeral;
                if (filaInversa != nullptr && !filaInversa->vazia()) {
                    caixa.cliente = filaInversa->cliente;
                    caixa.aberto = false;
                    filaInversa = filaInversa->proximo;
                    std::cout << "O cliente " << caixa.cliente->nome << " foi chamado para o caixa " << caixa.numero << std::endl;
            }

            }
            std::cout << "Deseja liberar outro caixa? (1-Sim ou 2-Não): ";
            int opcao;
            std::cin >> opcao;
            if (opcao == 1) {
                liberacaoCaixa(caixas, filaPrioridade, filaGeral);
            }
            else if (opcao != 2){
                std::cout << "Opção inválida!" << std::endl;
            }
            return;
        }
    }

    std::cout << "Caixa não encontrado!" << std::endl;
}

void relatorioCaixa(const std::vector<Caixa>& caixas) {
    int numero;
    std::cout << "Digite o número do caixa que deseja o relatório: ";
    std::cin >> numero;

    for (const auto& caixa : caixas) {
        if (caixa.numero == numero) {
            std::cout << "\nRelatório do caixa " << caixa.numero << " (" << caixa.tipo << "):\n";

            if (!caixa.clientesAtendidos.empty()) {
                std::cout << "Clientes atendidos:\n";
                int contador = 0;
                for (const auto& cliente : caixa.clientesAtendidos) {
                    std::cout << "Nome do cliente: " << cliente->nome << " - Tipo: " << cliente->tipo << std::endl;
                    contador++;
                }
                std::cout << "Total de clientes atendidos por este caixa hoje: " << contador << std::endl;

            } else {
                std::cout << "Nenhum cliente foi atendido por este caixa hoje.\n";
            }
            std::cout << "\nPressione Enter para voltar ao menu...";
            std::cin.ignore();
            std::cin.get();
            return;
        }
    }

    std::cout << "Caixa não encontrado!" << std::endl;
}

void relatorioFila(const std::shared_ptr<Fila>& fila, const std::string tipoFila) {
    if (fila == nullptr) {
        std::cout << "\nRelatório da fila " << tipoFila << ": \n";
        std::cout << "\nFila vazia!" << std::endl;
    } else {
        std::cout << "\nRelatório da fila " << tipoFila << ": \n";
        std::cout << "\n---------------------------------------------------" << std::endl;
        std::shared_ptr<Fila> aux = fila;
        int contador = 0;
        while (aux != nullptr) {
            std::cout << "Nome do cliente: " << aux->cliente->nome << std::endl;
            std::cout << "\n---------------------------------------------------" << std::endl;
            std::cout << "Tipo do cliente: " << aux->cliente->tipo << std::endl;
            std::cout << "\n---------------------------------------------------" << std::endl;
            aux = aux->proximo;
            contador++;
        }
        std::cout << "Total de pessoas na fila " << tipoFila << ": " << contador << std::endl;
    }
    std::cout << "Pressione Enter para voltar ao menu...";
    std::cout << "\n---------------------------------------------------" << std::endl;
    std::cin.ignore();
}
