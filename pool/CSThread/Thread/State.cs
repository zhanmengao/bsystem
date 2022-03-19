using System;
using System.Threading;


public partial class ThreadProgram
{
    //是否阻塞
    public static bool IsBlocked(Thread t)
    {
        return (t.ThreadState & ThreadState.WaitSleepJoin) != 0;
    }
    //简化的获取状态，排除了其中的无用枚举
    public static ThreadState GetState(Thread t)
    {
        return t.ThreadState & (ThreadState.Unstarted | ThreadState.WaitSleepJoin | ThreadState.Stopped);
    }
}