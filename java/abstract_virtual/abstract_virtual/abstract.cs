using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

//一、抽象方法：只在抽象类中定义，方法修饰符不能使用private,virtual,static.
public abstract class People   //声明一个抽象类
{
    public abstract void study();  //抽象方法只能定义在抽象类中。
}

public class Student : People   //继承抽象类
{

    public override void study()     //重写抽象类的抽象方法

    {
        Console.WriteLine("好好学习，天天向上!");

    }
}
/*
 * 总结：(1)象方法只能声明在抽象类中，使用关键字abstract

           (2)抽象类中的抽象方法必须被子类重写。

【抽象方法没有方法体,子类必须重写方法体!!，因此抽象方法可以看成是一个没有方法体的虚方法】
*/
