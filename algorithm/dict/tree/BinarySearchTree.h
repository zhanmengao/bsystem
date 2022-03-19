#pragma once
#include"BinaryTree.h"

namespace stb
{
template<class T>
class BinarySearchTree
{
public:
    //遍历
    void Order(TreeCallBack call)
    {
        TreeUtils<T>().LevelOrder(pRoot, call);
    }
public:
    virtual node_point Find(const value_type& val)
    {
        node_point node = pRoot;
        while (node)
        {
            if (node->Data > val)
            {
                node = node->pLeft;
            }
            else if (node->Data < val)
            {
                node = node->pRight;
            }
            else
            {
                return node;
            }
        }
        return nullptr;
    }
    //获取第index小的
    node_point At(unsigned index)
    {

    }
    virtual void Insert(const value_type& val)
    {
        Insert(pRoot, val);
    }
    virtual void Delete(const value_type& val)
    {
        Delete(pRoot, val);
    }
    virtual void Delete(node_point node)
    {
        Delete(pRoot, node->Data);
    }

    bool Empty() const
    {
        return pRoot == nullptr;
    }
    node_point Root()
    {
        return pRoot;
    }
protected:
    //find递归版本
    node_point Find(node_point node, const value_type& val)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        //值小于节点，往左走
        if (node->pData > val)
        {
            return Find(node->pLeft, val);
        }
        //值大于节点，往右走
        else if (node->pData < val)
        {
            return Find(node->pRight, val);
        }
        //命中
        else
        {
            return node;
        }
    }
    node_point At(node_point node, unsigned index)
    {
        if(node == nullptr)
        {
            return nullptr;
        }
        //获取当前节点的位置（比他小的有n个）
        unsigned s = TreeUtils<T>().Size(pRoot->pLeft);
        //
        if(s > index)
        {

        }
    }
    node_point Insert(node_point node, const value_type& val)
    {
        if (node == nullptr)
        {
            node = TreeUtils<T>().CreateNode(val);
            if (Empty())
            {
                pRoot = node;
            }
        }
        //值小于节点，往左子树放
        else if (node->Data > val)
        {
            node->pLeft = Insert(node->pLeft, val);
        }
        //值小于节点，往右子树放
        else  if (node->Data < val)
        {
            node->pRight = Insert(node->pRight, val);
        }
        //找到正确的位置，递归返回
        return node;
    }

    //一直向左
    node_point Min(node_point node)
    {
        if (node == nullptr || node->pLeft == nullptr)
        {
            return node;
        }
        return Min(node->pLeft);
    }
    //一直向右
    node_point Max(node_point node)
    {
        if (node == nullptr || node->pRight == nullptr)
        {
            return node;
        }
        return Max(node->pRight);
    }
    //删除val，用左子树中的最小代替
    node_point Delete(node_point root, const value_type& val)
    {
        if (root == nullptr)
        {
            return nullptr;
        }
        else if (root->Data == val)
        {
            //只有一个孩子的情况
            if (!root->pLeft)
            {
                node_point pDel = root;
                root = root->pRight;
                TreeUtils<T>().ClearNode(&pDel);
            }
            else if (!root->pRight)
            {
                node_point pDel = root;
                root = root->pLeft;
                TreeUtils<T>().ClearNode(&pDel);
            }
            //拿左子树最大节点代替
            else if (root->pLeft && root->pRight)
            {
                auto pMax = Max(root->pLeft);
                root->Data = pMax->Data;
                root->pLeft = Delete(root->pLeft, root->Data);
            }
        }
    }

protected:
    node_point pRoot = nullptr;
};
}
