#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <fstream>
#include <sstream>

#include <arvore.h>
#include <lista.h>

#include <momento.h>

/*
 * Coordenada de uma posição dos dados em bytes do arquivo.
 */
typedef std::streampos Coordenada;

/*
 * Estrutura de dados de uma linha do arquivo.
 */
typedef struct Linha
{
    Momento momento;

    double radiacao_global;
    double precipitacao_total;

    double pressao_atmosferica;
    double pressao_atmosferica_max;
    double pressao_atmosferica_min;

    double temperatura_ar;
    double temperatura_ar_max;
    double temperatura_ar_min;

    double temperatura_orvalho;
    double temperatura_orvalho_max;
    double temperatura_orvalho_min;

    double umidade_relativa;
    double umidade_relativa_max;
    double umidade_relativa_min;

    double vento_direcao;
    double vento_velocidade;
    double vento_rajada;

} Linha;

/*
 * Classe dedicada para a leitura e tratamento dos dados de um arquivo.
 */
class Series
{
private:
    std::ifstream fluxo;

    Arvore<std::string, std::string> cabecalho;
    Arvore<Momento, Coordenada> dados;

    /*
     * @brief Inicializa todo o sistema com a interpretação dos cabeçalhos de dados
     * e faz a indexação de cada momento de cada linha.
     */
    void Inicializar();

    /*
     * @brief Lê uma linha indexada por uma coordenada, e salva dentro do parâmetro
     * do tipo Linha*.
     * @param coordenada: coordenada da indexação do árquivo.
     * @param linha: linha a ser atulizada com os valores lido do arquivo.
     */
    bool LerLinha(Coordenada coordenada, Linha *linha);

public:
    /*
     * @brief Construtor padrão da classe.
     * @param arquivo: caminho absoluto para o arquivo desejado.
     */
    Series(const char* arquivo);
    ~Series();

    /*
     * @brief Faz a leitura de uma única linha do árquivo, e
     * escreve os valores para o parâmetro linha.
     * @param momento: Momento desejado.
     * @param linha: Linha que será escrito os dados.
     * @return true se nenhum problema ocorreu.
     *         false se houve um problema.
     */
    bool GetLinha(Momento momento, Linha *linha);

    /*
     * @brief Faz a leitura de várias linhas do arquivo, dado
     * dois momentos. Em seguida, escreve os valores para a lista fornecida.
     * @param de: Começo de busca.
     * @param ate: Fim de busca.
     * @param linhas: Lista a ser atualizada com os dados.
     * @return true se nenhum problema ocorreu.
     *         false se houve um problema.
     */
    bool GetLinhas(Momento de, Momento ate, Lista<Linha> *linhas);

    /*
     * @brief Retorna o cabeçalho do arquivo.
     */
    inline Arvore<std::string, std::string>& GetCabecalho()
    {
        return this->cabecalho;
    }

    /*
     * @brief Retorna os dados indexados do arquivo.
     */
    inline Arvore<Momento, Coordenada>& GetDados()
    {
        return this->dados;
    }
};
#endif