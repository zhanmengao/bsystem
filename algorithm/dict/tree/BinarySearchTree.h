#pragma once
#include"BinaryTree.h"

namespace stb
{
template<class T>
class BinarySearchTree
{
public:
    //����
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
    //��ȡ��indexС��
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
    //find�ݹ�汾
    node_point Find(node_point node, const value_type& val)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        //ֵС�ڽڵ㣬������
        if (node->pData > val)
        {
            return Find(node->pLeft, val);
        }
        //ֵ���ڽڵ㣬������
        else if (node->pData < val)
        {
            return Find(node->pRight, val);
        }
        //����
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
        //��ȡ��ǰ�ڵ��λ�ã�����С����n����
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
        //ֵС�ڽڵ㣬����������
        else if (node->Data > val)
        {
            node->pLeft = Insert(node->pLeft, val);
        }
        //ֵС�ڽڵ㣬����������
        else  if (node->Data < val)
        {
            node->pRight = Insert(node->pRight, val);
        }
        //�ҵ���ȷ��λ�ã��ݹ鷵��
        return node;
    }

    //һֱ����
    node_point Min(node_point node)
    {
        if (node == nullptr || node->pLeft == nullptr)
        {
            return node;
        }
        return Min(node->pLeft);
    }
    //һֱ����
    node_point Max(node_point node)
    {
        if (node == nullptr || node->pRight == nullptr)
        {
            return node;
        }
        return Max(node->pRight);
    }
    //ɾ��val�����������е���С����
    node_point Delete(node_point root, const value_type& val)
    {
        if (root == nullptr)
        {
            return nullptr;
        }
        else if (root->Data == val)
        {
            //ֻ��һ�����ӵ����
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
            //�����������ڵ����
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
