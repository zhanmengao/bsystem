#include<iostream>
#include<string>
#include<vector>
#include"State.h";
using namespace std;

//状态机模板
template<class T>
class StateMachine
{
public:
	StateMachine(T *onwer) :m_pOnwer(onwer);
public:
	void Update();                                 //每帧执行对应状态的逻辑
	void ChangeState(State<T> *newState);         //状态切换
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
	State<T> *m_pCurrentState;           //当前状态
	State<T> *m_pPreviousState;          //上一个状态
	State<T> *m_pGlovalState;            //全局状态
	T *m_pOnwer;                         //状态持有者
};