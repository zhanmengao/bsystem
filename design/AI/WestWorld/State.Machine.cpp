#include<iostream>
#include<string>
#include<vector>
#include"StateMachine.h"
using namespace std;

template<class T>
void StateMachine<T>::Update()
{
	//Ö´ÐÐÈ«¾Ö×´Ì¬
	if (this->m_pGlovalState != nullptr)
	{
		m_pGlovalState->Execute(m_pOnwer);
	}
	//Ö´ÐÐµ±Ç°×´Ì¬Âß¼­
	if (this->m_pCurrentState!= nullptr)
	{
		m_pCurrentState->Execute();
	}
}

template<class T>
void StateMachine<T>::ChangeState(State<T> *newState)
{
	//ÇÐ»»×´Ì¬
	this->m_pPreviousState = this->m_pCurrentState;
	//Ö´ÐÐÍË³öÂß¼­
	this->m_pCurrentState->Exit(this->m_pCurrentState);
	//ÉèÖÃÐÂ×´Ì¬
	this->m_pCurrentState = newState;
	m_pCurrentState->Enter(m_pOnwer);
}

template<class T>
void StateMachine<T>::RevertToPreviousState()
{
	ChangeState(m_pPreviousState);
}