// ***********************************************************************
// 
// 文件名：         SmsSender.cs
// 
// 创建日期：       2017/10/31
//  
// 功能说明：       {给指定手机号码发送短信，短信内容不能包含特殊字符}
//  
// 作者：           高亚斌
// 
// ***********************************************************************

using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using System.Text.RegularExpressions;

namespace SmsComputerMonitor
{
    public static class SmsSender
    {
        #region Fields and Properties


        private static readonly Regex LegalPhoneNum =
            new Regex(@"^((13[0-9])|(14[5|7])|(15([0-3]|[5-9]))|(18[0,5-9]))\d{8}$");

        private static readonly Regex LegalContent = new Regex(@"&");


        #endregion

        #region  Methods

        public static bool SendSms(string phoneNumber, string content)
        {
            //return true;
            if (LegalPhoneNum.IsMatch(phoneNumber))
            {
                var legalContent = LegalContent.Replace(content, "＆");
                legalContent = System.Web.HttpUtility.UrlEncode(legalContent, Encoding.UTF8);//编码
                var smsSendResult =
                    HttpSendSms(Settings.Default.SmsUrl + $"/?message={legalContent}&phoneNum={phoneNumber}");

                if (smsSendResult.Contains("短信提交成功"))
                    LogHelper.LogInfo($"短信提交成功：args = [{phoneNumber}:{content}]!");
                else
                    LogHelper.LogError($"短信提交失败:args = [{phoneNumber}:{content}],返回值:[{smsSendResult}]!");
                return true;
            }
            LogHelper.LogError($"发送号码不合法：[{phoneNumber}]->[{content}]");
            return false;
        }

        private static string HttpSendSms(string url)
        {
            try
            {
                WebResponse response;
                if (url != null)
                    response = WebRequest.Create(url).GetResponse();
                else
                    return "URL为空，不能调用！";
                var stream = response.GetResponseStream();
                if (stream != null)
                    using (var reader = new StreamReader(stream))
                    {
                        return reader.ReadToEnd();
                    }
            }
            catch (WebException exception)
            {
                LogHelper.LogError("HttpSendSms Error!", exception);
            }
            return $"[{url}]返回为空！";
        }

        #endregion
    }
}