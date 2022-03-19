<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
  <head>
    <title>My JSP 'addEmpUi.jsp' starting page</title>
  </head>
  
  <body>
   <h1>添加雇员</h1>
   <form action="${pageContext.request.contextPath}/employee.do?flag=addEmp" method="post">
   <table>
   <tr><td>name</td><td><input type="text" name="name" /></td></tr>
   <tr><td>email</td><td><input type="text" name="email" /></td></tr>
   <tr><td>grade</td><td><input type="text" name="grade" /></td></tr>
   <tr><td>pwd</td><td><input type="text" name="pwd" /></td></tr>
   <tr><td>salary</td><td><input type="text" name="salary" /></td></tr>
   <tr><td>department</td><td>
   <!-- 这里部门的信息，应当从数据库提出，而不能写死. -->
   <select name="departmentId">
   <option value="5">财务部</option>
   </select>
   </td></tr>
   <tr><td><input type="submit" value="添加"/></td>
   <td><input type="reset" value="重填" /></td></tr>
   </table>
   </form>
  </body>
</html>
