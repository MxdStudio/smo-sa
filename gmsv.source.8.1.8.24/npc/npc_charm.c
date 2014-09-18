#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"
#include "npc_charm.h"

//    ���  ���׻�����

//��٥��        ��  ���ߤ�    /          

//�������ϡa���Ǥ��e
/*
#define RATE  4		//�졼�ȡ�
#define CHARMHEAL 5 //    �β�����
#define WARU	3	//    ������

*/

#define RATE  10	//�졼�ȡ�
#define CHARMHEAL 5 //    �β�����
#define WARU	3	//    ������


static void NPC_Charm_selectWindow( int meindex, int toindex, int num);
int NPC_CharmCost(int meindex,int talker);
void NPC_CharmUp(int meindex,int talker);


/*********************************
* �����  
*********************************/
BOOL NPC_CharmInit( int meindex )
{
	/*--�����Υ����פ�����--*/
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPECHARM );
	return TRUE;

}


/*********************************
*   ��������줿���ν�  
*********************************/
void NPC_CharmTalked( int meindex , int talkerindex , char *szMes ,int color )
{

    /* �ץ쥤�䡼���Ф��Ƥ���  ������ */
    if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER )
    {
    	return;
    }
	
	/*--  �����ˤ��뤫�ɤ�����--*/
	if(NPC_Util_isFaceToFace( meindex ,talkerindex , 2) == FALSE) {
		/* ������åɰ�  �Τ� */
		if(NPC_Util_isFaceToChara( talkerindex, meindex, 1) == FALSE) return;
	}

	NPC_Charm_selectWindow( meindex, talkerindex, 0);
}


/*
 * �ƽ�  ��ʬ����
 */
static void NPC_Charm_selectWindow( int meindex, int toindex, int num)
{

	char token[1024];
	char escapedname[1024];
	int fd = getfdFromCharaIndex( toindex);
	int buttontype = 0;
	int windowtype = 0;
	int windowno = 0;
	int cost = 0;
	int chartype;
	
	/*--������ɥ������ץ�å��������������Τ��������--*/
  	windowtype = WINDOW_MESSAGETYPE_MESSAGE;

	switch( num) {
	  case 0:
  		/*--��      --*/
		sprintf(token,"4\n          = = ���e�v = = "
				     "\n\n   �w����{�A���ѭn������O�H"
				  "\n "
				  "\n\n        �m  ���ھy�O  �n "
				  "\n\n        �m ����]���� �n "
		);

	  	buttontype = WINDOW_BUTTONTYPE_NONE;
	  	windowtype = WINDOW_MESSAGETYPE_SELECT;
	  	windowno = CHAR_WINDOWTYPE_CHARM_START; 
	  	break;

	case 1:
		cost = NPC_CharmCost( meindex, toindex);
		if(cost == -1){
			sprintf(token,"             = = ���e�v = =  "
				      "\n\n        �A���y�O�u�O����"
				      "\n\n      �Хû��O���A���y�O��C"
			);
		  	buttontype = WINDOW_BUTTONTYPE_OK;
		}else{
			sprintf(token,"             = = ���e�v = =  "
				 	    "\n"
					  "\n\n �n�N�A���y�O�W�ɤ��I����"
					  "\n        �ݭn%6d��stone��I"
						"\n\n      �Y�ϳo�ˤ]�i�H�ܡH",cost
			);
		  	buttontype = WINDOW_BUTTONTYPE_YESNO;

		}
	  	windowtype = WINDOW_MESSAGETYPE_MESSAGE;
	  	windowno = CHAR_WINDOWTYPE_CHARM_END; 

		break;

	case 2:
		cost = NPC_CharmCost( meindex, toindex);
		chartype = CHAR_getInt( toindex, CHAR_IMAGETYPE);
		
		if(cost > CHAR_getInt( toindex, CHAR_GOLD)) {
			sprintf(token,"             = = ���e�v = =  "
					  "\n\n     �u�r!�A����������I"
				        "\n        ���h�s�n���U���A�ӧa�I"
			);

		}else{
			NPC_CharmUp( meindex, toindex);

			/*--�����Υ����פˤ�äƥ�å�������  ���Ƥߤ�--*/
			switch( chartype) {
			  case CHAR_IMAGETYPE_GIRL:
				sprintf(token,"             = = ���e�v = =  "
					  "\n\n   ��,�o�˧A���y�O�N�W��ù�I"
					  "\n\n      �U�ӷU�i�R�F�O�I"
				);

				break;
			  case CHAR_IMAGETYPE_BOY:
				sprintf(token,"             = = ���e�v = =  "
					  "\n\n   ��,�o�˧A���y�O�N�W��ù�I"
					  "\n\n    �O���O���~�n�Ӧh�F�O�H"
				);
			  	break;
			  	
			  case CHAR_IMAGETYPE_CHILDBOY:
			  case CHAR_IMAGETYPE_CHILDGIRL:
				sprintf(token,"             = = ���e�v = =  "
					  "\n\n   ��,�o�˧A���y�O�N�W��ù�I"
					  "\n\n      �u���ܱo�n�i�R��I"
				);
			 	break;
			 	
			   case CHAR_IMAGETYPE_MAN:
				sprintf(token,"             = = ���e�v = =  "
					  "\n\n   ��,�o�˧A���y�O�N�W��ù�I"
					  "\n\n       �ܱo��ŤF�O�I"
				);
			 	break;
			 	
			   case CHAR_IMAGETYPE_WOMAN:
				sprintf(token,"             = = ���e�v = =  "
				  "\n\n   ��,�o�˧A���y�O�N�W��ù�I"
				  "\n\n    �i�u�O�ܱo�U�ӷU���F�O�I"
				);
			 	break;
			 
			 }
		}

		buttontype = WINDOW_BUTTONTYPE_OK;
		windowtype = WINDOW_MESSAGETYPE_MESSAGE;
	  	windowno = CHAR_WINDOWTYPE_CHARM_END; 
		break;
	}
	
	makeEscapeString( token, escapedname, sizeof( escapedname));
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
void NPC_CharmWindowTalked( int meindex, int talkerindex, 
								int seqno, int select, char *data)
{
	if( NPC_Util_CharDistance( talkerindex, meindex ) > 2) return;

	switch( seqno){
	  case CHAR_WINDOWTYPE_CHARM_START:
	  	if(atoi( data) == 2) {
			NPC_Charm_selectWindow( meindex, talkerindex, 1 );
		}
		break;

	  case CHAR_WINDOWTYPE_CHARM_END:
	  	if(select == WINDOW_BUTTONTYPE_YES) {
			NPC_Charm_selectWindow( meindex, talkerindex, 2 );
		}
		break;
	}
	
}



/*--        --*/
void NPC_CharmUp(int meindex,int talker)
{
	int cost;
	int i;
	int petindex;
	char petsend[64];	

	/*--����򸺤餷�ޤ��礦--*/
	cost = NPC_CharmCost( meindex, talker);
	CHAR_setInt( talker, CHAR_GOLD,
			CHAR_getInt( talker, CHAR_GOLD) - cost);
	CHAR_send_P_StatusString( talker, CHAR_P_STRING_GOLD);

	/*--    ���������ʾ�ˤʤ���϶����ˣ������ˤ���--*/
	if(CHAR_getInt( talker, CHAR_CHARM) + CHARMHEAL >= 100) {
		CHAR_setInt( talker, CHAR_CHARM, 100);
	}else{
		/*--    �򥻥å�--*/
		CHAR_setInt(talker, CHAR_CHARM,
	 			(CHAR_getInt( talker, CHAR_CHARM) + CHARMHEAL));
	}
	
	/*--��  �������ι���--*/
	CHAR_complianceParameter( talker );
	CHAR_send_P_StatusString( talker, CHAR_P_STRING_CHARM);


	/*--�ڥåȤΥѥ�᡼���򹹿�--*/
	for( i = 0 ; i < CHAR_MAXPETHAVE ; i++){
    	petindex = CHAR_getCharPet( talker, i);

		if( petindex == -1  )  continue;

	   /*  ������    �����å�    */
		if( !CHAR_CHECKINDEX( talker ) )  continue;

		/*--�ѥ�᡼��Ĵ��--*/
		CHAR_complianceParameter( petindex );
		sprintf( petsend, "K%d", i );
		CHAR_sendStatusString( talker , petsend );
	}
}


/*--����η׻�--*/
int NPC_CharmCost(int meindex,int talker)
{
	int cost;
	int level;
	int charm;
	int trans;

	level = CHAR_getInt( talker, CHAR_LV);
	charm = CHAR_getInt( talker, CHAR_CHARM);
	trans = CHAR_getInt( talker, CHAR_TRANSMIGRATION);

	if(charm >= 100) return -1;
	
	if(charm <= 1) charm = WARU;
	
	/*-- �׻��� --*/
	cost = level * RATE * (charm / WARU) * (trans+1);

	return cost;

}
