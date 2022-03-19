<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
  <head>
    <title>My JSP 'login.jsp' starting page</title>
  </head>
  
  <body>
    <h1>管理员登陆</h1>
    <form action="/myssh/login.do?flag=login" method="post">
    <table>
    <tr><td>id:</td><td> <input type="text" name="id" /></td></tr>
    <tr><td>pw:</td><td><input type="password" name="pwd"/></td></tr>
    <tr><td><input type="submit" value="登陆" /></td><td><input type="reset" value="重填"/></td></tr>
    </table>
   
    <br/>
    
    </form>
  </body>
</html>
