using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
//二、虚方法：使用virtual修饰的方法：虚方法可以有方法体。
public class BaseClass         //创建一个基类
{
    public virtual string GetName()    //使用virtual关键字创建父类中的虚方法
    {
        return "父类虚方法体":     
     }
}

public class SubClass : BaseClass    //子类继承父类
{
    public override string GetName()   //子类重写父类虚方法
      {
               return "重写父类虚方法!";
       }
}
/*
以上的示例：父类中的虚方法被派生类重写了。

注意事项：virtual修饰符不能与private、static、abstract、override修饰符同时使用。

ps：override修饰符不能与new 、static、virtual修饰符同时使用，并且重写方法只能用于重写基类中的虚方法。

虚函数的限制：

* 虚函数仅适用于有继承关系的类对象，所以只有类的成员函数才能说明为虚函数；

*静态成员函数、内联函数、构造函数不能是虚函数；

*析构函数可以是虚函数
 */
