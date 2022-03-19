#include<iostream>
#include<string>
#include<vector>
#include"State.h";
using namespace std;

//״̬��ģ��
template<class T>
class StateMachine
{
public:
	StateMachine(T *onwer) :m_pOnwer(onwer);
public:
	void Update();                                 //ÿִ֡�ж�Ӧ״̬���߼�
	void ChangeState(State<T> *newState);         //״̬�л�
	void RevertToPreviousState();               
private:

	void setCurrentState(State<T> *CurrentState)
	{
		this->m_pCurrentState = CurrentState;
	}

	void setGlovalState(State *GlovalState)
	{
		this->m_pGlovalState = GlovalState;
	}
private:
	State<T> *m_pCurrentState;           //��ǰ״̬
	State<T> *m_pPreviousState;          //��һ��״̬
	State<T> *m_pGlovalState;            //ȫ��״̬
	T *m_pOnwer;                         //״̬������
};