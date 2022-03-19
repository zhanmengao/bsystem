/*(一维数组）
 * 功能：实现跳水比赛计分
 * 1、去掉最高分和最低分，6个取平均
 * 2、找出最高分和最低分的评委
 * 3、找出最佳评委和最差评委
 */
package com.pra;

import java.util.Scanner;

public class Demo15 {

	public static void main(String[] args) {
		Mark m=new Mark();
		m.mark();
	}
}
//打分类
class Mark{
	Scanner sr=new Scanner(System.in);
	Mark mm=new Mark();
	//裁判作为打分类的成员，我认为有8个裁判
	Judge []j=new Judge[8];
	//打分
	public void mark()
	{
		for(int i=0;i<j.length;i++)
		{ 
			System.out.println("请输入打分裁判号");
			int num=sr.nextInt();
			System.out.println("请输入该裁判打分");
			float score=sr.nextFloat();
			j[i]=new Judge(num,score); 
		}
		System.out.println();
		//排序
		Mark.total(j);
		mm.score(j);
		mm.MaxAndMin(j);
		mm.MvpAndLoser(j);
	}
	//排出有序序列（输入一个无序序列）
	public static Judge[] total(Judge judge[])
	{

		//这里我用选择排序法选出最高分和最低分
		for(int i=0;i<judge.length;i++)
		{
			//我认为第一个裁判打的分是最低分
			int MinNum=judge[i].getNum();
			float MinVal=judge[i].getScore();
			int MinIndex=i;		
			//认为最后一个裁判打的分是最高分（最大的已经放在后面，没必要比较）
			int MaxNum=judge[judge.length-i-1].getNum();
			float MaxVal=judge[judge.length-i-1].getScore();
			int MaxIndex=judge.length-i-1;
			
			//开始比较找出最大or最小
			for(int j=i+1;j<judge.length;j++)
			{
				//如果最小的比当前的大
				if(MinVal>judge[j].getScore())
				{
					//那么我们让当前的当最小的
					MinNum=judge[j].getNum();
					MinVal=judge[j].getScore();
					MinIndex=j;
				}
				//如果最大的比当前的小
				if(MaxVal<judge[j].getScore())
				{
					//那么我们让当前的最大
					MaxNum=judge[j].getNum();
					MaxVal=judge[j].getScore();
					MaxIndex=j;	
				}
			}
			//比出了本次的最小(最小的数下标为MinIndex）
			int tempNum=judge[i].getNum();
			float tempVal=judge[i].getScore();	
			judge[i].setNum(MinNum);
			judge[i].setScore(MinVal);		
			judge[MinIndex].setNum(tempNum);
			judge[MinIndex].setScore(tempVal);
			
			//还比出了本次的最大（最大的下标为MaxIndex）
			tempNum=judge[judge.length-1].getNum();
			tempVal=judge[judge.length-1].getScore();
			judge[judge.length-1].setNum(MaxNum);
			judge[judge.length-1].setScore(MaxVal);
			judge[MaxIndex].setNum(tempNum);
			judge[MaxIndex].setScore(tempVal);	
		}

		return judge;
	}
	//输入一个有序序列 返回得分
	public float score(Judge judge[])
	{
		float sum=0;
		//比较完后，去掉一个最高分和一个最低分
		for(int i=1;i<judge.length-1;i++)
		{
			sum+=judge[i].getScore();
		}
		return sum;
	}
	//找出最高分和最低分评委   输入一个有序序列 打印最高分和最低分
	public void MaxAndMin(Judge judge[])
	{
		System.out.println("打出最高分的评委为"+judge[0].getNum()+"号裁判 最高分为"+judge[0].getScore());
		System.out.println("打出最低分的评委为"+judge[judge.length-1].getNum()+"号裁判 最高分为"+judge[judge.length-1].getScore());
	}
	//找出最佳评委和最差评委 输入一个有序序列，打印最佳评委和最差评委
	public void MvpAndLoser(Judge judge[])
	{
		float sum=this.score(judge);
		//认为第一个是最佳评委
		int MvpNum=judge[0].getNum();
		int MvpIndex=0;	
		float MvpVal=judge[0].getScore();
		
		//认为最后一个是最差评委
		int LoserNum=judge[judge.length-1].getNum();
		float LoserVal=judge[judge.length-1].getScore();
		int LoserIndex=judge.length-1;
		//比较一次即可知道谁是最佳评委谁是最差评委
		for(int j=1;j<judge.length;j++)
		{
			//如果MvpVal的差值不是最小
			if(Math.abs(MvpVal-sum)>Math.abs(judge[j].getScore()-sum))
			{
				//认为j是最佳
				MvpNum=judge[j].getNum();
				MvpVal=judge[j].getScore();
				MvpIndex=j;			
			}
			//如果LoserVal的差值不是最大
			if(Math.abs(MvpVal-sum)<Math.abs(judge[j].getScore()-sum))
			{
				//认为j是最佳
				LoserNum=judge[j].getNum();
				LoserVal=judge[j].getScore();
				LoserIndex=j;			
			}
		}
		System.out.println("最佳评委是"+MvpNum+"号评委 他的评分是"+MvpVal);
		System.out.println("最差评委是"+LoserNum+"号评委 他的评分是"+LoserVal);			
	}
}
//裁判信息结构体
class Judge{
	private int num;//裁判号
	private float score;//裁判打分
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
