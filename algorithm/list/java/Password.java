package com.xianxingbiao;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Password {
	static Letter head=null;
	static BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
	public Password()
	{
		//����26����ĸ��ѭ������
		Letter a=new Letter('a');
		head=a;
		Letter b=new Letter('b');
		Letter c=new Letter('c');
		Letter d=new Letter('d');
		Letter e=new Letter('e');
		Letter f=new Letter('f');
		Letter g=new Letter('g');
		Letter h=new Letter('h');
		Letter i=new Letter('i');
		Letter j=new Letter('j');
		Letter k=new Letter('k');
		Letter l=new Letter('l');
		Letter m=new Letter('m');
		Letter n=new Letter('n');
		Letter o=new Letter('o');
		Letter p=new Letter('p');
		Letter q=new Letter('q');
		Letter r=new Letter('r');
		Letter s=new Letter('s');
		Letter t=new Letter('t');
		Letter u=new Letter('u');
		Letter v=new Letter('v');
		Letter w=new Letter('w');
		Letter x=new Letter('x');
		Letter y=new Letter('y');
		Letter z=new Letter('z');
		
		a.setPrevious(z);a.setNext(b);
		b.setPrevious(a);b.setNext(c);
		c.setPrevious(b);c.setNext(d);
		d.setPrevious(c);d.setNext(e);
		e.setPrevious(d);e.setNext(f);
		f.setPrevious(e);f.setNext(g);
		g.setPrevious(f);g.setNext(h);
		h.setPrevious(g);h.setNext(i);
		i.setPrevious(h);i.setNext(j);
		j.setPrevious(i);j.setNext(k);
		k.setPrevious(j);k.setNext(l);
		l.setPrevious(k);l.setNext(m);
		m.setPrevious(l);m.setNext(n);
		n.setPrevious(m);n.setNext(o);
		o.setPrevious(n);o.setNext(p);
		p.setPrevious(o);p.setNext(q);
		q.setPrevious(p);q.setNext(r);
		r.setPrevious(q);r.setNext(s);
		s.setPrevious(r);s.setNext(t);
		t.setPrevious(s);t.setNext(u);
		u.setPrevious(t);u.setNext(v);
		v.setPrevious(u);v.setNext(w);
		w.setPrevious(v);w.setNext(x);
		x.setPrevious(w);x.setNext(y);
		y.setPrevious(x);y.setNext(z);
		z.setPrevious(y);z.setNext(a);
	}

	public static void main(String args[]) throws Exception
	{
		Password ps=new Password();
		ps.Vigenere();
	}
	public static void kaisa()
	{
		Password Yi=new Password();
		System.out.println("**************��λ����***************** ");
		System.out.print("������λ����:");
		String n="";
		try {
			n=br.readLine();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}finally{
			try {
				br.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		System.out.print("ԭ���ģ�");
		for(int i=0;i<26;i++)
		{
			System.out.print(head.getFace());
			head=(Letter) head.getNext();
		}
		System.out.println();
		System.out.print("���ܺ�");
		//��ͷ���ָ��ָ���µ�λ��
		int N=Integer.parseInt(n);
		if(N>=0)
		{
			for(int i=0;i<N;i++)
			{
				head=(Letter) head.getPrevious();
			}
		}else{
			for(int i=0;i<(-N);i++)
			{
				head=(Letter) head.getNext();
			}
		}

		//��ӡ����
		for(int i=0;i<26;i++)
		{
			System.out.print(head.getFace());
			head=(Letter) head.getNext();
		}
	}
	
	//ά����������
	public void Vigenere() throws Exception
	{
		while(true)
		{
			Letter head=this.head;
			System.out.println("�����������ĸ:");
			String in=br.readLine();
			char MyIn=in.charAt(0);
			System.out.println("�����������Կ");
		    int miyao=Integer.parseInt(br.readLine());
		    
			if(("exit").equals(in))
				break;
			//�ҵ�Ŀ��ڵ�
			for(int i=0;i<26;i++)
			{
				if(head.getFace()==MyIn)
				{
					//�ҵ���
					break;
				}
				head=(Letter) head.getNext();				
			}
			//����miyao�Σ��ҵ���������
			for(int i=0;i<miyao;i++)
			{
				head=(Letter) head.getNext();
			}
			System.out.println("����Ϊ:"+head.getFace());
		}
		
	}
}
class Letter extends Node2{
	private char face;
	public char getFace() {
		return face;
	}
	public void setFace(char face) {
		this.face = face;
	}
	public Letter(char face)
	{
		this.face=face;
	}
}
