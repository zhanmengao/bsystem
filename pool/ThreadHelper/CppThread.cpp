#include"CppThread.h"

//��������𣬿��ܷ����
bool HierarchicalMutex::CheckForHierarchyViolation()
{
	std::lock_guard<std::mutex> lock(StaticMutex);
	if (NowHierarchyValue <= mHierarchyValue)
	{
		return false;
	}
	return true;
}
//�����ɹ��󣬸��µ�ǰ������
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
//��������ԭ������ǰ��������
void HierarchicalMutex::UnLock()
{
	std::lock_guard<std::mutex> lock(StaticMutex);
	NowHierarchyValue = previousHierarchyValue;
	mMutex.unlock();
}
//���Լ���
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