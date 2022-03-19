package com.symbolTable;
/*
 * 二叉查找树：
 */
public class BST<K extends Comparable<K>,V> implements ST<K,V>{
	//内部节点类
	class Node{
		private K key;
		private V value;
		private Node left;
		private Node right;
		private int N;//以该节点为根的子树中节点树
		public Node(K key,V value,Node left,Node right,int N)
		{
			this.key=key;
			this.left=left;
			this.N=N;
			this.right=right;
			this.value=value;
		}
	}
	private Node root;
	private int size;
	
	@Override
	//对外接口
	public void put(K key, V value) {
		root=put(root,key,value);		
	}
	//插入：递归算法
	private Node put(Node node,K key,V value)
	{
		//如果递归到底部了
		if(node==null)
		{
			return new Node(key,value,null,null,1);
		}else{
			//拿到当前key与要比较的key的差值
			int cmp=key.compareTo(node.key);
			//如果key比node.key小
			if(cmp<0)
			{
				//往左边找
				node.left=put(node.left, key, value);//返回左边的根节点
			}else if(cmp>0)
			{
				node.right=put(node.right,key,value);//返回右边的根节点
			}
			//如果等于 则修改
			else{
				node.value=value;
			}
			//更新n
			node.N=size(node.left)+size(node.right)+1;
			return node;
		}
	}

	@Override
	//对外的方法
	public V get(K key) {
		return get(root,key);	
	}
	//二叉查找树的递归算法(内部实现)
	private V get(Node root,K key)
	{
		if(root==null)
		{
			return null;
		}
		//与当前根比较
		int cmp=key.compareTo(root.key);
		//如果键比root大->找右子树
		if(cmp>0)
		{
			return get(root.right,key);
		}else if(cmp<0)
		{
			return get(root.left,key);
		}
		else{
			return root.value;
		}
	}

	@Override
	public int size() {
		return size(root);
	}
	private int size(Node node)
	{
		if(root==null)
			return 0;
		else
			return root.N;
	}
	public Node deleteMin()
	{
		//递归向左走
		return deleteMin(root);
	}
	private Node deleteMin(Node root)
	{
		//找到第一个没有左子树的节点
		if(root.right!=null)
		{
			//拿到min节点的右连接，替代min的位置
			root.left=deleteMin(root.left);
			root.N=size(root.left)+size(root.right)+1;
			return root;
		}else{
			//返回该节点的右连接
			return root.right;
		}
	}
	@Override
	public V remoove(K key) {
		// TODO Auto-generated method stub
		return remove(root,key).value;
	}
	private Node remove(Node x,K key)
	{
		if(x==null)
		{
			return null;
		}
		int cmp=key.compareTo(x.key);
		//如果在左子树
		if(cmp<0)
		{
			x.left=remove(x.left,key);
		}
		//如果在右子树
		else if(cmp>0)
		{
			x.right=remove(x.right,key);
		}
		//如果找到了
		else{
			//如果没有两个孩子节点，则退回到deleteMin或deleteMax的情况
			if(x.right==null) return x.left;
			if(x.left==null) return x.right;
			//如果有两个孩子，则拿到其右子树的最小节点，填充要删除的节点
			Node temp=x;
			x=min(temp.right);//x现在是要删除节点的右子树最小节点
			x.right=deleteMin(temp.right);
			x.left=temp.left;
		}
		//更新N
		x.N=size(x.left)+1+size(x.right);
		return x;
		
			
	}
	public BST<K, V>.Node min()
	{
		return min(root);
	}
	//功能：找到最小节点
	private BST<K, V>.Node min(BST<K, V>.Node x) {
		//向左走
		if(x.left==null)
			return x;
		else
			return min(x.left);
	}
	
}
