#include"CppThread.h"

//检查锁级别，看能否加锁
bool HierarchicalMutex::CheckForHierarchyViolation()
{
	std::lock_guard<std::mutex> lock(StaticMutex);
	if (NowHierarchyValue <= mHierarchyValue)
	{
		return false;
	}
	return true;
}
//加锁成功后，更新当前锁数据
void HierarchicalMutex::UpdateHierarchyValue()
{
	std::lock_guard<std::mutex> lock(StaticMutex);
	previousHierarchyValue = NowHierarchyValue;
	NowHierarchyValue = mHierarchyValue;
}

void HierarchicalMutex::Lock()
{
	while (1)
	{
		mMutex.lock();
		if (CheckForHierarchyViolation())
		{
			UpdateHierarchyValue();
		}
		else
		{
			mMutex.unlock();
		}
	}
}
//解锁，还原到加锁前的锁数据
void HierarchicalMutex::UnLock()
{
	std::lock_guard<std::mutex> lock(StaticMutex);
	NowHierarchyValue = previousHierarchyValue;
	mMutex.unlock();
}
//尝试加锁
bool HierarchicalMutex::TryLock()
{
	if (!CheckForHierarchyViolation())
	{
		return false;
	}
	if (!mMutex.try_lock())
	{
		return false;
	}
	UpdateHierarchyValue();
	return true;
}
unsigned long HierarchicalMutex::NowHierarchyValue(ULONG_MAX);
std::mutex HierarchicalMutex::StaticMutex;