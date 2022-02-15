#include "base_segment_tree.h"

template <typename T, typename ResT>
class segment_tree : public base_segment_tree<T, ResT>
{
public:
    // i don't know why but if i don't redefine it, program doesn't compile
    void update_tree(const std::vector<T> &values, const std::size_t &from, const std::size_t &where);
    ResT get_answer(const std::size_t &from, const std::size_t &where);
    
protected:
    void update_tree(const std::size_t &index, const std::size_t &from, const std::size_t &where);
    ResT get_answer(const std::size_t &index, const std::size_t &from, const std::size_t &where);
};

template <typename T, typename ResT>
void segment_tree<T, ResT>::update_tree(const std::vector<T> &values, const std::size_t &from, const std::size_t &where)
{
    base_segment_tree<T, ResT>::update_tree(values, from, where);
}

template <typename T, typename ResT>
void segment_tree<T, ResT>::update_tree(const std::size_t &index, const std::size_t &from, const std::size_t &where)
{
    if (this->tree[index].left > where || this->tree[index].right < from)
    {
        return;
    }
    if (this->tree[index].left != this->tree[index].right)
    {
        update_tree(2 * (index + 1) - 1, from, where);
        update_tree(2 * (index + 1), from, where);
    }
    this->calculate_tree_element_value(index);
}

template <typename T, typename ResT>
ResT segment_tree<T, ResT>::get_answer(const std::size_t &from, const std::size_t &where)
{
    return base_segment_tree<T, ResT>::get_answer(from, where);
}

template <typename T, typename ResT>
ResT segment_tree<T, ResT>::get_answer(const std::size_t &index, const std::size_t &from, const std::size_t &where)
{
    if (from <= this->tree[index].left && this->tree[index].right <= where)
    {
        return this->tree[index].value;
    }

    ResT result;
    std::size_t left_index = 2 * (index + 1) - 1, right_index = 2 * (index + 1);
    // both left and right segments include parts of needed
    if (from <= this->tree[left_index].right && this->tree[right_index].left <= where)
    {
        ResT res1 = get_answer(left_index, from, where),
             res2 = get_answer(right_index, from, where);
        result = this->res_function(res1, res2);
    }
    // left segment doesn't include any part of needed 
    else if (this->tree[right_index].left > where)
    {
        result = get_answer(left_index, from, where);
    }
    else
    {
        result = get_answer(right_index, from, where);
    }
    return result;
}