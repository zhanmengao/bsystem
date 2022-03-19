#pragma once

#ifdef __cplusplus
#include"TreeUtils.h"
#include"Stack.h"
#include"Queue.h"
#include<functional>

namespace stb
{
template<class T>
class BinaryTree
{
protected:
    node_point pRoot = nullptr;
public:
    //遍历
    void Order(TreeCallBack call)
    {
        TreeUtils<T>().LevelOrder(pRoot, call);
    }
    virtual void Insert(const value_type& val)
    {
        node_point pNode = TreeUtils<T>().CreateNode(val);
        if (pRoot == nullptr)
        {
            pRoot = pNode;
            return;
        }
        TreeUtils<T>().LevelOrder(pRoot, [&](node_point node)->bool
        {
            if (node->pLeft == nullptr)
            {
                node->pLeft = pNode;
                return false;
            }
            else if (node->pRight == nullptr)
            {
                node->pRight = pNode;
                return false;
            }
            return true;
        });
    }
    void Delete(const value_type& value)
    {
        auto pNode = Find(value);
        if (pNode != nullptr)
        {
            Delete(pNode);
        }
    }
    //用最后一个节点替换该节点
    virtual void Delete(node_point node)
    {
        if (node == nullptr)
        {
            return;
        }
        if (pRoot == node)
        {
            delete pRoot;
            pRoot = nullptr;
            return;
        }
        auto pEnd = End();
        if (pEnd == nullptr)
        {
            return;
        }
        node->Data = pEnd->Data;
        auto parent = GetParent(pEnd);
        if (parent == nullptr)
        {
            return;
        }
        if (parent->pLeft == node)
        {
            parent->pLeft = nullptr;
        }
        else if (parent->pRight == node)
        {
            parent->pRight = nullptr;
        }
        TreeUtils<T>().ClearNode(&node);
    }
    virtual node_point Find(const value_type& val)
    {
        node_point ret = nullptr;
        TreeUtils<T>().LevelOrder(pRoot, [&](node_point node)->bool
        {
            if (node->Data == val)
            {
                ret = node;
                return false;
            }
            return true;
        });
        return ret;
    }
public:
    void Clear()
    {
        TreeUtils<T>().Clear(pRoot);
    }
    bool Empty() const
    {
        return pRoot == nullptr;
    }
    unsigned Size()
    {
        unsigned mRet = 0;
        TreeUtils<T>().PreOrder(pRoot, [&](node_point node)->bool
        {
            mRet++;
            return false;
        });
        return mRet;
    }
    unsigned High()
    {
        unsigned level = 0;
        Queue<node_point> q;
        node_point pNode = pRoot;
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
    node_point Begin()
    {
        return pRoot;
    }
    node_point End()
    {
        if (pRoot == nullptr)
        {
            return nullptr;
        }
        Stack<node_point> s;
        TreeUtils<T>().LevelOrder(pRoot, [&](node_point node)->bool
        {
            s.Push(node);
            return true;
        });
        return s.Back();
    }

    node_point GetParent(node_point child)
    {
        node_point ret = nullptr;
        TreeUtils<T>().LevelOrder(pRoot, [&](node_point node)->bool
        {
            if (node->pLeft == child || node->pRight == node)
            {
                ret = node;
                return false;
            }
            return true;
        });
        return ret;
    }
    node_point LCA(node_point a, node_point b)
    {
        return TreeUtils<T>().LCA(pRoot, a, b);
    }
    void Mirror()
    {
        TreeUtils<T>().Mirror(pRoot);
    }
protected:
};

}

#endif
