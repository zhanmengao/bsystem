using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class People
{
    public int mAge;
}
public class PeopleManager
{
    //age是值类型 传进来的是拷贝
    public static void NumParameter(int age)
    {
        age = -1;                           //值参数被赋值，不影响调用外部age的值
    }
    //People是引用类型 传进来的是指针的值，可以往p的内存空间写值，改变其指向不影响外部
    public static void NumParameter(People p)
    {
        p.mAge = 20;                           //People是引用类型,其引用指向内存空间被改变
        p = new People();
        p.mAge = 50;
    }
    //int写ref 则传进来int的引用
    public static void RefParameter(ref int age)
    {
        age = -1;                                   //这里的age是引用，外部的值被改变
    }
    //People写ref  传进来的是二级指针 可以改变一级指针的指向
    public static void RefParameter(ref People p)
    {
        p.mAge = 20;                           //People是引用类型,其引用指向内存空间被改变
        p = new People();
        p.mAge = 50;                           //p指向的堆空间被改变
    }

    public static void OutParameter(out int age)
    {
        //Console.WriteLine(age);        ERROR 不能使用未赋值的age
        age = 99;
    }
}

namespace Parameter
{
    class Program
    {
        static void Main(string[] args)
        {
            int age = 12;
            People p = new People();
            p.mAge = 0;
            Console.WriteLine("Before NumParameter Age = " + age);
            PeopleManager.NumParameter(age);
            Console.WriteLine("After NumParameter Age = " + age);

            Console.WriteLine("Before NumParameter People Age = " + p.mAge);
            PeopleManager.NumParameter(p);
            Console.WriteLine("After NumParameter People Age = " + p.mAge);

            Console.WriteLine("Before RefParameter Age = " + age);
            PeopleManager.RefParameter(ref age);
            Console.WriteLine("After RefParameter Age = " + age);

            Console.WriteLine("Before RefParameter People Age = " + p.mAge);
            PeopleManager.RefParameter(ref p);
            Console.WriteLine("After RefParameter People Age = " + p.mAge);

            Console.WriteLine("Before OutParameter Age = " + age);
            PeopleManager.OutParameter(out age);
            Console.WriteLine("After OutParameter Age = " + age);
        }
    }
}
