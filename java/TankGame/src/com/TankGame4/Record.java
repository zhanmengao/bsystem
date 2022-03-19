/*
 * 记录类
 */
package com.TankGame4;
import java.util.*;
import java.io.*;

import javax.sound.sampled.*;

//播放声音的类
class AePlayWave extends Thread {
	private String filename;
	public AePlayWave(String wavfile) {
		filename = wavfile;
	}

	public void run() {
		File soundFile = new File(filename);
		AudioInputStream audioInputStream = null;
		try {
			audioInputStream = AudioSystem.getAudioInputStream(soundFile);
		} catch (Exception e1) {
			e1.printStackTrace();
			return;
		}

		AudioFormat format = audioInputStream.getFormat();
		SourceDataLine auline = null;
		DataLine.Info info = new DataLine.Info(SourceDataLine.class, format);

		try {
			auline = (SourceDataLine) AudioSystem.getLine(info);
			auline.open(format);
		} catch (Exception e) {
			e.printStackTrace();
			return;
		}

		auline.start();
		int nBytesRead = 0;
		//这是缓冲
		byte[] abData = new byte[2048];

		try {
			while (nBytesRead != -1) {
				nBytesRead = audioInputStream.read(abData, 0, abData.length);
				if (nBytesRead >= 0)
					auline.write(abData, 0, nBytesRead);
			}
		} catch (IOException e) {
			e.printStackTrace();
			return;
		} finally {
			auline.drain();
			auline.close();
		}
	}
}

//一个记录点类，记录点类有坐标和方向
class Node{
	int x=0;
	int y=0;
	int direct=0;
	public Node(int x,int y,int direct)
	{
		this.x=x;
		this.y=y;
		this.direct=direct;
	}
}

public class Record {

	public static int getResidue() {
		return residue;
	}
	public static void setResidue(int residue) {
		Record.residue = residue;
	}
	public static int getMylife() {
		return Mylife;
	}
	public static void setMylife(int mylife) {
		Mylife = mylife;
	}
	public static int getKill() {
		return kill;
	}
	public static void setKill(int kill) {
		Record.kill = kill;
	}
	public static int getTotal() {
		return total;
	}
	public static void setTotal(int total) {
		Record.total = total;
	}
	public Vector<Enemy> getV() {
		return v;
	}
	public void setV(Vector<Enemy> v) {
		this.v = v;
	}
	//记录类应该有：本关敌人坦克数量
	//我的坦克还有几条命
	//我击杀了几台敌人坦克（右）
	//积分
	private static int residue=20;
	private static int Mylife=3;
	private static int kill=0;
	private static int total=0;
	//让记录类知道外面的世界
	private static Vector<Enemy> v=new Vector<Enemy>();
	
	//让外界的世界知道记录类
	private static Vector<Node> node=new Vector<Node>();
	
	//类方法：每击杀一个敌人5级坦克获得100分（在击杀时调用）

	public static void killenemy()
	{
		residue--;
		kill++;
		total+=100;
	}
	//类方法：我被击杀
	public static void killme()
	{
		Mylife--;
	}
	//类方法：恢复总分
	public static void get_total()
	{
		FileReader fr=null;
		BufferedReader br=null;

			try {
				fr=new FileReader("F:\\坦克大战记录文件.txt");
				br=new BufferedReader(fr);

				//读总分文件（第一行）
				try {
					String s=br.readLine();
					//接收总分
					total=Integer.parseInt(s);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally{
				try {
					br.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				try {
					fr.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}	
	}
	//类方法：记录数据到文件
	public static void Write()
	{
		FileWriter fw=null;
		BufferedWriter bw=null;
		try {
			fw=new FileWriter("F:\\坦克大战记录文件.txt");
			bw=new BufferedWriter(fw);
			System.out.println("开始写入");
			//记录当前总分
			bw.write(total+"\r\n");
			//获取向量里面的坦克的x,y和方向，一次写一个 一个写一行
			//此时Record里面的向量v，已经是外界坦克向量
			for(int i=0;i<v.size();i++)
			{
				Enemy em=v.get(i);
				String s=em.getX()+" "+em.getY()+" "+em.getDirect()+"\r\n";
				bw.write(s);
			}
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}finally{
			try {
				bw.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			try {
				fw.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
	}
	//类方法：获取文件中的坐标 返回读到的节点向量
	public static Vector<Node> getNode()
	{
		FileReader fr=null;
		BufferedReader br=null;
			try {
				fr=new FileReader("F:\\坦克大战记录文件.txt");
				br=new BufferedReader(fr);
				System.out.println("开始读取数据");
				String s="";
				
				//此时数据是一行一行的
				//先把第一行：总分读出来
				s=br.readLine();
				total=Integer.parseInt(s);
				//再按行读出下面的内容
				while((s=br.readLine())!=null)
				{
					//写一个字符数组，以" "切割本次读到的坐标和方向（文件内容为：x y direct）
					String xyd[]=s.split(" ");
					//构造一个Node类对象
					Node n=new Node(Integer.parseInt(xyd[0]),Integer.parseInt(xyd[1]),Integer.parseInt(xyd[2]));
					//把n放入向量
					node.add(n);
				}
				System.out.println("读取完毕");
			} catch (Exception e) {
				// TODO Auto-generated catch block
				System.out.println("读档失败！");
				e.printStackTrace();
			}finally{
				try {
					br.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				try {
					fr.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			return node;
		}
	}





