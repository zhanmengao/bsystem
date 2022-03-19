#ifndef TREEUTILS_H
#define TREEUTILS_H
#include"Queue.h"
#include"List.hpp"
#include<functional>

namespace stb
{
template<typename T>
class TreeUtils
{
public:
#define value_type T
#define value_point T*
#define node_point Node<value_type> *
#define TreeCallBack std::function<bool(node_point)>
     //层序遍历
    void LevelOrder(node_point node, TreeCallBack call)
    {
        if (node == nullptr)
        {
            return;
        }
        Queue<node_point> queue;
        queue.Push(node);
        while (!queue.Empty())
        {
            node = queue.Front();
            queue.Pop();
            if (call != nullptr)
            {
                //如果返回false,则不再往下遍历
                if (!call(node))
                {
                    return;
                }
            }
            if (node->pLeft != nullptr)
            {
                queue.Push(node->pLeft);
            }
            if (node->pRight != nullptr)
            {
                queue.Push(node->pRight);
            }
        }
    }

    node_point LCA(node_point root, node_point a, node_point b)
    {
        if (root == nullptr)
        {
            return nullptr;
        }
        //找到该节点了，返回
        if (root == a || root == b)
        {
            return root;
        }
        node_point left = LCA(root->pLeft, a, b);
        node_point right = LCA(root->pRight, a, b);
        //如果分别在左右子树找到ab，则该节点为root
        if (left && right)
        {
            return root;
        }
        //如果都在左子树
        else if (left)
        {
            return left;
        }
        //如果都在右子树
        else if (right)
        {
            return right;
        }
        return nullptr;
    }

    void Mirror(node_point pNode)
    {
        if (pNode != nullptr)
        {
            if (pNode->pLeft != nullptr)
            {
                Mirror(pNode->pLeft);
            }
            if (pNode->pRight != nullptr)
            {
                Mirror(pNode->pRight);
            }
            node_point tmp = pNode->pLeft;
            pNode->pLeft = pNode->pRight;
            pNode->pRight = tmp;
        }
    }
    unsigned High(node_point pNode)
    {
        if (pNode == nullptr)
        {
            return 0;
        }
        else
        {
            unsigned leftHeight = High(pNode->pLeft);
            unsigned rightHeight = High(pNode->pRight);
            return  leftHeight > rightHeight ? leftHeight : rightHeight + 1;
        }
    }
    unsigned Size(node_point pNode)
    {
        if (pNode == nullptr)
        {
            return 0;
        }
        else
        {
            return Size(pNode->pLeft) + Size(pNode->pRight) + 1;
        }
    }
    void Clear(node_point* pNode)
    {
        if (pNode == nullptr || *pNode == nullptr)
        {
            return;
        }
        Clear(pNode->pLeft);
        Clear(pNode->pRight);
        ClearNode(&pNode);
    }

    void ClearNode(node_point* node)
    {
        if (node != nullptr && *node != nullptr)
        {
            delete (*node);
            *node = nullptr;
        }
    }
    node_point CreateNode(const value_type& val)
    {
        node_point pNode = new Node<value_type>();
        pNode->Data = val;
        return pNode;
    }
    //遍历函数规则：返回false则结束遍历
    //前序遍历：根，左，右
    bool PreOrder(node_point node, TreeCallBack call)
    {
        if (node == nullptr)
        {
            return true;
        }
        if (call != nullptr)
        {
            if (!call(node))
            {
                return false;
            }
        }
        if (!PreOrder(node->pLeft, call))
        {
            return false;
        }
        if (!PreOrder(node->pRight, call))
        {
            return false;
        }
        return true;
    }
    //前序遍历：非递归
    void PreOrderNonRecursive(node_point node, TreeCallBack call)
    {
        Stack<node_point> stack;
        while (true)
        {
            //为空说明当前节点的左子树遍历完成
            while (node != nullptr)
            {
                //先处理当前节点
                if (call != nullptr)
                {
                    if (!call(node))
                    {
                        return;
                    }
                }
                stack.Push(node);
                //遍历左边
                node = node->pLeft;
            }
            if (stack.Empty())
            {
                break;
            }
            node = stack.Back();
            stack.Pop();
            node = node->pRight;
        }
    }

    //中序遍历：左，根，右
    bool InOrder(node_point node, TreeCallBack call)
    {
        if (node == nullptr)
        {
            return true;
        }
        if (!InOrder(node->pLeft, call))
        {
            return false;
        }
        if (call != nullptr)
        {
            if (!call(node))
            {
                return false;
            }
        }
        if (!InOrder(node->pRight, call))
        {
            return false;
        }
        return true;
    }
    //中序遍历：非递归
    void InOrderNonRecursive(node_point node, TreeCallBack call)
    {
        Stack<node_point> stack;
        while (true)
        {
            //为空说明当前节点的左子树遍历完成
            while (node != nullptr)
            {
                stack.Push(node);
                //先遍历左边
                node = node->pLeft;
            }
            if (stack.Empty())
            {
                break;
            }
            node = stack.Back();
            stack.Pop();
            //处理当前节点（中）
            if (call != nullptr)
            {
                if (!call(node))
                {
                    return;
                }
            }
            node = node->pRight;
        }
    }

    //后序遍历：左，右，根
    bool PostOrder(node_point node, TreeCallBack call)
    {
        if (node == nullptr)
        {
            return true;
        }
        if (!PostOrder(node->pLeft, call))
        {
            return false;
        }
        if (!PostOrder(node->pRight, call))
        {
            return false;
        }
        if (call != nullptr)
        {
            if (!call(node))
            {
                return false;
            }
        }
        return true;
    }

    unsigned HighNonRecursive(node_point node)
    {
        unsigned level = 0;
        Queue<node_point> q;
        node_point pNode = node;
        if (pNode != nullptr)
        {
            q.Push(pNode);
            q.Push(nullptr);
        }
        while (!q.Empty())
        {
            pNode = q.Front();
            q.Pop();
            //如果是一个结束标记
            if (pNode == nullptr)
            {
                level++;
                if (!q.Empty())
                {
                    q.Push(nullptr);		///如果不是最后一层,则给下一层添加一个结束标记
                }
            }
            else
            {
                if (pNode->pLeft)
                {
                    q.Push(pNode->pLeft);
                }
                if (pNode->pRight)
                {
                    q.Push(pNode->pRight);
                }
            }
        }
        return level;
    }
};
}


#endif // TREEUTILS_H
