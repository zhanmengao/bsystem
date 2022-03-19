#pragma once
#include <stdio.h>
#include<iostream>
using namespace std;
/*
状态模式：对象的行为依赖于其所处的状态。

员工在9-12工作，12-13午休，13-17工作
*/
class Worker;
class State
{
public:
	virtual void doSomeThing(Worker *w) = 0;
};

class Worker
{
public:
	Worker()
	{
		m_currstate = new State1;
	}
	int getHour()
	{
		return m_hour;
	}
	void setHour(int hour) //改变状态 7 
	{
		m_hour = hour;
	}
	State* getCurrentState()
	{
		return m_currstate;
	}
	void setCurrentState(State* state)
	{
		m_currstate = state;
	}

	void doSomeThing() //
	{
		m_currstate->doSomeThing(this);
	}
private:
	int		m_hour;
	State	*m_currstate; //对象的当前状态
};


class State1 : public State
{
public:
	void doSomeThing(Worker *w)
	{
		if (w->getHour() == 7 || w->getHour() == 8)
		{
			cout << "吃早饭" << endl;
		}
		else
		{
			delete w->getCurrentState();  //状态1 不满足 要转到状态2
			w->setCurrentState(new State2);
			w->getCurrentState()->doSomeThing(w); //
		}
	}
};

class State2 : public State
{
public:
	void doSomeThing(Worker *w)
	{
		if (w->getHour() == 9 || w->getHour() == 10)
		{
			cout << "工作" << endl;
		}
		else
		{
			delete w->getCurrentState(); //状态2 不满足 要转到状态3 后者恢复到初始化状态
			w->setCurrentState(new State1); //恢复到当初状态
			cout << "当前时间点：" << w->getHour() << "未知状态" << endl;
		}
	}
};

void StateMain()
{
	Worker *w1 = new Worker;
	w1->setHour(7);
	w1->doSomeThing();

	w1->setHour(9);
	w1->doSomeThing();

	delete w1;
	cout << "hello..." << endl;
	system("pause");
	return;
}
class GameStateBase;

class Content
{
public:
	void Requeat(int value)
	{
		if (m_CurState != nullptr)
		{
			m_CurState->Handler(value);
		}
	}
	void SetState(GameStateBase* state)
	{
		if (m_CurState != nullptr)
		{
			delete m_CurState;
			m_CurState = nullptr;
		}
		m_CurState = state;
	}
protected:
private:
	GameStateBase* m_CurState = nullptr;
};

class GameStateBase
{
public:
	GameStateBase(Content *content)
	{
		this->content = content;
	}
	virtual void Handler(int value) = 0;
protected:
	Content* content;
};


class GameState1:public GameStateBase
{
public:
	GameState1(Content *content) :GameStateBase(content){}
	virtual void Handler(int value)
	{
		if (value <= 10 && content != nullptr)
		{
			printf("%u <=10,change state to state2\n", value);
			content->SetState(new GameState2(content));
		}
	}
protected:
private:
};

class GameState2 :public GameStateBase
{
public:
	GameState2(Content *content) :GameStateBase(content){}
	virtual void Handler(int value)
	{
		if (value > 10 && value <= 20 && content != nullptr)
		{
			printf("%u >10 <=20 ,change state to state3\n", value);
			content->SetState(new GameState3(content));
		}
	}
protected:
private:
};

class GameState3 :public GameStateBase
{
public:
	GameState3(Content *content) :GameStateBase(content){}
	virtual void Handler(int value)
	{
		if (value > 20 && content != nullptr)
		{
			printf("%u >20,change state to state1\n", value);
			content->SetState(new GameState1(content));
		}
	}
protected:
private:
};

void GameStateMain()
{
	Content *theContext = new Content();
	theContext->SetState(new GameState1(theContext));

	theContext->Requeat(5);
	theContext->Requeat(15);
	theContext->Requeat(25);
	theContext->Requeat(35);
}

#include <stdlib.h>
#include<stdio.h>
class StateController
{
public:
	void StateUpdate()
	{
		if (m_CurState != nullptr)
		{
			m_CurState->StateUpdate();
		}
	}
	void SetState(ISceneState* state)
	{
		if (m_CurState != state && m_CurState != nullptr)
		{
			m_CurState->StateEnd();
			m_CurState = state;
			state->StateBegin();
		}
	}
private:
	ISceneState *m_CurState;
};


class ISceneState
{
public:
	// 建造者
	ISceneState(StateController *Controller)
	{
		m_Controller = Controller;
	}
	// 开始
	virtual void StateBegin() = 0;
	// 结束
	virtual void StateEnd() = 0;
	// 更新
	virtual void StateUpdate() = 0;
	bool IsLoadOK() { return IsInit; }
protected:
	StateController *m_Controller;
	bool IsInit = false;
};
//开始状态
class StartState :public ISceneState
{
public:
	StartState(StateController *controller) :ISceneState(controller) {};

	// 开始
	virtual void StateBegin()
	{
		if (IsInit)
		{
			return;
		}
		IsInit = false;

		/*初始化*/
		printf("%s", "StartState Loading...");

		IsInit = true;
	}

	// 结束
	virtual void StateEnd()
	{
		printf("%s", "StartState Destroy...");
		IsInit = false;
	}

	// 更新
	virtual void StateUpdate()
	{
		if (IsInit)
		{
			//Update逻辑
		}
	}
};

//普通状态
class MainState :public ISceneState
{
public:
	MainState(StateController *controller) :ISceneState(controller) {};

	// 开始
	virtual void StateBegin()
	{

	}

	// 结束
	virtual void StateEnd()
	{

	}

	// 更新
	virtual void StateUpdate()
	{

	}
};

//战斗状态
class BattleState :public ISceneState
{
public:
	BattleState(StateController *controller) :ISceneState(controller) {};

	// 开始
	virtual void StateBegin()
	{

	}

	// 结束
	virtual void StateEnd()
	{

	}

	// 更新
	virtual void StateUpdate()
	{

	}
};