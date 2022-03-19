#include"BinarySearchTree.h"
using namespace stb;
int main(){

    {
        BinaryTree<int> tree;
        for (int i = 0; i < 10; i++)
        {
            tree.Insert(i);
        }
        tree.Order([](Node<int> *node)->bool
        {
            std::cout << node->Data << std::endl;
            return true;
        });
        std::cout << "High = "<<tree.High() << std::endl;
    }

    {
        BinarySearchTree<int> tree;
        for (int i = 0; i < 10; i++)
        {
            tree.Insert(i);
        }
        tree.Order([](Node<int> *node)->bool
        {
            std::cout << node->Data << std::endl;
            return true;
        });
        std::cout << "High = "<<TreeUtils<int>().High(tree.Root()) << std::endl;
    }
    {
        BinarySearchTree<int> tree;
        for (int i = 5; i < 10; i++)
        {
            tree.Insert(i);
        }
        for (int i = 0; i < 5; i++)
        {
            tree.Insert(i);
        }
        tree.Order([](Node<int> *node)->bool
        {
            std::cout << node->Data << std::endl;
            return true;
        });
        std::cout << "High = "<<TreeUtils<int>().High(tree.Root()) << std::endl;
    }
    return 0;
}
