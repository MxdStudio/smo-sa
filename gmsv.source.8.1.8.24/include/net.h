#ifndef __NET_H__
#define __NET_H__

#include "common.h"

#include <sys/time.h>
#include <netinet/in.h>
// Nuke +1 0902: For queuing control
#include <signal.h>

#if USE_MTIO
#include <pthread.h>
#endif

#include "link.h"


#undef EXTERN
#ifdef __NET_C__
#define EXTERN 
#else
#define EXTERN extern
#endif



/*
 * �ҥ����󤷤Ƥ��ʤ��b �ҥ����󤷤Ƥ���b �ҥ�����   �������  
 * �ҥ�������  ( ������   ) , �ҥ�������  ( ����ҥå�   )
 *  N �� NOT �b W �� WHILE , UL �� UNLOCK �λ�
 */
typedef enum
{
    NOTLOGIN,           /*  �ҥ����󤷤Ƥ��ʤ�  */
    LOGIN,              /*  �ҥ�����            */
    WHILELOGIN,         /*  �ҥ����󤷤Ƥ���    */
    WHILECREATE,        /*  ��äƤ���          */
    WHILELOGOUTSAVE,    /*  �ҥ������ȤΥ�����      */
    WHILECANNOTLOGIN,   /*  �ҥ�����Ǥ��ʤ��ä����Υ���ҥå���        */
    WHILECHARDELETE,    /*  �������      */
    WHILEDOWNLOADCHARLIST,  /*  �����ꥹ�ȥ�����ҡ���    */
    WHILECHANGEPASSWD,  /*  �ѥ����  ��      */

    WHILELOSTCHARSAVE,  /*  �ҥ��Ȥǥ���饻���֤�      */
    WHILELOSTCHARDELETE,/*  �ҥ��Ȥǥ����ä�      */

    WHILECLOSEALLSOCKETSSAVE, /* closeallsockets �ǥ���饻���֤�  */
    WHILESAVEWAIT,              /* �ҥ������ȤΥ����֤˹Ԥ����ν�  �Ԥ�  */
}LoginType;

/*     ���Ƥ��ʤ��b��������ȥ����Сb���饤����ȡb���ɥ� */
typedef enum
{
    NOTDETECTED,AC,CLI,ADM
}ConnectType;


/*  �����Ф���¸���ƶ���Ĺ��    */
#define CDKEYLEN    16
#define PASSWDLEN   16
/* �����  �����Բ�(����ʤ� Char ��STRING64 ��Ŭ�ڤ��ͤ��ڤ괹�뤳��*/
#define CHARNAMELEN     32

#define CLITIMEOUT_SEC  120     /* ���饤����Ȥ���read���ʤ��ä���b
                                   ���λ��֤ǥ����ॢ���ȡb�ҥ������ȡe */

#define		NET_STRING_SUCCESSFULL	"successful"
#define		NET_STRING_FAILED		"failed"

// �դĤ���    �Υ꡼�ɡf�饤�ȥХåե������N
#define RBSIZE 65536
#define WBSIZE (65536*6)
// ��������ȥ����С���

#define	AC_RBSIZE (65536*48)
//#define	AC_RBSIZE (65536*32)
//ttom modify because the second version had this
//#define	AC_WBSIZE (65536*16)
extern int AC_WBSIZE;

EXTERN int      bindedfd;     /*�ҡ����륢�ɥ쥹�˥Х���ɤ��������å�*/
EXTERN int      acfd;         /*��������ȥ����Ф˥��ͥ��ȼ��������å�*/
EXTERN int      ConnectLen;   /*�����ͤ�Ĺ��*/
#define CONNECT_WINDOWBUFSIZE 7

#ifdef _M_SERVER
EXTERN int		mfd;
#endif

#ifdef _NPCSERVER_NEW
EXTERN int		npcfd;
#endif

/* ��  ���� */
BOOL initConnect( int size );
void endConnect( void );
#define	CONNECT_endOne( sockfd, lin) _CONNECT_endOne( __FILE__, __LINE__, sockfd, lin)
#define	CONNECT_setCharaindex( fd, a) _CONNECT_setCharaindex( fd, a ,__FILE__,__LINE__)
#define CONNECT_getCharaindex(fd) _CONNECT_getCharaindex(fd,__FILE__,__LINE__)

BOOL _CONNECT_endOne( char *file, int fromline, int sockfd , int lin);
BOOL netloop( void );
BOOL netloop_faster( void );

int lsrpcClientWriteFunc( int fd , char* buf , int size );
char* GetOneLine( int fd );
BOOL initConnectOne( int sockfd, struct sockaddr_in* sin ,int len );
#if USE_MTIO
BOOL MTIO_appendWB( int conindex, char *src, int length );
#endif

/* CA , CD */
void CAcheck( void );
void CAflush( int charaindex );
void CAsend( int fd );
void CDcheck( void );
void CDflush( int charaindex );
void CDsend( int fd );
BOOL CONNECT_appendCAbuf( int fd , char* data, int size );
BOOL CONNECT_appendCDbuf( int fd , char* data, int size );

/* Serverstate */
void SERVSTATE_decrementCloseallsocketnum(void);
int SERVSTATE_getCloseallsocketnum( void );
int SERVSTATE_SetAcceptMore( int nvalue );
int SERVSTATE_getShutdown(void);
void SERVSTATE_setShutdown(int a);
int SERVSTATE_getLimittime(void);
int SERVSTATE_getDsptime(void);
void SERVSTATE_setDsptime(int a);
void SERVSTATE_setLimittime(int a);


/* ���Τۤ� */
void outputNetProcLog( int fd, int mode);
void chardatasavecheck( void );
void closeAllConnectionandSaveData( void );
BOOL SetShutdown( BOOL nvalue );
int GetShutdown( void );

/* �롼�פ��Ƹ����ؿ� */
int getfdFromCdkey( char* cd );
int getfdFromCharaIndex( int charaindex );
int getcdkeyFromCharaIndex( int charaindex , char *out, int outlen );
int getCharindexFromFdid( int fdid );
int getFdidFromCharaIndex( int charind );
int getfdFromFdid( int fdid );
int getfdFromCdkeyWithLogin( char* cd );


/* ����� */        
INLINE int CONNECT_checkfd( int fd );
BOOL CONNECT_isCLI( int fd );
BOOL CONNECT_isAC( int fd );
BOOL CONNECT_isUnderLogin( int fd );
BOOL CONNECT_isWhileLogout( int fd );
BOOL CONNECT_isWhileLogin( int fd );
BOOL CONNECT_isNOTLOGIN( int fd );
BOOL CONNECT_isLOGIN( int fd );

int CONNECT_getUse( int fd );
int CONNECT_getUse_debug( int fd, int i );

//void CONNECT_setCharaindex( int fd, int a );
void _CONNECT_setCharaindex( int fd, int a ,char *file,int line);
int _CONNECT_getCharaindex( int fd ,char *file,int line);
void CONNECT_getCdkey( int fd , char *out, int outlen );
void CONNECT_setCdkey( int fd , char *in);
void CONNECT_getPasswd( int fd , char *out, int outlen );
void CONNECT_setPasswd( int fd, char *in );
void CONNECT_getCharname( int fd , char *out, int outlen );
void CONNECT_setCharname( int fd, char *in );
int CONNECT_getFdid( int fd );
int CONNECT_getCtype( int fd );
void CONNECT_setCtype( int fd , int a );
void CONNECT_setDuelcharaindex( int fd, int i , int a );
int CONNECT_getDuelcharaindex( int fd, int i );
void CONNECT_setBattlecharaindex( int fd, int i , int a );
int CONNECT_getBattlecharaindex( int fd, int i );
void CONNECT_setJoinpartycharaindex( int fd, int i , int a);
int CONNECT_getJoinpartycharaindex( int fd, int i );
void CONNECT_setTradecardcharaindex( int fd, int i , int a );
int CONNECT_getTradecardcharaindex( int fd, int i );
int CONNECT_getClosed( int fd );
void CONNECT_setClosed( int fd, int a );

/* MT�ǥХå��ѥޥ��� */
#define CONNECT_endOne_debug(a) CONNECT_endOne( (a) , __LINE__ )

/* �ǡ�������ؿ�(�ޤ������ä��б���) */
void CONNECT_setCDKEY( int sockfd, char *cd );
void CONNECT_getCDKEY( int sockfd , char *out, int outlen );
void CONNECT_setState( int fd, int s );
int CONNECT_getState( int fd );
void CONNECT_checkStatecount( int a );
int CONNECT_checkStateSomeOne( int a, int maxcount);

#if USE_MTIO
int MTIO_setup( void );
void MTIO_join( void );
#endif   /* USE_MTIO */

BOOL CONNECT_acfdInitRB( int fd );
BOOL CONNECT_acfdInitWB( int fd );

// Nuke +3
int checkWalkTime(int fd);
int setBtime(int fd);

#ifdef _BATTLE_TIMESPEED
//void setDefBTime( int fd, unsigned int times);
//unsigned int getDefBTime( int fd);
BOOL CheckDefBTime( int charaindex, int fd, unsigned int lowTime, unsigned int battletime, unsigned int addTime);
#endif

#ifdef _CHECK_GAMESPEED
int getGameSpeedTime( int fd);
void setGameSpeedTime( int fd, int times);
#endif

#ifdef _TYPE_TOXICATION
void setToxication( int fd, int flg);
int getToxication( int fd);
#endif
int checkBEOTime(int fd);
void sigusr1(int i);
void sigusr2(int i);	// Arminius 6.26 signal
//ttom start
void CONNECT_set_watchmode(int fd, BOOL B_Watch);
BOOL CONNECT_get_watchmode(int fd);
void CONNECT_set_shutup(int fd,BOOL b_shut);//the avoid the user wash the screen
BOOL CONNECT_get_shutup(int fd);
unsigned long CONNECT_get_userip(int fd);
void CONNECT_set_pass(int fd,BOOL b_ps);
BOOL CONNECT_get_pass(int fd);
void CONNECT_set_state_trans(int fd,int a);
int CONNECT_get_state_trans(int fd);
//ttom end

// CoolFish: +9 2001/4/18
void CONNECT_setCloseRequest(int fd, int count);
void CONNECT_set_first_warp(int fd, BOOL b_ps);
BOOL CONNECT_get_first_warp(int fd);
int isDie(int fd);
void setDie(int fd);

// Arminius 6/22 encounter
int CONNECT_get_CEP(int fd);
void CONNECT_set_CEP(int fd, int cep);

// Arminius 7.12 login announce
int CONNECT_get_announced(int fd);
void CONNECT_set_announced(int fd, int a);

// shan trade(DoubleCheck) begin
int  CONNECT_get_confirm(int fd);
void CONNECT_set_confirm(int fd, BOOL b);
// end

#ifdef _BLACK_MARKET
int CONNECT_get_BMList(int fd, int i);
void CONNECT_set_BMList(int fd,int i, int b);
#endif

#ifdef _NO_WARP
// shan hjj add Begin
int  CONNECT_get_seqno(int fd);
void CONNECT_set_seqno(int fd,int a);
int  CONNECT_get_selectbutton(int fd);
void CONNECT_set_selectbutton(int fd, int a);
// shan End
#endif

void CONNECT_setTradecharaindex( int fd, int i , int a);

void CONNECT_setLastrecvtime( int fd, struct timeval *a );
void CONNECT_getLastrecvtime( int fd, struct timeval *a );
void CONNECT_setLastrecvtime_D( int fd, struct timeval *a );
void CONNECT_getLastrecvtime_D( int fd, struct timeval *a );
void CONNECT_SetBattleRecvTime( int fd, struct timeval *a );
void CONNECT_GetBattleRecvTime( int fd, struct timeval *a );
void CONNECT_setTradeTmp(int fd, char* a);
void CONNECT_getTradeTmp(int fd, char *trademsg, int trademsglen);
int checkNu(int fd);

// Arminius 7.2 Ra's amulet
void setNoenemy(int fd);
void clearNoenemy(int fd);
int getNoenemy(int fd);
void setEqNoenemy(int fd, int level);
void clearEqNoenemy(int fd);
int getEqNoenemy(int fd);

#ifdef _Item_MoonAct
void setEqRandenemy(int fd, int level);
void clearEqRandenemy(int fd);
int getEqRandenemy(int fd);
#endif

#ifdef _CHIKULA_STONE
void setChiStone(int fd, int nums);
int getChiStone(int fd);
#endif

// Arminius 7.31 cursed stone
void setStayEncount(int fd);
void clearStayEncount(int fd);
int getStayEncount(int fd);
void CONNECT_setBDTime( int fd, int nums);
int CONNECT_getBDTime( int fd);

// Arminius debug
void CONNECT_setUse( int fd , int a);

#ifdef _ITEM_PILEFORTRADE
void CONNECT_setTradeList( int fd, int num);
int CONNECT_getTradeList(int fd);
#endif

#define QUEUE_LENGTH1 6
#define QUEUE_LENGTH2 7

BOOL MSBUF_CHECKbuflen( int size, float defp);


#ifdef _GM_BROADCAST					// WON ADD �ȪA���i�t��


typedef struct tag_broadcast_struct
{
	int time;			// �T�����j�ɶ�
	int loop;			// ���榸��	
	int wait;			// ���i���𮧮ɶ�
	int next_msg;
	int max_msg_line;	// �T����
	char msg[10][128];	// ���i�T��
}broadcast_struct;

broadcast_struct BS;

void Init_GM_BROADCAST( int loop, int time, int wait, char *msg );
void GM_BROADCAST();
#endif

void IsSucceed(int fd,char *result);

#ifdef _DEATH_FAMILY_STRUCT		// WON ADD �a�ھԦs��ӭt���
void Init_FM_PK_STRUCT();
#endif

#ifdef _JZ_CIRC_AN
BOOL net_loadloopan(char *path);
#endif
#ifdef _JZ_LOCKIP
BOOL net_showip(int fd,char *sc);
BOOL net_Islockip(struct sockaddr_in* sin);
#endif

#ifdef _JZ_EPOLL
//�Ѥ@�Ӧr�Ŧ�IP�ഫ��LONG�ƭ�
BOOL CharIpToLongIp(char *desc);
//��l��EPOLL
BOOL init_epoll(int evslsnum,int maxplayer);
BOOL Delepollevents(int fd);
#endif

#endif 
