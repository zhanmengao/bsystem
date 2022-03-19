using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DateTimeFormat
{
  class Program
  {
    static void Main(string[] args)
    {
      Test2();
    }
    private static  void Test1()
    {
      DateTime dateValue = new DateTime(2009, 6, 1, 4, 37, 0);
      CultureInfo[] cultures = { new CultureInfo("en-US"), 
                                 new CultureInfo("fr-FR"),
                                 new CultureInfo("it-IT"),
                                 new CultureInfo("de-DE") };
      foreach (CultureInfo culture in cultures)
        Console.WriteLine("{0}: {1}", culture.Name, dateValue.ToString(culture));
    }
    private static void Test2()
    {
      string dateString = "2018/5/24/周四 15:35:28";
      int year, month, day, hour, min, second;
      MySQLDateTimeToString(dateString, out year, out month, out day, out hour, out min, out second);
      Console.WriteLine(year.ToString()+month+day+hour+min+second);
    }

    public static void MySQLDateTimeToString(string dateVal, out int year, out int month, out int day, out int hour, out int min, out int second)
    {
      int begin = 0;
      string[] words = dateVal.Split('/',' ',':');
      year = Convert.ToInt32(words[0]);
      month = Convert.ToInt32(words[1]);
      day = Convert.ToInt32(words[2]);
      hour = Convert.ToInt32(words[4]);
      min = Convert.ToInt32(words[5]);
      second = Convert.ToInt32(words[6]);
    }
  }
}
