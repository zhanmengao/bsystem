#ifndef TREE23_H
#define TREE23_H
#include"TreeUtils.h"
#include"Sort.h"
#include"SortTools.h"
namespace stb {

enum NODE23_TYPE
{
    NODE23_NONE = 0,
    NODE23_2 = 2,
    NODE23_3 = 3,
    NODE23_4 = 4,
};

typedef struct
{
    NODE23_TYPE nt = NODE23_NONE;
    void* node = NULL;
}Tree23Node;

template<typename T>
class Node2
{
public:
    Tree23Node* parent;
    Tree23Node* pLeft;
    Tree23Node* pRight;
    T Data;
};

template<typename T>
class Node3
{
public:
    Tree23Node* parent;
    Tree23Node* pLeft;
    Tree23Node* pRight;
    Tree23Node* pMid;
    T LeftData;
    T RightData;
    void Build(const Node2<T>& obj)
    {
        this->parent = obj.parent;
        this->pLeft = obj.pLeft;
        this->pRight = obj.pRight;
    }
};

template<typename T>
class Node4:public Node3<T>
{
public:
    T Data4;
    Tree23Node* p4;
    void Build(const Node3<T>& obj)
    {
        this->parent = obj.parent;
        this->pLeft = obj.pLeft;
        this->pRight = obj.pRight;
        this->pMid = obj.pMid;
        this->LeftData = obj.LeftData;
        this->RightData = obj.RightData;
    }
};


template<typename T>
class Tree23
{
public:
    //插入到2节点
    //直接把2节点变成3节点即可
    Tree23Node* Insert2(Node2<T>* pNode,const T& val)
    {
        Node3<T> *newNode = new Node3<T>();
        newNode->RightData = pNode->Data>val?pNode->Data:val;
        newNode->LeftData = pNode->Data>val?val:pNode->Data;
        newNode->Build(val);

        Tree23Node *ret = new Tree23Node();
        ret->nt = NODE23_TYPE::NODE23_3;
        ret->node = newNode;
        delete pNode;
        return ret;
    }
    //插入到只有一个3节点的空树
    //将3节点变成4节点，然后将4节点（3个值）拆成3个2节点
    Tree23Node* Insert3(Node3<T>* pNode,const T& val)
    {
        T arr[] = {pNode->LeftData,pNode->RightData,val};
        BubbleSort<T>(arr,3);

        Node2<T> *root = new Node2<T>();
        Node2<T>* left = new Node2<T>();
        Node2<T>* right = new Node2<T>();

        //将3节点拆为三个2节点
        Tree23Node* nroot = new Tree23Node();
        nroot->node = root;
        nroot->nt = NODE23_2;
        Tree23Node *nleft = new Tree23Node();
        nleft->node = left;
        nleft->nt = NODE23_2;
        Tree23Node *nright = new Tree23Node();
        nright->node = right;
        nright->nt = NODE23_2;

        //中间的节点做父节点
        root->Data = arr[1];
        root->pLeft = nleft;
        root->pRight = nright;

        left->parent = nroot;
        right->parent = nroot;
        left->Data = arr[0];
        right->Data = arr[2];
    }
    //向一个父节点为2节点的3节点中插入
    //将4节点分解为两个2节点，中间值拿到父节点（父节点变为3节点）
    Tree23Node* Insert32(Tree23Node* node,const T& val)
    {
        Node3<T> *pNode = static_cast<Node3<T>*>(node->node);
        if(pNode == NULL)
        {
            return NULL;
        }
        Node2<T>* parent = static_cast<Node2<T>*>(pNode->parent->node);
        if(parent == NULL)
        {
            return NULL;
        }
        T arr[] = {pNode->LeftData,pNode->RightData,val};
        BubbleSort<T>(arr,3);

        //中间值给到父节点，使其变为3节点
        Node3<T>* newParent = new Node3<T>();
        newParent->Build(*parent);

        //是正确的逻辑，为了代码清晰，在构造子树时实现
        //newParent->LeftData = std::min(arr[1],parent->Data);
        //newParent->RightData = std::max(arr[1],parent->Data);

        //往左边插入
        if(parent->pLeft == node)
        {
            //arr[1]比原先的父节点小，所以会走到左子树执行插入
            newParent->LeftData = arr[1];
            newParent->RightData = parent->Data;

            //父节点的右孩子不变，左孩子变为2节点，新增一个中间孩子，
            Node2<T>* newLeft = new Node2<T>();
            newLeft->Data = arr[0];
            newLeft->parent = newParent;
            node->node = newLeft;
            node->nt = NODE23_2;

            Tree23Node* newMidNode = new Tree23Node();
            Node2<T>* newMid = new Node2<T>();
            newMid->Data = arr[2];
            newMid->parent = newParent;
            newMidNode->node = newMid;
            newMidNode->nt = NODE23_2;

            newParent->pMid= newMidNode;
            //大小顺序为：pLeft(arr[0]) < LeftData(arr[1])
            //              < pMid(arr[2]) < RightData（原先的父节点）
        }
        //往右边插入
        else if(parent->pRight == node)
        {
            //arr[1]比原先的父节点大，所以会走到右子树执行插入
            newParent->LeftData = parent->Data;
            newParent->RightData = arr[1];

            //父节点的左孩子不变，右孩子变为2节点，新增一个中间孩子，
            Node2<T>* newRight = new Node2<T>();
            newRight->Data = arr[2];
            newRight->parent = newParent;
            node->node = newRight;
            node->nt = NODE23_2;

            Tree23Node* newMidNode = new Tree23Node();
            Node2<T>* newMid = new Node2<T>();
            newMid->Data = arr[0];
            newMid->parent = newParent;
            newMidNode->node = newMid;
            newMidNode->nt = NODE23_2;

            newParent->pMid= newMidNode;
            //大小顺序为： LeftData（原先的父节点）< pMid(arr[0])
            //              < RightData(arr[1]) < pRight(arr[2])
        }
        delete parent;
        return newParent;
    }

    //向一个父节点为3节点的3节点中插入
    //一直向上不断分解临时的4节点，并将中值插到个更高层的父节点，直到遇到一个2节点（再将2替换为3）
    Tree23Node* Insert33(Tree23Node* node,const T& val)
    {
        Node3<T> *pNode = static_cast<Node3<T>*>(node->node);
        if(pNode == NULL)
        {
            return NULL;
        }
        Node3<T>* parent = static_cast<Node3<T>*>(pNode->parent->node);
        if(parent == NULL)
        {
            return NULL;
        }
        T arr[] = {pNode->LeftData,pNode->RightData,val};
        BubbleSort<T>(arr,3);
        //如果是左插
        if(parent->pLeft == node)
        {
            //父节点成为4节点
            Node4<T>* newParent = new Node4<T>();
            newParent->Build(parent);
            newParent->Data4 = parent->RightData;       //e
            newParent->RightData = parent->LeftData;     //d
            newParent->LeftData = arr[1];               //b

            Node2<T>* newLeft = new Node2<T>();
            newLeft->Data = arr[0];                     //a
            Tree23Node* nodeLeft = new Tree23Node();
            nodeLeft->node = newLeft;
            nodeLeft->nt = NODE23_2;

            Node2<T>* newRight = new Node2<T>();
            newRight->Data = arr[2];                    //c
            Tree23Node* nodeRight = new Tree23Node();
            nodeRight->node = newRight;
            nodeRight->nt = NODE23_2;

            newParent->p4 = parent->pRight;
            newParent->pRight = parent->pMid;
            newParent->pMid = nodeRight;
            newParent->pLeft = nodeLeft;
        }
        //如果是中插
        else if(parent->pMid == node)
        {
            //父节点成为4节点
            Node4<T>* newParent = new Node4<T>();
            newParent->Build(parent);
            newParent->Data4 = parent->RightData;           //e
            newParent->RightData = arr[1];                  //c
            newParent->LeftData = parent->LeftData;         //a

            Node2<T>* newLeft = new Node2<T>();
            newLeft->Data = arr[0];                         //b
            Tree23Node* nodeLeft = new Tree23Node();
            nodeLeft->node = newLeft;
            nodeLeft->nt = NODE23_2;

            Node2<T>* newRight = new Node2<T>();
            newRight->Data = arr[2];                        //d
            Tree23Node* nodeRight = new Tree23Node();
            nodeRight->node = newRight;
            nodeRight->nt = NODE23_2;

            newParent->p4 = parent->pRight;
            newParent->pRight = nodeRight;
            newParent->pMid = nodeLeft;
            newParent->pLeft = parent->pLeft;
        }
        //如果是右插
        else if(parent->pRight == node)
        {
            //父节点成为4节点
            Node4<T>* newParent = new Node4<T>();
            newParent->Build(parent);
            newParent->Data4 = arr[1];                          //d
            newParent->RightData = parent->RightData;           //b
            newParent->LeftData = parent->LeftData;             //a

            Node2<T>* newLeft = new Node2<T>();
            newLeft->Data = arr[0];                             //c
            Tree23Node* nodeLeft = new Tree23Node();
            nodeLeft->node = newLeft;
            nodeLeft->nt = NODE23_2;

            Node2<T>* newRight = new Node2<T>();
            newRight->Data = arr[2];                            //e
            Tree23Node* nodeRight = new Tree23Node();
            nodeRight->node = newRight;
            nodeRight->nt = NODE23_2;

            newParent->p4 = nodeRight;
            newParent->pRight = nodeLeft;
            newParent->pMid = parent->pMid;
            newParent->pLeft = parent->pLeft;
        }
    }
protected:
    Tree23Node pRoot;
};
}



#endif // TREE23_H
