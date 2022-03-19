<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
  <head>
    <title>My JSP 'ok.jsp' starting page</title>
  </head>
  
  <body>
   <h4>恭喜你，操作成功</h4>
   <a href="#">返回继续添加</a>
   <a href="${pageContext.request.contextPath}/login.do?flag=goMainFrame">返回主界面</a>
  </body>
</html>
