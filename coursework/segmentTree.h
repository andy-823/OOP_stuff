#include <iostream>
#include <vector>
#include <typeinfo>

// в этом варианте дерева отрезков имеются предпросчитанные ответы на какую то задачу
// после изменения дерева, ответы пересчитываются для изменившихся поддеревьев
// если задача немного меняется то придется менять все дерево
// пример такой задачи: сообщать количество элементов на подотрезке, большее какого то произвольного значения
// также в этой реализации дерева отрезков все операции на дереве будут рекурсивными

// простой класс для хранения результатов
template<typename T, typename ResT>
class Element
{
public:
    // необходимо для lazyTree
    // если значение ложь, то значение в узле неверное
    bool correct_value = true;
    // ответ для подотрезка
    ResT res;
    // границы подотрезка
    int64_t left_border, right_border;
    // его поддеревья
    Element<T, ResT> *right = nullptr, *left = nullptr;

    Element(ResT arg = ResT())
    {
        res = arg;
    }
    Element(ResT arg, int64_t l, int64_t r)
    {
        res = arg;
        left_border = l;
        right_border = r;
    }
};

// здесь предполагается, что тип результата совпадает с типом элементов
// и для отрезка из 1 элемента результат будет равен этому элементу
// если типы разные, то необходима функция
// функция была реализована здесь, а не в классе,
// потому что у меня не получилось избавится от ошибок
template <typename T>
T DefaultArgumentResFunction(T arg) { return arg; }

template<typename T, typename ResT>
class segmentTree
{
protected:
    std::vector<T> original_array;
    // представляет собой дерево, ответы будем получать из него
    Element<T, ResT> *tree = nullptr;
    // эта функция выдает результат для объединенных 2 отрезков
    ResT (*ResInteractionFunction)(ResT, ResT);
    // эта функция выдает результат для отрезка из 1 элемента
    ResT (*ArgumentResFunction)(T);

    // я решил строить дерево рекурсивно, поскольку такая реализация показалась мне самой простой
    // при этом ее сложность составляет O(n), где n - длина нашего массива элементов
    // но возможно здесь очень большая константа
    Element<T, ResT> *createTree(int64_t l, int64_t r)
    {
        // создаем дерево/поддерево
        Element<T, ResT> *tree = new Element<T, ResT>();

        // отрезок, за который наш элемент отвечает
        tree->left_border = l;
        tree->right_border = r;

        // это лист, для него мы моем посчитать результат
        if (l == r)
        {
            tree->res = ArgumentResFunction(original_array[l]);
            return tree;
        }

        // сначала лучше построить поддеревья
        tree->left = createTree(l, l + (r - l) / 2);
        tree->right = createTree(l + (r - l) / 2 + 1, r);

        // мы построили поддеревья и значем для них ответ
        // теперь мы можем и для нашего дерева/поддерева узнать ответ
        tree->res = ResInteractionFunction(tree->left->res, tree->right->res);
        return tree;
    }

    virtual ResT getRes(Element<T, ResT> *subtree, int64_t l, int64_t r)
    {
        if (l <= subtree->left_border && r >= subtree->right_border) return subtree->res;

        ResT res;
        if (subtree->left->right_border >= l && subtree->right->left_border <= r)
        {
            ResT res1 = getRes(subtree->left, l, r);
            ResT res2 = getRes(subtree->right, l, r);
            res = ResInteractionFunction(res1, res2);
        }
        // правое поддерево не содержит части требуемого
        else if (subtree->right->left_border > r)
            res = getRes(subtree->left, l, r);
        else
            res = getRes(subtree->right, l, r);
        return res;
    }

    // обновление происходит за линейное время
    virtual void updateTree(Element<T, ResT> *subtree, int64_t l, int64_t r)
    {
        // поддерево не включает в себя какую либо часть подотрезка
        if (subtree->right_border < l || subtree->left_border > r) return;
        if (subtree->right_border == subtree->left_border)
        {
            subtree->res = ArgumentResFunction(original_array[subtree->right_border]);
            return;
        }
        updateTree(subtree->right, l, r);
        updateTree(subtree->left, l, r);
        subtree->res = ResInteractionFunction(subtree->left->res, subtree->right->res);
    }
public:
    segmentTree(const std::vector<T> &vec, ResT(*f1)(ResT, ResT), 
        ResT(*f2)(T) = DefaultArgumentResFunction)
    {
        if (vec.size() == 0) throw std::exception();

        original_array = vec;
        ResInteractionFunction = f1;
        ArgumentResFunction = f2;

        tree = createTree(0, original_array.size() - 1);
    }

    segmentTree(const segmentTree<T, ResT> &t)
    {
        original_array = t.original_array;
        ArgumentResFunction = t.ArgumentResFunction;
        ResInteractionFunction = t.ResInteractionFunction;

        tree = createTree(0, original_array.size() - 1);
    }

    virtual ResT getRes(int64_t l, int64_t r)
    {
        // были получены некорректные значения отрезка
        if (l > tree->right_border || r < tree->left_border || l > r) throw std::exception();

        return getRes(tree, l, r);
    }

    virtual void updateTree(std::vector<T> &vec, int64_t l, int64_t r)
    {
        // были получены некорректные значения отрезка
        if (l > tree->right_border || r < tree->left_border || l > r) throw  std::exception();

        // также некорректные значения
        if (r - l + 1 > vec.size()) throw;

        for (int64_t i = l; i <= r; i++)
            original_array[i] = vec[i - l];

        updateTree(tree, l, r);
    }

    ~segmentTree()
    {
        if (tree->right) delete tree->right;
        if (tree->left) delete tree->left;

        delete tree;

        ResInteractionFunction = nullptr;
        ArgumentResFunction = nullptr;
    }
};