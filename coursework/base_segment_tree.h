#ifndef BASE_SEGMENT_TREE_INCLUDED
#define BASE_SEGMENT_TREE_INCLUDED

#include <vector>

template<typename T>
struct tree_element
{
    T value;
    std::size_t right, left;

    bool correct = true; // necessary for lazy tree
    T lazy_param;
};

template<typename T, typename ResT>
class base_segment_tree
{
public:
    base_segment_tree() = default;
    ~base_segment_tree() = default;

    // creating
    void build_tree(const std::vector<T> &values, ResT (*res_function_to_set)(ResT&, ResT&), 
        ResT (*argument_res_function_to_set)(T&));
    void build_tree(const std::vector<T> &values, ResT (*res_function_to_set)(ResT&, ResT&));

    // needed interface
    // update has o(n) difficulty here, for better perfomance you are to write new one
    // i don't know why but if i don't redefine it in heirs, program doesn't compile
    virtual void update_tree(const std::vector<T> &values, const std::size_t &from, const std::size_t &where);
    virtual ResT get_answer(const std::size_t &from, const std::size_t &where);

protected:
    void build_tree(const std::size_t &index);
    void calculate_tree_element_value(const std::size_t &index);
    // must be redefined in heirs
    virtual void update_tree(const std::size_t &index, const std::size_t &from, const std::size_t &where) = 0;
    virtual ResT get_answer(const std::size_t &index, const std::size_t &from, const std::size_t &where) = 0;

    std::vector<T> original_array;
    std::vector<tree_element<ResT>> tree;
    ResT (*res_function)(ResT&, ResT&);
    // result for single element
    ResT (*argument_res_function)(T&);
};


template<typename T, typename ResT>
void base_segment_tree<T, ResT>::build_tree(const std::vector<T> &values, ResT (*res_function_to_set)(ResT&, ResT&), 
    ResT (*argument_res_function_to_set)(T&))
{
    if (values.size() == 0)
    {
        throw std::exception(); // "failed to build tree: got empty array");
    }

    res_function = res_function_to_set;
    argument_res_function = argument_res_function_to_set;

    original_array = values;
    tree.resize(3 * original_array.size() + 1);

    tree[0].left = 0;
    tree[0].right = original_array.size() - 1;
    build_tree(0);
    calculate_tree_element_value(0);
}

template<typename T, typename ResT>
void base_segment_tree<T, ResT>::build_tree(const std::vector<T> &values, ResT (*res_function_to_set)(ResT&, ResT&))
{
    // I failed to create something that would work with default param
    build_tree(values, res_function_to_set, [](T &arg){ return arg; });
}

template<typename T, typename ResT>
void base_segment_tree<T, ResT>::build_tree(const std::size_t &index)
{
    if (tree[index].left == tree[index].right)
    {
        return;
    }
    // we add 1 to index to work with positions in tree and decrease by 1 to return to indexes
    std::size_t left_index = 2 * (index + 1) - 1, right_index = 2 * (index + 1);

    tree[left_index].left = tree[index].left;
    tree[left_index].right = (tree[index].left + tree[index].right) / 2;
    build_tree(left_index);
    calculate_tree_element_value(left_index);

    tree[right_index].left = tree[left_index].right + 1;
    tree[right_index].right = tree[index].right;
    build_tree(right_index);
    calculate_tree_element_value(right_index);
}

template <typename T, typename ResT>
void base_segment_tree<T, ResT>::update_tree(const std::vector<T> &values, const std::size_t &from, const std::size_t &where)
{
    if (from > where)
    {
        throw std::exception(); // "failed to update: incorrect indexes");
    }
    if (where - from + 1 != values.size())
    {
        throw std::exception(); // "failed to update: mismatch of indexes with array size");
    }
    if (where >= original_array.size())
    {
        throw std::exception(); // "failed to update: indexes out of bounds");
    }

    for (std::size_t i = from; i <= where; i++)
    {
        original_array[i] = values[i - from];
    }
    update_tree(0, from, where);
}

template <typename T, typename ResT>
ResT base_segment_tree<T, ResT>::get_answer(const std::size_t &from, const std::size_t &where)
{
    if (from > where)
    {
        throw std::exception(); // "failed to get answer: incorrect indexes");
    }
    if (where >= original_array.size())
    {
        throw std::exception(); // "failed to get answer: indexes out of bounds");
    }

    return get_answer(0, from, where);
}

template <typename T, typename ResT>
void base_segment_tree<T, ResT>::calculate_tree_element_value(const std::size_t &index)
{
    // we suppose that index is correct
    if (tree[index].left != tree[index].right)
    {
        try
        {
            tree[index].value = res_function(tree[2 * (index + 1) - 1].value, tree[2 * (index + 1)].value);
        }
        catch(...)
        {
            throw std::exception(); // "failed to set value: check_res_function");
        }
    }
    else
    {
        try
        {
            tree[index].value = argument_res_function(original_array[tree[index].left]);
        }
        catch(...)
        {
            throw std::exception(); // "failed to set value: check argument_res_function");
        }
    }
}

#endif // BASE_SEGMENT_TREE_INCLUDED