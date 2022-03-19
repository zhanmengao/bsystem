<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ taglib prefix="c"  uri="http://java.sun.com/jsp/jstl/core" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
  <head>
    <title>My JSP 'publish.jsp' starting page</title>
    <script type="text/javascript">
    <!--
    	function check(){
    		var length=document.getElementById("content").innerText.length;
    		if(length>20){
    			window.alert("字数不能超过20");
    			return false;
    		}else{
    			return true;
    		}
    	}
    
    //-->
    </script>
  </head>
  
  <body>
  <center>
    <h1>发表评论页面</h1>
    
    欢迎 ${userForm.name } 发表评论<br/>
    
    <form action="${pageContext.request.contextPath}/content.do?flag=addContent" method="post">
    <textarea id="content" rows="8" name="content" cols="40"></textarea><br/><br/>
  	<select name="type">
  	
    <c:forEach items="${list}" var="type">
    <option value="${type.id }">${type.type }</option>
    </c:forEach>
  	</select> <br/><br/>
  	<input type="button" onclick="return check();" value="计算数字"  />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  	<input type="submit"  value="发送评论" />
  	</form>
 </center>
 </body>
</html>
