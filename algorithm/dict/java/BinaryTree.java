package com.tree;
/*
 * ʵ�ֶ������ṹ
 */
import com.StackAndQueue.*;
public class BinaryTree<T> {
	private BinaryNode<T> root;
	//ͨ������
	public BinaryTree()
	{
		
	}
	//����������->ǰ�����
	//���룺ͷ���,��ʼ��levelΪ1
	//�ݹ��㷨
	public void qianxubianli(BinaryNode root,int level)
	{
		if(root==null)
			return;
		else{
			System.out.println("Ԫ��ֵ"+root.getData()+"  "+"���ڵ�"+level+"��");//��
			qianxubianli(root.getLeft(),level+1);//��
			qianxubianli(root.getRight(),level+1);//��
		}
	}
	//����������������ö���
	public void cengxubianli()
	{
		//����
		LinkedQueue<BinaryNode> queue=new LinkedQueue<BinaryNode>();
		//���ڵ������
		if(root!=null)
		{
			queue.add(root);
		}
		//���׳����У�ִ���߼������������Һ��������
		while(!queue.IsEmpty())
		{
			//��֯�������
			BinaryNode now=queue.remove();
			//ִ�е�ǰ�߼�
			System.out.println(now.getData());
			if(now.getLeft()!=null)
			{
				queue.add(now.getLeft());
			}
			if(now.getRight()!=null)
			{
				queue.add(now.getRight());
			}
		}
		
	}
	//�ҵ��������ֵ
	//�ݹ��㷨
	public int FindMax(BinaryNode<T> root)
	{
		int max=Integer.MIN_VALUE;
		int rootVar =Integer.MIN_VALUE;
		int leftMax=Integer.MIN_VALUE;
		int rightMax=Integer.MIN_VALUE;
		//�ݹ��ҵ�ÿ�����������ֵ
		if(root!=null)
		{
			rootVar=(int)root.getData();
			//�ҵ�ÿ�����������������ֵ
			leftMax=FindMax(root.getLeft());
			//�ҵ�ÿ�����������������ֵ
			rightMax=FindMax(root.getRight());			
		}
		//�Ƚϵ�ǰ�������ֵ
		if(leftMax>rightMax)
		{
			max=leftMax;
		}else{
			max=rightMax;
		}
		if(max<rootVar)
		{
			max=rootVar;
		}
		return max;
	}
	//��������ĳ��Ԫ��
	//�ݹ��㷨
	public boolean selectNode(BinaryNode<T> root,T data)
	{
		boolean boo=false;
		//��ֹ����
		if(root.getData().equals(data))
		{
			boo=true;
		}
		//�ݹ飺����ҵ��� 
		else if(root!=null)
		{
			//����������
			boo=selectNode(root.getLeft(),data);
			if(boo==true)
			{
				return boo;
			}
			boo=selectNode(root.getRight(),data);
		}
		return boo;
	}
	//��ӽڵ㵽��
	//�ҵ���ײ�Ŀսڵ���Ϊ����� 
	public boolean add(BinaryNode<T> newNode)
	{
		LinkedQueue<BinaryNode> queue=new LinkedQueue<BinaryNode>();	
		//�������������������
		if(newNode!=null)
		{
			queue.add(newNode);
		}
		
		while(!queue.IsEmpty())
		{
			BinaryNode<T> now=queue.remove();
			//����ڵ�Ϊ�� ������½ڵ㣬��Ϊ�� ��add������
			if(now.getLeft()==null)
			{
				//����
				now.setLeft(newNode);
				return true;
			}
			else
			{
				queue.add(now.getLeft());
			}
			if(now.getRight()==null)
			{
				now.setRight(newNode);
				return true;
			}
			else
			{
				queue.add(now.getRight());
			}
		}
		return false;
	}
	//��ȡ���Ľڵ�->���ڵ�����������ڵ�֮��
	//�ݹ��㷨
	public int SizeOfTree(BinaryNode<T> root)
	{
		if(root==null)
		{
			return 0;
		}else{
			//�ڵ���=�������ڵ���+�������ڵ���
			return SizeOfTree(root.getLeft())+1+SizeOfTree(root.getRight());
		}
	}
	//ɾ����->�ݹ�ɾ��ÿ���ڵ�
	//�ݹ��㷨
	public void DeleteTree(BinaryNode<T> root)
	{
		if(root==null)
		{
			return;
		}
		else{
			//��ɾ����������ɾ��ÿ�������ĸ��ڵ㣨Ҷ�ӽڵ��ǿսڵ�ĸ��ڵ㣩
			if(root.getLeft()!=null)
			{
				DeleteTree(root.getLeft());
			}
			if(root.getRight()!=null)
			{
				DeleteTree(root.getRight());
			}
			root=null;	
		}
	}
	//��������߶�->�ҵ����ڵ������������������������
	//�ݹ��㷨
	public int getHeight(BinaryNode<T> root)
	{
		int leftHeight;
		int rightHeight;
		if(root==null)
		{
			return 0;
		}
		//����ÿ���������
		leftHeight=getHeight(root.getLeft());
		rightHeight=getHeight(root.getRight());
		if(leftHeight>rightHeight)
		{
			return leftHeight+1;//���ϸ��ڵ�
		}else{
			return rightHeight+1;
		}
		
	}
	//�ҵ�����������ڵ�->���ö���
	private BinaryNode<T> FindDeepestNode()
	{
		LinkedQueue<BinaryNode> queue=new LinkedQueue<BinaryNode>();
		BinaryNode<T> end=null;
		if(root!=null)
		{
			queue.add(root);
		}
		while(!queue.IsEmpty())
		{
			end=queue.remove();
			if(end.getLeft()!=null)
			{
				queue.add(end.getLeft());
			}
			if(end.getRight()!=null)
			{
				queue.add(end.getRight());
			}
		}
		return end;
		//�������һ��Ԫ�ؾ�������ڵ�
	}
	public T getDeepest()
	{
		return this.FindDeepestNode().getData();
	}
	//ɾ��ĳ���ڵ�->�ҵ��ڵ㣬���ҵ�ĩβ�ڵ㣬��Ŀ��ڵ��ֵ����Ϊβ�ڵ��ֵ����ɾ��β�ڵ㣬���ػ����Ŀ��ڵ��ֵ
	private  BinaryNode<T> remove(T data)
	{
		//�ҵ�Ŀ��ڵ㣨���������
		LinkedQueue<BinaryNode> queue=new LinkedQueue<BinaryNode>();
		if(root!=null)
		{
			queue.add(root);
		}
		//����ڵ������
		BinaryNode<T> end=null;
		//��Ҫ�ҵĽڵ������
		BinaryNode<T> find=null;
		
		while(!queue.IsEmpty())
		{
			end=queue.remove();
			if(end.getData()==data)
			{
				find=end;
			}
			if(end.getLeft()!=null)
			{
				queue.add(end.getLeft());
			}
			if(end.getRight()!=null)
			{
				queue.add(end.getRight());
			}
		}
		//����ԭ�ȵ�find
		BinaryNode<T> temp=new BinaryNode<T>();
		temp.setData(find.getData());
		temp.setLeft(find.getLeft());
		temp.setRight(find.getRight());
		//��find������һ��Ԫ�� Ȼ��ɾ�����һ��Ԫ��
		if(find!=null)
		{
			find.setData(end.getData());
			end=null;
		}
		return temp;
	}
	public T removeT(T data)
	{
		return this.remove(data).getData();
	}
	//�ж��������Ƿ���ͬ->�������
	public boolean compareTo(BinaryTree<T> another)
	{
		//�������
		LinkedQueue<BinaryNode> thisQue=new LinkedQueue<BinaryNode>();
		LinkedQueue<BinaryNode> anoQue=new LinkedQueue<BinaryNode>();
		//�ռ��
		if(this==null || another==null)
		{
			throw new NullPointerException();
		}
		//������ǿ���
		if(another.root==null & root==null)
		{
			return true;
		}
		//��ӵ�����
		if(root!=null && another.root!=null)
		{
			thisQue.add(root);
			anoQue.add(another.root);
		}
		while(!thisQue.IsEmpty() && !anoQue.IsEmpty())
		{
			//ȡ���Ƚ�
			BinaryNode thisNode=thisQue.remove();
			BinaryNode anoNode=anoQue.remove();
			if(thisNode.getData()!=anoNode.getData())
			{
				return false;
			}
			//��������Һ���
			if(thisNode.getLeft()!=null)
			{
				thisQue.add(thisNode.getLeft());
			}
			if(thisNode.getRight()!=null)
			{
				thisQue.add(thisNode.getRight());
			}
			if(anoNode.getLeft()!=null)
			{
				anoQue.add(anoNode.getLeft());
			}
			if(anoNode.getRight()!=null)
			{
				anoQue.add(anoNode.getRight());
			}	
		}
		//�������Ϊ�գ�˵��ǰ����� ���治��ȣ�
		if(!(thisQue.IsEmpty() && anoQue.IsEmpty()))
		{
			return false;
		}
		
		return true;
	}
	//Ҷ�ӽڵ����
	public int getCountLeaf()
	{
		int count=0;
		//�������
		LinkedQueue<BinaryNode> queue=new LinkedQueue<BinaryNode>();
		if(root!=null)
		{
			queue.add(root);
		}
		while(!queue.IsEmpty())
		{
			BinaryNode<T> nowNode=queue.remove();
			//���
			if(nowNode.getLeft()==null && nowNode.getRight()==null)
			{
				count++;
				continue;
			}
			
			if(nowNode.getLeft()!=null)
			{
				queue.add(nowNode.getLeft());
			}
			if(nowNode.getRight()!=null)
			{
				queue.add(nowNode.getRight());
			}
		}	
		return count;
	}
	//��ڵ����
	public int getCountHalfNode()
	{
		int count=0;
		//�������
		LinkedQueue<BinaryNode> queue=new LinkedQueue<BinaryNode>();
		if(root!=null)
		{
			queue.add(root);
		}
		while(!queue.IsEmpty())
		{
			boolean isAdd=false;
			BinaryNode<T> nowNode=queue.remove();
			//���
			if(nowNode.getLeft()!=null)
			{
				queue.add(nowNode.getLeft());
			}
			//�����ڵ�Ϊ�գ����־λΪtrue
			else{
				isAdd=true;
			}
			if(nowNode.getRight()!=null)
			{
				queue.add(nowNode.getRight());
			}else{
				isAdd=true;
			}
			//����ҽڵ㲻�����ұ�־λΪfalse
			if(isAdd==false){
				count++;
			}
		}	
		return count;
	}
	//�����нڵ�����֮�ͣ��ݹ飩--double����
	@SuppressWarnings("unused")
	public double sum(BinaryNode<Double> root)
	{
		if(root==null)
			return 0;
		return root.getData()+sum(root.getLeft())+sum(root.getRight());
	}
	//����
	public void MirrorOfBinaryTree()
	{
		MirrorOfBinaryTree(root);
	}
	private void MirrorOfBinaryTree(BinaryNode<T> root)
	{
		//����
		BinaryNode<T> temp=null;
		if(root!=null)		
		{
			//�Ƚ���
			temp=root.getLeft();
			root.setLeft(root.getRight());
			root.setRight(temp);	
			//�ݹ��������
			MirrorOfBinaryTree(root.getLeft());
			MirrorOfBinaryTree(root.getRight());
		}
	}
	//����ǰ���������������ؽ�������
	public BinaryTree<T> BuildBinaryTree(int intOrder[],int preOrder[],int start,int end)
	{
		
		return null;
	}
	//�������ڵ��еĹ�������
	public BinaryNode<T> LCA(BinaryNode<T> root,BinaryNode<T> a,BinaryNode<T> b)
	{
		BinaryNode<T> left,right;
		if(root==null)
		{
			return root;
		}
		if(root==a || root==b)
		{
			return root;
		}
		left=LCA(root.getLeft(),a,b);
		right=LCA(root.getRight(),a,b);
		if(left==a && right==b) return root;
		else
			return (left==a?left:right);
	} 
	
	
	
	
	
}
class BinaryNode<T>{
	private T data;//�����ֵ
	private BinaryNode left;//����
	private BinaryNode right;//�Һ���

	public T getData() {
		return data;
	}

	public BinaryNode getLeft() {
		return left;
	}

	public void setLeft(BinaryNode left) {
		this.left = left;
	}

	public BinaryNode getRight() {
		return right;
	}

	public void setRight(BinaryNode right) {
		this.right = right;
	}

	public void setData(T data) {
		this.data = data;
	}
	
}
