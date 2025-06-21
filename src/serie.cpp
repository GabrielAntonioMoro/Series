#include "serie.h"

#define QUANTIDADE_VARIAVEIS 17

Series::Series(const char* arquivo)
{
    this->fluxo = std::ifstream(arquivo);

    Inicializar();
}

void Series::Inicializar()
{
    // =================================================== //
    //              Lendo cabeçalho de dados               //

    std::string token;
    std::string key, value;

    Momento momento;
    Coordenada coordenada;

    fluxo.clear();
    fluxo.seekg(0);
    for (int i = 0; i < 8; i++)
    {
        std::getline(fluxo, token, '\n');
        size_t symbol = token.find(';');
        if (symbol == std::string::npos) continue;

        key = token.substr(0, symbol);
        value = token.substr(symbol + 1);

        size_t rem = key.find(':');
        if (rem != std::string::npos)
            key.erase(rem);
        
        cabecalho.Inserir(key, value);
    }

    // ==================================================== //
    //              Indexando série de linhas               //

    std::getline(fluxo, token, '\n'); // Linha de títulos. (Ignoramos)
    
    std::streampos inicio_da_linha = fluxo.tellg();

    while (!fluxo.eof())
    {
        std::getline(fluxo, token, '\n');

        if (token.empty()) {
            inicio_da_linha = fluxo.tellg();
            continue;
        }

        std::stringstream stream(token);

        // ---- Adquirindo momento a partir do stringstream.
        std::getline(stream, key, ';');
        momento.data.ano = std::stoi(key.substr(0, 4));
        momento.data.mes = std::stoi(key.substr(5, 2));
        momento.data.dia = std::stoi(key.substr(8, 2));
        
        std::getline(stream, key, ';');
        
        size_t symbol = key.find(':');
        if (symbol != std::string::npos)
            key.replace(symbol, 1, "");
        
        momento.horario.hora = std::stoi(key.substr(0, 2));
        momento.horario.minuto = std::stoi(key.substr(2, 2));
        
        // ---- Adquirindo posição (EVITAR ALTERAÇÕES).
        coordenada = inicio_da_linha + (std::streamoff)stream.tellg();

        this->dados.Inserir(momento, coordenada);
        inicio_da_linha = fluxo.tellg();
    }
}


bool Series::LerLinha(Coordenada coord, Linha *l)
{
    // ==================================================== //
    //              Leitura de linha indexada               //

    std::string line, token;
    int comma;

    if (!fluxo.is_open())
        return false;

    fluxo.clear();
    fluxo.seekg(coord, std::ios::beg);

    // Fazemos a leitura da linha inteira que contém os valores.
    std::getline(fluxo, line, '\n');
    std::stringstream stream(line);

    // Ordem definida nos arquivos do INMET
    double *variaveis[QUANTIDADE_VARIAVEIS] = {&l->precipitacao_total, &l->pressao_atmosferica, &l->pressao_atmosferica_max,
                             &l->pressao_atmosferica_min, &l->radiacao_global, &l->temperatura_ar,
                             &l->temperatura_orvalho, &l->temperatura_ar_max, &l->temperatura_ar_min,
                             &l->temperatura_orvalho_max, &l->temperatura_orvalho_min, &l->umidade_relativa_max,
                             &l->umidade_relativa_min, &l->umidade_relativa, &l->vento_direcao,
                             &l->vento_rajada, &l->vento_velocidade};

    for (int i = 0; i < QUANTIDADE_VARIAVEIS; i++)
    {
        double *p = variaveis[i];

        std::getline(stream, token, ';');
        // Caso a entrada esteja vazia ou o valor seja igual -9999 (Compatibilidade)
        if (token.empty() || token.compare("-9999") == 0) {
            *p = -1.0f;
            continue;
        }

        // Precisamos corrigir um problema...
        // stod espera por um ponto ao em vez de virgula...
        // Para esta correção, então, substituimos todos os pontos por virgula.   
        comma = token.find(',');
        if(comma != std::string::npos)
            token.replace(comma, 1, ".");

        // Inserimos na variável o valor lido.
        *p = std::stod(token);
    }

    return true;
}

bool Series::GetLinha(Momento m, Linha *linha)
{
    if (linha == nullptr)
        return false;

    auto no = dados.Buscar(m);

    if (no == nullptr) // Nossa linha não foi encontrada
        return false;

    return LerLinha(no->valor, linha);
}

bool Series::GetLinhas(Momento de, Momento ate, Lista<Linha> *linhas)
{
    if (linhas == nullptr)
        return false;

    auto lista = dados.Listar([&de, &ate](Momento momento_no) -> bool
                           { 
                            return momento_no >= de && momento_no <= ate; 
                        });

    if (lista.IsVazio()) // Nenhuma linha foi encontrada
        return false;
    
    for(auto i = lista.GetInicio(); i != nullptr; i = i->proximo) {
        Linha linha;

        linha.momento = i->valor->chave;
        if(!LerLinha(i->valor->valor, &linha))
            return false;

        linhas->Inserir(linha);
    }    

    return true;
}

Series::~Series()
{
    this->fluxo.clear();
}
