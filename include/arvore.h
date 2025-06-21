#ifndef ARVORE_H
#define ARVORE_H

#include <lista.h>
#include <functional>

template <typename Chave, typename Valor>
class Arvore
{
public:
    // Estrutura do Nó da árvore.
    typedef struct No
    {
        Chave chave;
        Valor valor;

        struct No *esquerda;
        struct No *direita;
    } No;

    /*
     * @brief É uma função definida como variável, para comparação de nós, caso seja necessário
     * especificar a comparação entre chaves de cada nó.
     */
    typedef std::function<bool(Chave)> ArvoreComparador;
    typedef std::function<int(Chave&,Chave&)> ArvoreBuscador;

private:
    No *raiz;

    /**
     * @brief Insere recursivamente um novo nó na sub-árvore.
     */
    No *inserirRecursivo(No *no, Chave &chave, Valor &valor)
    {
        if (no == nullptr)
            return new No{chave, valor, nullptr, nullptr};

        if (chave < no->chave)
            no->esquerda = inserirRecursivo(no->esquerda, chave, valor);
        else if (chave > no->chave)
            no->direita = inserirRecursivo(no->direita, chave, valor);
        else
            // Se a chave já existe, apenas atualizamos o valor.
            no->valor = valor;

        return no;
    }

    /**
     * @brief Busca recursivamente por um nó com a chave especificada.
     */
    No *buscarRecursivo(No *no, Chave &chave)
    {
        // Chave não encontrada.
        if (no == nullptr)
            return no;

        if (chave < no->chave)
            return buscarRecursivo(no->esquerda, chave);
        else if (chave > no->chave)
            return buscarRecursivo(no->direita, chave);
        else
            return no;
    }

    /**
     * @brief Busca recursivamente por um nó com a chave especificada e comparador especificado.
     */
    No *buscarRecursivo(No *no, Chave &chave, ArvoreBuscador comparador)
    {
        if (no == nullptr)
            return no;

        int comp = comparador(chave, no->chave);
        if (comp > 0)
            return buscarRecursivo(no->direita, chave);
        else if (comp < 0)
            return buscarRecursivo(no->esquerda, chave);
        else
            return no;
    }

    /**
     * @brief Encontra o nó com a menor chave na sub-árvore (o mais à esquerda).
     */
    No *encontrarMenor(No *no)
    {
        No *atual = no;
        while (atual && atual->esquerda != nullptr)
            atual = atual->esquerda;

        return atual;
    }

    /**
     * @brief Remove de maneira recursiva um nó com a chave especificada.
     */
    No *removerRecursivo(No *no, Chave &chave)
    {
        if (no == nullptr)
            return no; // Chave não encontrada.

        // Navega até o nó a ser removido.
        if (chave < no->chave)
            no->esquerda = removerRecursivo(no->esquerda, chave);
        else if (chave > no->chave)
            no->direita = removerRecursivo(no->direita, chave);

        else
        {
            // Nó com a chave encontrado.
            if (no->esquerda == nullptr)
            {
                No *temp = no->direita;
                delete no;
                return temp;
            }
            else if (no->direita == nullptr)
            {
                No *temp = no->esquerda;
                delete no;
                return temp;
            }

            No *temp = encontrarMenor(no->direita);

            no->chave = temp->chave;
            no->valor = temp->valor;

            no->direita = removerRecursivo(no->direita, temp->chave);
        }
        return no;
    }

    /**
     * @brief Destrói (desaloca) a árvore recursivamente em pós-ordem.
     */
    void destruirRecursivo(No *no)
    {
        if (no != nullptr)
        {
            destruirRecursivo(no->esquerda);
            destruirRecursivo(no->direita);
            delete no;
        }
    }

    void listarRecursivo(No* no, Lista<No*>* l, ArvoreComparador* b) {
        if(no != nullptr) {
            listarRecursivo(no->esquerda, l, b);

            if((*b)(no->chave))
                l->Inserir(no);

            listarRecursivo(no->direita, l, b);
        }
    }

public:
    Arvore() : raiz(nullptr) {}
    ~Arvore()
    {
        destruirRecursivo(raiz);
    }

    /** 
    * @brief Faz a inserção de um novo nó na árvore.
    */
    No *Inserir(Chave &chave, Valor &valor)
    {
        raiz = inserirRecursivo(raiz, chave, valor);
        return Buscar(chave); // Retorna o nó para consistência.
    }

    /**
     * @brief Faz a remoção de um nó da árvore.
     */
    void Remover(Chave &chave)
    {
        raiz = removerRecursivo(raiz, chave);
    }

    /**
     * @brief Busca por um nó da nossa árvore, atráves de uma chave.
     */
    No *Buscar(Chave &chave)
    {
        return buscarRecursivo(raiz, chave);
    }

    No *Buscar(Chave &chave, ArvoreBuscador buscador)
    {
        return buscarRecursivo(raiz, chave, buscador);
    }

    /**
    * @brief Lista todos os itens dado um comparador.
    */
    Lista<No*> Listar(ArvoreComparador comparador) {
        Lista<No*> lista;
        listarRecursivo(raiz, &lista, &comparador);
        return lista;
    }

    /**
    * @brief Retorna a raiz da árvore
    */
    No *GetRaiz()
    {
        return this->raiz;
    }

    /**
    * @brief Retorna se a árvore está vazia.
    */
    bool IsVazia()
    {
        return raiz == nullptr;
    }

};

#endif // !ARVORE_H