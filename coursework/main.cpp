#include "lazyTree.h"

#include <random>
// нужные штуки уже вклчены в дерево отрезков
// я не стал их включать еще раз

int sum(int a, int b) { return a + b; }

void TestGet(int to)
{
    std::cout << "performing testing get with integers from 1 to "<< to << "\n";
    std::vector<int> v(to);
    int count_first = 0, count_second = 0, total_tests = 0;
    for (int i = 0; i < to; i++) v[i] = i + 1;

    segmentTree<int, int> tree1(v, sum);
    lazyTree<int, int> tree2(v, sum);

    int res_correct, res_first, res_second;
    for (int i = 0; i < to; i++)
    for (int j = i; j < to; j++)
    {
        total_tests++;
        try
        {
            // from i + 1 to j + 1
            res_correct = 0;
            for (int k = i; k <= j; k++)
                    res_correct += v[k];
            res_first = tree1.getRes(i, j);
            res_second = tree2.getRes(i, j);

            count_first = res_first == res_correct ? count_first + 1 : count_first;
            count_second = res_second == res_correct ? count_second + 1 : count_second;
        }
        catch(...)
        {
            std::cout << "i = " << i << ", j = " << j << "\n"; 
        }   
    }
    std::cout << "segmentTree successful tests: " << count_first << " from " << total_tests << "\n";
    std::cout << "lazyTree successful tests: " << count_second << " from " << total_tests << "\n";
}

void testUpdate(int to)
{
    std::cout << "\nperforming testing update with integers from 1 to "<< to << "\n\n";
    std::vector<int> v(to);
    for (int i = 0; i < to; i++) v[i] = i + 1;

    segmentTree<int, int> tree1(v, sum);
    lazyTree<int, int> tree2(v, sum);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::cout << "gonna perform " << to << " tests\n";
    for (int i = 0; i < to; i++)
    {
        int first = gen() % to, second = gen() % 10;
        if (first > second) std::swap(first, second);
        std::vector<int> v2(second - first + 1, i + 1);
        std::cout << "test " << i + 1<< ": replacing from " << first 
            << " to " << second << " with " << i + 1 << "\n";

        tree1.updateTree(v2, first, second);
        tree2.updateTree(v2, first, second);

        for (int i = first; i <= second; i++) v[i] = v2[i - first];
        for (auto &a : v) std::cout << a << " ";
        std::cout << "\n";

        int count_first = 0, count_second = 0, total_tests = 0;
        int res_correct, res_first, res_second;
        for (int i = 0; i < to; i++)
        for (int j = i; j < to; j++)
        {
            total_tests++;
            try
            {   
                res_correct = 0;
                // from i + 1 to j + 1
                for (int k = i; k <= j; k++)
                    res_correct += v[k];

                res_first = tree1.getRes(i, j);
                res_second = tree2.getRes(i, j);

                count_first = res_first == res_correct ? count_first + 1 : count_first;
                count_second = res_second == res_correct ? count_second + 1 : count_second;
            }
            catch(...)
            {
                std::cout << "i = " << i << ", j = " << j << "\n"; 
            }   
        }
        std::cout << "segmentTree successful tests: " << count_first << " from " << total_tests << "\n";
        std::cout << "lazyTree successful tests: " << count_second << " from " << total_tests << "\n\n";   
    }
}

// тест на объекте сложной природы
// я решил провести его на векторах
// вывести вектор наибольшей длины и его длину на каком то отрезке
std::pair<std::vector<int>, int> len(std::vector<int> v) { return { v, v.size()}; }
std::pair<std::vector<int>, int> compare(std::pair<std::vector<int>, int> p1, std::pair<std::vector<int>, int> p2)
{ 
    if (p1.second > p2.second)
        return p1;
    return p2;
}

typedef std::pair<std::vector<int>, int> hard_type;

void testHard(int to)
{
    std::cout << "\nperforming testing with vector from 1 to "<< to << "\n\n";
    std::vector<std::vector<int>> v(to);

    for (int i = 0; i < to; i++) v[i] = std::vector<int>(i + 1, i + 1);

    segmentTree<std::vector<int>, hard_type> tree1(v, compare, len);
    lazyTree<std::vector<int>, hard_type>    tree2(v, compare, len);


    int count_first = 0, count_second = 0, total_tests = 0;
    hard_type res_first, res_second;
    for (int i = 0; i < to; i++)
    for (int j = i; j < to; j++)
    {
        total_tests++;
        try
        {   
            res_first = tree1.getRes(i, j);
            res_second = tree2.getRes(i, j);

            count_first = res_first.second == j + 1 ? count_first + 1 : count_first;
            count_second = res_second.second == j + 1 ? count_second + 1 : count_second;
        }
        catch(...)
        {
            std::cout << "i = " << i << ", j = " << j << "\n"; 
        }   
    }
    std::cout << "segmentTree successful tests: " << count_first << " from " << total_tests << "\n";
    std::cout << "lazyTree successful tests: " << count_second << " from " << total_tests << "\n\n";   
}

int main()
{
    TestGet(100);
    testUpdate(20);
    testHard(10);

    system("pause");

    return 0;
}