#ifndef _TERMIOS_H
#define _TERMIOS_H

#define TTY_BUF_SIZE 1024	// tty �еĻ��������ȡ�

/* 0x54 is just a magic number to make these relatively uniqe ('T') */
/* 0x54 ֻ��һ��ħ����Ŀ����Ϊ��ʹ��Щ����Ψһ('T') */

// tty �豸��ioctl ���������ioctl ����������ڵ�λ���С�
// ��������TC[*]�ĺ�����tty �������
// ȡ��Ӧ�ն�termios �ṹ�е���Ϣ(�μ�tcgetattr())��
#define TCGETS 0x5401
// ������Ӧ�ն�termios �ṹ�е���Ϣ(�μ�tcsetattr()��TCSANOW)��
#define TCSETS 0x5402
// �������ն�termios ����Ϣ֮ǰ����Ҫ�ȵȴ�����������������ݴ�����(�ľ�)�������޸Ĳ���
// ��Ӱ����������������Ҫʹ��������ʽ(�μ�tcsetattr()��TCSADRAIN ѡ��)��
#define TCSETSW 0x5403
// ������termios ����Ϣ֮ǰ����Ҫ�ȵȴ�����������������ݴ����꣬����ˢ��(���)������С�
// �����ã��μ�tcsetattr()��TCSAFLUSH ѡ���
#define TCSETSF 0x5404
// ȡ��Ӧ�ն�termio �ṹ�е���Ϣ(�μ�tcgetattr())��
#define TCGETA 0x5405
// ������Ӧ�ն�termio �ṹ�е���Ϣ(�μ�tcsetattr()��TCSANOW ѡ��)��
#define TCSETA 0x5406
// �������ն�termio ����Ϣ֮ǰ����Ҫ�ȵȴ�����������������ݴ�����(�ľ�)�������޸Ĳ���
// ��Ӱ����������������Ҫʹ��������ʽ(�μ�tcsetattr()��TCSADRAIN ѡ��)��
#define TCSETAW 0x5407
// ������termio ����Ϣ֮ǰ����Ҫ�ȵȴ�����������������ݴ����꣬����ˢ��(���)������С�
// �����ã��μ�tcsetattr()��TCSAFLUSH ѡ���
#define TCSETAF 0x5408
// �ȴ�������д������(��)���������ֵ��0������һ��break���μ�tcsendbreak()��tcdrain()����
#define TCSBRK 0x5409
// ��ʼ/ֹͣ���ơ��������ֵ��0�����������������1�������¿������������������2�������
// ���룻�����3�������¿�����������루�μ�tcflow()����
#define TCXONC 0x540A
//ˢ����д�������û���ͻ����յ���û�ж����ݡ����������0����ˢ��(���)������У������1��
// ��ˢ��������У������2����ˢ�������������У��μ�tcflush()����
#define TCFLSH 0x540B
// ��������TIOC[*]�ĺ�����tty ��������������
// �����ն˴�����·ר��ģʽ��
#define TIOCEXCL 0x540C
// ��λ�ն˴�����·ר��ģʽ��
#define TIOCNXCL 0x540D
// ����tty Ϊ�����նˡ�(TIOCNOTTY - ��ֹtty Ϊ�����ն�)��
#define TIOCSCTTY 0x540E
// ��ȡָ���ն��豸���̵���id(�μ�tcgetpgrp())��
#define TIOCGPGRP 0x540F
// ����ָ���ն��豸���̵���id(�μ�tcsetpgrp())��
#define TIOCSPGRP 0x5410
// ������������л�δ�ͳ����ַ�����
#define TIOCOUTQ 0x5411
// ģ���ն����롣��������һ��ָ���ַ���ָ����Ϊ����������װ���ַ������ն��ϼ���ġ��û�����
// �ڸÿ����ն��Ͼ��г����û�Ȩ�޻���ж����Ȩ�ޡ�
#define TIOCSTI 0x5412
// ��ȡ�ն��豸���ڴ�С��Ϣ���μ�winsize �ṹ����
#define TIOCGWINSZ 0x5413
// �����ն��豸���ڴ�С��Ϣ���μ�winsize �ṹ����
#define TIOCSWINSZ 0x5414
// ����modem ״̬�������ߵĵ�ǰ״̬����λ��־�����μ�����185-196 �У���
#define TIOCMGET 0x5415
// ���õ���modem ״̬�������ߵ�״̬(true ��false)(Individual control line Set)��
#define TIOCMBIS 0x5416
// ��λ����modem ״̬�������ߵ�״̬(Individual control line clear)��
#define TIOCMBIC 0x5417
// ����modem ״̬���ߵ�״̬�����ĳһ����λ��λ����modem ��Ӧ��״̬���߽���Ϊ��Ч��
#define TIOCMSET 0x5418
// ��ȡ����ز�����־(1 - ������0 - �ر�)��
// ���ڱ������ӵ��ն˻������豸������ز���־�ǿ����ģ�����ʹ��modem ��·���ն˻��豸��
// �ǹرյġ�Ϊ����ʹ��������ioctl ���ã�tty ��·Ӧ������O_NDELAY ��ʽ�򿪵ģ�����open()
// �Ͳ���ȴ��ز���
#define TIOCGSOFTCAR 0x5419
// ��������ز�����־(1 - ������0 - �ر�)��
#define TIOCSSOFTCAR 0x541A
// ������������л�δȡ���ַ�����Ŀ��
#define TIOCINQ 0x541B

// ���ڴ�С(Window size)���Խṹ���ڴ��ڻ����п����ڻ�����Ļ��Ӧ�ó���
// ioctls �е�TIOCGWINSZ ��TIOCSWINSZ ��������ȡ��������Щ��Ϣ��
struct winsize
{
  unsigned short ws_row;	// �����ַ�������
  unsigned short ws_col;	// �����ַ�������
  unsigned short ws_xpixel;	// ���ڿ�ȣ�����ֵ��
  unsigned short ws_ypixel;	// ���ڸ߶ȣ�����ֵ��
};

// AT&T ϵͳV ��termio �ṹ��
#define NCC 8			// termio �ṹ�п����ַ�����ĳ��ȡ�
struct termio
{
  unsigned short c_iflag;	/* input mode flags */// ����ģʽ��־��
  unsigned short c_oflag;	/* output mode flags */// ���ģʽ��־��
  unsigned short c_cflag;	/* control mode flags */// ����ģʽ��־��
  unsigned short c_lflag;	/* local mode flags */// ����ģʽ��־��
  unsigned char c_line;		/* line discipline */// ��·��̣����ʣ���
  unsigned char c_cc[NCC];	/* control characters */// �����ַ����顣
};

// POSIX ��termios �ṹ��
#define NCCS 17			// termios �ṹ�п����ַ�����ĳ��ȡ�
struct termios
{
  unsigned long c_iflag;	/* input mode flags */// ����ģʽ��־��
  unsigned long c_oflag;	/* output mode flags */// ���ģʽ��־��
  unsigned long c_cflag;	/* control mode flags */// ����ģʽ��־��
  unsigned long c_lflag;	/* local mode flags */// ����ģʽ��־��
  unsigned char c_line;		/* line discipline */// ��·��̣����ʣ���
  unsigned char c_cc[NCCS];	/* control characters */// �����ַ����顣
};

/* c_cc characters *//* c_cc �����е��ַ� */
// ������c_cc �����Ӧ�ַ�������ֵ��
#define VINTR 0			// c_cc[VINTR] = INTR (^C)��\003���ж��ַ���
#define VQUIT 1			// c_cc[VQUIT] = QUIT (^\)��\034���˳��ַ���
#define VERASE 2		// c_cc[VERASE] = ERASE (^H)��\177�������ַ���
#define VKILL 3			// c_cc[VKILL] = KILL (^U)��\025����ֹ�ַ���
#define VEOF 4			// c_cc[VEOF] = EOF (^D)��\004���ļ������ַ���
#define VTIME 5			// c_cc[VTIME] = TIME (\0)��\0�� ��ʱ��ֵ(�μ�����˵��)��
#define VMIN 6			// c_cc[VMIN] = MIN (\1)��\1�� ��ʱ��ֵ��
#define VSWTC 7			// c_cc[VSWTC] = SWTC (\0)��\0�� �����ַ���
#define VSTART 8		// c_cc[VSTART] = START (^Q)��\021����ʼ�ַ���
#define VSTOP 9			// c_cc[VSTOP] = STOP (^S)��\023��ֹͣ�ַ���
#define VSUSP 10		// c_cc[VSUSP] = SUSP (^Z)��\032�������ַ���
#define VEOL 11			// c_cc[VEOL] = EOL (\0)��\0�� �н����ַ���
#define VREPRINT 12		// c_cc[VREPRINT] = REPRINT (^R)��\022������ʾ�ַ���
#define VDISCARD 13		// c_cc[VDISCARD] = DISCARD (^O)��\017�������ַ���
#define VWERASE 14		// c_cc[VWERASE] = WERASE (^W)��\027�����ʲ����ַ���
#define VLNEXT 15		// c_cc[VLNEXT] = LNEXT (^V)��\026����һ���ַ���
#define VEOL2 16		// c_cc[VEOL2] = EOL2 (\0)��\0�� �н���2��

/* c_iflag bits *//* c_iflag ����λ */
// termios �ṹ����ģʽ�ֶ�c_iflag ���ֱ�־�ķ��ų�����
#define IGNBRK 0000001		// ����ʱ����BREAK ������
#define BRKINT 0000002		// ��BREAK ʱ����SIGINT �źš�
#define IGNPAR 0000004		// ������żУ�������ַ���
#define PARMRK 0000010		// �����żУ���
#define INPCK 0000020		// ����������żУ�顣
#define ISTRIP 0000040		// �����ַ���8 λ��
#define INLCR 0000100		// ����ʱ�����з�NL ӳ��ɻس���CR��
#define IGNCR 0000200		// ���Իس���CR��
#define ICRNL 0000400		// ������ʱ���س���CR ӳ��ɻ��з�NL��
#define IUCLC 0001000		// ������ʱ����д�ַ�ת����Сд�ַ���
#define IXON 0002000		// ����ʼ/ֹͣ��XON/XOFF��������ơ�
#define IXANY 0004000		// �����κ��ַ����������
#define IXOFF 0010000		// ����ʼ/ֹͣ��XON/XOFF��������ơ�
#define IMAXBEL 0020000		// ���������ʱ���塣

/* c_oflag bits *//* c_oflag ����λ */
// termios �ṹ�����ģʽ�ֶ�c_oflag ���ֱ�־�ķ��ų�����
#define OPOST 0000001		// ִ���������
#define OLCUC 0000002		// �����ʱ��Сд�ַ�ת���ɴ�д�ַ���
#define ONLCR 0000004		// �����ʱ�����з�NL ӳ��ɻس�-���з�CR-NL��
#define OCRNL 0000010		// �����ʱ���س���CR ӳ��ɻ��з�NL��
#define ONOCR 0000020		// ��0 �в�����س���CR��
#define ONLRET 0000040		// ���з�NL ִ�лس����Ĺ��ܡ�
#define OFILL 0000100		// �ӳ�ʱʹ������ַ�����ʹ��ʱ���ӳ١�
#define OFDEL 0000200		// ����ַ���ASCII ��DEL�����δ���ã���ʹ��ASCII NULL��
#define NLDLY 0000400		// ѡ�����ӳ١�
#define NL0 0000000		// �����ӳ�����0��
#define NL1 0000400		// �����ӳ�����1��
#define CRDLY 0003000		// ѡ��س��ӳ١�
#define CR0 0000000		// �س��ӳ�����0��
#define CR1 0001000		// �س��ӳ�����1��
#define CR2 0002000		// �س��ӳ�����2��
#define CR3 0003000		// �س��ӳ�����3��
#define TABDLY 0014000		// ѡ��ˮƽ�Ʊ��ӳ١�
#define TAB0 0000000		// ˮƽ�Ʊ��ӳ�����0��
#define TAB1 0004000		// ˮƽ�Ʊ��ӳ�����1��
#define TAB2 0010000		// ˮƽ�Ʊ��ӳ�����2��
#define TAB3 0014000		// ˮƽ�Ʊ��ӳ�����3��
#define XTABS 0014000		// ���Ʊ��TAB ���ɿո񣬸�ֵ��ʾ�ո�����
#define BSDLY 0020000		// ѡ���˸��ӳ١�
#define BS0 0000000		// �˸��ӳ�����0��
#define BS1 0020000		// �˸��ӳ�����1��
#define VTDLY 0040000		// �����Ʊ��ӳ١�
#define VT0 0000000		// �����Ʊ��ӳ�����0��
#define VT1 0040000		// �����Ʊ��ӳ�����1��
#define FFDLY 0040000		// ѡ��ҳ�ӳ١�
#define FF0 0000000		// ��ҳ�ӳ�����0��
#define FF1 0040000		// ��ҳ�ӳ�����1��

/* c_cflag bit meaning *//* c_cflag ����λ�ĺ��� */
// termios �ṹ�п���ģʽ��־�ֶ�c_cflag ��־�ķ��ų�����8 ����������
#define CBAUD 0000017		// ��������λ�����롣
#define B0 0000000 /* hang up */	/* �Ҷ���· */
#define B50 0000001		// ������ 50��
#define B75 0000002		// ������ 75��
#define B110 0000003		// ������ 110��
#define B134 0000004		// ������ 134��
#define B150 0000005		// ������ 150��
#define B200 0000006		// ������ 200��
#define B300 0000007		// ������ 300��
#define B600 0000010		// ������ 600��
#define B1200 0000011		// ������ 1200��
#define B1800 0000012		// ������ 1800��
#define B2400 0000013		// ������ 2400��
#define B4800 0000014		// ������ 4800��
#define B9600 0000015		// ������ 9600��
#define B19200 0000016		// ������ 19200��
#define B38400 0000017		// ������ 38400��
#define EXTA B19200		// ��չ������A��
#define EXTB B38400		// ��չ������B��
#define CSIZE 0000060		// �ַ�λ��������롣
#define CS5 0000000		// ÿ�ַ�5 ����λ��
#define CS6 0000020		// ÿ�ַ�6 ����λ��
#define CS7 0000040		// ÿ�ַ�7 ����λ��
#define CS8 0000060		// ÿ�ַ�8 ����λ��
#define CSTOPB 0000100		// ��������ֹͣλ��������1 ����
#define CREAD 0000200		// ������ա�
#define CPARENB 0000400		// �������ʱ������żλ������ʱ������żУ�顣
#define CPARODD 0001000		// ����/����У������У�顣
#define HUPCL 0002000		// �����̹رպ�Ҷϡ�
#define CLOCAL 0004000		// ���Ե��ƽ����(modem)������·��
#define CIBAUD 03600000 /* input baud rate (not used) */	/* ���벨����(δʹ��) */
#define CRTSCTS 020000000000 /* flow control */	/* ������ */

#define PARENB CPARENB		// �������ʱ������żλ������ʱ������żУ�顣
#define PARODD CPARODD		// ����/����У������У�顣

/* c_lflag bits *//* c_lflag ����λ */
// termios �ṹ�б���ģʽ��־�ֶ�c_lflag �ķ��ų�����
#define ISIG 0000001		// ���յ��ַ�INTR��QUIT��SUSP ��DSUSP��������Ӧ���źš�
#define ICANON 0000002		// �����淶ģʽ����ģʽ����
#define XCASE 0000004		// ��������ICANON�����ն��Ǵ�д�ַ��ġ�
#define ECHO 0000010		// ���������ַ���
#define ECHOE 0000020		// ��������ICANON����ERASE/WERASE ������ǰһ�ַ�/���ʡ�
#define ECHOK 0000040		// ��������ICANON����KILL �ַ���������ǰ�С�
#define ECHONL 0000100		// ��������ICANON����ʹECHO û�п���Ҳ����NL �ַ���
#define NOFLSH 0000200		// ������SIGINT ��SIGQUIT �ź�ʱ��ˢ������������У���
// ����SIGSUSP �ź�ʱ��ˢ��������С�
#define TOSTOP 0000400		// ����SIGTTOU �źŵ���̨���̵Ľ����飬�ú�̨������ͼд
// �Լ��Ŀ����նˡ�
#define ECHOCTL 0001000		// ��������ECHO�����TAB��NL��START ��STOP �����ASCII
// �����źŽ������Գ���^X ʽ����X ֵ�ǿ��Ʒ�+0x40��
#define ECHOPRT 0002000		// ��������ICANON ��IECHO�����ַ��ڲ���ʱ����ʾ��
#define ECHOKE 0004000		// ��������ICANON����KILL ͨ���������ϵ������ַ������ԡ�
#define FLUSHO 0010000		// �����ˢ�¡�ͨ������DISCARD �ַ����ñ�־����ת��
#define PENDIN 0040000		// ����һ���ַ��Ƕ�ʱ����������е������ַ��������ԡ�
#define IEXTEN 0100000		// ����ʵ��ʱ��������봦��

/* modem lines *//* modem ��·�źŷ��ų��� */
#define TIOCM_LE 0x001		// ��·����(Line Enable)��
#define TIOCM_DTR 0x002		// �����ն˾���(Data Terminal Ready)��
#define TIOCM_RTS 0x004		// ������(Request to Send)��
#define TIOCM_ST 0x008		// �������ݷ���(Serial Transfer)��[??]
#define TIOCM_SR 0x010		// �������ݽ���(Serial Receive)��[??]
#define TIOCM_CTS 0x020		// �������(Clear To Send)��
#define TIOCM_CAR 0x040		// �ز����(Carrier Detect)��
#define TIOCM_RNG 0x080		// ����ָʾ(Ring indicate)��
#define TIOCM_DSR 0x100		// �����豸����(Data Set Ready)��
#define TIOCM_CD TIOCM_CAR
#define TIOCM_RI TIOCM_RNG

/* tcflow() and TCXONC use these *//* tcflow()��TCXONC ʹ����Щ���ų��� */
#define TCOOFF 0		// ���������
#define TCOON 1			// ����������������
#define TCIOFF 2		// ϵͳ����һ��STOP �ַ���ʹ�豸ֹͣ��ϵͳ�������ݡ�
#define TCION 3			// ϵͳ����һ��START �ַ���ʹ�豸��ʼ��ϵͳ�������ݡ�

/* tcflush() and TCFLSH use these *//* tcflush()��TCFLSH ʹ����Щ���ų��� */
#define TCIFLUSH 0		// ����յ������ݵ�������
#define TCOFLUSH 1		// ����д�����ݵ������͡�
#define TCIOFLUSH 2		// ����յ������ݵ�����������д�����ݵ������͡�

/* tcsetattr uses these *//* tcsetattr()ʹ����Щ���ų��� */
#define TCSANOW 0		// �ı�����������
#define TCSADRAIN 1		// �ı���������д�����������֮������
#define TCSAFLUSH 2		// �ı���������д�����������֮���������н��յ���
// ��û�ж�ȡ�����ݱ�����֮������

typedef int speed_t;		// ��������ֵ���͡�

// ����termios_p ��ָtermios �ṹ�еĽ��ղ����ʡ�
extern speed_t cfgetispeed (struct termios *termios_p);
// ����termios_p ��ָtermios �ṹ�еķ��Ͳ����ʡ�
extern speed_t cfgetospeed (struct termios *termios_p);
// ��termios_p ��ָtermios �ṹ�еĽ��ղ���������Ϊspeed��
extern int cfsetispeed (struct termios *termios_p, speed_t speed);
// ��termios_p ��ָtermios �ṹ�еķ��Ͳ���������Ϊspeed��
extern int cfsetospeed (struct termios *termios_p, speed_t speed);
// �ȴ�fildes ��ָ������д������ݱ����ͳ�ȥ��
extern int tcdrain (int fildes);
// ����/����fildes ��ָ�������ݵĽ��պͷ��͡�
extern int tcflow (int fildes, int action);
// ����fildes ָ������������д����û�����Լ��������յ�����û�ж�ȡ�����ݡ�
extern int tcflush (int fildes, int queue_selector);
// ��ȡ����fildes ��Ӧ����Ĳ����������䱣����termios_p ��ָ�ĵط���
extern int tcgetattr (int fildes, struct termios *termios_p);
// ����ն�ʹ���첽�������ݴ��䣬����һ��ʱ������������һϵ��0 ֵ����λ��
extern int tcsendbreak (int fildes, int duration);
// ʹ��termios �ṹָ��termios_p ��ָ�����ݣ��������ն���صĲ�����
extern int tcsetattr (int fildes, int optional_actions,
		      struct termios *termios_p);

#endif
