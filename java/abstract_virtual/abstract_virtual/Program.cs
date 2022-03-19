using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
/*
 * 三、两者区别：

总结：抽象方法是只有方法名称，没有方法体（也就是没有方法具体实现），子类必须重写父类抽象方法；

           虚函数是该方法有方法体，但是子类可以覆盖，也可不覆盖。

（1）虚方法有方法体，抽象方法没有方法体。抽象方法是一种强制派生类覆盖的方法，否则派生类将不能被实例化；

（2）抽象方法只能在抽象类中声明，虚方法不是；

（3）派生类必须重写抽象类中的抽象方法，虚方法则不必要。
 */
namespace abstract_virtual
{
    class Program
    {
        static void AbstractMain()
        {
            Student t = new Student();//实例化派生类

            People p = t;   //使用派生类对象实例化抽象类

            //以上两句等价于  People p = new Student();//使用派生类对象实例化抽象类；

            p.study(); //使用抽象类对象调用抽象类中的抽象方法study  
        }
        static void VirtualMain()
        {
            SubClass sub = new SubClass();
            BaseClass bas = sub;
            BaseClass tBas = new BaseClass();
            Console.WriteLine(sub.GetName());
            Console.WriteLine(bas.GetName());
            Console.WriteLine(tBas.GetName());
        }
        static void Main(string[] args)
        {
            AbstractMain();
            VirtualMain();
        }
    }
}
