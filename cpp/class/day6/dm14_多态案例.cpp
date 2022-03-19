

#include <iostream>
using namespace std;

//HeroFighter  AdvHeroFighter EnemyFighter


class HeroFighter
{
public:
	virtual int power()  //C++�������������⴦��
	{
		return 10;
	}
};

class EnemyFighter
{
public:
	int attack()
	{
		return 15;
	}
};


class AdvHeroFighter : public HeroFighter
{
public:
	virtual int power()
	{
		return 20;
	}
};

class AdvAdvHeroFighter : public HeroFighter
{
public:
	virtual int power()
	{
		return 30;
	}
};

//��̬����
//1 PlayObj��������̨  ����һ�����
//15:20
void PlayObj(HeroFighter *hf, EnemyFighter *ef)
{
	//��дvirtual�ؼ��� �Ǿ�̬���� C++����������HeroFighter����,ȥִ�� ������͵�power���� �ڱ���������׶ξ��Ѿ������˺����ĵ���
	//��̬����: �ٰ�:  //�����е�ʱ��,���ݾ������(���������),ִ�в�ͬ����ĺ��� ,���ֳɶ�̬.
	if (hf->power() > ef->attack())  //hf->power()�������û��ж�̬����
	{
		printf("����win\n");
	}
	else
	{
		printf("���ǹҵ�\n");
	}
}


//��̬��˼��
//�������3�����
//��װ: ͻ��c�����ĸ���....����������������ʱ��,����ʹ�ö�������� �Ͷ���ķ��� 
//�̳�: A B ���븴��
//��̬ : ����ʹ��δ��...


//��̬����Ҫ
//ʵ�ֶ�̬����������
//C���� ��Ӹ�ֵ ��ָ����ڵ��������
//��c���Ե����е����� (1 ������������  2 ��������  3 *p�ڱ����ú�����ȥ��ӵ��޸�ʵ�ε�ֵ)

//ʵ�ֶ�̬����������
//1 Ҫ�м̳� 
//2 Ҫ���麯����д
//3 �ø���ָ��(��������)ָ���������....

void main1401()
{
	HeroFighter		hf;
	AdvHeroFighter	Advhf;
	EnemyFighter	ef;

	AdvAdvHeroFighter advadvhf;

	PlayObj(&hf, &ef);
	PlayObj(&Advhf, &ef);

	PlayObj(&advadvhf, &ef) ; //������ �ܰ����Ǻ�����д�Ĵ���,����������

	cout<<"hello..."<<endl;
	system("pause");

}
void main1402()
{
	
	HeroFighter		hf;
	AdvHeroFighter	Advhf;
	EnemyFighter	ef;
	
	if (hf.power() > ef.attack())
	{
		printf("����win\n");
	}
	else
	{
		printf("���ǹҵ�\n");
	}

	if (Advhf.power() > ef.attack())
	{
		printf("Adv ����win\n");
	}
	else
	{
		printf("Adv ���ǹҵ�\n");
	}

	cout<<"hello..."<<endl;
	system("pause");
	return ;
}