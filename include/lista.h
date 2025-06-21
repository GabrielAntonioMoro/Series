#ifndef LISTA_H
#define LISTA_H

#include <functional>

template <typename Valor>
class Lista
{
public:
    struct No
    {
        Valor valor;
        No *proximo;
    };

    // Usado para comparar os valores dentro da nossa lista.
    typedef std::function<bool(Valor &)> ListaComparador;

private:
    No *inicio = nullptr;
    No *fim = nullptr;
    long long tamanho = 0;

public:
    Lista() = default;
    ~Lista()
    {
        Limpar();
    }

    /**
     * Evitamos que cópias surjam da nossa lista.
     */
    Lista(const Lista &) = delete;
    Lista &operator=(const Lista &) = delete;
    Lista(Lista &&) = default;
    Lista &operator=(Lista &&) = default;

    /**
     * @brief Insere um valor dentro da nossa lista.
     */
    No *Inserir(Valor valor)
    {
        No *novoNo = new No{valor, nullptr};

        if (IsVazio())
        {
            inicio = novoNo;
            fim = novoNo;
        }
        else
        {
            fim->proximo = novoNo;
            fim = novoNo;
        }

        tamanho++;
        return novoNo;
    }

    /**
     * @brief Remove um valor, se existir, da nossa lista
     */
    No *Remover(Valor valor)
    {
        No *atual = inicio;
        No *anterior = nullptr;

        while (atual != nullptr && atual->valor != valor)
        {
            anterior = atual;
            atual = atual->proximo;
        }

        if (atual == nullptr)
            return nullptr;

        No *proximoDoRemovido = atual->proximo;

        if (anterior == nullptr)
        {
            inicio = atual->proximo;
        }
        else
        {
            anterior->proximo = atual->proximo;
        }

        if (fim == atual)
        {
            fim = anterior;
        }

        delete atual;
        tamanho--;

        return proximoDoRemovido;
    }

    No *Remover(long long indice)
    {
        if (indice < 0 || indice >= tamanho)
            return nullptr;

        No *paraRemover = nullptr;
        tamanho--;

        if (indice == 0)
        {
            paraRemover = inicio;
            inicio = paraRemover->proximo;

            if (inicio == nullptr)
            {
                fim = nullptr;
            }
            delete paraRemover;
            return inicio;
        }

        No *anterior = inicio;
        for (long long i = 0; i < indice - 1; ++i)
        {
            anterior = anterior->proximo;
        }

        paraRemover = anterior->proximo;
        anterior->proximo = paraRemover->proximo;

        if (anterior->proximo == nullptr)
        {
            fim = anterior;
        }

        delete paraRemover;
        return anterior->proximo;
    }

    /**
     * @brief Busca e retorna o nosso valor, se existir na lista
     */
    No *Buscar(Valor valor) const
    {
        No *atual = inicio;
        while (atual != nullptr)
        {
            if (atual->valor == valor)
                return atual;
            atual = atual->proximo;
        }
        return nullptr;
    }

    /**
     * @brief Busca por um valor atráves do indice de posição dela.
     */
    No *Buscar(long long indice) const
    {
        if (indice < 0 || indice >= tamanho)
            return nullptr;

        No *atual = inicio;
        for (long long i = 0; i < indice; ++i)
            atual = atual->proximo;

        return atual;
    }

    /**
     * @brief Busca por um valor, dado um critério 'condicao'.
     */
    No *Buscar(ListaComparador condicao)
    {
        No *atual = inicio;
        while (atual != nullptr)
        {
            if (condicao(atual->valor))
                return atual;
            atual = atual->proximo;
        }
        return nullptr;
    }

    /**
     * @brief Lista todos os itens dado uma 'condicao'.
     */
    Lista<Valor> Listar(ListaComparador condicao)
    {
        Lista<Valor> lista;

        No *atual = inicio;
        while (atual != nullptr)
        {
            if (condicao(atual->valor))
                lista.Inserir(atual);
            atual = atual->proximo;
        }

        return lista;
    }

    /**
     * @brief Retorna o início da lista encadeada.
     */
    No *GetInicio() const
    {
        return this->inicio;
    }

    /**
     * @brief Retorna o tamanho da lista.
     */
    long long GetTamanho() const
    {
        return this->tamanho;
    }

    /**
    * @brief Diz se a lista está vazia.
    */
    bool IsVazio() const
    {
        return tamanho == 0;
    }

    /**
    * @brief Limpa a memória da nossa lista.
    */
    void Limpar()
    {
        No *atual = inicio;
        while (atual != nullptr)
        {
            No *proximo = atual->proximo;
            delete atual;
            atual = proximo;
        }
        inicio = nullptr;
        fim = nullptr;
        tamanho = 0;
    }
};

#endif // !LISTA_H