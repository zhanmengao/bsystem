package com.tree;
/*
 * 实现二叉树结构
 */
import com.StackAndQueue.*;
public class BinaryTree<T> {
	private BinaryNode<T> root;
	//通过输入
	public BinaryTree()
	{
		
	}
	//遍历二叉树->前序遍历
	//输入：头结点,初始化level为1
	//递归算法
	public void qianxubianli(BinaryNode root,int level)
	{
		if(root==null)
			return;
		else{
			System.out.println("元素值"+root.getData()+"  "+"处于第"+level+"层");//中
			qianxubianli(root.getLeft(),level+1);//左
			qianxubianli(root.getRight(),level+1);//右
		}
	}
	//层序遍历方法：利用队列
	public void cengxubianli()
	{
		//队列
		LinkedQueue<BinaryNode> queue=new LinkedQueue<BinaryNode>();
		//根节点入队列
		if(root!=null)
		{
			queue.add(root);
		}
		//队首出队列，执行逻辑，并将其左右孩子入队列
		while(!queue.IsEmpty())
		{
			//组织其出队列
			BinaryNode now=queue.remove();
			//执行当前逻辑
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
	//找到树中最大值
	//递归算法
	public int FindMax(BinaryNode<T> root)
	{
		int max=Integer.MIN_VALUE;
		int rootVar =Integer.MIN_VALUE;
		int leftMax=Integer.MIN_VALUE;
		int rightMax=Integer.MIN_VALUE;
		//递归找到每个子树中最大值
		if(root!=null)
		{
			rootVar=(int)root.getData();
			//找到每个子树的左子树最大值
			leftMax=FindMax(root.getLeft());
			//找到每个子树的右子树最大值
			rightMax=FindMax(root.getRight());			
		}
		//比较当前子树最大值
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
	//搜索树中某个元素
	//递归算法
	public boolean selectNode(BinaryNode<T> root,T data)
	{
		boolean boo=false;
		//终止条件
		if(root.getData().equals(data))
		{
			boo=true;
		}
		//递归：如果找到了 
		else if(root!=null)
		{
			//到左子树找
			boo=selectNode(root.getLeft(),data);
			if(boo==true)
			{
				return boo;
			}
			boo=selectNode(root.getRight(),data);
		}
		return boo;
	}
	//添加节点到树
	//找到最底层的空节点作为插入点 
	public boolean add(BinaryNode<T> newNode)
	{
		LinkedQueue<BinaryNode> queue=new LinkedQueue<BinaryNode>();	
		//遍历树：层序遍历方法
		if(newNode!=null)
		{
			queue.add(newNode);
		}
		
		while(!queue.IsEmpty())
		{
			BinaryNode<T> now=queue.remove();
			//如果节点为空 则插入新节点，不为空 则add到队列
			if(now.getLeft()==null)
			{
				//插入
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
	//获取树的节点->根节点的所有子树节点之和
	//递归算法
	public int SizeOfTree(BinaryNode<T> root)
	{
		if(root==null)
		{
			return 0;
		}else{
			//节点数=左子树节点数+右子树节点数
			return SizeOfTree(root.getLeft())+1+SizeOfTree(root.getRight());
		}
	}
	//删除树->递归删除每个节点
	//递归算法
	public void DeleteTree(BinaryNode<T> root)
	{
		if(root==null)
		{
			return;
		}
		else{
			//先删除子树，再删除每个子树的根节点（叶子节点是空节点的根节点）
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
	//求二叉树高度->找到根节点的左子树和右子树的最大深度
	//递归算法
	public int getHeight(BinaryNode<T> root)
	{
		int leftHeight;
		int rightHeight;
		if(root==null)
		{
			return 0;
		}
		//计算每棵子树深度
		leftHeight=getHeight(root.getLeft());
		rightHeight=getHeight(root.getRight());
		if(leftHeight>rightHeight)
		{
			return leftHeight+1;//加上根节点
		}else{
			return rightHeight+1;
		}
		
	}
	//找到二叉树最深节点->利用队列
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
		//队列最后一个元素就是最深节点
	}
	public T getDeepest()
	{
		return this.FindDeepestNode().getData();
	}
	//删除某个节点->找到节点，并找到末尾节点，将目标节点的值设置为尾节点的值，再删除尾节点，返回缓存的目标节点的值
	private  BinaryNode<T> remove(T data)
	{
		//找到目标节点（层序遍历）
		LinkedQueue<BinaryNode> queue=new LinkedQueue<BinaryNode>();
		if(root!=null)
		{
			queue.add(root);
		}
		//最深节点的引用
		BinaryNode<T> end=null;
		//所要找的节点的引用
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
		//保存原先的find
		BinaryNode<T> temp=new BinaryNode<T>();
		temp.setData(find.getData());
		temp.setLeft(find.getLeft());
		temp.setRight(find.getRight());
		//将find变成最后一个元素 然后删除最后一个元素
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
	//判断两棵树是否相同->层序遍历
	public boolean compareTo(BinaryTree<T> another)
	{
		//层序遍历
		LinkedQueue<BinaryNode> thisQue=new LinkedQueue<BinaryNode>();
		LinkedQueue<BinaryNode> anoQue=new LinkedQueue<BinaryNode>();
		//空检测
		if(this==null || another==null)
		{
			throw new NullPointerException();
		}
		//如果都是空树
		if(another.root==null & root==null)
		{
			return true;
		}
		//添加到队列
		if(root!=null && another.root!=null)
		{
			thisQue.add(root);
			anoQue.add(another.root);
		}
		while(!thisQue.IsEmpty() && !anoQue.IsEmpty())
		{
			//取出比较
			BinaryNode thisNode=thisQue.remove();
			BinaryNode anoNode=anoQue.remove();
			if(thisNode.getData()!=anoNode.getData())
			{
				return false;
			}
			//添加其左右孩子
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
		//如果不都为空（说明前面相等 后面不相等）
		if(!(thisQue.IsEmpty() && anoQue.IsEmpty()))
		{
			return false;
		}
		
		return true;
	}
	//叶子节点个数
	public int getCountLeaf()
	{
		int count=0;
		//层序遍历
		LinkedQueue<BinaryNode> queue=new LinkedQueue<BinaryNode>();
		if(root!=null)
		{
			queue.add(root);
		}
		while(!queue.IsEmpty())
		{
			BinaryNode<T> nowNode=queue.remove();
			//检测
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
	//半节点个数
	public int getCountHalfNode()
	{
		int count=0;
		//层序遍历
		LinkedQueue<BinaryNode> queue=new LinkedQueue<BinaryNode>();
		if(root!=null)
		{
			queue.add(root);
		}
		while(!queue.IsEmpty())
		{
			boolean isAdd=false;
			BinaryNode<T> nowNode=queue.remove();
			//检测
			if(nowNode.getLeft()!=null)
			{
				queue.add(nowNode.getLeft());
			}
			//如果左节点为空，则标志位为true
			else{
				isAdd=true;
			}
			if(nowNode.getRight()!=null)
			{
				queue.add(nowNode.getRight());
			}else{
				isAdd=true;
			}
			//如果右节点不存在且标志位为false
			if(isAdd==false){
				count++;
			}
		}	
		return count;
	}
	//求所有节点数据之和（递归）--double可用
	@SuppressWarnings("unused")
	public double sum(BinaryNode<Double> root)
	{
		if(root==null)
			return 0;
		return root.getData()+sum(root.getLeft())+sum(root.getRight());
	}
	//镜像
	public void MirrorOfBinaryTree()
	{
		MirrorOfBinaryTree(root);
	}
	private void MirrorOfBinaryTree(BinaryNode<T> root)
	{
		//遍历
		BinaryNode<T> temp=null;
		if(root!=null)		
		{
			//先交换
			temp=root.getLeft();
			root.setLeft(root.getRight());
			root.setRight(temp);	
			//递归地往下找
			MirrorOfBinaryTree(root.getLeft());
			MirrorOfBinaryTree(root.getRight());
		}
	}
	//根据前序遍历和中序遍历重建二叉树
	public BinaryTree<T> BuildBinaryTree(int intOrder[],int preOrder[],int start,int end)
	{
		
		return null;
	}
	//二叉树节点中的公共祖先
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
	private T data;//本身的值
	private BinaryNode left;//左孩子
	private BinaryNode right;//右孩子

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
