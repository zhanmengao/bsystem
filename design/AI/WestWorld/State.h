#include<iostream>
#include<string>
#include<vector>
using namespace std;

//由状态自己调整状态的切换
template<class T>
class State{
public:
	virtual void Enter(T *pOnwer) = 0;
	virtual void Execute(T *pOnwer) = 0;
	virtual void Exit(T *pOnwer) = 0;
protected:
private:
};