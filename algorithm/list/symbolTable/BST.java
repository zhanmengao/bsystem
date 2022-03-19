package com.symbolTable;
/*
 * �����������
 */
public class BST<K extends Comparable<K>,V> implements ST<K,V>{
	//�ڲ��ڵ���
	class Node{
		private K key;
		private V value;
		private Node left;
		private Node right;
		private int N;//�Ըýڵ�Ϊ���������нڵ���
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
	//����ӿ�
	public void put(K key, V value) {
		root=put(root,key,value);		
	}
	//���룺�ݹ��㷨
	private Node put(Node node,K key,V value)
	{
		//����ݹ鵽�ײ���
		if(node==null)
		{
			return new Node(key,value,null,null,1);
		}else{
			//�õ���ǰkey��Ҫ�Ƚϵ�key�Ĳ�ֵ
			int cmp=key.compareTo(node.key);
			//���key��node.keyС
			if(cmp<0)
			{
				//�������
				node.left=put(node.left, key, value);//������ߵĸ��ڵ�
			}else if(cmp>0)
			{
				node.right=put(node.right,key,value);//�����ұߵĸ��ڵ�
			}
			//������� ���޸�
			else{
				node.value=value;
			}
			//����n
			node.N=size(node.left)+size(node.right)+1;
			return node;
		}
	}

	@Override
	//����ķ���
	public V get(K key) {
		return get(root,key);	
	}
	//����������ĵݹ��㷨(�ڲ�ʵ��)
	private V get(Node root,K key)
	{
		if(root==null)
		{
			return null;
		}
		//�뵱ǰ���Ƚ�
		int cmp=key.compareTo(root.key);
		//�������root��->��������
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
		//�ݹ�������
		return deleteMin(root);
	}
	private Node deleteMin(Node root)
	{
		//�ҵ���һ��û���������Ľڵ�
		if(root.right!=null)
		{
			//�õ�min�ڵ�������ӣ����min��λ��
			root.left=deleteMin(root.left);
			root.N=size(root.left)+size(root.right)+1;
			return root;
		}else{
			//���ظýڵ��������
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
		//�����������
		if(cmp<0)
		{
			x.left=remove(x.left,key);
		}
		//�����������
		else if(cmp>0)
		{
			x.right=remove(x.right,key);
		}
		//����ҵ���
		else{
			//���û���������ӽڵ㣬���˻ص�deleteMin��deleteMax�����
			if(x.right==null) return x.left;
			if(x.left==null) return x.right;
			//������������ӣ����õ�������������С�ڵ㣬���Ҫɾ���Ľڵ�
			Node temp=x;
			x=min(temp.right);//x������Ҫɾ���ڵ����������С�ڵ�
			x.right=deleteMin(temp.right);
			x.left=temp.left;
		}
		//����N
		x.N=size(x.left)+1+size(x.right);
		return x;
		
			
	}
	public BST<K, V>.Node min()
	{
		return min(root);
	}
	//���ܣ��ҵ���С�ڵ�
	private BST<K, V>.Node min(BST<K, V>.Node x) {
		//������
		if(x.left==null)
			return x;
		else
			return min(x.left);
	}
	
}
