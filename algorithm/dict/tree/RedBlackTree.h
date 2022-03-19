#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H
#include"TreeUtils.h"
#include<memory>
#include<functional>
namespace stb {
#define RED true            //红 == 3节点
#define BLACK false         //黑 == 2节点
typedef bool RBColor;
template<typename T>
class RedBlackNode:public Node<T>
{
public:
    RBColor color = BLACK;
    RedBlackNode()
    {
    }
    RedBlackNode(const T& rData,RBColor color):color(color)
    {
        Node<T>::Data = rData;
    }
public:
    inline RedBlackNode<T>* GetLeft()
    {
        return static_cast<RedBlackNode<T>*>(this->pLeft);
    }
    inline void SetLeft(RedBlackNode<T>* pNode)
    {
        this->pLeft = static_cast<RedBlackNode<T>*>(pNode);
    }
    inline RedBlackNode<T>* GetRight()
    {
        return static_cast<RedBlackNode<T>*>(this->pRight);
    }
    inline void SetRight(RedBlackNode<T>* pNode)
    {
        this->pRight = static_cast<RedBlackNode<T>*>(pNode);
    }
    inline bool IsRed() const
    {
        return color == RED;
    }
};


class RedBlackTree
{
    using RNode = RedBlackNode<int>;
    typedef RNode* RNodePtr;
public:
    void insert(const int& val)
    {
        root = insert(RNodePtr(root),val);
        root->color = BLACK;
    }
    RNode* Root()
    {
        return root;
    }
private:
    //插入规则：用红链接与新节点相连
    RNodePtr insert(RNodePtr h,const int& val)
    {
        if(h == NULL)
        {
            return RNodePtr(new RNode(val,RED));
        }
        //这里和二叉搜索树一致
        if(h->Data > val)
        {
            RNodePtr newLeft = insert(RNodePtr(h->GetLeft()),val);
            h->SetLeft(newLeft);
        }
        else if(h->Data < val)
        {
            RNodePtr newRight = insert(RNodePtr(h->GetRight()),val);
            h->SetRight(newRight);
        }
        else
        {
            h->Data = val;
        }

        //如果底部是2节点（黑）
        //左插：变成3节点 <--> 变红
        //右插：变成3节点 <--> 变红 + 右旋

        //如果底部是3节点（红）d - b （前序遍历）
        //将4节点分解为两个2节点，中间值拿到父节点
        //新键最大：被插到右边 --》变色 d b e
        //新键最小：被插到孩子的左边，此时左边有两个连续的红节点 d b a(左直线) --》 右旋d -> b a d ，变色
        //新键在中间：被插到孩子的右边，此时左边有两个连续的红节点d b c
        //                          --》左旋b -> d c b（左直线） ，右旋d -> c b d ，变色

		//红链接均为左链接（3节点特征）
        //如果右子节点是红色，而左子节点是黑色，进行左旋
        if(IsRed(h->GetRight()) && ! IsRed(h->GetLeft()))
        {
            h = rotateLeft(h);
        }
        //如果连着两条红，右旋（没有任何一个节点同时与两条红链接相连）
        if(IsRed(h->GetLeft()) && IsRed(h->GetLeft()->GetLeft()))
        {
            h = rotateRight(h);
        }
        //双红，转色
        if(IsRed(h->GetLeft()) && IsRed(h->GetRight()))
        {
            flipColor(h);
        }
        return h;
    }
private:
    bool IsRed(RNode* h)
    {
        if(h == NULL)
        {
            return false;
        }
        return h->IsRed();
    }
    void flipColor(RNodePtr h)
    {
        h->color = RED;
        h->GetLeft()->color = BLACK;
        h->GetRight()->color = BLACK;
    }

    //简单理解为 h和h的右孩子交换位置，新根为h的右孩子
    //由于右孩子大于h，所以新根的左孩子指向老根
    RNodePtr rotateLeft(RNodePtr h)
    {
        RNodePtr x = RNodePtr(h->GetRight());         //h的右孩子是x

        //h右孩子（x） 和 x左孩子 互换
        h->SetRight(x->GetLeft());
        x->SetLeft(h);                  //x的左孩子指向原先的根

        x->color = h->color;
        h->color = RED;

        return x;

    }
    //简单理解为 h和h的左孩子交换位置，新根为h的左孩子
    //由于左孩子小于h，所以新根的右孩子指向老根
    RNodePtr rotateRight(RNodePtr h)
    {
        RNodePtr x = RNodePtr(h->GetLeft());          //h的左孩子是x

        //h左孩子（x） 和 x右孩子 互换
        h->SetLeft(x->GetRight());
        x->SetRight(h);             //x的右孩子指向原先的根

        x->color = h->color;
        h->color = RED;
        return x;
    }
private:
    RNode* root = NULL;
};

void mainRBT()
{

}
}


#endif // REDBLACKTREE_H
