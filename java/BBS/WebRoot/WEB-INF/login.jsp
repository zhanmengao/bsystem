<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
  <head>
    <title>My JSP 'login.jsp' starting page</title>
  </head>
  
  <body>
    <h1>登录界面</h1>
    <form action="${pageContext.request.contextPath}/login.do?flag=login" method="post">
    姓名:　<input type="text" name="name" />
    <input type="radio" name="sex" value="男" />男
    <input type="radio" name="sex" value="女" />女
    <input type="submit" value="登录" /> 
    </form>
  </body>
</html>
