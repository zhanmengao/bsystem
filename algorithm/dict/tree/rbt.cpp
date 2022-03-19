#include"RedBlackTree.h"
using namespace stb;
int main(){
    {
        RedBlackTree tree;
        for (int i = 0; i < 10; i++)
        {
            tree.insert(i);
        }
        TreeUtils<int>().LevelOrder(tree.Root(),[](Node<int> *node)->bool
        {
            std::cout << node->Data << std::endl;
            return true;
        });
        std::cout << "High = "<<TreeUtils<int>().High(tree.Root()) << std::endl;
    }
    return 0;
}
