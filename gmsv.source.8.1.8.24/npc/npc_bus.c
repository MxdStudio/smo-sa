#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "npc_bus.h"
#include "handletime.h"

/* 
 * �Х��ߤ����ʻ��򤹤�NPC
 */
 
enum {
	NPC_WORK_ROUTETOX = CHAR_NPCWORKINT1,		/* �ɤ��ء�  ��   */
	NPC_WORK_ROUTETOY = CHAR_NPCWORKINT2,		/* �ɤ��ء�  ��   */
	NPC_WORK_ROUTEPOINT = CHAR_NPCWORKINT3,		/* ����    �� */
	NPC_WORK_ROUNDTRIP = CHAR_NPCWORKINT4,		/* �Ԥ������꤫  ���u�Ԥ� ���u����  */
	NPC_WORK_MODE = CHAR_NPCWORKINT5,
	NPC_WORK_CURRENTROUTE = CHAR_NPCWORKINT6, 
	NPC_WORK_ROUTEMAX = CHAR_NPCWORKINT7,
	NPC_WORK_WAITTIME = CHAR_NPCWORKINT8,
	NPC_WORK_CURRENTTIME = CHAR_NPCWORKINT9,
	NPC_WORK_SEFLG = CHAR_NPCWORKINT10,
};

/* ��  ��å�������enum */
enum {
	NPC_BUS_MSG_GETTINGON,
	NPC_BUS_MSG_NOTPARTY,
	NPC_BUS_MSG_OVERPARTY,
	NPC_BUS_MSG_DENIEDITEM,
	NPC_BUS_MSG_ALLOWITEM,
	NPC_BUS_MSG_LEVEL,
	NPC_BUS_MSG_GOLD,
	NPC_BUS_MSG_EVENT,
	NPC_BUS_MSG_START,
	NPC_BUS_MSG_END,
};
typedef struct {
	char	option[32];
	char	defaultmsg[128];
}NPC_BUS_MSG;
NPC_BUS_MSG		busmsg[] = {
	{ "msg_gettingon",	"PAON�I�]�A�L�k�󤤳~�[�J�ڭ̭�I�^"},
	{ "msg_notparty",	"PAPAON�I�I�L�k�H�ζ��[�J��I"},
	{ "msg_overparty",	"PAON�I�I�H�Ƥw���C"},
	{ "msg_denieditem",		"PAPAON�I�I�ڥi���n�o�ӹD��I"},
	{ "msg_allowitem",		"�z��~(�Q�n���ӹD���!)"},
	{ "msglevel",		"PAPAON�I�I�A�������٤�����I"},
	{ "msg_stone",		"PAPAON�I�I����������I"},
	{ "msg_event",		"PAON�I�I�A�L�k�[�J��I"},
	{ "msg_start",		"�z��~(�X�o�i��)"},
	{ "msg_end",		"�z��~(��ù)"}
	
};

static int NPC_BusSetPoint( int meindex, char *argstr);
static void NPC_BusSetDestPoint( int meindex, char *argstr);
static BOOL NPC_BusCheckDeniedItem( int meindex, int charaindex, char *argstr);
static BOOL NPC_BusCheckLevel( int meindex, int charaindex, char *argstr);
static int NPC_BusCheckStone( int meindex, int charaindex, char *argstr);
static void NPC_BusSendMsg( int meindex, int talkerindex, int tablenum);
static int NPC_BusGetRoutePointNum( int meindex, char *argstr );
static void NPC_Bus_walk( int meindex);

#define		NPC_BUS_LOOPTIME		200

/* �Ԥ����֥ǥե���� */
#define		NPC_BUS_WAITTIME_DEFAULT	180

#define		NPC_BUS_WAITINGMODE_WAITTIME	5000

/*********************************
* �����  
*********************************/
BOOL NPC_BusInit( int meindex )
{
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	int i;
	char	buf[256];
	int	routenum;
	int	waittime;
	int seflg;
	
	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));

	/* �ʤ���Ф����ʤ������Υ����å� */
	routenum = NPC_Util_GetNumFromStrWithDelim( argstr, "routenum");//��p���u�ƶq

	if( routenum == -1 ) {
		print( "npcbus:nothing routenum \n");
		return FALSE;
	}
	CHAR_setWorkInt( meindex, NPC_WORK_ROUTEMAX, routenum);
	
	for( i = 1; i <= routenum; i ++ ) {
		char routetostring[64];
		snprintf( routetostring, sizeof( routetostring), "routeto%d", i);
		if( NPC_Util_GetStrFromStrWithDelim( argstr, routetostring,buf, sizeof(buf))
			== NULL ) 
		{
			print( "npcbus:nothing route to \n");
			return FALSE;
		}

	}
	waittime = NPC_Util_GetNumFromStrWithDelim( argstr, "waittime");

	if( waittime == -1 ) waittime = NPC_BUS_WAITTIME_DEFAULT;
	CHAR_setWorkInt( meindex, NPC_WORK_WAITTIME, waittime);

	seflg = NPC_Util_GetNumFromStrWithDelim( argstr, "seflg");

	if( seflg == -1 ) seflg = TRUE;
	CHAR_setWorkInt( meindex, NPC_WORK_SEFLG, seflg);
    
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPEBUS );
	
	CHAR_setWorkInt( meindex, NPC_WORK_MODE, 0);
	CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, 2);
	CHAR_setWorkInt( meindex, NPC_WORK_ROUNDTRIP, 0);
	CHAR_setWorkInt( meindex, NPC_WORK_CURRENTROUTE, 0);
			
	CHAR_setInt( meindex, CHAR_LOOPINTERVAL, 
						NPC_BUS_WAITINGMODE_WAITTIME);
    
    /* ���ߤλ��֤򥻥å� */
    CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);

    for( i = 0; i < CHAR_PARTYMAX; i ++) {
    	CHAR_setWorkInt( meindex, CHAR_WORKPARTYINDEX1 + i, -1);
    }
	
	/* �롼�ȷ��ꤹ�� */
{
	int rev;
	int r = CHAR_getWorkInt( meindex, NPC_WORK_ROUTEMAX);
	CHAR_setWorkInt( meindex, NPC_WORK_CURRENTROUTE, RAND( 1, r));
	//print( "route:%d\n",CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));

	/*   �������� */
	rev = NPC_Util_GetNumFromStrWithDelim( argstr, "reverse");

	if( rev == 1 ) {
		int num = NPC_BusGetRoutePointNum( meindex, argstr);

		if( num <= 0 ) {
			print( "npcbus:Very strange�I\n");
			return FALSE;
		}
		CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, num-1);
		CHAR_setWorkInt( meindex, NPC_WORK_ROUNDTRIP, 1);
	}
	/* �롼�Ȥ򥻥åȤ��� */
	NPC_BusSetPoint( meindex, argstr);
	/* �Ԥ����  ������ */
	NPC_BusSetDestPoint( meindex, argstr);
}

    return TRUE;
}


/*********************************
*   ��������줿���ν�  
*********************************/
void NPC_BusTalked( int meindex , int talkerindex , char *szMes ,
                     int color )
{
    int i;
    int	partyflg = FALSE;
	
    /* �ץ쥤�䡼���Ф��Ƥ���  ������ */
    if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }
	/* ��ʬ�Υѡ�  ��  ���  ���ɤ���Ĵ�٤� */
	for( i = 0; i < CHAR_PARTYMAX; i ++ ) {
		int index = CHAR_getWorkInt( meindex, CHAR_WORKPARTYINDEX1+i);
		if( CHAR_CHECKINDEX(index)){
			if( index == talkerindex) {
				partyflg = TRUE;
			}
		}
	}
	if( !partyflg ) {
		//NPC_BusCheckJoinParty( meindex, talkerindex, TRUE);
	}
	else {
		if( CHAR_getWorkInt( meindex, NPC_WORK_MODE) == 0 ) {
			int i;
	//		#define NPC_BUS_DEBUGROUTINTG	"routingtable:"
			if( strstr( szMes, "�X�o" )  ||
				strstr( szMes, "�X�o" )  ||
				strstr( szMes, "Go" )  ||
				strstr( szMes, "go" ))
			{
				CHAR_setWorkInt( meindex, NPC_WORK_MODE,1);
				
				/* �롼�״ؿ��θƽФ��򬤤�®�٤ˤ��� */
	 			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, NPC_BUS_LOOPTIME);
				/* SE   �餹  �ޥ�⥹�ζ���   */
				if( CHAR_getWorkInt( meindex, NPC_WORK_SEFLG )) {
					CHAR_sendSEoArroundCharacter( 
									CHAR_getInt( meindex, CHAR_FLOOR),
									CHAR_getInt( meindex, CHAR_X),
									CHAR_getInt( meindex, CHAR_Y),
									60,
									TRUE);
				}
				/* ��  ������Υ�å�����*/
				for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
					int partyindex = CHAR_getWorkInt( meindex, CHAR_WORKPARTYINDEX1+i);
					if( CHAR_CHECKINDEX( partyindex)) {
						NPC_BusSendMsg( meindex, partyindex, NPC_BUS_MSG_START);
					}
				}
			}
		}
#if 0
		else if( strstr( szMes, "����" )  ||
			strstr( szMes, "����" )  ||
			strstr( szMes, "stop" )  ||
			strstr( szMes, "Stop" ))
		{
			CHAR_setWorkInt( meindex, NPC_WORK_MODE,2);

			/* �롼�״ؿ��Υ��󥿡��Х��¿������  */
			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, 
						NPC_BUS_WAITINGMODE_WAITTIME);
		    /* ���ߤλ��֤򥻥å� */
		    CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);
		}
		else if( strstr( szMes, NPC_BUS_DEBUGROUTINTG )) {
			/* �ǥХå��� */
			char *p = strstr( szMes,NPC_BUS_DEBUGROUTINTG);
			char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
			char *argstr;

			argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
			if( p) {
				int a = atoi( p+strlen(NPC_BUS_DEBUGROUTINTG));
				if( a <0 ) a = 1;
				CHAR_setWorkInt( meindex, NPC_WORK_CURRENTROUTE, a);
			}
			//print( "route:%d\n",CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));
			/* �롼�Ȥ򥻥åȤ��� */
			NPC_BusSetPoint( meindex, argstr);
		}
#endif
	}
}
/**************************************
 * �롼�״ؿ�
 **************************************/
void NPC_BusLoop( int meindex)
{
	int	i;
	switch( CHAR_getWorkInt( meindex, NPC_WORK_MODE )) {
	  case 0:
	    /* �Ԥ��⡼�ɤλ��f���֤�����å����� */
		/* ���֤��Фä��Τǡf��  ���� */
		if( CHAR_getWorkInt( meindex, NPC_WORK_CURRENTTIME) 
			+ CHAR_getWorkInt( meindex, NPC_WORK_WAITTIME) 
			< NowTime.tv_sec)
		{
			/* SE   �餹  �ޥ�⥹�ζ���   */
			if( CHAR_getWorkInt( meindex, NPC_WORK_SEFLG )) {
				CHAR_sendSEoArroundCharacter( 
								CHAR_getInt( meindex, CHAR_FLOOR),
								CHAR_getInt( meindex, CHAR_X),
								CHAR_getInt( meindex, CHAR_Y),
								60,
								TRUE);
			}
			/* ��  ������Υ�å�����*/
			for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
				int partyindex = CHAR_getWorkInt( meindex, CHAR_WORKPARTYINDEX1+i);
				if( CHAR_CHECKINDEX( partyindex)) {
					NPC_BusSendMsg( meindex, partyindex, NPC_BUS_MSG_START);
				}
			}
			
			CHAR_setWorkInt( meindex, NPC_WORK_MODE,1);
			/* �롼�״ؿ��θƽФ��򬤤�®�٤ˤ��� */
			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, NPC_BUS_LOOPTIME);
		}
		return;
	  case 1:
	  	/* ���� */
	  	NPC_Bus_walk( meindex);
	  case 2:
		/* �ߤޤäƤ���⡼�� */
		/* ���֤��Фä��Τǡf��  ���� */
		if( CHAR_getWorkInt( meindex, NPC_WORK_CURRENTTIME) 
			+ (CHAR_getWorkInt( meindex, NPC_WORK_WAITTIME) /3)
			< NowTime.tv_sec)
		{
			CHAR_setWorkInt( meindex, NPC_WORK_MODE,1);
			/* �롼�״ؿ��θƽФ��򬤤�®�٤ˤ��� */
			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, NPC_BUS_LOOPTIME);
		
		}
		return;
	  case 3:
		/* ���夷�Ƥ�f���饤����Ȥ�  ���Ԥ��ΰ٤ˡf
		 * ���������ǥ������Ȥ򤤤�Ƥ��
		 */
		if( CHAR_getWorkInt( meindex, NPC_WORK_CURRENTTIME) + 3
			< NowTime.tv_sec)
		{
			char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
			char *argstr;

			argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
			/* �롼�״ؿ��θƽФ����٤����� */
			CHAR_setInt( meindex, CHAR_LOOPINTERVAL, 
						NPC_BUS_WAITINGMODE_WAITTIME);
			
			/* �롼�ȷ��ꤹ�� */
			{
				int r = CHAR_getWorkInt( meindex, NPC_WORK_ROUTEMAX);
				CHAR_setWorkInt( meindex, NPC_WORK_CURRENTROUTE, RAND( 1, r));
				//print( "route:%d\n",CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));
			}
			/* �Ԥ�����ե饰  �� */
			CHAR_setWorkInt( meindex, NPC_WORK_ROUNDTRIP, 
							CHAR_getWorkInt( meindex, NPC_WORK_ROUNDTRIP)^1);

			/* ���ݥ���Ȥ�Ĵ�� */
			/* �����  ���   */
			if( CHAR_getWorkInt( meindex, NPC_WORK_ROUNDTRIP) == 1)  {
				/* ���Υ롼�Ȥ�  ��ݥ���ȿ���  �� */
				int num = NPC_BusGetRoutePointNum( meindex, argstr);
				CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, num-1);
			}
			else {
				CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, 
							CHAR_getWorkInt( meindex, NPC_WORK_ROUTEPOINT) +1);
			}
			/* ���Υݥ���Ȥκ�  �򥻥åȤ��� */
			NPC_BusSetPoint( meindex, argstr);
			/* �Ԥ����  ������ */
			NPC_BusSetDestPoint( meindex, argstr);
			/* �ѡ�  ��  �����  �򤹤� */
			CHAR_DischargeParty( meindex, 0);
		    /* ���ߤλ��֤򥻥å� */
		    CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);
			/* �⡼�ɥ��ꥢ */
			CHAR_setWorkInt( meindex, NPC_WORK_MODE, 0);
		}
		return;
	  default:
	    break;
	}
}
/**************************************
 * �����e
 **************************************/
static void NPC_Bus_walk( int meindex)
{	
	POINT	start, end;
	int dir;
	int ret;
	int i;

	/* �����ط� */
	/* ���夷�����ν�   */
	start.x = CHAR_getInt( meindex, CHAR_X);
	start.y = CHAR_getInt( meindex, CHAR_Y);
	end.x = CHAR_getWorkInt( meindex, NPC_WORK_ROUTETOX);
	end.y = CHAR_getWorkInt( meindex, NPC_WORK_ROUTETOY);
	
	/* ���夷���ΤǱ��Υݥ���Ȥ� */
	if( start.x == end.x && start.y == end.y ) {
		int add = 1;
		char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
		char *argstr;

		argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));

		if( CHAR_getWorkInt( meindex, NPC_WORK_ROUNDTRIP ) == 1 ) {
			add *= -1;
		}
		CHAR_setWorkInt( meindex, NPC_WORK_ROUTEPOINT, 
						CHAR_getWorkInt( meindex, NPC_WORK_ROUTEPOINT) +add);
		if( NPC_BusSetPoint( meindex, argstr) == FALSE ) {
			/*     ������*/
			/* �Ԥ��⡼�ɤˤ��� */
			CHAR_setWorkInt( meindex, NPC_WORK_MODE,3);
			
			/* SE   �餹  �ޥ�⥹�ζ���   */
			if( CHAR_getWorkInt( meindex, NPC_WORK_SEFLG )) {
				CHAR_sendSEoArroundCharacter( 
								CHAR_getInt( meindex, CHAR_FLOOR),
								CHAR_getInt( meindex, CHAR_X),
								CHAR_getInt( meindex, CHAR_Y),
								60,
								TRUE);
			}
			/* �夤�����Υ�å�����*/
			for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
				int partyindex = CHAR_getWorkInt( meindex, CHAR_WORKPARTYINDEX1+i);
				if( CHAR_CHECKINDEX( partyindex)) {
					NPC_BusSendMsg( meindex, partyindex, NPC_BUS_MSG_END);
				}
			}
		    /* ���ߤλ��֤򥻥å� */
		    CHAR_setWorkInt( meindex, NPC_WORK_CURRENTTIME, NowTime.tv_sec);
			return;
		}
		else {
			return;
		}
	}
	/*-------------------------------------------------------*/
	/* ���������   */
	
	/*   ������� */
	dir = NPC_Util_getDirFromTwoPoint( &start,&end );

	/* �����������    �ѡ�  �������ǻȤ�   */
	end.x = CHAR_getInt( meindex, CHAR_X);
	end.y = CHAR_getInt( meindex, CHAR_Y);
	
	/* �Ҥä����ä����ΰ٤ν�   */
	for( i = 0; i < 100; i ++ ) {	
		if( dir < 0 ) {
			dir = RAND( 0,7);
		}	
		dir = NPC_Util_SuberiWalk( meindex, dir);
		if( dir >= 0 && dir <= 7) break;
	}
	
	if( dir >= 0 && dir <= 7 ) {
		/* ���� */
		ret = CHAR_walk( meindex, dir, 0);

		if( ret == CHAR_WALKSUCCESSED ) {
			/* ��ʬ���Ƥʤ���֤򬤤����� */
			int	i;
			for( i = 1; i < CHAR_PARTYMAX; i ++ ) {
				int toindex = CHAR_getWorkInt( meindex, i + CHAR_WORKPARTYINDEX1);
				if( CHAR_CHECKINDEX(toindex) ) {
					int		parent_dir;
					/* �Ҥΰ�  �ȡf�Ƥά������ΰ�  ����  ������� */
					/* ���� */
					start.x = CHAR_getInt( toindex, CHAR_X);
					start.y = CHAR_getInt( toindex, CHAR_Y);
					parent_dir = NPC_Util_getDirFromTwoPoint( &start,&end );
					/* ����ǥ��������׼���󬤤���  ������٤ˡf
					 * ���λҤ����λҤ�  ���ɤ��褦�ˤ���
					 */
					end = start;
					if( parent_dir != -1 ) {
						CHAR_walk( toindex, parent_dir, 0);
					}
				}
			}
	    }
	}
}
/**************************************
 * ���ξ��򥻥åȤ���
 **************************************/
static int NPC_BusSetPoint( int meindex, char *argstr)
{
	char	buf[4096];
	char	buf2[256];
	char	buf3[256];
	int ret;
	char routetostring[64];
	
	snprintf( routetostring, sizeof( routetostring), "routeto%d", 
				CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));
	
	if( NPC_Util_GetStrFromStrWithDelim( argstr, routetostring,buf, sizeof(buf))
		== NULL ) 
	{
		print( "npcbus:nothing route \n");
		return FALSE;
	}
	ret = getStringFromIndexWithDelim( buf, ";", 
								CHAR_getWorkInt( meindex, NPC_WORK_ROUTEPOINT),
								buf2, sizeof(buf2));
		
	if( ret == FALSE ) return FALSE;

	ret = getStringFromIndexWithDelim( buf2, ",", 1,
								buf3, sizeof(buf3));
	if( ret == FALSE) return FALSE;
	CHAR_setWorkInt( meindex, NPC_WORK_ROUTETOX, atoi( buf3));
	
	ret = getStringFromIndexWithDelim( buf2, ",", 2,
								buf3, sizeof(buf3));
	if( ret == FALSE) return FALSE;
	CHAR_setWorkInt( meindex, NPC_WORK_ROUTETOY, atoi( buf3));
	return TRUE;
}
/**************************************
 * route  �椫��f  �������ä��餽���
 * �ι�ΤȤ��˥��åȤ���e
 **************************************/
static void NPC_BusSetDestPoint( int meindex, char *argstr)
{
	char 	buf[256];
	char	routename[256];

	snprintf( routename, sizeof( routename), "routename%d", 
				CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));

	if( NPC_Util_GetStrFromStrWithDelim( argstr, routename, buf, sizeof( buf))
		!= NULL ) 
	{
		CHAR_setChar( meindex, CHAR_OWNTITLE, buf);
		CHAR_sendCToArroundCharacter( CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX));
	}
}
/**************************************
 * ���ꤵ�줿����  ���  �äƤ��뤫�����å�����
 *   �äƤ��������
 **************************************/
static BOOL NPC_BusCheckDeniedItem( int meindex, int charaindex, char *argstr)
{
	char	buf[1024];
	BOOL	found = TRUE;

	if( NPC_Util_GetStrFromStrWithDelim( argstr, "denieditem", buf, sizeof( buf))
		!= NULL ) 
	{
		int	i;
		int ret;
		for( i = 1; ; i ++) {
			int itemid;
			char buf2[64];
			int j;
			ret = getStringFromIndexWithDelim( buf, ",", i, buf2, sizeof(buf2));
			if( ret == FALSE ) break;
			itemid = atoi( buf2);
			for( j = 0; j < CHAR_MAXITEMHAVE; j ++) {
				int itemindex = CHAR_getItemIndex( charaindex, j);
				if( ITEM_CHECKINDEX( itemindex)) {
					if( ITEM_getInt( itemindex, ITEM_ID) == itemid) {
						found = FALSE;
						break;
					}
				}
			}
		}
	}
	return found;
}
/**************************************
 * ���ꤵ�줿����  ���  �äƤ��뤫�����å�����
 *   �äƤ��ʤ��Ȥ���
 **************************************/
BOOL NPC_BusCheckAllowItem( int meindex, int charaindex, BOOL pickupmode)
{
	char	buf[1024];
	BOOL	found = TRUE;
	BOOL	pickup = FALSE;
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	
	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
	
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "pickupitem", buf, sizeof( buf))!= NULL ){
		pickup = TRUE;
	}
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "allowitem", buf, sizeof( buf))!= NULL ){
		int	i;
		int ret;
		for( i = 1; ; i ++) {
			int itemid;
			char buf2[64];
			int j;
			BOOL	getflg;
			ret = getStringFromIndexWithDelim( buf, ",", i, buf2, sizeof(buf2));
			if( ret == FALSE ) break;
			itemid = atoi( buf2);
			getflg = FALSE;
			for( j = 0; j < CHAR_MAXITEMHAVE; j ++) {
				int itemindex = CHAR_getItemIndex( charaindex, j);
				if( ITEM_CHECKINDEX( itemindex)) {
					if( ITEM_getInt( itemindex, ITEM_ID) == itemid) {
						if( pickupmode && pickup && !getflg) {
							CHAR_DelItem( charaindex, j);
							getflg = TRUE;
						}
						break;
					}
				}
			}
			if( j == CHAR_MAXITEMHAVE) {
				found = FALSE;
				break;
			}
		}
	}
	return found;
}

/**************************************
 * ���ꤵ�줿��٥�ʾ夫�����å�����
 **************************************/
static BOOL NPC_BusCheckLevel( int meindex, int charaindex, char *argstr)
{
	int		level;
	
	/* �ʤ���Ф����ʤ������Υ����å� */
	level = NPC_Util_GetNumFromStrWithDelim( argstr, "needlevel");
	if( level == -1 ) {
		return TRUE;
	}
	if( CHAR_getInt( charaindex, CHAR_LV) >= level ) return TRUE;
	
	return FALSE;
}
/**************************************
 * ��������å�����
 * -1 ��   0�ʾ�v    �f����  ��Stone
 **************************************/
static int NPC_BusCheckStone( int meindex, int charaindex, char *argstr)
{
	int		gold;
	
	/* �ʤ���Ф����ʤ������Υ����å� */
	gold = NPC_Util_GetNumFromStrWithDelim( argstr, "needstone");
	if( gold == -1 ) {
		return 0;
	}
	if( CHAR_getInt( charaindex, CHAR_GOLD) >= gold ) return gold;
	
	return -1;
}
/**************************************
 * ��å�����������
 * �����Υ�å��������ʤ���Хǥե���ȥ�å�����������
 **************************************/
static void NPC_BusSendMsg( int meindex, int talkerindex, int tablenum)
{
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char	buf[256];
	char	msg[256];
	if( tablenum < 0 || tablenum >= arraysizeof( busmsg)) return;
	
	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
	
	if( NPC_Util_GetStrFromStrWithDelim( argstr, busmsg[tablenum].option, buf, sizeof( buf))
		!= NULL ) 
	{
		strcpy( msg, buf);
	}
	else {
		snprintf( msg, sizeof(msg),busmsg[tablenum].defaultmsg);
	}
	CHAR_talkToCli( talkerindex, meindex, msg, CHAR_COLORYELLOW);
}
/**************************************
 * �롼��  ���֥�Υݥ���Ȥο����  ����
 **************************************/
static int NPC_BusGetRoutePointNum( int meindex, char *argstr )
{
	int		i;
	char	buf[4096];
	char	buf2[256];
	int ret;
	char routetostring[64];
	
	snprintf( routetostring, sizeof( routetostring), "routeto%d", 
				CHAR_getWorkInt( meindex, NPC_WORK_CURRENTROUTE));
	
	if( NPC_Util_GetStrFromStrWithDelim( argstr, routetostring,buf, sizeof(buf))
		== NULL ) 
	{
		print( "npcbus:nothing route \n");
		return -1;
	}
	for( i = 1; ; i ++ ) {
		ret = getStringFromIndexWithDelim( buf, ";", i, buf2, sizeof(buf2));
		if( ret == FALSE) break;
	}
	return( i -1);
}
BOOL NPC_BusCheckJoinParty( int meindex, int charaindex, BOOL msgflg)
{
    //int		fd;
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	int		ret;
	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
	/* ������åɰ�  �Τ� */
	if( !NPC_Util_charIsInFrontOfChar( charaindex, meindex, 1 )) return FALSE; 
	/*     ��֤ϵ�  ���� */
	if( CHAR_getWorkInt( meindex, NPC_WORK_MODE) != 0 ) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_GETTINGON);
		return FALSE;
	}
	/* �ѡ��Ƥ����ä������ */
	if( CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE ) != CHAR_PARTY_NONE) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_NOTPARTY);
		return FALSE;
	}
	/* �ѡ�  ���οͿ�������å����� */
	if( CHAR_getEmptyPartyArray( meindex) == -1 ) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_OVERPARTY);
		return FALSE;
	}
	/* ����  ��Υ����å��򤹤�(�ػߥ���  ��) */
	if( !NPC_BusCheckDeniedItem( meindex, charaindex, argstr)) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_DENIEDITEM);
		return FALSE;
	}
#ifdef _ITEM_CHECKWARES
	if( CHAR_CheckInItemForWares( charaindex, 0) == FALSE )	{
		CHAR_talkToCli( charaindex, -1, "�L�k��a�f���W���C", CHAR_COLORYELLOW);
		return FALSE;
	}
#endif
	/* ����  ��Υ����å��򤹤�(  �ץ���  ��) */
	if( !NPC_BusCheckAllowItem( meindex, charaindex, FALSE)) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_ALLOWITEM);
		return FALSE;
	}
	/* ��٥�Υ����å��򤹤� */
	if( !NPC_BusCheckLevel( meindex, charaindex, argstr)) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_LEVEL);
		return FALSE;
	}
	/* ���٥��  �������å����� */
//	if( CHAR_getInt( charaindex, CHAR_NOWEVENT) != 0 ||
//		CHAR_getInt( charaindex, CHAR_NOWEVENT2) != 0 ||
//		CHAR_getInt( charaindex, CHAR_NOWEVENT3) != 0 )
//	{
//		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_EVENT);
//		return FALSE;
//	}
	/* ���Υ����å��򤹤�  �������Τǡf  �������å��ˤ��뤳�ȡ�   */
	ret = NPC_BusCheckStone( meindex, charaindex, argstr);
	if( ret == -1 ) {
		if( msgflg) NPC_BusSendMsg( meindex, charaindex, NPC_BUS_MSG_GOLD);
		return FALSE;
	}
	if( ret != 0 ) {
		char msgbuf[128];
		/* ����Ȥ� */
		CHAR_setInt( charaindex, CHAR_GOLD, 
					CHAR_getInt( charaindex, CHAR_GOLD) - ret);
		/* ���� */
		CHAR_send_P_StatusString( charaindex, CHAR_P_STRING_GOLD);
		snprintf( msgbuf, sizeof( msgbuf), "��I�F%d Stone�I", ret);
		CHAR_talkToCli( charaindex, -1, msgbuf, CHAR_COLORYELLOW);
	}
	/* �ѡ�  ����  �� */
	//CHAR_JoinParty_Main( charaindex, meindex);
	
	//fd = getfdFromCharaIndex( charaindex );
	
	//lssproto_PR_send( fd, 1, 1);
	return TRUE;
}
