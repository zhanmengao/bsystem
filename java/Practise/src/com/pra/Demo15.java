/*(һά���飩
 * ���ܣ�ʵ����ˮ�����Ʒ�
 * 1��ȥ����߷ֺ���ͷ֣�6��ȡƽ��
 * 2���ҳ���߷ֺ���ͷֵ���ί
 * 3���ҳ������ί�������ί
 */
package com.pra;

import java.util.Scanner;

public class Demo15 {

	public static void main(String[] args) {
		Mark m=new Mark();
		m.mark();
	}
}
//�����
class Mark{
	Scanner sr=new Scanner(System.in);
	Mark mm=new Mark();
	//������Ϊ�����ĳ�Ա������Ϊ��8������
	Judge []j=new Judge[8];
	//���
	public void mark()
	{
		for(int i=0;i<j.length;i++)
		{ 
			System.out.println("�������ֲ��к�");
			int num=sr.nextInt();
			System.out.println("������ò��д��");
			float score=sr.nextFloat();
			j[i]=new Judge(num,score); 
		}
		System.out.println();
		//����
		Mark.total(j);
		mm.score(j);
		mm.MaxAndMin(j);
		mm.MvpAndLoser(j);
	}
	//�ų��������У�����һ���������У�
	public static Judge[] total(Judge judge[])
	{

		//��������ѡ������ѡ����߷ֺ���ͷ�
		for(int i=0;i<judge.length;i++)
		{
			//����Ϊ��һ�����д�ķ�����ͷ�
			int MinNum=judge[i].getNum();
			float MinVal=judge[i].getScore();
			int MinIndex=i;		
			//��Ϊ���һ�����д�ķ�����߷֣������Ѿ����ں��棬û��Ҫ�Ƚϣ�
			int MaxNum=judge[judge.length-i-1].getNum();
			float MaxVal=judge[judge.length-i-1].getScore();
			int MaxIndex=judge.length-i-1;
			
			//��ʼ�Ƚ��ҳ����or��С
			for(int j=i+1;j<judge.length;j++)
			{
				//�����С�ıȵ�ǰ�Ĵ�
				if(MinVal>judge[j].getScore())
				{
					//��ô�����õ�ǰ�ĵ���С��
					MinNum=judge[j].getNum();
					MinVal=judge[j].getScore();
					MinIndex=j;
				}
				//������ıȵ�ǰ��С
				if(MaxVal<judge[j].getScore())
				{
					//��ô�����õ�ǰ�����
					MaxNum=judge[j].getNum();
					MaxVal=judge[j].getScore();
					MaxIndex=j;	
				}
			}
			//�ȳ��˱��ε���С(��С�����±�ΪMinIndex��
			int tempNum=judge[i].getNum();
			float tempVal=judge[i].getScore();	
			judge[i].setNum(MinNum);
			judge[i].setScore(MinVal);		
			judge[MinIndex].setNum(tempNum);
			judge[MinIndex].setScore(tempVal);
			
			//���ȳ��˱��ε���������±�ΪMaxIndex��
			tempNum=judge[judge.length-1].getNum();
			tempVal=judge[judge.length-1].getScore();
			judge[judge.length-1].setNum(MaxNum);
			judge[judge.length-1].setScore(MaxVal);
			judge[MaxIndex].setNum(tempNum);
			judge[MaxIndex].setScore(tempVal);	
		}

		return judge;
	}
	//����һ���������� ���ص÷�
	public float score(Judge judge[])
	{
		float sum=0;
		//�Ƚ����ȥ��һ����߷ֺ�һ����ͷ�
		for(int i=1;i<judge.length-1;i++)
		{
			sum+=judge[i].getScore();
		}
		return sum;
	}
	//�ҳ���߷ֺ���ͷ���ί   ����һ���������� ��ӡ��߷ֺ���ͷ�
	public void MaxAndMin(Judge judge[])
	{
		System.out.println("�����߷ֵ���ίΪ"+judge[0].getNum()+"�Ų��� ��߷�Ϊ"+judge[0].getScore());
		System.out.println("�����ͷֵ���ίΪ"+judge[judge.length-1].getNum()+"�Ų��� ��߷�Ϊ"+judge[judge.length-1].getScore());
	}
	//�ҳ������ί�������ί ����һ���������У���ӡ�����ί�������ί
	public void MvpAndLoser(Judge judge[])
	{
		float sum=this.score(judge);
		//��Ϊ��һ���������ί
		int MvpNum=judge[0].getNum();
		int MvpIndex=0;	
		float MvpVal=judge[0].getScore();
		
		//��Ϊ���һ���������ί
		int LoserNum=judge[judge.length-1].getNum();
		float LoserVal=judge[judge.length-1].getScore();
		int LoserIndex=judge.length-1;
		//�Ƚ�һ�μ���֪��˭�������ί˭�������ί
		for(int j=1;j<judge.length;j++)
		{
			//���MvpVal�Ĳ�ֵ������С
			if(Math.abs(MvpVal-sum)>Math.abs(judge[j].getScore()-sum))
			{
				//��Ϊj�����
				MvpNum=judge[j].getNum();
				MvpVal=judge[j].getScore();
				MvpIndex=j;			
			}
			//���LoserVal�Ĳ�ֵ�������
			if(Math.abs(MvpVal-sum)<Math.abs(judge[j].getScore()-sum))
			{
				//��Ϊj�����
				LoserNum=judge[j].getNum();
				LoserVal=judge[j].getScore();
				LoserIndex=j;			
			}
		}
		System.out.println("�����ί��"+MvpNum+"����ί ����������"+MvpVal);
		System.out.println("�����ί��"+LoserNum+"����ί ����������"+LoserVal);			
	}
}
//������Ϣ�ṹ��
class Judge{
	private int num;//���к�
	private float score;//���д��
	public Judge(int num,float score)
	{
		this.num=num;
		this.score=score;
	}
	
	public int getNum() {
		return num;
	}
	public void setNum(int num) {
		this.num = num;
	}
	public float getScore() {
		return score;
	}
	public void setScore(float score) {
		this.score = score;
	}		
}
