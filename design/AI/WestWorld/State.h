#include<iostream>
#include<string>
#include<vector>
using namespace std;

//��״̬�Լ�����״̬���л�
template<class T>
class State{
public:
	virtual void Enter(T *pOnwer) = 0;
	virtual void Execute(T *pOnwer) = 0;
	virtual void Exit(T *pOnwer) = 0;
protected:
private:
};