#include<iostream>
#include<string>
#include<vector>
#include"StateMachine.h"
using namespace std;

template<class T>
void StateMachine<T>::Update()
{
	//ִ��ȫ��״̬
	if (this->m_pGlovalState != nullptr)
	{
		m_pGlovalState->Execute(m_pOnwer);
	}
	//ִ�е�ǰ״̬�߼�
	if (this->m_pCurrentState!= nullptr)
	{
		m_pCurrentState->Execute();
	}
}

template<class T>
void StateMachine<T>::ChangeState(State<T> *newState)
{
	//�л�״̬
	this->m_pPreviousState = this->m_pCurrentState;
	//ִ���˳��߼�
	this->m_pCurrentState->Exit(this->m_pCurrentState);
	//������״̬
	this->m_pCurrentState = newState;
	m_pCurrentState->Enter(m_pOnwer);
}

template<class T>
void StateMachine<T>::RevertToPreviousState()
{
	ChangeState(m_pPreviousState);
}