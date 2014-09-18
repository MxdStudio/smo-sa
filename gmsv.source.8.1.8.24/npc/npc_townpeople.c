#include "version.h"
#include "object.h"
#include "char_base.h"
#include "char.h"
#include "util.h"
#include "handletime.h"
#include "anim_tbl.h"
#include "npc_door.h"
#include "lssproto_serv.h"
#include "npcutil.h"


/*
 *  �ޤ��ΤҤȤӤ�  by nakamura
 *  ������  �uTownPeople
 *    ��������줿�Ȥ���NPCARGUMENT��  ��  ��  �������Ƥ����ͤ�  ���e
 *
 *  �ޤ��bnpcarg�� ����ˤ���,��  �ϤƤ󤭤������Ǥ���
 *  �Τ褦��  �ѥ���ޤ�ǥ�ߥ��Ȥ���  ���Υ᡼��������
 *  �񤯤��Ȥ��Ǥ��b���ξ�������Ǥ���  ���餷��٤�e
 *  npcgen.perl �Ǥϡb MAN�Ǥ���e MSG�ȤۤȤ�ɶ��ʤ����ʤ�
 *
 */

/*
 *   ��������줿�Ȥ���NPCARGUMENT�򤽤Τޤ�  ���e
 */
void NPC_TownPeopleTalked( int index, int talker, char *msg, int color )
{
	char arg1[NPC_UTIL_GETARGSTR_BUFSIZE], token[NPC_UTIL_GETARGSTR_LINEMAX];
	char *arg;
    int i, tokennum;

    /* 3����åɰ�  �ξ������������� */

	if( CHAR_getInt(talker,CHAR_WHICHTYPE) == CHAR_TYPEPLAYER 
        && NPC_Util_charIsInFrontOfChar( talker, index, 3 ) ){

        arg = NPC_Util_GetArgStr( index, arg1, sizeof( arg1));

        tokennum = 1;
        /* ����ޤǶ��ڤ�줿�ȡ����󤬲������뤫������ */
        for( i=0;arg[i]!='\0';i++ ){
            if( arg[i] == ',' ) tokennum++;
        }

        /* ������Ǥɤ�����뤫���ơb���Υȡ����������� */
        getStringFromIndexWithDelim( arg,",",
                                     rand()%tokennum+1,token, sizeof(token));

        CHAR_talkToCli( talker, index, token, CHAR_COLORWHITE );
    }
}

/*
 * ���������e
 */
BOOL NPC_TownPeopleInit( int meindex )
{

    //CHAR_setInt( meindex , CHAR_HP , 0 );
    //CHAR_setInt( meindex , CHAR_MP , 0 );
    //CHAR_setInt( meindex , CHAR_MAXMP , 0 );
    //CHAR_setInt( meindex , CHAR_STR , 0 );
    //CHAR_setInt( meindex , CHAR_TOUGH, 0 );
    //CHAR_setInt( meindex , CHAR_LV , 0 );

    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPETOWNPEOPLE );
    //CHAR_setFlg( meindex , CHAR_ISOVERED , 1 );
    //CHAR_setFlg( meindex , CHAR_ISATTACKED , 0 );  /*   �⤵��ʤ���� */
    
    return TRUE;
}
