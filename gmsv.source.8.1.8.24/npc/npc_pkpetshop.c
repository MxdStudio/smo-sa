//krynn 2001/12/6
//PKPetShop

#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "pet_skill.h"
#include "readmap.h"
#include "log.h"
#include "enemy.h"
#include "npc_pkpetshop.h"
#include "battle.h"

#ifdef _PKPETSHOP
#define MAXSHOPPET 33

static void NPC_PKPetShop_selectWindow( int meindex, int talker, int num,int select);
void NPC_PKPetShop_BuyMain(int meindex,int talker,int before );
void NPC_PKPetShop_GetPetList(char *argstr,char * argtoken2);
BOOL NPC_PKPetShop_SetNewPet(int meindex,int talker,char *data);
BOOL NPC_PKPetShop_SellNewPet(int meindex,int talker,char *data);

void NPC_PKPetShop_Menu(int meindex,int talker);
int NPC_PKPetShop_GetLimtPetList(int talker,char *argstr,char *token2,int sell);
void NPC_PKPetShop_SellMain(int meindex,int talker,int select);
int NPC_GetSellPetList(int itemindex,int flg,char *argstr,char *argtoken,int select,int sell);
BOOL NPC_AddPetBuy(int meindex, int talker,int petID,int kosuu,double rate);
void NPC_PetStrStr(int petID,double rate,char *name,char *token2, int index);
int NPC_SellPetstrsStr(int itemindex,int flg,double rate,char *argtoken,int select,int sell);
void NPC_LimitPetShop(int meindex,int talker,int select);
void NPC_PKPetShop_ExpressmanCheck(int meindex,int talker);
 

/*--���    ���--*/
enum{
	NPC_PKPETSHOP_WORK_NO 		= CHAR_NPCWORKINT1,
	NPC_PKPETSHOP_WORK_EV 		= CHAR_NPCWORKINT2,
	NPC_PKPETSHOP_WORK_EXPRESS	= CHAR_NPCWORKINT3,
};

		  
typedef struct {
	char	arg[32];
	int		type;
}PKPETSHOP_NPC_Shop;


static PKPETSHOP_NPC_Shop		TypeTable[] = {
	{ "FIST",		0 },
	{ "AXE",		1 },
	{ "CLUB",		2 },
	{ "SPEAR",		3},
	{ "BOW",		4},
	{ "SHIELD",		5},
	{ "HELM",		6 },
	{ "ARMOUR",		7 },
	{ "BRACELET",	8},
	{ "ANCLET",		9 },
	{ "NECKLACE",	10},
	{ "RING",		11},
	{ "BELT",		12},
	{ "EARRING",	13},
	{ "NOSERING",	14},
	{ "AMULET",		15},
	{ "OTHER",		16},
	{ "BOOMERANG",	17},
	{ "BOUNDTHROW",	18},
	{ "BREAKTHROW",	19},
#ifdef _ITEM_TYPETABLE
	{ "DISH",	20},
	{ "METAL",	21},
	{ "JEWEL",	22},
	{ "WARES",	23},
	{ "WBELT",	24},
	{ "WSHIELD", 25},
	{ "WSHOES",	26},
	{ "WGLOVE",	27},
	{ "ANGELTOKEN",	28},
	{ "HEROTOKEN",	29},
#endif	
	{ "ACCESSORY",	30},
	{ "OFFENCE",	40},
	{ "DEFENCE",	50},

};


/*********************************
* ��l
*********************************/
BOOL NPC_PKPetShopInit( int meindex )
{

	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;

	/*--����������--*/
	CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPEPKPetShop );
	
	if((argstr = NPC_Util_GetArgStr(meindex, argstr1, sizeof(argstr1))) == NULL ) 
	{
		print("NPC_PKPetShopInit_GetArgStrErr");
		return FALSE;
	}
	if(strstr(argstr,"LIMITSHOP") != NULL) 
	{		/*-  ����귴�ѥե饰--*/
		CHAR_setWorkInt( meindex, NPC_PKPETSHOP_WORK_NO, 1);
	}
	else
	{
		CHAR_setWorkInt( meindex, NPC_PKPETSHOP_WORK_NO, 0);
	}
	
	if(strstr( argstr, "EVENT") != NULL) 
	{		/*-  ����귴�ѥե饰--*/
		CHAR_setWorkInt( meindex, NPC_PKPETSHOP_WORK_EV, 1);
	}
	else
	{
		CHAR_setWorkInt( meindex, NPC_PKPETSHOP_WORK_EV, 0);
	}

	if(strstr( argstr, "EXPRESS") != NULL) 
	{		/*-�������ե饰--*/
		CHAR_setWorkInt( meindex, NPC_PKPETSHOP_WORK_EXPRESS, 1);
	}
	else
	{
		CHAR_setWorkInt( meindex, NPC_PKPETSHOP_WORK_EXPRESS, 0);
	}
	return TRUE;
}


/*********************************
*   ��������줿���ν�  
*********************************/
void NPC_PKPetShopTalked( int meindex , int talker , char *szMes ,int color )
{
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char	buff[1024];
	char	buf2[256];
	char 	token[1024];
	int 	i = 1;
	BOOL	sellonlyflg = FALSE;
	char	sellmsg[1024];

    /* �ץ쥤�䡼���Ф��Ƥ���  ������ */
    if( CHAR_getInt( talker , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) 
	{
    	return;
    }

	/*--  �����ˤ��뤫�ɤ�����--*/
	if(NPC_Util_isFaceToFace( meindex, talker, 2) == FALSE) 
	{		/* ������åɰ�  �Τ� */
		if( NPC_Util_CharDistance( talker, meindex ) > 1) return;
	}

    if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1))) == NULL)
	{
    	print("NPC_PKPetShopInit_GetArgStrErr");
    	return;
    }

	if( NPC_Util_GetStrFromStrWithDelim( argstr, "sellonly_msg", 
										 token, sizeof( token))
		!= NULL)
	{
		sellonlyflg = TRUE;
		strcpysafe(sellmsg, sizeof( sellmsg), token);
	}

	/*--  ��  ��������ɥ��ˤ����뤫�ɤ���--*/
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "buy_msg", 
		buff, sizeof( buff)) != NULL )
	{
	    while(getStringFromIndexWithDelim(buff,",",i,buf2,sizeof(buf2)) != FALSE)
		{
			i++;
			if( strstr( szMes, buf2) != NULL) {
				if( CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_EV) == 0) {
					/*-  ����귴  ���ɤ����Υ����å�--*/
					if( CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_NO) == 1) {
						if( sellonlyflg ) {
							NPC_PKPetShop_selectWindow( meindex, talker, 3, -1);
							return;
						}
					}
					else{
						NPC_PKPetShop_selectWindow( meindex, talker, 1, -1);
						return;
					}
				}else{
					if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_NO) == 1) {
						if( sellonlyflg) {
							NPC_PKPetShop_selectWindow( meindex, talker, 3, -1);
							return;
						}
					}else{
						NPC_PKPetShop_selectWindow( meindex, talker, 1, -1);
						return;
					}
		 			return;
		 	 	}
			}
		}
	}
	i=1;

	/*--  ��  �륦����ɥ��ˤ����뤫�ɤ���--*/
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "sell_msg", 
		buff, sizeof( buff)) != NULL )
	{
	    while( getStringFromIndexWithDelim(buff,",", i,buf2,sizeof(buf2))
	     != FALSE )
		{
			i++;
			if(strstr(szMes,buf2) != NULL) {
				NPC_PKPetShop_selectWindow( meindex, talker, 2, -1);
				return;
			}
		}
	}
	i = 1;


	/*--����¾�Υҥ�Ȥ򤯤����դ�  ��������--*/
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "other_msg", 
		buff, sizeof( buff)) != NULL )
	{
	    while(getStringFromIndexWithDelim( buff, ",", i, buf2, sizeof( buf2))
	     !=FALSE)
		{
			i++;
			if(strstr(szMes,buf2) != NULL) {
				/*--�ҥ�ȥ�å�����--*/
				if(NPC_Util_GetStrFromStrWithDelim( argstr, "hint_msg", 
				token, sizeof( token)) != NULL)
				{
					CHAR_talkToCli( talker, meindex, token, CHAR_COLORWHITE);
					return;
				}
			}
		}	
	}

	/*-  ����귴  ���ɤ����Υ����å�--*/
	if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_NO) == 1)
	{
		if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_EV) == 1) 
		{
			if( sellonlyflg) 
			{
				CHAR_talkToCli( talker, meindex, sellmsg, CHAR_COLORWHITE);
				return;
			}
		}
		else
		{	/*--  ����귴  �Υ�å�����--*/
			if( sellonlyflg) 
			{
				NPC_PKPetShop_selectWindow( meindex, talker, 3, -1);
				return;
			}
		}
	}
	else
	{
		if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_EV) == 1) {
			if( sellonlyflg) {
				CHAR_talkToCli( talker, meindex, sellmsg, CHAR_COLORWHITE);
				return;
			}
		}else{
			
			if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_EXPRESS) == 1) {
				NPC_PKPetShop_ExpressmanCheck( meindex, talker);
			}else{ 
				/*--�����ޤ�  ���鶦�̥�����ɥ�(��˥塼)  ��--*/
				/*--  ����귴  �Ǥʤ���  �̤�Ź�ʤ��˥塼  ��--*/
				NPC_PKPetShop_selectWindow( meindex, talker, 0, -1);
			}
		}
	}
				
}

static void NPC_PKPetShop_selectWindow( int meindex, int talker, int num,int select)
{

	print("\n NPC_PKPetShop_selectWindow()");
	print("\n num = %d ", num);
	switch( num) {
	  case 0:
		/*--��˥塼    --*/
		/*--�ѥ�᡼������--*/
		CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);
		
		if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_EXPRESS) == 1 ) 
		{
			if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_NO) ==0 ) 
			{
				NPC_PKPetShop_ExpressmanCheck( meindex, talker);
			}
		}
		else if(CHAR_getWorkInt( meindex, NPC_PKPETSHOP_WORK_NO) == 1) 
		{
		}
		else
		{
		  	NPC_PKPetShop_Menu( meindex, talker);
		}
	  	break;

	  case 1:
	  	/*--  ��    --*/
	  	NPC_PKPetShop_BuyMain( meindex, talker, select);
	  	break;

	  case 2:
	  	/*--  ��    --*/
	  	NPC_PKPetShop_SellMain( meindex, talker, select);
	  	break;

	  case 3:
	  	/*--  ����귴  �Ǥ���    --*/
	  	NPC_LimitPetShop( meindex, talker, select);
	  	break;

	}
}



/*-----------------------------------------
 * ���饤����Ȥ����֤äƤ������˸ƤӽФ����e
 *
-------------------------------------------*/
void NPC_PKPetShopWindowTalked( int meindex, int talkerindex, 
								int seqno, int select, char *data)
{
	/*--       �Τޤ��ˤ��ʤ��Ȥ��Ͻ�   --*/
	if( NPC_Util_CharDistance( talkerindex, meindex ) > 3) {
		/*--�ѥ�᡼������--*/
		CHAR_send_P_StatusString( talkerindex, CHAR_P_STRING_GOLD);
		return;
	}

	makeStringFromEscaped( data);
	switch( seqno){

	  case CHAR_WINDOWTYPE_WINDOWITEMSHOP_STARTMSG:
		/*--�ƥ�˥塼��  �Ф�--*/
		/*--  ��--*/
		if(atoi( data) == 1 )	NPC_PKPetShop_selectWindow(meindex, talkerindex, 1, -1);

		/*--  ��--*/
		if(atoi( data) == 2)	NPC_PKPetShop_selectWindow(meindex, talkerindex, 2, -1);

		/*--�Ф�--*/
		if(atoi( data) == 3)	return;/*--���⤷�ʤ�--*/
	
		break;


	  case CHAR_WINDOWTYPE_WINDOWITEMSHOP_BUY_MSG:
		/*--����  ����ɲ�--*/
		if(NPC_PKPetShop_SetNewPet(meindex , talkerindex, data) == TRUE) {

			NPC_PKPetShop_selectWindow( meindex, talkerindex, 1, 0);

		}else{
			NPC_PKPetShop_selectWindow( meindex, talkerindex ,0, -1);
		}

		break;


	  case CHAR_WINDOWTYPE_WINDOWITEMSHOP_SELL_MSG:
		/*--����  ��κ��--*/
		if(NPC_PKPetShop_SellNewPet(meindex , talkerindex, data) == TRUE) {
			NPC_PKPetShop_selectWindow( meindex, talkerindex, 2, 0);

		}else{
			NPC_PKPetShop_selectWindow( meindex,  talkerindex, 0, -1);
		}

		break;
			
	  case CHAR_WINDOWTYPE_WINDOWITEMSHOP_LIMIT:
		 if(select == WINDOW_BUTTONTYPE_YES) {
			NPC_PKPetShop_selectWindow( meindex, talkerindex ,2, -1);

		}else  if(select == WINDOW_BUTTONTYPE_NO) {
			return;
		}else if(select == WINDOW_BUTTONTYPE_OK) {
				NPC_PKPetShop_selectWindow( meindex, talkerindex, 2, -1);
		}
		break;
	
	  case CHAR_WINDOWTYPE_WINDOWITEMSHOP_EXPRESS:
		if(atoi(data) == 2) {
			NPC_PKPetShop_selectWindow( meindex, talkerindex, 1, -1);
		}else if(atoi( data) == 4) {
			NPC_PKPetShop_selectWindow( meindex, talkerindex, 2, -1);
		}
	}
}


/*-----------------------------------------
 *
 *  ����  �ᥤ��(���饤�����������  �  
 *
 *krynn 2001/12/9 �[������
 *before == -1
 *before <> -1
 *krynn end
 *-----------------------------------------*/
void NPC_PKPetShop_BuyMain(int meindex,int talker,int before )
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char token[NPC_UTIL_GETARGSTR_BUFSIZE];
	int fd = getfdFromCharaIndex( talker);

    if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
       	print("itemshop_GetArgStr_Err");
       	return;
    }

	/*--����Υǡ����Ȥ���---*/
	if(before != -1) 
	{	/*--����Υǡ������ĤäƤ���Τ�¾�ξ�  ������ʤ�    --*/
		sprintf(token,"0|0");

		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_ITEMSHOPMAIN, 
				WINDOW_BUTTONTYPE_NONE, 
				CHAR_WINDOWTYPE_WINDOWITEMSHOP_BUY_MSG,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);
	}else{

		char token2[NPC_UTIL_GETARGSTR_BUFSIZE];
		char buff2[256];
    	char buff[256];

		/*--�ƥ�å��������  ���ƥ��饤����Ȥ�����ǡ�������-*/
		/*--���Τ��  �Ϥ����ʤ�����--*/
		NPC_Util_GetStrFromStrWithDelim( argstr, "main_msg", buff, sizeof( buff));
		NPC_Util_GetStrFromStrWithDelim( argstr, "buy_main", buff2, sizeof( buff2));
		sprintf(token,"0|1|%d|%s|%s|%s|", CHAR_WINDOWTYPE_WINDOWITEMSHOP_STARTMSG,
				CHAR_getChar( meindex, CHAR_NAME), buff, buff2);

		NPC_Util_GetStrFromStrWithDelim( argstr, "what_msg", buff, sizeof( buff));
		NPC_Util_GetStrFromStrWithDelim( argstr, "level_msg", buff2, sizeof( buff));
		snprintf( token2, sizeof( token2), "%s|%s", buff, buff2);

		/*--  ��  ��  --*/
		strncat( token, token2, sizeof( token));
			
		NPC_Util_GetStrFromStrWithDelim( argstr, "realy_msg", buff, sizeof( buff));
		NPC_Util_GetStrFromStrWithDelim( argstr, "itemfull_msg", buff2, sizeof( buff2));
		sprintf( token2, "|%s|%s", buff, buff2);

		/*--  ��    ��--*/
		strncat(token , token2,sizeof(token));
		strcpy(token2, "|");
			
		/*--����  ���  ��      --*/
		NPC_PKPetShop_GetPetList( argstr, token2 );
		// krynn 2001/12/12 bebug ��
		print("%s",token2);
		// end krynn

		/*--��å������ȥ���  ���  �ι�  --*/
		strncat( token, token2, sizeof( token));
	}

		/*--  ��  �򥨥������פ�����--*/
//		makeEscapeString( token, escapedname, sizeof(escapedname));

	/*--����������--*/
	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_ITEMSHOPMAIN, 
				WINDOW_BUTTONTYPE_NONE, 
				CHAR_WINDOWTYPE_WINDOWITEMSHOP_BUY_MSG,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);

}

/*------------------------------------------------------
 *����  ��ط���  ��  ����(  ��  
 *------------------------------------------------------*/
void NPC_PKPetShop_GetPetList(char *argstr,char *argtoken)
{

	int i = 1;
	int tmp;
	int EnemyCnt;
	char *name ;
	char buff2[256];
	char buff[NPC_UTIL_GETARGSTR_LINEMAX];
	char token2[NPC_UTIL_GETARGSTR_BUFSIZE];
	double rate = 1.0;
	int loopcnt = 0;

	/*-�졼�Ȥ�  ��e  ����У��Ǹ���--*/
	if(NPC_Util_GetStrFromStrWithDelim( argstr, "buy_rate", buff2, sizeof( buff2))
	 != NULL){
		rate = atof( buff2);
	}
		
	/*-  ������  ���    --*/
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "PetList", buff, sizeof( buff))
	 != NULL )
	{
	    while( getStringFromIndexWithDelim(buff,",",i,buff2,sizeof(buff2))
	     !=FALSE )
	    {
			i++;
	    	/*--����ե�����Υ���  �ब "-"�Ƕ��ڤ��Ƥ��뤫�Υ����å�--*/
			if(strstr( buff2, "-") == NULL) {
				/*--�ޤ�  ��      --*/
				// krynn 2001/12/10
				EnemyCnt = ENEMY_getEnemyNum();
				for( tmp=0 ; tmp < EnemyCnt ; tmp++ )
				{
					if( ENEMY_getInt( tmp , ENEMY_ID ) == atoi( buff2 ) )
					{
						print("\nNPC_PKPetShop_GetPetList: tmp = %d", tmp);
						break;
					}
				}
				if( tmp == EnemyCnt )
				{
					return;
				}
				name = ENEMY_getChar( tmp , ENEMY_NAME );
				// krynn end
				/*--  ����        �ʤ�b����  �ब¸�ߤ��ʤ��ΤǤ���  ���  ��--*/
				if(name == NULL) continue;

				loopcnt++;
				if(loopcnt == MAXSHOPPET) break;

				/*--  �ݤΥפҥȥ���Ĥ���--*/
				NPC_PetStrStr( atoi( buff2), rate, name, token2, tmp);

				/*--��å�����  ��  ��  ��--*/
	    		strncat( argtoken, token2, sizeof(token2));
			}
			else
			{
				return;
				/*krynn 2001/12/13 �o�q���ӥΤ���F�A�� mark�A�אּ���� return
				--����  �ब  15-25  �η��Ƕ��ڤ��Ƥ�����--
				int start;
				int end;

				//-"-"�Ƕ��ڤ�줿�Ϥ�ο��ͤ�  �ο��ͤ��  --
				getStringFromIndexWithDelim( buff2, "-", 1, token2, sizeof(token2));
				start = atoi( token2);
				getStringFromIndexWithDelim( buff2, "-", 2 ,token2, sizeof(token2));
				end = atoi( token2);

				//--  �椬�դˤʤäƤ�����b  ���ؤ���
				if(start > end)
				{
					tmp = start;
					start = end;
					end = tmp;
				}

				end++;

				//--"-"�Ƕ��ڤ�줿ʬ�Υ���  ���  ��  ��--
				for(; start < end ; start++ ) 
				{
					//--�ޤ�  ��      --

				 	name = ITEM_getNameFromNumber( start );
					//--  ����        �ʤ�b����  �ब¸�ߤ��ʤ��ΤǤ���  ���  ��--
					if(name == NULL) continue;

					loopcnt++;
					if(loopcnt == MAXSHOPPET) break;

					//--  �ݤΥפҥȥ���Ĥ���--
					NPC_PetStrStr( start, rate, name, token2, tmp);

					//--��å�����  ��  ��  ��--
		    		strncat( argtoken, token2, sizeof(token2));
				}*/
			}
		}
	}
}


/*-----------------------------------------------------------------
	�פҥȥ���
-------------------------------------------------------------------*/
void NPC_PetStrStr(int petID,double rate,char *name,char *token2,int index)
{
	int i;
	int gold;
	int level;
	int graNo;
	int TempNo;
	int EnemyTempNum;
	char info[1024];
	//char tryItem[256];	// krynn 2001/12/12 �u�O�n�� getItemInfoFromNumber Ū�X�Ӫ��r�������˪�
	char escape[256] = {"PK Server �d"};	

	//krynn 2001/12/10 try
	//gold  = ITEM_getcostFromITEMtabl( itemID);
	//level = ITEM_getlevelFromITEMtabl( itemID);
	//graNo = ITEM_getgraNoFromITEMtabl( itemID);
	TempNo = ENEMY_getInt( index , ENEMY_TEMPNO );
	EnemyTempNum = ENEMYTEMP_getEnemyNum();
	for( i=0 ; i < EnemyTempNum ; i++ )
	{
		if( ENEMYTEMP_getInt( i , E_T_TEMPNO ) == TempNo )
		{
			break;
		}
	}
	if( i == EnemyTempNum )
	{
		return;
	}
	gold  = RAND(0,20);
	level = 0;
	graNo = ENEMYTEMP_getInt( i , E_T_IMGNUMBER );
	//print("\nPKPetShop::NPC_PetStrStr(): TempNo = %d ; graNo = %d",TempNo,graNo);
	/*--�졼�Ȥ򤫤���--*/
	gold=(int)(gold * rate);

	//strcpy( tryItem,ITEM_getItemInfoFromNumber( 10 ) );
	//print("PKPetShop::NPC_PetStrStr(): try = %s\n",tryItem);
	//krynn end

	makeEscapeString( escape, info, sizeof( info));

	makeEscapeString( name, escape, sizeof( escape));

	sprintf( token2, "%s|0|%d|%d|%d|%s|", escape, level, gold, graNo, info);
}


/*-------------------------------------------
 *(  ��)
 *���饤����Ȥ����֤ä�  ����̤�  �Ǥ�����
 *
 *------------------------------------------*/
BOOL NPC_PKPetShop_SetNewPet(int meindex,int talker,char *data)
{

	char buf[1024];
	char buff2[128];
	int i = 1, j = 1;
	int select;
	int kosuu = 0;
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	double rate = 1.0;
	int gold = 0;		
	int EmptyPetCnt=0;

	/*--�֤äƤ����ǡ����򥻥쥯�ȤȸĿ���ʬ��--*/
	print("\nNPC_PKPetShop_SetNewPet: data = %s",data);
	getStringFromIndexWithDelim( data, "|", 1, buf, sizeof( buf));
	select=atoi(buf);		// krynn 2001/12/10  select �O���a�Ǧ^�n�R���ĴX��
	print("\nNPC_PKPetShop_SetNewPet: select = %d",select);
	if(select == 0) return FALSE;
	getStringFromIndexWithDelim( data, "|", 2, buf, sizeof( buf));
	kosuu=atoi(buf);
	print("\nNPC_PKPetShop_SetNewPet: kosuu(���a�n�R���ƶq) = %d",kosuu);

	if( kosuu <= 0 ) return FALSE;
    
	/* �����  �ΥڥåȤζ�����õ�� */
    if( !CHAR_CHECKINDEX(talker) )
	{
		return FALSE;
	}
	for( i=0 ; i < CHAR_MAXPETHAVE ; i++ ) 
	{
	    if( CHAR_getCharPet( talker,i) == -1 )
		{
			EmptyPetCnt++;
		}
    }
	// �� EmptyPetCnt ���d���Ŧ�
    /* ������  �� */
 	print("\nNPC_PKPetShop_SetNewPet:EmptyPetCnt(���a�����Ŧ�) = %d",EmptyPetCnt);
	if( EmptyPetCnt <= 0 ) return FALSE;
	if( EmptyPetCnt > CHAR_MAXPETHAVE )
	{
		EmptyPetCnt = CHAR_MAXPETHAVE;
	}
	if( kosuu > EmptyPetCnt )
	{
		kosuu = EmptyPetCnt;
	}

	// krynn 2001/12/10
	// ���ӥΤ���A�ҥH mark �_��
	/*--�Ŀ��Υ����å�  ������    �뤫��
	for( i = CHAR_STARTITEMARRAY ; i < CHAR_MAXITEMHAVE ; i++ ) {
		itemindex = CHAR_getItemIndex( talker , i );

		if( !ITEM_CHECKINDEX( itemindex) ) {
			kosuucnt++;
		 }
	}

	--�֤ä�  ���Ŀ���  ��  �ݤθĿ�  ������¦  ���¿���ȶ��������Τ�
	--�����С�¦��  ��  ���
	if( kosuucnt < kosuu){
		kosuu = kosuucnt;
	}
	--���Ҥξ��ϥ��顼
	if(kosuucnt == 0 ){
		return FALSE;
	}
	krynn end */

	i = 1;

	/*--��Ź�Υե�����  �����ϥե����뤬�����ʤ��ä��Ȥ��Ͻ�  --*/
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
       	print("shop_GetArgStr_Err");
       	return FALSE;
	}

	/*---�졼�Ȥ��    �ʤ����1.0)-*/
	if(NPC_Util_GetStrFromStrWithDelim( argstr, "buy_rate", buf, sizeof( buf))
	 != NULL) {
		rate= atof( buf);
	}
	
	/*--����  ����ɲä�Ԥ��Ȥ���-*/
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "PetList", 
		buf, sizeof( buf)) != NULL )
	{
		while(getStringFromIndexWithDelim(buf , "," , j, buff2, sizeof(buff2))
		 != FALSE )
		{
			j++;
			/*--  "-"���ޤޤ�Ƥ��뤫�ɤ���--*/
			if(strstr( buff2, "-") == NULL)
			{	// krynn 2001/12/10 try
				//if( ITEM_getcostFromITEMtabl(atoi(buff2)) !=-1) {
					// 
					if ( i == select)
					{
						/*---����  ��κ---*/
						/*--�Ŀ�ʬ�--*/
						if(NPC_AddPetBuy(meindex, talker,atoi(buff2),kosuu,rate) != TRUE)
						{
							return FALSE;
						}
						return TRUE;
					}
					i++;		
				// krynn end}
			}else{
				/*--����  �ब  15-25  �η�������줿���--*/
				int start;
				int end;

				/* "-"�Ƕ��ڤ�줿�Ϥ�ο��ͤ�  �ο��ͤ��  --*/
				getStringFromIndexWithDelim( buff2, "-", 1, argstr, sizeof(argstr));
				start = atoi( argstr);
				getStringFromIndexWithDelim( buff2, "-", 2 ,argstr, sizeof(argstr));
				end = atoi( argstr);
				end++;

				/*--  �椬�դˤʤäƤ�����b  ���ؤ���**/
				if(start > end){
					gold = start;
					start = end;
					end = gold;
				}

				/*--"-"�Ƕ��ڤ�줿ʬ�Υ���  ����  ��  ��--*/
				for(; start < end ; start++ ) {
					if( ITEM_getcostFromITEMtabl( start) != -1) {
						if ( i == select) {
							/*---����  ��κ---*/
							/*--�Ŀ�ʬ�--*/
							if(NPC_AddPetBuy(meindex, talker, start, kosuu, rate) != TRUE)
							{
								return FALSE;
							}
							return TRUE;
						}
						i++;
					}
				}
			}
		}
	}

	return FALSE;

}


/*---------------------------------------------
 *����  ����ɲä�Ԥ�
 *--------------------------------------------*/
BOOL NPC_AddPetBuy(int meindex, int talker,int petID,int kosuu,double rate)
{
	int i,j,k,index,EnemyCnt,UpLevel;
	int gold;
	int ret;
	int maxgold;
	int Grade=0;
	char buf[1024];
	char msgbuf[64];
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	
	// krynn 2001/12/15 get get's grade of this PKPetShop
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
       	print("shop_GetArgStr_Err");
       	return FALSE;
	}
	if(NPC_Util_GetStrFromStrWithDelim( argstr, "Grade", buf, sizeof( buf))
	 != NULL) 
	{
		Grade = atoi( buf );
	}
	// end krynn

	/*--�졼�Ȥ򤫤���--*/
	// krynn 2001/12/11 mark and change
	//gold = ITEM_getcostFromITEMtabl( itemID);
	gold = 4;
	// krynn end
	gold = (int)(gold * rate);
	maxgold = gold * kosuu;

	if(CHAR_getInt( talker, CHAR_GOLD) < maxgold ) return FALSE;

	/*--����򸺤餹--*/
	CHAR_setInt( talker, CHAR_GOLD,CHAR_getInt( talker, CHAR_GOLD) - maxgold);

	EnemyCnt = ENEMY_getEnemyNum();
	for( index=0 ; index < EnemyCnt ; index++ )
	{
		if( ENEMY_getInt( index , ENEMY_ID ) == petID )
		{
			print("\nNPC_AddPetBuy: index = %d", index);
			break;
		}
	}
	if( index == EnemyCnt )
	{
		return FALSE;
	}

	/*--�Ŀ�ʬ�--*/
	for(i = 0 ; i < kosuu ; i++)
	{
		if( (ret = ENEMY_createPetFromEnemyIndex( talker , index )) == -1 )
		{
			return FALSE;
		};
		/******************/
		/* �ڥåȾ�  ���� */
		/******************/
		// �ɤ��ˤϤ��ä�����
		for( j = 0 ; j < CHAR_MAXPETHAVE ; j++ )
		{
			if( CHAR_getCharPet( talker , j ) == ret )
			{
				break;
			}
		}
		if( j == CHAR_MAXPETHAVE ){
			return FALSE;
		}
		if( CHAR_CHECKINDEX( ret ) == TRUE ){
			CHAR_setMaxExpFromLevel( ret, Grade);
			UpLevel = CHAR_LevelUpCheck( ret , talker);
			for( k = 0; k < UpLevel; k ++ ){
				CHAR_PetLevelUp( ret );
				CHAR_PetAddVariableAi( ret, AI_FIX_PETLEVELUP );
			}
			CHAR_complianceParameter( ret );
			CHAR_setInt( ret , CHAR_HP , CHAR_getWorkInt( ret , CHAR_WORKMAXHP ) );

			snprintf( msgbuf, sizeof( msgbuf ), "K%d", j );
			CHAR_sendStatusString( talker, msgbuf );
			
			snprintf( msgbuf, sizeof( msgbuf ), "W%d", j );
			CHAR_sendStatusString( talker, msgbuf );
		}
	}
	CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);
	return TRUE;
}



/*----------------------------------------
 *��˥塼    
 *----------------------------------------*/
void NPC_PKPetShop_Menu(int meindex,int talker)
{
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char	token[NPC_UTIL_GETARGSTR_LINEMAX];
	char	buff[256];
	int		fd = getfdFromCharaIndex( talker);

	/* Ź��  ��  ��å�����| */
	/*--��Ź�Υե�����  �����ϥե����뤬�����ʤ��ä��Ȥ��Ͻ�  --*/
    if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
		print("shop_GetArgStr_Err");
       	return;
    }
    	
    NPC_Util_GetStrFromStrWithDelim( argstr, "main_msg", buff, sizeof( buff));
	snprintf(token, sizeof(token),"%s|%s",CHAR_getChar( meindex, CHAR_NAME), buff);

	//	print("%s",escapedname);
	/*--����������--*/
	//krynn 2001/12/10	�o�̪��ѼƦ��G�i�H���� ItemShop �� code�A���Υά�
	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_ITEMSHOPMENU, 
				WINDOW_BUTTONTYPE_NONE, 
				CHAR_WINDOWTYPE_WINDOWITEMSHOP_STARTMSG,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);
	//krynn end
}


/*-------------------------------------------
 *	  ���  (�פҥȥ����  
 *	
 *-------------------------------------------*/
void NPC_PKPetShop_SellMain(int meindex,int talker,int before)
{

	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char	token[NPC_UTIL_GETARGSTR_BUFSIZE];
	int fd = getfdFromCharaIndex( talker);

    if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
       	print("shop_GetArgStr_Err");
       	return;
    }

	/*--����Υǡ����Ȥ���---*/
	if(before != -1) {

		/*--����Υǡ������ĤäƤ���Τ�¾�ξ�  ������ʤ�    --*/
		sprintf(token,"1|0");
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_ITEMSHOPMAIN
							+CHAR_getWorkInt(meindex,NPC_PKPETSHOP_WORK_NO), 
					WINDOW_BUTTONTYPE_NONE, 
					CHAR_WINDOWTYPE_WINDOWITEMSHOP_SELL_MSG,
					CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
					token);
	
	}else{

		char token2[NPC_UTIL_GETARGSTR_BUFSIZE];
		char buff2[256];
	   	char buff[256];

		/*--�ƥ�å��������  ���ƥ��饤����Ȥ�����ǡ�������-*/
		/*--���Τ��  �Ϥ����ʤ�����--*/
		NPC_Util_GetStrFromStrWithDelim( argstr, "main_msg", buff, sizeof( buff));
		NPC_Util_GetStrFromStrWithDelim( argstr, "sell_main", buff2, sizeof( buff));
		sprintf( token, "1|1|%d|%s|%s|%s|", CHAR_WINDOWTYPE_WINDOWITEMSHOP_STARTMSG,
				CHAR_getChar( meindex, CHAR_NAME), buff, buff2);

		NPC_Util_GetStrFromStrWithDelim( argstr, "stone_msg", buff, sizeof( buff));

		if(CHAR_getWorkInt(meindex,NPC_PKPETSHOP_WORK_EXPRESS) == 1 ) {
			NPC_Util_GetStrFromStrWithDelim( argstr, "exrealy_msg", buff2, sizeof(buff2));
		}else{
			NPC_Util_GetStrFromStrWithDelim( argstr, "realy_msg", buff2, sizeof( buff2));
		}
		sprintf( token2,"%s|%s|", buff, buff2);

		/*--����  ���  ��      --*/
		NPC_PKPetShop_GetLimtPetList( talker,argstr, token2, -1);

		/*--��å������ȥ���  ���  �ι�  --*/
		strncat( token, token2, sizeof( token));
	
		/*--����������--*/
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_ITEMSHOPMAIN+
					CHAR_getWorkInt(meindex,NPC_PKPETSHOP_WORK_NO), 
					WINDOW_BUTTONTYPE_NONE, 
					CHAR_WINDOWTYPE_WINDOWITEMSHOP_SELL_MSG,
					CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
					token);
	}
}





/*--------------------------------

��ʬ����Ź��  �륢��  ���  ��  ��ν�  

 *-------------------------------*/
int NPC_PKPetShop_GetLimtPetList(int talker, char *argstr, char* token2,int sell)
{

	char token[NPC_UTIL_GETARGSTR_LINEMAX];
	char buff[NPC_UTIL_GETARGSTR_LINEMAX];
	char token3[NPC_UTIL_GETARGSTR_LINEMAX];
	int k = 0 , i = 1 , j = 0;
	int imax;
	int itemtype = 0;
	int itemindex;
	int okflg = 0;
	char buf[256];
	int flg=0;
	int cost;
	
	
	if(sell == -1 ){
		i = CHAR_STARTITEMARRAY;
		imax = CHAR_MAXITEMHAVE;
		flg = -1;
	}else{
		i= sell;
		imax= sell + 1;
		flg = 1;
	}	
	
	/*---  ��륢��  ����  ����e--*/
	/**  ���ڼ���륢��  ����äƤ�����  �졼�ȤǷ׻�--*/
	/*-- ���ڼ�����  �ǥפҥ���ब    --*/

	for( ; i < imax ; i++ ){
		okflg=0;
		itemindex = CHAR_getItemIndex( talker , i );
		
		if( ITEM_CHECKINDEX( itemindex) ){

			/*--����  ��Υ����פ����פ�����b  ���--*/
			if( NPC_Util_GetStrFromStrWithDelim( argstr,"LimitItemType",
			buff, sizeof( buff))
			 != NULL )
			{
				k = 1;
				while(getStringFromIndexWithDelim(buff , "," , k, token, sizeof(token))
				 != FALSE )
				{
#ifdef _ITEM_TYPETABLE
					int cmpmaxitem = sizeof(TypeTable)/sizeof(TypeTable[0]);
#endif
					k++;
#ifdef _ITEM_TYPETABLE
					for(j = 0 ; j < cmpmaxitem ; j++){
#else
					for(j = 0 ; j < ITEM_CATEGORYNUM+3 ; j++){
#endif
						if(strcmp( TypeTable[ j].arg  , token) == 0 ) {
							itemtype = TypeTable[ j].type;
							if(ITEM_getInt(itemindex,ITEM_TYPE) == itemtype) {

								/*--  ��  �--*/
								cost = NPC_GetSellPetList(itemindex,0,argstr,token3,i,sell);
								if(cost != -1) return cost;
								strncat( token2, token3, sizeof( token3));
								okflg = 1;
							}else if(itemtype == 30){
								if( 8 <= ITEM_getInt(itemindex,ITEM_TYPE) 
									&& (ITEM_getInt(itemindex,ITEM_TYPE) <= 15) ){
									/*--  ��  �--*/
									cost = NPC_GetSellPetList(itemindex,0,argstr,token3,i,sell);
									if(cost != -1) return cost;
									strncat(token2,token3,sizeof(token3));
									okflg = 1;
								}
							}else if(itemtype == 40){
								if(( 0 <= ITEM_getInt(itemindex,ITEM_TYPE) 
								  && (ITEM_getInt(itemindex,ITEM_TYPE) <= 4)) 
								 || (17 <= ITEM_getInt(itemindex,ITEM_TYPE) 
								  && (ITEM_getInt(itemindex,ITEM_TYPE) <= 19))
								) {
									/*--  ��  �--*/
									cost = NPC_GetSellPetList(itemindex,0,argstr,token3,i,sell);
									if(cost != -1) return cost;
									strncat(token2,token3,sizeof(token3));
									okflg = 1;
								}
							}else if(itemtype == 50){
								if( 5 <= ITEM_getInt(itemindex,ITEM_TYPE) 
								&& (ITEM_getInt(itemindex,ITEM_TYPE) <= 7) ){
									/*--  ��  �--*/
									cost = NPC_GetSellPetList(itemindex,0,argstr,token3,i,sell);
									if(cost != -1) return cost;
									strncat(token2,token3,sizeof(token3));
									okflg = 1;
								}
							}
							break;
						}
					}
						
					if(okflg == 1) break;
				}
			}
			/*--�����פˤʤ���  ��  �椷��  ��륢��  �फ�����å�--*/
			if( (NPC_Util_GetStrFromStrWithDelim( argstr, "LimitItemNo",
			buff,sizeof( buff))
			 != NULL)
				&& okflg == 0 )
			{
				k = 1;
				while(getStringFromIndexWithDelim(buff , "," , k, token, sizeof(token))
				 != FALSE )
				{
					k++;
					/*--�������ꤵ��Ƥʤ��ä���b  �뤹��-*/
					if(strstr( token, "-")==NULL && strcmp(token,"") != 0) {
						if(ITEM_getInt(itemindex,ITEM_ID) == atoi(token)) {
							/*--  ��  �--*/
							cost = NPC_GetSellPetList(itemindex,0,argstr,token3,i,sell);
							if(cost != -1) return cost;
							strncat(token2,token3,sizeof(token3));
							okflg=1;
						}
					}else if (strstr( token, "-") != NULL){
						int start;
						int end;
						int work;

						/*--  ��  �--*/
						/* "-"�Ƕ��ڤ�줿�Ϥ�ο��ͤ�  �ο��ͤ��  --*/
						getStringFromIndexWithDelim( token, "-", 1, buf, sizeof(buf));
						start = atoi( buf);
						getStringFromIndexWithDelim( token, "-", 2 ,buf, sizeof(buf));
						end = atoi( buf);

						/*--  �椬�դˤʤäƤ�����b  ���ؤ���**/
						if(start > end){
							work = start;
							start = end;
							end = work;
						}

						end++;
						/*--"-"�Ƕ��ڤ�줿ʬ�Υ���  ����  ��  ��--*/
						if( (start <= ITEM_getInt(itemindex,ITEM_ID))
						 && (ITEM_getInt(itemindex,ITEM_ID) < end) )
						 {
							/*--  ��  �--*/
							cost = NPC_GetSellPetList(itemindex,0,argstr,token3,i,sell);
							if(cost != -1) return cost;
							strncat(token2,token3,sizeof(token3));
							okflg = 1;
						}
					}
				}
			}

			/*--  �뤳�Ȥ��Ǥ��ʤ�--*/
			if(okflg == 0) {
				cost = NPC_GetSellPetList(itemindex, 1, argstr, token3, i, sell);
				if(sell != -1) return -1;
				strncat( token2, token3, sizeof( token3));
			}
			
		}
	}
	return -1;
}



/*----------------------------------------------------------

	���饤����Ȥ���������פҥȥ���κ

 *----------------------------------------------------------*/
int NPC_GetSellPetList(int itemindex,int flg, char *argstr,char *argtoken,int select,int sell)
{

	char buff[256];
	double rate = 0.2;
	char buff2[256];
	char buff3[64];
	int k = 1;
	int cost = -1;

	/*--���ڼ����졼��--**/
	if(NPC_Util_GetStrFromStrWithDelim( argstr,"special_item",buff, sizeof( buff))
	!= NULL)
	{
		if(NPC_Util_GetStrFromStrWithDelim( argstr,"special_rate",buff2, sizeof( buff2))
		 != NULL )
		{
			rate = atof(buff2);
		}else{
			rate = 1.2;
		}
		
		while(getStringFromIndexWithDelim(buff , "," , k, buff2, sizeof(buff2)) !=FALSE )
		{
			k++;

			if(strstr( buff2, "-") == NULL && strcmp(buff2,"") != 0) {
				if(ITEM_getInt(itemindex,ITEM_ID) == atoi(buff2)){
					cost = NPC_SellPetstrsStr( itemindex,0, rate, argtoken,select,sell);
					return cost;
				}
			}else if (strstr( buff2, "-") != NULL){
				int start;
				int end;
				int work;

				/*--  ��  �--*/
				/* "-"�Ƕ��ڤ�줿�Ϥ�ο��ͤ�  �ο��ͤ��  --*/
				getStringFromIndexWithDelim( buff2, "-", 1, buff3, sizeof(buff3));
				start = atoi( buff3);
				getStringFromIndexWithDelim( buff2, "-", 2 ,buff3, sizeof(buff3));
				end = atoi( buff3);

				/*--  �椬�դˤʤäƤ�����b  ���ؤ���**/
				if(start > end){
					work = start;
					start = end;
					end = work;
				}
				end++;
	
				/*--"-"�Ƕ��ڤ�줿ʬ�Υ���  ����  ��  ��--*/
				if( (start <= ITEM_getInt(itemindex,ITEM_ID))
					&&  (ITEM_getInt(itemindex,ITEM_ID) < end)
				){
			
					cost = NPC_SellPetstrsStr( itemindex,0, rate, argtoken,select,sell);
					return cost;
				}
			}
		}
	}

	/*--�Ρ��ޥ�졼��--*/
	if( NPC_Util_GetStrFromStrWithDelim( argstr,"sell_rate",buff, sizeof( buff))
	 != NULL )
	{
		rate = atof(buff);
		cost = NPC_SellPetstrsStr( itemindex, flg ,rate, argtoken,select,sell);
		return cost;
	}

	return cost;
}


/*------------------------------------------
 
   ��  ����(������)
 
*------------------------------------------*/
int NPC_SellPetstrsStr(int itemindex,int flg,double rate,char *argtoken,int select,int sell)
{
	int cost;
	char escapedname[256];
	char name[256];	
	char *eff;
	

	cost = ITEM_getInt( itemindex, ITEM_COST);
	cost = (int)(cost * rate);

	if(sell != -1) return cost;

		
	//strcpy( escapedname, ITEM_getChar( itemindex, ITEM_NAME));
	strcpy( escapedname, ITEM_getChar( itemindex, ITEM_SECRETNAME));
	makeEscapeString( escapedname, name, sizeof( name));
	eff=ITEM_getChar(itemindex, ITEM_EFFECTSTRING);
	makeEscapeString( eff, escapedname, sizeof(escapedname));



	sprintf(argtoken,"%s|%d|%d|%d|%s|%d|",
			name,
			flg,
			cost,
			ITEM_getInt( itemindex, ITEM_BASEIMAGENUMBER),
			escapedname,
			select
	);

	return -1;

}

/*--------------------------------------------
 *
 *���饤����Ȥ����̤��֤äƤ����Ȥ�    ��    
 *
 *-------------------------------------------*/
BOOL NPC_PKPetShop_SellNewPet(int meindex,int talker,char *data)
{
	int select;
	int cost;
	int oldcost;
	char token[32];
	int k;
	int itemindex;
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char token2[256];
	
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {
		print("GetArgStrErr");
		return FALSE;
	}
	
	/*--�֤ä�  ���ǡ�����ʬ��--*/
	getStringFromIndexWithDelim(data , "|" ,1, token, sizeof( token));
	select = atoi(token);
	getStringFromIndexWithDelim(data , "|" ,2, token, sizeof( token));
	oldcost = atoi(token);

	if(select == 0) return FALSE;

	cost = NPC_PKPetShop_GetLimtPetList( talker,argstr, token2,select);


	/*--�㤦��Τ򤦤��Ȥ����Ȥ��Υ��顼--*/
	if(oldcost != cost || cost == -1)
	{
		int fd = getfdFromCharaIndex( talker);
		char token[256];
		
		sprintf(token,"\n\n�u�r!�藍�_"
					"\n\n�藍�_�� ! �i���i�H�A��@���O�H"
		);
		
		k = select;
		itemindex = CHAR_getItemIndex( talker ,k);
		
		
		/*--�ҥ��ν�  --*/
		if(itemindex != -1) {
			print("\n%s(%d,%d,%d):�M��ܪ��F�褣�Perror([%s(%d)]�n�����F�^",
					CHAR_getChar(talker, CHAR_NAME),
					CHAR_getInt( talker, CHAR_FLOOR),
					CHAR_getInt( talker, CHAR_X ),
					CHAR_getInt( talker, CHAR_Y ),
					ITEM_getChar(itemindex, CHAR_NAME),
					ITEM_getInt( itemindex, ITEM_ID )
			);
			LogItem(
					CHAR_getChar( talker, CHAR_NAME ), /* �����   */
					CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD �bitem��log���W�[item�W��
					itemindex,
#else
	       			ITEM_getInt( itemindex, ITEM_ID ),  /* ����  ��  �� */
#endif
					"SellErr",
					CHAR_getInt( talker, CHAR_FLOOR),
					CHAR_getInt( talker, CHAR_X ),
					CHAR_getInt( talker, CHAR_Y ),
					ITEM_getChar( itemindex, ITEM_UNIQUECODE),
						ITEM_getChar( itemindex, ITEM_NAME),
						ITEM_getInt( itemindex, ITEM_ID)
			);
		}else{
			print("\n%s(%d,%d,%d):�M��ܪ��F�褣�Perror(�S������item�s�b�^",
					CHAR_getChar(talker, CHAR_NAME),
					CHAR_getInt( talker, CHAR_FLOOR),
					CHAR_getInt( talker, CHAR_X ),
					CHAR_getInt( talker, CHAR_Y )
			);
			LogItem(
					CHAR_getChar( talker, CHAR_NAME ), /* �����   */
					CHAR_getChar( talker, CHAR_CDKEY ),
	       			-1,  /* ����  ��  �� */
					"SellErr",
					CHAR_getInt( talker, CHAR_FLOOR),
					CHAR_getInt( talker, CHAR_X ),
					CHAR_getInt( talker, CHAR_Y ),
					"-1", "NULL", -1 );
		}
		/*--����������--*/
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, 
				WINDOW_BUTTONTYPE_OK, 
				CHAR_WINDOWTYPE_WINDOWITEMSHOP_LIMIT,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);

		
		return FALSE;
	}

	k = select;
	itemindex=CHAR_getItemIndex( talker ,k);

	if(itemindex != -1) {
		{
			LogItem(
				CHAR_getChar( talker, CHAR_NAME ), /* �����   */
				CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD �bitem��log���W�[item�W��
				itemindex,
#else
	       		ITEM_getInt( itemindex, ITEM_ID ),  /* ����  ��  �� */
#endif
				"Sell",
				CHAR_getInt( talker,CHAR_FLOOR),
				CHAR_getInt( talker,CHAR_X ),
				CHAR_getInt( talker,CHAR_Y ),
				ITEM_getChar( itemindex, ITEM_UNIQUECODE),
				ITEM_getChar( itemindex, ITEM_NAME),
				ITEM_getInt( itemindex, ITEM_ID)

			);
		}
		
	}

	CHAR_DelItem( talker, k);
	CHAR_AddGold( talker, cost);
	CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);

	return TRUE;
}



void NPC_LimitPetShop(int meindex,int talker,int select)
{

	int fd = getfdFromCharaIndex( talker);
	char token[NPC_UTIL_GETARGSTR_LINEMAX];
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buf[1024];

	/*--��Ź�Υե�����  �����ϥե����뤬�����ʤ��ä��Ȥ��Ͻ�  --*/
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {
       	print("shop_GetArgStr_Err");
       	return;
	}

	/*--  ����귴  Ź�Ǥ���å�����-*/
	if(NPC_Util_GetStrFromStrWithDelim( argstr, "sellonly_msg", buf, sizeof( buf))
	!=NULL)
	{
		sprintf(token,"\n\n%s", buf);

		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE, 
				WINDOW_BUTTONTYPE_YESNO, 
				CHAR_WINDOWTYPE_WINDOWITEMSHOP_LIMIT,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);

	}else{
		CHAR_talkToCli( talker, meindex, "�o�O�R��M�����C",CHAR_COLORWHITE);
	}
	return;
}


/*--����������--*/
void NPC_PKPetShop_ExpressmanCheck(int meindex,int talker)
{
	int fd = getfdFromCharaIndex( talker);
	char token[1024];
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buf[1024];

	/*--��Ź�Υե�����  �����ϥե����뤬�����ʤ��ä��Ȥ��Ͻ�  --*/
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {
       	print("shop_GetArgStr_Err");
       	return;
	}


	/*--  ����귴  Ź�Ǥ���å�����-*/
	NPC_Util_GetStrFromStrWithDelim( argstr, "main_msg", buf, sizeof( buf));
	sprintf(token,"4\n              %s\n\n%s"
					"\n\n          ��  ���u  ��      "
				  "\n\n          �ե�I�����"
					,CHAR_getChar(meindex,CHAR_NAME),buf);

	lssproto_WN_send( fd, WINDOW_MESSAGETYPE_SELECT, 
			WINDOW_BUTTONTYPE_CANCEL, 
			CHAR_WINDOWTYPE_WINDOWITEMSHOP_EXPRESS,
			CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
			token);

	return;


}


#endif
 // _PKPETSHOP




