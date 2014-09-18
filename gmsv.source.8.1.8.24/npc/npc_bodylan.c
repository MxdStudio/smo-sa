#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "npc_windowhealer.h"




/*
 *���a�ܥǥ���󥲡�����Ȥ���  ������NPC
 * �ޤ�  ��������줿��b���Υץ쥤�䡼��
 * ����NPC��INDEX����¸����e
 *
 *���aNPC�μ���ǥ�������󤬤��ä���b���������򵯤������ץ쥤�䡼��
 *NPC��INDEX����  ���Ƥ��뤫�����å�
 *�����å�    �ʤ鱭�ϥ��������μ������󥹥����å�����
 *��˾�Υ��������򤷤Ƥ�����ץ쥤�䡼�Υ�����Ȥ򥢥åפ�����e
 *������ȥ��å�  �b����������    �ʤ����ξ��إ�פ�����
 *
 */

enum {
	BODYLAN_E_COMMANDNUM = CHAR_NPCWORKINT1,	// ���ޥ�ɤ�Ĺ��
};


// ������ɥ��⡼��
enum{
	BODYLAN_WIN_FIRST,
	BODYLAN_WIN_LAST_GOOD,
	BODYLAN_WIN_LAST_NG,
	BODYLAN_WIN_GOOD_NO,
	BODYLAN_WIN_ALREADY,
	BODYLAN_WIN_NOT_PREEVENT,
	BODYLAN_WIN_END
};

static void NPC_BodyLan_Profit( int meindex, int playerindex );

static void NPC_BodyLan_Window(
	int meindex,
	int talkerindex,
	int mode
);


/*********************************
* �����  
*********************************/
BOOL NPC_BodyLanInit( int meindex )
{

	char szP[256], szArg1[4096];
	char *szArg;
	char buf[256];
	int i, needSeq;

    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPEEVENT );

    if((szArg = NPC_Util_GetArgStr( meindex, szArg1, sizeof( szArg1)))== NULL ){
    	print( "npc_bodylan.c:�S���޼�(%s)\n",
    		CHAR_getChar(meindex,CHAR_NPCARGUMENT) );
    	return FALSE;
    }


	// �������󥹤�õ��
	if( NPC_Util_GetStrFromStrWithDelim( szArg, "Act", szP, sizeof( szP ) ) == NULL ){
		print( "npc_bodylan:�ʧ@��r�C�|���]�w(%s)\n",	szArg );
		return FALSE;
	}

	for( i = 0 ; ; i ++ ){
		//   �פʽ�  
		if( getStringFromIndexWithDelim( szP, ",", i, buf, sizeof( buf)) != FALSE ){
			needSeq = atoi(buf);
			// �ޥ��ʥ���  ���餳���ޤ�
			if( needSeq < 0 ){
				if( i <= 0 ){
					print( "npc_bodylan:�ʧ@�C�|���]�w(%s)\n", szArg );
				}
				//     ���򥻥å�
				CHAR_setWorkInt( meindex, BODYLAN_E_COMMANDNUM, i );
				break;
			}else{
				// �롼�פ��ƿ�����
			}
		}else{
			if( i <= 0 ){
				print( "npc_bodylan:�ʧ@�C�|���]�w(%s)\n", szArg );
			}
			//     ���򥻥å�
			CHAR_setWorkInt( meindex, BODYLAN_E_COMMANDNUM, i );
			break;
		}
	}



    return TRUE;

}




/*********************************
*   ��������줿���ν�  
*********************************/
void NPC_BodyLanTalked( int meindex , int talkerindex , char *szMes ,int color )
{
	char szP[256], szArg1[4096];
	char *szArg;
	int EventNo = -1,Pre_Event = -1;

    /* �ץ쥤�䡼���Ф��Ƥ���  ������ */
    if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }

	/* ������åɰ�  �Τ� */
	if( NPC_Util_CharDistance( talkerindex, meindex ) > 2 )
	{
		return;
	}

	// ����  ��  
    if( (szArg = NPC_Util_GetArgStr( meindex, szArg1, sizeof( szArg1 )) ) == NULL ){
    	print( "npc_bodylan.c:�S���޼�(%s)\n",
    		CHAR_getChar(meindex,CHAR_NPCARGUMENT) );
    	return ;
    }else{
		// ���٥��  ���  
		if( NPC_Util_GetStrFromStrWithDelim( szArg, "EventNo", szP, sizeof( szP ) ) != NULL ){
			EventNo = atoi(szP);
		}
		// ����  �ץ��٥��  ���  
		if( NPC_Util_GetStrFromStrWithDelim( szArg, "Pre_Event", szP, sizeof( szP ) ) != NULL ){
			Pre_Event = atoi(szP);
		}
	}

	// ������  �ץ��٥��  �椬�����
	if( Pre_Event >= 0 ){
		// ���٥�Ȥ��Ф��Ƥɤ����Ƥ��뤫�����å�
		if( NPC_EventCheckFlg( talkerindex, Pre_Event ) == FALSE ){
			//   �ץ��٥�Ȥ򥯥ꥢ���Ƥ��ʤ��ä��餳�Υ����
			NPC_BodyLan_Window( meindex, talkerindex, BODYLAN_WIN_NOT_PREEVENT );
			return;
		}
	}
	// ���٥��  �椬�����
	if( EventNo >= 0 ){
		// ���٥�Ȥ��Ф��Ƥɤ����Ƥ��뤫�����å�
		if( NPC_EventCheckFlg( talkerindex, EventNo ) == TRUE ){
			//   �äƤ����餳�Υ����
			NPC_BodyLan_Window( meindex, talkerindex, BODYLAN_WIN_ALREADY );
			return;
		}
	}

	// ���󤳤Υץ쥤�䡼�ϼ�ʬ�����äƤ�����
	if( CHAR_getWorkInt( talkerindex, CHAR_WORKTRADER ) == meindex ){

		// ����˥��ޥ�ɤ���  ��  ���Ƥ�����
		if( CHAR_getWorkInt( talkerindex, CHAR_WORKSHOPRELEVANT )
		 >= CHAR_getWorkInt( meindex, BODYLAN_E_COMMANDNUM )
		){
			// ��  ������    ����
			NPC_BodyLan_Window( meindex, talkerindex, BODYLAN_WIN_LAST_GOOD );
			return;
		}else{
			// ��  
			NPC_BodyLan_Window( meindex, talkerindex, BODYLAN_WIN_LAST_NG );
			return;
		}
	}else{
		// ���ƤʤΤǳФ���
		// �ץ쥤�䡼�˼�ʬ�Υ���ǥå�������¸������
		CHAR_setWorkInt( talkerindex, CHAR_WORKTRADER, meindex );
		// �������󥹤�  �餫��˽����
		CHAR_setWorkInt( talkerindex, CHAR_WORKSHOPRELEVANT, 1 );

		// ���κݥ���դȤ�����٤����ʤ��z�z�z
		NPC_BodyLan_Window( meindex, talkerindex, BODYLAN_WIN_FIRST );
		return;
	}
}


/*=======================================
 * watch ��  
 *======================================*/
void NPC_BodyLanWatch(
	int objmeindex,
	int objmoveindex,
    CHAR_ACTION act,
    int x,
    int y,
    int dir,
    int* opt,
    int optlen
)
{
	char szP[256], szArg1[4096];
	char *szArg;
	char buf[256];
	int actindex;
	int meindex;
	int seqNo, needSeq;

	// ����饯���ʳ��ϥ꥿����
	if( OBJECT_getType(objmoveindex) != OBJTYPE_CHARA ) return;
	actindex = OBJECT_getIndex(objmoveindex);
	// �ץ쥤�䡼�ʳ��ϥ꥿����
	if( CHAR_getInt( actindex, CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) return;

	// ��ʬ�Υ���ǥå���
	meindex = OBJECT_getIndex(objmeindex);

	// ��ʬ��INDEX����  ���Ƥ��뤫�����ʤ���Х꥿����
	if( CHAR_getWorkInt( actindex, CHAR_WORKTRADER ) != meindex ){
		return;
	}

	// �ɤ��ޤǥ��������򤷤Ƥ��뤫
	seqNo = CHAR_getWorkInt( actindex, CHAR_WORKSHOPRELEVANT );
	//   �ʾ���  �餫��
	if( seqNo < 1 )seqNo = 1;

	// ����  ��  
    if((szArg = NPC_Util_GetArgStr( meindex, szArg1, sizeof( szArg1 )) ) == NULL ){
    	print( "npc_bodylan.c:�S���޼�(%s)\n",
    		CHAR_getChar(meindex,CHAR_NPCARGUMENT) );
    	return ;
    }


	// �������󥹤�õ��
	if( NPC_Util_GetStrFromStrWithDelim( szArg, "Act", szP, sizeof( szP ) ) == NULL ){
		print( "npc_bodylan:�ʧ@��r�C�|���]�w(%s)\n",	szArg );
		return;
	}

	//   �פʽ�  
	if(getStringFromIndexWithDelim( szP, ",", seqNo, buf, sizeof( buf)) != FALSE ){
		needSeq = atoi(buf);
	}else{
		// �ʤ����ʤ��ä���  �餫��
		CHAR_setWorkInt( actindex, CHAR_WORKSHOPRELEVANT, 1 );

//		print( "���L�Y�F�C�^��̪�C\n", seqNo, needSeq );
		// ��������٤����z�z�z
		return;
	}

	// ����  �פʥ�������󤬰��פ���
	if( needSeq == act ){
		// ���פ����e���줬    ��
//		print( "���\\(%d���ƬO%d)\n", seqNo, needSeq );
		seqNo ++;
		if( seqNo >= CHAR_getWorkInt( meindex, BODYLAN_E_COMMANDNUM ) ){
//			print( "�b�������C\n" );
		}
		// ���ΰ�  ����¸
		CHAR_setWorkInt( actindex, CHAR_WORKSHOPRELEVANT, seqNo );
	}else{
		// ��  ��������  �餫����  ��
		CHAR_setWorkInt( actindex, CHAR_WORKSHOPRELEVANT, 1 );
//		print( "(%d���ƬO%d)\n", seqNo, needSeq );
	}

}



//********* ��  ���Τ�     *********
static void NPC_BodyLan_Profit( int meindex, int playerindex )
{
	char szArg1[4096], szP[256];
	char *szArg;
	int fl, x, y, pmode, i, subindex, parent;

	// ����  ��  
    if(( szArg = NPC_Util_GetArgStr( meindex, szArg1, sizeof( szArg1 ) )) == NULL ){
    	print( "npc_bodylan.c:�S���޼�(%s)\n",
    		CHAR_getChar(meindex,CHAR_NPCARGUMENT) );
    	return ;
    }

	//*********************************************
	//
	//   ��    ���Σ��e�������
	//
	//*********************************************
	if( NPC_Util_GetStrFromStrWithDelim( szArg, "Warp", szP, sizeof( szP ) ) != NULL ){
		// ��    �˥�סe��  ���
		if( sscanf( szP, "%d,%d,%d", &fl, &x, &y ) == 3 ){
		}else{
			print( "npc_bodylan: �L�kŪ���Ŷ��y��(%s)\n", szP );
			return;
		}
		// �ѡ�  ���Ȥ�Ǥ���Ҷ�����
		pmode = CHAR_getWorkInt( playerindex, CHAR_WORKPARTYMODE );
		switch( pmode ){
		case 1: // ��ʬ����
			parent = playerindex;
			break;
		case 2: // ��ʬ���Ҷ��e�Ƥ��  
			parent = CHAR_getWorkInt( playerindex, CHAR_WORKPARTYINDEX1 );
			break;
		default:
			// �ѡ�  ������ʤ��ä��e��ʬ�������
			CHAR_warpToSpecificPoint( playerindex, fl, x, y );
			return;
		}
		// �������
		for( i = 0; i < CHAR_PARTYMAX; i ++ ){
			subindex = CHAR_getWorkInt( parent, CHAR_WORKPARTYINDEX1+i );
			if( CHAR_CHECKINDEX( subindex ) == FALSE )continue;
			// �ѡ�  ���������
			CHAR_warpToSpecificPoint( subindex, fl, x, y );
		}
	}



}

#if 1
static void NPC_BodyLan_Window(
	int meindex,
	int talkerindex,
	int mode
)
{
	char token[1024];
	char escapedname[2048];
	char szArg1[4096];
	char *szArg;
	char szP[256];
	int fd;
	int buttontype = 0, windowtype = 0, windowno = 0;

	if( CHAR_CHECKINDEX( talkerindex ) == FALSE )return;
	fd = getfdFromCharaIndex( talkerindex );

	// ����  ��  
    if(( szArg = NPC_Util_GetArgStr( meindex, szArg1, sizeof( szArg1 ) )) == NULL ){
    	print( "npc_bodylan.c:�S���޼�(%s)\n",
    		CHAR_getChar(meindex,CHAR_NPCARGUMENT) );
    	return ;
    }

	szP[0] = 0;

	switch( mode ){
	  case BODYLAN_WIN_FIRST:
		//   ���  ���줿���Υ����
		if( NPC_Util_GetStrFromStrWithDelim( szArg, "First", szP, sizeof( szP ) ) == NULL ){
			print( "npc_bodylan:�@�}�l���ܪ���r�S����J(%s)\n",	szArg );
			return;
		}
		sprintf( token,"%s", szP );
		buttontype=WINDOW_BUTTONTYPE_YES|WINDOW_BUTTONTYPE_NO;
	  	windowtype=WINDOW_MESSAGETYPE_MESSAGE;
	  	windowno=mode;
	  	break;

	  case BODYLAN_WIN_LAST_GOOD:
		//     ��������ä���
		if( NPC_Util_GetStrFromStrWithDelim( szArg, "Good", szP, sizeof( szP ) ) == NULL ){
			print( "npc_bodylan:����ɻ�����r�S����J(%s)\n",	szArg );
			return;
		}
		sprintf( token,"%s", szP );
		buttontype=WINDOW_BUTTONTYPE_YESNO;// YES|NO
	  	windowtype=WINDOW_MESSAGETYPE_MESSAGE;
	  	windowno=mode;
	  	break;

	  case BODYLAN_WIN_LAST_NG:
		//     ��������ä���
		if( NPC_Util_GetStrFromStrWithDelim( szArg, "Ng", szP, sizeof( szP ) ) == NULL ){
			print( "npc_bodylan:�����ɻ�����r�S����J(%s)\n",	szArg );
			return;
		}
		sprintf( token,"%s", szP );
		buttontype=WINDOW_BUTTONTYPE_OK;// OK
	  	windowtype=WINDOW_MESSAGETYPE_MESSAGE;
	  	windowno=mode;
	  	break;

	  case BODYLAN_WIN_GOOD_NO:
		// �������    �򥭥�󥻥�
		if( NPC_Util_GetStrFromStrWithDelim( szArg, "Good_No", szP, sizeof( szP ) ) == NULL ){
			print( "npc_bodylan:����ɨ������~����r�S����J(%s)\n",	szArg );
			return;
		}
		sprintf( token,"%s", szP );
		buttontype=WINDOW_BUTTONTYPE_OK;// OK
	  	windowtype=WINDOW_MESSAGETYPE_MESSAGE;
	  	windowno=mode;
	  	break;

	  case BODYLAN_WIN_ALREADY:
		// ���Ǥ˥��٥�Ȥ�  ���Ƥ������
		if( NPC_Util_GetStrFromStrWithDelim( szArg, "Good_No", szP, sizeof( szP ) ) == NULL ){
			print( "npc_bodylan:�ƥ󵲧��ɪ���r�S����J(%s)\n",	szArg );
			return;
		}
		sprintf( token,"%s", szP );
		buttontype=WINDOW_BUTTONTYPE_OK;	// OK
	  	windowtype=WINDOW_MESSAGETYPE_MESSAGE;
	  	windowno=mode;
	  	break;

	case BODYLAN_WIN_NOT_PREEVENT:
		// ������  �פʥ��٥�Ȥ򤳤ʤ��Ƥ��ʤ����
		if( NPC_Util_GetStrFromStrWithDelim( szArg, "Pre_Not", szP, sizeof( szP ) ) == NULL ){
			print( "npc_bodylan:?�ƫe�ƥ󵲧�����r�S����J(%s)\n",	szArg );
			return;
		}
		sprintf( token,"%s", szP );
		buttontype=WINDOW_BUTTONTYPE_OK;	// OK
	  	windowtype=WINDOW_MESSAGETYPE_MESSAGE;
	  	windowno=mode;
	  	break;
	  default:
	  	return;
	}

	makeEscapeString( token, escapedname, sizeof(escapedname));
	/*-��������������--*/
	lssproto_WN_send( fd, windowtype,
					buttontype,
					windowno,
					CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
					escapedname);


}



/*-----------------------------------------
���饤����Ȥ����֤äƤ������˸ƤӽФ����e
-------------------------------------------*/
void NPC_BodyLanWindowTalked(
	int meindex,
	int talkerindex,
	int seqno,
	int select,
	char *data
)
{


	if( NPC_Util_CharDistance( talkerindex, meindex ) > 2) return;

	switch( seqno){
	case BODYLAN_WIN_LAST_GOOD:	// �������OK�����줿��
		if(select==WINDOW_BUTTONTYPE_YES ){
			NPC_BodyLan_Profit( meindex, talkerindex );
			// �ץ쥤�䡼�˼�ʬ�Υ���ǥå�����˺�줵����
			CHAR_setWorkInt( talkerindex, CHAR_WORKTRADER, -1 );
			// �������󥹤�  �餫��˽����
			CHAR_setWorkInt( talkerindex, CHAR_WORKSHOPRELEVANT, 1 );
		}else
		if( select == WINDOW_BUTTONTYPE_NO ){
			// ������˥���󥻥뤵�줿��
			NPC_BodyLan_Window( meindex, talkerindex, BODYLAN_WIN_GOOD_NO );
			// �ץ쥤�䡼�˼�ʬ�Υ���ǥå�����˺�줵����
			CHAR_setWorkInt( talkerindex, CHAR_WORKTRADER, -1 );
			// �������󥹤�  �餫��˽����
			CHAR_setWorkInt( talkerindex, CHAR_WORKSHOPRELEVANT, 1 );
		}
		break;
	default:
		break;
	}

}



#endif

