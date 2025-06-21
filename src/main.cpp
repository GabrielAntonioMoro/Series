#include <serie.h>

#include <iostream>
#include <ctype.h>

#define MODO_INDEFINIDO 0
#define MODO_ESPECIFICO 1
#define MODO_GENERALIZADO 2

#define STATUS_ERRO -1
#define STATUS_OK 0
#define STATUS_CANCELADO 1

/*
 * [0] = Indefinido
 * [1] = Específico
 * [2] = Generalizado
 */
int modo;
bool exit_program = false;

Momento primaria;
Momento secundaria;

Series *series;

// Mostra o cabeçalho do programa.
void UIShowInformativo();

// Mostra na tela todas as opções de consultas e questiona o usuário.
bool UIShowConsulta();
// Mostra na tela o questionário sobre as datas.
bool UIShowQuestionario();
// Mostra na tela o resultado.
void UIShowResultado();

// Questiona o usuário e salva os dados para 'momento'.
bool UIGetData(Momento *momento);
// Questiona o usuário para várias escolhas separadas por ','.
bool UIGetEscolhas(int *, int);
// Questiona o usuário por um único número.
bool UIGetEscolha(int *, bool *);

// Estrutura do cabeçalho da nossa tabela.
void UIShowTabelaHeader();

// Estrutura do conteúdo da nossa tabela.
void UIShowTabelaContent(Linha l);

// Estrutura do rodapé da nossa tabela.
void UIShowTabelaFooter(long itens, double *somas, double *maiores, double *menores);

// Pausa a execução e espera que o usuário pressione Enter.
void UIGetEnterParaContinuar();

// Faz a leitura somente de digitos da entrada do usuário
int UIEntrada(int *v);

// Função de entrada do programa.
int main(int argc, char *argv[])
{
    // 1- O nosso programa.
    // 2- O arquivo que queremos carregar.
    if(argc < 2 || argc > 2) {
        printf("\nInforme corretamente a entrada para o programa.\n");
        printf("Exemplo de entrada: \n");
        printf("\t$ ./programa \"diretorio/do/arquivo/INMET.CSV\"\n");
        printf("Saindo do programa.\n\n");

        return -1;
    } 

    series = new Series(argv[1]);

    while (exit_program == false)
    {
        modo = MODO_INDEFINIDO;

        system("clear");

        if (!UIShowConsulta() || !UIShowQuestionario())
            continue;

        UIShowResultado();
    }

    delete series;
    return 0;
}

void UIShowInformativo()
{
    system("clear");
    if (modo == MODO_INDEFINIDO)
        return;

    printf(" - Opção: ");
    switch (modo)
    {
    case 1:
        printf("[1] Consulta específica.");
        break;
    case 2:
        printf("[2] Consulta genérica.");
        break;
    default:
        printf("Não reconhecido.");
    }
    printf("\n");
}

bool UIShowConsulta()
{
    int escolha = 0;

    printf("Que tipo de consulta você deseja fazer?\n");
    printf(" [1] Mostrar resumo dado *um* momento específico.\n");
    printf(" [2] Mostre o resumo durante *dois* momentos específicados.\n");
    printf(" [0] Sair do programa.\n");

    printf(" $ Informe sua escolha: ");
    if (!UIGetEscolha(&escolha, nullptr))
        return false;

    modo = escolha;
    if (modo == 0)
        exit_program = true;

    return modo == MODO_ESPECIFICO || modo == MODO_GENERALIZADO;
}

bool UIShowQuestionario()
{
    Momento m1, m2;

    UIShowInformativo();

    if (modo == MODO_GENERALIZADO)
    { // Um periodo específico
        printf("Será necessário informar um período (dois momentos).");
        printf("Caso você não queira especificar algum campo, deixe em branco.");

        if (!UIGetData(&m1))
            return false;

        if (!UIGetData(&m2))
            return false;

        primaria = m1;
        secundaria = m2;
    }
    else if (modo == MODO_ESPECIFICO)
    { // Um momento específico.
        printf("Será necessário informar somente um momento\n");
        printf("Caso você não queira especificar algum campo, deixe em branco.\n");

        if (!UIGetData(&m1))
            return false;

        primaria = m1;
    }
    else
    { // Erro.
        printf("Flag não reconhecida! Erro interno.");
        return false;
    }

    return true;
}

void UIShowResultado()
{
    int j;
    double d;

    double *soma = (double *)calloc(sizeof(double), 17);
    double *maior = (double *)calloc(sizeof(double), 17);
    double *menor = (double *)calloc(sizeof(double), 17);

    Lista<Linha> linhas;
    Linha l;

    UIShowInformativo();

    if (modo == MODO_ESPECIFICO)
        secundaria = primaria;

    if (!series->GetLinhas(primaria, secundaria, &linhas))
    {
        std::cerr << "Erro ao listar todos os itens, verifique se seu arquivo.csv é suportado." << std::endl;
        exit_program = true;
        UIGetEnterParaContinuar();
        return;
    }

    UIShowTabelaHeader();
    for (auto i = linhas.GetInicio(); i != nullptr; i = i->proximo)
    {
        l = i->valor;

        double variaveis[17] = {l.precipitacao_total, l.pressao_atmosferica, l.pressao_atmosferica_max,
                                l.pressao_atmosferica_min, l.radiacao_global, l.temperatura_ar,
                                l.temperatura_orvalho, l.temperatura_ar_max, l.temperatura_ar_min,
                                l.temperatura_orvalho_max, l.temperatura_orvalho_min, l.umidade_relativa_max,
                                l.umidade_relativa_min, l.umidade_relativa, l.vento_direcao,
                                l.vento_rajada, l.vento_velocidade};

        for (j = 0; j < 17; j++)
        {
            d = variaveis[j];

            soma[j] += d;

            if(d <= 0)
                continue;

            if (d > maior[j])
                maior[j] = d;

            if (menor[j] == 0 || d < menor[j])
                menor[j] = d;
        
        }

        UIShowTabelaContent(i->valor);
    }

    UIShowTabelaFooter(linhas.GetTamanho(), soma, maior, menor);
    UIGetEnterParaContinuar();
}

bool UIGetData(Momento *momento)
{
    bool ignorado = false;
    int ano = MOMENTO_DONT_COMPARE, mes = MOMENTO_DONT_COMPARE, dia = MOMENTO_DONT_COMPARE,
        hora = MOMENTO_DONT_COMPARE, minuto = MOMENTO_DONT_COMPARE;

    printf("Específique o Momento:\n");
    printf(" - Ano [Ex: 2024]: ");
    if (!UIGetEscolha(&ano, &ignorado))
        return false;

    if (!ignorado)
        if (ano < 2000 || ano > 2025)
        {
            std::cerr << "\nInforme um ano dentro de 2000 e 2025\n";
            return false;
        }

    printf(" - Mês [Ex: 12]: ");
    if (!UIGetEscolha(&mes, &ignorado))
        return false;

    if (!ignorado)
        if (mes < 1 || mes > 12)
        {
            std::cerr << "\nInforme um mês dentro de 1 e 12\n";
            return false;
        }

    printf(" - Dia [Ex: 31]: ");
    if (!UIGetEscolha(&dia, &ignorado))
        return false;

    if (!ignorado)
        if (dia < 1 || dia > 31)
        {
            std::cerr << "\nInforme um dia do mês dentro de 1 e 31\n";
            return false;
        }

    printf(" - Hora [Ex: 23]: ");
    if (!UIGetEscolha(&hora, &ignorado))
        return false;

    if (!ignorado)
        if (hora < 0 || dia > 23)
        {
            std::cerr << "\nInforme uma hora dentro de 0 e 23\n";
            return false;
        }

    printf(" - Minuto [Ex: 59]: ");
    if (!UIGetEscolha(&minuto, &ignorado))
        return false;

    if (!ignorado)
        if (hora < 0 || dia > 23)
        {
            std::cerr << "\nInforme um minuto dentro de 0 e 23\n";
            return false;
        }

    *momento = Momento(dia, mes, ano, hora, minuto);
    return true;
}

bool UIGetEscolhas(int *escolhas, int tamanho)
{
    int i, value;

    std::string line, part;
    std::stringstream stream;

    printf("Informe suas escolhas separadas por ',' [ Exemplo: $ 1,2,3 ]:\n");
    printf("$ ");

    std::cin >> line;
    stream = std::stringstream(line);

    i = 0;
    while (std::getline(stream, part, ','))
    {
        if (i >= tamanho)
        {
            std::cerr << "\nTamanho de entrada excedido! Informe somente " << tamanho << " entradas.\n";
            return false;
        }

        if (part.empty())
            break;

        for (char c : part)
        {
            // Saímos da verificação se o caractere inserido não for um número.
            if (!std::isdigit(c))
            {
                std::cerr << "\n\nInforme somente digitos.\n";
                UIGetEnterParaContinuar();
                return false;
            }
        }

        value = std::stoi(part);

        if (value < 0 && value > 10)
        {
            std::cerr << "\n\nValor: " << value << " fora de escala. (-1 > x > 10)";
            UIGetEnterParaContinuar();
            return false;
        }

        escolhas[i] = value;
        i++;
    }

    return true;
}

bool UIGetEscolha(int *v, bool *ignored)
{
    int st = UIEntrada(v);

    if (st == STATUS_CANCELADO)
    {
        if (ignored != nullptr)
        {
            *ignored = true;
            return true;
        }
        return false;
    }

    return st == STATUS_OK;
}

int UIEntrada(int *v)
{
    std::string digitos;

    int ch;
    int status;

    while (true)
    {
        ch = std::cin.get();

        if (ch == '\n')
        {                        // Usuário pressionou ENTER.
            if (digitos.empty()) // Se nada foi digitado, então sai como cancelado.
                return STATUS_CANCELADO;
            else
            {
                *v = std::stoi(digitos);
                return STATUS_OK;
            }
        }

        // Backspace terá a funcionalidade de apagar um caractere.
        if (((char)ch) == '\b' || ch == 127)
        {
            if (digitos.empty())
                continue;

            digitos.pop_back();
            std::cout << "\b \b" << std::flush;
        }
        else if (std::isdigit(ch))
        {
            // Adiciona o dígito à string e o exibe no console
            digitos += (char)ch;
            std::cout << std::flush;
        }
    }

    std::cout << std::endl;

    return STATUS_ERRO;
}

void UIShowTabelaHeader()
{
    printf("%-4s|%-4s|%-6s|", "Dia", "Mes", "Ano");
    printf("%-5s|%-5s|", "Hora", "Min");

    printf("%-13s|", "Precipitacao");
    printf("%-16s|", "Radiacao Global");
    printf("%-12s|", "Pressao");
    printf("%-16s|", "Pressao Max");
    printf("%-16s|", "Pressao Min");
    printf("%-12s|", "Temp. Ar");
    printf("%-14s|", "Temp. Ar Max");
    printf("%-14s|", "Temp. Ar Min");
    printf("%-14s|", "Temp. Orvalho");
    printf("%-14s|", "Temp. Or. Max");
    printf("%-14s|", "Temp. Or. Min");
    printf("%-10s|", "Umidade");
    printf("%-13s|", "Umidade Max");
    printf("%-13s|", "Umidade Min");
    printf("%-14s|", "Vento Direcao");
    printf("%-11s|", "Vento Vel.");
    printf("%-12s|", "Vento Rajada");
    printf("\n");

    printf("%-4s|", "dd");
    printf("%-4s|", "mm");
    printf("%-6s|", "aaaa");
    printf("%-5s|", "hh");
    printf("%-5s|", "MM");
    printf("%-13s|", "(mm)");
    printf("%-16s|", "(Kj/m^2)");
    printf("%-12s|", "(mB)");
    printf("%-16s|", "(mB)");
    printf("%-16s|", "(mB)");
    printf("%-12s|", "(oC)");
    printf("%-14s|", "(oC)");
    printf("%-14s|", "(oC)");
    printf("%-14s|", "(oC)");
    printf("%-14s|", "(oC)");
    printf("%-14s|", "(oC)");
    printf("%-10s|", "(%)");
    printf("%-13s|", "(%)");
    printf("%-13s|", "(%)");
    printf("%-14s|", "(ogr)");
    printf("%-11s|", "(m/s)");
    printf("%-12s|", "(m/s)");
    printf("\n");

     for(int i = 1; i <= 274; i++)
        printf("-");

    printf("\n");
}

void UIShowTabelaContent(Linha l)
{
    printf("%-4d|%-4d|%-6d|%-5d|%-5d|%-13.2f|%-16.2f|%-12.2f|%-16.2f|%-16.2f|%-12.2f|%-14.2f|%-14.2f|%-14.2f|%-14.2f|%-14.2f|%-10.2f|%-13.2f|%-13.2f|%-14.2f|%-11.2f|%-12.2f|\n",
           l.momento.data.dia,
           l.momento.data.mes,
           l.momento.data.ano,
           l.momento.horario.hora,
           l.momento.horario.minuto,
           l.precipitacao_total,
           l.radiacao_global,
           l.pressao_atmosferica,
           l.pressao_atmosferica_max,
           l.pressao_atmosferica_min,
           l.temperatura_ar,
           l.temperatura_ar_max,
           l.temperatura_ar_min,
           l.temperatura_orvalho,
           l.temperatura_orvalho_max,
           l.temperatura_orvalho_min,
           l.umidade_relativa,
           l.umidade_relativa_max,
           l.umidade_relativa_min,
           l.vento_direcao,
           l.vento_velocidade,
           l.vento_rajada);
}

void UIShowTabelaFooter(long itens, double *somas, double *maiores, double *menores)
{
    double *medias = (double *)calloc(sizeof(double), 17);
    
    for(int i = 1; i <= 274; i++)
        printf("-");

    printf("\n");
    for (int i = 0; i < 17; i++)
        medias[i] = somas[i] / itens;
    
    printf("%28s|%-13.2f|%-16.2f|%-12.2f|%-16.2f|%-16.2f|%-12.2f|%-14.2f|%-14.2f|%-14.2f|%-14.2f|%-14.2f|%-10.2f|%-13.2f|%-13.2f|%-14.2f|%-11.2f|%-12.2f|\n",
           "Medias: ", medias[0], medias[1], medias[2], medias[3], medias[4], medias[5], medias[6], medias[7],
           medias[8], medias[9], medias[10], medias[11], medias[12], medias[13], medias[14], medias[15], medias[16]);

     printf("%28s|%-13.2f|%-16.2f|%-12.2f|%-16.2f|%-16.2f|%-12.2f|%-14.2f|%-14.2f|%-14.2f|%-14.2f|%-14.2f|%-10.2f|%-13.2f|%-13.2f|%-14.2f|%-11.2f|%-12.2f|\n",
           "Soma total:", somas[0], somas[1], somas[2], somas[3], somas[4], somas[5], somas[6], somas[7],
           somas[8], somas[9], somas[10], somas[11], somas[12], somas[13], somas[14], somas[15], somas[16]);

     printf("%28s|%-13.2f|%-16.2f|%-12.2f|%-16.2f|%-16.2f|%-12.2f|%-14.2f|%-14.2f|%-14.2f|%-14.2f|%-14.2f|%-10.2f|%-13.2f|%-13.2f|%-14.2f|%-11.2f|%-12.2f|\n",
           "Maiores valores:", maiores[0], maiores[1], maiores[2], maiores[3], maiores[4], maiores[5], maiores[6], maiores[7],
           maiores[8], maiores[9], maiores[10], maiores[11], maiores[12], maiores[13], maiores[14], maiores[15], maiores[16]);

     printf("%28s|%-13.2f|%-16.2f|%-12.2f|%-16.2f|%-16.2f|%-12.2f|%-14.2f|%-14.2f|%-14.2f|%-14.2f|%-14.2f|%-10.2f|%-13.2f|%-13.2f|%-14.2f|%-11.2f|%-12.2f|\n",
           "Menores valores:", menores[0], menores[1], menores[2], menores[3], menores[4], menores[5], menores[6], menores[7],
           menores[8], menores[9], menores[10], menores[11], menores[12], menores[13], menores[14], menores[15], menores[16]);
}

// Pausa a execução e espera que o usuário pressione Enter.
void UIGetEnterParaContinuar()
{
    printf("\nPressione ENTER para continuar...");
    // Limpa o buffer de entrada antes de esperar pelo Enter
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;      // Consome o resto da linha anterior
    getchar(); // Espera pelo Enter
}
