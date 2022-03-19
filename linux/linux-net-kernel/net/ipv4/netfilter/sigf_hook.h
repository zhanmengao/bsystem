#ifndef	__SIGF_HOOK_H__
#define	__SIGF_HOOK_H__


/*
	֧����Щģ��
*/
#define	SIGF_MOD_IP		0
#define	SIGF_MOD_TCP		1
#define	SIGF_MOD_UDP		2
#define	SIGF_MOD_GIOP		3
#define	SIGF_MOD_SHPKT	4
#define	SIGF_MOD_MAX		5

/*
	�ص������ķ���ֵ�ĵ�8λ
*/
#define SIGF_DROP		0	/* ֹͣ���ã���ת���ñ��� */
#define SIGF_FORWARD	1	/* ֹͣ���ã�ת���ñ��� */
#define SIGF_CONTINUE	2	/* ����������һģ��Ĵ����� */

/*
	�ص������ķ���ֵ��һЩ��־���ɸߵ��Ͷ���
*/
//#define	SIGF_DONT_FREE	0x80000000	/* ��Ҫ�ͷ�mbuf */

/*
	����ȡ�ص������ķ���ֵ�ĵ�8λ
*/
#define	SIGF_GET_RS(_X)	((_X) & 0xFF)
#define	SIGF_IS_RS(_X, _V) (SIGF_GET_RS(_X) == (_V))

/*
	���ڻص������䴫�ݲ��������ݽṹ
*/
typedef struct sigf_info_s {
	/* ��ַ���˿ںŶ��������ֽ��� */
	u32	src_addr;
	u32	dst_addr;
	u8	ip_hdr_len;
	u8	proto;
	u16	reserv;
	u16	src_port;
	u16	dst_port;
	/* IP���ݱ����ܳ��� */
	u16	ip_total_len;
	/* UDP/TCP�û����ݵĿ�ʼƫ�� */
	u16	user_data_ofs;
	/* TCP/UDP�û����ݳ��� */
	u16	user_data_len;
	struct sk_buff	*skb;
	void	*ext;
} sigf_info_t;

#if 0
/* 
	��ĳģ��ע���Լ��Ļص�����
	callback: int sigf_hook_func(sigf_info_t *si);
*/
STATUS sigf_hook_reg(int mod, FUNCPTR callback, int pri);
/* 
	��ʼ����ע����ĳģ���ϵĻص�����
*/
extern int sigf_hook(int mod, sigf_info_t *si);

extern STATUS sigf_hook_init_phase1(void);
extern STATUS sigf_hook_init_phase2(void);
#endif

#endif


