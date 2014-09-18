#include "version.h"
#include "autil.h"
#include "lssproto_serv.h"
#include "common.h"	// for StoneAge
#include "log.h"	// for StoneAge
#include "char.h"
#include <stdio.h>
#include <time.h>
#include <net.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

void lssproto_SetServerLogFiles( char *r , char *w )
{
	lssproto_strcpysafe( lssproto_writelogfilename , w , sizeof(lssproto_writelogfilename ));
	lssproto_strcpysafe( lssproto_readlogfilename , r , sizeof(lssproto_readlogfilename ));
}
int lssproto_InitServer(	int (*writefunc)(int,char*,int) ,	int worksiz )
{
	int i;
	if( (void*)writefunc == NULL){lssproto.write_func = lssproto_default_write_wrap;} else {lssproto.write_func = writefunc;}
	lssproto_AllocateCommonWork(worksiz);
	lssproto_stringwrapper = (char**)calloc( 1,sizeof(char*) * MAXLSRPCARGS);
	if(lssproto_stringwrapper ==NULL)return -1;
	memset( lssproto_stringwrapper , 0, sizeof(char*)*MAXLSRPCARGS);
	for(i=0;i<MAXLSRPCARGS;i++){
		lssproto_stringwrapper[i] = (char*)calloc( 1, worksiz );
		if( lssproto_stringwrapper[i] == NULL){
			for(i=0;i<MAXLSRPCARGS;i++){free( lssproto_stringwrapper[i]);return -1;}
		}
	}
	lssproto.ret_work = (char*)calloc( 1,sizeof( worksiz ));
	if( lssproto.ret_work == NULL ){ return -1; }
	return 0;
}
void lssproto_CleanupServer( void )
{
	int i;
	free( lssproto.work );
	free( lssproto.arraywork);
	free( lssproto.escapework );
	free( lssproto.val_str);
	free( lssproto.token_list );
	for(i=0;i<MAXLSRPCARGS;i++){free( lssproto_stringwrapper[i]);}
	free( lssproto_stringwrapper );
	free( lssproto.ret_work );
}
#ifdef _JZ_BEST
typedef struct taglssproto_func
{
    int func;
    void*	ls_func;
    void* de_func;
}lssprotofunc;

static lssprotofunc lsprofunc[]=
{	
	{LSSPROTO_W_RECV,lssproto_W_recv,lssproto_dedata_IIC},//0
	{LSSPROTO_W2_RECV,lssproto_W2_recv,lssproto_dedata_IIC},//1
	{LSSPROTO_XYD_SEND,NULL,NULL},//2
	{LSSPROTO_EV_RECV,lssproto_EV_recv,lssproto_dedata_IIIII},//3
	{LSSPROTO_EV_SEND,NULL,NULL},//4
	{LSSPROTO_EN_RECV,NULL,NULL},//5
	{LSSPROTO_DU_RECV,lssproto_DU_recv,lssproto_dedata_II},//6
	{LSSPROTO_EN_SEND,NULL,NULL},//7
	{LSSPROTO_EO_RECV,lssproto_EO_recv,lssproto_dedata_I},//8
	{LSSPROTO_BU_RECV,lssproto_BU_recv,lssproto_dedata_I},//9
	{LSSPROTO_JB_RECV,lssproto_JB_recv,lssproto_dedata_II},//10
	{LSSPROTO_LB_RECV,lssproto_LB_recv,lssproto_dedata_II},//11
	{LSSPROTO_RS_SEND,NULL,NULL},//12
	{LSSPROTO_RD_SEND,NULL,NULL},//13
	{LSSPROTO_B_RECV,lssproto_B_recv,lssproto_dedata_C},//14
	{LSSPROTO_B_SEND,NULL,NULL},//15
	{LSSPROTO_SKD_RECV,lssproto_SKD_recv,lssproto_dedata_II},//16
	{LSSPROTO_ID_RECV,lssproto_ID_recv,lssproto_dedata_IIII},//17
	{LSSPROTO_PI_RECV,lssproto_PI_recv,lssproto_dedata_III},//18
	{LSSPROTO_DI_RECV,lssproto_DI_recv,lssproto_dedata_III},//19
	{LSSPROTO_DG_RECV,lssproto_DG_recv,lssproto_dedata_III},//20
	{LSSPROTO_DP_RECV,lssproto_DP_recv,lssproto_dedata_III},//21
	{LSSPROTO_I_SEND,NULL,NULL},//22
	{LSSPROTO_MI_RECV,lssproto_MI_recv,lssproto_dedata_II},//23
	{LSSPROTO_SI_SEND,NULL,NULL},//24
	{LSSPROTO_MSG_RECV,lssproto_MSG_recv,lssproto_dedata_ICI},//25
	{LSSPROTO_MSG_SEND,NULL,NULL},//26
	{LSSPROTO_PMSG_RECV,lssproto_PMSG_recv,lssproto_dedata_IIICI},//27
	{LSSPROTO_PME_SEND,NULL,NULL},//28
	{LSSPROTO_AB_RECV,lssproto_AB_recv,NULL},//29
	{LSSPROTO_AB_SEND,NULL,NULL},//30
	{LSSPROTO_ABI_SEND,NULL,NULL},//31
	{LSSPROTO_DAB_RECV,lssproto_DAB_recv,lssproto_dedata_I},//32
	{LSSPROTO_AAB_RECV,lssproto_AAB_recv,lssproto_dedata_II},//33
	{LSSPROTO_L_RECV,lssproto_L_recv,lssproto_dedata_I},//34
	{LSSPROTO_TK_RECV,lssproto_TK_recv,lssproto_dedata_IICII},//35
	{LSSPROTO_TK_SEND,NULL,NULL},//36
	{LSSPROTO_MC_SEND,NULL,NULL},//37
	{LSSPROTO_M_RECV,lssproto_M_recv,lssproto_dedata_IIIII},//38
	{LSSPROTO_M_SEND,NULL,NULL},//39
	{LSSPROTO_C_RECV,lssproto_C_recv,lssproto_dedata_I},//40
	{LSSPROTO_C_SEND,NULL,NULL},//41
	{LSSPROTO_CA_SEND,NULL,NULL},//42
	{LSSPROTO_CD_SEND,NULL,NULL},//43
	{LSSPROTO_R_SEND,NULL,NULL},//44
	{LSSPROTO_S_RECV,lssproto_S_recv,lssproto_dedata_C},//45
	{LSSPROTO_S_SEND,NULL,NULL},//46
	{LSSPROTO_D_SEND,NULL,NULL},//47
	{LSSPROTO_FS_RECV,lssproto_FS_recv,lssproto_dedata_I},//48
	{LSSPROTO_FS_SEND,NULL,NULL},//49
	{LSSPROTO_HL_RECV,lssproto_HL_recv,lssproto_dedata_I},//50
	{LSSPROTO_HL_SEND,NULL,NULL},//51
	{LSSPROTO_PR_RECV,lssproto_PR_recv,lssproto_dedata_III},//52
	{LSSPROTO_PR_SEND,NULL,NULL},//53
	{LSSPROTO_KS_RECV,lssproto_KS_recv,lssproto_dedata_I},//54
	{LSSPROTO_KS_SEND,NULL,NULL},//55
	{LSSPROTO_AC_RECV,lssproto_AC_recv,lssproto_dedata_III},//56
	{LSSPROTO_MU_RECV,lssproto_MU_recv,lssproto_dedata_IIII},//57
	{LSSPROTO_PS_RECV,lssproto_PS_recv,lssproto_dedata_IIIC},//58
	{LSSPROTO_PS_SEND,NULL,NULL},//59
	{LSSPROTO_ST_RECV,lssproto_ST_recv,lssproto_dedata_I},//60
	{LSSPROTO_DT_RECV,lssproto_DT_recv,lssproto_dedata_I},//61
	{LSSPROTO_FT_RECV,lssproto_FT_recv,lssproto_dedata_C},//62
	{LSSPROTO_SKUP_SEND,NULL,NULL},//63
	{LSSPROTO_SKUP_RECV,lssproto_SKUP_recv,lssproto_dedata_I},//64
	{LSSPROTO_KN_RECV,lssproto_KN_recv,lssproto_dedata_IC},//65
	{LSSPROTO_WN_SEND,NULL,NULL},//66
	{LSSPROTO_WN_RECV,lssproto_WN_recv,lssproto_dedata_IIIIIC},//67
	{LSSPROTO_EF_SEND,NULL,NULL},//68
	{LSSPROTO_SE_SEND,NULL,NULL},//69
	{LSSPROTO_SP_RECV,lssproto_SP_recv,lssproto_dedata_III},//70
	{LSSPROTO_CLIENTLOGIN_RECV,lssproto_ClientLogin_recv,lssproto_dedata_CC},//71
	{LSSPROTO_CLIENTLOGIN_SEND,NULL,NULL},//72
	{LSSPROTO_CREATENEWCHAR_RECV,lssproto_CreateNewChar_recv,lssproto_dedata_ICIIIIIIIIIII},//73
	{LSSPROTO_CREATENEWCHAR_SEND,NULL,NULL},//74
	{LSSPROTO_CHARDELETE_RECV,lssproto_CharDelete_recv,lssproto_dedata_C},//75
	{LSSPROTO_CHARDELETE_SEND,NULL,NULL},//76
	{LSSPROTO_CHARLOGIN_RECV,lssproto_CharLogin_recv,lssproto_dedata_C},//77
	{LSSPROTO_CHARLOGIN_SEND,NULL,NULL},//78
	{LSSPROTO_CHARLIST_RECV,lssproto_CharList_recv,NULL},//79
	{LSSPROTO_CHARLIST_SEND,NULL,NULL},//80
	{LSSPROTO_CHARLOGOUT_RECV,lssproto_CharLogout_recv,lssproto_dedata_I},//81
	{LSSPROTO_CHARLOGOUT_SEND,NULL,NULL},//82
	{LSSPROTO_PROCGET_RECV,lssproto_ProcGet_recv,NULL},//83
	{LSSPROTO_PROCGET_SEND,NULL,NULL},//84
	{LSSPROTO_PLAYERNUMGET_RECV,lssproto_PlayerNumGet_recv,NULL},//85
	{LSSPROTO_PLAYERNUMGET_SEND,NULL,NULL},//86
	{LSSPROTO_ECHO_RECV,lssproto_Echo_recv,lssproto_dedata_C},//87
	{LSSPROTO_ECHO_SEND,NULL,NULL},//88
	{LSSPROTO_SHUTDOWN_RECV,lssproto_Shutdown_recv,lssproto_dedata_CI},//89
	{LSSPROTO_NU_SEND,NULL,NULL},//90
	{LSSPROTO_TD_RECV,lssproto_TD_recv,lssproto_dedata_C},//91
	{LSSPROTO_TD_SEND,NULL,NULL},//92
	{LSSPROTO_FM_SEND,NULL,NULL},//93
	{LSSPROTO_FM_RECV,lssproto_FM_recv,lssproto_dedata_C},//94
	{LSSPROTO_WO_SEND,NULL,NULL},//95
	{LSSPROTO_PETST_RECV,lssproto_PETST_recv,lssproto_dedata_II},//96
	{LSSPROTO_BM_RECV,lssproto_BM_recv,lssproto_dedata_I},//97   // _BLACK_MARKET

#ifdef _MIND_ICON
	{LSSPROTO_MA_RECV,lssproto_MA_recv,lssproto_dedata_III},//98
#else
	{0,NULL,NULL},
#endif

#ifdef _FIX_DEL_MAP              // WON ADD ���a��a�ϰe�ʺ�
	{LSSPROTO_DM_RECV,lssproto_DM_recv,NULL},//99
#else
	{0,NULL,NULL},
#endif

#ifdef _ITEM_CRACKER
	{LSSPROTO_IC_SEND,NULL,NULL},//100
#else
	{0,NULL,NULL},
#endif

#ifdef _MAGIC_NOCAST             // ���F:�I�q
	{LSSPROTO_NC_SEND,NULL,NULL},//101
#else
	{0,NULL,NULL},
#endif

	{0,NULL,NULL},//102
	
#ifdef _CHECK_GAMESPEED
	{LSSPROTO_CS_RECV,lssproto_CS_recv,NULL},//103 //�[�t���w
	{LSSPROTO_CS_SEND,NULL,NULL},//104 //�[�t���w
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

	{0,NULL,NULL},//105
	
#ifdef _TEAM_KICKPARTY
	{LSSPROTO_KTEAM_RECV,lssproto_KTEAM_recv,lssproto_dedata_I},//106
#else
	{0,NULL,NULL},
#endif

#ifdef _PETS_SELECTCON
	{LSSPROTO_PETST_SEND,NULL,NULL},//107
#else
	{0,NULL,NULL},
#endif

#ifdef _NEWREQUESTPROTOCOL			// (���i�}) Syu ADD �s�WProtocol�n�D�Ӷ�
	{LSSPROTO_RESIST_RECV,lssproto_RESIST_recv,NULL},//108
	{LSSPROTO_RESIST_SEND,NULL,NULL},//109
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

#ifdef _OUTOFBATTLESKILL			// (���i�}) Syu ADD �D�԰��ɧޯ�Protocol
	{LSSPROTO_BATTLESKILL_RECV,lssproto_BATTLESKILL_recv,lssproto_dedata_I},//110
	{LSSPROTO_BATTLESKILL_SEND,NULL,NULL},//111
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

#ifdef _CHATROOMPROTOCOL			// (���i�}) Syu ADD ��ѫ��W�D
	{LSSPROTO_CHATROOM_RECV,lssproto_CHATROOM_recv,lssproto_dedata_C},//112
	{LSSPROTO_CHATROOM_SEND,NULL,NULL},//113
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

	{LSSPROTO_SPET_RECV,lssproto_SPET_recv,lssproto_dedata_I},//114		// Robin �ݾ��d
	{LSSPROTO_SPET_SEND,NULL,NULL},//115

#ifdef _STREET_VENDOR
	{LSSPROTO_STREET_VENDOR_RECV,lssproto_STREET_VENDOR_recv,lssproto_dedata_C},//116		// �\�u�\��
	{LSSPROTO_STREET_VENDOR_SEND,NULL,NULL},//117
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

#ifdef _RIGHTCLICK
	{LSSPROTO_RCLICK_RECV,lssproto_RCLICK_recv,lssproto_dedata_IC},//118
	{LSSPROTO_RCLICK_SEND,NULL,NULL},//119
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

#ifdef _JOBDAILY
	{LSSPROTO_JOBDAILY_SEND,NULL,NULL},//120		// CYG�@���Ȥ�x�\��
	{LSSPROTO_JOBDAILY_RECV,lssproto_JOBDAILY_recv,lssproto_dedata_C},//121
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

	{0,NULL,NULL},//121
	
#ifdef _TEACHER_SYSTEM
	{LSSPROTO_TEACHER_SYSTEM_RECV,lssproto_TEACHER_SYSTEM_recv,lssproto_dedata_C},//122		// �ɮv�\��
	{LSSPROTO_TEACHER_SYSTEM_SEND,NULL,NULL},//123
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif

#ifdef _ADD_STATUS_2
	{LSSPROTO_S2_RECV,NULL,NULL},//124
	{LSSPROTO_S2_SEND,NULL,NULL},//125
#else
	{0,NULL,NULL},
	{0,NULL,NULL},
#endif
};

#define DME() print("<DME(%d)co:%d func:%d flg:%d>",fd,__LINE__,func,lsret)

int lssproto_ServerDispatchMessage(int fd, char *encoded)
{
	typedef int(*dedata_func)(int,void*);
	typedef void (*lssv_func)(int);
	int func,fieldcount,lsret=-10;
	static char raw[65500];

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);
	
	util_DecodeMessage(raw,encoded);

	if( !util_SplitMessage(raw,SEPARATOR) ){
		print("\nDME1:package=%s\n",raw);
		util_DiscardMessage();
		lsret = -2;
		DME(); return -1;
	}
	if (!util_GetFunctionFromSlice(&func, &fieldcount)) {
		logHack(fd,HACK_GETFUNCFAIL);
		util_DiscardMessage();
		lsret = -3;
		DME(); return -1;
	}
	if(func>=arraysizeof(lsprofunc) || func < 0){
		logHack(fd,HACK_FUNCERR);
		util_DiscardMessage();
		lsret = -4;
		DME(); return -1;
	}
	if(func==LSSPROTO_CLIENTLOGIN_RECV || func ==LSSPROTO_PROCGET_RECV)
		strcpy(PersonalKey, _DEFAULT_PKEY);
	
	lssv_func lssvfunc=(lssv_func)lsprofunc[func].ls_func;
	if(lssvfunc==NULL){
		print("lsfunc is null\n");
		logHack(fd,HACK_LSFUNCNULL);
		util_DiscardMessage();
		lsret = -5;
		DME(); return -1;
	}else if(lsprofunc[func].func==func){
		dedata_func dedatafunc=(dedata_func)lsprofunc[func].de_func;
		if(dedatafunc==NULL){
			lssvfunc(fd);
			util_DiscardMessage();
			return 0;
		}else{
			lsret = dedatafunc(fd,lssvfunc);
			if(lsret<0){
				util_DiscardMessage();
				logHack(fd,HACK_CHECKSUMERROR);
				lsret = -6;
				DME(); return -1;
			}else{
				util_DiscardMessage();
				return lsret;
			}
		}
	}
	util_DiscardMessage();
	logHack(fd,HACK_NOTDISPATCHED);
	DME(); return -1;
}

int lssproto_dedata_IIC(int fd,void* pf)
{
	typedef void (*lsfunc_IIC)(int,int,int,char*);
	lsfunc_IIC pf2=(lsfunc_IIC)pf;
	int checksum=0, checksumrecv;
	int x;
	int y;
	char *direction=malloc(65500);
	if(direction==NULL)return -2;
		
	checksum += util_deint(2, &x);
	checksum += util_deint(3, &y);
	checksum += util_destring(4, direction);
	util_deint(5, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(direction);
		return -1;
	}
	pf2(fd, x, y, direction);
	free(direction);
	return 0;
}

int lssproto_dedata_IIIII(int fd,void* pf)
{
	typedef void (*lsfunc_IIIII)(int,int,int,int,int,int);
	lsfunc_IIIII pf2=(lsfunc_IIIII)pf;
	int checksum=0, checksumrecv;
	int event;
	int seqno;
	int x;
	int y;
	int dir;

	checksum += util_deint(2, &event);
	checksum += util_deint(3, &seqno);
	checksum += util_deint(4, &x);
	checksum += util_deint(5, &y);
	checksum += util_deint(6, &dir);
	util_deint(7, &checksumrecv);
	if (checksum!=checksumrecv) {
		return -1;
	}
	pf2(fd, event, seqno, x, y, dir);
	return 0;
}

int lssproto_dedata_II(int fd,void* pf)
{
	typedef void (*lsfunc_II)(int,int,int);
	lsfunc_II pf2=(lsfunc_II)pf;
	int checksum=0, checksumrecv;
	int x;
	int y;

	checksum += util_deint(2, &x);
	checksum += util_deint(3, &y);
	util_deint(4, &checksumrecv);
	if (checksum!=checksumrecv) {
		return -1;
	}
	pf2(fd, x, y);
	return 0;
}

int lssproto_dedata_I(int fd,void* pf)
{
	typedef void (*lsfunc_I)(int,int);
	lsfunc_I pf2=(lsfunc_I)pf;
	int checksum=0, checksumrecv;
	int dummy;

	checksum += util_deint(2, &dummy);
	util_deint(3, &checksumrecv);
	if (checksum!=checksumrecv) {
		return -1;
	}
	pf2(fd, dummy);
	return 0;
}

int lssproto_dedata_C(int fd,void* pf)
{
	typedef void (*lsfunc_C)(int,char*);
	lsfunc_C pf2=(lsfunc_C)pf;
	int checksum=0, checksumrecv;
	char *command=malloc(65500);
	if(command==NULL)return -2;
		
	checksum += util_destring(2, command);
	util_deint(3, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(command);
		return -1;
	}		
	pf2(fd, command);
	free(command);
	return 0;
}

int lssproto_dedata_IIII(int fd,void* pf)
{
	typedef void (*lsfunc_IIII)(int,int,int,int,int);
	lsfunc_IIII pf2=(lsfunc_IIII)pf;
	int checksum=0, checksumrecv;
	int x;
	int y;
	int haveitemindex;
	int toindex;

	checksum += util_deint(2, &x);
	checksum += util_deint(3, &y);
	checksum += util_deint(4, &haveitemindex);
	checksum += util_deint(5, &toindex);
	util_deint(6, &checksumrecv);
	if (checksum!=checksumrecv) {
		return -1;
	}
	pf2(fd, x, y, haveitemindex, toindex);
	return 0;
}
int lssproto_dedata_III(int fd,void* pf)
{
	typedef void (*lsfunc_III)(int,int,int,int);
	lsfunc_III pf2=(lsfunc_III)pf;
	int checksum=0, checksumrecv;
	int x;
	int y;
	int dir;

	checksum += util_deint(2, &x);
	checksum += util_deint(3, &y);
	checksum += util_deint(4, &dir);
	util_deint(5, &checksumrecv);
	if (checksum!=checksumrecv) {
		return -1;
	}
	pf2(fd, x, y, dir);
	return 0;
}
int lssproto_dedata_ICI(int fd,void* pf)
{
	typedef void (*lsfunc_ICI)(int,int,char*,int);
	lsfunc_ICI pf2=(lsfunc_ICI)pf;
	int checksum=0, checksumrecv;
	int index;
	char *message=malloc(65500);
	int color;
	if(message==NULL)return -2;
	checksum += util_deint(2, &index);
	checksum += util_destring(3, message);
	checksum += util_deint(4, &color);
	util_deint(5, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(message);
		return -1;
	}
	pf2(fd, index, message, color);
	free(message);
	return 0;
}

int lssproto_dedata_IIICI(int fd,void* pf)
{
	typedef void (*lsfunc_IIICI)(int,int,int,int,char*,int);
	lsfunc_IIICI pf2=(lsfunc_IIICI)pf;
	int checksum=0, checksumrecv;
	int index;
	int petindex;
	int itemindex;
	char *message=malloc(65500);
	int color;
	if(message==NULL)return -2;
	checksum += util_deint(2, &index);
	checksum += util_deint(3, &petindex);
	checksum += util_deint(4, &itemindex);
	checksum += util_destring(5, message);
	checksum += util_deint(6, &color);
	util_deint(7, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(message);
		return -1;
	}
	pf2(fd, index, petindex, itemindex, message, color);
	free(message);
	return 0;
}

int lssproto_dedata_IICII(int fd,void* pf)
{
	typedef void (*lsfunc_IICII)(int,int,int,char*,int,int);
	lsfunc_IICII pf2=(lsfunc_IICII)pf;
	int checksum=0, checksumrecv;
	int x;
	int y;
	char *message=malloc(65500);
	int color;
	int area;
	if(message==NULL)return -2;
	checksum += util_deint(2, &x);
	checksum += util_deint(3, &y);
	checksum += util_destring(4, message);
	checksum += util_deint(5, &color);
	checksum += util_deint(6, &area);
	util_deint(7, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(message);
		return -1;
	}
	pf2(fd, x, y, message, color, area);
	free(message);
	return 0;
}

int lssproto_dedata_IIIC(int fd,void* pf)
{
	typedef void (*lsfunc_IIIC)(int,int,int,int,char*);
	lsfunc_IIIC pf2=(lsfunc_IIIC)pf;
	int checksum=0, checksumrecv;
	int havepetindex;
	int havepetskill;
	int toindex;
	char *data=malloc(65500);
	if(data==NULL)return -2;
	checksum += util_deint(2, &havepetindex);
	checksum += util_deint(3, &havepetskill);
	checksum += util_deint(4, &toindex);
	checksum += util_destring(5, data);
	util_deint(6, &checksumrecv);
	if (checksum!=checksumrecv) {
		return -1;
	}
	pf2(fd, havepetindex, havepetskill, toindex, data);
	return 0;
}

int lssproto_dedata_IC(int fd,void* pf)
{
	typedef void (*lsfunc_IC)(int,int,char*);
	lsfunc_IC pf2=(lsfunc_IC)pf;
	int checksum=0, checksumrecv;
	int havepetindex;
	char *data=malloc(65500);
	if(data==NULL)return -2;
	checksum += util_deint(2, &havepetindex);
	checksum += util_destring(3, data);
	util_deint(4, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(data);
		return -1;
	}
	pf2(fd, havepetindex, data);
	free(data);
	return 0;
}
int lssproto_dedata_IIIIIC(int fd,void* pf)
{
	typedef void (*lsfunc_IIIIIC)(int,int,int,int,int,int,char*);
	lsfunc_IIIIIC pf2=(lsfunc_IIIIIC)pf;
	int checksum=0, checksumrecv;
	int x;
	int y;
	int seqno;
	int objindex;
	int select;
	char *data=malloc(65500);
	
	if(data==NULL)return -2;
		
	checksum += util_deint(2, &x);
	checksum += util_deint(3, &y);
	checksum += util_deint(4, &seqno);
	checksum += util_deint(5, &objindex);
	checksum += util_deint(6, &select);
	checksum += util_destring(7, data);
		
	util_deint(8, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(data);
		return -1;
	}
	pf2(fd, x, y, seqno, objindex, select, data);
	free(data);
	return 0;
}

int lssproto_dedata_CC(int fd,void* pf)
{
	typedef void (*lsfunc_CC)(int,char*,char*);
	lsfunc_CC pf2=(lsfunc_CC)pf;
	int checksum=0, checksumrecv;
	char *cdkey=malloc(65500);
	if(cdkey==NULL)return -3;
		
	char *passwd=malloc(65500);
	if(passwd==NULL){
		free(cdkey);
		return -2;
	}

	checksum += util_destring(2, cdkey);
	checksum += util_destring(3, passwd);
		
	util_deint(4, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(cdkey);
		free(passwd);
		return -1;
	}
	pf2(fd, cdkey, passwd);
	free(cdkey);
	free(passwd);
	return 0;
}

int lssproto_dedata_ICIIIIIIIIIII(int fd,void* pf)
{
	typedef void (*lsfunc_ICIIIIIIIIIII)(int,int,char*,int,int,int,int,int,int,int,int,int,int,int);
	lsfunc_ICIIIIIIIIIII pf2=(lsfunc_ICIIIIIIIIIII)pf;
	int checksum=0, checksumrecv;
	int dataplacenum;
	char *charname=malloc(65500);
	if(charname==NULL)return -2;
	int imgno;
	int faceimgno;
	int vital;
	int str;
	int tgh;
	int dex;
	int earth;
	int water;
	int fire;
	int wind;
	int hometown;

	checksum += util_deint(2, &dataplacenum);
	checksum += util_destring(3, charname);
	checksum += util_deint(4, &imgno);
	checksum += util_deint(5, &faceimgno);
	checksum += util_deint(6, &vital);
	checksum += util_deint(7, &str);
	checksum += util_deint(8, &tgh);
	checksum += util_deint(9, &dex);
	checksum += util_deint(10, &earth);
	checksum += util_deint(11, &water);
	checksum += util_deint(12, &fire);
	checksum += util_deint(13, &wind);
	checksum += util_deint(14, &hometown);
	util_deint(15, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(charname);
		return -1;
	}
	pf2(fd, dataplacenum, charname, imgno, faceimgno, vital, str, tgh, dex, earth, water, fire, wind, hometown);
	free(charname);
	return 0;
}

int lssproto_dedata_CI(int fd,void* pf)
{
	typedef void (*lsfunc_CI)(int,char*,int);
	lsfunc_CI pf2=(lsfunc_CI)pf;
	int checksum=0, checksumrecv;
	char *passwd=malloc(65500);
	if(passwd==NULL)return -2;
	int min;

	checksum += util_destring(2, passwd);
	checksum += util_deint(3, &min);
	util_deint(4, &checksumrecv);
	if (checksum!=checksumrecv) {
		free(passwd);
		return -1;
	}
	pf2(fd, passwd, min);
	free(passwd);
	return 0;
}
#else
#define DME() print("<DME(%d)%d:%d>",fd,__LINE__,func)

int lssproto_ServerDispatchMessage(int fd, char *encoded)
{
	int func,fieldcount;
	char raw[65500];

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	
	util_DecodeMessage(raw,encoded);

	if( !util_SplitMessage(raw,SEPARATOR) ){
		print("\nDME1:package=%s\n",raw);
		DME(); return -1;
	}
	if (!util_GetFunctionFromSlice(&func, &fieldcount)) {
		logHack(fd,HACK_GETFUNCFAIL);
		DME(); return -1;
	}
	
	if (func==LSSPROTO_W_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		char direction[65500];

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_destring(4, direction);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_W_recv(fd, x, y, direction);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_W2_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		char direction[65500];

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_destring(4, direction);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_W2_recv(fd, x, y, direction);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_EV_RECV) {
		int checksum=0, checksumrecv;
		int event;
		int seqno;
		int x;
		int y;
		int dir;

		checksum += util_deint(2, &event);
		checksum += util_deint(3, &seqno);
		checksum += util_deint(4, &x);
		checksum += util_deint(5, &y);
		checksum += util_deint(6, &dir);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_EV_recv(fd, event, seqno, x, y, dir);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DU_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DU_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_EO_RECV) {
		int checksum=0, checksumrecv;
		int dummy;

		checksum += util_deint(2, &dummy);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_EO_recv(fd, dummy);
		util_DiscardMessage();
		return 0;
	}
     
	if (func==LSSPROTO_BU_RECV) {
		int checksum=0, checksumrecv;
		int dummy;

		checksum += util_deint(2, &dummy);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_BU_recv(fd, dummy);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_JB_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_JB_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_LB_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_LB_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_B_RECV) {
		int checksum=0, checksumrecv;
		char command[65500];

		checksum += util_destring(2, command);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
		lssproto_B_recv(fd, command);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SKD_RECV) {
		int checksum=0, checksumrecv;
		int dir;
		int index;

		checksum += util_deint(2, &dir);
		checksum += util_deint(3, &index);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_SKD_recv(fd, dir, index);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_ID_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int haveitemindex;
		int toindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &haveitemindex);
		checksum += util_deint(5, &toindex);
		util_deint(6, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_ID_recv(fd, x, y, haveitemindex, toindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PI_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int dir;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &dir);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_PI_recv(fd, x, y, dir);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DI_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int itemindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &itemindex);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DI_recv(fd, x, y, itemindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DG_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int amount;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &amount);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DG_recv(fd, x, y, amount);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DP_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int petindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &petindex);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DP_recv(fd, x, y, petindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_MI_RECV) {
		int checksum=0, checksumrecv;
		int fromindex;
		int toindex;

		checksum += util_deint(2, &fromindex);
		checksum += util_deint(3, &toindex);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_MI_recv(fd, fromindex, toindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_MSG_RECV) {
		int checksum=0, checksumrecv;
		int index;
		char message[65500];
		int color;

		checksum += util_deint(2, &index);
		checksum += util_destring(3, message);
		checksum += util_deint(4, &color);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_MSG_recv(fd, index, message, color);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PMSG_RECV) {
		int checksum=0, checksumrecv;
		int index;
		int petindex;
		int itemindex;
		char message[65500];
		int color;

		checksum += util_deint(2, &index);
		checksum += util_deint(3, &petindex);
		checksum += util_deint(4, &itemindex);
		checksum += util_destring(5, message);
		checksum += util_deint(6, &color);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_PMSG_recv(fd, index, petindex, itemindex, message, color);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_AB_RECV) {
		int checksum=0, checksumrecv;
		util_deint(2, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_AB_recv(fd);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DAB_RECV) {
		int checksum=0, checksumrecv;
		int index;

		checksum += util_deint(2, &index);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DAB_recv(fd, index);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_AAB_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_AAB_recv(fd, x, y);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_L_RECV) {
		int checksum=0, checksumrecv;
		int dir;

		checksum += util_deint(2, &dir);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_L_recv(fd, dir);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_TK_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		char message[65500];
		int color;
		int area;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_destring(4, message);
		checksum += util_deint(5, &color);
		checksum += util_deint(6, &area);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_TK_recv(fd, x, y, message, color, area);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_M_RECV) {
		int checksum=0, checksumrecv;
		int fl;
		int x1;
		int y1;
		int x2;
		int y2;

		checksum += util_deint(2, &fl);
		checksum += util_deint(3, &x1);
		checksum += util_deint(4, &y1);
		checksum += util_deint(5, &x2);
		checksum += util_deint(6, &y2);
		util_deint(7, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_M_recv(fd, fl, x1, y1, x2, y2);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_C_RECV) {
		int checksum=0, checksumrecv;
		int index;

		checksum += util_deint(2, &index);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_C_recv(fd, index);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_S_RECV) {
		int checksum=0, checksumrecv;
		char category[65500];

		checksum += util_destring(2, category);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_S_recv(fd, category);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_FS_RECV) {
		int checksum=0, checksumrecv;
		int flg;

		checksum += util_deint(2, &flg);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_FS_recv(fd, flg);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_HL_RECV) {
		int checksum=0, checksumrecv;
		int flg;

		checksum += util_deint(2, &flg);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_HL_recv(fd, flg);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PR_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int request;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &request);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_PR_recv(fd, x, y, request);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_KS_RECV) {
		int checksum=0, checksumrecv;
		int petarray;
		checksum += util_deint(2, &petarray);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_KS_recv(fd, petarray);
		util_DiscardMessage();
		return 0;
	}

#ifdef _STANDBYPET
	if (func==LSSPROTO_SPET_RECV) {
		int checksum=0, checksumrecv;
		int standbypet;
		checksum += util_deint(2, &standbypet);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_SPET_recv(fd, standbypet);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _RIGHTCLICK
	if (func==LSSPROTO_RCLICK_RECV) {
		int checksum=0, checksumrecv;
		int type;
		char data[1024];
		checksum += util_deint(2, &type);
		checksum += util_destring(3, data);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_RCLICK_recv(fd, type, data);
		util_DiscardMessage();
		return 0;
	}
#endif

	if (func==LSSPROTO_AC_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int actionno;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &actionno);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_AC_recv(fd, x, y, actionno);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_MU_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int array;
		int toindex;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &array);
		checksum += util_deint(5, &toindex);
		util_deint(6, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_MU_recv(fd, x, y, array, toindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PS_RECV) {
		int checksum=0, checksumrecv;
		int havepetindex;
		int havepetskill;
		int toindex;
		char data[65500];

		checksum += util_deint(2, &havepetindex);
		checksum += util_deint(3, &havepetskill);
		checksum += util_deint(4, &toindex);
		checksum += util_destring(5, data);
		util_deint(6, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_PS_recv(fd, havepetindex, havepetskill, toindex, data);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_ST_RECV) {
		int checksum=0, checksumrecv;
		int titleindex;

		checksum += util_deint(2, &titleindex);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_ST_recv(fd, titleindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_DT_RECV) {
		int checksum=0, checksumrecv;
		int titleindex;

		checksum += util_deint(2, &titleindex);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_DT_recv(fd, titleindex);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_FT_RECV) {
		int checksum=0, checksumrecv;
		char data[65500];

		checksum += util_destring(2, data);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_FT_recv(fd, data);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SKUP_RECV) {
		int checksum=0, checksumrecv;
		int skillid;

		checksum += util_deint(2, &skillid);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_SKUP_recv(fd, skillid);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_KN_RECV) {
		int checksum=0, checksumrecv;
		int havepetindex;
		char data[65500];

		checksum += util_deint(2, &havepetindex);
		checksum += util_destring(3, data);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_KN_recv(fd, havepetindex, data);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_WN_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int seqno;
		int objindex;
		int select;
		char data[65500];

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &seqno);
		checksum += util_deint(5, &objindex);
		checksum += util_deint(6, &select);
		checksum += util_destring(7, data);
		
		util_deint(8, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_WN_recv(fd, x, y, seqno, objindex, select, data);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SP_RECV) {
		int checksum=0, checksumrecv;
		int x;
		int y;
		int dir;

		checksum += util_deint(2, &x);
		checksum += util_deint(3, &y);
		checksum += util_deint(4, &dir);
		util_deint(5, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_SP_recv(fd, x, y, dir);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CLIENTLOGIN_RECV) {
		int checksum=0, checksumrecv;
		char cdkey[65500];
		char passwd[65500];
		
		strcpy(PersonalKey, _DEFAULT_PKEY);

		checksum += util_destring(2, cdkey);
		checksum += util_destring(3, passwd);
		
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		
		lssproto_ClientLogin_recv(fd, cdkey, passwd);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CREATENEWCHAR_RECV) {
		int checksum=0, checksumrecv;
		int dataplacenum;
		char charname[65500];
		int imgno;
		int faceimgno;
		int vital;
		int str;
		int tgh;
		int dex;
		int earth;
		int water;
		int fire;
		int wind;
		int hometown;

		checksum += util_deint(2, &dataplacenum);
		checksum += util_destring(3, charname);
		checksum += util_deint(4, &imgno);
		checksum += util_deint(5, &faceimgno);
		checksum += util_deint(6, &vital);
		checksum += util_deint(7, &str);
		checksum += util_deint(8, &tgh);
		checksum += util_deint(9, &dex);
		checksum += util_deint(10, &earth);
		checksum += util_deint(11, &water);
		checksum += util_deint(12, &fire);
		checksum += util_deint(13, &wind);
		checksum += util_deint(14, &hometown);
		util_deint(15, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_CreateNewChar_recv(fd, dataplacenum, charname, imgno, faceimgno, vital, str, tgh, dex, earth, water, fire, wind, hometown);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CHARDELETE_RECV) {
		int checksum=0, checksumrecv;
		char charname[65500];

		checksum += util_destring(2, charname);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_CharDelete_recv(fd, charname);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CHARLOGIN_RECV) {
		int checksum=0, checksumrecv;
		char charname[65500];

		checksum += util_destring(2, charname);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_CharLogin_recv(fd, charname);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CHARLIST_RECV) {
		int checksum=0, checksumrecv;
#ifdef _PKSEVER_VER
		int star = 0;
		util_deint(2, &checksumrecv);
		util_deint(3, &star);
#else
		util_deint(2, &checksumrecv);
#endif
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}

//#ifdef _PKSEVER_VER
//		lssproto_CharList_recv( fd, star);
//#else
		lssproto_CharList_recv( fd);
//#endif

		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_CHARLOGOUT_RECV) {
		int checksum=0, checksumrecv;
		int Flg=1;
#ifdef _CHAR_NEWLOGOUT
		checksum += util_deint(2, &Flg);
		util_deint(3, &checksumrecv);
#else
		util_deint(2, &checksumrecv);
#endif
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_CharLogout_recv(fd, Flg);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PROCGET_RECV) {
		int checksum=0, checksumrecv;
		
		strcpy( PersonalKey, _DEFAULT_PKEY);

		util_deint(2, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_ProcGet_recv(fd);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_PLAYERNUMGET_RECV) {
		int checksum=0, checksumrecv;
		util_deint(2, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_PlayerNumGet_recv(fd);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_ECHO_RECV) {
		int checksum=0, checksumrecv;
		char test[65500];

		checksum += util_destring(2, test);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_Echo_recv(fd, test);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_SHUTDOWN_RECV) {
		int checksum=0, checksumrecv;
		char passwd[65500];
		int min;

		checksum += util_destring(2, passwd);
		checksum += util_deint(3, &min);
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_Shutdown_recv(fd, passwd, min);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_TD_RECV) {
		int checksum=0, checksumrecv;
		char message[65500];

		checksum += util_destring(2, message);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_TD_recv(fd, message);
		util_DiscardMessage();
		return 0;
	}

	if (func==LSSPROTO_FM_RECV) {
		int checksum=0, checksumrecv;
		char message[65500];

		checksum += util_destring(2, message);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_FM_recv(fd, message);
		util_DiscardMessage();
		return 0;
	}
    
	if (func==LSSPROTO_PETST_RECV) {
		int checksum=0, checksumrecv;
		int nPet;
		int sPet;		

		checksum += util_deint(2, &nPet);
		checksum += util_deint(3, &sPet);		
		util_deint(4, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
		lssproto_PETST_recv(fd, nPet, sPet);		
		util_DiscardMessage();
		return 0;
	}
    
	// _BLACK_MARKET
	if (func==LSSPROTO_BM_RECV) {
		int checksum=0, checksumrecv;
		int iindex;		

		checksum += util_deint(2, &iindex);		
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
		lssproto_BM_recv(fd, iindex);		
		util_DiscardMessage();
		return 0;
	}

#ifdef _FIX_DEL_MAP           // WON ADD ���a��a�ϰe�ʺ�
	if (func==LSSPROTO_DM_RECV) {
	    char buffer[2];
	    buffer[0] = '\0';
		lssproto_DM_recv( fd );
		util_DiscardMessage();
		return 0;
	}

#endif

#ifdef _CHECK_GAMESPEED
	if (func==LSSPROTO_CS_RECV) {
	    char buffer[2];
	    buffer[0] = '\0';
		lssproto_CS_recv( fd );
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _TEAM_KICKPARTY
	if ( func == LSSPROTO_KTEAM_RECV ) {
		int checksum = 0, checksumrecv;
		int sindex;
		checksum += util_deint( 2, &sindex);
		util_deint( 3, &checksumrecv);
		if(checksum!=checksumrecv){
			util_DiscardMessage();
			logHack( fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_KTEAM_recv( fd, sindex);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _MIND_ICON
	if(func==LSSPROTO_MA_RECV){
		int checksum = 0, checksumrecv;
		int nMind;
		int x, y;
		
		checksum += util_deint( 2, &nMind);
		checksum += util_deint( 3, &x);
		checksum += util_deint( 4, &y);
		util_deint( 5, &checksumrecv);
		if(checksum!=checksumrecv){
			util_DiscardMessage();
			logHack( fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
		lssproto_MA_recv(fd, x, y, nMind);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _CHATROOMPROTOCOL			// (���i�}) Syu ADD ��ѫ��W�D
	if (func==LSSPROTO_CHATROOM_RECV) {
		int checksum=0, checksumrecv;
		char test[65500];

		checksum += util_destring(2, test);
		util_deint(3, &checksumrecv);
		if (checksum!=checksumrecv) {
			util_DiscardMessage();
			logHack(fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_CHATROOM_recv(fd, test);
		util_DiscardMessage();
		return 0;
	}
#endif

#ifdef _NEWREQUESTPROTOCOL			// (���i�}) Syu ADD �s�WProtocol�n�D�Ӷ�
	if ( func==LSSPROTO_RESIST_RECV ) {
	    char buffer[2];
	    buffer[0] = '\0';
		lssproto_RESIST_recv( fd );
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _OUTOFBATTLESKILL			// (���i�}) Syu ADD �D�԰��ɧޯ�Protocol
	if(func==LSSPROTO_BATTLESKILL_RECV){
		int checksum = 0, checksumrecv;
		int iNum;
		
		checksum += util_deint( 2, &iNum);
		util_deint( 3, &checksumrecv);
		if(checksum!=checksumrecv){
			util_DiscardMessage();
			logHack( fd, HACK_CHECKSUMERROR);
			DME(); return -1;
		}		
		lssproto_BATTLESKILL_recv(fd, iNum);
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _STREET_VENDOR
	if(func == LSSPROTO_STREET_VENDOR_RECV){
		int checksum = 0,checksumrecv;
		char message[65500];

		checksum += util_destring(2,message);
		util_deint(3,&checksumrecv);
		if(checksum != checksumrecv){
			util_DiscardMessage();
			logHack(fd,HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_STREET_VENDOR_recv(fd,message);
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _JOBDAILY
	if(func == LSSPROTO_JOBDAILY_RECV){
		int checksum = 0,checksumrecv;
		char buffer[16384];

		buffer[0] = '\0';
		memset(buffer,0,16384); //kkkkkkkkk
		checksum += util_destring(2,buffer);
		util_deint(3,&checksumrecv);
		if(checksum != checksumrecv){
			util_DiscardMessage();
			logHack(fd,HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		print("\njobdaily:%s ",buffer);
		lssproto_JOBDAILY_recv(fd,buffer);
		util_DiscardMessage();
		return 0;

	}
#endif
#ifdef _TEACHER_SYSTEM
	if(func == LSSPROTO_TEACHER_SYSTEM_RECV){
		int checksum = 0,checksumrecv;
		char message[65500];

		checksum += util_destring(2,message);
		util_deint(3,&checksumrecv);
		if(checksum != checksumrecv){
			util_DiscardMessage();
			logHack(fd,HACK_CHECKSUMERROR);
			DME(); return -1;
		}
		lssproto_TEACHER_SYSTEM_recv(fd,message);
		util_DiscardMessage();
		return 0;
	}
#endif
#ifdef _ADD_STATUS_2
	if(func == LSSPROTO_S2_RECV){
		return 0;
	}
#endif

	util_DiscardMessage();
	logHack(fd,HACK_NOTDISPATCHED);
	DME(); return -1;
}
#endif
/*
   servertoclient XYD( int x, int y, int dir );
 	�����  ��    �˥ץ쥤�䡼�ΰ�  ��  Ĵ�����뤿��˻Ȥ��e
*/
void lssproto_XYD_send(int fd,int x,int y,int dir)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	checksum += util_mkint(buffer, dir);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_XYD_SEND, buffer);
}

void lssproto_EV_send(int fd,int seqno,int result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, seqno);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_EV_SEND, buffer);
}

void lssproto_EN_send(int fd,int result,int field)
{
	char buffer[65500];
	int checksum=0;

	//print(" EN_send ");

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, result);
	checksum += util_mkint(buffer, field);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_EN_SEND, buffer);
}


void lssproto_RS_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RS_SEND, buffer);
}


void lssproto_RD_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RD_SEND, buffer);
}

void lssproto_B_send(int fd,char* command)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, command);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_B_SEND, buffer);
}

void lssproto_I_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_I_SEND, buffer);
}

void lssproto_SI_send(int fd,int fromindex,int toindex)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, fromindex);
	checksum += util_mkint(buffer, toindex);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SI_SEND, buffer);
}

void lssproto_MSG_send(int fd,int aindex,char* text,int color)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, aindex);
	checksum += util_mkstring(buffer, text);
	checksum += util_mkint(buffer, color);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_MSG_SEND, buffer);
}


void lssproto_PME_send(int fd,int objindex,int graphicsno,int x,int y,int dir,int flg,int no,char* cdata)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, objindex);
	checksum += util_mkint(buffer, graphicsno);
	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	checksum += util_mkint(buffer, dir);
	checksum += util_mkint(buffer, flg);
	checksum += util_mkint(buffer, no);
	checksum += util_mkstring(buffer, cdata);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PME_SEND, buffer);
}


void lssproto_AB_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_AB_SEND, buffer);
}


void lssproto_ABI_send(int fd,int num,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, num);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_ABI_SEND, buffer);
}

void lssproto_TK_send(int fd,int index,char* message,int color)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, index);
	checksum += util_mkstring(buffer, message);
	checksum += util_mkint(buffer, color);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_TK_SEND, buffer);
}

void lssproto_MC_send(int fd,int fl,int x1,int y1,int x2,int y2,int tilesum,int objsum,int eventsum,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, fl);
	checksum += util_mkint(buffer, x1);
	checksum += util_mkint(buffer, y1);
	checksum += util_mkint(buffer, x2);
	checksum += util_mkint(buffer, y2);
	checksum += util_mkint(buffer, tilesum);
	checksum += util_mkint(buffer, objsum);
	checksum += util_mkint(buffer, eventsum);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_MC_SEND, buffer);
}


void lssproto_M_send(int fd,int fl,int x1,int y1,int x2,int y2,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, fl);
	checksum += util_mkint(buffer, x1);
	checksum += util_mkint(buffer, y1);
	checksum += util_mkint(buffer, x2);
	checksum += util_mkint(buffer, y2);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_M_SEND, buffer);
}

/*
   servertoclient C( string data );
       (Characters)
       ���饤����Ȥ� CA ��  �ơb���Υ����ˤĤ���Ĵ�ʤ��ä����ϡb C 
       �Ǥ��δؿ����׵�Ǥ���e�����Фϥ����å����ʤ��e
       
 	string data
 		  �ե����ޥåȤ� �ʲ���3��  �ι�  �򥳥�ޤǤʤ�٤�
 	    ��ΤǤ���.���줾��ι�  ��  �ȤϤ���ˤ��ƫ�'|'�Ǥ������
 	    �Ƥ���e
 	    
 	      ���ƫ�Ǥ�����줿�ȡ�����12�Ĥξ��
 		  �ȡ������    ��
 
 		  WHICHTYPE|CHARINDEX|X|Y|DIR|BASEIMG|LEVEL|NAMECOLOR|NAME|SELFTITLE|WALKABLE|HEIGHT|POPUPNAMECOLOR
 		  ���Υ��֥������Ȥϥ���饯�����Ǥ���Ȥ������ȤǤ���e
 		  
 		  WHICHTYPE �ϡf���Υ���饯�������ɤ����ä���  �Τ��
 		  �Ǥ��뤫�e����ϥ����С��Ǥϰʲ��Τ褦���������Ƥ���e
 		  
 		typedef enum
 		{
 		    CHAR_TYPENONE,          ���Ǥ�ʤ�
 		    CHAR_TYPEPLAYER,        �ץ쥤�䡼
 		    CHAR_TYPEENEMY,         Ũ
 		    CHAR_TYPEPET,           �ڥå�
 		    CHAR_TYPEDOOR,          �ɥ�
 		    CHAR_TYPEBOX ,              
 		    CHAR_TYPEMSG ,          ��  
 		    CHAR_TYPEWARP ,         ��ץ�����
 		    CHAR_TYPESHOP ,         Ź
 		    CHAR_TYPEHEALER ,       �ҡ��顼
 		    CHAR_TYPEOLDMAN ,       Ĺ  
 		    CHAR_TYPEROOMADMIN,     ��  ����
 		    CHAR_TYPETOWNPEOPLE,    �ޤ��ΤҤ�
 		    CHAR_TYPEDENGON,        ����  
 		    CHAR_TYPEADM,           ����  
 		    CHAR_TYPETEMPLE,        Temple master
 		    CHAR_TYPESTORYTELLER,   ���  
 		    CHAR_TYPERANKING,       ��  ����󥭥�  ��      
 		    CHAR_TYPEOTHERNPC,      ����¾�θ����оݤˤʤ�ʤ�NPC
 		    CHAR_TYPEPRINTPASSMAN,  �ɥ��Υѥ����  ������NPC
 		    CHAR_TYPENPCENEMY,      ����Ũ
 		    CHAR_TYPEACTION,        ����������  ������NPC
 		    CHAR_TYPEWINDOWMAN,     ������ɥ�  ������NPC    ���Ȥ���)
 		    CHAR_TYPESAVEPOINT,     �����֥ݥ����
 		    CHAR_TYPEWINDOWHEALER,  ������ɥ������פΥҡ��顼
 		    CHAR_TYPEITEMSHOP,	    ��Ź
 		    CHAR_TYPESTONESHOP,	    ��  ��  �ڥåȤ�  ��  
 		    CHAR_TYPEDUELRANKING,   DUEL��󥭥�NPC
 		    CHAR_TYPEWARPMAN,	    ��ץޥ�NPC
 		    CHAR_TYPEEVENT,	    ���٥��NPC
 		    CHAR_TYPEMIC,	    ���٥��NPC
 		    CHAR_TYPELUCKYMAN,	    ���٥��NPC
 		    CHAR_TYPEBUS,	    �ޥ�⥹�Х�
 		    CHAR_TYPECHARM,	    ���٥��NPC
 		    CHAR_TYPENUM,
 		}CHAR_TYPE;
 		  
 		  �����¾�Υץ쥤�䡼��NPC��Ũ�Ǥ���e�����С�������
 		  ����Ȥ��ϸ�����  �ϤˤĤ�����  ��������e�Ĥ�
 		  �ꥯ�饤����Ȥϡb���Υѥ��åȤ�����Ȥä��Ȥ��ˤ���
 		  �ѥ��åȤ˽񤫤�Ƥ��ʤ�������  �äƤ�����ä��Ƥ�
 		  �ޤäƤ褤�Ȥ������ȤǤ���e�ޤ��b�ޥ������������
 		  �碌���Ȥ���  �������  �Ϥ��ξ�  �Τߤ˴𤤤Ƥ���e
 		  ������b�ޥ�����������򤢤碌���Ȥ���  �����뤳�Ȥ�
 		    �����줿���ϡb�����С��ϡb���δؿ���    Ū�˸Ƥ�
 		  �����ʤ���Фʤ�ʤ��e SELFTITLE�ˤĤ��Ƥϡb�ǥ�ߥ�
 		  �Ǥ��뤿�ƫ��դ��ޤʤ��褦�˥��������פ���Ƥ���e
 		  ���饤����Ȥϥ��������Υ��ޥ��(CA)������ޤǤ�  
 		  ����  ������eCHARINDEX�����С�  �ΰ�դ˥�����  
 		  ��Ǥ���  ��bBASEIMG��  ���Τ����  ��bLEVEL�ϥ���
 		  ��Υ�٥�(0�ʤ�  �����ʤ��e�����ͤ�NPC�ʤɤ˻Ȥ��e)
 		  WALKABLE��1�ΤȤ����ξ���̲᤹�뤳�Ȥ��Ǥ��b0�ʤ���
 		  �᤹�뤳�Ȥ��Ǥ��ʤ��eHEIGHT�Ϲ⤵���Ĥ�Τ�������
 		  �ʤ��Τ��λ���e
 		  
 		  ����饯������  ���ȼ�  �ι�ϡb<a href="#escaping">
 		  ���������פ���ʤ���Фʤ�ʤ��e'|'�ǥȡ������
 		  �������Ƥ��饨�������פ�������e���������פ����'
 		  |'���ۤ���  ���˶��������Τǡb  ���ñ���'|'���
 		  ��ߥ��Ȥ��Ƥ褤�e��������  ��b  ���ȼ�  �ι�򥨥�
                   �����פ��Ƥ��餿�ƫ�ǤĤʤ��Ǥ�����������e
                   �ޤ��f�ڥåȤξ��ϼ�  �ι������˥桼����������
                   �����ڥåȤ�  ������������Ƥ���e
 		  
 	      ���ƫ�Ǥ�����줿�ȡ�����6�Ĥξ��
 		  �ȡ������    ��
 		  INDEX|X|Y|BASEIMG|LEVEL|ITEM1LINEINFO
 		  �ǼM  ��  ���Ƥ��륢��  ��ˤĤ��Ƥξ�  �Ǥ���e
 		  INDEX�ϥ����Υ���ǥ��å��Ȥ��֤�ʤ�INDEX�Ǥ���e
 		  ���Υ���  ���ä����˻��Ѥ���eX,Y�ϥ���  ��Υ��ҡ�
 		  �Х��  �eBASEIMG��  ����  ��eITEM1LINEINFO��1��
 		  info��  �����뤿��ξ�  �Ǥ���e����  �०����ɥ�  
 		  ��  ���Ѥξ�  ��  ��  ˡ���Ѱդ���e����  ��˴ؤ���
 		  ��CA��  �ʤ��eITEM1LINEINFO��<a href="#escape">��������
 		  �פ����e���Υ��������פ�  ˡ�Ͼ�ι�  �򻲾ȡe
 
 	      ���ƫ�Ǥ�����줿�ȡ�����4�Ĥξ��
 		  �ȡ������    ��
 		  INDEX|X|Y|VALUE
 		  �ǼM  ��  ���Ƥ��붳��ˤĤ��Ƥξ�  �Ǥ���e    �Ϥ�
 		  �٤ƿ����eINDEX,X,Y �ϥ���  ���  ���eVALUE �Ϥɤ��
 		  ���ά����Ȥ������Ǥ���e����  ��ˤĤ��Ƥξ�  �Ǥ���e
 	      ���ƫ�Ǥ�����줿�ȡ�����1�Ĥξ��
 		  INDEX
 		  ���Υ�����C�϶������ʤ��e
*/
void lssproto_C_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_C_SEND, buffer);
}

/*
   servertoclient CA( string data );
       (CharacterAction)
       ������  �Ϥˤ��륭���Υ����������֤򹹿�����e
       �����С����饯�饤����Ȥ˰�  Ū����������e
       �ƥ�����1��������󤴤Ȥ������e�����С��ϥ���������Ǥ����
       �����̤��뤳�ȡe
       
 	string data
 	CHARINDEX|X|Y|ACTION|PARAM1|PARAM2|PARAM3|PARAM4|....��
 	    ��ޤǤ����å���Τˤ���ePARAM�ϥ�������󤴤Ȥ˸Ŀ���
 	    �Ȥ������⤳�Ȥʤ�e�ʲ��ϥ���������  �eX,Y�ϰ�  �Ǥ�
 	    �Υ��������Ǥ�b�����餷����  ����ꤹ��e
 	    ����  ��  �ϥ��������פ���ʤ��e
 	  
 	        ACTION  PARAM1  PARAM2 
 	       PARAM3  PARAM4  
 	        Stand:0    ��0~7        
 	        Walk:1    ��0~7        
 	        Attack:2    ��0~7        
 	        Throw:3    ��0~7        
 	        Damage:4    ��0~7      
 	        Dead:5    ��0~7        
 	        UseMagic:6    ��0~7  
 	        UseItem:7    ��0~7  
 	        Effect:8    ��0~7  ���ե�����  ��  
 	        Down:10 (�ݤ��)    ��0~7  
 	        Sit:11 (�¤�)    ��0~7  
 	        Hand:12 (��򿶤�)    ��0~7  
 	        Pleasure:13 (���)    ��0~7  
 	        Angry:14 (�ܤ�)    ��0~7  
 	        Sad:15 (  ����)    ��0~7  
 	        Guard:16 (������)    ��0~7  
 	        actionwalk:17 (����������Ѭ���)    ��0~7  
 	        nod:18 (���ʤ���)    ��0~7  
 	        actionstand:19 (�����������  ���ݡ��N)    ��0~7  
 	        Battle:20 (��  ��  )    ��0~7  BattleNo(-1 �ʤ�  ���ä�    SideNo  HelpNo  ���ʤ������Ƥ�CA  ���f���ʤ�ä��f�ޤ���  ��    
 	        Leader:21 (�꡼������  )    ��0~7  0:  ���ä� 1:  ��  
 	        Watch:22 (��  ����)    ��0~7  0:  ���ä� 1:  ��  
 	        namecolor:23(  ���ο���  )    ��0~7    ���ο�  ��  
 	        Turn:30(  ��  ��)    ��0~7    
 	        Warp:31(���)    ��0~7    
 	      
 	ACTION���ͤ������ǡb    �Ͼ��  �κ�ü�ι�  �Ǥ���e
*/
void lssproto_CA_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CA_SEND, buffer);
}

/*
   servertoclient CD( string data );
       (CharacterDelete)
 	data �ϥǥ�ߥ�  ,  �Ƕ��ڤ�줿����ǥå����e
 	  �������������  ��e
       ����ID��  �äƤ��륭��饯�����ä������˥����Ф��饯�饤����Ȥ�
       �������e
*/
void lssproto_CD_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CD_SEND, buffer);
}

/*
   <a name="R">servertoclient R( string data );
       (Radar)
       �졼������    �򥯥饤����Ȥ���������e���饤����ȤϤ���������
       �׵᤹�뤳�ȤϤʤ��e�����С���Ŭ���ʥ����ߥ󥰤���������e
       ���Ȥ���10�����뤯���ȤȤ��b1ʬ���ȤȤ��e
 
 	string data
 	x ,y, kind ,�ν���ͤ�'|'�Ǥ����äƤʤ�٥���Τ�
 	    �����'|'�Ǥʤ�٥���Ρe
 	    x,y�ϥ���餫������а�  �ekind�ϰʲ��ˤ��᤹���ͤǤϤʤ�
 	    ����e�졼�����ˤ��Ĥ�  �μ�  ����ꤹ��e�����Ƥ�����
 	    �μ�  �ϡb����ޤǤ˾�����  ˡ�Ȥ��b�������  �����b�����
 	    �����С����ͤ���e
 	 
 	        kind����        
 	        E  Ũ  
 	        P  �ץ쥤�䡼  
 	        S  ��  
 	        G  ����  
 	        I  ����  ��(���ʳ�)  
   
   data��  �u"12|22|E|13|24|P|14|28|P"
   ����  ��  �ϥ��������פ���ʤ��e
*/
void lssproto_R_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_R_SEND, buffer);
}

/*
   servertoclient S( string data );
       (Status)
       �����Υ�  ����������������e
       �ǡ����� ��  ���국��  ��(��  ��)     �ȤʤäƤ���e�Ĥޤ�  ���
       1  ���򸫤�в��Υ�  ��������ʬ��e    ��2  ��  ����Ǥ���e
           �ϰʲ��Υե����ޥåȤˤ��������e���ƫ൭��'|'���ǥ�ߥ��Ǥ���e
       2��  �ʹߤΥȡ�����    �Ǥ���e
       
 	P ���ѥ�᡼��
 	    kubun hp maxhp mp maxmp str tough exp maxexp exp 
 	    level attackpower deffencepower 
 	    fixdex fixcharm fixluck fixfireat fixwaterat fixearthat fixwindat
 	    gold �դ��Ƥ���
 	    �ι��index   �� ���ʾι� 
 
 	    �ǥ�ߥ��� '|' �Ǥ���e�ޤ�  ���ȼ��ʾι�ϡb
 	    <a href="#escaping">���������פ�����Τ�
 	    ���ƫ�ǤĤʹ�����Τ��̿������e
 	    ���줾����ͤη��ϰʲ��e
 	    kubun �ˣ���  �äƤ�������ѥ�᡼���������f
 	    ���ʾ���ȡf�ƥӥåȤ�  �äƤ����ΤΥѥ�᡼������  ��  �ä���������Ƥ���e
 	     ���Сf����  ����hp��Maxhp��kubun �ʹߤΥȡ������  ����Ƥ���e
 	    
 	        kunun  int  
 	        Hp(    )hp  int  
 	        MaxHp  int  
 	        Mp(���ѥѥ)  int  
 	        MaxMp  int  
 	        Vital(    )  int  
 	        Str(    )  int  
 	        Tough(���פ�)  int  
 	        Dex(���ᤵ)  int  
 	        Exp(�и���)exp  int  
 	        MaxExp(���ޤǤηи���)  int  
 	        Level(��٥�)  int  
 	        Attack(  ��  )  int  
      	        Defense(��    )  int  
 	        fQuick(  �����ᤵ)  int  
 	        fCharm(    )  int  
 	        fLuck(��)  int  
 	        fEarth(�M)fixearthat  int  
 	        fWater(  )  int  
 	        fFire(��)  int  
 	        fWid(��)fixwindat  int  
 	        Gold(����)  int  
 	        �ι��index  int  
 	        �ǥ奨��ݥ����  int  
 	        ž�����  int  
 	          ��    ��    
 	        ���ʾι�    ��    
 	    
 	    ��    ( char ��    ���Τޤ޽� )
 	    
 	    P10|20|10|20|10|10|10|1|2|13|13|1|100|10|�ؤ��|abc|def
 	
 	C ��  
 	    floor maxx maxy x y
 	    ��  | �Ƕ��ڤä�����e
 	    ��    ( char ��    ���Τޤ޽� )
 	    
 	    C1024|100|100|10|20
 	    
 	    �嵭��  �Ǥϡb�դҥ�ID1024, �����N100x100 X 10,Y 20�ΤȤ�
 	    ��˥���饯����������Ȥ���    �ˤʤ�e���Υޥ������Υ�
 	    �ҥ���դ��᤿��  �򥯥饤����Ȥ��Τ뤳�Ȥ��Ǥ���ΤϤ���
 	    ���ޥ�ɤΤߤǤ���e�����η�̤ϥդҥ�  �ΰ�  �����狼���
 	    ���e��äơb�����೫�ϻ��b���ʻ��ѤΤȤ��b��פΤȤ��ʤ�
 	    �ϡb���Υ��ޥ�ɤ�Ĥ��ä��������ͤ��������ʤ��ƤϤʤ�ʤ��e
 	    
 	I ������  ��
 	    (����  ��1)|(����  ��2)...(����  ��n)
 	    ����  ���    �ϰʲ���  ˡ�ǥѥå�����e
 	    �ҤȤĤΥ���  ��ϡb���ʤ餺�ʲ���6�ĤΥȡ�����Υ��åȤ�
 	    ���������e�ҤȤĤҤȤĤ�    �ϡb
 	    
 	      ��|  ��2|��  ����|����  ���    |���˥�  ��|���Ѳ�  ���|�����о�|����  ���٥�|�ե饰  
 	    
 	      ����  ��  �ϼ�  ��٥�Ǽ�  Ū��  �������e
 	        ��2�ϼ�  ��٥�ˤ�äơb����  ���    ��  ��  ��e
 		  ���饤����ȤΥ���  ��  ��2��  ��  �����e
 	      ��  ������  ���ο����ؤ�Τ˻Ȥ��e
 	      ����  ���    �e
 	      ���˥�  ��ϡb  ��  ��e
 	      ���Ѳ�  ���Ȥϡf���Υ���  �ब�ɤ��ǻ��Ѥ��������  ����  �äƤ���e
 	          ����ϡf�����С��Ǥϰʲ��Τ褦���������Ƥ���e
 	          
 			typedef enum
 			{
 				ITEM_FIELD_ALL,			���٤Ƥξ��ǻȤ���
 				ITEM_FIELD_BATTLE,		��    �Τ�
 				ITEM_FIELD_MAP,			�̾�ޥå׾�Τ�
 
 			}ITEM_FIELDTYPE;
 	          
 	      �����оݤȤϡf���Υ���  �����Ѥ��뤳�Ȥν�  ���оݤ�  �äƤ���e
 	          �����С��Ǥϰʲ��Τ褦���������Ƥ���e
 	          
 			typedef enum
 			{
 				ITEM_TARGET_MYSELF,		��ʬ�Τ�
 				ITEM_TARGET_OTHER,		¾�ο�  ��ʬ�ޤ�)
 				ITEM_TARGET_ALLMYSIDE,		    ��  
 				ITEM_TARGET_ALLOTHERSIDE,	���¦��  
 				ITEM_TARGET_ALL,		����
 			}ITEM_TARGETTYPE;
 		
   	        <font size=+1>���ο�����100��­���ȡf���Ǥ���Ԥ��оݤȤʤ�e
 	      ����  ���٥�e���Υ�٥�ʾ�μԤǤʤ�����  ��  �ʤ��e
 	      �ե饰  �e����ʥե饰����������e�ƥӥåȤ��б��ϰʲ����̤�e

 		      0bit    �ڥåȥ᡼���������  �ʥ���  �फ�ɤ����e������  ���ȣ��e�����������Ǥ��ʤ��e  
 		      1Bit    ���Υ���  �ब�穨��  �뤫�ɤ����e  
 		      2Bit    ���Υ���  �ब    ���ɤ����e1����    �Ǥ���e  
 	    
 	    ������  �Ǥ�<a href="#escaping">���������פ���e
 	    ��    ( char ��    ���Τޤ޽� )
 	    
 	    I���� �� ��|str+2|1|����\|����|10|2|0
 	    
 	    ��  �������ͤΰ�  �ϡb�����ͤ�
 	    
 	        1  ��  ����A  
 	        2  ��  ����B  	      
 	        3  ��  ����C  
 	        4  ��  ����D  
 	    
 	    �Ȥ���e�ޤ��b����  ��  �ζ���  ʬ�ˤĤ��Ƥϡb5�ĤΥȡ�����
 	    ���ǡb�Ĥޤꤿ�ƫब�ʤ������֤���������Ƥ���e
 	
 	S ��������
 	    (������0)|(������1)|(������2) ... (������n)
 	    �ι��  ���ǡb����餬��Ƥ�    �������ʬ����������e�Ĥ�
 	    ��ʤ���Τ� ||| �Ȥʤ�ΤǤ���e
 	    �ƥ���ȥ��    �ϡb
 	    ������μ�  ������|��٥�
 	    �Ǥ���e
 	    ��    ( char ��    ���Τޤ޽� )
 	    
 	    S10|2|20|2|||
 	
 	T �ι�
 	    (�ι�0)|(�ι�1)|(�ι�2) ... (�ι�n)
 	    �ι�Υ���ȥ꤬���ξ��� '|' ��  ³����Τǡb  ³������
 	    �Τ�    ���ƥ�����󤷤ƤϤʤ�ʤ��e���ʤ餺    �Ŀ�ʬ����
 	    ����e
 	    �ҤȤġb�ҤȤĤ�    �ϡb
 	      ��
 	    �ΤߤǤ���e
 	    ��    ( �����Ƥ��� char ��    ���ΤޤޤǤ��� )
 	    
 	    T����|��äѤ餤
 	    
 	    �����<a href="#escaping">���������פ���e
 
 	M   ʬ�ѥ�᡼��
 	    ����饯�����Υ�  �������Τ����b    ��  ���������(����
 	    ��)�Ȥ����Ǥʤ����(    �ͤʤ�)������e�ɤ줫�Υ�  ������
 	    ��  ������뤿�Ӥ�  ����  �Υѥ�᡼������������Τϥͥå�
 	    ����ȥ�ե��å���  �̤˻Ȥ����Ȥˤʤ�Τǡb  ��HP ,
 	    MP,EXP���ͤ����ˤ��ܤä��ѥ�᡼���������ޥ�ɤ��Ѱդ���e
 	    ���줬����M���ޥ�ɤǤ���e2  ��  �ʹߤ�  ��ȡ������HP��
 	    ������ ,     �ȡ������MP�b  ���ȡ������EXP�Ǥ���e
 	    �ʲ��˶�    �򤷤᤹�e
 	    
 	    M54|210|8944909
 	    
 	    ����  �Ǥ�HP�θ����ͤ�54�bMP�θ����ͤ�210�b�и��ͤ�8944909
 	    �ˤʤäƤ���Τ��e����3��  ��  ��  ��  �٤��⤤��  ����
 	    ����    �������ޥ�ɲ��������b�����󤳤�M���ޥ�ɤΤ���
 	    ���P���ޥ�ɤ����ѥ�᡼�����������Ƥ�褤�e�������b  Ŭ
 	    �˥ͥåȥ����Ȥ�����ˤϡb���Υ��ޥ�ɤ�Ȥ����Ȥ��侩
 	    �����e
 	    
         D ������ID
 	    �ҥ�������ˤ������륭����index�f����Ȼ��֤�����e
 	    D1000|912766409�Ȥ���������e
 	
 	E ���󥫥���ȳ�    ����)|(���)
 	    ���󥫥����  ����������e��  ��n/100
 	    ���饤����Ȥϡf���¤��ͤ��饹�����Ȥ��ơf��������  ��+1������  �ǥ��󥫥���Ȥ��뤫��  �Ǥ���e
 	    �������f��¤��ͤ�ۤ��ʤ��褦�ˤ���e
 	    ����ˤ��줬�����Ƥ���ޤǤ��γ�  �ǥ��󥫥���Ȥ�׻�������e
 	
 	J0   J6 �Ȥ�����Ѥ�    
 	    �Ȥ�����Ѥ�    ����������e
 	    0   6 �Ϥ��줾�쥢��  �����  �ս���б����Ƥ���e��  ����  �ս�    �f  �f��f����    
 	    ������  ��  �ʤ�  ������f��  ��ĥ�ΰ٤����Ƥ���  �ս�ξ�  ������
 	    J0|kubun|mp|field|target|name|comment
 	    �Ȥ����ե����ޥåȤˤʤäƤ���e
 	    kubun �Ϥ���  �����뤫  �����e������  ���e����ʹߤ˥ȡ����󤹤�ʤ��e�����Ȥ���e
 	    mp�Ͼ�  ��  ��  ���e
 	    field�Ϥɤξ��ǻȤ��뤫�e�����С��Ǥϰʲ���  ���������Ƥ���e
 	    
 		typedef enum
 		{
 			MAGIC_FIELD_ALL,		���٤Ƥξ��ǻȤ���
 			MAGIC_FIELD_BATTLE,		��    �Τ�
 			MAGIC_FIELD_MAP,		�̾�ޥå׾�Τ�
 			
 		}MAGIC_FIELDTYPE;
 	    
 	    target�Ϥɤ���оݤ˽�  �뤫�e�����С��Ǥϰʲ��Τ褦���������Ƥ���e
 	    
 		typedef enum
 		{
 			MAGIC_TARGET_MYSELF,		��ʬ�Τ�
 			MAGIC_TARGET_OTHER,		¾�ο�  ��ʬ�ޤ�)
 			MAGIC_TARGET_ALLMYSIDE,		    ��  
 			MAGIC_TARGET_ALLOTHERSIDE,	���¦��  
 			MAGIC_TARGET_ALL,		����
 			MAGIC_TARGET_NONE,		ï����  ��  �ʤ��e  ��䤿��λ�
 			MAGIC_TARGET_OTHERWITHOUTMYSELF,¾�ο�  ��ʬ�ޤޤʤ�)
 			MAGIC_TARGET_WITHOUTMYSELFANDPET, ��ʬ�ȥڥåȰʳ�
 			MAGIC_TARGET_WHOLEOTHERSIDE,        �Υ�������  
 		}MAGIC_TARGETTYPE;
 	    
 	    <font size=+1>���ο�����100��­���ȡf���Ǥ���Ԥ��оݤȤʤ�e
 	    
 	    name�ϼ���  �e
 	    comment�Ϥ��μ��Ѥ�    �e
 	
 	N0    N3 ��֤Υѥ�᡼��
 	    ��֤ˤʤäƤ���ͤξ�  ����������e
 	    N0|kubun|level|charaindex|maxhp|hp|mp|name
 	    
 	    kubun �Ϥ���  �����뤫  �����e������  ���e����ʹߤ˥ȡ����󤹤�ʤ��e�����ѥ�᡼���e
 	    2�ʾ���ȡf�ƥӥåȤ�  �äƤ����ΤΥѥ�᡼��  1bit    level 2bit   charaindex�ʤ�  
 	    �����������e
 	    charaindex ����֤�charaindex
 	    level�Ϥ��οͤΥ�٥�e
 	    maxhp�Ϥ��οͤ�MAXHP
 	    hp�Ϥ��οͤθ��ߤ�HP
 	    mp�Ϥ��οͤε�  
 	    name �Ϥ��οͤ�  ���e
 	
 	K0  K4 ��  �ѥ�᡼��
 	    hp maxhp mp maxmp str tough exp 
 	    level attackpower deffencepower 
 	    fixdex fixcharm fixluck fixfireat fixwaterat fixearthat fixwindat
 	      �� status
 	    K��  ��0  4����ꤷ�ơb�ɤζ�  ������ꤹ�뤳�ȡe
 	    ���α��Υȡ������0��  ���餽�Υڥå�  ��  ���ȸ������e
 	    ������ϣ��Ǥ���e1�������ѥ�᡼���e
 	    2�ʾ���ȡf�ƥӥåȤ�  �äƤ����ΤΥѥ�᡼��  1bit    hp  2bit   maxhp�ʤ�  
 	    �����������e
 	    �ǥ�ߥ��� '|' �Ǥ���e�ޤ�  ���ȼ��ʾι�ϡb
 	    <a href="#escaping">���������פ�����Τ�
 	    ���ƫ�ǤĤʹ�����Τ��̿������e
 	    ���줾����ͤη��ϰʲ��e
 	    
 	        No.(  ��)  int  
 	        islive(����)  int  
 	        GRA(  ��  ��)  int  
 	        Hp(�ѵ�  )  int  
 	        MaxHp(    �ѵ�  )  int  
 	        Mp  int  
 	        MapMp  int  
 	        Exp(�и���)  int  
 	        MaxExp(���ޤǤηи���)  int  
 	        Level(��٥�)  int  
 	        Attack(  ��  )  int  
 	        Defense(��    )  int  
 	        Quick(���ᤵ)  int  
 	        Ai(������)  int  
 	        fEarth(�M)  int  
 	        fWater(  )  int  
 	        fFire(��)  int  
 	        fWid(��)  int  
 	        Slot(    ��  ��)  int  
 	          ��  �����ĥե饰  int  
 	          ��    ��    
 	        �桼�����ڥå�      ��    
 	    
 	      ��  �����ĥե饰�Ȥϡf���ΥڥåȤ�  ����  �����Ƥ褤���ɤ����Υե饰�ǡf
 	    1 ����  ��    �f������  ���ԲĤȤʤ�e
 	    
 	    ��    ( char ��    ���Τޤ޽� )
 	    
 	    P10|20|10|20|10|10|10|1|2|13|13|1|100|10|�ؤ��|PC
 
 	W0  W4 �ڥåȤ�  �ǡ���
 	  W0|skillid|field|target|name|comment| x 7
 	  W0    W4 �Ϥ��줾��ΥڥåȤ��б����Ƥ���e
 	  petskillid �ϡf�ڥåȤ�  ��  ��epet_skillinfo.h���������Ƥ���e
 	  field �Ϥ���  ���ɤ��ǻ��ѤǤ��뤫�e�����С��Ǥϰʲ��Τ褦���������Ƥ���e
 	  
 		typedef enum
 		{
 			PETSKILL_FIELD_ALL,		���٤Ƥξ��ǻȤ���
 			PETSKILL_FIELD_BATTLE,		��    �Τ�
 			PETSKILL_FIELD_MAP,		�̾�ޥå׾�Τ�
 
 		}PETSKILL_FIELDTYPE;
 	  
 	  target �Ϥ���  ���оݤ��ɤ�������Τ��e�����С��Ǥϱ��Τ褦���������Ƥ���e
 	  
 		typedef enum
 		{
 			PETSKILL_TARGET_MYSELF,		��ʬ�Τ�
 			PETSKILL_TARGET_OTHER,		¾�ο�  ��ʬ�ޤ�)
 			PETSKILL_TARGET_ALLMYSIDE,	    ��  
 			PETSKILL_TARGET_ALLOTHERSIDE,	���¦��  
 			PETSKILL_TARGET_ALL,		����
 			PETSKILL_TARGET_NONE,		ï����  ��  �ʤ��e  ��䤿��λ�
 			PETSKILL_TARGET_OTHERWITHOUTMYSELF,¾�ο�  ��ʬ�ޤޤʤ�) 
 			PETSKILL_TARGET_WITHOUTMYSELFANDPET, ��ʬ�ȥڥåȰʳ�
 		}PETSKILL_TARGETTYPE;
 	  
 	  name ��  ��  ���e
 	  comment�Ϥ���  ���Ф���    �e
 	  target|name|comment| ��  �ο���ʬ���Ԥ������Ƥ���e
 	    �ϰ�����Ĥޤǡe    ��  ��  ���Ƥ���  "|"�Τߤ�³��  ���
 	  �ϥ��饤����Ȥǵͤ��  ��������e
*/
void lssproto_S_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_S_SEND, buffer);
}

/*
   servertoclient D( int category , int dx , int dy , string data );
       (Display)
           �˲���  �������  �e
 
       
 	int category
 	����  �����뤫�e
 	    
 	        ��        
 	        1  ��ʬ�ʳ���Ϳ�������᡼���edata��  ��
 		    �ˤʤä��͡e  
 		    2  ��ʬ�����������᡼���edata��  ��  ��
 		      �ʤä���  
 		
 	int dx
 	�ޥ�����餫��Υ���å����а�  X�e��  Ū�ˤϥ��٥�Ȥ�  ��
 	    ������  �e���饤����ȤϤ����ͤ���Ŭ��Ŭ�ڤʰ�  ��׻�����
 	        ����e
 	int dy
 	���а�  Y
 	string data 
 	  ������    �e    ��category�ˤ�äƷ�ޤ�e
 	    ����  ��  ��<a href="#escaping">���������פ���ʤ����
 	    �ʤ�ʤ��e
*/
void lssproto_D_send(int fd,int category,int dx,int dy,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, category);
	checksum += util_mkint(buffer, dx);
	checksum += util_mkint(buffer, dy);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_D_SEND, buffer);
}

/*
   servertoclient FS( int flg);
       (FlgSet)PS�Τα����e�ޤ��Ϥҥ�������ʤɤ˼�ʬ�ξ��֤Ȥ��������Ƥ���e
       
         int flg
          0 bit  0: ���Off                  1: ���On
                ����  ����  1 bit  0: ��      ����off          1: ��      ����On    
              2 bit  0: DUEL off                 1: DUEL On
              3 bit  0:   �̤Υ���åȥ⡼��     1: �ѡ�  ���ˤ�������åȤ�  �Фʤ��⡼��
              4 bit  0:   �ɸ�OK               1:   �ɸ򴹵�  
*/
void lssproto_FS_send(int fd,int flg)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, flg);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_FS_SEND, buffer);
}

/*
   servertoclient HL( int flg);
       (HeLp)HL�Τα����e�ޤ��ϥѡ�  ������֤���  �ζ������⡼�ɤ�  ���������������Ƥ���e
       
         int flg
          0: �������⡼��Off                  1: �������⡼��On
*/
void lssproto_HL_send(int fd,int flg)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, flg);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_HL_SEND, buffer);
}

/*
   servertoclient PR( int request, int result);
       (PartyRequest)����׵�α����ePR�򥯥饤����Ȥ����äƤ��ʤ��Ƥ⤳����������������e
        �ѡ�  ����  ����  �Ƥ�  ����  �ʤɤλ��e
       
       	int request
       	0: ���� 1:  ��
         int result
         0: ��   1: ��  
*/
void lssproto_PR_send(int fd,int request,int result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, request);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PR_SEND, buffer);
}

#ifdef _PETS_SELECTCON
void lssproto_PETS_send(int fd,int petarray,int result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, petarray);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PETST_SEND, buffer);
}
#endif

void lssproto_KS_send(int fd,int petarray,int result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, petarray);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_KS_SEND, buffer);
}

void lssproto_SPET_send(int fd, int standbypet, int result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, standbypet);
	checksum += util_mkint(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SPET_SEND, buffer);
}

/*
   servertoclient PS( int result, int havepetindex, int havepetskill, int toindex);
       (PetSkill use result)�ڥåȤ�  ��Ȥä���̥��饤����Ȥ�PS���б����������Ƥ���e
       result �ʳ��ϥ��饤����Ȥ�PS���б����Ƥ���e�ե�����ɾ夫�餷���ƤФ�ʤ��e
       
 	int result
 	��̡e0: ��   1:��  
         int havepetindex
         ��    �ΥڥåȤ����Ѥ������e
         int havepetskill
         ��    ��  ����Ѥ������e
 	int toindex
 	 ï��  ˡ����Ѥ������e����ϥ��֥������Ȥ䥭����index�ǤϤʤ��e�ʲ���  �ˤʤäƤ���e
 	
 	  ��ʬ    = 0
 	  �ڥå�  = 1   5
 	  ���    = 6   10   S N ��0  4���б��e��ʬ���Ȥ�ޤޤ�Ƥ���  
 	
 	�оݤ������f�Ȥ�ʬ����ʤ��f�Ȥ��ξ���-1����������e
*/
void lssproto_PS_send(int fd,int result,int havepetindex,int havepetskill,int toindex)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, result);
	checksum += util_mkint(buffer, havepetindex);
	checksum += util_mkint(buffer, havepetskill);
	checksum += util_mkint(buffer, toindex);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PS_SEND, buffer);
}

/*
   servertoclient SKUP( int point );
       (SKillUP)
       �����륢�åפ��Ǥ�����򥵡��Ф����Τ���e�����ľ干���뤫����ꤹ��e
*/
void lssproto_SKUP_send(int fd,int point)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, point);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SKUP_SEND, buffer);
}

/*
   servertoclient WN( int windowtype, int buttontype, int seqno, int objindex, string data );
       (Window)
       ������ɥ���  ������ȥ����С������饤����Ȥ����Τ���e
       
         int windowtype
         ������ɥ������סe�ɤΤ褦�ʷ����Υ�����ɥ���  �����뤫�e�����buttontype ���Ȥ߹�碌��
             ������ɥ������ꤹ��e
             �����С��Ǥϰʲ���  �����ꤵ��Ƥ���e
           
 		typedef enum
 		{
 			WINDOW_MESSAGETYPE_MESSAGE,			��å������Τ�
 			WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT,		��å������Ȱ��    
 			WINDOW_MESSAGETYPE_SELECT,			��  ������ɥ�
 			WINDOW_MESSAGETYPE_PETSELECT,			�ڥå���  ������ɥ�
 			WINDOW_MESSAGETYPE_PARTYSELECT,			�����  ������ɥ�
 			WINDOW_MESSAGETYPE_PETANDPARTYSELECT,		�ڥåȡf�����  ������ɥ�
 			WINDOW_MESSAGETYPE_ITEMSHOPMENU,		��Ź�Υ�˥塼������ɥ�
 			WINDOW_MWSSAGETYPE_ITEMSHOPYMAIN,		��Ź�Υᥤ�󥦥���ɥ�
 			WINDOW_MESSAGETYPE_LIMITITEMSHOPMAIN,		  ����귴  ����  �ರ�Υᥤ�󥦥���ɥ�
 			WINDOW_MESSAGETYPE_PETSKILLSHOP,		�ڥåȤ�  �����󥦥���ɥ�
 			WINDOW_MESSAGETYPE_WIDEMESSAGE,			��å������Τ�    ����    
 			WINDOW_MESSAGETYPE_WIDEMESSAGEANDLINEINPUT,	��å������Ȱ��        ����    
 
 		}WINDOW_MESSAGETYPE;
 
         int buttontype
         �ܥ���η�������ꤹ��e�����С��Ǥϰʲ����Ѥ��������Ƥ���e
             
 		#define		WINDOW_BUTTONTYPE_NONE		(0)
 		#define		WINDOW_BUTTONTYPE_OK		(1 << 0)
 		#define		WINDOW_BUTTONTYPE_CANCEL	(1 << 1)
 		#define		WINDOW_BUTTONTYPE_YES		(1 << 2)
 		#define		WINDOW_BUTTONTYPE_NO		(1 << 3)
 		#define		WINDOW_BUTTONTYPE_PREV		(1 << 4)
 		#define		WINDOW_BUTTONTYPE_NEXT		(1 << 5)
             
             �������Ȥ߹�碌����������e  ���СfYES�ܥ����NO�ܥ���  ��������
             WINDOW_BUTTONTYPE_YES | WINDOW_BUTTONTYPE_NO   (=12)
             ������e
             
         int seqno
         ���Υ�����ɥ���  ��򼨤��e�����С�����  ����e
             ���饤����Ȥ�WN�ˤƤ��Υ�����ɥ��Τ����̤��֤��Τˡf����  ���ź������������e
             ����ˤ�äƥ����С��Ϥɤ�NPC�Τɤξ�  �ǤΥ�����ɥ�����  �ǽ�  ��褦�ˤ���e
         int objindex
         ���Υ�����ɥ���Ф��ȸ��ä�NPC�ʤɤ�index����  ����Ƥ���e
             ����  �ब�Ф��ȸ��ä�����-1�ʤɤ�  �äƤ���e
             ���饤����Ȥϡf������ɥ�      ��WN�פҥȥ���Ǥ��ο��ͤ򤽤Τޤ��֤���  ���e
         string data
         ��å�����    �򼨤��e    �ϥ��������פ���e"\n"�Ƕ��ڤ�Ȳ��ԤȤ�����  �Ȥ���e
             �ޤ��f���饤����ȤΥ�����ɥ���  ����  �벣����ۤ����ݤϼ�  Ū�˲��Ԥ����e
             ��  ��Τ��륦����ɥ��Ǥϡf"\n"�Ƕ��ڤ�줿  ��Υȡ����󤬥�å������ιԿ�
             �Ȥʤ�f���˥�å������Υȡ�����  ��Υȡ�����ǻ��ꤵ�줿�Ŀ�³���f
             ���Τ��ȤΥȡ�����  �����Ĥ��Ĥ���  ��Ȥʤ�e�ޤ��������� 1 ����
               ��������ơf��  ��������WN�Ǥ������λ����֤��e
         string data(��Ź��  
         ��å�����    �򼨤��e    �ϥ��������פ���e"\n"�Ƕ��ڤ�Ȳ��ԤȤ�����  �Ȥ���e
             �ޤ��f���饤����ȤΥ�����ɥ���  ����  �벣����ۤ����ݤϼ�  Ū�˲��Ԥ����e
             �ǡ���  �Ϲ�    ��"  "�Ƕ��ڤ��Ƥ��ޤ��e
    ��  ����
  	  ��  ��ե饰    �����u  �룱    ���Υǡ����Ȥ����ե饰  �Ȥ����u�Ȥ�ʤ��u1    
 		Ź��  ��  ��å�����  Ź��å�����  �Ŀ���  ��å�����  ��٥�­��ʤ���å�����  ��  ��å�����  
 		����  �ब���äѤ���å�����  ����  ��      ����  ���ʤ��ե饰    ����u��  ���ʤ��u��    ����  ���٥�  
 		����    ��  ��        ����  ��      ����  ���ʤ��ե饰  ����  ���٥�  ����    ��  ��      
       		
       		0|1|���८���  �ﲰ|����ä��㤤�b���Τ褦����|����ä��㤤�ɤ�ˤ���|����  �פ���|�������  ����ˤϥ�٥뤬
       		­��ʤ����ɤ����Τ���|  ���ˤ��������|������������  �ब���äѤ�����ͤ�����  �֡�����|
       		��|������|����������|����  ��  ��\n����  ��|����|��|����|����������|    ��������\n  ���ˤϤĤ�  \n�¤�  
 
         ��  ���
 		  ��  ��ե饰    �����u  �룱    ���Υǡ����Ȥ����ե饰  �Ȥ����u�Ȥ�ʤ��u1    
 		Ź��  ��  ��å�����  Ź��å�����  ���⤬���äѤ��ˤʤä��㤦��å�����|��  ��å�����  
 		����  ��      ���  ��ʤ��ե饰  ����    ��  ��        ����  ��      ���  ��ʤ��ե饰  
 		����    ��  ��        
       
       		��|1|���८���  ��|����ä��㤤�b�ɤ뤹�롪  ����ä��㤤�ɤ��  �äƤ����e|  ���ˤ��������|
       		������ȡb���⤬�����С������㤦��  �֡�����|��|����|����������|
       		����  ��  ��\n����  ��|����|��|����|����������|    ��������\n  ���ˤϤĤ�  \n�¤�  
         �㥢��  ��  �����
 		�ե饰    ��=��    ���Ȳ���  �����뤫  
 		Ź��  ��  
 		  �����å�����  ����ʾ�  �����ʤ���å�����  ��  ��å�����  
 		  ��    �����뤫�ե饰  ��  �֣��f�Բġ֣�    ����    ��  ��  ��ԥ���ե�  ����  ��    ��  ������  
 		  ��    �����뤫�ե饰  ��  �֣��f�Բġ֣�    ����    ��  ��  ��ԥ���ե�  ����  ��    ��  ������  
         �㥢��  ���������
 		�ե饰  �������=��    
 		Ź��  ��  �����Ф���å�����  ����  �ब��  �λ��Υ�å�����|��  ��å�����  
 		  ��  �����Ф��뤫�ɤ����ե饰  ��  �֣��f�Բġ֣�    ����  ���٥�  ����    ��  ��  ��ԥ���ե�  
 		  ��  �����Ф��뤫�ɤ����ե饰  ��  �֣��f�Բġ֣�    ����  ���٥�  ����    ��  ��  ��ԥ���ե�  
 
         �㤽��¾��
 		Ź��  ��  ��å�����
       
       		���८���  ��|����ä��㤤�b�ɤ뤹�롪
 	
         string data(�ڥåȤ�  ��������  
         ��å�����    �򼨤��e    �ϥ��������פ���e"\n"�Ƕ��ڤ�Ȳ��ԤȤ�����  �Ȥ���e
             �ޤ��f���饤����ȤΥ�����ɥ���  ����  �벣����ۤ����ݤϼ�  Ū�˲��Ԥ����e
             �ǡ���  �Ϲ�    ��"  "�Ƕ��ڤ��Ƥ��ޤ��e
 	   ��ڥåȤ�  ��  ��
 		���ξ�  �Ȥ����ɤ�����  �Ȥ��u0�a�Ȥ�ʤ��u1    Ź��  ��  �ᥤ���å�����  
 		      ����                ����        
 	
 		1  ���८���  �ﲰ  ����ä��㤤�ޤä��e���Ф餷��  �Фä��Ǥ���  
 		    �οؤ��έ�  500    ��  ������    �a  ��  ������          
 		�M��콵  1000  ������  �������뤫��  Ŭ��    	
*/
void lssproto_WN_send(int fd,int windowtype,int buttontype,int seqno,int objindex,char* data)
{
	char buffer[65500];
	int checksum=0;
#ifdef _DEBUG_JZ
	print("WN_SEND:START");
#endif
#ifdef _NO_WARP
	// shan hanjj add Begin
    CONNECT_set_seqno(fd,seqno);
    CONNECT_set_selectbutton(fd,buttontype);    
	// shan End
#endif

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, windowtype);
	checksum += util_mkint(buffer, buttontype);
	checksum += util_mkint(buffer, seqno);
	checksum += util_mkint(buffer, objindex);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_WN_SEND, buffer);
	
}

/*
   servertoclient EF( int effect, int level, string option );
       (EFfect)
       ��䱫��ߤ餷����e���饤����Ȥ���  Ū�ʸ���  �����������e
       
         int effect
         ����  ��e���줾���­��������ȡf    �Ȥ�θ��̤�  ���ޤ��e���ˤ���ȱ����㤬�ߤä���Ȥ����ޤ��e
           
             1:��
             2:��
             4:�����    ��  
           
         int level
         ���̤ζ����e���Ͼä��e��  ����  ���ζ�����  ���e
         string option
         ��  ��ĥ��
*/
/* Arminius
  �s�W�����l���ʵe
  int effect = 10   ���l
      level dont care
      option = "pet1x|pet1y|dice1|pet2x|pet2y|dice2" (no makeEscapeString)
*/
void lssproto_EF_send(int fd,int effect,int level,char* option)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, effect);
	checksum += util_mkint(buffer, level);
	checksum += util_mkstring(buffer, option);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_EF_SEND, buffer);
}

/*
   servertoclient SE( int x, int y, int senumber, int sw );
       (SoundEffect)
 	���饤����Ȥ�SE��  �餹�褦�˻ؼ�����e
       
         int x,y
         SE��  �����κ�  �e
         int senumber
             ��  ��
         int sw
             ��  �餹���ߤ�뤫�e
           
             0:�ߤ��
             1:  �餹
*/
void lssproto_SE_send(int fd,int x,int y,int senumber,int sw)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	checksum += util_mkint(buffer, senumber);
	checksum += util_mkint(buffer, sw);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_SE_SEND, buffer);
}

/*
   servertoclient ClientLogin(string result);
       ClientLogin�������e
       
 	string result
 	"ok" �Ȥ���  ��  �e����  ��  �ϥ��������פ���ʤ��e
*/
void lssproto_ClientLogin_send(int fd,char* result)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CLIENTLOGIN_SEND, buffer);
}

/*
   servertoclient CreateNewChar(string result,string data);
       CreateNewChar�������e
       
 	string result
 	"successful" �� "failed" �Τ����줫�e����  ��  �ϥ�������
 	    �פ��ʤ��e
 	string data
 	"failed" �λ���    �򼨤��ʹ֤θ���ʬ��  ��
 	      �Ǥ���e��������ȥ����Ф�����������ΤޤޤǤ���e
 	    �ʲ���  ��  

 	    "failed bad parameter"

 	    �ξ��ϡb������ΤȤ��˵���Υѥ�᡼����  �Ϥ�ۤ���
 	    ����Ȥ����Ȥ��  ����e����ϥ����ॵ���С�����  ������
 	    �������Ǥ���e����  ��  �ϥ��������פ��ʤ��e
*/
void lssproto_CreateNewChar_send(int fd,char* result,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CREATENEWCHAR_SEND, buffer);
}

/*
   servertoclient CharDelete(string result,string data);
       CharDelete �������e
       
 	string result
 	"successful" �� "failed" �Τ����줫�e���������פ��ʤ��e
 	string data
 	"failed" �λ���    �򼨤��ʹ֤θ���ʬ��  ��
 	      �Ǥ���e��������ȥ����Ф�����������ΤޤޤǤ���e
 	    �ǥ�ߥ���դ��ޤʤ��Τǡb���������פ��ʤ��e
*/
void lssproto_CharDelete_send(int fd,char* result,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHARDELETE_SEND, buffer);
}

/*
   servertoclient CharLogin(string result,string data);
       CharaLogin�������e
       
 	string result
 	"successful" �� "failed" �Τ����줫�e���������פ��ʤ��e
 	string data
 	"failed" �λ��� ����    ��  ��  �e���������פ��ʤ��e
*/
void lssproto_CharLogin_send(int fd,char* result,char* data)
{
	char buffer[65500];
	int checksum=0;
	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);

	util_SendMesg(fd, LSSPROTO_CHARLOGIN_SEND, buffer);
}
/*
   servertoclient CharList(string result,string data);
       CharList�������e
       
 	string result
 	"successful" �� "failed" �Τ����줫�e���������פ��ʤ�
 	string data
 	result��"successful"�λ��ϡb��������ȥ����С�����¸�����
 	    ���뤹�٤ƤΥ�����  ���b���׼�����򥹥ڡ����Ƕ��ڤä�
 	    ��Ĥ�  ��  �e����  ��  ���äƤ���Τϡb��������ȥ����Сe
 	    result �� "failed" �λ���    �򼨤��ʹ֤θ���ʬ��  ��  ��
 	    ����e��  �λ��Υ��׼�����  �Ȥϰʲ��Τȶ���Ǥ���e
 	    dataplace|faceimage|level|maxhp|atk|def|quick|charm|earth|water|fire|wind|logincount|name|place
 	    
 	      dataplace
 	      �����֥ǡ����β�    ���e����ǥ���饯�����ꥹ�Ȥβ�    ������ꤹ��e
 	      faceimage
 	      ���  ��  ��
 	      level
 	      �����Υ�٥�
 	      maxhp,atk,def,quick,charm
 	      �ƥѥ�᡼���e
 	      earth.water,fire,wind
 	      ��°����
 	      logincount
 	      �ҥ����󥫥����
 	      name
 	      ������  ��
 	      place
 	      �����Τ�����
 	    
 	    "|" �Ƕ��ڤ��Ƥ���e ���줾��ι�  �ϡb<a
 	    href="#escaping">���������פ���Ƥ���e���Τ��Ȥ��ƫ�
 	    �ǤĤʹ���e
*/
void lssproto_CharList_send(int fd,char* result,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHARLIST_SEND, buffer);
	IsSucceed(fd,result);
}

/*
   servertoclient CharLogout(string result , string data);
       Logout���Ф��������e
       
 	string result
 	"successful" �� "failed" �Τ����줫�e���������פ��ʤ��e
 	string data
 	"failed" �λ��ˤΤ߰�  ������b��  ��    (����)�򼨤��ʹ�
 	    �θ���ʬ��  ��  �Ǥ���e���������פ��ʤ��e
*/
void lssproto_CharLogout_send(int fd,char* result,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, result);
	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHARLOGOUT_SEND, buffer);
}

/*
   servertoclient ProcGet( string data);
 	ProcGet�������e
       
 	string data
 	���������פ���e�����ॵ���С���    ��  ����������e    ��log/proc�ե�����˽񤫤��    ��  ���e
*/
void lssproto_ProcGet_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");
	
	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PROCGET_SEND, buffer);
}

/*
   servertoclient PlayerNumGet( int logincount, int player);
 	PlayerNumGet�������e
       
 	int logincount,player
*/
void lssproto_PlayerNumGet_send(int fd,int logincount,int player)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, logincount);
	checksum += util_mkint(buffer, player);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_PLAYERNUMGET_SEND, buffer);
}

/*
   servertoclient Echo( string test );
       Echo���Ф��������e
       
 	string test
 	�����ۤ�    ���줿  ��  �e���������פ��ʤ��e
*/
void lssproto_Echo_send(int fd,char* test)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, test);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_ECHO_SEND, buffer);
}

// CoolFish: Trade 2001/4/18

void lssproto_TD_send(int fd, int index, char* message)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_TD_SEND, buffer);
}
#ifdef _CHATROOMPROTOCOL			// (���i�}) Syu ADD ��ѫ��W�D
void lssproto_CHATROOM_send ( int fd , char* message )
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CHATROOM_SEND, buffer);
}
#endif
#ifdef _NEWREQUESTPROTOCOL			// (���i�}) Syu ADD �s�WProtocol�n�D�Ӷ�
void lssproto_RESIST_send ( int fd , char* message )
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RESIST_SEND, buffer);
}
#endif
#ifdef _OUTOFBATTLESKILL			// (���i�}) Syu ADD �D�԰��ɧޯ�Protocol
void lssproto_BATTLESKILL_send ( int fd , char* message )
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_BATTLESKILL_SEND, buffer);
}
#endif
void lssproto_NU_send(int fd, int nu)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, nu);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_NU_SEND, buffer);
}


void lssproto_FM_send(int fd, char* message)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, message);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_FM_SEND, buffer);
}


void lssproto_WO_send(int fd,int effect)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, effect);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_WO_SEND, buffer);
}
#ifdef _ITEM_CRACKER
void lssproto_IC_send(int fd, int x, int y)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, x);
	checksum += util_mkint(buffer, y);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_IC_SEND, buffer);
}
#endif
#ifdef _MAGIC_NOCAST             // ���F:�I�q
void lssproto_NC_send(int fd,int flg)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, flg);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_NC_SEND, buffer);
}
#endif

#ifdef _CHECK_GAMESPEED
void lssproto_CS_send( int fd, int deltimes)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint( buffer, deltimes);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_CS_SEND, buffer);
}
#endif

#ifdef _STREET_VENDOR
void lssproto_STREET_VENDOR_send(int fd,char *message)
{
	char buffer[65500];
	int checksum = 0;

	strcpy(buffer,"");

	CONNECT_getCdkey(fd,PersonalKey,4096);
	strcat(PersonalKey,_RUNNING_KEY);

	checksum += util_mkstring(buffer,message);
	util_mkint(buffer,checksum);
	util_SendMesg(fd,LSSPROTO_STREET_VENDOR_SEND,buffer);
}
#endif

#ifdef _RIGHTCLICK
void lssproto_RCLICK_send(int fd, int type, char* data)
{
	char buffer[65500];
	int checksum=0;

	print("\n RCLICK_send( type=%d data=%s) ", type, data );

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkint(buffer, type);
	checksum += util_mkint(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_RCLICK_SEND, buffer);
}
#endif

#ifdef _JOBDAILY
void lssproto_JOBDAILY_send(int fd,char *data)
{
	char buffer[65500];
	int checksum = 0;

	strcpy(buffer,"");

	CONNECT_getCdkey(fd,PersonalKey,4096);
	strcat(PersonalKey,_RUNNING_KEY);

	//print("tocli_data:%s ",data);
	checksum += util_mkstring(buffer,data);
	util_mkint(buffer,checksum);
	//print("tocli_buffer:%s ",buffer);
	util_SendMesg(fd,LSSPROTO_JOBDAILY_SEND,buffer);
}
#endif

#ifdef _TEACHER_SYSTEM
void lssproto_TEACHER_SYSTEM_send(int fd,char *data)
{
	char buffer[65500];
	int checksum = 0;

	strcpy(buffer,"");

	CONNECT_getCdkey(fd,PersonalKey,4096);
	strcat(PersonalKey,_RUNNING_KEY);

	checksum += util_mkstring(buffer,data);
	util_mkint(buffer,checksum);
	util_SendMesg(fd,LSSPROTO_TEACHER_SYSTEM_SEND,buffer);
}
#endif

#ifdef _ADD_STATUS_2
void lssproto_S2_send(int fd,char* data)
{
	char buffer[65500];
	int checksum=0;

	strcpy(buffer,"");

	CONNECT_getCdkey( fd, PersonalKey, 4096);
	strcat(PersonalKey, _RUNNING_KEY);

	checksum += util_mkstring(buffer, data);
	util_mkint(buffer, checksum);
	util_SendMesg(fd, LSSPROTO_S2_SEND, buffer);
}
#endif
