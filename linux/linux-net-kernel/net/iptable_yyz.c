/*
//iptable������mange filter nat����ں˶�ӦhookΪMANGLE_VALID_HOOKS  FILTER_VALID_HOOKS   NAT_VALID_HOOKS

NAT ��һ��� mangle �� -t ����ָ��Ҫ�����ĸ���filter ��Ĭ�ϵı����û�� -t ��������Ĭ�϶� filter �������

iptables -t filter -A INPUT ! -s 127.0.0.1 -p icmp -j DROP
-j ǰ���ǹ����������-j ��ʼ�ǹ������Ϊ��Ŀ�ģ��������������Ϊ����filter ���е� INPUT �������в���һ������
����Դ��ַ��Ϊ 127.0.0.1 �� icmp ������������

//iptable������mange filter nat����ں˶�ӦhookΪMANGLE_VALID_HOOKS  FILTER_VALID_HOOKS   NAT_VALID_HOOKS

-t ָ������(filter nat mangle)
-A -D (�ֱ����Ϊ���������Ǻ���ľ��ǹ���)
iptable -t mangle ��Ҫ�������ñ�ǣ���ip rule��ip routeһ�����ò���·�ɣ��ο�<iptables ��mangle�� .>http://blog.csdn.net/w_s_xin/article/details/24796539

Filter���������ݰ���Ĭ�ϱ� 

  

��1��INPUT   ������������Ŀ���ַ�Ǳ��������ݰ����Խ��뱾�����ݰ��Ĺ��ˣ� 

��2��OUTPUT �����������б������������ݰ�����Դ��ַ�����ݰ��Ĺ��ˣ� 

��3��FORWARD������������·�����������ݰ���Դ��ַ��Ŀ���ַ�����Ǳ��������ݰ��� 

  

NAT�������ַת����Network Address Translation�� 

�����һ�����ݰ�������NAT��Masquerade,��ô�������ݰ����ᱻ����ͬ�Ķ�����Ҳ�����������ݰ����ᱻһ��һ����NAT������һ�����İ�ֻ�ᾭ�������һ�Σ��κ�ʱ�򶼲�Ҫ��������κ�һ�������й��ˡ� 

    ������������ 

��1��DNAT���ı����ݰ���Ŀ�ĵ�ַʹ������·�ɵ�ĳ̨���� 

            ��ʹ�����ܹ����ʾ������ķ������� 

��2��SNAT: �ı����ݰ���Դ��ַ��ʹ�������ܷ��ʹ����� 

��3��NASQUERADE:��SNATһ��ʹ�������ܷ��ʹ������޹̶�IPʹ��PPP.PPPoE�Ȳ�����������Internet 

  

Nat�����3������ 

��1��PREROUTING�� :���ݰ��������ǽʱ�ı����Ŀ�ĵ�ַ�� 

��2��OUTPUT�����ı䱾�ز������ݰ���Ŀ���ַ�� 

��3��POSTROUTING:�����ݰ��뿪����ǽʱ�ı����ݰ���Դ��ַ�� 

  

Mangle���޸����ݰ����ı��ͷ������(TTL, TOS,  MARK ) 

(1)TOS :���øı����ݰ��ķ������ͣ���Ҫʹ��TOS���÷���Internet�İ����Ǵ�������TOS��·�ɣ������ڱ��������κι��ˡ�SNAT��DNAT 

(2)TTL:�ı����ݰ�������ʱ�䣬���������ݰ�ֻ��һ�������TTL����ƭISP,��ЩISP����ϣ����̨���������һ�����ӷ���Internet,ISPͨ�����һ̨����������������ݰ��Ƿ��в�ͬ��TTL���жϡ� 

(3)Mask:�����ݰ���������ı�ǣ�ͨ��������ô������ƺͻ���������ࡣ 

  

Mangle���5������ 

��1��PREROUTING  �������ݰ��������ǽ��·���ж�֮ǰ�ı����ݰ��� 

��2��POSTROUTING�������ݰ�ȷ����Ŀ���ַ�󣬼�·���ж�ǰ�ı����ݰ��� 

��3��OUTPUT���������ݰ���ȷ��Ŀ�ĵ�ַǰ�ı����ݰ� 

��4��INPUT���������ݰ����뱾����Ӧ�ó������֮ǰ�ı����ݰ��� 

��5��FORWARD������һ��·���ж�֮�����һ��·���ж�ǰ�ı����ݰ��� 





iptables���--ת
������http://yijiu.blog.51cto.com/433846/1356254

iptables���

�������

1.����ǽ������������Ե:���ڽ�����������߱����������ݱ��ģ����������趨�õļ���������飬���μ����ɵı���һ�ɰ������ȶ���õĴ������������Ӧ����

��linux����tcp/ipЭ��ջ�����ں˵��У���ζ�ű��ĵĴ��������ں��д���ģ�Ҳ����˵����ǽ�����ڹ������ں��У�����ǽ�������ں������tcp/ip������������λ�ã��ù���ȥ��飬�������ܹ���������

iptables��������tcp/ip���ĵ����ԣ�Դip��Ŀ��ip��Դ�˿ڡ�Ŀ��˿ڣ�

tcp��־λ:   syn��syn+ack��ack�� fin��urg��psh��rst ��

2.Ӧ������

�ڶ�������������Ӧ�����أ�����squid��ʹ��acl����Ӧ�ò㣩varish��һ��������ȡ�

3�����ּ��ϵͳ��IDS����

���������ּ��ϵͳ  NIDS

���������ּ��ϵͳ  HIDS

����IDS���õļ������У�snort��

4.���ַ���ϵͳ��IPS���������۹�

����һ�����ּ��ϵͳ�Ƿǳ��鷳�ģ���Ϊ��������������һ��λ��

����IPS���õļ������У� tripwire ��

iptables��������

��linux��˵�����ܹ�ʵ����������ǽ�Ĺ����������������������Ե����ô�ȿ��԰����������ǽ�Ľ�ɫ�������Ǵ������

������������

�����Ĩ����ب�·�ɨ�Ӧ�ó��򣨵ȴ��û����󣩨��ں˴���·�ɨ����ͱ���

iptables������

��:

filter��Ҫ�����������йأ���Ҫ�������ǽ���� ���˱����������������ݰ���Ĭ��ʹ�õı�;

input   :�����������Ŀ���ַ�Ǳ�����ַ�����ݰ������ǹ��˽������������ݰ�;

forward  :����ת�����������������뱾�������ݰ�����NAT��ϵ�ܴ�;

output   :������Դ��ַ�����ݰ������ǶԱ������������ݰ�;

NAT��

���������ַת��������Դ��Ŀ��IP��ַ�Ͷ˿ڵ�ת����һ�����ڹ�������������˿ڵ�ת������

snat    :��ַת��

dnat    :���ַת��

pnat    :��˿�ת��

mangle ��

�����Ĳ������޸ı��ı�־λ������װ����

5�����㣨��������

��PREROUTING

��INPUT

��FORWORD

��OUTPUT

��POSTROUTING    

�����������������������磬��input�����������magle�Ĺ���Ҳ������fileter�Ĺ�������ڱ�д�����ʱ��Ӧ����ָ������ָ����

netfilter��Ҫ������tcp/ipЭ��ջ�ϵģ���Ҫ������tcp�����ײ���udp�����ײ�

��������Զ��壺

1.�����Э��

��Ҫ������ipЭ�鱨����

2.�����Э�����ԣ�

��Ҫ������

tcp

udp

icmp  icmp�䲢�����������崫���ģ����ǹ����������ʹ����֮���һ�������Э��

3.ip���ĵ����ԣ�

IP���ĵ�����Ϊ: Դ��ַ.Ŀ���ַ

4.iptables����ƥ��

iptables��β鿴�����

��д��ĸѡ�����ʵ��ĳ�ֹ��ܣ��������ɾ����չ�������

Сд��ĸѡ�����ƥ�估������

-L ��list �б�

    -n :���ָ�ʽ��ʾip�Ͷ˿ڣ�

    --line-numbers:��ʾ�кţ�

    -x �� ��ʾ��ȷֵ����Ҫ����λ���㣻

 

-t :  ָ����

     -t{fillter|nat|mangle|raw}

-v �� ��ʾ��ϸ��Ϣ -v -vvv -vvvv ..������ʾ����ϸ����Ϣ

 

5.���������

��������

-F �������

���nat���е�input������ʽ���£�

#iptables-t nat -F INPUT

#���fllter����������

#iptables-F

-P : �趨Ĭ�ϲ��ԣ�Ϊָ��������Ĭ�ϲ��ԣ���ʽ���£�

#����fllter��input����Ĭ�Ϲ���Ϊ����

iptables-t fllter -P INPUT DROP

-N �� �½�һ���Զ�����������������ɾ�������̫�࣬�����Զ�������

#�Զ�����ֻ�ܱ����òſ��Է�������

iptables-N fillter_web

-X : ɾ���Զ����������������й������޷�ɾ��

-Z ������������

iptables-Z

-E ���������Զ�����

 

iptables�������

-A   ��append���ӹ��򣬽������Ĺ�����ӵ�����β��

-I[n] ������Ϊ��n������

-D   : ɾ����n������

-R[n] : �滻��N��

������Ķ�Ӧ��ϵ��

fillter ��INPUT FORWORD OUTPUT

nat : PREROUTING POSTROUTING  OUTPUT

ʹ��-tָ�������鿴ָ�����ڵĹ���

#iptables-t nat -L -n

raw : prerouting output

iptables-t raw -L -n

mangle: prerouting input forword output postrouting

iptables-t mangle -L -n

#�鿴����

[root@test3~]# iptables -L -n
Chain INPUT (policy ACCEPT)
target     prot opt source              destination        

Chain FORWARD (policy ACCEPT)
target     prot optsource              destination        

Chain OUTPUT (policy ACCEPT)
target     prot optsource              destination  

ͨ�����Ͽ��Թ۲쵽��ÿһ��������Ĭ�ϲ��ԣ�policy ACCEPT

ͨ��ֻ��Ҫ�޸�fllter���Ĭ�ϲ��Լ��ɣ��ɴ�����б������������ʱ�����ĳ��������ô��ᾭ��input������˽����ı��Ķ�����Ҫ�����˵ģ���ô��ȥ�ı�������Ҫ���ˣ�����Щ�ض��ĳ�����Ҳ��Ҫ������

����д�����ʱ�����Ž�����д����ȷ���ϣ�����ǳ��ش�

�����Ĭ�ϲ��Զ���2����������ͨ��-vѡ����Թ۲�����ƥ�����

#iptables -t nat -L -n -v

[root@sshgw~]# iptables -L -n -v

ChainINPUT (policy ACCEPT 7 packets, 975 bytes)

pkts bytestarget     prot opt in     out    source              destination        

   0    0 ACCEPT     all  --  lo     *      0.0.0.0/0           0.0.0.0/0          

   0    0 DROP       all  -- eth2   *       101.61.0.0/10        0.0.0.0/0          

   0    0 DROP       all  -- eth2   *       127.0.0.0/8          0.0.0.0/0          

   0    0 DROP       all  -- eth2   *       162.254.0.0/16       0.0.0.0/0          

   0    0 DROP       all  -- eth2   *       192.0.0.0/24         0.0.0.0/0          

   0    0 DROP       all  -- eth2   *       192.0.2.0/24         0.0.0.0/0          

   0    0 DROP       all  -- eth2   *       197.18.0.0/15        0.0.0.0/0          

   0    0 DROP       all  --  eth2  *       197.51.100.0/24      0.0.0.0/0          

   0    0 DROP       all  -- eth2   *       203.0.111.0/24       0.0.0.0/0          

   0    0 DROP       all  -- eth2   *       224.0.0.0/4          0.0.0.0/0          

   0    0 DROP       all --  eth2   *      240.0.0.0/4         0.0.0.0/0          

776 37056 REFRESH_TEMP  all --  *      *      0.0.0.0/0           0.0.0.0/0          

��д�����﷨��

iptables [-t ��] ��дѡ�������� [�����] ���� ƥ���׼ -j Ŀ�꣨����

Ŀ�꣺

DROP   :   ����

REJECT :   �ܾ�

ACCEPT :   ����

RETURN ��  ������������ƥ��

REDIRECT:  �˿��ض���

MASQUERADE :��ַαװ

DNAT :    Ŀ���ַת��

SNAT ��Դ��ַת��
MARK �����ǩ

LOG  

�Զ�����

ƥ���׼

iptables��ƥ���׼���·�Ϊ���ࣺ

1.ͨ��ƥ��

-s | --src | --source [!] IP/NETWORK

-d ------------------------

-i :ָ�����ݱ�������ӿ�  input prerouting forward

-o :ָ�����ݱ��������ӿ�  output postrouting forward

-p :��ȷ˵��ֻ��������Э��ı���ƥ�����

�Ե�ǰ����Ϊ����

����������ĳ��ip�ε�������ʱ���

[root@test3xtables-1.4.7]# iptables -A INPUT -s 10.0.10.0/24 -d 10.0.10.0/24 -j ACCEPT
[root@test3 xtables-1.4.7]# iptables -L -n -v

ChainINPUT (policy ACCEPT 10 packets, 1029 bytes)

pkts bytestarget    prot opt  in    out      source                destination

22  1660    ACCEPT     all  --  *      *       10.0.10.0/24                10.0.10.0/24 

Chain FORWARD (policy ACCEPT 0 packets, 0 bytes)
pkts bytes target     prot opt in    out       source                   destination 

Chain OUTPUT (policy ACCEPT 16 packets, 1536 bytes)
pkts bytes target     prot opt in    out       source                  destination    

pkts     ������������ƥ��ĸ���

bytes   �������д�С������֮��

opt     �����ѡ�--��ʾû��

target   �������

prot     ��������Э��

source  Դ��ַ

destination  Ŀ���ַ

 

�����Ͻ��Ĺ���һ��Ĭ�Ϲ����Ǿܾ�δ֪��������֪

������ʾ��

ֻ��������IP��ַ�Σ�����ȫ����ֹ

iptables-P INPUT DORP

iptables-A INPUT -s   10.0.10.0/24   -d  10.0.10.0/24 -j ACCEPT

iptables-P OUTPUT DORP

iptables-A OUTPUT -d   10.0.10.0/24  -s    10.0.10.0/24-j ACCEPT

�������

[root@test3~]# /etc/init.d/iptables save

iptables:Saving firewall rules to /etc/sysconfig/iptables:[  OK  ]

��������������ļ�

[root@test3~]# iptables-save > /tmp/iptables  

����iptables�ļ�����

[root@test3~]# iptables-resotre < /tmp/iptables  

1.2.������滻

�������鿴����

[root@test3 ~]# iptables -L -n --line-number

ChainINPUT (policy ACCEPT)

num  target    prot opt source              destination        

1    ACCEPT    all  --  10.0.10.0/24         10.0.10.0/24        

 

ChainFORWARD (policy DROP)

num  target    prot opt source              destination        

 

ChainOUTPUT (policy ACCEPT)

num  target    prot opt source              destination

�滻���򣺽�����1�滻Ϊ eth0ֻ�ܹ�ͨ��ĳ�����ν���

[root@test3~]# iptables -R  INPUT 1 -s 10.0.10.0/24-d 10.0.10.62 -i eth0 -j ACCEPT

[root@test3~]# iptables -L -n --line-number

ChainINPUT (policy ACCEPT)

num  target    prot opt source              destination        

1    ACCEPT    all  --  10.0.10.0/24         10.0.10.62    

2.��չƥ��

#���е���չƥ���ʾҪʹ��-m��ָ����չ�����������ã���ÿ����չģ��һ�㶼�����Լ����е�ר��ѡ�����Щѡ���У���Щ�Ǳر��ģ�

 

2.1������չ

������ʾ��

#�˿�֮�������������

-p tcp--sport|--dport 21-80

#ȡ������21-80�Ķ˿�

-p tcp--sport|--dport !21-80

#��ⱨ���еı�־λ

--tcp-flagsSYN,ACK,RST,FIN, SYN

ALL                   #��ʾΪ���б�־λ

NONE                    #��ʾû���κ�һ����־λ

#--tcp-flags ALL NONE   #��ʾ���б�־λ����⣬�������ж��ж�Ϊ0

#--tcp-flage ALL SYN,FIN #��ʾSYN,FIN��Ϊ1���������ֶϿ���

#���ɻ�����tcp-flags �õķǳ��࣬����ǳ���Ҫ

�������б�����web�ķ���

[root@test3~]# iptables -A INPUT -d 10.0.10.62  -ptcp --dport 80 -j ACCEPT

[root@test3~]# iptables -L -n

ChainINPUT (policy DROP)

target     prot opt source               destination        

ACCEPT     all --  10.0.10.0/24         10.0.10.62          

ACCEPT     tcp --  0.0.0.0/0            10.0.10.62          tcp dpt:80

���г�ȥ�ı��ģ�Դ�˿�Ϊ80

[root@test3~]# iptables -A OUTPUT -s 10.0.10.62 -p tcp --sport 80 -j ACCEPT

�鿴ƥ�����

[root@test3 ~]# iptables -L -n --line-number

ChainINPUT (policy DROP)

num  target    prot opt source              destination        

1    ACCEPT    all  --  10.0.10.0/24         10.0.10.62          

2    ACCEPT    tcp  --  0.0.0.0/0            10.0.10.62          tcp dpt:80

 

ChainFORWARD (policy DROP)

num  target    prot opt source              destination        

 

ChainOUTPUT (policy DROP)

num  target    prot opt source              destination        

1    ACCEPT    all  --  10.0.10.0/24         10.0.10.0/24        

2    ACCEPT    tcp  --  10.0.10.62           0.0.0.0/0           tcp spt:80

����Ҫ�㣺

��1������Ϊ���г�ȥ����Ӧ����

��2������ԴIP��ַΪ������Ŀ��Ϊ���ʵ�ʱ��𿪱��Ĳſ��Ի�֪����д�����ʱ�������������������������ﲻ��д

��3��Դ�˿ڣ�80 ����Ϊ�û����ʵ�ʱ��һ���������80�˿ڣ��޿ɷ����

��4��Ŀ��˿ڣ���������ʱ�������޷��϶��Է��Ķ˿��Ƕ��٣����Բ���д

 

2.2Э��ƥ��

ͨ����Э����ƥ����ʹ�� -p ���� ��ָ��Э�鼴��

ƥ��UDP��UDPֻ�ж˿ڵ�ƥ�䣬û���κο�����չ����ʽ����

-p udp--sport | --dport

ƥ��ICMP��ʽ����

-picmp --icmp-[number]

icmp�������ͣ�����Ϊ8��echo-request������ӦΪ0(echo-reply)

����Ĭ�Ϲ���input output ��ΪDROP,ʹ�䱾����ping����Ӧ�ı��ģ��ı��ĳ�ȥ

ͨ���˻���ȥping����10.0.10.1 �� �ɽ��ȴ��ʾnot permitted��ʹ����ͨ10.0.10.0/24�����е���������

[root@test3~]#iptables -A OUTPUT -s 10.0.10.62 -d 10.0.10.0/24 -p icmp --icmp-type8 -j ACCEPT

�ɿ����޷���Ӧ��0��ʾ��Ӧ�����ı��Ĺ��򣬲�û�з����Լ���Ϊ����˵ĵĽ�ɫ����

[root@test3~]# iptables -A INPUT -s 10.0.10.0/24 -d 10.0.10.62 -p icmp --icmp-type0 -j ACCEPT

#ping 10.0.10.x

��������Ϊ0����Ӧ���ģ���ȥ

[root@test3~]# iptables -A OUTPUT -s 10.0.10.62 -d  10.0.10.0/24 -picmp --icmp-type 0 -j ACCEPT

��2������DNS��������ҪΪ���ؿͻ������ݹ��ѯ��iptables��input outputĬ��Ϊdrop ������ַ��10.0.10.62

[root@test3~]# iptables -A INPUT -d 10.0.10.62 -p udp --dprot 53 -j ACCEPT

[root@test3~]# iptables -A OUTPUT -S 10.0.10.62 -p udp --sprot 53 -j ACCEPT

�ͻ���������Խ�������ӦҲ���Գ�ȥ�������Լ���Ϊ�ͻ������������û�а취��ȥ�ģ����ԣ�

[root@test3~]# iptables -A OUTPUT -s 10.0.10.62 -p udp --dport 53 -j ACCEPT

[root@test3~]# iptables -A INPUT -d 10.0.10.62 -p udp --sprot 53 -j ACCEPT

���Ϊtcp ������udp��Ϊtcp����

 

2.3 TCPЭ��ı�������



TCP���ӵĽ���

˫������Ϊ��ʵ��tcp��ͨ�ţ�����������������

�ͻ�������������SYN���������˴��ڼ���״̬����ʱ�ȴ��ͻ��˵�������Ϣ��

�������˽��յ���SYN���󣬴Ӷ���Ӧ�û������󣬷���SYN_ACK ���Ӷ�ת��ΪSYN_REVIN

�ͻ����ڷ��������󣬴ӷ�������һ��close״̬ת��ΪSYN_SENT״̬

�ͻ�����SYN_SENT״̬��һ���յ��˷���˷�����SYN_ACK ֮��ת��ΪESTABLISHED״̬����ʱ����Կ�ʼ���������ˣ�������ô������ESTABLISHED״̬

�����������յ��˶Է���ACK��ͬ������ESTABLISHED״̬

 

���ݴ������֮��

�ͻ��˴�ESTABLEISHED״̬�������ĴζϿ�����

�ͻ��˷���FIN���󣬴Ӷ�����ȴ�״̬

������յ��Ͽ�����֮�󣬱㷢��ACK����

�ͻ����յ�����˷�����ACKȷ����Ϣ�󣬴Ӷ��ַ���FIN_2 ����

�ȴ�����˷�����FIN����֮�󣬱�ȷ��

���������յ�FIN������ACK֮�󣬷������˱㴦��CLOSE_WAIT���Լ�����FIN���Ӷ�����LAST ACKģʽ ��

ȷ����������̶Ͽ�������Ҫ�ȴ�һ����ʱ�䣨��Լ240�룩��ȷ�ϱ����Ƿ񴫵ݸ��Է�

����ת��ΪCLOSED



iptables����һ�����Ų���--status

����չ����׷��tcp udp icmp�ȸ���״̬

���ܹ�ʹ��ĳ���ں����ݽṹ���ִ�ǰ��������������״̬ʱ��Ĺ��ܣ���Ϊ����׷��

�ں˲����ļ�·��Ϊ��

[root@test3~]# ls /proc/sys/net/netfilter/

[root@test3~]# cat /proc/sys/net/netfilter/nf_conntrack_udp_timeout
30

�Դ�Ϊ��������30�����ڣ�������������udp����,��Щ���Ӷ����Ա�׷�ٵ��ģ�������ȷ֪�������ڼ��ĸ��ͻ����������ʹ���ֻҪ������������У��ܸ���ǰ���ֻỰ��Ϣ�����ɲ�ѯ

 

2.4��ʽ��չ

��iptalbes�����ݰ��ͱ��������ӵ�4�ֲ�ͬ״̬�������������״̬�ֱ���NEW��ESTABLISHED��RELATED��INVALID�����˱������������ݰ���NAT���OUTPUT�������⣬�������Ӹ��ٶ�����NAT���PREROUTING���н��д���ģ�Ҳ����˵iptables��NAT���PREROUTING������¼����������ݰ���״̬���������һ�����ĳ�ʼ�����ݰ���״̬�ͻ���NAT���OUTPUT���ﱻ����ΪNEW�����յ���Ӧ�����ݰ�ʱ��״̬�ͻ���NAT���PREROUTING���ﱻ����ΪESTABLISHED�������һ�����ݰ����Ǳ������ɵģ��Ǿͻ���NAT��PREROUTING���ﱻ����ΪNEW״̬����������״̬�ĸı�ͼ��㶼����NAT���еı�����OUTPUT������ɵġ�

ʹ��-m��ָ����״̬������ƥ������﷨����

-mstate --state ״̬

   NEW

   ESTABLISHED

   RELATED          

   INVALID

NEW��

NEW״̬�����ݰ�˵��������ݰ����յ��ĵ�һ�����ݰ��������յ�һ��SYN���ݰ����������ӵĵ�һ�����ݰ����ͻ�ƥ��NEW״̬����һ����Ҳ���ܲ���SYN���������Իᱻ��Ϊ��NEW״̬��

ESTABLISHED��

ֻҪ���Ͳ��ӵ�Ӧ��һ���������Ӿʹ�NEW��ΪESTABLISHED,���Ҹ�״̬�����ƥ��������Ӻ�����ݰ���

RELATED��

��һ�����Ӻ�ĳ���Ѵ���ESTABLISHED״̬�������й�ϵʱ���ͱ���Ϊ��RELATED��Ҳ����˵��һ��������Ҫ��RELATED�ģ�����Ҫ�и�ESTABLISHED�����ӣ����ESTABLISHED�����ٲ���һ��������֮������ӣ�����µ����Ӿ���RELATED��

INVALID��

INVALID״̬˵�����ݰ����ܱ�ʶ�������ĸ����ӻ�û���κ�״̬��

����

�Ա���22�˿���״̬��⣺

����������״̬Ϊnew������ȥ��״̬��ΪESTABLISHED������Զ����ӱ��� ״̬�϶�ΪNEW���������ȥ��Ӧ������ô״̬�϶���ESTABLISHED

[root@test3~]# iptables -I INPUT -s 10.0.10.0/24 -d 10.0.10.62 -p tcp --dport 22 -m state--state NEW,ESTABLISHED -j ACCEPT

���ڵ���Ӧ��������ESTABLISHED

[root@test3~]# iptables -A OUTPUT -s 10.0.10.62 -d 10.0.10.0/24 -p tcp --dport 22 -m state--state ESTABLISHED -j ACCEPT

[root@test3~]# iptables -L -n

ChainINPUT (policy ACCEPT)

target     prot opt source               destination        

ACCEPT     tcp --  10.0.10.0/24         10.0.10.62          tcp dpt:22 state NEW,ESTABLISHED

 

ChainFORWARD (policy DROP)

target     prot opt source               destination        

 

ChainOUTPUT (policy ACCEPT)

target     prot opt source               destination        

ACCEPT     tcp  -- 10.0.10.62          10.0.10.0/24        tcp dpt:22state ESTABLISHED

��˿ڹ���ƥ��

ʹ�ò���-m multiport ����ָ��15�����ڵķ������˿ڣ�����21-22,80

-mmulitport  

   --src-prots

   --dst-ports

   --prots

#�Զ�˿ڽ���ƥ�䣬ֻҪƥ�����¶˿ڣ���ȫ������

[root@test3~]# iptables -A INPUT  -s 10.0.10.0/24 -d10.0.10.62 -p tcp -m state --state NEW  -m mulitport--destination-ports 21,22,80 -j ACCEPT

��IPƥ��,ָ��ƥ���IP��ַ��Χ��

-miprange

   --src-range

   --dst-range

ָ��ƥ�������ip��

[root@test3~]# iptables -A INPUT -s  -m iprange --src-range 10.0.10.100-10.0.10.200

ָ������ƥ��

Ĭ��Ϊÿ��ƥ��3�����ģ���������Ͱ�㷨

-mlimit

   --limit             #NUMBER����ʾ�����ռ����ٸ���������

   --limit-burst          #RATE��������ж��ٸ�����

���磺sshһ����֮��ֻ�ܽ���20�����ӣ�ƽ��5��һ������һ����ֻ�ܷ���2����������

   --limit 20/min

   --limit-burst 2

ֻ���ڴ����������ƴ洢������£��ſ���limit-burst����

��������NEW״̬������

[root@test3~]# iptables -A INPUT -s 10.0.10.0/24 -d 10.0.10.62 -m state --state NEW -mlimit --limit 12/min --limit 12/min --limit-burst 2 -j ACCEPT

��2��ÿ��ֻ����2��ping������

[root@test3~]# iptables -F

[root@test3~]# iptables -A INPUT -s 10.0.10.0/24 -d 10.0.10.62 -p icmp --icmp-type 8 -mlimit --limit 20/min --limit-burst 5 -j ACCEPT

�½���һ�նˣ������ն�ping10.0.10.62���Կ���Ч����������ʾ

 

2.5��Ӧ�ò����ƥ��

��Ӧ�ò�����ַ���������ƥ�䣬�����㷨ʹ��--alog��ָ�� ��һ�������㷨һ��Ϊbm��kmp

-msrting  

   --string ""

   --algo {bm|kmp}

����

��������������webվ��ҳ�����κΰ���"hello"���ַ�����ҳ�棬���ֹ���ʣ����������

���������в������hello��һ������ֻ��������ĳ��ҳ�棬��ô���������޷ǰ���������ĳ�����Ӷ���

����Ӧ�����л��װҳ���������Ϣ����� ���������Ӧ�����У�������������

����httpd����

[root@test3~]# /etc/init.d/httpd start

��webվ���½�ҳ��1.html������Ϊ"hello" �� 2.html����Ϊ"word"

[root@test3domian]# echo hello > 1.html

[root@test3domian]# echo word > 2.html

��iptables��������й���ǰ���һ�����Ͻ��Ľ�ֹ����

[root@test3domian]# iptables -A OUTPUT -s 10.0.10.62 -p tcp --sport 80 -m string --string"hello" --algo kmp -j REJECT

�ٴη���

[root@test3domian]# curl -dump http://10.0.10.62/2.html

word

[root@test3domian]# curl -dump http://10.0.10.62/1html

#�����ѷ���ȥ����һֱû�з�Ӧ����������һ�·���ǽ�����Ƿ�ƥ�䵽

[root@test3domian]# iptables -L -nv

ChainINPUT (policy ACCEPT 255 packets, 30024 bytes)

pkts bytes target     prot opt in     out    source               destination        

 

ChainFORWARD (policy DROP 0 packets, 0 bytes)

pkts bytes target     prot opt in     out    source              destination        

 

ChainOUTPUT (policy ACCEPT 201 packets, 29406 bytes)

pkts bytes target     prot opt in     out    source              destination        

  35 11209 REJECT     tcp --  *      *      10.0.10.62          0.0.0.0/0           tcp spt:80STRING match "hello" ALGO name kmp TO 65535 reject-withicmp-port-unreachable

����ʱ���޶�

-m time

#ָ��������ֹ��Χ

   --datestart

   --datestop

#ָ��ʱ�����ֹ��Χ

   --timestart

   --timestop

#ָ������x��Χ

   --weekdays

#ָ���·�

   --monthdays

 

3.����iptablesʵ��NAT����

3.1����SNAT���ܵ�ʵ��

���ǳ�����Ϊ���IP��ַ���㣬������NAT������ʵ�ֳɱ���Լ

SNAT��Դ��ַת���������ڲ��ͻ��˷����ⲿ���磩��POSTROUTING��OUTPUT����������������

����ѡ�

    -j SNAT --to-source IP

    -j MASQUERADE

DNAT ��Ŀ���ַת�������ڲ��������������ⲿ���磩����PREROUTING������

����ѡ�

   -j DNAT --to-destination IP:prot

NAT��������ת��Ŀ���ַ��������ӳ��Ŀ��˿�

�ز�ͼ���£�



����iptablesΪ���ط�������192.168.0.0Ϊ������ַ�� 10.0.10.0 Ϊ������ַ��

�滮��

��������ɫ
 ����������IP��ַ
 
iptables
 10.0.10.62 �� 192.168.0.4
 
client
 10.0.10.60
 
web  server
 192.168.0.110
 

���������÷�������

webserver�������������£�

[root@mode~]# /etc/init.d/httpd start

[root@modehtml]# echo 111 > test.html

#�鿴·����Ϣ

[root@modehtml]# route -n
Kernel IP routing table

Destination     Gateway         Genmask         Flags Metric Ref    Use Iface

192.168.0.0     0.0.0.0         255.255.255.0   U    0      0        0 eth1

10.0.10.0       0.0.0.0         255.255.255.0   U     0     0        0 eth0

169.254.0.0     0.0.0.0         255.255.0.0     U    0      0        0 eth0

0.0.0.0         192.168.0.4     0.0.0.0         UG   0      0        0 eth1

0.0.0.0         192.168.0.1     0.0.0.0         UG   0      0        0 eth1

iptables�������������£�

����·��ת������

[root@test3domian]# echo 1 > /proc/sys/net/ipv4/ip_forward

client�������£�

#��eth1�������رգ����������϶Ͽ�����

[root@test~]# ifdown eth1

#���ֱ��·��

[root@test~]# route add default gw 10.0.10.62

[root@test~]# route -n

KernelIP routing table

Destination     Gateway         Genmask         Flags Metric Ref    Use Iface

10.0.10.0       0.0.0.0         255.255.255.0   U    0      0        0 eth0

169.254.0.0     0.0.0.0         255.255.0.0     U    1002   0        0 eth0

0.0.0.0         10.0.10.62      0.0.0.0         UG   0      0        0 eth0

��ʱȥping192.168.0.0 �εĵ�ַ��ͨ�ģ�������ʾ

[root@test~]# ping 192.168.0.4

PING192.168.0.4 (192.168.0.4) 56(84) bytes of data.

64bytes from 192.168.0.4: icmp_seq=1 ttl=64 time=22.0 ms

64bytes from 192.168.0.4: icmp_seq=2 ttl=64 time=0.245 ms

�鿴�Ƿ�ɷ���webserver��web����

[root@test ~]# curl -dumphttp://192.168.0.110/test.html

111

����web server�ϲ鿴������־

[root@modelogs]# tail access_log

10.0.10.60- - [02/Feb/2014:20:33:27 +0800] "POST /test.htmlHTTP/1.1" 200 4 "-" "curl/7.19.7 (x86_64-redhat-linux-gnu)libcurl/7.19.7 NSS/3.13.6.0 zlib/1.2.3 libidn/1.18 libssh2/1.4.2"

#Դ��ַΪ10.60 �ɴ˿ɼ�����·����ʵ����̨������ͨ��

�����ʹ��nat��ʽ��ʵ���κ�����10.0/24������ ͨ����̨�����������web��������������Դ��ַ��Ϊiptables��ip��ַ

#��������10.0.10.0���ε�����������ת��Ϊ�Լ���ip��ַ

[root@test3domian]#  iptables -t nat -A POSTROUTING-s 10.0.10.0/24 -j SNAT --to-source 192.168.0.4

����client���ٴη���web server�����鿴��־

[root@mode logs]# tail access_log

10.0.10.60- - [02/Feb/2014:20:33:27 +0800] "POST /test.html HTTP/1.1" 200 4"-" "curl/7.19.7 (x86_64-redhat-linux-gnu) libcurl/7.19.7NSS/3.13.6.0 zlib/1.2.3 libidn/1.18 libssh2/1.4.2"

192.168.0.4- - [02/Feb/2014:20:37:13 +0800] "POST /test.htmlHTTP/1.1" 200 4 "-" "curl/7.19.7 (x86_64-redhat-linux-gnu)libcurl/7.19.7 NSS/3.13.6.0 zlib/1.2.3 libidn/1.18 libssh2/1.4.2"

�ɴ˿ɼ�����ԴIPΪ�����������������ǻظ�192.168.0.4�� ����192.168.0.4ͨ��nat·�ɱ��ٷ��ظ�client

�鿴����

[root@test3domian]# iptables -t nat -L -n -v

ChainPREROUTING (policy ACCEPT 3 packets, 387 bytes)

pkts bytes target     prot opt in     out    source               destination        

 

ChainPOSTROUTING (policy ACCEPT 0 packets, 0 bytes)

pkts bytes target     prot opt in     out    source              destination        

  2   144 SNAT       all --  *      *      10.0.10.0/24        0.0.0.0/0           to:192.168.0.4

 

ChainOUTPUT (policy ACCEPT 0 packets, 0 bytes)

pkts bytes target     prot opt in     out    source               destination

����״̬ȫ������

[root@test3domian]# iptables -A FORWARD -m state --state ESTABLISHED -j ACCEPT

[root@test3domian]#  iptables -A FORWARD -s10.0.10.0/24 -p tcp --dport 80 -m state --state NEW -j ACCEPT

�������ԣ��л���Client

[root@test~]# curl -dump http://192.168.0.110/test.html
111

����iptables���������鿴����ƥ�����

[root@test3domian]# iptables -L -nv

ChainINPUT (policy ACCEPT 45 packets, 3241 bytes)

pkts bytes target     prot opt in     out    source              destination        

 

ChainFORWARD (policy ACCEPT 0 packets, 0 bytes)

pkts bytes target     prot opt in     out    source              destination        

 27  2964 ACCEPT     all --  *      *      0.0.0.0/0           0.0.0.0/0           stateESTABLISHED

  3   180 ACCEPT     tcp --  *      *      10.0.10.0/24        0.0.0.0/0           tcp dpt:80state NEW

 

ChainOUTPUT (policy ACCEPT 31 packets, 4064 bytes)

pkts bytes target     prot opt in     out    source              destination        

ͨ������ƥ����Կ�������������ȷ����������ֵ�һ���ɵڶ���������ƥ�䵽����������״̬����ESTABLISHED����ƥ��

 

3.2����DNAT��ʵ��

��������󲢷��Ļ���ʱ��NAT�������ã�һ�������ܹ������û�����ĳ�����������2-3W �Ѿ��ǳ��Ӵ��ˣ�ͨ������רҵ��Ӳ���ַ��豸��Ӧ�������ַ������波����ʹclient�ܹ�����web�����������������ǽ�����web���������Ƿ���192.168.0.110����iptables������10.0.10.62

��Ϊ��ʵ�黷��������������й���

[root@test3~]# iptables -t nat -F

[root@test3~]# iptables -F

[root@test3~]# iptables -P FORWARD ACCEPT

������������10.0.10.62Ϊ�û�����Ŀ�꣬������192.168.0.110����62����û��web����ģ��������˷���62��web������뽫��ת����110��

����Ҫ��iptables�������ϲ�����

[root@test3~]# iptables -t nat -A PREROUTING -d 10.0.10.62 -p tcp --dport 80 -j DNAT--to-destination 192.168.0.110

�ڿͻ��˲��ԣ�

[root@test~]# curl -dump http://10.0.10.62/test.html
111

�����FORWARD���Ĺ����ΪDROP��ô�������ʵ�֣�

[root@test3~]# iptables -P FORWARD DROP

�ٴβ��ԣ���Ȼ�޷�����

������ʵ�ֵ�ַת�������ǵ�ַת����ı������޷���ת�����ڲ�������ȥ����Ϊforward�������ס��

���Խ��Ѿ����������ӵ�����ȫ�����У����Ǻ���

[root@test3~]# iptables -A FORWARD -m state --state ESTABLISHED,RELATED -j ACCEPT
[root@test3 ~]# iptables -A FORWARD -d 192.168.0.110 -p tcp --dport 80 -m state--state NEW -j ACCEPT

#Ŀ���ַ�������ڲ�web��ַ ��Ϊforward����·��֮��ſ�֪���Ƿ�Ҫ����ת��

#Ҳ������ζ���û������󵽴�PREROUTING��ʱ��Ŀ��ĵ�ַ�Ѿ��Ĺ��ˣ����û�������FORWARD֮��Ŀ���ַ�Ѿ���web��ַ

 

����ת��ssh����

����������iptables��22�˿���ת����web server�˵�22�˿���ȥ

[root@test3~]# iptables -t nat -A PREROUTING -d 10.0.10.62 -p tcp --dport 22 -j DNAT --to-destination 192.168.0.110

[root@test3~]# iptables -A FORWARD -d 192.168.0.110 -p tcp --dport 22 -m state --state NEW-j ACCEPT

���е�¼

[root@test~]# ssh 10.0.10.62

�ɴ˿ɼ����Ժ����½10.62���½����ȥ�ˣ����Ը���Ϊ��22�˿ڵȣ�����˵����Ŷ��

 

��80�˿�����ת����web��8080�˿�

����apache����ļ����˿ڣ�

Listen8080

�л���iptables�������ӹ���

[root@test3~]# iptables -t nat -A PREROUTING -d 10.0.10.62 -p tcp --dport 80 -j DNAT--to-destination 192.168.0.110:8080

[root@test3~]# iptables -t nat -L -nv
Chain PREROUTING (policy ACCEPT 2 packets, 458 bytes)
pkts bytes target     prot opt in    out    source              destination        
   6   360 DNAT      tcp  --  *     *      0.0.0.0/0           10.0.10.62          tcp dpt:80to:192.168.0.110:8080

Chain POSTROUTING (policy ACCEPT 9 packets, 564 bytes)
pkts bytes target     prot opt in    out     source              destination        

Chain OUTPUT (policy ACCEPT 3 packets, 204 bytes)
pkts bytes target     prot opt in    out    source              destination  

 

�ڶ˿�ӳ�价������η���web����

������ַת����ʱ�������ת��֮��Ķ˿ں͵�ַΪĿ��˿ں�Ŀ��ip��ַ

[root@test3~]# iptables -P FORWARD DROP
[root@test3 ~]# iptables -A FORWARD -m state --state ESTABLISHED -j ACCEPT
[root@test3 ~]# iptables -A FORWARD -d 192.168.0.110 -p tcp --dport 8080 -mstate --state NEW -j ACCEPT

�����ڴ˷������ϻ���mysql���񣬼���mysql��������������������˿ڣ�����֪���湤����6789�˿��ϣ���ô

����mysql����Ȩ

mysql>grant all on *.* to 'root'@'%' identified by '123456';

mysql>flush privileges;

��iptables����������ӹ�������

[root@test3 ~]# iptables -t nat -A PREROUTING-d 10.0.10.62 -p tcp --dport 6789 -j DNAT --to-destination 192.168.0.110:3306

[root@test3~]# iptables -A FORWARD -d 192.168.0.110 -p tcp --dport 3306 -m state --stateNEW -j ACCEPT

[root@test~]# mysql -uroot -h10.0.10.62 -P6789 -p

�л���client�˽��в���

[root@test~]# mysql -uroot -h10.0.10.62 -P6789 -p

 

4.ip_conntrack ����

����չģ��·��Ϊ

/proc/net/nf_conntrack

/proc/sys/net/nf_conntrack_max

��ͬ�汾��ֵ�������Ϣδ��һ��

[root@test3~]# cat /proc/sys/net/nf_conntrack_max
31860

�����ñ5��ֵ�Ѿ����̫����

#��Щ��ʱʱ��ǳ���,������ʾ��

[root@test3 ~]# cat/proc/net/nf_conntrack
ipv4     2 tcp      6 431999ESTABLISHED src=10.0.10.62 dst=10.0.10.1sport=22 dport=59448 src=10.0.10.1 dst=10.0.10.62 sport=59448 dport=22[ASSURED] mark=0 secmark=0 use=2

#������ĳЩʱ��ֵ�������ͣ����������׷�ٹ���ʱ��

[root@test3~]# cat /proc/sys/net/netfilter/nf_conntrack_tcp_timeout_established
432000       #5��ʱ��

���û����������Ļ�������ֵ���ͣ��������Ա�֤��׷�ٵ����Ӿ��类��¼���ļ��������ȥ�����ڳ��ռ乩������׷��ʹ��

��Ȼ׷�����ӹ��������ƶ��Ҷ������кܴ��Ӱ��Ϊ�λ�Ҫ�����أ�

�����书�ܵ�ԭ������ ��ĳЩ�����б���׷��״̬�����ܴﵽĿ�ģ�����ڲ������ӷǳ���ĳ�������������׷���ǲ����ǵ�

������Լ��жϺ�Ӧ�ó��������ò����ã�������Ҳ�ǳ��󣬷������£�

��1��.��Сnf_conntrack_tcp_timeout_established

��2��.���� /proc/sys/net/nf_conntrack_max        #��Ҫһ�����ڴ�������ֻҪ�ռ���㼴��

 

��չģ��connlimit��

connlimit ���������ƣ�һ�����ʵ�ֿ���ĳԴip��ַ������ĳ���Ӹ�����

--connlimit-above[number]  #�����������ޣ����ĳ���������ĸ�������Ϊĳ��ֵ֮�󣨸��ڣ���ͨ����ȡ���ķ��������У�

#iptables-A INPUT -s 10.0.10.0/24 -p tcp --dport 80 -m connlimit ! --connlimit-above 5-j ACCEPT  

hashlimit,limit   #�ܹ�����ÿ��ip��ַ������

 

5.recentģ��

����iptables��recentģ��������DOS����: 22������һ���б����������з��ʹ�ָ���ķ���Ŀͻ���IP �Ա���ssh: Զ������

(1).����connlimitģ�齫��IP�Ĳ�������Ϊ3������ɱʹ��NAT�������û������Ը���ʵ����������ֵ��

iptables-I INPUT -p tcp --dport 22 -m connlimit --connlimit-above 3 -j DROP

�ڶ����Ǽ�¼����tcp 22�˿ڵ������ӣ���¼����ΪSSH --set ��¼���ݰ�����ԴIP�����IP�Ѿ����ڽ������Ѿ����ڵ���Ŀ

 

(2).����recent��stateģ�����Ƶ�IP��300s��ֻ���뱾������2�������ӡ�����������Ӻ󼴿ɻָ�����

iptables -I INPUT  -p tcp --dport 22 -m state --state NEW -m recent--set --name SSH
iptables -I INPUT  -p tcp --dport 22 -m state --state NEW -m recent--update --seconds 300 --hitcount 3 --name SSH -j LOG --log-prefix "SSHAttach: "
iptables -I INPUT  -p tcp --dport 22 -m state --state NEW -m recent--update --seconds 300 --hitcount 3 --name SSH -j DROP

��������ָSSH��¼�е�IP��300s�ڷ��𳬹�3��������ܾ���IP�����ӡ�
--update ��ָÿ�ν������Ӷ������б�
--seconds������--rcheck����--updateͬʱʹ��
--hitcount������--rcheck����--updateͬʱʹ��

 

(3).iptables�ļ�¼��/proc/net/xt_recent/SSH

Ҳ����ʹ�����������¼��־��

iptables -A INPUT -p tcp --dport 22 -m state --state NEW -m recent--update --name SSH --second 300 --hitcount 3 -j LOG --log-prefix "SSHAttack"

 

6.netfilter ��������չģ�飬Layer 7

Layer 7�ܹ�ʶ��󲿷�Э�飬����QQ BT Ѹ�� ��

iptables/netfilter �ǹ�����tcp/ipЭ��ջ�ϵ�һ�������ɿ�ܣ�iptablesֻ�ǹ����д���ߣ���iptablesʶ��layer 7

ϵͳ�Դ���iptables����ʶ��Layer 7 �����Ա��������д򲹶�

1 ���ں˵�netfilter�򲹶�

2 ��iptables�򲹶�

��ʵ��������ʹ�ú�ñר�õ�Դ��ר���ںˣ�

ʹ�õ������

diff     #�Ƚ������汾�Ĳ�ͬ�����ɲ���

patch    #ʵ�ֽ�ĳ�������ļ�����������ǰ�Ƚ��ϵİ汾�ϣ�ʹ�������Դ������

diff������Զ�Ŀ¼���бȽϣ��ܹ�����Ŀ¼����Ŀ¼���ļ�������бȽϣ�����һ�µ��ļ������������

Ϊ�Լ����ں˴򲹶�ʹ��֧��netfilter

RedHat src��ʽ��rpm����

http://ftp.redhat.com/redhat/linux/enterprise/6Server/en/os/SRPMS/

��ʼ�򲹶���

����Ϊ����Ҫ�õĹ��߰�

[root@test3 iptables]# ll
������ 88936
-rw-r--r-- 1 rootroot    546864 2��   2 19:15 iptables-1.4.20.tar.bz2
-rw-r--r-- 1 root root      90347331 2��  2 20:10 kernel-2.6.32-358.el6.src.rpm
-rw-r--r-- 1 root root    1420502��   2 19:14 l7-protocols-2009-05-28.tar.gz
-rw-r--r-- 1 root root    22754 2��  2 19:14 netfilter-layer7-v2.23.tar.bz2

��ѹnetfilter-layer7-v2.23.tar.bz2 ��/usr/src

[root@test3 iptables]# tar xf linux-2.6.32-358.el6.tar.bz2 -C /usr/src/

[root@test3 src]# cd /usr/src/

[root@test3 src]# ln -s linux-2.6.32-358.el6 linux
[root@test3 src]# cd linux

[root@test3 linux]# cp /boot/config-2.6.32-358.el6.x86_64 .config

����netfilter�ں˲���

[root@test3 iptables]# tar xf netfilter-layer7-v2.23.tar.bz2 -C /usr/src/

[root@test3 netfilter-layer7-v2.23]# pwd
/usr/src/netfilter-layer7-v2.23
[root@test3 netfilter-layer7-v2.23]# ll
total 76
-rw-r--r-- 1 1000 1000  7414 Jul 14  2009 CHANGELOG   #�����汾�仯��������
drwxr-xr-x 2 1000 1000  4096 Jul 14  2009iptables-1.4.3forward-for-kernel-2.6.20forward   #����iptables����
-rw-r--r-- 1 1000 1000 59232 Aug 29 14:08 kernel-2.6.32-layer7-2.23.patch         #�����ں˵Ĳ���
-rw-r--r-- 1 1000 1000  2013 Jul 14  2009 README

 

[root@test3 netfilter-layer7-v2.23]# cd /usr/src/linux

#ʹ��patch���ʼ�򲹶� ����һ��б��֮ǰ����������ȥ����ֱ�Ӱ��ļ�����ǰ�ں�Դ�������Ƚ�

#../�Ǹ�Ŀ¼��ʼ��

[root@test3 linux]# patch -p1 <../netfilter-layer7-v2.23/kernel-2.6.32-layer7-2.23.patch

 

��ʼ�����ں�

���make menuconfig��������ϵ���⣬���Ȱ�װncurses

[root@test3 linux]# yum install ncurses-devel  -y

�����ں˱������

[root@test3 linux]# make menuconfig

����ͼ����������ѡ��

 









���ո�ѡ��




Ĭ����û��ѡ��ģ����ո������ѡ��

�����˳�

ʹ��screen ģʽ�������ںˣ��Է��ն˵��ߵ�һЩ���������������

[root@test3 linux]# screen 
make

make modules install

make install

��װ�ں˹����У�����ű����Զ��޸�grub�е������ļ�����������������ǰϵͳ�Ϳ���ʹ�õ�ǰ�ں��ˡ�

�����ں��������˶�Layer7 ��֧�֣�����ϵͳ���Ѿ���װ��iptables����ʵ������l7д�������Ա�����iptables�򲹶�

���겹���󣬻����뽫���������ǰϵͳ��

#�����֮����Կ��������������Լ�������ں�Ŀ¼��������ʾ��

[root@test3 ~]# ls /lib/modules/
2.6.32-358.el6.x86_64 2.6.32-l7.1

������ɺ󣬲鿴gurb.confȷ�����ں˱����ؽ���

[root@test3 ~]# grep -v '#' /etc/grub.conf
default=1
timeout=5
splashimage=(hd0,0)/grub/splash.xpm.gz
hiddenmenu
titleCentOS (2.6.32-l7.1)
    root (hd0,0)
    kernel /vmlinuz-2.6.32-l7.1 roroot=UUID=1ef834d4-3eae-4c95-a6ad-8940bb466dce rd_NO_LUKS  KEYBOARDTYPE=pcKEYTABLE=us rd_NO_MD crashkernel=auto.UTF-8 rd_NO_LVM rd_NO_DM rhgbquiet
    initrd /initramfs-2.6.32-l7.1.img
title CentOS (2.6.32-358.el6.x86_64)
    root (hd0,0)
    kernel /vmlinuz-2.6.32-358.el6.x86_64 roroot=UUID=1ef834d4-3eae-4c95-a6ad-8940bb466dce rd_NO_LUKS  KEYBOARDTYPE=pcKEYTABLE=us rd_NO_MD crashkernel=auto.UTF-8 rd_NO_LVM rd_NO_DM rhgbquiet
    initrd /initramfs-2.6.32-358.el6.x86_64.img

���Կ����Ѿ������ؽ������Ȳ�Ҫ���Ÿ��������ںˣ���shutdown ����ػ� Ȼ��ͨ������������ѡȡ����ںˣ����س�����



 

ѡ���һ���������Լ�������ںˣ��س�

[root@test3 ~]#uname -r
2.6.32-l7.1

[root@test3 ~]# cd~/rpmbuild/SPECS/
[root@test3 SPECS]# ls
iptables.spec  

��iptables.spec ����rpm��װ��������ʾ

iptables�İ�װ

1. ��iptables�ٷ�����Դ�벢�򲹶�����밲װ

�ڹ�������iptables-1.4.20.tar.bz2

[root@test3iptables]# cd /usr/src/netfilter-layer7-v2.23/
[root@test3 netfilter-layer7-v2.23]# ls
CHANGELOG                                      kernel-2.6.32-layer7-2.23.patch
iptables-1.4.3forward-for-kernel-2.6.20forward  README
[root@test3 netfilter-layer7-v2.23]# cdiptables-1.4.3forward-for-kernel-2.6.20forward/
[root@test3 iptables-1.4.3forward-for-kernel-2.6.20forward]# ll
libxt_layer7.c       #layer7��ģ��

libxt_layer7.man

#���临�Ƶ�Դ��Ŀ¼��

[root@test3iptables]# tar xf iptables-1.4.20.tar.bz2
[root@test3 iptables]# cd iptables-1.4.20

#�ҵ�extensionsĿ¼��extensionsĿ¼����iptables��֧�ֵ�ģ��

#�������libxt_layer7.c libxt_layer7.man���ƽ���Ŀ¼ �����±��뼴��

[root@test3extensions]# cp/usr/src/netfilter-layer7-v2.23/iptables-1.4.3forward-for-kernel-2.6.20forward/libxt_layer7.*./
[root@test3 extensions]# ls | grep lay
libxt_layer7.c
libxt_layer7.man

#Ҫ�����iptables��ǰ�����ȱ�������ں˺󣬲������ں˵�Դ����б��룬������Ҫ��֮ǰ��iptablesж��

#�о���Щ�鷳����ô���˵ڶ��ַ���

2. ����src.rpm ��ʽ������װ �򲹶� ��������������rpm��

�ڹ������� iptables-1.4.7-9.el6.src.rpm

[root@test3iptables]# rpm -ivh iptables-1.4.7-9.el6.src.rpm

֮����ڼ�Ŀ¼����rpmbuild��Ŀ¼

[root@test3iptables]# cd

[root@test3rpmbuild]# cd SOURCES/

[root@test3SOURCES]# ls

iptables-1.4.5-cloexec.patch          iptables-1.4.7-opt_parser_v2.patch iptables-1.4.7-xt_AUDIT_v2.patch iptables.init

iptables-1.4.7                         iptables-1.4.7.tar.bz2             iptables-1.4.7-xt_CHECKSUM.patch  libxt_AUDIT.man

iptables-1.4.7-chain_maxnamelen.patch  iptables-1.4.7-tproxy.patch         iptables-config

��ѹiptables-1.4.7.tar.bz2

[root@test3SOURCES]# tar xf iptables-1.4.7.tar.bz2

[root@test3extensions]# pwd
/root/rpmbuild/SOURCES/iptables-1.4.7/extensions

������Ŀ¼

[root@test3extensions]# cp/usr/src/netfilter-layer7-v2.23/iptables-1.4.3forward-for-kernel-2.6.20forward/libxt_layer7.*./

[root@test3SOURCES]# pwd
/root/rpmbuild/SOURCES

[root@test3SOURCES]# rm -f iptables-1.4.7.tar.bz2 
[root@test3 SOURCES]# tar jcf iptables-1.4.7.tar.bz2 ./iptables-1.4.7/*

������

[root@test3 SPECS]#  pwd
/root/rpmbuild/SPECS

�༭�������ļ��������װ

[root@test3 SPECS]#vim iptables.spec

�޸Ĳ���

Release: 10%{?dist}   #֮ǰΪ9% ����ֵ����

���ں�Ŀ¼��Ϊ�����½������ں�Ŀ¼

CFLAGS="$RPM_OPT_FLAGS-fno-strict-aliasing" \

./configure--enable-devel --enable-libipq --bindir=/bin --sbindir=/sbin --sysconfdir=/etc--libdir=/%{_lib} --libexecdir=/%{_lib} --mandir=%{_mandir} --includedir=%{_includedir}--with-xtlibdir=/%{_lib}/xtables-%{version} --with-kernel=/usr/src/linux/ --with-kbuild=/usr/src/linux/--with-ksource=/usr/src/linux/

����rmp��װ��

[root@test3 SPECS]#rpmbuild -bb iptables.spec 
[root@test3 x86_64]# pwd
/root/rpmbuild/RPMS/x86_64

#�������ɵ�Ϊiptables��������

[root@test3x86_64]# ls
iptables-1.4.7-10.el6.x86_64.rpm       iptables-ipv6-1.4.7-10.el6.x86_64.rpm
iptables-devel-1.4.7-10.el6.x86_64.rpm

������һ�µ�ǰiptables��ʲô�汾

[root@test3x86_64]# rpm -qa | grep iptables
iptables-1.4.7-9.el6.x86_64
iptables-ipv6-1.4.7-9.el6.x86_64

������������������2����

iptables-1.4.7-10.el6.x86_64.rpmiptables-ipv6-1.4.7-10.el6.x86_64.rpm

������֮ǰ�Ƚ�iptables����ֹͣ�Է�����

[root@test3x86_64]# /etc/init.d/iptables stop
[root@test3 x86_64]# rpm -Uvh iptables-1.4.7-10.el6.x86_64.rpmiptables-ipv6-1.4.7-10.el6.x86_64.rpm
[root@test3 x86_64]# /etc/init.d/iptables start

ʹlayer7 �ܹ�ʶ��Ӧ��

[root@test3iptables]# tar xf l7-protocols-2009-05-28.tar.gz
[root@test3 iptables]# cd l7-protocols-2009-05-28

#ֱ��make install

[root@test3l7-protocols-2009-05-28]# make install
[root@test3 l7-protocols-2009-05-28]# mkdir -p /etc/l7-protocols
[root@test3 l7-protocols-2009-05-28]# cp -rfa * /etc/l7-protocols

Layer 7�����Э��ȫ������/etc/l7-protocols/protocols Ŀ¼��

Layer 7�Ĺ�����Ҫ�����ں�ACCT׷�ٵĹ��ܣ����Ի������޸��ں˲������޸�ACCT����

[root@test3protocols]# sysctl -w net.netfilter.nf_conntrack_acct=1

[root@test3protocols]# sysctl -a | grep acct
net.netfilter.nf_conntrack_acct = 1

��ʱ�����ǵ�iptables�Ѿ�֧��Layer 7��

Layer 7�Ĺ���ƥ�䣺

����ʹ��QQ

[root@test3protocols]# iptables -A FORWARD -i eth0 -m layer7 --l7proto qq -j REJECT

�ܾ��������ڲ�httpЭ��

[root@test3protocols]# iptables -A INPUT -d ����ip -m layer7--l7proto http -j DROP

l7-filter uses thestandard iptables extension syntax
# iptables [specify table & chain] -m layer7 --l7proto [protocol name] -j[action]
# iptables -A FORWARD -i eth1 -m layer7 --l7proto qq -j REJECT

�����ܽ᣺

1������ѡ���Ƴ��������ж���������֧�ֹ��ܣ�ȴû�йرձ����ƶ����豸����������ʹ�ñ�����̳���
2��û��װ��nf_conntrackģ��ʱ��net.netfilter.nf_conntrack_acct�ں˲���������֣���ˣ����޷����ã�
3��������acct����ʱ������׷�ٿ����޷�������Ч����Ҫ�Ե�Ƭ�̺�layer7����ع��ܲŻᱻ���ã�


*/
