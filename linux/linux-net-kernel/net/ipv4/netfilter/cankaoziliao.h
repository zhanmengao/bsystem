
/*
��������linux��ȫ�ײο�����
http://bbs.chinaunix.net/thread-1930079-1-1.html
http://bbs.chinaunix.net/thread-1930079-1-1.html


netfilter ���Ӹ���:http://blog.csdn.net/ye_shizhe/article/details/17331947

ip_queue netfilter �û��ռ� �ں˿ռ�ͨ��  �ο�http://bbs.chinaunix.net/thread-2013041-1-1.html




linux�ں�Э��ջ���ֲ�ṹ��BSD socket,INET socket��ȣ��ο�:http://blog.csdn.net/yming0221/article/details/7488828
http://blog.csdn.net/column/details/linux-kernel-net.html

���ں����׽���struct socket��struct sock��struct inet_sock��struct tcp_sock��struct raw_sock��struct udp_sock��struct inet_connection_sock��struct inet_timewait_sock��struct tcp_timewait_sock�Ĺ�ϵ���������أ�
*struct socket�����BSD���socket��Ӧ�ó�����ù�ϵͳ�������ȴ����������׽��֣����;���Э���޹ء�

*struct inet_sock��INETЭ����ʹ�õ�socket�ṹ�����Կ���λ��INET�㣬��struct sock��һ����չ�����ĵ�һ�����Ծ���struct sock�ṹ��

*struct sock������崫���Э����ص��׽��֣������ں˵Ĳ�������������׽��֡�

*struct tcp_sock��TCPЭ����׽��ֱ�ʾ�����Ƕ�struct inet_connection_sock����չ�����һ�����Ծ���struct inet_connection_sock inet_conn��

*struct raw_sock��ԭʼ���͵��׽��ֱ�ʾ��ICMPЭ���ʹ�������׽��֣����Ƕ�struct sock����չ��

*struct udp_sock��UDPЭ���׽��ֱ�ʾ�����Ƕ�struct inet_sock�׽��ֵ���չ��

*struct inet_connetction_sock��������������Э����׽��֣��Ƕ�struct inet_sock�׽�����չ��


struct tcp_sock { 
         struct inet_connection_sock     inet_conn; //inet_connection_sock has to be the first member of tcp_sock
         ... 
};

inet_connection_sock - INET connection oriented sock
struct inet_connection_sock { 
         struct inet_sock           icsk_inet; //inet_sock has to be the first member!

         ... 
};

struct inet_sock - representation of INET sockets
struct inet_sock { 
         struct sock             sk; //       sk and pinet6 has to be the first two members of inet_sock ... 
};



linux�ں�netlink������⣬�ο���ַ:http://blog.chinaunix.net/uid-21768364-id-3244600.html


socketϵͳ���õ��ں˷���  http://blog.chinaunix.net/uid-20357359-id-1963464.html
Ӧ�ò㴴��socket�����/NETĿ¼����� socket.c����   
SYSCALL_DEFINE3(socket, int, family, int, type, int, protocol)

TCP��������  �ο���ַ:http://blog.csdn.net/qy532846454/article/details/7882819


PF_INETЭ���壬Ӧ�ò㴴���׽��֣��ں˴�����̣��ο�http://jianshu.io/p/5d82a685b5b6


netlink��ϸ���̷������������̣��ܺ�:<netlinkʵ�ַ���>  http://wenku.baidu.com/link?url=uUzbboQ-xcQp_7UGIdijovfc9n92beFpaccQfaj8wQ08PN_vaXBQA4ZjY44MdF3s_VbZr7ncoEdRNfjPHdS3swljlwMqw5bbeCTJI593fKS




�ӷ�����P119���Կ�����NETFILTER�ܹ�����IP�����ʹ����֮����д���





===========================================================PPPЭ��=========================================================================

ppp�ο�����: �ο�:http://blog.csdn.net/istone107/article/details/8065758
ppp�ں˼ܹ�ͼ�����Բο�:http://blog.csdn.net/vfatfish/article/details/9283703  Linux PPPʵ��Դ�����
ppp�Ӵ��ڽ������ݣ�����·��̴�����ʱ����ppp_sync_openʵ��ppp_sync_process,ppp_input  �����շ����̲ο�http://blog.csdn.net/absurd/article/details/1596496

pppd���ں˹�ϵ����:
1.����/dev/ttys0��ʱ������ں˴����̹߳滮�ṹstruct asyncppp����Ϊ�䴴��struct channel�ṹ��һ�����ڶ�Ӧһ��channel��
2.������/dev/ppp�ļ���fd��channel������ioctl��ʱ��Ӧ�ó��ֵ�fd��ִ���ں˲���channel
3.����unit�������ں�struct ppp�ṹ�������ٴδ��´򿪵�/dev/ppp��ppp_dev_fd���ӵ��ں˶�Ӧ��unit��ppp_dev_fd�ͺ��ں�unit�����ˡ�
http://blog.csdn.net/efan_linux/article/details/4594489 ppp֡�ķ��ͣ������˽�ppp֡ͷѹ������ѹ������Ϣ

PPPѹ����زο�:http://www.csdn123.com/html/20130308/51/d25bba0e39ecad98206ca042e8d40d98.htm PPPЭ���漰���ļ�������ѹ������
TCPͷ��ѹ��  http://wenku.baidu.com/link?url=F6TfSsu4DHrakjn6uhrIUNibqnartKJu4i7PZ6SlcumCR877IKKx7hyjvH0bONRo73cMjEy48bxo1PMNbgUffa8K9yvB4z7LhJ6_4jXNb17
TCPͷ��ѹ��:http://blog.csdn.net/wisage/article/details/7059257   ����·PPP�����㷨�о����Ľ��ĵ���ѹ������ϸ˵��
���²㵽�ϲ�����:tty->asyn_ppp(��·��� ͨ��)->ppp����->Э��ջ
��tty��������ʱ����·��̴���������Ӧ������������·��̵�ʱ���ȡtty���ݣ�Э��ջͨ��unit�������ݣ����ȵ�ppp�����Ի��ڴ���unit��ʱ��ע�ᷢ�ͺ���
�ο�:Linux����������������
de.pdf56ҳ  MPP���Բο���������ĵ���LCP NCP������ϸ˵��  ppp
*/




/*
�������˽�֪ʶ��:
skb�ṹ�еķ����Կռ䣬skb_shinfo
�����ռ�name space
���ж�ԭ��
������������
ע��netfilter�����Ͼ�������Щ����������������ʲô
·�ɱ���洢��ѯ���� :http://blog.csdn.net/qy532846454/article/details/7568994
���listen(�ں�Ϊinet_listen)�����е�sk_max_ack_backlog��Ҳ����listen�����ĵڶ�������
select poll epoll �ο�:http://blog.csdn.net/dianhuiren/article/details/6764190  http://blog.csdn.net/21aspnet/article/details/2627662
linux i/oģ�� http://blog.sina.com.cn/s/blog_4697cdcd0100s3uh.html 
http://www.cnblogs.com/fora/archive/2011/06/30/2094324.html
http://www.cnblogs.com/hustcat/archive/2009/09/18/1569661.html
http://blog.csdn.net/tonyxf121/article/details/7878313
GSO TSO,�ο�xuexizongjie.c��http://www.cnblogs.com/yizhinantian/archive/2011/04/03/2004266.html������ܺú���⡣
http://blog.aliyun.com/673?spm=0.0.0.0.WFJJGP  ѧϰ�����Ʋ�Ʒ�е�:�����������̣�1����accept��������?   һ����7�¡�
http://blog.csdn.net/russell_tao/article/details/18711023

*/

