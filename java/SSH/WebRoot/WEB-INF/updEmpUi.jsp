<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
  <head>
    <title>My JSP 'updEmpUi.jsp' starting page</title>
  </head>
  
  <body>
    <h1>修改雇员</h1>
    <form action="${pageContext.request.contextPath}/employee.do?flag=updEmp" method="post">
   <table>
   <tr><td>id</td><td><input type="text"  readonly="readonly" value="${emp.id }" name="id"/></td></tr>
   <tr><td>name</td><td><input type="text" value="${emp.name }" name="name" /></td></tr>
   <tr><td>email</td><td><input type="text" value="${emp.email }" name="email" /></td></tr>
   <tr><td>grade</td><td><input type="text"  value="${emp.grade }" name="grade" /></td></tr>
   <tr><td>pwd</td><td><input type="text" value="${emp.pwd }" name="pwd" /></td></tr>
   <tr><td>salary</td><td><input type="text" value="${emp.salary }" name="salary" /></td></tr>
   <tr><td>department</td><td>
   <!-- 这里部门的信息，应当从数据库提出，而不能写死. -->
   <select name="departmentId">
   <option value="5">财务部</option>
   <option value="100">人事部</option>
   </select>
   </td></tr>
   <tr><td><input type="submit" value="修改"/></td>
   <td><input type="reset" value="重填" /></td></tr>
   </table>
   </form>
  </body>
</html>
