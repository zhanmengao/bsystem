/*
 * ��¼��
 */
package com.TankGame4;
import java.util.*;
import java.io.*;

import javax.sound.sampled.*;

//������������
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
		//���ǻ���
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

//һ����¼���࣬��¼����������ͷ���
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
	//��¼��Ӧ���У����ص���̹������
	//�ҵ�̹�˻��м�����
	//�һ�ɱ�˼�̨����̹�ˣ��ң�
	//����
	private static int residue=20;
	private static int Mylife=3;
	private static int kill=0;
	private static int total=0;
	//�ü�¼��֪�����������
	private static Vector<Enemy> v=new Vector<Enemy>();
	
	//����������֪����¼��
	private static Vector<Node> node=new Vector<Node>();
	
	//�෽����ÿ��ɱһ������5��̹�˻��100�֣��ڻ�ɱʱ���ã�

	public static void killenemy()
	{
		residue--;
		kill++;
		total+=100;
	}
	//�෽�����ұ���ɱ
	public static void killme()
	{
		Mylife--;
	}
	//�෽�����ָ��ܷ�
	public static void get_total()
	{
		FileReader fr=null;
		BufferedReader br=null;

			try {
				fr=new FileReader("F:\\̹�˴�ս��¼�ļ�.txt");
				br=new BufferedReader(fr);

				//���ܷ��ļ�����һ�У�
				try {
					String s=br.readLine();
					//�����ܷ�
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
	//�෽������¼���ݵ��ļ�
	public static void Write()
	{
		FileWriter fw=null;
		BufferedWriter bw=null;
		try {
			fw=new FileWriter("F:\\̹�˴�ս��¼�ļ�.txt");
			bw=new BufferedWriter(fw);
			System.out.println("��ʼд��");
			//��¼��ǰ�ܷ�
			bw.write(total+"\r\n");
			//��ȡ���������̹�˵�x,y�ͷ���һ��дһ�� һ��дһ��
			//��ʱRecord���������v���Ѿ������̹������
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
	//�෽������ȡ�ļ��е����� ���ض����Ľڵ�����
	public static Vector<Node> getNode()
	{
		FileReader fr=null;
		BufferedReader br=null;
			try {
				fr=new FileReader("F:\\̹�˴�ս��¼�ļ�.txt");
				br=new BufferedReader(fr);
				System.out.println("��ʼ��ȡ����");
				String s="";
				
				//��ʱ������һ��һ�е�
				//�Ȱѵ�һ�У��ֶܷ�����
				s=br.readLine();
				total=Integer.parseInt(s);
				//�ٰ��ж������������
				while((s=br.readLine())!=null)
				{
					//дһ���ַ����飬��" "�и�ζ���������ͷ����ļ�����Ϊ��x y direct��
					String xyd[]=s.split(" ");
					//����һ��Node�����
					Node n=new Node(Integer.parseInt(xyd[0]),Integer.parseInt(xyd[1]),Integer.parseInt(xyd[2]));
					//��n��������
					node.add(n);
				}
				System.out.println("��ȡ���");
			} catch (Exception e) {
				// TODO Auto-generated catch block
				System.out.println("����ʧ�ܣ�");
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





