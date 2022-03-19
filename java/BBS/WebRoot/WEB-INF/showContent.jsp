<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ taglib prefix="c"  uri="http://java.sun.com/jsp/jstl/core" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
  <head>
    <title>My JSP 'showContent.jsp' starting page</title>
  </head>
  
  <body>
   <h3>显示内容页面</h3>
   <table>
   <tr><td>name</td><td>sex</td><td>content</td><td>type</td><td>删除</td></tr>
   <c:forEach var="content" items="${contentsList}">
   <tr><td>${content.name }</td><td>${content.sex }</td><td>${content.content }</td><td>${content.type.type }</td><td><a href="#">删除</a></td></tr>
   </c:forEach>
   </table>
  </body>
</html>
