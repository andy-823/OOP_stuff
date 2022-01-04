#include "segmentTree.h"

template<typename T, typename ResT> 
class lazyTree : public segmentTree<T, ResT>
{
private:
    // здесь обновляется результат для какого то поддерева, которое мы сюда передали
    // предполагается, что на всем отрезке он неверен
    void updateTree(Element<T, ResT> *subtree)
    {
        // обновляем результат для узла
        subtree->res = this->ArgumentResFunction(this->original_array[subtree->left_border]);
        for (int64_t i = subtree->left_border + 1; i <= subtree->right_border; i++)
            subtree->res = this->ResInteractionFunction(subtree->res, this->ArgumentResFunction(this->original_array[i]));
        
        // результат стал корректным для поддерева
        // но остается некорректным для его потомков
        subtree->correct_value = true;
        if (subtree->left_border == subtree->right_border) return; // потомков нет
        subtree->right->correct_value = false;
        subtree->left->correct_value = false;
    }

    // рекурсивная функция для выявления узлов, лежащих в подотрезке
    virtual void updateTree(Element<T,ResT> *subtree, int64_t l, int64_t r)
    {
        // может оказаться так, что ранее дерево мы уже обновляли
        // допустим, мы пришли в узел, в котором correct_value == false
        // тогда дальнейшая работа в данном поддереве не имеет смысла, тк
        // у нас такое правило: если мы в процессе получения результата пришли
        // в узел с некорректным значением, то мы обновляем для узла значение
        // и говорим, что в его потомках тоже значение неверное
        // данный подотрезок не входит в "неверный"
        if (subtree->left_border > r || subtree->right_border < l) return;
        // теперь основной случай: подотрезок, за который отвечает наш узел,
        // полностью находится в неверном
        if (subtree->left_border >= l && subtree->right_border <= r || !subtree->correct_value)
        {
            updateTree(subtree);
            return;
        }
        // ни один из случаев выше не подошел
        updateTree(subtree->left, l, r);
        updateTree(subtree->right, l, r);
        // обновили результат для потомков, теперь и для данного можно обновить
        subtree->res = this->ResInteractionFunction(subtree->left->res, subtree->right->res);
    }

    // здесь мы работаем так: если мы пришли в подотрезок, в котором значение некорректное
    // то мы это значение обновляем
    // и идем дальше
    // если какой тои з потомков не входит в нужный нам подотрезок
    // то мы в него не идем
    virtual ResT getRes(Element<T, ResT> *subtree, int64_t l, int64_t r)
    {
        if (!subtree->correct_value) updateTree(subtree);
        if (subtree->left_border >= l && subtree->right_border <= r)
            return subtree->res;
        
        ResT res;
        // у нас хотя бы одно поддерево содержит часть необходимого
        // оба поддерева включают необходимый отрезок
        if (subtree->left->right_border >= l && subtree->right->left_border <= r)
        {
            ResT res1 = getRes(subtree->left, l, r);
            ResT res2 = getRes(subtree->right, l, r);
            res = this->ResInteractionFunction(res1, res2);
        }
        // правое поддерево не содержит части требуемого
        else if (subtree->right->left_border > r)
            res = getRes(subtree->left, l, r);
        else
            res = getRes(subtree->right, l, r);

        return res;
    }    
public:
    lazyTree(const std::vector<T> &vec, ResT(*f1)(ResT, ResT), 
        ResT(*f2)(T) = DefaultArgumentResFunction) : segmentTree<T, ResT>(vec, f1, f2) {}
    lazyTree(const lazyTree<T, ResT> &t) : segmentTree<T, ResT>(t) {}

    // идея следующая: выявить поддеревья составляющие отрезок
    // и обновить для них значения
    // и соответсвтенно для родителей тоже
    // мы не будем обновлять значение для их детей
    // сложность все равно получается O(len * m * k + log n * m)
    // len - длина обновляемого отрезка
    // m - асимптотика функции объединения результатов
    // k - асимптотика функции получения результата для единичного отрезка
    // n - длина исходного массива
    // быстрее сделать не получится, тк будем либо спускаться вниз
    // и получим примерно такую же асимпотику, но с большей константой
    // либо мы будем обновлять какие то поддеревья, которые перед отрезком
    // но в итоге будет проделано больше операций
    // если бы была более конкретная задача, например увеличение чисел на каком то
    // отрезке (мы можем делать только замену, тк разные объекты имеют разные свойства,
    // и мы, очевидно, их не можем знать), то мы бы проводили такое обновление за log n
    virtual void updateTree(const std::vector<T> &vec, int64_t l, int64_t r)
    {
        // были получены некорректные значения отрезка
        if (l > this->tree->right_border || r < this->tree->left_border || l > r) throw std::exception();

        // также некорректные значения
        if (r - l + 1 > this->original_array.size()) throw std::exception();

        for (int64_t i = l; i <= r; i++)
            this->original_array[i] = vec[i - l];

        // поддеревья пометим когда будем получать результат
        updateTree(this->tree, l, r);
    }      

    ResT getRes(int64_t l, int64_t r)
    {
        // были получены некорректные значения отрезка
        if (l > this->tree->right_border || r < this->tree->left_border || l > r) throw std::exception();

        return getRes(this->tree, l , r);
    }
};