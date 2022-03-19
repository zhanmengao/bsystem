#ifndef KEYBOARD_H
#define KEYBOARD_H
/*
* Thanks to Alfred Leung for US keyboard patches
* Wolfgang Thiel for German keyboard patches
* Marc Corsini for the French keyboard
*/
/*
* ��лAlfred Leung �����US ���̲�������
* Wolfgang Thiel ����˵�����̲�������
* Marc Corsini ����˷��ļ��̲�������
*/

// ɨ����-ASCII �ַ�ӳ���
// ������config.h �ж���ļ�������(FINNISH��US��GERMEN��FRANCH)������Ӧ����ɨ����ӳ��
// ��ASCII �ַ���
#if defined(KBD_FINNISH)
// �����Ƿ�������̵�ɨ����ӳ���
unsigned char key_map[] = {
 0,27, // ɨ����0x00,0x01 ��Ӧ��ASCII �룻
 '1','2','3','4','5','6','7','8','9','0','+', // ɨ����0x02,...0x0c,0x0d ��Ӧ��ASCII �룬�������ơ�
 127,9,
 'q','w','e','r','t','y','u','i','o','p','}',
 0,13,0,
 'a','s','d','f','g','h','j','k','l','|','{',
 0,0,
 '\'','z','x','c','v','b','n','m',',','.','-',
 0,'*',0,32, /* 36-39 */ /* ɨ����0x36-0x39 ��Ӧ��ASCII �� */
 0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0, /* 3A-49 */ /* ɨ����0x3A-0x49 ��Ӧ��ASCII �� */
 '-',0,0,0,'+', /* 4A-4E */ /* ɨ����0x4A-0x4E ��Ӧ��ASCII �� */
 0,0,0,0,  0,0,0, /* 4F-55 */ /* ɨ����0x4F-0x55 ��Ӧ��ASCII �� */
 '<',
 0,0,0,0,  0,0,0,0,  0,0
};
// shift ��ͬʱ����ʱ��ӳ���
unsigned char shift_map[] = {
 0,27,
 '!','\"','#','$','%','&','/','(',')','=','?','`',
 127,9,
 'Q','W','E','R','T','Y','U','I','O','P',']','^',
 13,0,
 'A','S','D','F','G','H','J','K','L','\\','[',
 0,0,
 '*','Z','X','C','V','B','N','M',':','_',
 0,'*',0,32, /* 36-39 */
 0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0, /* 3A-49 */ /* ɨ����0x3A-0x49 ��Ӧ��ASCII �� */
 '-',0,0,0,'+', /* 4A-4E */
 0,0,0,0,0,0,0, /* 4F-55 */
 '>',
 0,0,0,0,  0,0,0,0,  0,0
};
// alt ��ͬʱ����ʱ��ӳ���
unsigned char alt_map[] = {
 0,0,
 '\0','@','\0','$','\0','\0','{','[',']','}','\\','\0',
 0,0,
 0,0,0,0,0,0,0,0,0,0,0,
 '~',13,0,
 0,0,0,0,0,0,0,0,0,0,0,
 0,0,
 0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0, /* 36-39 */
 0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0, /* 3A-49 */ /* ɨ����0x3A-0x49 ��Ӧ��ASCII �� */
 0,0,0,0,0, /* 4A-4E */
 0,0,0,0,0,0,0, /* 4F-55 */
 '|',
 0,0,0,0,  0,0,0,0,  0,0
};

#elif defined(KBD_US)

// ��������ʽ���̵�ɨ����ӳ���
unsigned char key_map[] = {
 0,27,
 '1','2','3','4','5','6','7','8','9','0','-','=',
 127,9,
 'q','w','e','r','t','y','u','i','o','p','[',']',
 13,0,
 'a','s','d','f','g','h','j','k','l','\'',
 '`',0,
 '\\','z','x','c','v','b','n','m',',','.','/',
 0,'*',0,32, /* 36-39 */
 0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0, /* 3A-49 */ /* ɨ����0x3A-0x49 ��Ӧ��ASCII �� */
 '-',0,0,0,'+', /* 4A-4E */
 0,0,0,0,0,0,0,/* 4F-55 */
 '<',
 0,0,0,0,  0,0,0,0,  0,0
};

unsigned char shift_map[] = {
 0,27,
 '!','@','#','$','%','^','&','*','(',')','_','+',
 127,9,
 'Q','W','E','R','T','Y','U','I','O','P','{','}',
 13,0,
 'A','S','D','F','G','H','J','K','L',':','\"',
 '~',0,
 '|','Z','X','C','V','B','N','M','<','>','?',
 0,'*',0,32, /* 36-39 */
 0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0, /* 3A-49 */ /* ɨ����0x3A-0x49 ��Ӧ��ASCII �� */
 '-',0,0,0,'+', /* 4A-4E */
 0,0,0,0,0,0,0, /* 4F-55 */
 '>',
 0,0,0,0,  0,0,0,0,  0,0
};

unsigned char alt_map[] = {
 0,0,
 '\0','@','\0','$','\0','\0','{','[',']','}','\\','\0',
 0,0,
 0,0,0,0,0,0,0,0,0,0,0,
 '~',13,0,
 0,0,0,0,0,0,0,0,0,0,0,
 0,0,
 0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0, /* 36-39 */
 0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0, /* 3A-49 */ /* ɨ����0x3A-0x49 ��Ӧ��ASCII �� */
 0,0,0,0,0, /* 4A-4E */
 0,0,0,0,0,0,0, /* 4F-55 */
 '|',
 0,0,0,0,  0,0,0,0,  0,0
};

#elif defined(KBD_GR)

// �����ǵ�����̵�ɨ����ӳ���
unsigned char key_map[] = {
 0,27,
 '1','2','3','4','5','6','7','8','9','0','\\','\'',
 127,9,
 'q','w','e','r','t','z','u','i','o','p','@','+',
 13,0
 'a','s','d','f','g','h','j','k','l','[',']','^',
 0,'#',
 'y','x','c','v','b','n','m',',','.','-',
 0,'*',0,32, /* 36-39 */
 0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0, /* 3A-49 */ /* ɨ����0x3A-0x49 ��Ӧ��ASCII �� */
 '-',0,0,0,'+', /* 4A-4E */
 0,0,0,0,0,0,0, /* 4F-55 */
 '<',
 0,0,0,0,  0,0,0,0,  0,0
};

unsigned char shift_map[] = {
 0,27,
 '!','\"','#','$','%','&','/','(',')','=','?','`',
 127,9,
 'Q','W','E','R','T','Z','U','I','O','P','\\','*',
 13,0,
 'A','S','D','F','G','H','J','K','L','{','}','~',
 0,'',
 'Y','X','C','V','B','N','M',':','_',
 0,'*',0,32, /* 36-39 */
 0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0, /* 3A-49 */ /* ɨ����0x3A-0x49 ��Ӧ��ASCII �� */
 '-',0,0,0,'+', /* 4A-4E */
 0,0,0,0,0,0,0, /* 4F-55 */
 '>',
 0,0,0,0,  0,0,0,0,  0,0
};

unsigned char alt_map[] = {
 0,0,
 '\0','@','\0','$','\0','\0','{','[',']','}','\\','\0',
 0,0,
 '@',0,0,0,0,0,0,0,0,0,0,
 '~',13,0,
 0,0,0,0,0,0,0,0,0,0,0,
 0,0,
 0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0, /* 36-39 */
 0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0, /* 3A-49 */ /* ɨ����0x3A-0x49 ��Ӧ��ASCII �� */
 0,0,0,0,0, /* 4A-4E */
 0,0,0,0,0,0,0, /* 4F-55 */
 '|',
 0,0,0,0,  0,0,0,0,  0,0
};

#elif defined(KBD_FR)

// �����Ƿ�����̵�ɨ����ӳ���
unsigned char key_map[] = {
 0,27,
 '&','{','\"','\'','(','-','}','_','/','@',')','=',
 127,9,
 'a','z','e','r','t','y','u','i','o','p','^','$',
 13,0,
 'q','s','d','f','g','h','j','k','l','m','|',
 '`',0,42, /* coin sup gauche, don't know, [*|mu] */
 'w','x','c','v','b','n',',',':','!',
 0,'*',0,32, /* 36-39 */
 0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0, /* 3A-49 */ /* ɨ����0x3A-0x49 ��Ӧ��ASCII �� */
 '-',0,0,0,'+', /* 4A-4E */
 0,0,0,0,0,0,0, /* 4F-55 */
 '<',
 0,0,0,0,  0,0,0,0,  0,0
};

unsigned char shift_map[] = {
 0,27,
 '1','2','3','4','5','6','7','8','9','0',']','+',
 127,9,
 'A','Z','E','R','T','Y','U','I','O','P','<','>',
 13,0,
 'Q','S','D','F','G','H','J','K','L','M','%',
 '~',0,'#',
 'W','X','C','V','B','N','?','.','/','\\',
 0,'*',0,32, /* 36-39 */
 0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0, /* 3A-49 */ /* ɨ����0x3A-0x49 ��Ӧ��ASCII �� */
 '-',0,0,0,'+', /* 4A-4E */
 0,0,0,0,0,0,0, /* 4F-55 */
 '>',
 0,0,0,0,  0,0,0,0,  0,0
};

unsigned char alt_map[] = {
 0,0,
 '\0','~','#','{','[','|','`','\\','^','@',']','}',
 0,0,
 '@',0,0,0,0,0,0,0,0,0,0
 '~',13,0,
 0,0,0,0,0,0,0,0,0,0,0
 0,0,
 0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0, /* 36-39 */
 0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0, /* 3A-49 */ /* ɨ����0x3A-0x49 ��Ӧ��ASCII �� */
 0,0,0,0,0, /* 4A-4E */
 0,0,0,0,0,0,0, /* 4F-55 */
 '|',
 0,0,0,0,  0,0,0,0,  0,0
};

#else

#error "KBD-type not defined"

#endif


extern void none();
extern void do_self();
extern void ctrl();
extern void alt();
extern void unctrl();
extern void unalt();
extern void lshift();
extern void unlshift();
extern void rshift();
extern void unrshift();
extern void caps();
extern void uncaps();
extern void scroll();
extern void num();
extern void cursor();
extern void func();
extern void minus();

/*
* This table decides which routine to call when a scan-code has been
* gotten. Most routines just call do_self, or none, depending if
* they are make or break.
*/
/* ������һ���ӳ����ַ��ת����ȡ��ɨ�����͸��ݴ˱������Ӧ��ɨ���봦���ӳ���
* ��������õ��ӳ�����do_self��������none����������ǰ���(make)�����ͷż�(break)��
*/
typedef void (*kf)();
kf key_table[]={
 none,   do_self,do_self,do_self, /* 00-03 s0 esc 1 2 */
 do_self,do_self,do_self,do_self, /* 04-07 3 4 5 6 */
 do_self,do_self,do_self,do_self, /* 08-0B 7 8 9 0 */
 do_self,do_self,do_self,do_self, /* 0C-0F + ' bs tab */
 do_self,do_self,do_self,do_self, /* 10-13 q w e r */
 do_self,do_self,do_self,do_self, /* 14-17 t y u i */
 do_self,do_self,do_self,do_self, /* 18-1B o p } ^ */
 do_self,ctrl,   do_self,do_self, /* 1C-1F enter ctrl a s */
 do_self,do_self,do_self,do_self, /* 20-23 d f g h */
 do_self,do_self,do_self,do_self, /* 24-27 j k l | */
 do_self,do_self,lshift, do_self, /* 28-2B { para lshift , */
 do_self,do_self,do_self,do_self, /* 2C-2F z x c v */
 do_self,do_self,do_self,do_self, /* 30-33 b n m , */
 do_self,minus,  rshift, do_self, /* 34-37 . - rshift * */
 alt,    do_self,caps,   func, /* 38-3B alt sp caps f1 */
 func,   func,   func,   func, /* 3C-3F f2 f3 f4 f5 */
 func,   func,   func,   func, /* 40-43 f6 f7 f8 f9 */
 func,   num,    scroll, cursor, /* 44-47 f10 num scr home */
 cursor, cursor, do_self,cursor, /* 48-4B up pgup - left */
 cursor, cursor, do_self,cursor, /* 4C-4F n5 right + end */
 cursor, cursor, cursor, cursor, /* 50-53 dn pgdn ins del */
 none,   none,   do_self,func, /* 54-57 sysreq ? < f11 */
 func,   none,   none,   none, /* 58-5B f12 ? ? ? */
 none,none,none,none, /* 5C-5F ? ? ? ? */
 none,none,none,none, /* 60-63 ? ? ? ? */
 none,none,none,none, /* 64-67 ? ? ? ? */
 none,none,none,none, /* 68-6B ? ? ? ? */
 none,none,none,none, /* 6C-6F ? ? ? ? */
 none,none,none,none, /* 70-73 ? ? ? ? */
 none,none,none,none, /* 74-77 ? ? ? ? */
 none,none,none,none, /* 78-7B ? ? ? ? */
 none,none,none,none, /* 7C-7F ? ? ? ? */
 none,none,none,none, /* 80-83 ? br br br */
 none,none,none,none, /* 84-87 br br br br */
 none,none,none,none, /* 88-8B br br br br */
 none,none,none,none, /* 8C-8F br br br br */
 none,none,none,none, /* 90-93 br br br br */
 none,none,none,none, /* 94-97 br br br br */
 none,none,none,none, /* 98-9B br br br br */
 none,unctrl,none,none, /* 9C-9F br unctrl br br */
 none,none,none,none, /* A0-A3 br br br br */
 none,none,none,none, /* A4-A7 br br br br */
 none,none,unlshift,none, /* A8-AB br br unlshift br */
 none,none,none,none, /* AC-AF br br br br */
 none,none,none,none, /* B0-B3 br br br br */
 none,none,unrshift,none, /* B4-B7 br br unrshift br */
 unalt,none,uncaps,none, /* B8-BB unalt br uncaps br */
 none,none,none,none, /* BC-BF br br br br */
 none,none,none,none, /* C0-C3 br br br br */
 none,none,none,none, /* C4-C7 br br br br */
 none,none,none,none, /* C8-CB br br br br */
 none,none,none,none, /* CC-CF br br br br */
 none,none,none,none, /* D0-D3 br br br br */
 none,none,none,none, /* D4-D7 br br br br */
 none,none,none,none, /* D8-DB br ? ? ? */
 none,none,none,none, /* DC-DF ? ? ? ? */
 none,none,none,none, /* E0-E3 e0 e1 ? ? */
 none,none,none,none, /* E4-E7 ? ? ? ? */
 none,none,none,none, /* E8-EB ? ? ? ? */
 none,none,none,none, /* EC-EF ? ? ? ? */
 none,none,none,none, /* F0-F3 ? ? ? ? */
 none,none,none,none, /* F4-F7 ? ? ? ? */
 none,none,none,none, /* F8-FB ? ? ? ? */
 none,none,none,none  /* FC-FF ? ? ? ? */
};

#endif
