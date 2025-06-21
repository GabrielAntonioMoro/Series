#ifndef MOMENTO_H
#define MOMENTO_H

#include <string>

// Significa que não queremos comparar nossos valores.
#define MOMENTO_DONT_COMPARE -1

// Estrutura de Data: 00/00/0000
struct Data
{
    int dia, mes, ano;

    Data()
    {
        this->dia = MOMENTO_DONT_COMPARE;
        this->mes = MOMENTO_DONT_COMPARE;
        this->ano = MOMENTO_DONT_COMPARE;
    }

    inline Data(int dia, int mes, int ano)
    {
        this->dia = dia;
        this->mes = mes;
        this->ano = ano;
    }

    inline int Compare(const Data &other)
    {
        // Caso na data que seja fornecido, tenha variáveis de valores igual a 0,
        // para esta lógica, estamos indicando que não queremos comparar esses parâmetros.

        if (this->ano != MOMENTO_DONT_COMPARE && other.ano != MOMENTO_DONT_COMPARE)
        {
            if (this->ano > other.ano)
                return 1;
            else if (this->ano < other.ano)
                return -1;
        }

        if (this->mes != MOMENTO_DONT_COMPARE && other.mes != MOMENTO_DONT_COMPARE)
        {
            if (this->mes > other.mes)
                return 1;
            else if (this->mes < other.mes)
                return -1;
        }

        if (this->dia != MOMENTO_DONT_COMPARE && other.dia != MOMENTO_DONT_COMPARE)
        {
            if (this->dia > other.dia)
                return 1;
            else if (this->dia < other.dia)
                return -1;
        }
        return 0;
    }

    bool operator==(const Data &other)
    {
        return Compare(other) == 0;
    }

    bool operator!=(const Data &other)
    {
        return Compare(other) != 0;
    }

    bool operator>(const Data &other)
    {
        return Compare(other) > 0;
    }

    bool operator<(const Data &other)
    {
        return Compare(other) < 0;
    }

    bool operator>=(const Data &other)
    {
        return Compare(other) >= 0;
    }

    bool operator<=(const Data &other)
    {
        return Compare(other) <= 0;
    }
};

// Estrutura de horário: 00:00
struct Horario
{
    int hora, minuto;

    Horario()
    {
        this->hora = MOMENTO_DONT_COMPARE;
        this->minuto = MOMENTO_DONT_COMPARE;
    };

    inline Horario(int hora, int minuto)
    {
        this->hora = hora;
        this->minuto = minuto;
    }

    inline int Compare(const Horario &other)
    {
        // Caso no horário que seja fornecido, tenha variáveis de valores igual a 0,
        // para esta lógica, estamos indicando que não queremos comparar esses parâmetros.

        if (this->hora != MOMENTO_DONT_COMPARE && other.hora != MOMENTO_DONT_COMPARE)
        {
            if (this->hora > other.hora)
                return 1;
            else if (this->hora < other.hora)
                return -1;
        }

        if (this->minuto != MOMENTO_DONT_COMPARE && other.minuto != MOMENTO_DONT_COMPARE)
        {
            if (this->minuto > other.minuto)
                return 1;
            else if (this->minuto < other.minuto)
                return -1;
        }

        return 0;
    }

    bool operator==(const Horario &other)
    {
        return Compare(other) == 0;
    }

    bool operator!=(const Horario &other)
    {
        return Compare(other) != 0;
    }

    bool operator>(const Horario &other)
    {
        return Compare(other) > 0;
    }

    bool operator<(const Horario &other)
    {
        return Compare(other) < 0;
    }

    bool operator>=(const Horario &other)
    {
        return Compare(other) >= 0;
    }

    bool operator<=(const Horario &other)
    {
        return Compare(other) <= 0;
    }
};

// Estrutura de momento, engloba data e horario juntos.
// 00/00/0000 00:00
struct Momento
{
    Data data;
    Horario horario;

    Momento() = default;

    inline Momento(Data data, Horario horario)
    {
        this->data = data;
        this->horario = horario;
    }

    inline Momento(int dia, int mes = MOMENTO_DONT_COMPARE, int ano = MOMENTO_DONT_COMPARE)
    {
        this->data = Data(dia, mes, ano);
    }

    inline Momento(int dia, int mes = MOMENTO_DONT_COMPARE, int ano = MOMENTO_DONT_COMPARE, int hora = MOMENTO_DONT_COMPARE, int minuto = MOMENTO_DONT_COMPARE)
    {
        this->data = Data(dia, mes, ano);
        this->horario = Horario(hora, minuto);
    }

    inline int Compare(const Momento &other)
    {
        int v = this->data.Compare(other.data);
        if (v != 0)
            return v;

        v = this->horario.Compare(other.horario);
        if (v != 0)
            return v;
        return 0;
    }

    bool operator==(const Momento &other)
    {
        return Compare(other) == 0;
    }

    bool operator!=(const Momento &other)
    {
        return Compare(other) != 0;
    }

    bool operator>(const Momento &other)
    {
        return Compare(other) > 0;
    }

    bool operator<(const Momento &other)
    {
        return Compare(other) < 0;
    }

    bool operator>=(const Momento &other)
    {
        return Compare(other) >= 0;
    }

    bool operator<=(const Momento &other)
    {
        return Compare(other) <= 0;
    }
};

#endif // !MOMENTO_H