<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
  <head>
    <title>My JSP 'publishMessage.jsp' starting page</title>
  </head>
  
  <body>
    <font size="6"><a href="#">查看信息</a></font> <font size="6"><a href="#">退出系统</a></font>
 	<form action="${pageContext.request.contextPath }/goPublishMessageUI.do?flag=publishMessage" method="post">
 	<table>
 	<tr><td>接收人</td><td><input type="text" name="getter" /></td></tr>
 	<tr><td>发送内容</td><td><textarea name="content" rows="5" cols="20">请输入内容</textarea></td></tr>
 	<tr><td><input type="submit" value="发布" /></td><td><input type="reset"  value="重新填写"/></td></tr>
 	</table>
 	</form>
  </body>
</html>
