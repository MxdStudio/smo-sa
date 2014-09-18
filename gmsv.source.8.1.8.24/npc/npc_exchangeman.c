#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "net.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "npc_exchangeman.h"
#include "npc_eventaction.h"
#include "log.h"
#include "battle.h"
#include "handletime.h"
#include "enemy.h"
#include "npc_warp.h"

static void NPC_ExChangeMan_selectWindow( int meindex, int talker,int num);
BOOL NPC_TypeCheck(int meindex,int talker,char *szMes);
int NPC_ExChangeManEventCheck( int meindex, int talker, char *buff1);
BOOL NPC_EventItemCheck(int meindex,int talker,int itemNo,int flg);
BOOL NPC_EventLevelCheck(int meindex,int talker,int level,int flg);
// Arminius 8.14 move to .h (for pet talk)
//BOOL NPC_EventBigSmallCheck(int meindex,int talker,char* buf);
BOOL NPC_EventFreeIfCheck(int meindex,int talker,char* buf,int kosuu,int flg);
BOOL NPC_EventBigSmallLastCheck(int point1,int mypoint,int flg);
BOOL NPC_ENDEventNoCheck(int meindex,int talker,int shiftbit,int flg);
BOOL NPC_NOWEventNoCheck(int meindex,int talker,int shiftbit,int flg);
BOOL NPC_TiemCheck(int meindex,int talker,int time,int flg);
BOOL NPC_EventAddPet(int meindex, int talker, char *buff2,int mode);
BOOL NPC_RandItemGet(int meindex, int talker,int rand_j,char *buf);

#ifdef _JZ_TRANSX
BOOL NPC_EventTrans(int meindex, int talker, char *buff2);
#endif

BOOL NPC_PetLvCheckType2(int petindex,int meindex,int talker,char *buf,int mode);
BOOL NPC_PetLvCheck(int meindex,int talker,char *buf,int mode);
void NPC_RequestMain(int meindex,int talker,char *buf);
void NPC_AcceptMain(int meindex,int  talker ,char*buf);
void NPC_MsgDisp(int meindex,int talker,int num);
BOOL NPC_EventAdd(int meindex,int talker,int mode);
BOOL NPC_AcceptDel(int meindex,int talker,int mode);
BOOL NPC_SavePointCheck(int meindex,int talker,int shiftbit,int flg);

BOOL NPC_EventReduce(int meindex,int talker,char *buf);
BOOL NPC_EventDelItem(int meindex,int talker,char *buf,int breakflg);
BOOL NPC_EventDelItemEVDEL(int meindex,int talker,char *buf,char *nbuf,int breakflg);
BOOL NPC_EventAddItem(int meindex,int talker,char *buf);
int NPC_EventFile(int meindex,int talker,char *buf);
BOOL NPC_EventDelPet(int  meindex,int  talker, int petsel);
void NPC_PetSkillMakeStr(int meindex,int toindex,int select);
void NPC_CleanMain(int meindex,int talker,char *buf);	
void NPC_EventPetSkill( int meindex, int talker, char *data);
BOOL NPC_ItemFullCheck(int meindex,int talker,char *buf,int mode,int evcnt);
void NPC_CharmStatus(int meindex,int talker);
BOOL NPC_ImageCheck(int meindex,int talker,int kosuu,int flg);
int NPC_EventGetCost(int meindex,int talker,char *arg);

BOOL NPC_EventWarpNpc(int meindex,char *buf);


enum {
	CHAR_WORK_EVENTWARP	= CHAR_NPCWORKINT4,

};

/*********************************
* �����  
*********************************/
BOOL NPC_ExChangeManInit( int meindex )
{

	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buf[1024*2];
	int i = 1;
	char buf4[256];

	CHAR_setWorkInt(meindex,CHAR_WORK_EVENTWARP,1);
	
#if 0
	char buf2[512];
	char buf3[256];
	int j=1;
	int nameflg=0;
	int itemno;
	char *ret;
	char sendbuf[2][10]={"�L�k�ǰe�C","�i�H�ǰe�C"};
	char droplogbuf[2][10]={"�S�����C","�����F�C"};
	int flg=0;
#endif			


	/*--NPC�Υ����פ򥻥åȤ���--*/
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPEEVENT );


	/*--����ե������  ��  ��--*/
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1))) == NULL) {
		print("NPC_ExChange.c TypeCheck: GetArgStrErr\n");
		print("NPCName=%s\n", CHAR_getChar( meindex ,CHAR_NAME));
		return FALSE;
	}

	i = 1;
	/*--���٥��NPC����������  �������å�����--*/
	/*--�����å���  �b  �Ȥ��ƾä��뤫���b�ڥåȥ᡼�������뤫��--*/
	while(getStringFromIndexWithDelim( argstr ,"EventEnd" ,i ,buf ,sizeof(buf))
	!= FALSE)
	{
		i++;
		/*--�����ǽ񤫤�Ƥ���Τϡb��˥󥰤�Ф�-*/
		if(strstr( buf,"GetItem") != NULL) {
			if(strstr( buf,"ItemFullMsg") == NULL) {
				NPC_Util_GetStrFromStrWithDelim(buf,"EventNo",buf4,sizeof(buf4));
				print("\n### EventNo=%d %s:ItemFullMsg ����###",
				atoi( buf4),
				CHAR_getChar( meindex, CHAR_NAME));
			}
		}

		if(strstr( buf,"GetPet") != NULL) {
			if(strstr( buf,"PetFullMsg") == NULL) {
				NPC_Util_GetStrFromStrWithDelim(buf,"EventNo",buf4,sizeof(buf4));
				print("\n### ventNo=%d %s:PetFullMsg ���� ###",
				atoi(buf4),
				CHAR_getChar(meindex,CHAR_NAME));
			}
		}

		if(strstr( buf,"GetEgg") != NULL) {
			if(strstr( buf,"PetFullMsg") == NULL) {
				NPC_Util_GetStrFromStrWithDelim(buf,"EventNo",buf4,sizeof(buf4));
				print("\n### ventNo=%d %s:PetFullMsg ���� ###",
				atoi(buf4),
				CHAR_getChar(meindex,CHAR_NAME));
			}
		}

		if(strstr( buf,"GetStone") != NULL){
			if(strstr( buf,"StoneFullMsg") == NULL){
				NPC_Util_GetStrFromStrWithDelim(buf,"EventNo",buf4,sizeof(buf4));
				print("\n### EventNo=%d %s:StonFullMsg ���� ###",
				atoi( buf4),
				CHAR_getChar( meindex, CHAR_NAME));
			}
		}

		if(strstr( buf,"DelStone") != NULL) {
			if(strstr( buf,"StoneLessMsg") == NULL) {
				NPC_Util_GetStrFromStrWithDelim(buf,"EventNo",buf4,sizeof(buf4));
				print("\n### EventNo=%d %s:StoneLessMsg ���� ###",
				atoi(buf4),
				CHAR_getChar(meindex,CHAR_NAME));
			}
		}
	}


	i = 1;

#if 0
	/*--���٥��NPC����������  �������å�����--*/
	/*--�����å���  �b  �Ȥ��ƾä��뤫���b�ڥåȥ᡼�������뤫��--*/
	while(getStringFromIndexWithDelim( argstr ,"EventEnd" ,i ,buf ,sizeof(buf))
	!=FALSE)
	{
		i++;
		/*--�����ǽ񤫤�Ƥ���Τϡb��˥󥰤�Ф�-*/
		if(NPC_Util_GetStrFromStrWithDelim( buf,"GetItem", buf2,sizeof( buf2)) 
		!=NULL)
		{
			j=1;
			flg=0;
			while(getStringFromIndexWithDelim( buf2,",", j, buf3, sizeof(buf3))
			!=FALSE)
			{
				j++;
				if(strstr( buf2, "*")!=NULL){
					getStringFromIndexWithDelim( buf3 ,"*" ,1, buf4 ,sizeof(buf4));
					itemno = atoi(buf4);
				}else{
					/*--����  ��ʥ�С����å�--*/
					itemno = atoi(buf3);
				}
				
				ret=NPC_Util_GetStrFromStrWithDelim( buf,"NoWarning", buf4,sizeof(buf4));
				if(ret != NULL){
					char buff[8];
					int k=1;

					while(getStringFromIndexWithDelim( buf4,"," ,k ,buff, sizeof(buff))
					!=FALSE){
						k++;
						if(itemno == atoi(buff))
						{
							flg=1;
							break;
						}
					}
				}
				
				if(flg == 0){
					if( (itemno >=2400 && itemno < 3000) &&(
					(ITEM_getdropatlogoutFromITEMtabl(itemno) == 1)
					 || (ITEM_getvanishatdropFromITEMtabl(itemno) == 1)
					 || (ITEM_getcanpetmailFromITEMtabl(itemno) == 0))
					 ){
						if(nameflg==0){
							print("\n%s",CHAR_getChar( meindex ,CHAR_NAME));
							nameflg=1;
						}
						print("\nID:%-5d,Logout:%-8s,DropCls:%-8s,SendMail:%-8s,%-16s",
						itemno,
						droplogbuf[ITEM_getdropatlogoutFromITEMtabl(itemno)],
						droplogbuf[ITEM_getvanishatdropFromITEMtabl(itemno)],
						sendbuf[ITEM_getcanpetmailFromITEMtabl(itemno)],
						ITEM_getNameFromNumber(itemno)
						);
					}
				}
				
			}
		}
	
	}
#endif
	return TRUE;

}

/*--      �ΰ�  --*/
BOOL NPC_EventWarpNpc(int meindex,char *buf)
{
	char token[32];
	char buf2[32];
	int floor,x,y;
	int ret;
	int cnt;
	int dcnt = 1;

	/*--���ߤΥ�פν�  --*/
	cnt = CHAR_getWorkInt(meindex,CHAR_WORK_EVENTWARP);
	while(getStringFromIndexWithDelim(buf,",", dcnt,buf2,sizeof(buf2) )
	 !=FALSE ){
		dcnt++;
	}
	
	if(dcnt <= cnt) cnt=1;


	while(getStringFromIndexWithDelim(buf,",", cnt,buf2,sizeof(buf2)) !=FALSE )
	{
		cnt++;
	    ret=getStringFromIndexWithDelim(buf2,".", 1,token,sizeof(token));
        if( ret == FALSE ) {
        	 continue;
       	}
        floor = atoi( token );

        ret=getStringFromIndexWithDelim(buf2,".", 2,token,sizeof(token));
        if( ret == FALSE ){
          continue;
        }
        x = atoi( token );

        ret=getStringFromIndexWithDelim(buf2,".", 3,token,sizeof(token));
        if( ret == FALSE ) {
         continue;
        }
        y = atoi( token );

		CHAR_setWorkInt(meindex,CHAR_WORK_EVENTWARP, cnt);
        CHAR_warpToSpecificPoint(meindex, floor, x, y);
		return TRUE;
	}
	
	return FALSE;

}


/*********************************
*   ��������줿���ν�  
*********************************/
void NPC_ExChangeManTalked( int meindex , int talkerindex , char *szMes ,int color )
{
	/* �ץ쥤�䡼���Ф��Ƥ���  ������ */
    if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }

	/*--  �����ˤ��뤫�ɤ�����--*/
	if(NPC_Util_isFaceToFace( meindex ,talkerindex ,2) == FALSE) {
		/* ������åɰ�  �Τ� */
		if(NPC_Util_isFaceToChara(talkerindex,meindex,1 ) == FALSE) return;
	}
	
	/*--�ץ쥤�䡼�Υ��    ������--*/
	CHAR_setWorkInt( talkerindex ,CHAR_WORKSHOPRELEVANT, 0);
	CHAR_setWorkInt( talkerindex ,CHAR_WORKSHOPRELEVANTSEC, 0);
	CHAR_setWorkInt( talkerindex ,CHAR_WORKSHOPRELEVANTTRD, 0);

	/*--������  ��ʬ����--*/
	NPC_TypeCheck( meindex, talkerindex, szMes);

}

/*------------------------------------------------------------
 *����ե������  ��  �ߡb��郎���ä������פν�  �򤹤�
 *-------------------------------------------------------------*/
BOOL NPC_TypeCheck(int meindex,int talker,char *szMes)
{

	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buf[1024*2];
	char buf2[512];
	int i = 1;
	int EvNo;
	int evch;
	
	/*--����ե������  ��  ��--*/
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
		print("NPC_ExChange.c TypeCheck: GetArgStrErr\n");
		print("NPCName=%s\n", CHAR_getChar(meindex,CHAR_NAME));
		return FALSE;
	}

	/*-- ����ե������ʬ�䤹�� --*/
	while(getStringFromIndexWithDelim( argstr,"EventEnd",i,buf,sizeof( buf))
	!= FALSE){
		i++;
		CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANT, 0);
		CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 0);

		/*-- ���٥�ȥʥ�С��򥲥å� --*/
		if(NPC_Util_GetStrFromStrWithDelim(buf,"EventNo",
								buf2, sizeof( buf2) ) == NULL) continue;

		if(strstr( buf2, "-") != 0) {
			EvNo = -1;
		}else{
			EvNo = atoi( buf2);
		}

		/*-- �ե饰�����å���Ԥ��e���Ǥ˥��٥�Ȥ���  ���Ƥ�����ϱ��� --*/
		if(NPC_EventCheckFlg( talker, EvNo) == TRUE) continue;

		/*--������ɤΥ����å���Ԥ�--*/
		if(NPC_Util_GetStrFromStrWithDelim( buf,"KeyWord", buf2,sizeof( buf2) ) 
		 != NULL){
			if(strcmp( szMes, buf2) != 0) continue;
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANT, i);
		}

		/*--�ڥåȤ�  ���Υ����å���  �äƤ뤫������å�--*/
		if(NPC_Util_GetStrFromStrWithDelim( buf,"Pet_Name", buf2,sizeof( buf2)) != NULL){
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 1);
		}
		evch = NPC_ExChangeManEventCheck( meindex, talker, buf);
		if(evch != -1) {
			if(NPC_Util_GetStrFromStrWithDelim( buf,"TYPE", buf2, sizeof( buf2)) != NULL){
				
				if( strstr( buf2, "REQUEST") != NULL) {
					NPC_RequestMain( meindex, talker, buf);
					return TRUE;
				}else if( strstr( buf2, "ACCEPT") != NULL) {
					/*-��  ������Ȥ�  --*/
					NPC_AcceptMain( meindex, talker, buf);
					return TRUE;
					
				}else if(strstr( buf2, "MESSAGE") != NULL) {
					char token[14*100];
					char token2[14*50];
					if(NPC_Util_GetStrFromStrWithDelim( buf,"NomalMsg",	token,sizeof( token) )
						!= NULL ) {
						if(NPC_AcceptDel( meindex, talker, 0) == FALSE) return FALSE;
						if(EvNo != -1)
							NPC_NowEventSetFlg( talker, EvNo);
						CHAR_talkToCli( talker, meindex, token, CHAR_COLORWHITE);
						if(NPC_Util_GetStrFromStrWithDelim( buf,"NpcWarp", token2,sizeof( token2) )
							!= NULL){
							NPC_EventWarpNpc(meindex,token2);
						}
						if(NPC_Util_GetStrFromStrWithDelim( buf, "EndSetFlg", buf2, sizeof( buf2) )
							!=NULL){
							char buf3[16];
							int k = 1;
							if(EvNo != -1) NPC_NowEventSetFlgCls( talker, EvNo);
							while(getStringFromIndexWithDelim(buf2 , "," ,k, buf3, sizeof(buf3))
								!=FALSE ){	
								k++;
								NPC_EventSetFlg( talker, atoi( buf3));
							}
						}
						if( NPC_Util_GetStrFromStrWithDelim( buf, "CleanFlg",
							buf2, sizeof( buf2)) != NULL) {
							char buf3[16];
							int k = 1;
							k= 1 ;
							while(getStringFromIndexWithDelim(buf2 , "," , k, 
								buf3, sizeof(buf3))
								!= FALSE ){
								NPC_NowEndEventSetFlgCls( talker, atoi(buf3));
								k++;
							}
						}
#ifdef _ANGEL_SUMMON
						// �����l�����
						if( NPC_Util_GetStrFromStrWithDelim( buf, "MISSIONOVER",
							buf2, sizeof( buf2)) != NULL)
						{
							int mindex;
							int mission;
							char nameinfo[64];
							
							mission = atoi( buf2);
							mindex = checkIfAngel( talker);
							
							if( mission == missiontable[mindex].mission )
							{
								char msg[1024];
								
								print(" ====�����l�����==== ");

								CHAR_setInt( talker, CHAR_HEROCNT, 
									CHAR_getInt( talker, CHAR_HEROCNT)+1 );
								getMissionNameInfo( talker, nameinfo);
								saacproto_ACMissionTable_send( acfd, MISSION_HERO_COMPLETE, 4, nameinfo, "");
								sprintf( msg, " �������� i:%d m:%d %s ", mindex, mission, nameinfo);
								print( msg);
								LogAngel( msg );
							}
							
						}
						
						// �M���l�����
						if( NPC_Util_GetStrFromStrWithDelim( buf, "MISSIONCLEAN",
							buf2, sizeof( buf2)) != NULL)
						{
							int mindex;
							int mission;
							char nameinfo[64];
							
							mission = atoi( buf2);
							mindex = checkIfAngel( talker);
							
							if( mission == missiontable[mindex].mission )
							{
								char msg[1024];

								getMissionNameInfo( talker, nameinfo);
								saacproto_ACMissionTable_send( acfd, mindex, 3, nameinfo, "");

								sprintf( msg, " ������ i:%d m:%d %s ", mindex, mission, nameinfo);
								print( msg);
								LogAngel( msg );
							}
							
						}
#endif

					}else if(NPC_Util_GetStrFromStrWithDelim(buf, "NomalWindowMsg",
						token, sizeof( token) )
						!=NULL) {
						/*--������ɥ��ǥ�å��������֤�--*/
						int fd = getfdFromCharaIndex( talker);
#ifdef _NEWEVENT
						if(NPC_Util_GetStrFromStrWithDelim(buf, "NomalWindowMsg1",
							buf2, sizeof(buf2)) != NULL){
							lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
						}else {
#endif
							char token2[14*50];
							if(NPC_AcceptDel( meindex, talker,0) == FALSE) return FALSE;
							if( EvNo != -1) NPC_NowEventSetFlg( talker, EvNo);
							
							if(NPC_Util_GetStrFromStrWithDelim( buf,"NpcWarp",
								token2,sizeof( token2) ) != NULL){
								NPC_EventWarpNpc(meindex,token2);
							}
							if(NPC_Util_GetStrFromStrWithDelim( buf, "EndSetFlg", 
								buf2, sizeof( buf2) ) !=NULL){
								char buf3[16];
								int k = 1;
								if(EvNo != -1) NPC_NowEventSetFlgCls( talker, EvNo);
								while(getStringFromIndexWithDelim(buf2 , "," ,k, 
									buf3, sizeof(buf3)) !=FALSE ){	
									k++;
									NPC_EventSetFlg( talker, atoi( buf3));
								}
							}
							//���٥�ȥե饰��ä�
							if( NPC_Util_GetStrFromStrWithDelim( buf, "CleanFlg",
								buf2, sizeof( buf2)) != NULL) {
								char buf3[16];
								int k = 1;
								
								k= 1 ;
								while(getStringFromIndexWithDelim(buf2 , "," , k, 
									buf3, sizeof(buf3))	!= FALSE ){
									NPC_NowEndEventSetFlgCls( talker, atoi(buf3));
									k++;
								}
							}
#ifdef _ANGEL_SUMMON
							// �����l�����
							if( NPC_Util_GetStrFromStrWithDelim( buf, "MISSIONOVER",
								buf2, sizeof( buf2)) != NULL)
							{
								int mindex;
								int mission;
								char nameinfo[64];
								
								mission = atoi( buf2);
								mindex = checkIfAngel( talker);
								
								if( mission == missiontable[mindex].mission )
								{
									char msg[1024];
									
									print(" ====�����l�����==== ");

									CHAR_setInt( talker, CHAR_HEROCNT, 
										CHAR_getInt( talker, CHAR_HEROCNT)+1 );
									getMissionNameInfo( talker, nameinfo);
									saacproto_ACMissionTable_send( acfd, MISSION_HERO_COMPLETE, 4, nameinfo, "");
									sprintf( msg, " �������� i:%d m:%d %s ", mindex, mission, nameinfo);
									print( msg);
									LogAngel( msg );
								}
								
							}
							
							// �M���l�����
							if( NPC_Util_GetStrFromStrWithDelim( buf, "MISSIONCLEAN",
								buf2, sizeof( buf2)) != NULL)
							{
								int mindex;
								int mission;
								char nameinfo[64];
								
								mission = atoi( buf2);
								mindex = checkIfAngel( talker);
								
								if( mission == missiontable[mindex].mission )
								{
									print(" ====�M���l�����==== ");
									
									getMissionNameInfo( talker, nameinfo);
									saacproto_ACMissionTable_send( acfd, mindex, 3, nameinfo, "");
								}
								
							}
#endif
							
							lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_YES,
								CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
#ifdef _NEWEVENT
						}
#endif
					}
					
					return TRUE;
					
				}else if(strstr( buf2, "CLEAN") != NULL) {
					NPC_CleanMain( meindex, talker, buf);
					return TRUE;
				}
			}
		}
	}

	strcpy( buf, "\0");
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "NomalMainMsg", buf, sizeof( buf)) == NULL )	{
		print("\n err:NOT FIND NomalMainMsg");
		return FALSE;
	}
	i = 1;
	while(getStringFromIndexWithDelim( buf, ",", i, buf2, sizeof( buf2))!= FALSE){
		i++;
	}
	i--;
	i = rand()%i + 1;
	getStringFromIndexWithDelim( buf,",", i, buf2, sizeof( buf2));
	CHAR_talkToCli( talker, meindex, buf2,  CHAR_COLORWHITE);
	return TRUE;
}


/*-----------------------------------
 *������  �˽�  ��ʬ����
 ------------------------------------*/
static void NPC_ExChangeMan_selectWindow( int meindex, int talker,int num)
{

	switch( num) {
		/*--�ꥯ�����ȷ�--*/
		  case 0:
		  /*-���٥��  �ʤΤǡb���٥�Ȥ�ߤ�뤫�ɤ�����  ��--*/
		  	NPC_MsgDisp( meindex, talker, 0);
			break;

		  case 1:
		  /*--�����˥��٥�ȹԤ�--*/
	  		NPC_MsgDisp( meindex, talker, 5);
		  	break;

			/*--�������ץȷ�--*/
		  case 2:
			/*--�������--*/
			NPC_MsgDisp( meindex, talker, 6);
			break;

		  case 3:
			/*--�������--*/
			NPC_MsgDisp( meindex, talker, 11);
			break;
	}

}

/*-------------------------------------
 *���٥�Ȥ��  �����  �Υᥤ��
 --------------------------------------*/
void NPC_RequestMain(int meindex,int talker,char *buf)
{
	int shiftbit;
	char buf2[128];
		
	/*--���٥�ȥʥ�С��򥲥å�--*/
	NPC_Util_GetStrFromStrWithDelim( buf,"EventNo", buf2,sizeof( buf2) );
	shiftbit = atoi( buf2);

	if(NPC_NowEventCheckFlg( talker, shiftbit) == TRUE) {
		NPC_ExChangeMan_selectWindow( meindex, talker, 0);
	}else{
		NPC_ExChangeMan_selectWindow( meindex, talker, 1);
	}
}

/*---------------------------------------
 * ���٥�Ƚ�  �Υᥤ��
 ----------------------------------------*/
void NPC_AcceptMain(int meindex,int  talker ,char *buf)
{

	NPC_ExChangeMan_selectWindow( meindex, talker, 2);
}

/*----------------------------------------
 *  �ե饰�򥯥ꥢ����٤Υᥤ��
 ----------------------------------------*/
void NPC_CleanMain(int meindex,int talker,char *buf)
{
	NPC_ExChangeMan_selectWindow( meindex, talker, 3);
}

/*-----------------------------------------
���饤����Ȥ����֤äƤ������˸ƤӽФ����e
-------------------------------------------*/
void NPC_ExChangeManWindowTalked( int meindex, int talkerindex, 
								int seqno, int select, char *data)
{
	/*--       �Τޤ��ˤ��ʤ��Ȥ��Ͻ�   --*/
	if( NPC_Util_CharDistance( talkerindex, meindex ) > 2) {
		return;
	}


	switch( seqno) {

		/*--�ե饰��ä����Υ�����ɥ�--*/
	  case CHAR_WINDOWTYPE_WINDOWEVENT_CLEANMSG:
		if( select == WINDOW_BUTTONTYPE_YES) {
			NPC_MsgDisp( meindex, talkerindex, 12);
		}else if(select == WINDOW_BUTTONTYPE_NO) {
		}
		break;

#ifdef _NEWEVENT
	  case CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG:
		if(select == WINDOW_BUTTONTYPE_NEXT) {
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
				CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD) + 1);
			NPC_MsgDisp( meindex, talkerindex, 99);
		}else if(select == WINDOW_BUTTONTYPE_PREV) {
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
				CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD) - 1);
			NPC_MsgDisp( meindex, talkerindex, 99);
		}
		break;
#else
	  case CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG:
		break;
#endif
		/*--�ꥯ�����ȤΤ��꤬�Ȥ���å�����--*/
	  case CHAR_WINDOWTYPE_WINDOWEVENT_REQTHANK:
			if(select == WINDOW_BUTTONTYPE_NEXT) {
				 CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
					CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)+1);
				NPC_MsgDisp( meindex, talkerindex, 22);
			}else if(select == WINDOW_BUTTONTYPE_PREV) {
				 CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
					CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)-1);
				NPC_MsgDisp( meindex, talkerindex, 22);
			}
		break;
		
		/*--�������ץȤΤ��꤬�Ȥ���å�����--*/
	  case CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANK:
			if(select == WINDOW_BUTTONTYPE_NEXT) {
				 CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
					CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)+1);
				NPC_MsgDisp( meindex, talkerindex, 82);
			}else if(select == WINDOW_BUTTONTYPE_PREV) {
				 CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
					CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)-1);
				NPC_MsgDisp( meindex, talkerindex, 82);
			}
		break;

		/*--���ߤΥ��٥�Ȥ�ߤ�뤫--*/
	  case CHAR_WINDOWTYPE_WINDOWEVENT_NOWEVENT:
		if(select == WINDOW_BUTTONTYPE_YES) {
			NPC_MsgDisp( meindex, talkerindex, 7);
		}else if(select == WINDOW_BUTTONTYPE_NO){
			NPC_MsgDisp( meindex, talkerindex, 1);
		}
		break;
		
		/*--�ꥯ�����ȤΥᥤ�󥦥���ɥ�--*/
	  case CHAR_WINDOWTYPE_WINDOWEVENT_REQMAINMSG:
		if(select == WINDOW_BUTTONTYPE_YES) {
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,0);
			NPC_MsgDisp( meindex, talkerindex, 2);
		}else if(select == WINDOW_BUTTONTYPE_NO) {
		
		}else if(select == WINDOW_BUTTONTYPE_NEXT) {
			 CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
						CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)+1);
			NPC_MsgDisp( meindex, talkerindex, 55);
		}else if(select == WINDOW_BUTTONTYPE_PREV) {
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
				CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)-1);
			NPC_MsgDisp( meindex, talkerindex, 55);
		}
		break;

		/*--�������Υᥤ��Υ�����ɥ�--*/
	  case CHAR_WINDOWTYPE_WINDOWEVENT_ACCMAINMSG:
		if(select == WINDOW_BUTTONTYPE_YES) {
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,0);
#ifdef _NEWEVENT
			NPC_MsgDisp( meindex, talkerindex, 81);
#else
			NPC_MsgDisp( meindex, talkerindex, 8);
#endif
		}else if(select == WINDOW_BUTTONTYPE_NO) {
		}else if(select == WINDOW_BUTTONTYPE_NEXT) {
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
				CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)+1);
			NPC_MsgDisp( meindex, talkerindex, 66);
		}else if(select == WINDOW_BUTTONTYPE_PREV) {
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
				CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)-1);
			NPC_MsgDisp( meindex, talkerindex, 66);
		}
		break;

#ifdef _NEWEVENT
	  case CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANKNEW:
		  if(select == WINDOW_BUTTONTYPE_OK){
			CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,0);
				NPC_MsgDisp( meindex, talkerindex, 8);
		  }else if(select == WINDOW_BUTTONTYPE_NEXT) {
				 CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
					CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)+1);
				NPC_MsgDisp( meindex, talkerindex, 81);
		  }else if(select == WINDOW_BUTTONTYPE_PREV) {
				 CHAR_setWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD,
					CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTTRD)-1);
				NPC_MsgDisp( meindex, talkerindex, 81);
		  }
		  break;
#endif
		/*--�ڥåȤ�  ��Ϳ���륦����ɥ�--*/
	  case CHAR_WINDOWTYPE_WINDOWPETSKILLSHOP:
			if(CHAR_getWorkInt( talkerindex ,CHAR_WORKSHOPRELEVANTTRD) ==0) {
				NPC_EventPetSkill( meindex, talkerindex, data);
				NPC_MsgDisp( meindex, talkerindex, 25);
			}else{
				NPC_EventPetSkill( meindex, talkerindex, data);
				NPC_MsgDisp( meindex, talkerindex, 88);
			}
		break;
	}
}


/*-------------------------
 *  ����ߤ����Ƥ��뤫
 --------------------------*/
int NPC_ExChangeManEventCheck( int meindex, int talker, char *buff1)
{
	char buff2[512];
	char buff3[128];
	char buf[512];
	int i = 1,j = 1;
	int loop = 0;
	
	NPC_Util_GetStrFromStrWithDelim( buff1, "EVENT", buf, sizeof( buf));

	/*-- EVENt�ξ����  �˸��Ƥ��� --*/
	while( getStringFromIndexWithDelim(buf, ",", i, buff2,sizeof( buff2))
	 !=FALSE )
	{
		i++;
		/*-- "&"������Τ�  ����� --*/
		if(strstr( buff2, "&")!=NULL){
			j = 1;
			loop = 0;
			while(getStringFromIndexWithDelim(buff2, "&", j, buff3, sizeof( buff3))
			 != FALSE )
			{
				j++;
				if(NPC_EventBigSmallCheck( meindex, talker, buff3) == FALSE)
				{
					loop = 1;
					break;
				}
			}
				
			if(loop == 0){
				i--;
				return i;
			}
		/*--��郎�ҤȤ�--*/
		}else{
			if(NPC_EventBigSmallCheck( meindex, talker, buff2) == TRUE) {
				i--;
				return i;
			}
		}
	
	}

	return -1;

}

/*---------------------------------
 *   ����������å�
 --------------------------------*/
BOOL NPC_EventBigSmallCheck(int meindex,int talker, char* buf)
{

	char buff2[128];
	int kosuu;
	
	/*--�ڥåȤΥ����å����Ϥ��äƤ�����ϰ�öPET��������å����� --*/
	if(strstr( buf, "PET") != NULL) {
		if(strstr( buf, "EV") != NULL) {
			if(NPC_PetLvCheck( meindex, talker, buf, 1) != FALSE) return TRUE;
		}else{
			if(NPC_PetLvCheck( meindex, talker, buf, 0) != FALSE) return TRUE;
		}
		return FALSE;
	}

	/*--�ʲ��ϡb  �̤ε�������å� --*/
	if(strstr( buf, "<") != NULL) {
		getStringFromIndexWithDelim( buf, "<", 2, buff2, sizeof( buff2));
		kosuu = atoi( buff2);
		getStringFromIndexWithDelim( buf, "<", 1, buff2, sizeof( buff2));

		if(NPC_EventFreeIfCheck( meindex, talker, buff2, kosuu, 1) == TRUE) {
			return TRUE;
		}

	}else if(strstr( buf, ">") != NULL) {
		getStringFromIndexWithDelim( buf, ">", 2, buff2, sizeof(buff2));
		kosuu = atoi(buff2);
		getStringFromIndexWithDelim( buf, ">" ,1, buff2, sizeof(buff2));

		if(NPC_EventFreeIfCheck( meindex, talker, buff2, kosuu, 2)==TRUE) {
			return TRUE;
		}

	}else if(strstr( buf, "!=" ) != NULL) {
		getStringFromIndexWithDelim( buf, "!=", 2, buff2, sizeof( buff2));
		kosuu = atoi( buff2);
		getStringFromIndexWithDelim( buf, "!=", 1, buff2, sizeof( buff2));
		if(NPC_EventFreeIfCheck( meindex, talker, buff2, kosuu, 3) == TRUE) {
			return TRUE;
		}
		
	}else if(strstr( buf, "=") != NULL) {
		getStringFromIndexWithDelim( buf, "=", 2, buff2, sizeof( buff2));
		kosuu = atoi( buff2);
		getStringFromIndexWithDelim( buf, "=", 1, buff2, sizeof( buff2));

		if(strstr( buf, "*") != NULL) {
			if(NPC_EventReduce( meindex, talker, buf) == TRUE){
				return TRUE;
			}
		}else if(NPC_EventFreeIfCheck( meindex, talker, buff2, kosuu, 0)
		== TRUE)
		{
			return TRUE;
		}
	}
	
	return FALSE;

}


/*-----------------------------------
 * ��  ��  �����Ƥ��뤫�Υ����å�
 *-----------------------------------*/
BOOL NPC_EventFreeIfCheck(int meindex,int talker,char* buf,int kosuu,int flg)
{

	/*--��٥�����å�--*/
	if(strcmp( buf, "LV") == 0) {
		if(NPC_EventLevelCheck( meindex, talker, kosuu, flg) == TRUE) {
			return TRUE;
		}
	}

	/*--����  ������å�--*/
	if(strcmp( buf, "ITEM") == 0) {
		if(NPC_EventItemCheck( meindex, talker, kosuu, flg) == TRUE) {
			return TRUE;
		}
	}

	/*--��  ���٥�ȥ����å�--*/
	if(strcmp( buf, "ENDEV") == 0) {
		if(NPC_ENDEventNoCheck( meindex, talker, kosuu, flg) == TRUE) {
			return TRUE;
		}
	}

	if(strcmp( buf, "NOWEV") == 0) {
		if(NPC_NOWEventNoCheck( meindex, talker, kosuu, flg) == TRUE) {
			return TRUE;
		}
	}

	if(strcmp( buf, "SP" ) == 0) {
		if(NPC_SavePointCheck( meindex, talker, kosuu, flg) == TRUE) {
			return TRUE;
		}
	}

	if(strcmp( buf, "TIME") == 0) {
		if(NPC_TiemCheck( meindex, talker, kosuu, flg) == TRUE) {
			return TRUE;
		}
	}

#ifdef _PROFESSION_SKILL			// WON ADD �H��¾�~�ޯ�
	if(strcmp(buf,"CLASS")==0){
		if(NPC_ActionClassCheck(meindex,talker,kosuu,flg)==TRUE)
				return TRUE;
	}
#endif

	if(strcmp( buf, "IMAGE") == 0) {
		if(NPC_ImageCheck( meindex, talker, kosuu, flg) == TRUE) {
			return TRUE;
		}
	}

#ifdef _ANGEL_SUMMON
	if(strcmp(buf,"ANGEL_NOW")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_DOING, 0)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_NOW")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_DOING, 0)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"ANGEL_OVER")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_HERO_COMPLETE, 0)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_OVER")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_HERO_COMPLETE, 0)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"ANGEL_OUT")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_TIMEOVER, 0)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_OUT")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_TIMEOVER, 0)==TRUE)
				return TRUE;
	}

	if(strcmp(buf,"ANGEL_I_NOW")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_DOING, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_I_NOW")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_DOING, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"ANGEL_I_OVER")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_HERO_COMPLETE, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_I_OVER")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_HERO_COMPLETE, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"ANGEL_I_OUT")==0){
		if( NPC_ActionMissionAngelCheck(meindex,talker,kosuu, MISSION_TIMEOVER, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp(buf,"HERO_I_OUT")==0){
		if( NPC_ActionMissionHeroCheck(meindex,talker,kosuu, MISSION_TIMEOVER, TRUE)==TRUE)
				return TRUE;
	}
	if(strcmp( buf, "HEROCNT") == 0) {
		//if(NPC_EventLevelCheck( meindex, talker, kosuu, flg) == TRUE) {
		if(NPC_HeroCompleteCountCheck( meindex, talker, kosuu, flg) == TRUE)
			return TRUE;
	}

#endif
#ifdef _NPC_EXCHANGEMANTRANS
	//�P�_���
	{
		//print("\n�P�_���");
		if(strcmp(buf,"TRANS")==0){
			int mytrans;
			mytrans = CHAR_getInt(talker,CHAR_TRANSMIGRATION);
			if(NPC_EventBigSmallLastCheck( kosuu, mytrans, flg) == TRUE)
				return TRUE;
		}
	}
#endif
#ifdef _PROSK99
	if(strstr( buf, "PROSK" ) != NULL) {//PROSK99>16 ¾�޵��Ťj�󵥩�99%���ƶq�n�W�L16��
		char *p=NULL;
		int i,level=0,count=0;
		CHAR_HaveSkill *pSkil;
		if( p = strstr( buf, "PROSK" ) )
			level = atoi(p+5);
		else 
			level = 100;
		for( i = 0; i < CHAR_SKILLMAXHAVE; i ++ ){
			if( pSkil = CHAR_getCharHaveSkill( talker, i ) )
				if( SKILL_getInt( &pSkil->skill, SKILL_LEVEL) >= level )
					++count;
		}	
		if(NPC_EventBigSmallLastCheck( kosuu, count, flg) == TRUE)
			return TRUE;
	}
#endif

#ifdef _ADD_reITEM
	if(strstr( buf, "reITEM" ) != NULL) {
		int i,count=0,itemindex=-1;
		for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ){
			itemindex = CHAR_getItemIndex( talker , i );
			if( !ITEM_CHECKINDEX( itemindex) )
				++count;
		}
		if(NPC_EventBigSmallLastCheck( kosuu, count, flg) == TRUE)
			return TRUE;
	}
#endif

	return FALSE;

}

/*--------------------------------
 *  �ڥåȤΥ�٥������å�����
 ---------------------------------*/
BOOL NPC_PetLvCheck(int meindex,int talker,char *buf,int mode)
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buff2[16];
	char buf3[8];
	int petno;
	int baseno;
	int petlevel = 0;
	int flg=0;
	int mypetlevel = 0;
	int i = 0;
	int petindex;
	char name[512];
	int cnt = 0;
	int loop;
	int pettran=-1;

	/*--���ꤵ�줿�ڥåȤΥʥ�С�  ID)���  --*/
	getStringFromIndexWithDelim( buf, "-", 2, buff2, sizeof( buff2));
	if(strstr(buff2,"*") != NULL) {
		getStringFromIndexWithDelim( buff2, "*", 1, buf3, sizeof( buf3));
		petno = atoi( buf3);
		getStringFromIndexWithDelim( buff2, "*", 2, buf3, sizeof( buf3));
		loop = atoi( buf3);

	}else{
		petno = atoi( buff2);
		loop = 1;

	}
	if(getStringFromIndexWithDelim( buf, "-", 3, buff2, sizeof( buff2)))
		{
			pettran = atoi( buff2);
		}
	/*--�ڥå�  �������Ƥ��뤫�Υ����å�--*/
	for(i=0; i < CHAR_MAXPETHAVE ; i++) {
		if(loop == cnt) return TRUE;
		
		petindex = CHAR_getCharPet( talker, i);
		if( petindex == -1  )  continue;
		if(pettran != -1)
			{
				if(pettran!=CHAR_getInt( petindex, CHAR_TRANSMIGRATION))
					continue;
			}
		/*--�ڥåȤ�ID���  ���Ƥ���--*/
		baseno = CHAR_getInt( petindex, CHAR_PETID);

		/* �ڥåȤ�ID�ȡb���ߤΥ⡼�ɤ�Ĵ�٤� */
		if( (baseno == petno) && (CHAR_getInt( petindex, CHAR_ENDEVENT)
		 == mode))
		 {
			/*--��٥������å�����--*/
			mypetlevel = CHAR_getInt( petindex, CHAR_LV);
			getStringFromIndexWithDelim( buf, "-", 1, buff2, sizeof( buff2));

			if(strstr( buf, "<") != NULL) {
				flg = 1;
				getStringFromIndexWithDelim( buff2, "<", 2, buf3, sizeof(buf3));
			}else if(strstr( buf, ">") != NULL) {
				getStringFromIndexWithDelim( buff2, ">", 2, buf3, sizeof(buf3));
				flg = 2;
			}else if(strstr( buf, "=") != NULL) {
				getStringFromIndexWithDelim( buff2, "=", 2, buf3, sizeof(buf3));
				flg = 0;
			}

			petlevel = atoi( buf3);

			if(NPC_EventBigSmallLastCheck( petlevel, mypetlevel, flg) == TRUE) {
				/*--�ڥåȤ�  �������ꤵ��Ƥ�����  ���Υ����å�  ��--*/
				if(CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC)==1) {
					char *usename;
					
					/*--����ե������  ��  ��--*/
					if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
						print("PetLevel:GetArgStrErr");
						return FALSE;
					}

					usename = CHAR_getUseName( petindex);
					NPC_Util_GetStrFromStrWithDelim( argstr, "Pet_Name",
													name,sizeof( name));
	
					if(strcmp( name, usename) == 0)
					{
						cnt++;
						continue;
					}
				}else {
					CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 0);
					cnt++;
					continue;
				}
			}
		}
	}
	
	if(loop == cnt) return TRUE;
	if(i == CHAR_MAXPETHAVE) return FALSE;

	return FALSE;
}


/*--------------------------------
 *  �ڥåȤΥ�٥������å�����
 ---------------------------------*/
int NPC_PetLvCheckType2(int petindex,int meindex,int talker,char *buf,int mode)
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buff2[16];
	char buf3[8];
	int petno;
	int baseno;
	int petlevel = 0;
	int flg = 0;
	int mypetlevel = 0;
	char name[512];
	
	/*--���ꤵ�줿�ڥåȤΥʥ�С�  ID)���  --*/
	getStringFromIndexWithDelim( buf, "-", 2, buff2, sizeof( buff2));
	if(strstr(buff2,"*") != NULL) {
		getStringFromIndexWithDelim( buff2, "*", 1, buf3, sizeof( buf3));
		petno = atoi( buf3);
	}else{
		petno = atoi( buff2);
	}

	
	/*--�ڥåȤ�ID���  ���Ƥ���--*/
	baseno = CHAR_getInt( petindex, CHAR_PETID);

	/*--�ڥåȤ�ID�ȡb���ߤΥ⡼�ɤ�Ĵ�٤�--*/
	if( (baseno == petno) && (CHAR_getInt( petindex, CHAR_ENDEVENT)
	 == mode))
	{
		/*--��٥������å�����--*/
		mypetlevel=CHAR_getInt( petindex, CHAR_LV);
		getStringFromIndexWithDelim( buf, "-", 1,buff2,sizeof(buff2));

		if(strstr( buf, "<") != NULL) {
			flg = 1;
			getStringFromIndexWithDelim( buff2, "<", 2,
										 buf3, sizeof(buf3));
		}else if(strstr( buf, ">") != NULL) {
			getStringFromIndexWithDelim( buff2, ">", 2, buf3, sizeof(buf3));
			flg = 2;
		}else if(strstr( buf, "=") != NULL) {
			getStringFromIndexWithDelim( buff2, "=", 2, buf3, sizeof(buf3));
			flg = 0;
		}
		
		petlevel = atoi(buf3);

		if(NPC_EventBigSmallLastCheck( petlevel, mypetlevel, flg)
		 == TRUE)
		{
			/*--�ڥåȤ�  �������ꤵ��Ƥ�����  ���Υ����å�  ��--*/
			if(CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC) == 1) {
				char *usename;

				/*--����ե������  ��  ��--*/
				if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
					print("PetLevel:GetArgStrErr");
					return FALSE;
				}
				
				usename = CHAR_getUseName( petindex);
				NPC_Util_GetStrFromStrWithDelim(argstr,"Pet_Name",
												name, sizeof( name));
	
				if(strcmp( name, usename) == 0)
				{
					return TRUE;
				}
			}else {
				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 0);
				return TRUE;
			}
		}
	}

	return FALSE;

}

/*------------------------
 *��  ���Υ����å�
 --------------------------*/
BOOL NPC_SavePointCheck(int meindex,int talker,int shiftbit,int flg)
{
	int point;

	point = CHAR_getInt( talker, CHAR_SAVEPOINT);
	
	if( (point & ( 1<< shiftbit))  == ( 1<< shiftbit)) {
		if(flg == 3) return FALSE;
		return TRUE;
	}

	if(flg == 3) return TRUE;
	return FALSE;
}

/*------------------------
 *���᡼���Υ����å�
 --------------------------*/
BOOL NPC_ImageCheck(int meindex,int talker,int kosuu,int flg)
{
	int image;

	image = CHAR_getInt( talker, CHAR_IMAGETYPE);
	
	if(NPC_EventBigSmallLastCheck( image, kosuu, flg) == TRUE) {
		return TRUE;
	}
	return FALSE;

}


/*-----------------------------------------
 * ���֤Υ����å���Ԥ�(�����ޤ�����)
 ------------------------------------------*/
BOOL NPC_TiemCheck(int meindex,int talker,int time,int flg)
{
	LSTIME		nowlstime;
	int now;

	RealTimeToLSTime( NowTime.tv_sec, &nowlstime);
	now = getLSTime( &nowlstime);

	if(NPC_EventBigSmallLastCheck( time, now, flg) == TRUE) {
		return TRUE;
	}
	return FALSE;

}

/*------------------------------
 *--Item������å�				
 -------------------------------*/
BOOL NPC_EventItemCheck(int meindex,int talker,int itemNo,int flg)
{
	int i;
	int itemindex = -1;
	int id;

	//Change fix ���ˬd�����b���W���D��
	for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ) {

		itemindex=CHAR_getItemIndex( talker , i );
		if( ITEM_CHECKINDEX( itemindex) ) {
			id=ITEM_getInt( itemindex ,ITEM_ID );

			if(NPC_EventBigSmallLastCheck( itemNo, id, flg) == TRUE) {
				if(flg == 0)	return TRUE;
				continue;
			}else{
				if(flg == 0) continue;
				return FALSE;
			}
		}
	}

	if(flg == 3) return TRUE;

	return FALSE;

}


/*------------------------------
 * level������å�				
 -------------------------------*/
BOOL NPC_EventLevelCheck(int meindex,int talker,int level,int flg)
{

	int mylevel;
	mylevel = CHAR_getInt(talker,CHAR_LV);

	if(NPC_EventBigSmallLastCheck( level, mylevel, flg) == TRUE)
	{
		if(flg == 3) return FALSE;
		return TRUE;
	}

	if(flg == 3) return TRUE;
	return FALSE;

}



/*------------------------------
 * ENDEventNo������å�
 -------------------------------*/
BOOL NPC_ENDEventNoCheck(int meindex,int talker,int shiftbit,int flg)
{

	if(NPC_EventCheckFlg( talker, shiftbit) == TRUE) {
		if(flg == 3) return FALSE;
		return TRUE;
	}

	if(flg == 3) return TRUE;
	return FALSE;
}


/*------------------------------
 * NOWEventNo������å�
 -------------------------------*/
BOOL NPC_NOWEventNoCheck(int meindex,int talker,int shiftbit,int flg)
{
	if(NPC_NowEventCheckFlg( talker, shiftbit) == TRUE) {
		if(flg == 3) return TRUE;
		return TRUE;
	}

	if(flg == 3) return TRUE;
	return FALSE;
}

/*---------------------------------------
 *    �������å�
 --------------------------------------*/
BOOL NPC_EventBigSmallLastCheck(int point1,int mypoint,int flg)
{

	if(flg == 0) {
		if(point1 == mypoint) {
			return TRUE;
		}
	}else if(flg == 1) {
		if(mypoint < point1) {
			return TRUE;
		}
	}else if(flg == 2) {
		if(mypoint > point1) {
			return TRUE;
		}
	}else if(flg == 3) {
		if(point1 != mypoint) {
			return TRUE;
		}
	}
	return FALSE;
}


/*---------------------------------
 *��å��������  ����
 *----------------------------------*/
#ifdef _CHANNEL_MODIFY
extern int *piOccChannelMember;
#endif
void NPC_MsgDisp(int meindex,int talker,int num)
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
#ifdef _NEWEVENT
	char buf[4096];
	char token2[4096];
	char buf2[4096];
	char token[4096];
	char tmp[4096];
#else
	char buf[1024*2];
	char token2[50*14];
	char buf2[512];
	char token[512];
	char tmp[32];
#endif
	int i = 1;
	int work = 0;
	int EvNo = 0;
	int fd = getfdFromCharaIndex( talker);
	int buttontype = WINDOW_BUTTONTYPE_OK;
	int windowtype = CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG;
	int evch;

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
		print("MsgDisp:GetArgStrErr");
		print("NPCName=%s\n", CHAR_getChar( meindex, CHAR_NAME));
		return ;
	}
	if(num == 22 || num == 82 || num == 55 || num == 66) {
		int pwork;

		pwork = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
		pwork = (pwork / 100) -1 ;
		getStringFromIndexWithDelim( argstr, "EventEnd", pwork, buf,sizeof( buf));
		
	}else{
		/*����ե������ʬ�䤹��*/
		while(getStringFromIndexWithDelim( argstr, "EventEnd", i, buf, sizeof(buf))
		 != FALSE)
		{
			i++;
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 0);
			
			/*--���٥�ȥʥ�С��򥲥å�--*/
			NPC_Util_GetStrFromStrWithDelim( buf, "EventNo", buf2, sizeof( buf2));
			if(strstr( buf2, "-1") != 0) {
				EvNo = -1;
			}else{
				EvNo = atoi( buf2);
			}
			EvNo = atoi( buf2);

			/*--�ե饰�����å���Ԥ��e���Ǥˡb���Υ��٥�Ȥ���  ���Ƥ�����ϱ���--*/
			if(NPC_EventCheckFlg( talker, EvNo) == TRUE) continue;

			/*--������ɤΥ����å���Ԥ�--*/
			if(NPC_Util_GetStrFromStrWithDelim( buf,"Pet_Name", buf2,sizeof( buf2)) 
			 != NULL )
			{
				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 1);
			}

			/*--������ɤΥ����å���Ԥ�--*/
			if(NPC_Util_GetStrFromStrWithDelim( buf,"KeyWord", buf2,sizeof( buf2) ) 
			 != NULL)
			{
					if(CHAR_getWorkInt( talker ,CHAR_WORKSHOPRELEVANT) != i) continue;
			}

			evch = NPC_ExChangeManEventCheck( meindex, talker, buf);

			if(evch != -1) {

			/*--���٥�Ⱦ��Υ����å�--*/
		//	if(NPC_ExChangeManEventCheck( meindex, talker, buf) == TRUE) {
				break;
			}
		}
	}
	
	//print(" NPC_MsgDisp:%d ", num);
	
	switch( num) {
		  case 0:
			/*���ߤΥ��٥�Ȥ�ߤ�뤫�ɤ�����  ����å�����--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "StopMsg", 
												buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token,sizeof( buf2), buf2);
				buttontype = WINDOW_BUTTONTYPE_YESNO;
				windowtype = CHAR_WINDOWTYPE_WINDOWEVENT_NOWEVENT;
			}
		break;
		
		  case 1:
		  	/*--���뤫�ɤ�����  ����������NO�����������Υ�å�����--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "NoStopMsg",
												 buf2, sizeof( buf2))
			 != NULL)
			{
				strcpysafe( token, sizeof(buf2), buf2);

			}
		break;
		
		  case 7:
			/*--���ߤΥ��٥�Ȥ�ߤ᤿�Ȥ��Υ�å�����--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "EndStopMsg",
												 buf2, sizeof( buf2))
			 != NULL)
			{
				strcpysafe( token, sizeof( buf2), buf2);
			
				/*--���٥��  ��  �äƤ������Ͼõ�--*/
				NPC_EventAdd( meindex, talker, 1);
			
				/*--�ե饰��ä�--*/
				NPC_NowEventSetFlgCls( talker, EvNo);
			
				/*--�Ǥä��Τǡb    �򤵹���--*/
				if(CHAR_getInt( talker, CHAR_CHARM) > 0) {
					CHAR_setInt( talker, CHAR_CHARM, CHAR_getInt(talker,CHAR_CHARM) -1);
					CHAR_complianceParameter( talker );
					CHAR_send_P_StatusString( talker, CHAR_P_STRING_CHARM);
					NPC_CharmStatus( meindex, talker);
				}
			}
		break;
		
		  case 2:
				/*--���꤬�Ȥ���å�����--*/
				if( NPC_Util_GetStrFromStrWithDelim( buf, "ThanksMsg",
													 buf2, sizeof( buf2) )
				 != NULL)
				{
					strcpysafe( token,sizeof( buf2), buf2);
					/*--���٥��  �ե饰�򥻥å�--*/
					if(NPC_EventAdd(meindex, talker, 0) != FALSE) {
						NPC_NowEventSetFlg( talker, EvNo);
					}
					else {
						return;
					}
				
				
					if(NPC_Util_GetStrFromStrWithDelim( buf, "EndSetFlg", 
													buf2, sizeof( buf2) )
					!=NULL)
					{
						char buf3[16];
						int k = 1;
						if(EvNo != -1) NPC_NowEventSetFlgCls( talker, EvNo);
						
						while(getStringFromIndexWithDelim(buf2 , "," ,k, 
												buf3, sizeof(buf3))
						 !=FALSE )
						{	
							k++;
							NPC_EventSetFlg( talker, atoi( buf3));
						}
					}

					if( NPC_Util_GetStrFromStrWithDelim( buf, "CleanFlg",
													 buf2, sizeof( buf2)) 
					 != NULL) {
					 	char buf3[16];
						int k = 1;

					 	k= 1 ;
						while(getStringFromIndexWithDelim(buf2 , "," , k, 
															buf3, sizeof(buf3))
						!= FALSE )
						{
							NPC_NowEndEventSetFlgCls( talker, atoi(buf3));
							k++;
						}
					}

				
				
				}
				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, 1);
				work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
				work = work + ( i * 100);
				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, work);
				
				if(NPC_Util_GetStrFromStrWithDelim( buf,"NpcWarp",
										token2,sizeof( token2) )
				!= NULL){
					NPC_EventWarpNpc(meindex,token2);
				}

				/*--���ڡ�����������ϥܥ���襦����ɥ��Υ����פ򤫤���--*/
				if(strstr( buf, "ThanksMsg2") != NULL) {
					/*��������������*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_REQTHANK,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}

		break;

		   case 22:
				
				work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
				work = work % 100;

				if(work == 1){
					if( NPC_Util_GetStrFromStrWithDelim( buf, "ThanksMsg",
														 buf2, sizeof( buf2) )
					 != NULL)
					{
						strcpysafe( token,sizeof( buf2), buf2);
					}

					/*--���ڡ�����������ϥܥ���襦����ɥ��Υ����פ򤫤���--*/
					if(strstr( buf, "ThanksMsg2") != NULL) {
					
						/*��������������*/
						lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
									WINDOW_BUTTONTYPE_NEXT,
									CHAR_WINDOWTYPE_WINDOWEVENT_REQTHANK,
									CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
									token);
						return;
					}
				
				}else{
					
					sprintf( tmp, "ThanksMsg%d", work);
					if( NPC_Util_GetStrFromStrWithDelim( buf, tmp,
														 buf2, sizeof( buf2) )
					 != NULL) {
						strcpysafe( token,sizeof( buf2), buf2);
					}
	
					work++;
					sprintf( tmp, "ThanksMsg%d", work);
	
	
					/*--���ڡ�����������ϥܥ���襦����ɥ��Υ����פ򤫤���--*/
					if(strstr( buf, tmp) != NULL) {
						
						/*��������������*/
						lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
									WINDOW_BUTTONTYPE_NEXT,
									CHAR_WINDOWTYPE_WINDOWEVENT_REQTHANK,
									CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
									token);
						return;
					}else{
						lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
									WINDOW_BUTTONTYPE_OK,
									CHAR_WINDOWTYPE_WINDOWEVENT_REQTHANK,
									CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
									token);
						return;
					}
				}
		break;

		  case 25:
			  /*--���꤬�Ȥ���å�����--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "ThanksMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token,sizeof( buf2), buf2);
				/*--���٥��  �ե饰�򥻥å�--*/
				if(NPC_EventAdd(meindex, talker, 2) != FALSE) {
					NPC_NowEventSetFlg( talker, EvNo);
				}
			}
		break;


		  case 3:
			/*--  �̤Υ�å�����--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "NomalMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe(token, sizeof( buf2), buf2);
			}
		break;

		  case 4:
		  	/*--����  �ब���äѤ��λ��Υ�å�����--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "ItemFullMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe(token, sizeof( buf2), buf2);
			}
		break;

		case 5:
			/*--��  �򤹤�ݤΥᥤ��Υ�å�����--*/
				if( NPC_Util_GetStrFromStrWithDelim( buf, "RequestMsg",
													 buf2, sizeof( buf2) )
				 != NULL)
				{
          
          if(strstr(buf2,"%8d") != NULL) {
						char buf3[64];
						int cost;
						
						NPC_Util_GetStrFromStrWithDelim( buf, "DelStone",
													 buf3, sizeof( buf3) );
						cost = NPC_EventGetCost( meindex, talker, buf3);
						
						sprintf(token,buf2,cost);

					}else{
						strcpysafe( token,sizeof( buf2), buf2);
					}
					buttontype = WINDOW_BUTTONTYPE_YESNO;
					windowtype = CHAR_WINDOWTYPE_WINDOWEVENT_REQMAINMSG;
				}

				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, 1);
				work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
				work = work + ( i * 100);
				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, work);

				/*--���ڡ�����������ϥܥ��󥦥���ɥ��Υ����פ򤫤���--*/
				if(strstr( buf, "RequestMsg2") != NULL) {
					/*��������������*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_REQMAINMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
		break;

	   case 55:
				
				work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
				work = work % 100;

				if(work == 1){
					if( NPC_Util_GetStrFromStrWithDelim( buf, "RequestMsg",
														 buf2, sizeof( buf2) )
					 != NULL)
					{
						strcpysafe( token, sizeof( buf2), buf2);
					}

					/*--���ڡ�����������ϥܥ���襦����ɥ��Υ����פ򤫤���--*/
					if(strstr( buf, "RequestMsg2") != NULL){
						/*��������������*/
						lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
									WINDOW_BUTTONTYPE_NEXT,
									CHAR_WINDOWTYPE_WINDOWEVENT_REQMAINMSG,
									CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
									token);
						return;
					}
				}else{
					
					sprintf( tmp, "RequestMsg%d", work);
					if( NPC_Util_GetStrFromStrWithDelim( buf, tmp,
														 buf2, sizeof( buf2) )
					 != NULL){
						strcpysafe( token,sizeof( buf2), buf2);
							
					}
					work++;
					sprintf( tmp, "RequestMsg%d", work);
					/*--���ڡ�����������ϥܥ���襦����ɥ��Υ����פ򤫤���--*/
					if(strstr( buf, tmp) != NULL) {
						
						/*��������������*/
						lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
									WINDOW_BUTTONTYPE_NEXT,
									CHAR_WINDOWTYPE_WINDOWEVENT_REQMAINMSG,
									CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
									token);
						return;
					}else{
						lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
									WINDOW_BUTTONTYPE_YESNO,
									CHAR_WINDOWTYPE_WINDOWEVENT_REQMAINMSG,
									CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
									token);
						return;
					}
				}
		break;


		  case 6:
			  //print(" 6_����? ");
			/*--�������κݤΥᥤ��Υ�å�����--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "AcceptMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				if(strstr(buf2,"%8d") != NULL) {
					char buf3[64];
					int cost;
						
					NPC_Util_GetStrFromStrWithDelim( buf, "DelStone",
											 buf3, sizeof( buf3) );
					cost = NPC_EventGetCost( meindex, talker, buf3);
						
					sprintf( token, buf2, cost);
				}else{
					strcpysafe( token, sizeof( buf2), buf2);
				}
				buttontype = WINDOW_BUTTONTYPE_YESNO;
				windowtype = CHAR_WINDOWTYPE_WINDOWEVENT_ACCMAINMSG;
			}
	
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, 1);
			work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
			work = work + ( i * 100);
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, work);

			/*--���ڡ�����������ϥܥ��󥦥���ɥ��Υ����פ򤫤���--*/
			if(strstr( buf, "AcceptMsg2") != NULL) {
				/*��������������*/
				lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
							WINDOW_BUTTONTYPE_NEXT,
							CHAR_WINDOWTYPE_WINDOWEVENT_ACCMAINMSG,
							CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
							token);
				return;
			}
		break;


	   case 66:
		   //print(" 66_����? ");
			work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
			work = work % 100;

			if(work == 1){
				if( NPC_Util_GetStrFromStrWithDelim( buf, "AcceptMsg",
													 buf2, sizeof( buf2) )
				 != NULL)
				{
					strcpysafe( token,sizeof( buf2), buf2);
				}

				/*--���ڡ�����������ϥܥ���襦����ɥ��Υ����פ򤫤���--*/
				if(strstr( buf, "AcceptMsg2") != NULL) {
					/*��������������*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_ACCMAINMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
			}else{
			
				sprintf( tmp, "AcceptMsg%d", work);
				if( NPC_Util_GetStrFromStrWithDelim( buf, tmp,
													 buf2, sizeof( buf2) )
				 != NULL){
					strcpysafe( token,sizeof( buf2), buf2);
				}
				work++;
				sprintf( tmp, "AcceptMsg%d", work);
				/*--���ڡ�����������ϥܥ���襦����ɥ��Υ����פ򤫤���--*/
				if(strstr( buf, tmp) != NULL) {
					
					/*��������������*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_ACCMAINMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}else{
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_YESNO,
								CHAR_WINDOWTYPE_WINDOWEVENT_ACCMAINMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
			}
		break;

		case 8:
				//print(" 8_����? ");
			
				if( NPC_Util_GetStrFromStrWithDelim( buf, "ThanksMsg",
													buf2, sizeof( buf2) )
				 != NULL)
				{
					strcpysafe( token, sizeof( buf2), buf2);
					/*--���٥��  �ե饰�򥻥å�--*/
					if(NPC_AcceptDel( meindex, talker,0) == FALSE){
						return ;
					}
					
					if(NPC_Util_GetStrFromStrWithDelim( buf, "EndSetFlg", 
						buf2, sizeof( buf2) )
						!=NULL)
					{
						char buf3[16];
						int k = 1;
						if(EvNo != -1) NPC_NowEventSetFlgCls( talker, EvNo);
						
						while(getStringFromIndexWithDelim(buf2 , "," ,k, 
							buf3, sizeof(buf3))
							!=FALSE )
						{	
							k++;
							NPC_EventSetFlg( talker, atoi( buf3));
						}
					}
					
					if( NPC_Util_GetStrFromStrWithDelim( buf, "Charm",
						buf2, sizeof( buf2))
						!=NULL)
					{
						/*--�����ޤ�  ����b    ��干��---*/
						if( CHAR_getInt( talker, CHAR_CHARM) < 100 && EvNo > 0 ){
							if(CHAR_getInt( talker, CHAR_CHARM) + atoi(buf2) > 100){
								CHAR_setInt( talker, CHAR_CHARM,100);
								
							}else{
								CHAR_setInt( talker, CHAR_CHARM, 
									CHAR_getInt(talker,CHAR_CHARM) + atoi(buf2));
								
							}
							
							CHAR_complianceParameter( talker );
							CHAR_send_P_StatusString( talker, CHAR_P_STRING_CHARM);
							NPC_CharmStatus( meindex, talker);
						}
					}
					
					if( NPC_Util_GetStrFromStrWithDelim( buf, "CleanFlg",
						buf2, sizeof( buf2)) 
						!= NULL) {
						char buf3[16];
						int k = 1;
						
						k= 1 ;
						while(getStringFromIndexWithDelim(buf2 , "," , k, 
							buf3, sizeof(buf3))
							!= FALSE )
						{
							NPC_NowEndEventSetFlgCls( talker, atoi(buf3));
							k++;
						}
					}
					
					
					// Robin add �M��¾�~
					if( NPC_Util_GetStrFromStrWithDelim( buf, "PROFESSION",
						buf2, sizeof( buf2)) != NULL)
					{
						int p_class = atoi( buf2);
						//Change fix 2004/07/05
						for( i=0; i<CHAR_STARTITEMARRAY; i++){ //�ˬd�˳Ƥ��D��
							if( ITEM_CHECKINDEX( CHAR_getItemIndex( talker , i ) )){
								if( CHAR_getInt( talker, PROFESSION_CLASS ) != 0
									&& CHAR_getInt( talker, PROFESSION_CLASS ) != ITEM_getInt( CHAR_getItemIndex( talker , i ), ITEM_NEEDPROFESSION) 
									&& ITEM_getInt( CHAR_getItemIndex( talker , i ), ITEM_NEEDPROFESSION) > 0 ){
									p_class = 1;
									CHAR_talkToCli( talker, -1, "�Х���¾�~�˳Ƴq�q���U�I", CHAR_COLORWHITE);
									break;
								}
							}
						}
						if( p_class == 0)
						{
							print(" ====�M��¾�~==== ");
							
							// �٭��I��	
							//if( PROFESSION_RESTORE_POINT( talker ) != 1 )	return FALSE;
							
							// �]�w¾�~�k�s
							CHAR_setInt( talker, PROFESSION_CLASS, p_class );	
#ifdef _CHANNEL_MODIFY
							if(CHAR_getInt(talker,PROFESSION_CLASS) > 0){
								int i,pclass = CHAR_getInt(talker,PROFESSION_CLASS) - 1;
								for(i=0;i<getFdnum();i++){
									if(*(piOccChannelMember + (pclass * getFdnum()) + i) == talker){
										*(piOccChannelMember + (pclass * getFdnum()) + i) = -1;
										break;
									}
								}
							}
#endif
							
							// �]�w¾�~���� 0
							CHAR_setInt( talker, PROFESSION_LEVEL, 0 );
							
							// �ޯ��I���k�s
							CHAR_setInt(talker, PROFESSION_SKILL_POINT, 0 );
							
							// �٭�ܩ�
							for( i=0; i<3; i++ )
								CHAR_setInt( talker, PROFESSION_FIRE_R+i, /*CHAR_getInt( toindex, PROFESSION_FIRE_R+i) + value*/0 );
							
							// �٭�ŮvMP�W��
							CHAR_setInt( talker , CHAR_MAXMP , 100 );
							
							// �R���Ҧ��ޯ�
							CHAR_CHAT_DEBUG_delsk( talker, "all" );
							
							// ?? Andy add
							CHAR_setInt( talker , ATTACHPILE, 0);
							
							// �M�����ȺX��
							NPC_NowEndEventSetFlgCls( talker, 145);
							NPC_NowEndEventSetFlgCls( talker, 146);
							NPC_NowEndEventSetFlgCls( talker, 147);					
							
							CHAR_sendStatusString( talker , "S");
							
							CHAR_sendCToArroundCharacter( CHAR_getWorkInt( talker , CHAR_WORKOBJINDEX ));
							
						}
						
					}

#ifdef _ANGEL_SUMMON
					// �����l�����
					if( NPC_Util_GetStrFromStrWithDelim( buf, "MISSIONOVER",
						buf2, sizeof( buf2)) != NULL)
					{
						int mindex;
						int mission;
						char nameinfo[64];
						
						mission = atoi( buf2);
						mindex = checkIfAngel( talker);

						if( mission == missiontable[mindex].mission )
						{
							char msg[1024];
							
							print(" ====�����l�����==== ");

							CHAR_setInt( talker, CHAR_HEROCNT, 
								CHAR_getInt( talker, CHAR_HEROCNT)+1 );
							getMissionNameInfo( talker, nameinfo);
							saacproto_ACMissionTable_send( acfd, MISSION_HERO_COMPLETE, 4, nameinfo, "");
							sprintf( msg, " �������� i:%d m:%d %s ", mindex, mission, nameinfo);
							print( msg);
							LogAngel( msg );
						}
						
					}

					// �M���l�����
					if( NPC_Util_GetStrFromStrWithDelim( buf, "MISSIONCLEAN",
						buf2, sizeof( buf2)) != NULL)
					{
						int mindex;
						int mission;
						char nameinfo[64];
						
						mission = atoi( buf2);
						mindex = checkIfAngel( talker);

						if( mission == missiontable[mindex].mission )
						{
							print(" ====�M���l�����==== ");

							getMissionNameInfo( talker, nameinfo);
							saacproto_ACMissionTable_send( acfd, mindex, 3, nameinfo, "");
						}
						
					}
#endif
				}
				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, 1);
				work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
				work = work + ( i * 100 );
				CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, work);
	
				if(NPC_Util_GetStrFromStrWithDelim( buf,"NpcWarp",
													token2,sizeof( token2) )
					!= NULL){
						NPC_EventWarpNpc(meindex,token2);
				}
				/*--���ڡ�����������ϥܥ��󥦥���ɥ��Υ����פ򤫤���--*/
#ifdef _NEWEVENT
#else
				if(strstr( buf, "ThanksMsg2") != NULL){
					/*��������������*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANK,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
#endif
		break;

#ifdef _NEWEVENT
		case 81:
			//print(" 81_����? ");

			work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
			work = work % 100;
			sprintf(tmp, "ThanksMsg%d", work + 1);
			if(work == 0 && strstr(buf, tmp) == NULL)
			{
				if(NPC_Util_GetStrFromStrWithDelim(buf, "ThanksMsg",
					buf2, sizeof( buf2)) != NULL)
					strcpysafe( token,sizeof( buf2), buf2);
				if(strstr(buf, "ThanksMsg2") != NULL)
					lssproto_WN_send(fd, WINDOW_MESSAGETYPE_MESSAGE,
					WINDOW_BUTTONTYPE_NEXT,
					CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANKNEW,
					CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
					token);
				else
					lssproto_WN_send(fd, WINDOW_MESSAGETYPE_MESSAGE,
					WINDOW_BUTTONTYPE_OK,
					CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANKNEW,
					CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
					token);
				return;
			}
			else
			{
				if(NPC_Util_GetStrFromStrWithDelim(buf, tmp,
					buf2, sizeof( buf2)) != NULL)
					strcpysafe( token,sizeof( buf2), buf2);
				work++;
				sprintf(tmp, "ThanksMsg%d", work + 1);
				
				if(strstr(buf, tmp) != NULL)
				{
					lssproto_WN_send(fd, WINDOW_MESSAGETYPE_MESSAGE,
						WINDOW_BUTTONTYPE_NEXT,
						CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANKNEW,
						CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
						token);
					return;
				}
				else
				{
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
						WINDOW_BUTTONTYPE_OK,
						CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANKNEW,
						CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
						token);
					return;
				}
			}
			
			break;
#endif	
	
	   case 82:
			work = CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTTRD);
			work = work % 100;

			if(work == 1){
				if( NPC_Util_GetStrFromStrWithDelim( buf, "ThanksMsg",
													 buf2, sizeof( buf2))
				 != NULL)
				{
					strcpysafe( token,sizeof( buf2), buf2);
				}

				/*--���ڡ�����������ϥܥ���襦����ɥ��Υ����פ򤫤���--*/
				if(strstr( buf, "ThanksMsg2") != NULL){
					/*��������������*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANK,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
			}else{
				
				sprintf( tmp, "ThanksMsg%d", work);
				if( NPC_Util_GetStrFromStrWithDelim( buf, tmp,
													 buf2, sizeof( buf2) )
				!= NULL)
				{
					strcpysafe( token,sizeof( buf2), buf2);
				}
				
				work++;
				sprintf( tmp, "ThanksMsg%d", work);

				/*--���ڡ�����������ϥܥ���襦����ɥ��Υ����פ򤫤���--*/
				if(strstr( buf, tmp) != NULL) {
						
					/*��������������*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANK,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}else{
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_OK,
								CHAR_WINDOWTYPE_WINDOWEVENT_ACCTHANK,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
			}
			break;

		case 88:
			/*--�������ץȤ�--*/
			/*--���꤬�Ȥ���å�����--*/
			/*--�ڥåȤ�  ��--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "ThanksMsg",
												buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token, sizeof( buf2), buf2);
				/*--���٥��  �ե饰�򥻥å�--*/
				if(NPC_AcceptDel( meindex, talker,1) == FALSE){
					return ;
				}
				
				if(NPC_Util_GetStrFromStrWithDelim( buf, "EndSetFlg", 
												buf2, sizeof( buf2) )
				!=NULL)
				{
					char buf3[16];
					int k = 1;
				
					if(EvNo != -1) NPC_NowEventSetFlgCls( talker, EvNo);
					
					while(getStringFromIndexWithDelim(buf2 , "," ,k, 
											buf3, sizeof( buf3))
					 !=FALSE )
					{	
						k++;
						NPC_EventSetFlg( talker, atoi( buf3));
					}
				}

				if( NPC_Util_GetStrFromStrWithDelim( buf, "Charm",
													buf2, sizeof( buf2))
				!=NULL)
				{
					/*--�����ޤ�  ����b    ��干��---*/
					if( CHAR_getInt( talker, CHAR_CHARM) < 100 && EvNo > 0 ){
						if(CHAR_getInt( talker, CHAR_CHARM) + atoi(buf2) > 100){
							CHAR_setInt( talker, CHAR_CHARM,100);
						}else{
							CHAR_setInt( talker, CHAR_CHARM, 
									CHAR_getInt(talker,CHAR_CHARM) + atoi( buf2));
						}
						CHAR_complianceParameter( talker );
						CHAR_send_P_StatusString( talker, CHAR_P_STRING_CHARM);
						NPC_CharmStatus( meindex, talker);
					}
				}
			}

		break;


		  case 9:
			
		  	if( NPC_Util_GetStrFromStrWithDelim( buf, "NomalWindowMsg",
												buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token, sizeof( buf2), buf2);
				if(EvNo != -1) NPC_NowEventSetFlg( talker, EvNo);
			}
#ifdef _NEWEVENT
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, 1);
			work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
			work = work + ( i * 100);
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, work);

			/*--���ڡ�����������ϥܥ��󥦥���ɥ��Υ����פ򤫤���--*/
			if(strstr( buf, "NomalWindowMsg2") != NULL) {
				/*��������������*/
				lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
							WINDOW_BUTTONTYPE_NEXT,
							CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG,
							CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
							token);
				return;
			}
#endif
		  	break;
		  	
#ifdef _NEWEVENT
	  case 99:
			work = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD);
			work = work % 100;

			if(work == 1)
			{
				if( NPC_Util_GetStrFromStrWithDelim( buf, "NomalWindowMsg1",
									 buf2, sizeof( buf2) ) != NULL)
					strcpysafe( token,sizeof( buf2), buf2);

				/*--���ڡ�����������ϥܥ���襦����ɥ��Υ����פ򤫤���--*/
				if(strstr( buf, "NomalWindowMsg2") != NULL) {
					/*��������������*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
			}else{
			
				sprintf( tmp, "NomalWindowMsg%d", work);
				if( NPC_Util_GetStrFromStrWithDelim( buf, tmp,
													 buf2, sizeof( buf2) )
				 != NULL){
					strcpysafe( token,sizeof( buf2), buf2);
				}
				work++;
				sprintf( tmp, "NomalWindowMsg%d", work);
				/*--���ڡ�����������ϥܥ���襦����ɥ��Υ����פ򤫤���--*/
				if(strstr( buf, tmp) != NULL) {
					
					/*��������������*/
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_NEXT,
								CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}else{
					int fd = getfdFromCharaIndex( talker);
					char token2[14*50];
					/*--�����ǡb����  ��ʤɤ��ɲä�Ԥ�--*/
					if(NPC_AcceptDel( meindex, talker,0) == FALSE) return;
	
					/*�ե饰�򥻥åȤ��Ƥ���Сb���åȤ���--*/
					if( EvNo != -1) NPC_NowEventSetFlg( talker, EvNo);
						
					if(NPC_Util_GetStrFromStrWithDelim( buf,"NpcWarp",
										token2,sizeof( token2) )!= NULL){
								NPC_EventWarpNpc(meindex,token2);
					}
					
					//�ե饰��  �Ƥ�
					if(NPC_Util_GetStrFromStrWithDelim( buf, "EndSetFlg", 
										buf2, sizeof( buf2) )!=NULL)
					{
						char buf3[16];
						int k = 1;
				
						if(EvNo != -1) NPC_NowEventSetFlgCls( talker, EvNo);
				
						while(getStringFromIndexWithDelim(buf2 , "," ,k, 
								buf3, sizeof(buf3)) !=FALSE )
						{	
							k++;
							NPC_EventSetFlg( talker, atoi( buf3));
						}
					}
					//���٥�ȥե饰��ä�
					if( NPC_Util_GetStrFromStrWithDelim( buf, "CleanFlg",
							 buf2, sizeof( buf2)) != NULL) {
						 	char buf3[16];
							int k = 1;

						 	k= 1 ;
							while(getStringFromIndexWithDelim(buf2 , "," , k, 
																buf3, sizeof(buf3))
							!= FALSE )
							{
								NPC_NowEndEventSetFlgCls( talker, atoi(buf3));
								k++;
							}
					}
					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
								WINDOW_BUTTONTYPE_YES,
								CHAR_WINDOWTYPE_WINDOWEVENT_STARTMSG,
								CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
								token);
					return;
				}
			}
		  break;
#endif
		  case 10:
			  
			/*--�ڥåȤ����äѤ��λ��Υ�å�����--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "PetFullMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token, sizeof( buf2), buf2);
				print(" PetFullMsg:%s ", token);
			}
			else
			{
				print(" PetFullMsg_Error:%d ", __LINE__);
			}
		break;

	  case 11:
			/*--�ե饰��ä��Ȥ��Υ�å�����--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "CleanMainMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token, sizeof( buf2), buf2);
				buttontype = WINDOW_BUTTONTYPE_YESNO;
				windowtype = CHAR_WINDOWTYPE_WINDOWEVENT_CLEANMSG;
			}
		break;
	  case 12:
			/*--�ե饰��ä���  �Υå�����--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "CleanFlgMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				char buf3[64];
				int loop=1;
				
				strcpysafe( token, sizeof( buf2), buf2);
				NPC_Util_GetStrFromStrWithDelim( buf, "CleanFlg",
												 buf2, sizeof( buf2) );
				while(getStringFromIndexWithDelim(buf2 , "," , loop, buf3, sizeof(buf3))
				!= FALSE )
				{
					NPC_NowEndEventSetFlgCls( talker, atoi(buf3));
					loop++;
				}
			}
		break;

	  case 13:
			/*--���⤬���äѤ��Υ�å�����--*/
			if( NPC_Util_GetStrFromStrWithDelim( buf, "StoneFullMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token, sizeof( buf2), buf2);
			}
		break;
		
		 case 14:
			/*--���⤬����ʤ���å�����--*/
					if( NPC_Util_GetStrFromStrWithDelim( buf, "StoneLessMsg",
												 buf2, sizeof( buf2) )
			 != NULL)
			{
				strcpysafe( token, sizeof( buf2), buf2);
			}
		break;
	}

	/*��������������*/
#ifdef _NEWEVENT
	if(num != 8)
#endif
	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
					buttontype,
					windowtype,
					CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
					token);

}

/*--------------------------------------
 *���٥���ѤΥ���  ����ɲä���
 ---------------------------------------*/
BOOL NPC_EventAdd(int meindex,int talker,int mode)
{

	char buf[64];
	char buff2[128];
	int j = 1,i = 0;
	int kosuucnt = 0;
	int itemindex;
	char buff[1024*2];
	int rand_j = 0;
	int rand_flg = 0;
	int rand_cnt = 0;
	int evcnt;
	int b_flg = 0;

	/*--����ե�����  �˽񤤤Ƥ������  �����Ƥ����Τ�  ���Ф�--*/
	evcnt = NPC_EventFile( meindex, talker, buff);

	if(evcnt == -1) return FALSE;

//	if(NPC_EventFile( meindex, talker, buff) == FALSE) return FALSE;

	/*--����  ��ο�������å�--*/
	if(NPC_ItemFullCheck( meindex, talker, buff, mode, evcnt) == FALSE){
		NPC_MsgDisp( meindex, talker, 4);
		return FALSE;
	}

	/*--��������å���  ��--*/
	if(NPC_Util_GetStrFromStrWithDelim( buff, "DelStone", buff2, sizeof( buff2) )
	!=NULL)
	{
		int stone;
		
		stone = NPC_EventGetCost( meindex, talker, buff2);

		if( (CHAR_getInt( talker, CHAR_GOLD) - stone) < 0 ){
			NPC_MsgDisp( meindex, talker, 14);
			return FALSE;
		}
	}


	/*--�ڥåȤ�  ���ɲä���--*/
	if((NPC_Util_GetStrFromStrWithDelim( buff, "pet_skill", buff2, sizeof( buff2) )
	 !=NULL) && mode == 0 ){
		if(mode == 0) {
			NPC_PetSkillMakeStr( meindex, talker, 1);
			return TRUE;
		}
	}
	
	
	if(mode == 2) mode = 0;
	
	/*--�ڥåȤ��ɲä���---*/
	if(NPC_Util_GetStrFromStrWithDelim( buff, "GetPet", buff2, sizeof( buff2) )
	 !=NULL)
	{	
		if(mode == 0){
			if(NPC_EventAddPet( meindex, talker, buff2,0) == FALSE) {
				NPC_MsgDisp( meindex, talker, 10);
				return FALSE;
			}
		}else if(mode == 1){
		
		}
	}

	// Robin add �qNPC���d���J
	if(NPC_Util_GetStrFromStrWithDelim( buff, "GetEgg", buff2, sizeof( buff2) )
	 !=NULL)
	{	
		if(mode == 0){
			if(NPC_EventAddEgg( meindex, talker, buff2,0) == FALSE) {
				print(" AddEgg_Error:%d ", __LINE__);
				NPC_MsgDisp( meindex, talker, 10);
				return FALSE;
			}
		}else if(mode == 1){
		
		}
	}

	if(NPC_Util_GetStrFromStrWithDelim( buff, "DelItem", buff2, sizeof( buff2) ) !=NULL){
		if(strstr(buff2,"EVDEL") != NULL){
			char work[256];
			char nbuf[256];
			
			NPC_Util_GetStrFromStrWithDelim(buf, "EVENT", buff2, sizeof( buff2));
			getStringFromIndexWithDelim( buff2, ",", evcnt, work, sizeof( work));

			if(strstr(buff,"Break")!=NULL){
				b_flg=1;
			}else{
				b_flg=0;
			}
			
			if(NPC_Util_GetStrFromStrWithDelim( buff, "NotDel", nbuf, sizeof( nbuf)) !=  NULL) {
				NPC_EventDelItemEVDEL( meindex, talker, work, nbuf,b_flg);
			}else{
				NPC_EventDelItemEVDEL( meindex, talker, work, "-1",b_flg);
			}
			
		}else{
			if(strstr(buff,"Break")!=NULL){
				b_flg=1;
			}else{
				b_flg=0;
			}

			NPC_EventDelItem( meindex, talker, buff2, b_flg);
		}
	}

	/*--�������---*/
	if(NPC_Util_GetStrFromStrWithDelim( buff, "DelStone", buff2, sizeof( buff2) )
	!=NULL)
	{
		int stone;
		char token[128];
		
		stone = NPC_EventGetCost( meindex, talker, buff2);

		CHAR_setInt(talker,CHAR_GOLD,CHAR_getInt( talker, CHAR_GOLD) - stone);

		/*--�ѥ�᡼������--*/
		CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);
		sprintf(token, "�I�X%d��stone�C", stone);
		CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
	}


	/*--�����ॢ��  ��---*/
	if(NPC_Util_GetStrFromStrWithDelim( buff, "GetRandItem", buf, sizeof( buf) ) != NULL ){
		j=1;
		while( getStringFromIndexWithDelim( buf , "," , j, buff2, sizeof( buff2)) != FALSE ){
			j++;
			rand_j++;
		}
		rand_cnt = 1;
	}


	/*--  �̤Υ���  �ॲ�å�--*/
	if(NPC_Util_GetStrFromStrWithDelim( buff, "GetItem", buf, sizeof( buf)) != NULL ){
		if(mode == 0) {
			j = 1;
			/*--���ĤΥ���  ���  ������--*/
			while(getStringFromIndexWithDelim( buf, "," , j, buff2, sizeof(buff2)) !=FALSE ){
				j++;
			}
			
			j--;
			for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ){
				itemindex=CHAR_getItemIndex( talker , i );
				if( !ITEM_CHECKINDEX( itemindex) ){
					kosuucnt++;
				 }
			}
			j= j + rand_cnt;
			if( kosuucnt < j ){
				NPC_MsgDisp( meindex, talker, 4);
				return FALSE;

			}else{
				if(NPC_Util_GetStrFromStrWithDelim( buff, "GetRandItem", buff2, sizeof( buff2))	!=NULL)	{
	 				rand_flg = 1;
					NPC_RandItemGet( meindex, talker, rand_j, buff2);
	 			}
				if(NPC_EventAddItem( meindex, talker, buf) == FALSE) return FALSE;
			}
		}else if(mode == 1){
			if(strstr(buff,"Break")!=NULL){
				b_flg=1;
			}else{
				b_flg=0;
			}
			NPC_EventDelItem( meindex, talker, buf,b_flg);
		}
	}

	if( (NPC_Util_GetStrFromStrWithDelim( buff, "GetRandItem", buf, sizeof( buf) )
		!=NULL) && (rand_flg == 0) && mode == 0 )
	{	
		/*--�����å�   ������    �뤫��-*/
		for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ){
			itemindex=CHAR_getItemIndex( talker , i );
			if( !ITEM_CHECKINDEX( itemindex) ){
				kosuucnt++;
			 }
		}
		/*--���������뤫�ɤ���--*/
		if( kosuucnt == 0 ){
			NPC_MsgDisp( meindex, talker, 4);
			return FALSE;
		}

	
		NPC_RandItemGet( meindex, talker, rand_j,buf);
	}
	
#ifdef _EXCHANGEMAN_REQUEST_DELPET
	if( NPC_Util_GetStrFromStrWithDelim( buff, "DelPet", buff2, sizeof( buff2) ) != NULL && mode == 0 ) {
		char buf4[32];
		char buf5[256];
		int i=1;
		char work[256];
		int j=1;
		
		if(strstr(buff2,"EVDEL") != NULL){
			NPC_Util_GetStrFromStrWithDelim( buf, "EVENT", buf5, sizeof( buf5));
			getStringFromIndexWithDelim( buf5, ",",evcnt , buff2, sizeof( buff2));	
			while(getStringFromIndexWithDelim( buff2, "&", j, work, sizeof( work))!=FALSE){
				j++;			
				if(strstr( work, "PET") != NULL) {
					if(strstr( work, "EV") != NULL) {
						if(NPC_PetLvCheck( meindex, talker, work, 1) == FALSE) return FALSE;
					}else{
						if(NPC_PetLvCheck( meindex, talker, work, 0) == FALSE) return FALSE;
					}
				}
			}
			j = 1;		
			while(getStringFromIndexWithDelim( buff2, "&", j, work, sizeof( work)) != FALSE){
				j++;
				if(strstr( work, "PET") == NULL ) continue;
	
				if(strstr(work,"*") != NULL){
					int cnt=0;
					int petcnt=0;
					int petindex;
					
					getStringFromIndexWithDelim( work, "*", 2, buf4, sizeof( buf4));
					cnt = atoi(buf4);
					
					for(i=0 ; i < CHAR_MAXPETHAVE ; i++){
						petindex = CHAR_getCharPet( talker, i);
		
						if( petindex == -1  )  continue;
		
						if(strstr( work, "EV") != NULL) {
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 1)
							== FALSE) {
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 0) == FALSE) {
								continue;
							}
						}

						if(NPC_EventDelPet( meindex, talker,i) == FALSE) return FALSE;
						
						petcnt++;
						/*--����Ŀ�ʬ�ΥڥåȤ�ä������ɤ�����--*/
						if(cnt == petcnt) break;

					}
				}else{
					/*--�ڥåȤ������������ꤷ�Ƥʤ��Τǳ����ڥåȤ����ƾä�--*/

					int petindex;

					for( i = 0; i < CHAR_MAXPETHAVE; i ++ ) {
						petindex = CHAR_getCharPet( talker, i);

					    if( !CHAR_CHECKINDEX( petindex) )  continue;

						if(strstr( buff2, "EV") != NULL){
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 1) == FALSE){
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 0) == FALSE) {
								continue;
							}
						}
					
						if(NPC_EventDelPet( meindex, talker, i) == FALSE) return FALSE;
					}
				}
			}
		
		}else{
			/*--�⤦1�١b�ڥåȤ�  �äƤ��뤫�Υ����å���Ԥ�--*/
			while(getStringFromIndexWithDelim( buff2, ",", j, work, sizeof( work))
			!=FALSE)
			{
				j++;
				/*--�ڥåȤΥ����å����Ϥ��äƤ�����ϰ�öPET��������å����� --*/
				if(strstr( work, "PET") != NULL) {
					if(strstr( work, "EV") != NULL) {
						if(NPC_PetLvCheck( meindex, talker, work, 1) == FALSE) return FALSE;
					}else{
						if(NPC_PetLvCheck( meindex, talker, work, 0) == FALSE) return FALSE;
					}
				}
			}
			j = 1;
			
			while(getStringFromIndexWithDelim( buff2, ",", j, work, sizeof( work)) != FALSE)
			{
				j++;
				if(strstr(work,"*") != NULL)
				{
					int cnt=0;
					int petcnt=0;
					int petindex;
					
					getStringFromIndexWithDelim( work, "*", 2, buf4, sizeof( buf4));
					cnt = atoi(buf4);
					
					for(i=0 ; i < CHAR_MAXPETHAVE ; i++){
						petindex = CHAR_getCharPet( talker, i);
		
						if( petindex == -1  )  continue;
		
						if(strstr( work, "EV") != NULL) {
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 1) == FALSE) {
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 0) == FALSE) {
								continue;
							}
						}

						if(NPC_EventDelPet( meindex, talker,i) == FALSE) return FALSE;
						
						petcnt++;
						/*--����Ŀ�ʬ�ΥڥåȤ�ä������ɤ�����--*/
						if(cnt == petcnt) break;
					}
				}else{
					/*--�ڥåȤ������������ꤷ�Ƥʤ��Τǳ����ڥåȤ����ƾä�--*/

					int petindex;

					for( i = 0; i < CHAR_MAXPETHAVE; i ++ ) {
						petindex = CHAR_getCharPet( talker, i);

					    if( !CHAR_CHECKINDEX( petindex) )  continue;

						if(strstr( buff2, "EV") != NULL){
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 1) == FALSE){
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 0) == FALSE) {
								continue;
							}
						}
						if(NPC_EventDelPet( meindex, talker, i) == FALSE) return FALSE;
					}
				}
			}
		}
	}
#endif
	return TRUE;

}


/*--------------------------------------
 *������ǥ���  ��򥲥åȤ�����
 ------------------------------------ */
BOOL NPC_RandItemGet(int meidex,int talker,int rand_j,char *buf)
{
	char buff2[64];
	int randitem;
	int ret;
	int itemindex;
	char token[128];

	if(rand_j == 0) {
		print("Event:�ѩ󢯪����J�A�X�{���~�C");
 		return FALSE;
 	}
 	
	randitem = rand()%rand_j;
	if(randitem == 0) randitem = rand_j;

	getStringFromIndexWithDelim(buf , "," , randitem, buff2, sizeof(buff2)) ;

	itemindex = ITEM_makeItemAndRegist( atoi( buff2));

	if(itemindex == -1) return FALSE;
	
	/*����  ����ɲ�(  �ܥ���  ��  �ˤ���Ƥ��ޤ�  */
	ret = CHAR_addItemSpecificItemIndex( talker, itemindex);
	if( ret < 0 || ret >= CHAR_MAXITEMHAVE ) {
		print( "npc_exchangeman.c: additem error itemindex[%d]\n", itemindex);
		ITEM_endExistItemsOne( itemindex);
		return FALSE;
	}

	if(itemindex != -1) {
		LogItem(
			CHAR_getChar( talker, CHAR_NAME ), /* �����   */
			CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD �bitem��log���W�[item�W��
			itemindex,
#else
			ITEM_getInt( itemindex, ITEM_ID),  /* ����  ��  �� */
#endif
			"EventAddItem(���ȻݨD�ұo�쪺�D��)",
			CHAR_getInt( talker,CHAR_FLOOR),
			CHAR_getInt( talker,CHAR_X ),
 			CHAR_getInt( talker,CHAR_Y ),
            ITEM_getChar( itemindex, ITEM_UNIQUECODE),
			ITEM_getChar( itemindex, ITEM_NAME),
			ITEM_getInt( itemindex, ITEM_ID)
		);
	}
					
	sprintf(token,"���U�F%s",ITEM_getChar( itemindex, ITEM_NAME));
	CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);

	CHAR_sendItemDataOne( talker, ret);
	return TRUE;

}


/*----------------------------------------------------
 * ���٥��
 ----------------------------------------------------*/
BOOL NPC_AcceptDel(int meindex,int talker,int mode )
{

	char buf[1024*2];
	char buff2[256];
	int rand_j = 0;
	int j = 0;
	int evcnt;
	int b_flg = 0;
	evcnt = NPC_EventFile( meindex, talker, buf);
	if(evcnt == -1) return FALSE;

	/*--����ե�����  �ˤ�����Ƥ������  �����Ƥ����Τ�  ���Ф�--*/
//	if(NPC_EventFile(meindex,talker,buf)==FALSE) return FALSE;

	/*--����  ��ο�������å�--*/
	if(NPC_ItemFullCheck( meindex, talker, buf, mode, evcnt) == FALSE){
		NPC_MsgDisp( meindex, talker, 4);
		return FALSE;
	}
	
	/*--��������å���  ��--*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "DelStone", buff2, sizeof( buff2) )
	!=NULL)
	{
		int stone;
		stone = NPC_EventGetCost( meindex, talker, buff2);

		if( (CHAR_getInt( talker, CHAR_GOLD) - stone) < 0 ) {
			NPC_MsgDisp( meindex, talker, 14);
			return FALSE;
		}
	}


	/*--��������å���  ��--*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetStone", buff2, sizeof( buff2) )!=NULL){
		int stone;
		stone = atoi(buff2);

		if( (CHAR_getInt( talker, CHAR_GOLD) + stone) >= CHAR_getMaxHaveGold( talker) ) {
			NPC_MsgDisp( meindex, talker, 13);
			return FALSE;
		}
	}


	/*--�ڥåȤ�  ���ɲä���--*/
	if((NPC_Util_GetStrFromStrWithDelim( buf, "pet_skill", buff2, sizeof( buff2) )
	 !=NULL) && mode == 0 ){
		if(mode == 0){
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTTRD, 1);
			NPC_PetSkillMakeStr( meindex, talker, 1);
			return TRUE;
		}
	}

	/*--�ڥåȤ�ä�---*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "DelPet", buff2, sizeof( buff2) ) != NULL) {
		char buf4[32];
		char buf5[256];
		int i=1;
		char work[256];
		int j=1;
		
		if(strstr(buff2,"EVDEL") != NULL){
			NPC_Util_GetStrFromStrWithDelim( buf, "EVENT", buf5, sizeof( buf5));
			getStringFromIndexWithDelim( buf5, ",",evcnt , buff2, sizeof( buff2));
		
		
			/*--�⤦1�١b�ڥåȤ�  �äƤ��뤫�Υ����å���Ԥ�--*/
			while(getStringFromIndexWithDelim( buff2, "&", j, work, sizeof( work))
			!=FALSE)
			{
				j++;
				
				/*--�ڥåȤΥ����å����Ϥ��äƤ�����ϰ�öPET��������å����� --*/
				if(strstr( work, "PET") != NULL) {
					if(strstr( work, "EV") != NULL) {
						if(NPC_PetLvCheck( meindex, talker, work, 1) == FALSE) return FALSE;
					}else{
						if(NPC_PetLvCheck( meindex, talker, work, 0) == FALSE) return FALSE;
					}
				}
			}
			j = 1;
			
			
			while(getStringFromIndexWithDelim( buff2, "&", j, work, sizeof( work))
			!= FALSE)
			{
				j++;
				if(strstr( work, "PET") == NULL ) continue;
	
				if(strstr(work,"*") != NULL)
				{
					int cnt=0;
					int petcnt=0;
					int petindex;
					
					getStringFromIndexWithDelim( work, "*", 2, buf4, sizeof( buf4));
					cnt = atoi(buf4);
					
					for(i=0 ; i < CHAR_MAXPETHAVE ; i++){
						petindex = CHAR_getCharPet( talker, i);
		
						if( petindex == -1  )  continue;
		
						if(strstr( work, "EV") != NULL) {
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 1)
							== FALSE) {
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 0) 
							== FALSE) {
								continue;
							}
						}

						if(NPC_EventDelPet( meindex, talker,i) == FALSE) return FALSE;
						
						petcnt++;
						/*--����Ŀ�ʬ�ΥڥåȤ�ä������ɤ�����--*/
						if(cnt == petcnt) break;

					}
				}else{
					/*--�ڥåȤ������������ꤷ�Ƥʤ��Τǳ����ڥåȤ����ƾä�--*/

					int petindex;

					for( i = 0; i < CHAR_MAXPETHAVE; i ++ ) {
						petindex = CHAR_getCharPet( talker, i);

					    if( !CHAR_CHECKINDEX( petindex) )  continue;

						if(strstr( buff2, "EV") != NULL){
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 1)
							== FALSE){
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 0)
							== FALSE) {
								continue;
							}
						}
					
						if(NPC_EventDelPet( meindex, talker, i) == FALSE) return FALSE;
					}
				}
			}
		
		}else{
		
			/*--�⤦1�١b�ڥåȤ�  �äƤ��뤫�Υ����å���Ԥ�--*/
			while(getStringFromIndexWithDelim( buff2, ",", j, work, sizeof( work))
			!=FALSE)
			{
				j++;
				/*--�ڥåȤΥ����å����Ϥ��äƤ�����ϰ�öPET��������å����� --*/
				if(strstr( work, "PET") != NULL) {
					if(strstr( work, "EV") != NULL) {
						if(NPC_PetLvCheck( meindex, talker, work, 1) == FALSE) return FALSE;
					}else{
						if(NPC_PetLvCheck( meindex, talker, work, 0) == FALSE) return FALSE;
					}
				}
			}
			j = 1;
			
			
			while(getStringFromIndexWithDelim( buff2, ",", j, work, sizeof( work))
			!= FALSE)
			{
				j++;
				if(strstr(work,"*") != NULL)
				{
					int cnt=0;
					int petcnt=0;
					int petindex;
					
					getStringFromIndexWithDelim( work, "*", 2, buf4, sizeof( buf4));
					cnt = atoi(buf4);
					
					for(i=0 ; i < CHAR_MAXPETHAVE ; i++){
						petindex = CHAR_getCharPet( talker, i);
		
						if( petindex == -1  )  continue;
		
						if(strstr( work, "EV") != NULL) {
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 1)
							== FALSE) {
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2(petindex, meindex, talker, work, 0) 
							== FALSE) {
								continue;
							}
						}

						if(NPC_EventDelPet( meindex, talker,i) == FALSE) return FALSE;
						
						petcnt++;
						/*--����Ŀ�ʬ�ΥڥåȤ�ä������ɤ�����--*/
						if(cnt == petcnt) break;

					}
				}else{
					/*--�ڥåȤ������������ꤷ�Ƥʤ��Τǳ����ڥåȤ����ƾä�--*/

					int petindex;

					for( i = 0; i < CHAR_MAXPETHAVE; i ++ ) {
						petindex = CHAR_getCharPet( talker, i);

					    if( !CHAR_CHECKINDEX( petindex) )  continue;

						if(strstr( buff2, "EV") != NULL){
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 1)
							== FALSE){
								continue;
							}
						}else{
							if(NPC_PetLvCheckType2( petindex, meindex, talker, work, 0)
							== FALSE) {
								continue;
							}
						}
					
						if(NPC_EventDelPet( meindex, talker, i) == FALSE) return FALSE;
					}
				}
			}
		}
	}
	


	/*--����򤢹���---*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetStone", buff2, sizeof( buff2) )
	!=NULL)
	{
		int stone;
		char token[128];

		stone = atoi(buff2);
		CHAR_setInt(talker,CHAR_GOLD,CHAR_getInt( talker, CHAR_GOLD) + stone);
		/*--�ѥ�᡼������--*/
		CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);
	
		sprintf( token,"�o��%d��stone", stone);
		CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
	}


	/*--�ڥåȤ��ɲä���---*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetPet", buff2, sizeof( buff2) )
	!=NULL)
	{
		if(NPC_EventAddPet( meindex, talker, buff2,1) == FALSE) {
			NPC_MsgDisp( meindex, talker, 10);
			return FALSE;
		}
	}
#ifdef _JZ_TRANSX
	if(NPC_Util_GetStrFromStrWithDelim( buf, "JZ_TRANS", buff2, sizeof( buff2) )
	!=NULL)
	{
		if(NPC_EventTrans( meindex, talker, buff2) == FALSE) {
			NPC_MsgDisp( meindex, talker, 10);
			return FALSE;
		}
	}
#endif
#ifdef _JZ_NEW_ADDMEMBERPOINT
	if(NPC_Util_GetStrFromStrWithDelim( buf, "BMM", buff2, sizeof( buff2) )
	!=NULL)
	{
		int randMM=0;
		char buff3[32];
		while(1){
			if(getStringFromIndexWithDelim( buff2, ",", randMM, buff3,sizeof( buff3))){
				randMM++;
			}else{
				break;
			}
		}
		if(NPC_EventBMM( meindex, talker, buff2,randMM) == FALSE) {
			NPC_MsgDisp( meindex, talker, 10);
			return FALSE;
		}
	}
#endif
	
	// Robin add �qNPC���d���J
	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetEgg", buff2, sizeof( buff2) )
	!=NULL)
	{
		if(NPC_EventAddEgg( meindex, talker, buff2,1) == FALSE) {
			print(" AddEgg_Error:%d ", __LINE__);
			NPC_MsgDisp( meindex, talker, 10);
			return FALSE;
		}
	}

	/*--����  ���ä�--*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "DelItem", buff2, sizeof( buff2) )	!= NULL){
		if(strstr(buff2,"EVDEL") != NULL){
			char work[256];
			char nbuf[256];
		
			NPC_Util_GetStrFromStrWithDelim(buf, "EVENT", buff2, sizeof( buff2));
			getStringFromIndexWithDelim( buff2, ",", evcnt, work, sizeof( work));

			if(strstr(buf,"Break")!=NULL){
				b_flg=1;
			}else{
				b_flg=0;
			}

			if(NPC_Util_GetStrFromStrWithDelim( buf, "NotDel", nbuf, sizeof( nbuf))	!= NULL ){
				NPC_EventDelItemEVDEL( meindex, talker, work, nbuf,b_flg);
			}else{
				NPC_EventDelItemEVDEL( meindex, talker, work, "-1",b_flg);
			}
		}else{
			if(strstr(buf,"Break")!=NULL){
				b_flg=1;
			}else{
				b_flg=0;
			}
			NPC_EventDelItem( meindex, talker, buff2,b_flg);
		}
	}


	/*--�������---*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "DelStone", buff2, sizeof( buff2) )
	!=NULL)
	{
		int stone;
		char token[128];
		
		stone = NPC_EventGetCost( meindex, talker, buff2);

		CHAR_setInt(talker,CHAR_GOLD,CHAR_getInt( talker, CHAR_GOLD) - stone);
		/*--�ѥ�᡼������--*/
		CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);
		sprintf( token, "�I�X%d��stone�C", stone);
		CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
	}


	/*--�����ॢ��  ����ɲä���---*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetRandItem", buff2, sizeof( buff2) )
	!=NULL)
	{
		char buf3[32];
		j = 1;
		while(getStringFromIndexWithDelim(buff2 , "," , j, buf3, sizeof( buf3))
		 != FALSE )
		{
			j++;
			rand_j++;
		}
		NPC_RandItemGet( meindex, talker, rand_j, buff2);
	}


	/*--����  ����ɲä���---*/
	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetItem", buff2, sizeof( buff2) )
	!=NULL)
	{
		 NPC_EventAddItem( meindex, talker, buff2);
	}

	/*--�ѥ�᡼��������--*/
	CHAR_complianceParameter( talker );
	CHAR_send_P_StatusString( talker ,
					CHAR_P_STRING_ATK|CHAR_P_STRING_DEF|
					CHAR_P_STRING_CHARM|CHAR_P_STRING_QUICK|
					CHAR_P_STRING_WATER|CHAR_P_STRING_FIRE|
					CHAR_P_STRING_WIND|CHAR_P_STRING_EARTH
				);
	
	return TRUE;

}


/*--------------------------
 * ���٥�ȥڥåȤ���
 ---------------------------*/
BOOL NPC_EventDelPet(int  meindex,int  talker, int petsel)
{

	int petindex;
	char szPet[128];
	int defpet;
	char msgbuf[64];

	int fd = getfdFromCharaIndex( talker );

	petindex = CHAR_getCharPet( talker, petsel);

    if( !CHAR_CHECKINDEX(petindex) ) return FALSE;

	if( CHAR_getInt( talker, CHAR_RIDEPET) == petsel ) {

		//CHAR_talkToCli( talker, -1, "�M�������d���L�k��X�I", CHAR_COLORYELLOW );
    	//return	FALSE;

		CHAR_setInt( talker, CHAR_RIDEPET, -1);
		CHAR_send_P_StatusString( talker, CHAR_P_STRING_RIDEPET );
		CHAR_complianceParameter( talker );
		CHAR_sendCToArroundCharacter( CHAR_getWorkInt( talker , CHAR_WORKOBJINDEX ));
		print(" DelRidePet ");
	}
	print(" EventDelPet ");

	/*--���Υ���餬��    �ʤ�b�ڥåȤ򤦤餺��  --*/
	if( CHAR_getWorkInt( CONNECT_getCharaindex( fd),
   	                     CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE) return FALSE;
	/*--���ΥڥåȤ��Хȥ뤫�ɤ����Υ����å�--*/
	defpet = CHAR_getInt( talker, CHAR_DEFAULTPET);
	if(defpet == petsel){
		CHAR_setInt( talker, CHAR_DEFAULTPET, -1);
		lssproto_KS_send( fd, -1, TRUE);
	}

	snprintf( msgbuf,sizeof( msgbuf), "��X%s�C",
									CHAR_getChar( petindex, CHAR_NAME));
	CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORWHITE);

	// �ڥåȤ��  �����ҥ�
	LogPet(
		CHAR_getChar( talker, CHAR_NAME ), /* �����   */
		CHAR_getChar( talker, CHAR_CDKEY ),
		CHAR_getChar( petindex, CHAR_NAME),
		CHAR_getInt( petindex, CHAR_LV),
		"EvnetDell(���ȧR��)",
		CHAR_getInt( talker,CHAR_FLOOR),
		CHAR_getInt( talker,CHAR_X ),
		CHAR_getInt( talker,CHAR_Y ),
		CHAR_getChar( petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
	);


	/*   ��  ����ä� */
	CHAR_setCharPet( talker, petsel, -1);

	CHAR_endCharOneArray( petindex );

	snprintf( szPet, sizeof( szPet ), "K%d", petsel);
	// �������ڥåȥ�  ����������Ĥ���
	CHAR_sendStatusString( talker, szPet );

	return TRUE;

}


/*----------------------------
 *  �ڥåȤ��ɲä���
 ------------------------------*/
BOOL NPC_EventAddPet(int meindex, int talker, char *buff2,int mode)
{
	int	ret;
	char msgbuf[64];
	int	enemynum;
	int	enemyid;
	int	i;
	int petindex;

	/*--�ڥå�  �������Ƥ��뤫�Υ����å�--*/
	for( i = 0 ;i < CHAR_MAXPETHAVE ; i++) {
		petindex = CHAR_getCharPet( talker, i);
		if( petindex == -1  )  break;
	}

	if(i == CHAR_MAXPETHAVE) return FALSE;


	/* ������ID����index���   */
	if(strstr(buff2,",") != NULL){
		char buf2[16];
		while(getStringFromIndexWithDelim( buff2, ",", i, buf2, sizeof( buf2))
		!= FALSE) {
			i++;
		}
		i--;
		i = rand()%i + 1;
		getStringFromIndexWithDelim( buff2, ",", i, buf2, sizeof( buf2));
		enemyid = atoi( buf2);
	}else{
		enemyid = atoi( buff2);
	}
	
	enemynum = ENEMY_getEnemyNum();
	for( i = 0; i < enemynum; i ++ ) {
		if( ENEMY_getInt( i, ENEMY_ID) == enemyid) {
			break;
		}
	}

	if( i == enemynum ) return FALSE;

	ret = ENEMY_createPetFromEnemyIndex( talker, i);

	for( i = 0; i < CHAR_MAXPETHAVE; i ++ ){
		if( CHAR_getCharPet( talker, i ) == ret )break;
	}
	if( i == CHAR_MAXPETHAVE ) i = 0;
	if( CHAR_CHECKINDEX( ret ) == TRUE ){
		CHAR_setMaxExpFromLevel( ret, CHAR_getInt( ret, CHAR_LV ));
	}

	petindex = CHAR_getCharPet(talker,i);

    if( !CHAR_CHECKINDEX( petindex) )return FALSE;
	CHAR_complianceParameter( petindex );
	snprintf( msgbuf, sizeof( msgbuf ), "K%d", i );
	CHAR_sendStatusString( talker, msgbuf );

	snprintf( msgbuf, sizeof( msgbuf ), "W%d", i );
	CHAR_sendStatusString( talker, msgbuf );
	snprintf( msgbuf,sizeof( msgbuf), "����%s�C",
								CHAR_getChar(petindex,CHAR_NAME));
	CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORWHITE);

	// �ڥåȤ���  �줿�ҥ�
	LogPet(
		CHAR_getChar( talker, CHAR_NAME ), /* �����   */
		CHAR_getChar( talker, CHAR_CDKEY ),
		CHAR_getChar( petindex, CHAR_NAME),
		CHAR_getInt( petindex, CHAR_LV),
		"EventGet(���ȱo��)",
		CHAR_getInt( talker,CHAR_FLOOR),
		CHAR_getInt( talker,CHAR_X ),
		CHAR_getInt( talker,CHAR_Y ),
		CHAR_getChar( petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
	);



	if(mode == 0) {
		/*--���٥���ѤΥڥå�--*/
		CHAR_setInt( petindex, CHAR_ENDEVENT, 1);
	}

	return TRUE;

}

// Robin add NPC���d���J
BOOL NPC_EventAddEgg(int meindex, int talker, char *buff2,int mode)
{
	int	ret;
	char msgbuf[64];
	int	enemynum;
	int	enemyid;
	int	petid;
	int raise;
	int	i;
	int petindex;
	char buf2[32];
	char buf3[32];

	print("\n ���J��!!:%s ", buff2);

	/*--�ڥå�  �������Ƥ��뤫�Υ����å�--*/
	for( i = 0 ;i < CHAR_MAXPETHAVE ; i++) {
		petindex = CHAR_getCharPet( talker, i);
		if( petindex == -1  )  break;
	}

	if(i == CHAR_MAXPETHAVE) {
		print(" AddEgg_Error:%d ", __LINE__);
		return FALSE;
	}


	/* ������ID����index���   */
	if(strstr(buff2,",") != NULL){ // �O�_���H�����P
		while(getStringFromIndexWithDelim( buff2, ",", i, buf2, sizeof( buf2))
		!= FALSE) {
			i++;
		}
		i--;
		i = rand()%i + 1;
		getStringFromIndexWithDelim( buff2, ",", i, buf2, sizeof( buf2));

	}else{
		strcpy( buf2, buff2);
	}

	getStringFromIndexWithDelim( buf2, ";", 1, buf3, sizeof( buf3));
	enemyid = atoi( buf3); // �d���JID
	getStringFromIndexWithDelim( buf2, ";", 2, buf3, sizeof( buf3));
	petid = atoi( buf3); // ��X���d��ID
	getStringFromIndexWithDelim( buf2, ";", 3, buf3, sizeof( buf3));
	raise = atoi( buf3); //   �i����


	// �Nenemyid�নenemybaseid
	enemynum = ENEMY_getEnemyNum();
	for( i = 0; i < enemynum; i ++ ) {
		if( ENEMY_getInt( i, ENEMY_ID) == petid) {
			break;
		}
	}
	if( i == enemynum ) {
		print(" AddEgg_Error:%d ", __LINE__);
		return FALSE;
	}
	petid = ENEMY_getInt( i, ENEMY_TEMPNO);


	enemynum = ENEMY_getEnemyNum();
	for( i = 0; i < enemynum; i ++ ) {
		if( ENEMY_getInt( i, ENEMY_ID) == enemyid) {
			break;
		}
	}
	if( i == enemynum ) {
		print(" AddEgg_Error:%d ", __LINE__);
		return FALSE;
	}

	ret = ENEMY_createPetFromEnemyIndex( talker, i);

	for( i = 0; i < CHAR_MAXPETHAVE; i ++ ){
		if( CHAR_getCharPet( talker, i ) == ret )break;
	}
	if( i == CHAR_MAXPETHAVE ) i = 0;
	if( CHAR_CHECKINDEX( ret ) == TRUE ){
		CHAR_setMaxExpFromLevel( ret, CHAR_getInt( ret, CHAR_LV ));
	}

	petindex = CHAR_getCharPet(talker,i);

    if( !CHAR_CHECKINDEX( petindex) ) {
		print(" AddEgg_Error:%d ", __LINE__);
		return FALSE;
	}

	// �]���d���J
	CHAR_setInt( petindex, CHAR_FUSIONBEIT, 1);
	CHAR_setInt( petindex, CHAR_FUSIONCODE, -1);
	CHAR_setInt( petindex, CHAR_FUSIONRAISE, raise);
	CHAR_setInt( petindex, CHAR_FUSIONINDEX, petid);
	CHAR_setInt( petindex, CHAR_FUSIONTIMELIMIT, (int)time( NULL));
	
	CHAR_complianceParameter( petindex );
	snprintf( msgbuf, sizeof( msgbuf ), "K%d", i );
	CHAR_sendStatusString( talker, msgbuf );

	snprintf( msgbuf, sizeof( msgbuf ), "W%d", i );
	CHAR_sendStatusString( talker, msgbuf );
	snprintf( msgbuf,sizeof( msgbuf), "����%s�C",
								CHAR_getChar(petindex,CHAR_NAME));
	CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORWHITE);

	// �ڥåȤ���  �줿�ҥ�
	LogPet(
		CHAR_getChar( talker, CHAR_NAME ), /* �����   */
		CHAR_getChar( talker, CHAR_CDKEY ),
		CHAR_getChar( petindex, CHAR_NAME),
		CHAR_getInt( petindex, CHAR_LV),
		"EventGetEgg(���ȱo��)",
		CHAR_getInt( talker,CHAR_FLOOR),
		CHAR_getInt( talker,CHAR_X ),
		CHAR_getInt( talker,CHAR_Y ),
		CHAR_getChar( petindex, CHAR_UNIQUECODE)   // shan 2001/12/14
	);



	if(mode == 0) {
		/*--���٥���ѤΥڥå�--*/
		CHAR_setInt( petindex, CHAR_ENDEVENT, 1);
	}

	return TRUE;

}


/*---------------------------
 *  ���٥��  ��������
 -----------------------------**/
BOOL NPC_EventDelItem(int meindex,int talker,char *buf,int breakflg)
{

	int i = 1, j = 1, k = 1 ;
	char buff3[128];
	char buf2[32];
	int itemindex;
	char token[256];

	while(getStringFromIndexWithDelim(buf , "," , k, buff3, sizeof(buff3))
	 !=FALSE )
	{
		k++;
		if(strstr(buff3,"*") != NULL) {
			int itemno;
			int kosuu;
			int id;
			int cnt=0;
				
			getStringFromIndexWithDelim( buff3, "*", 1, buf2, sizeof( buf2));
			itemno = atoi( buf2); 
			getStringFromIndexWithDelim( buff3, "*", 2, buf2, sizeof( buf2));
			kosuu = atoi( buf2);
	 
			if( breakflg != 1 )
                NPC_ActionDoPileDelItem( talker, itemno, kosuu);//�����|���B�z
            else{
			    for( i =0 ; i < CHAR_MAXITEMHAVE ; i++ ){
				    itemindex = CHAR_getItemIndex( talker , i );
				    if( ITEM_CHECKINDEX( itemindex) ) {
					    id = ITEM_getInt( itemindex , ITEM_ID );
					    if(itemno == id) {
						    cnt++;
						    LogItem(
							CHAR_getChar( talker, CHAR_NAME ), // �����   
							CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD �bitem��log���W�[item�W��
							itemindex,
#else
							ITEM_getInt( itemindex, ITEM_ID),  // ����  ��  �� 
#endif
							"EventDelItem(���ȻݨD�Ҧ��^���D��)",
							CHAR_getInt( talker, CHAR_FLOOR),
							CHAR_getInt( talker, CHAR_X ),
 							CHAR_getInt( talker, CHAR_Y ),
							ITEM_getChar( itemindex, ITEM_UNIQUECODE),
							ITEM_getChar( itemindex, ITEM_NAME),
							ITEM_getInt( itemindex, ITEM_ID)
	     					);
	 
		    				if(breakflg == 1){
			    			    sprintf(token,"%s �a�F",ITEM_getChar( itemindex, ITEM_NAME));
							    CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
							}else{
							    sprintf(token,"��X%s",ITEM_getChar( itemindex, ITEM_NAME));
							    CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
							}
						    //--����  �������--
						    CHAR_setItemIndex( talker, i ,-1);
						    ITEM_endExistItemsOne(itemindex);
						    CHAR_sendItemDataOne( talker, i);

						    if(cnt == kosuu){
							    break;
							}
						}
					}
				}
			}
		}
		else{
			/*--�����  ��Υ���  �������---*/
			for( j = 0 ;  j < CHAR_MAXITEMHAVE ; j++){
				itemindex = CHAR_getItemIndex( talker ,j);

				if( ITEM_CHECKINDEX( itemindex)){
					if( atoi( buff3) == ITEM_getInt( itemindex, ITEM_ID)) {
						LogItem(
							CHAR_getChar( talker, CHAR_NAME ), /* �����   */
							CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD �bitem��log���W�[item�W��
							itemindex,
#else
							ITEM_getInt( itemindex, ITEM_ID),  /* ����  ��  �� */
#endif
							"EventDelItem(���ȻݨD�Ҧ��^���D��)",
							CHAR_getInt( talker,CHAR_FLOOR),
							CHAR_getInt( talker,CHAR_X ),
							CHAR_getInt( talker,CHAR_Y ),
							ITEM_getChar( itemindex, ITEM_UNIQUECODE),
							ITEM_getChar( itemindex, ITEM_NAME),
							ITEM_getInt( itemindex, ITEM_ID)
						);
						if(breakflg == 1){
								sprintf(token,"%s �a�F",ITEM_getChar( itemindex, ITEM_NAME));
							CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
						}else{
							sprintf( token, "��X%s�C", 
											ITEM_getChar( itemindex, ITEM_NAME));
							CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
						}
						/*--����  �������--*/
						CHAR_setItemIndex( talker, j ,-1);
						ITEM_endExistItemsOne( itemindex);
						/*--�ѥ�᡼������--*/
						CHAR_sendItemDataOne( talker, j);
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL NPC_EventDelItemEVDEL(int meindex,int talker,char *buf,char *nbuf,int breakflg)
{

	int k = 1, l = 1;
	char buff3[128];
	char buf2[32];
	char buf4[32];



	while(getStringFromIndexWithDelim(buf , "&" , k, buf4, sizeof(buf4)) !=FALSE ){
		int itemno;
		k++;
		if(strstr(buf4,"ITEM") == NULL) continue;
		getStringFromIndexWithDelim(buf4 , "=" , 2, buff3, sizeof(buff3));

		if(strstr(buff3,"*") != NULL) {
			int kosuu;
#ifndef _ITEM_PILENUMS
			char token[256];
			int i, itemindex, id, cnt;
#endif
			getStringFromIndexWithDelim( buff3, "*", 1, buf2, sizeof( buf2));
			itemno = atoi( buf2); 
			getStringFromIndexWithDelim( buff3, "*", 2, buf2, sizeof( buf2));
			kosuu = atoi( buf2);
			if(strstr(nbuf,"-1") == NULL){
				l = 1;
				while(getStringFromIndexWithDelim(nbuf , "," , l, buf2, sizeof(buf2))){
					l++;
					if(itemno == atoi( buf2)){
						l = -1;
						break;
					}
				}
				if(l == -1) continue;
			}
#ifdef _ITEM_PILENUMS
			NPC_ActionDoPileDelItem( talker, itemno, kosuu);
#else
			cnt = 0;
			for( i =0 ; i < CHAR_MAXITEMHAVE ; i++ ){
				itemindex = CHAR_getItemIndex( talker , i );
				if( ITEM_CHECKINDEX( itemindex) ) {
					id = ITEM_getInt( itemindex , ITEM_ID );
					if(itemno == id) {
						cnt++;
						LogItem(
							CHAR_getChar( talker, CHAR_NAME ), /* �����   */
							CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD �bitem��log���W�[item�W��
							itemindex,
#else
							ITEM_getInt( itemindex, ITEM_ID),  /* ����  ��  �� */
#endif
							"EventDelItem(���ȻݨD�Ҧ��^���D��)",
							CHAR_getInt( talker, CHAR_FLOOR),
							CHAR_getInt( talker, CHAR_X ),
 							CHAR_getInt( talker, CHAR_Y ),
							ITEM_getChar( itemindex, ITEM_UNIQUECODE),
							ITEM_getChar( itemindex, ITEM_NAME),
							ITEM_getInt( itemindex, ITEM_ID)
						);
						if(breakflg == 1){
							sprintf(token,"%s �a�F",ITEM_getChar( itemindex, ITEM_NAME));
							CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
						}else{
							sprintf(token,"��X%s",ITEM_getChar( itemindex, ITEM_NAME));
							CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
						}
						CHAR_setItemIndex( talker, i ,-1);
						ITEM_endExistItemsOne(itemindex);
						CHAR_sendItemDataOne( talker, i);
						if(cnt == kosuu){
							break;
						}
					}
				}
			}		
#endif
		}else{
#ifndef _ITEM_PILENUMS
			int j, itemindex;
			char token[256];
#endif
			if(strstr(nbuf,"-1") == NULL){
				l = 1;
				while(getStringFromIndexWithDelim(nbuf , "," , l, buf2, sizeof(buf2))){
					l++;
					if(atoi(buff3) == atoi( buf2)){
						l = -1;
						break;
					}
				}
				if(l == -1) continue;
			}
			itemno = -1;
#ifdef _ITEM_PILENUMS
			NPC_ActionDoPileDelItem( talker, itemno, itemno);
#else
			for( j = 0 ;  j < CHAR_MAXITEMHAVE ; j++){
				itemindex = CHAR_getItemIndex( talker ,j);
				if( ITEM_CHECKINDEX( itemindex)){
					if( atoi( buff3) == ITEM_getInt( itemindex, ITEM_ID)) {
						LogItem(
							CHAR_getChar( talker, CHAR_NAME ), /* �����   */
							CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD �bitem��log���W�[item�W��
							itemindex,
#else
							ITEM_getInt( itemindex, ITEM_ID),  /* ����  ��  �� */
#endif
							"EventDelItem(���ȻݨD�Ҧ��^���D��)",
							CHAR_getInt( talker,CHAR_FLOOR),
							CHAR_getInt( talker,CHAR_X ),
							CHAR_getInt( talker,CHAR_Y ),
							ITEM_getChar( itemindex, ITEM_UNIQUECODE),
							ITEM_getChar( itemindex, ITEM_NAME),
							ITEM_getInt( itemindex, ITEM_ID)
						);
						if(breakflg == 1){
							sprintf(token,"%s �a�F",ITEM_getChar( itemindex, ITEM_NAME));
							CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
						}else{
							sprintf( token, "��X%s�C", 
										ITEM_getChar( itemindex, ITEM_NAME));
							CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);
						}
						CHAR_setItemIndex( talker, j ,-1);
						ITEM_endExistItemsOne( itemindex);
						CHAR_sendItemDataOne( talker, j);
					}
				}
			}
#endif
		}
	}

	return TRUE;
}


/*----------------------
 * ���٥��  ���ɲ�]
 -----------------------*/
BOOL NPC_EventAddItem(int meindex,int talker,char *buf)
{

	char buff3[128];
	int i = 1;
	int itemindex;
	char buf3[32];
	int ret;
	char token[256];

	while(getStringFromIndexWithDelim(buf , "," , i, buff3, sizeof( buff3)) 
	!= FALSE )
	{
		i++;
		if(strstr( buff3, "*") != NULL) {
			
			int itemno;
			int kosuu;
			int loop = 0;

			getStringFromIndexWithDelim( buff3, "*", 1, buf3,sizeof( buf3));
			itemno = atoi( buf3);
			getStringFromIndexWithDelim( buff3, "*", 2, buf3,sizeof( buf3));
			kosuu = atoi( buf3);
	
			for(loop = 0 ; loop < kosuu ; loop++) {
				itemindex = ITEM_makeItemAndRegist( itemno);
	
				if(itemindex == -1) return FALSE;
	
				/*����  ����ɲ�(  �ܥ���  ��  �ˤ���Ƥ��ޤ�  */
				ret = CHAR_addItemSpecificItemIndex( talker, itemindex);
				if( ret < 0 || ret >= CHAR_MAXITEMHAVE ) {
				    print("npc_exchange:ACCEPTadditem error itemindex[%d]\n",itemindex);
					ITEM_endExistItemsOne( itemindex);
					return FALSE;
				}

				if(itemindex != -1) {
					LogItem(
						CHAR_getChar( talker, CHAR_NAME ), /* �����   */
						CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD �bitem��log���W�[item�W��
						itemindex,
#else
	    				ITEM_getInt( itemindex, ITEM_ID),  /* ����  ��  �� */
#endif
						"EventAddItem(���ȻݨD�ұo�쪺�D��)",
						CHAR_getInt( talker, CHAR_FLOOR),
						CHAR_getInt( talker, CHAR_X ),
						CHAR_getInt( talker, CHAR_Y ),
						ITEM_getChar( itemindex, ITEM_UNIQUECODE),
						ITEM_getChar( itemindex, ITEM_NAME),
						ITEM_getInt( itemindex, ITEM_ID)
					);
				}
				sprintf( token, "����%s�C", ITEM_getChar( itemindex, ITEM_NAME));
				CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);

				CHAR_sendItemDataOne( talker, ret);
			}
		}else{
			itemindex = ITEM_makeItemAndRegist( atoi( buff3));
	
			if(itemindex == -1) return FALSE;

			/*����  ����ɲ�(  �ܥ���  ��  �ˤ���Ƥ��ޤ�  */
			ret = CHAR_addItemSpecificItemIndex( talker, itemindex);
			if( ret < 0 || ret >= CHAR_MAXITEMHAVE ) {
				print( "npc_exchange.c: ACCEPTadditem error itemindex[%d]\n", itemindex);
				ITEM_endExistItemsOne( itemindex);
				return FALSE;
			}

			if(itemindex != -1) {
				LogItem(
				CHAR_getChar( talker, CHAR_NAME ), /* �����   */
				CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD �bitem��log���W�[item�W��
				itemindex,
#else
	       		ITEM_getInt( itemindex, ITEM_ID),  /* ����  ��  �� */
#endif
				"EventAddItem(���ȻݨD�ұo�쪺�D��)",
				CHAR_getInt( talker, CHAR_FLOOR),
				CHAR_getInt( talker, CHAR_X ),
 				CHAR_getInt( talker, CHAR_Y ),
				ITEM_getChar( itemindex, ITEM_UNIQUECODE),
				ITEM_getChar( itemindex, ITEM_NAME),
				ITEM_getInt( itemindex, ITEM_ID)
			);
			}
			sprintf(token,"����%s�C", ITEM_getChar( itemindex, ITEM_NAME));
			CHAR_talkToCli( talker, -1, token, CHAR_COLORWHITE);

			CHAR_sendItemDataOne( talker, ret);
		}
	}

	return TRUE;
}


/*---------------------------------
 *����  �����Ƥ���Τ�  ���Ф�
 ------------- ---------------------*/
int NPC_EventFile(int meindex,int talker,char *arg)
{

	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buff2[256];
	char buf[1024*2];
	int i=1;
	int EvNo=0;
	int evch;
	
	/*--����ե������  ��  ��--*/
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {

		print("GetArgStrErr");
		return -1;
	}

	/*����ե������ʬ�䤹��*/
	while(getStringFromIndexWithDelim( argstr, "EventEnd", i, buf,sizeof( buf))
	!= FALSE)
	{
		CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 0);
		i++;
		/*--���٥�ȥʥ�С��򥲥å�--*/
		NPC_Util_GetStrFromStrWithDelim( buf, "EventNo", buff2,sizeof( buff2) );
		EvNo = atoi( buff2);

		/*--�ե饰�����å���Ԥ��e���Ǥˡb���Υ��٥�Ȥ���  ���Ƥ�����ϱ���--*/
		if(NPC_EventCheckFlg( talker, EvNo) == TRUE) continue;

		/*--������ɤΥ����å���Ԥ�--*/
		if(NPC_Util_GetStrFromStrWithDelim( buf,"Pet_Name", buff2,sizeof( buff2) ) 
		 != NULL)
		{
			CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, 1);
		}
		/*--������ɤΥ����å���Ԥ�--*/
		if(NPC_Util_GetStrFromStrWithDelim( buf,"KeyWord", buff2,sizeof( buff2) ) 
		 != NULL)
		{
			if(CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANT) != i ) continue;
		}

		evch = NPC_ExChangeManEventCheck( meindex, talker, buf);

		if(evch != -1) {
		/*--���٥�Ⱦ��Υ����å�--*/
//		if(NPC_ExChangeManEventCheck( meindex, talker, buf) == TRUE) {
			strcpysafe(arg, sizeof( buf) ,buf);
			return evch;
		}
	}

	return -1;

}


/*---------------------------------
 * ����  ��Υ����å�������Ԥ�
 -----------------------------------*/
BOOL NPC_EventReduce(int meindex,int talker,char *buf)
{

	char buf2[512];
	char buf3[256];
	int id = 0;
	int i;
	int itemindex;
	int itemno;
	int kosuu;
	int cnt = 0;
	
	getStringFromIndexWithDelim( buf, "=", 2, buf2, sizeof( buf2));
	getStringFromIndexWithDelim( buf2, "*", 1, buf3, sizeof( buf3));
	itemno = atoi( buf3);
	getStringFromIndexWithDelim( buf2, "*", 2, buf3, sizeof( buf3));
	kosuu = atoi( buf3);
	
	for( i = 0 ; i < CHAR_MAXITEMHAVE ; i++ ){
		itemindex = CHAR_getItemIndex( talker , i );
		if( ITEM_CHECKINDEX( itemindex) ){
			id = ITEM_getInt( itemindex ,ITEM_ID);
			if(itemno == id){
				//change add �W�[����|���P�_
				int pilenum = ITEM_getInt( itemindex, ITEM_USEPILENUMS);
				if( pilenum )
					cnt+=pilenum;
				else
					cnt++;

				if(cnt >= kosuu){
					return TRUE;
				}
			}
		}
	}

	return FALSE;

}

/*----------------------------------
 *�ڥåȤ˥������Ф�������
 ----------------------------------*/
void NPC_EventPetSkill( int meindex, int talker, char *data)
{

	int skill;
	int pet;
	int slot;
	int cost;
	int skillID = 0;
	char buf[64];
	char argstr[1024];
	char msg[512];
	int petindex;
	char msgbuf[128];
	int fd = getfdFromCharaIndex( talker );
	int evcnt;
	
	if( NPC_Util_CharDistance( talker, meindex ) > 2) {
		return;
	}

	evcnt = NPC_EventFile( meindex, talker, argstr);

	if(evcnt == -1) return;

	/*--����ե�����  �˹ᤫk��  �������  �����Ƥ����Τ�  ���Ф�--*/
//	if(NPC_EventFile( meindex, talker, argstr) == FALSE) return ;

	/*���饤����Ȥ����֤äƤ���ǡ���--*/
	/*�ɤ�  �򤨤�������(��    ��)  �ɤΥڥåȤ���(��    ��)  
	�ɤΥ��ҥåȤ���(��    ��)  ����*/
	makeStringFromEscaped( data);

	/*--�ǡ����ο���ʬ��--*/
	getStringFromIndexWithDelim( data, "|", 1, buf, sizeof( buf));
	skill = atoi( buf);
	getStringFromIndexWithDelim( data, "|", 2, buf, sizeof( buf));
	pet = atoi( buf);
	getStringFromIndexWithDelim( data, "|", 3, buf ,sizeof( buf));
	slot = atoi(buf);
	getStringFromIndexWithDelim( data, "|", 4, buf, sizeof( buf));
	cost = atoi( buf);

	/*--������    �򥲥åȤ���--*/
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "pet_skill", 
									msg, sizeof( msg)) != NULL) 
	{
		getStringFromIndexWithDelim( msg, ",", skill, buf ,sizeof( buf));
		skillID = atoi( buf);
	}
	slot--;


	petindex = CHAR_getCharPet( talker, pet-1);

	if(petindex == -1){
		print("PetindexErr");
		return;
	}

	/*--���Υ���餬��    �ʤ�b�ڥåȤ򤦤餺��  --*/
	if( CHAR_getWorkInt( CONNECT_getCharaindex( fd),
                         CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE) return ;

	/*--�ڥåȤ�  ��Ф�������--*/
	CHAR_setPetSkill( petindex, slot, skillID);
			
	/*--����򸺤餹--*/
	CHAR_setInt( talker, CHAR_GOLD, (CHAR_getInt( talker, CHAR_GOLD) - cost));

	snprintf( msgbuf, sizeof( msgbuf ), "W%d",pet-1);

	// �������ڥåȤ�  �ǡ���������Ĥ���
	CHAR_sendStatusString( talker, msgbuf );
			
	CHAR_sendStatusString( talker, "P");

}


/*----------------------------------------
 *  ����  �ब���äѤ����ɤ����Υ����å�
----------- ------------------------------*/
BOOL NPC_ItemFullCheck(int meindex,int talker,char *buf,int mode,int evcnt)
{

	char buff2[256];
	int i = 1,j = 1;
	int maxitem = 0;
	int kosuucnt = 0;
	int itemindex;
	char buf3[256];
	int rand_j = 0;
	int rand_cnt = 0;
	char nbuf[256];
	char buff6[16];
	int l = 1;
	
	if(NPC_Util_GetStrFromStrWithDelim( buf, "DelItem", buff2, sizeof( buff2) )	!= NULL ){
		if(strstr(buff2,"EVDEL") != NULL){
			char buff4[64];
			char buff5[32];
			
			i = 1;
			NPC_Util_GetStrFromStrWithDelim( buf, "EVENT", buff2, sizeof( buff2));
			getStringFromIndexWithDelim(buff2, "," , evcnt, buf3, sizeof(buf3));
			
			while(getStringFromIndexWithDelim(buf3, "&" , j, buff4, sizeof(buff4))){
				j++;
				if(strstr(buff4,"ITEM") != NULL) {
					if(strstr(buff4,"*") != NULL) {
						int itemno;

						l = 1;
						getStringFromIndexWithDelim(buff4, "=" , 2, buff5, sizeof(buff5));
						getStringFromIndexWithDelim(buff5, "*" , 1, buff6, sizeof(buff6));
						itemno = atoi(buff6);
						
						if(NPC_Util_GetStrFromStrWithDelim( buf, "NotDel", nbuf, sizeof( nbuf))	!=NULL){
							while(getStringFromIndexWithDelim(nbuf , "," , l,buff6, sizeof(buff6)))
							{
								l++;
								if(itemno == atoi( buff6))
								{
									l = -1;
									break;
								}
							}
						}
						if(l == -1) continue;
						getStringFromIndexWithDelim(buff5, "*" , 2, buff6, sizeof(buff6));
						maxitem -= atoi( buff6);
					}else{
						getStringFromIndexWithDelim(buff4, "=" , 2, buff5, sizeof(buff5));
						l = 1; 

						if(NPC_Util_GetStrFromStrWithDelim( buf, "NotDel", nbuf, sizeof( nbuf))
						!=NULL){
							while(getStringFromIndexWithDelim(nbuf , "," , l,buff6, sizeof(buff6)))	{
								l++;
								if(atoi(buff5) == atoi( buff6)){
									l = -1;
									break;
								}
							}
						}
						if(l == -1) continue;
						for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ) {
							itemindex = CHAR_getItemIndex( talker , i );
							if( ITEM_CHECKINDEX( itemindex)) {
								if(atoi( buff5) == ITEM_getInt( itemindex, ITEM_ID)) {
									maxitem--;
								}
							}
						}
					}
				}
			}
		}else{
			char buff3[128];
			i = 1;
			while(getStringFromIndexWithDelim(buff2, "," , i, buff3, sizeof(buff3)) !=FALSE ){
				i++;
				if(strstr( buff3, "*") != NULL){
					getStringFromIndexWithDelim( buff3, "*", 2, buf3, sizeof( buf3));
					maxitem -= atoi( buf3);
				}else{
					for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ) {
						itemindex=CHAR_getItemIndex( talker , i );
						if( ITEM_CHECKINDEX( itemindex)) {
							if(atoi( buff3) == ITEM_getInt( itemindex, ITEM_ID)) {
							maxitem--;
							}
						}
					}
				}
			}

		}
	}

	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetRandItem", buff2, sizeof( buff2) ) !=NULL && mode == 0){
		j = 1;
		while(getStringFromIndexWithDelim(buff2 , "," , j, buf3, sizeof( buf3)) !=FALSE ){
			j++;
			rand_j++;
		}
		rand_cnt = 1;
		for( i = CHAR_STARTITEMARRAY ; i <CHAR_MAXITEMHAVE ; i++ ) {
			itemindex = CHAR_getItemIndex( talker , i );
			if( !ITEM_CHECKINDEX(itemindex) ){
				kosuucnt++;
			}
		}
		if( maxitem == 0 && kosuucnt== 0){
			return FALSE;
		}
	}


	if(NPC_Util_GetStrFromStrWithDelim( buf, "GetItem", buff2, sizeof( buff2) )
	!= NULL && mode == 0){
		char buff3[256];
		j = 1;
		while(getStringFromIndexWithDelim(buff2 , "," , j, buff3, sizeof( buff3)) != FALSE ){
			j++;
			if(strstr(buff3,"*") !=NULL ) {
				getStringFromIndexWithDelim( buff3, "*", 2, buf3,sizeof( buf3));
				maxitem += atoi( buf3);
			}else{
				maxitem++;
			}
		}
		kosuucnt=0;
		for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ){
			itemindex = CHAR_getItemIndex( talker , i );
			if( !ITEM_CHECKINDEX( itemindex)){
				kosuucnt++;
			 }
		}
		maxitem = maxitem + rand_cnt;
		if( kosuucnt < maxitem){
			return FALSE;
		}
	}


	return TRUE;
}

void NPC_CharmStatus(int meindex,int talker)
{
	int i = 0;
	int petindex;
	char petsend[64];	
	for(i = 0 ; i < CHAR_MAXPETHAVE ; i++) {
    	petindex = CHAR_getCharPet( talker, i);

		if( petindex == -1  )  continue;
		if( !CHAR_CHECKINDEX( talker ) )  continue;
		CHAR_complianceParameter( petindex );
		sprintf( petsend, "K%d", i );
		CHAR_sendStatusString( talker , petsend );
	}
}

int NPC_EventGetCost(int meindex,int talker,char *arg)
{

	int cost;
	int level;
	char buf[32];
	if(strstr( arg, "LV") != NULL) {
		level = CHAR_getInt( talker, CHAR_LV);
		getStringFromIndexWithDelim( arg, "*", 2, buf,sizeof( buf));
		cost = level * atoi( buf);
	}else{
		cost = atoi( arg);
	}
	return cost;

}
#ifdef _JZ_NEW_ADDMEMBERPOINT
BOOL NPC_EventBMM(int meindex, int talker, char *buff2,int mode)
{
	int	ret;
	char msgbuf[64], buf1[256];
	int	enemynum;
	int	enemyid;
	int		i,j;
	int level=79;
	int LevelUpPoint;
	int vital, str, tgh, dex;
	float modai;
	
	int randmm;
	int mmnum,petindex;
	for( i = 0 ;i < CHAR_MAXPETHAVE ; i++) {
		petindex = CHAR_getCharPet( talker, i);
		if( petindex == -1  )
			break;
	}

  if( i == CHAR_MAXPETHAVE )      {
		snprintf( msgbuf,sizeof( msgbuf), "�d���w���I�I");
		CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORYELLOW);
		return FALSE;
	}
	
	if (mode>0){
		randmm = rand()%mode;
		if(randmm == 0) randmm = mode;
		getStringFromIndexWithDelim( buff2, ",", randmm, buf1,sizeof( buf1));
		if (strstr(buf1,"*")!=NULL){
			getStringFromIndexWithDelim( buf1, "*", 1, msgbuf,sizeof( msgbuf));
			randmm = atoi(msgbuf);
			getStringFromIndexWithDelim( buf1, "*", 2, msgbuf,sizeof( msgbuf));
			mmnum = atoi(msgbuf);
		}else{
			randmm = atoi(buf1);
			mmnum = 1;
		}
		if (mmnum<1)mmnum = 1;
		if (mmnum>5)mmnum = 5;
		if(randmm== 1 ){
			enemyid = 1479;
		}else if( randmm== 2){
			enemyid = 2547;
		}else{
			print("ExChangeMan err:BMM-%s\n",buf1);
		}
		enemynum = ENEMY_getEnemyNum();
		for( i = 0; i < enemynum; i ++ ) {
			if( ENEMY_getInt( i, ENEMY_ID) == enemyid) {
				break;
			}
		}
		if( i == enemynum ) return FALSE;
		for(j = 0; j < mmnum; j ++ ) {
			ret = ENEMY_createPetFromEnemyIndex( talker, i);
			if( !CHAR_CHECKINDEX( ret))return;
			vital = 50;
			str = 50;
			tgh = 50;
			dex = 50;
			LevelUpPoint = ( vital << 24 ) + ( str << 16 ) + ( tgh << 8 ) + ( dex << 0 );
			CHAR_setInt( ret, CHAR_ALLOCPOINT , LevelUpPoint );
			print("\n lvup <<%d %d %d %d>>", vital, str, tgh, dex );

			for( i = 0; i < CHAR_MAXPETHAVE; i ++ ){
				if( CHAR_getCharPet( talker, i ) == ret )break;
			}
			if( i == CHAR_MAXPETHAVE )i = 0;

			if( CHAR_CHECKINDEX( ret ) == TRUE ){
				CHAR_setMaxExpFromLevel( ret, CHAR_getInt( ret, CHAR_LV ));
			}
				if( level > 79 ) level = 79;
				if( level < 0 ) level = 0;
				if( level > 0 ){
					int k;
					for( k = 1; k < level; k ++ ){	//�ɯ�
						CHAR_PetLevelUp( ret );
						CHAR_PetAddVariableAi( ret, AI_FIX_PETLEVELUP );
						CHAR_setInt( ret, CHAR_LV, CHAR_getInt( ret, CHAR_LV) +1 );
					}
				}
			snprintf( msgbuf, sizeof( msgbuf ), "K%d", i );
			CHAR_sendStatusString( talker, msgbuf );
			snprintf( msgbuf, sizeof( msgbuf ), "W%d", i );
			CHAR_sendStatusString( talker, msgbuf );
		}
		snprintf( msgbuf,sizeof( msgbuf), "��o79��MM %d �u\n",mmnum,randmm);
		CHAR_talkToCli( talker, -1, msgbuf,  CHAR_COLORYELLOW);
	}
}
#endif
#ifdef _JZ_TRANSX
BOOL NPC_EventTrans(int meindex, int talker, char *buff2)
{
	int transtemp;
	int work[10];
	if(strlen(buff2))CHAR_talkToCli( talker, -1,buff2, CHAR_COLORYELLOW);
	transtemp = CHAR_getInt(	talker, CHAR_TRANSMIGRATION)+1;
	
	if( transtemp > _JZ_TRANSX){
		CHAR_talkToCli( talker, -1,"�L�k�A��ͤF", CHAR_COLORWHITE);
		return FALSE;
	}

	CHAR_setInt(talker ,CHAR_TRANSMIGRATION,transtemp);

	NPC_TransmigrationStatus(0, talker, work);
	NPC_TransmigrationFlg_CLS(0, talker);
	CHAR_setInt(talker, CHAR_LV ,1);
	CHAR_setMaxExp( talker, 0);
   
  CHAR_setInt( talker,CHAR_SKILLUPPOINT,transtemp*10);
  CHAR_Skillupsend( talker );
  CHAR_setInt( talker , CHAR_BASEIMAGENUMBER , CHAR_getInt( talker , CHAR_BASEBASEIMAGENUMBER) );
	LogTensei(
		CHAR_getChar( talker, CHAR_NAME ), /* �����   */
		CHAR_getChar( talker, CHAR_CDKEY),
		"Tensei(�D���F��NPC���)",
		work[0],
		CHAR_getInt(talker,CHAR_TRANSMIGRATION),
		work[1],
		NPC_StartpointCheck( meindex, talker),
		0, 0,
		work[2],work[3],work[4],work[5],
		work[6],work[7],work[8],work[9]
	);
	//�p��H����O
	CHAR_complianceParameter( talker );
	CHAR_setInt(talker, CHAR_HP, CHAR_getWorkInt(talker, CHAR_WORKMAXHP) );
	CHAR_setInt(talker,CHAR_LASTTALKELDER, NPC_StartpointCheck(0,talker));
	//��s�Ȥ��
	CHAR_sendStatusString( talker, "P" );
	CHAR_sendCToArroundCharacter( CHAR_getWorkInt( talker, CHAR_WORKOBJINDEX ));
}
#endif