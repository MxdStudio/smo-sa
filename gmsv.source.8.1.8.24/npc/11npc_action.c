#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "npc_action.h"

/* 
 * �ץ쥤�䡼�Υ���������  ������NPC�e
 * ñ�������֤����ꤹ����������e
 * �����f�ޤ���  �äƤ���Υ��������ˤ�  �����ʤ��e
 *
 * �����u
 *      msgcol:		��å������ο��e�ǥե���Ȥϲ���
 *      normal:		  �̤����äƤ�������  ���ʥ����������Ф��Ƥ�����
 *		attack:		  �⥢���������Ф�������
 *		damage:		���᡼��������������������Ф�������
 *		down:		�ݤ�륢���������Ф�������
 *		sit:		�¤륢���������Ф��Ƥ�����
 *		hand:		��򿶤륢���������Ф��Ƥ�����
 *		pleasure:	��֥����������Ф��Ƥ�����
 *		angry:		�ܤ륢���������Ф��Ƥ�����
 *		sad:		  ���ॢ���������Ф��Ƥ�����
 *		guard:		�����ɤ��륢���������Ф��Ƥ�����
 */
 

#define		NPC_ACTION_MSGCOLOR_DEFAULT		CHAR_COLORYELLOW

enum {
	CHAR_WORK_MSGCOLOR	= CHAR_NPCWORKINT1,
};

/*********************************
* �����  
*********************************/
BOOL NPC_ActionInit( int meindex )
{
	char	argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	int		tmp;
	
	tmp = NPC_Util_GetNumFromStrWithDelim( argstr, "msgcol");
	if( tmp == -1 ) tmp = NPC_ACTION_MSGCOLOR_DEFAULT;
	CHAR_setWorkInt( meindex, CHAR_WORK_MSGCOLOR, tmp);
    
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPEACTION );
	
	
    return TRUE;
}




/*********************************
*   ��������줿���ν�  
*********************************/
void NPC_ActionTalked( int meindex , int talkerindex , char *szMes ,
                     int color )
{
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char	buf[64];

    /* �ץ쥤�䡼���Ф��Ƥ���  ������ */
    if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }
	/* ������åɰ�  �Τ� */
	if( !NPC_Util_charIsInFrontOfChar( talkerindex, meindex, 1 )) return; 

	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));

	if( NPC_Util_GetStrFromStrWithDelim( argstr, 
										"normal",
										buf, sizeof( buf))
    	!= NULL )
	{
    	CHAR_talkToCli( talkerindex, meindex ,buf , 
    					CHAR_getWorkInt( meindex, CHAR_WORK_MSGCOLOR ));
	}
	
}
/*********************************
* watch��  
*********************************/
void NPC_ActionWatch( int meobjindex, int objindex, CHAR_ACTION act,
                    int x,int y,int dir, int* opt,int optlen )
{
	int		meindex;
	int		index;
	char	argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char	buf[64];
	int		i;
    struct  {
    	CHAR_ACTION		act;
    	char			*string;
    }searchtbl[] = {
	    { CHAR_ACTATTACK,	"attack"},
	    { CHAR_ACTDAMAGE,	"damage"},
		{ CHAR_ACTDOWN,		"down"},
		{ CHAR_ACTSIT,		"sit"},
		{ CHAR_ACTHAND,		"hand"},
		{ CHAR_ACTPLEASURE,	"pleasure"},
		{ CHAR_ACTANGRY,	"angry"},
		{ CHAR_ACTSAD,		"sad"},
		{ CHAR_ACTGUARD,	"guard"},
		{ CHAR_ACTNOD,		"nod"},
		{ CHAR_ACTTHROW,	"throw"},
    };
	
	if( OBJECT_getType( objindex) != OBJTYPE_CHARA) return;
	index = OBJECT_getIndex( objindex);
	/* �ץ쥤�䡼�ˤΤ�  ������ */
	if( CHAR_getInt( index, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER) return;
    
    meindex = OBJECT_getIndex( meobjindex);
    
    /* ������äƣ�����åɤǤʤ���  �����ʤ� */
    if( NPC_Util_isFaceToFace( meindex, index, 1 ) != TRUE ) return;
	
	argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof( argstr1));
	
	for( i = 0; i < arraysizeof( searchtbl); i ++ ) {
		if( searchtbl[i].act == act) {
			if( NPC_Util_GetStrFromStrWithDelim( argstr, 
												searchtbl[i].string,
												buf, sizeof( buf))
		    	!= NULL )
			{
		    	CHAR_talkToCli( index, meindex ,buf , 
		    					CHAR_getWorkInt( meindex, CHAR_WORK_MSGCOLOR ));
				break;
			}
		}
	}
}
