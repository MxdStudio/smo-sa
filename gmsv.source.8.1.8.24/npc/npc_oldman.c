#include "version.h"
#include "object.h"
#include "char_base.h"
#include "char.h"
#include "util.h"
#include "npcutil.h"
#include "npc_oldman.h"
#include "title.h"
#include "char_data.h"
#include "readmap.h"

/* hoge */

BOOL NPC_OldmanInit( int meindex )
{

#if 0
    int     oldmanid=0, bornfl=0, bornx=0 ,borny=0;
    char*   npcarg;
    char    token[32]="";
/*    print( "���Ѫ��޼�: '%s'\n" , CHAR_getChar(
                         meindex, CHAR_NPCARGUMENT ));*/
    npcarg = CHAR_getChar(meindex,CHAR_NPCARGUMENT);

    getStringFromIndexWithDelim( npcarg,"|",1,token,sizeof(token));
    oldmanid = atoi( token );
    getStringFromIndexWithDelim( npcarg,"|",2,token,sizeof(token));
    bornfl = atoi( token );
    getStringFromIndexWithDelim( npcarg,"|",3,token,sizeof(token));
    bornx = atoi( token );
    getStringFromIndexWithDelim( npcarg,"|",4,token,sizeof(token));
    borny = atoi( token );

    /* Ĺ  ��ID��Ф��ƶ����ʥ� */
    CHAR_setWorkInt( meindex , CHAR_WORKOLDMANID ,oldmanid );
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPEHEALER );
    CHAR_setFlg( meindex , CHAR_ISATTACKED , 0 );
    CHAR_setFlg( meindex , CHAR_ISOVERED , 0 );

    if( MAP_IsValidCoordinate( bornfl,bornx,borny )== TRUE
        /*  �դҥ�ID���������ɤ�����Ĵ�٤�  */
        &&
        CHAR_ElderSetPosition( oldmanid ,bornfl,bornx, borny ) 
        == TRUE ){
        /* Ĺ  �ΰ�  ����������ʤ� */
        return TRUE;
    }else{
        printf( "Invalid elder npcarg=%s\n", npcarg );
        
        /* ���ޤ�˽��פʥ��顼�ʤΤǡbneverMake���ʤ��e
             �󥨥顼��å������Ф����� */
        return FALSE;
    }
#endif

	return FALSE;

}


/*
 *
 *  Ĺ  �e
 *
 *  ����ʻŻ���  ��������줿�Ȥ��˾ι��  ��򤷤���
 * ���뤳�Ȥ��e
 * * by ringo
 */

void NPC_OldmanTalked( int meindex , int talkerindex , char *msg ,
                     int color )

{
    char message[1024];
    char *n = CHAR_getChar( talkerindex , CHAR_NAME );
    int title_change = 0;

    /* �Ϥʤ��������ۤˡb��ʬ��ID�򵭲������� */
    CHAR_setInt( talkerindex, CHAR_LASTTALKELDER ,
                 CHAR_getWorkInt( meindex, CHAR_WORKOLDMANID ) );

    /*

      if( title_change == 0 && strcmp( n , "ī�G" ) == 0 ){
        
        if( NPC_Util_HaveTitle( talkerindex , 3 ) == 0 ){
            char *ts;
            NPC_Util_AddOneTitle( talkerindex , 3 );
            ts = TITLE_makeTitleStatusString( talkerindex , 3 );
            snprintf( message , sizeof(message),
                  "���O�Ӥ������W�r, �N�s%s�a�I",
                  ts );
            title_change = 1;
        }
    }
    */
{
	int		addcnt,delcnt;
	title_change = TITLE_TitleCheck_Nomsg( talkerindex, 0, &addcnt, &delcnt);
	
    if( !title_change){
        snprintf( message, sizeof( message ) ,
              "�Ӫ��n%s�b���A�ٸ���"
              "���O�٨S�ӶܡH" , n );
	    CHAR_talkToCli( talkerindex , meindex , message , CHAR_COLORWHITE );
    }
    else {
    	#define		NPC_OLDMAN_TITLE_UNIT1		"TSU"
    	#define		NPC_OLDMAN_TITLE_UNIT2		"KO"
    	
        snprintf( message, sizeof( message ) ,"�Ӫ��n%s",n );
    	CHAR_talkToCli( talkerindex , meindex , message , CHAR_COLORWHITE );
		if( delcnt > 0 && addcnt > 0 ) {
	        snprintf( message, sizeof( message ) ,
	    			"�A�ॢ�F%d%s �ٸ������C"
	    			"���O�o��F%d%s ���ٸ����ܡH"
	    			, delcnt,
	    			delcnt < 10 ?  NPC_OLDMAN_TITLE_UNIT1:NPC_OLDMAN_TITLE_UNIT2,
	    			addcnt,
	    			addcnt < 10 ?  NPC_OLDMAN_TITLE_UNIT1:NPC_OLDMAN_TITLE_UNIT2
	    			);
	    	CHAR_talkToCli( talkerindex , meindex , message , CHAR_COLORWHITE );
		}
		else if( delcnt > 0 ) {
	        snprintf( message, sizeof( message ) ,
	    			"�A�w�ॢ�o��%d%s�ٸ������C", delcnt,
	    			delcnt < 10 ?  NPC_OLDMAN_TITLE_UNIT1:NPC_OLDMAN_TITLE_UNIT2);
	    	CHAR_talkToCli( talkerindex , meindex , message , CHAR_COLORWHITE );
		}
		else if( addcnt > 0 ) {
	        snprintf( message, sizeof( message ) ,
	    			"���A%d%s���ٸ��a�I", addcnt, // CoolFish: d%s -> %d%s 2001/4/18
	    			addcnt < 10 ?  NPC_OLDMAN_TITLE_UNIT1:NPC_OLDMAN_TITLE_UNIT2);
	    	CHAR_talkToCli( talkerindex , meindex , message , CHAR_COLORWHITE );
		}
	    if( delcnt > 0 ) {
	        snprintf( message, sizeof( message), 
	                    "���h%d%s �ٸ��I", delcnt,
	    				delcnt < 10 ?  NPC_OLDMAN_TITLE_UNIT1:NPC_OLDMAN_TITLE_UNIT2);
	        CHAR_talkToCli( talkerindex, -1, message,  CHAR_COLORYELLOW);
	    }
	    if( addcnt > 0 ) {
	        snprintf( message, sizeof( message), 
	                    "��o%d%s �ٸ��I", addcnt,
	    				addcnt < 10 ?  NPC_OLDMAN_TITLE_UNIT1:NPC_OLDMAN_TITLE_UNIT2);
	        CHAR_talkToCli( talkerindex, -1, message,  CHAR_COLORYELLOW);
	    }
    }
}
    if( title_change ) CHAR_sendStatusString( talkerindex , "T" );
    
}
