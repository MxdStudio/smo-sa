#include "version.h"
#include <stdio.h>

#include "configfile.h"
#include "buf.h"
#include "char_base.h"
#include "char.h"
#include "battle.h"
#include "handletime.h"
#include "map_deal.h"
#include "addressbook.h"
#include "lssproto_serv.h"
#include "pet.h"
#include "petmail.h"
#include "npcutil.h"
#include "log.h"

#define		PETMAIL_BATTLETIMEOUT		660
#define		PETMAIL_IDLETIMEOUT		30
#define		PETMAIL_IDLETIME		10

#define		PETMAIL_IDLE_RETURNOWNER	(60*30)
//andy_edit
//#define	PETMAIL_IDLEDISCARD	(60*60)  //Syu �ץ��d�l���ݮɶ����@�p��
#define	PETMAIL_IDLEDISCARD	(60*3)

/* petmail ��P�t�����ݮɶ�  shan add */
#define         PETMAIL_JS_TIMEOUT              (2*60)

static int PETMAIL_getIdleTime( int index);
static void PETMAIL_sendPetmail( int index, int tocharaindex);
static void PETMAIL_IdleProc1( int index);
static void PETMAIL_IdleProc2( int index);
static void PETMAIL_IdleProc3( int index);
static void PETMAIL_IdleProc4( int index);
static void PETMAIL_IdleProc5( int index);

static void PETMAIL_ReturnWait( int index);
static void PETMAIL_returnMail( int index, int tocharaindex);
static int PETMAIL_offmsg_max;        
#define		PETMAILOFFMSGFILE			"petmail.txt"

#define PETMAIL_DEFTOTALNUM 1000
static int PetMailTotalnums = 0;

BOOL PETMAIL_sendPetMail( int cindex, int aindex, 
					int havepetindex, int haveitemindex, char* text , int color )
{
	char	textbuffer[2048];
	struct  tm tm1;
	Char 	*ch;
	ADDRESSBOOK_entry *ae;
	int		petindex;
	int		itemindex =-1;
	int tocharaindex,playernum,i;

	//�P�_ �H�� �d�� �D�� �ﹳ�W��
	if( !CHAR_CHECKINDEX( cindex) )return FALSE;
	if( haveitemindex != -1 ) {
		itemindex = CHAR_getItemIndex( cindex, haveitemindex);
		if( ITEM_CHECKINDEX( itemindex)) {
			if( ITEM_getInt( itemindex, ITEM_CANPETMAIL) == 0 ) {
				print( "err? crack?\n");
				return FALSE;
			}
		}
	}
	petindex = CHAR_getCharPet( cindex, havepetindex);
	if( petindex == -1  ) return FALSE;
	ch  = CHAR_getCharPointer( petindex);
	if( ch == NULL ) return FALSE;
	ae = CHAR_getAddressbookEntry( cindex , aindex );
	if( ae == NULL )return FALSE;

#ifdef _AVID_TRADETRYBUG  //�d���l��
	if( CHAR_getWorkInt( cindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE ){
		CHAR_talkToCli( cindex, -1, "������A���L�k�ǻ��d���l��C", CHAR_COLORYELLOW );
		return FALSE;
	}
#endif
	//�M��
    if (CHAR_getInt( cindex, CHAR_RIDEPET) == havepetindex ){
    	CHAR_talkToCli(cindex, -1, "�M�������d���L�k�ǻ��d���l��I", CHAR_COLORYELLOW);
    	return	FALSE;
    }
	if( CHAR_getWorkInt( cindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE){
		CHAR_talkToCli( cindex, -1, "�԰����A���L�k�ǻ��d���l��C", CHAR_COLORYELLOW );
		return	FALSE;
	}
    if (CHAR_getInt(petindex, CHAR_PETFAMILY) == 1){
	   	CHAR_talkToCli(cindex, -1, "�a�ڦu�@�~�L�k�ǻ��l���I", CHAR_COLORYELLOW);
	   	return	FALSE;
	}
// Terry add fix can send mail to offline character 2004/2/5
	if(ae->online == 0){
		CHAR_talkToCli(cindex, -1, "�Ӫ��a���b�u�W�I", CHAR_COLORYELLOW);
   	return	FALSE;
	}
	playernum = CHAR_getPlayerMaxNum();
	for(i = 0;i<playernum;i++){
		if(CHAR_CHECKINDEX(i) &&
			strcmp(CHAR_getChar(i,CHAR_CDKEY),ae->cdkey) == 0 &&
			strcmp(CHAR_getChar(i,CHAR_NAME),ae->charname) == 0 ) break;
	}
	if(i == playernum){
		CHAR_talkToCli(cindex, -1, "�Ӫ��a���b���P�y�W�I", CHAR_COLORYELLOW);
   	return	FALSE;
	}
// end
#ifdef _MAP_TIME
	if((CHAR_getInt(cindex,CHAR_FLOOR) >= 30017 && CHAR_getInt(cindex,CHAR_FLOOR) <= 30021)){
		char	msgbuf[512];
		snprintf(msgbuf,sizeof(msgbuf),"���a�Ϥ��o�H�e�D��I");
		CHAR_talkToCli(cindex,-1,msgbuf,CHAR_COLORWHITE);
		return FALSE;
	}
	if((CHAR_getInt(i,CHAR_FLOOR) >= 30017 && CHAR_getInt(i,CHAR_FLOOR) <= 30021)){
		char	msgbuf[512];
		snprintf(msgbuf,sizeof(msgbuf),"���Ҧb�a�Ϥ��o���H�e�D��I");
		CHAR_talkToCli(cindex,-1,msgbuf,CHAR_COLORWHITE);
		return FALSE;
	}
#endif

#ifdef _BAD_PLAYER             // WON ADD �e�a���a�h��
	if( CHAR_getInt(cindex,CHAR_FLOOR)==887 ){
		char	msgbuf[512];
		snprintf( msgbuf, sizeof( msgbuf), "�藍�_�A�z�b�۫ݫǤ��A�L�k�H�e�H��C");
		CHAR_talkToCli( cindex, -1, msgbuf, CHAR_COLORWHITE);
		return FALSE;
	}
#endif

#ifdef _DEATH_FAMILY_GM_COMMAND	// WON ADD �a�ھ�GM���O
	{
		char	msgbuf[512];
		snprintf( msgbuf, sizeof( msgbuf), "��P�t�a��PK�A�L�k�H�e�H��C");
		CHAR_talkToCli( cindex, -1, msgbuf, CHAR_COLORWHITE);
		return FALSE;
	}
#endif

	// Syu Add 2003/05/30
	if( ( CHAR_getInt(cindex,CHAR_FLOOR) >= 8200 && CHAR_getInt(cindex,CHAR_FLOOR) <= 8213 ) ) 
	{
		char	msgbuf[512];
		snprintf( msgbuf, sizeof( msgbuf), "�^���Գ����o�H�e�D��I");
		CHAR_talkToCli( cindex, -1, msgbuf, CHAR_COLORWHITE);
		return FALSE;
	}

#ifdef _PET_LIMITLEVEL
		if( CHAR_getInt( petindex, CHAR_LIMITLEVEL) > 0 )	{
			CHAR_talkToCli(cindex, -1, "�S���d������ǻ��l���I", CHAR_COLORYELLOW);
			return FALSE;
		}
#endif

#ifdef _PETMAIL_DEFNUMS
		if( CHAR_getWorkInt( cindex, CHAR_PETMAILNUMS) > 6 ){
			CHAR_talkToCli(cindex, -1, "�l���d���L�h�A�ȮɵL�k�ǻ��d���l��I", CHAR_COLORYELLOW);
			return	FALSE;
		}else if( PETMAIL_CheckIsMyOffmsg( cindex, ae->cdkey, ae->charname) > 5 ){
			CHAR_talkToCli(cindex, -1, "����̥ثe�l���d���W�L5�ʡA�ȮɵL�k�ǻ��d���l��I", CHAR_COLORYELLOW);
			return	FALSE;
		}else if( CHAR_getWorkInt( cindex, CHAR_PETMAILSENDTIME) > (int)time( NULL)){
			char Mess1[256];
			sprintf( Mess1,"�ݵ���%d��~��A���H�e�d���l��I",
				(int)time( NULL) - CHAR_getWorkInt( cindex, CHAR_PETMAILSENDTIME));
			CHAR_talkToCli( cindex, -1, Mess1, CHAR_COLORYELLOW);
			return	FALSE;
		}else if( PetMailTotalnums >= PETMAIL_DEFTOTALNUM ){
			CHAR_talkToCli(cindex, -1, "�ثe�t�ζl��L�h�A�еy��A�H�C", CHAR_COLORYELLOW);
			return	FALSE;
		}else{
			int nums = CHAR_getWorkInt( cindex, CHAR_PETMAILNUMS);
			nums ++;
			CHAR_setWorkInt( cindex, CHAR_PETMAILNUMS, nums);
			CHAR_setWorkInt( cindex, CHAR_PETMAILSENDTIME, (int)time( NULL)+6);
			PetMailTotalnums ++;
		}
#endif
	memcpy( &tm1, localtime( (time_t *)&NowTime.tv_sec), sizeof( tm1));
    snprintf( textbuffer, sizeof( textbuffer), 
    		"%2d/%02d %2d:%02d|%s", tm1.tm_mon +1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min, text);
	{
		int		offmsgindex;
		offmsgindex = PETMAIL_addOffmsg( cindex, ae->cdkey, ae->charname, text, color);
		if( offmsgindex == -1 ) {
			print( "offmsg buffer over\n");
			return FALSE;
		}

#ifdef _FIX_PETMAIL2        // WON ADD �ץ��d�l2
		if( !PET_dropPetFLXY( cindex, havepetindex, PETMAIL_SPOOLFLOOR, PETMAIL_SPOOLX,PETMAIL_SPOOLY) ){
			CHAR_talkToCli( cindex, -1, "�d���l�󥢱�", CHAR_COLORYELLOW);
			return FALSE;
		}
#endif
		CHAR_setInt( petindex, CHAR_FLOOR, CHAR_getInt( cindex, CHAR_FLOOR));
		CHAR_setInt( petindex, CHAR_X, CHAR_getInt( cindex, CHAR_X));
		CHAR_setInt( petindex, CHAR_Y, CHAR_getInt( cindex, CHAR_Y));			
		CHAR_sendPMEToArroundCharacter( cindex, petindex, 0, CHAR_getInt( petindex, CHAR_PETMAILEFFECT));
		CHAR_setInt( petindex, CHAR_MAILMODE,CHAR_PETMAIL_IDLE2);
		CHAR_setInt( petindex, CHAR_LOOPINTERVAL, PETMAIL_LOOPINTERVAL2);
		strcpysafe( ch->charfunctable[CHAR_LOOPFUNC].string,
					sizeof( ch->charfunctable[CHAR_LOOPFUNC]), "PETMAIL_Loop");
		CHAR_constructFunctable( petindex);
		CHAR_setInt( petindex, CHAR_PETMAILBUFINDEX, offmsgindex);
		CHAR_setInt( petindex, CHAR_PETMAILIDLETIME, NowTime.tv_sec);
		CHAR_setInt( petindex, CHAR_PETMAILFROMFLOOR, CHAR_getInt( cindex, CHAR_FLOOR));
		CHAR_setInt( petindex, CHAR_PETMAILFROMX, CHAR_getInt( cindex, CHAR_X));
		CHAR_setInt( petindex, CHAR_PETMAILFROMY,CHAR_getInt( cindex, CHAR_Y));		

#ifdef _WON_PET_MAIL_LOG   // WON ADD �W�[�H�d�l�� LOG
		LogPet(
			CHAR_getChar( cindex, CHAR_NAME ), 
			CHAR_getChar( cindex, CHAR_CDKEY ),
			CHAR_getChar( petindex, CHAR_NAME),
			CHAR_getInt( petindex, CHAR_LV),
			"Pet_Send_Mail(�H�d�l)",
			CHAR_getInt( cindex,CHAR_FLOOR),
			CHAR_getInt( cindex,CHAR_X ),
			CHAR_getInt( cindex,CHAR_Y ),
			CHAR_getChar( petindex, CHAR_UNIQUECODE)  
		);
#endif
		if( haveitemindex != -1 ) {
			CHAR_setItemIndex( cindex, haveitemindex, -1);
			CHAR_setItemIndex( petindex, CHAR_STARTITEMARRAY, itemindex);
			CHAR_sendItemDataOne( cindex, haveitemindex);
			LogItem(
				CHAR_getChar( cindex, CHAR_NAME ),
				CHAR_getChar( cindex, CHAR_CDKEY ),
				itemindex,
				"pm_have(�d�l->�H�e���D��)",
				CHAR_getInt( cindex,CHAR_FLOOR),
				CHAR_getInt( cindex,CHAR_X ),
	 			CHAR_getInt( cindex,CHAR_Y ),
				ITEM_getChar( itemindex, ITEM_UNIQUECODE),
						ITEM_getChar( itemindex, ITEM_NAME),
						ITEM_getInt( itemindex, ITEM_ID)

			);
		}
		{
			char token[256];
			tocharaindex = PETMAIL_CheckPlayerExist( petindex, 0);
			sprintf( token, "�H�e�d���l��(%s)��%s�C",
				CHAR_getUseName( petindex), CHAR_getUseName( tocharaindex) );
			CHAR_talkToCli( cindex, -1, token, CHAR_COLORYELLOW);
		}
		return TRUE;
	}
	return FALSE;
}

static PETMAIL_offmsg * PETMAIL_offmsgbuf;
BOOL PETMAIL_initOffmsgBuffer( int count )
{
    int size = sizeof( PETMAIL_offmsg )*count ;
	FILE	*fp;
    char	filename[256];
    char    line[2048];
    int		linenum = 0;
    
    PETMAIL_offmsgbuf = (PETMAIL_offmsg*)allocateMemory( size );
    
    if( PETMAIL_offmsgbuf == NULL ) return FALSE;

	PetMailTotalnums = 0;

    memset( PETMAIL_offmsgbuf, 0,size );

    PETMAIL_offmsg_max = count;
    snprintf( filename ,sizeof( filename ) ,"%s/%s", getStoredir(), PETMAILOFFMSGFILE);
    fp = fopen( filename , "rt" );
    if( fp == NULL ) return TRUE;

	while(  fgets( line, sizeof( line ), fp ) != NULL ){
		char	buf[1024];
		int		index;
		if( !getStringFromIndexWithDelim( line, "|", 1, buf, sizeof( buf))) {
			print( "read error line[%d]\n", linenum);
			continue;
		}
		index = atoi( buf);
		if( index >= PETMAIL_offmsg_max ) {
			print( "index over offmsg_max [%d]\n", index);
			continue;
		}
		if( PETMAIL_offmsgbuf[index].use == TRUE) {
			print( "offmsg already use index[%d]\n", index);
			continue;
		}
		
		if( !getStringFromIndexWithDelim( line, "|", 2, buf, sizeof( buf))) {
			print( "read error line[%d]\n", linenum);
			continue;
		}
		PETMAIL_offmsgbuf[index].send_tm = atoi( buf);
		
		if( !getStringFromIndexWithDelim( line, "|", 3, buf, sizeof( buf))) {
			print( "read error line[%d]\n", linenum);
			continue;
		}
		PETMAIL_offmsgbuf[index].color = atoi( buf);
		
		if( !getStringFromIndexWithDelim( line, "|", 4, buf, sizeof( buf))) {
			print( "read error line[%d]\n", linenum);
			continue;
		}
		strcpysafe( PETMAIL_offmsgbuf[index].text, 
					sizeof( PETMAIL_offmsgbuf[index].text),
					buf);
		
		if( !getStringFromIndexWithDelim( line, "|", 5, buf, sizeof( buf))) {
			print( "read error line[%d]\n", linenum);
			continue;
		}
		strcpysafe( PETMAIL_offmsgbuf[index].destcd, 
					sizeof( PETMAIL_offmsgbuf[index].destcd),
					buf);
		
		if( !getStringFromIndexWithDelim( line, "|", 6, buf, sizeof( buf))) {
			print( "read error line[%d]\n", linenum);
			continue;
		}
		makeStringFromEscaped(buf);
		strcpysafe( PETMAIL_offmsgbuf[index].destcharname, 
					sizeof( PETMAIL_offmsgbuf[index].destcharname),
					buf);
		if( !getStringFromIndexWithDelim( line, "|", 7, buf, sizeof( buf))) {
			print( "read error line[%d]\n", linenum);
			continue;
		}
		strcpysafe( PETMAIL_offmsgbuf[index].srccd, 
					sizeof( PETMAIL_offmsgbuf[index].srccd),
					buf);
		if( !getStringFromIndexWithDelim( line, "|", 8, buf, sizeof( buf))) {
			print( "read error line[%d]\n", linenum);
			continue;
		}
		makeStringFromEscaped(buf);
		strcpysafe( PETMAIL_offmsgbuf[index].srccharname, 
					sizeof( PETMAIL_offmsgbuf[index].srccharname),
					buf);
		linenum ++;
	}
	fclose( fp);
    return TRUE;
    
}

BOOL PETMAIL_addOffmsg( int fromindex, char *tocdkey, char *tocharaname,
                            char *text , int color )
{

    int i;

    for( i = 0 ; i < PETMAIL_offmsg_max; i++ ) {
        if( PETMAIL_offmsgbuf[i].use == 0 ) {
            PETMAIL_offmsg *om = & PETMAIL_offmsgbuf[i];
            om->use = 1;
            time( & om->send_tm );

            strcpysafe( om->srccd , sizeof( om->srccd ), 
            			CHAR_getChar( fromindex, CHAR_CDKEY));
            strcpysafe( om->srccharname , sizeof( om->srccharname ),
                        CHAR_getChar( fromindex, CHAR_NAME));
            strcpysafe( om->destcd , sizeof( om->destcd ), tocdkey );
            strcpysafe( om->destcharname , sizeof(om->destcharname), tocharaname);
            strcpysafe( om->text , sizeof(om->text), text );
            om->color = color;
            return i;
        }
    }
    return -1;
}

PETMAIL_offmsg *PETMAIL_getOffmsg( int offmsgindex)
{
	if( offmsgindex < 0 || offmsgindex >= PETMAIL_offmsg_max ) return NULL;
	return &PETMAIL_offmsgbuf[offmsgindex];
}

BOOL PETMAIL_deleteOffmsg( int offmsgindex)
{
	if( offmsgindex < 0 || offmsgindex >= PETMAIL_offmsg_max ) return FALSE;
	PETMAIL_offmsgbuf[offmsgindex].use = FALSE;
	return TRUE;
}

void PETMAIL_proc( void )
{
    int i;
	static time_t PETMAIL_check_time_store = 0;
    time_t t;
    time( &t );
    if( t < PETMAIL_check_time_store) return;
    PETMAIL_check_time_store = t + PETMAIL_CHECK_OFFMSG_EXPIRE_INTERVAL;
    for(i = 0; i < PETMAIL_offmsg_max ; i++){
        if( PETMAIL_offmsgbuf[i].use && PETMAIL_offmsgbuf[i].send_tm < ( t - PETMAIL_OFFMSG_TIMEOUT )){
            PETMAIL_offmsgbuf[i].use = FALSE;
        }
    }
}

BOOL storePetmail( void)
{
	FILE	*fp;
    char	filename[256];
	char	escapebuf1[ 64],escapebuf2[ 64];
	int		i;

    /*  �ե�����  �����  */
    snprintf( filename ,sizeof( filename ) ,"%s/%s" ,
    				getStoredir(), PETMAILOFFMSGFILE);
    fp = fopen( filename , "wt" );
    if( fp == NULL ) return FALSE;
    for(i = 0; i < PETMAIL_offmsg_max ; i++){
        if( PETMAIL_offmsgbuf[i].use ){
        	fprintf( fp, "%d|%ud|%d|%s|%s|%s|%s|%s|",
        				i, (unsigned int)PETMAIL_offmsgbuf[i].send_tm,
        				PETMAIL_offmsgbuf[i].color, 
        				PETMAIL_offmsgbuf[i].text,
        				PETMAIL_offmsgbuf[i].destcd,
        				makeEscapeString( PETMAIL_offmsgbuf[i].destcharname,
        									escapebuf1, sizeof( escapebuf1)),
        				PETMAIL_offmsgbuf[i].srccd,
        				makeEscapeString( PETMAIL_offmsgbuf[i].srccharname,
        									escapebuf2, sizeof( escapebuf2)));
        }
	}
	fclose( fp);
	return TRUE;
}

BOOL PETMAIL_CheckPlayerExist( int index, int mode)
{
	int		i;
	char	*searchcd = NULL;
	char	*searchname = NULL;
	PETMAIL_offmsg *adof;
	int     playernum = CHAR_getPlayerMaxNum();
	
	if( mode == 0 ) {
		adof = PETMAIL_getOffmsg( CHAR_getInt( index, CHAR_PETMAILBUFINDEX));
		if( adof == NULL ) return FALSE;
		searchcd = adof->destcd;
		searchname = adof->destcharname;
	}
	else if( mode == 1 ) {
		searchcd = CHAR_getChar( index, CHAR_OWNERCDKEY);
		searchname = CHAR_getChar( index, CHAR_OWNERCHARANAME);
		if( searchcd == NULL || searchname == NULL ) return FALSE;
	}
	for( i = 0 ; i < playernum ; i ++) {
		if( CHAR_CHECKINDEX( i) &&
			strcmp( CHAR_getChar( i, CHAR_CDKEY), searchcd ) == 0 &&
			strcmp( CHAR_getChar( i, CHAR_NAME), searchname) == 0 ) {
			return i;
		}
	}
	return -1;
}

void PETMAIL_Loopfunc( int index)
{
	int		mode = CHAR_getInt( index, CHAR_MAILMODE);
	switch( mode ){
	  case CHAR_PETMAIL_IDLE1:
		PETMAIL_IdleProc1( index);
		break;
	  case CHAR_PETMAIL_IDLE2:
		PETMAIL_IdleProc2( index);
		break;
	  case CHAR_PETMAIL_RETURNWAIT://�^�ӵ���
	  	PETMAIL_ReturnWait( index);
	  	break;
	  case CHAR_PETMAIL_IDLE3:
		PETMAIL_IdleProc3( index);
		break;
	  case CHAR_PETMAIL_IDLE4://�M��D�H
		PETMAIL_IdleProc4( index);
	  	break;
	  case CHAR_PETMAIL_IDLE5: //�W�L�ɶ��L�k���D�H
		PETMAIL_IdleProc5( index);
	  	break;
	  default:
		break;
	}
}

static int PETMAIL_getIdleTime( int index)
{
#define		PETMAIL_DIVRANGE		25		//	ʬ�䤹�묤
#define		PETMAIL_IDLELEVELRANGE	10		//	ʬ�䤹��  ��
#define		PETMAIL_IDLEUNITTIME	3		//	�ֳ�
	int		d;
	int		dex = CHAR_getWorkInt( index, CHAR_WORKQUICK) ;

	d = dex / PETMAIL_DIVRANGE;
	if( d < 0 ) d = 0;
	if( d > PETMAIL_IDLELEVELRANGE ) d = PETMAIL_IDLELEVELRANGE;
	d = PETMAIL_IDLELEVELRANGE - d ;
	
	return d * PETMAIL_IDLEUNITTIME;
	
#undef		PETMAIL_DIVRANGE
#undef		PETMAIL_IDLELEVELRANGE
#undef		PETMAIL_IDLEUNITTIME
}

static void PETMAIL_sendPetmail( int index, int tocharaindex)
{
	int index_to_my_info;
	int		itemindex,ret;
	index_to_my_info =  ADDRESSBOOK_getIndexInAddressbook( tocharaindex, 
						CHAR_getChar( index, CHAR_OWNERCDKEY),
						CHAR_getChar( index, CHAR_OWNERCHARANAME));
#ifdef _DEATH_FAMILY_GM_COMMAND	// WON ADD �a�ھ�GM���O
	{
		char	msgbuf[512];
		snprintf( msgbuf, sizeof( msgbuf), "��P�t�a��PK�A�L�k�H�e�H��C");
		CHAR_talkToCli( index, -1, msgbuf, CHAR_COLORWHITE);
		return ;
	}
#endif

	
	if( index_to_my_info <  0 ) {
		char	msgbuf[512];

		snprintf( msgbuf, sizeof( msgbuf), 
					"%s��%s �ש�ӤF�I"
					"�ѩ���S���z���W���A�ҥH�H��Q�h�^�F�C",
					CHAR_getChar( index, CHAR_OWNERCHARANAME),
					CHAR_getUseName( index));
		CHAR_talkToCli( tocharaindex, -1, msgbuf, CHAR_COLORWHITE);
	}

#ifdef _BAD_PLAYER             // WON ADD �e�a���a�h��
	else if( CHAR_getInt(tocharaindex,CHAR_FLOOR)==887 ){
		char	msgbuf[512];
		snprintf( msgbuf, sizeof( msgbuf), 					
					"�ѩ�z�b�۫ݫǤ��A�L�k�����H��A�ҥH�H��Q�h�^�F�C" );
		CHAR_talkToCli( tocharaindex, -1, msgbuf, CHAR_COLORWHITE);
	}
#endif

	//Syu Add 06/16
	else if( CHAR_getInt(tocharaindex,CHAR_FLOOR) >=8200 &&
		CHAR_getInt(tocharaindex,CHAR_FLOOR) <= 8213 ){
		char	msgbuf[512];
		snprintf( msgbuf, sizeof( msgbuf), 					
					"�^���Գ����o�H�e�D��I" );
		CHAR_talkToCli( tocharaindex, -1, msgbuf, CHAR_COLORWHITE);
	}
	else {
		struct  tm tm1;
		char	textbuffer[2048];
		char	escapebuf[128];
		int		fd;
		PETMAIL_offmsg *offmsg;
		itemindex = CHAR_getItemIndex( index, CHAR_STARTITEMARRAY);
		ret = CHAR_addItemSpecificItemIndex( tocharaindex, itemindex);
		if( ret < 0 || ret >= CHAR_MAXITEMHAVE){
			CHAR_DropItem( index, CHAR_STARTITEMARRAY);
			LogItem(
				CHAR_getChar( tocharaindex, CHAR_NAME ), 
				CHAR_getChar( tocharaindex, CHAR_CDKEY ),
				itemindex,
				"pm_putground(�d�l->�D����w���A��m�a�W)",
				CHAR_getInt( index,CHAR_FLOOR),
				CHAR_getInt( index,CHAR_X ),
	 			CHAR_getInt( index,CHAR_Y ),
				ITEM_getChar( itemindex, ITEM_UNIQUECODE),
						ITEM_getChar( itemindex, ITEM_NAME),
						ITEM_getInt( itemindex, ITEM_ID)

			);
		}
		else {
			CHAR_setItemIndex( index, CHAR_STARTITEMARRAY, -1);
			CHAR_sendItemDataOne( tocharaindex, ret);
			LogItem(
				CHAR_getChar( tocharaindex, CHAR_NAME ),
				CHAR_getChar( tocharaindex, CHAR_CDKEY ),
				itemindex,
				"pm_getitem(�d�l->���쪺�D��)",
				CHAR_getInt( index,CHAR_FLOOR),
				CHAR_getInt( index,CHAR_X ),
	 			CHAR_getInt( index,CHAR_Y ),
				ITEM_getChar( itemindex, ITEM_UNIQUECODE),
						ITEM_getChar( itemindex, ITEM_NAME),
						ITEM_getInt( itemindex, ITEM_ID)
			);
		}
		offmsg = PETMAIL_getOffmsg( 
						CHAR_getInt( index, CHAR_PETMAILBUFINDEX));
		memcpy( &tm1, localtime( (time_t *)&NowTime.tv_sec), 
									sizeof( tm1));
	    snprintf( textbuffer, sizeof( textbuffer), 
	    		"%2d/%02d %2d:%02d|%s|%d|%d|%s|%d", 
	    		tm1.tm_mon +1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min,
	    		offmsg->text, 
	    		CHAR_getInt( index, CHAR_BASEIMAGENUMBER),
	    		CHAR_getInt( index, CHAR_LV),
	    		makeEscapeString( CHAR_getUseName( index), escapebuf,
	    							sizeof( escapebuf)),
				ITEM_getInt( itemindex, ITEM_BASEIMAGENUMBER)
	    		);
		fd = getfdFromCharaIndex( tocharaindex);
		if( fd != -1 ) lssproto_MSG_send( fd , index_to_my_info , 
									textbuffer , offmsg->color );
		PETMAIL_deleteOffmsg( CHAR_getInt( index, CHAR_PETMAILBUFINDEX));
		CHAR_setInt( index, CHAR_PETMAILBUFINDEX, -1);
		CHAR_setInt( index, CHAR_PETSENDMAILCOUNT, 
					CHAR_getInt( index, CHAR_PETSENDMAILCOUNT) +1);
		{
			char   *cdkey = CHAR_getChar( index, CHAR_CDKEY);
			if( cdkey) printl( LOG_TALK, "CDKEY=%s\tTEXT=%s" , cdkey, offmsg->text );
			else 	   printl( LOG_TALK, "CDKEY=(null)\tTEXT=%s" , offmsg->text );
		}
	}
}

static void PETMAIL_IdleProc1( int index)
{

	int		tocharaindex;
	int		warp = FALSE;

	tocharaindex = PETMAIL_CheckPlayerExist( index, 0);
	if( !CHAR_CHECKINDEX( tocharaindex) ) {
		warp = TRUE;
	}else {
		if( CHAR_getInt( tocharaindex, CHAR_FLOOR) == 8215 ){//�ȪA���� andy
			warp = TRUE;
		}else if( CHAR_getWorkInt( tocharaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE ){//����������l��
			warp = TRUE;
		}else if( CHAR_getWorkInt( tocharaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE ){		
			int battleindex = CHAR_getWorkInt( tocharaindex, CHAR_WORKBATTLEINDEX );
			if( !BATTLE_CHECKINDEX( battleindex ) || BattleArray[battleindex].flgTime > 60  ){
				warp = TRUE;
			}
		}else {
			int	ret;
			int distance;
			distance = NPC_Util_CharDistance( tocharaindex, index);
			if( distance > CHAR_DEFAULTSEESIZ /2 ) {
				warp = TRUE;
			}else if( distance > 1 ){
				int dir = NPC_Util_GetDirCharToChar( index, tocharaindex, 0);
				if( dir != -1 ) {
					dir = NPC_Util_SuberiWalk( index, dir);
				}
				if( dir != -1 ) {
					ret = CHAR_walk( index, dir, 0);
					if( ret != CHAR_WALKSUCCESSED) {
						dir = -1;
					}
				}
				if( dir == -1 ) {
					warp = TRUE;
				}
			}else {
				PETMAIL_sendPetmail( index, tocharaindex);
				CHAR_setInt( index, CHAR_MAILMODE,CHAR_PETMAIL_RETURNWAIT);
				CHAR_setInt( index, CHAR_PETMAILIDLETIME, NowTime.tv_sec);
				CHAR_setInt( index, CHAR_LOOPINTERVAL, PETMAIL_LOOPINTERVAL2);
			}
		}
	}
	if( warp) {
		if( CHAR_getInt( index, CHAR_FLOOR) != PETMAIL_SPOOLFLOOR) {
			CHAR_warpToSpecificPoint( index, 
								PETMAIL_SPOOLFLOOR, 
								PETMAIL_SPOOLX,PETMAIL_SPOOLX);
			CHAR_setInt( index, CHAR_MAILMODE,CHAR_PETMAIL_IDLE2);
			CHAR_setInt( index, CHAR_PETMAILIDLETIME, NowTime.tv_sec);
			CHAR_setInt( index, CHAR_LOOPINTERVAL, PETMAIL_LOOPINTERVAL2);
		}
	}
}

static void PETMAIL_IdleProc2( int index)
{
  	unsigned int t = CHAR_getInt( index, CHAR_PETMAILIDLETIME);
	if( NowTime.tv_sec > t + PETMAIL_getIdleTime( index)) {
		int		tocharaindex;
		int		cnt;
		tocharaindex = PETMAIL_CheckPlayerExist( index, 0);
		if( tocharaindex != -1 ) {
			if( CHAR_getInt( tocharaindex, CHAR_FLOOR) == 8215 ){//�ȪA���� andy
			}else if( CHAR_getWorkInt( tocharaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE ){//����������l��
			}else if( CHAR_getWorkInt( tocharaindex, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE){
			}else {
				int		fl, x, y, ch_x, ch_y;
				fl = CHAR_getInt( tocharaindex, CHAR_FLOOR);
				ch_x = CHAR_getInt( tocharaindex, CHAR_X);
				ch_y = CHAR_getInt( tocharaindex, CHAR_Y);
				for( cnt = 0; cnt < 10; cnt ++ ) {
					x = RAND( ch_x -1, ch_x + 1);
					y = RAND( ch_y -1, ch_y + 1);
					if( MAP_walkAble( tocharaindex, fl,x,y)) {
						break;
					}
				}
				if( cnt == 10 ) {
					x = ch_x;	y = ch_y;
				}
				CHAR_setInt( index, CHAR_MAILMODE,CHAR_PETMAIL_RETURNWAIT);
				CHAR_setInt( index, CHAR_PETMAILIDLETIME, NowTime.tv_sec);
				CHAR_warpToSpecificPoint( index,fl, x,y);
				PETMAIL_sendPetmail( index, tocharaindex);
				return;
			}
		}else{
			if( NowTime.tv_sec > t + PETMAIL_IDLETIMEOUT ) {
				PETMAIL_deleteOffmsg( CHAR_getInt( index, CHAR_PETMAILBUFINDEX));
				CHAR_setInt( index, CHAR_MAILMODE, CHAR_PETMAIL_IDLE3);
				CHAR_setInt( index, CHAR_PETMAILIDLETIME, NowTime.tv_sec);
				CHAR_setInt( index,	CHAR_LOOPINTERVAL, PETMAIL_LOOPINTERVAL1);
			}	
			return;
		}
		if( NowTime.tv_sec > t + PETMAIL_BATTLETIMEOUT ) {
			PETMAIL_deleteOffmsg( CHAR_getInt( index, CHAR_PETMAILBUFINDEX));
			CHAR_setInt( index, CHAR_MAILMODE, CHAR_PETMAIL_IDLE3);
			CHAR_setInt( index, CHAR_PETMAILIDLETIME, NowTime.tv_sec);
			CHAR_setInt( index,	CHAR_LOOPINTERVAL, PETMAIL_LOOPINTERVAL1);
		}					
	}
}

static void PETMAIL_ReturnWait( int index)
{
  	unsigned int t = CHAR_getInt( index, CHAR_PETMAILIDLETIME);
	if( NowTime.tv_sec > t + PETMAIL_IDLETIME) {
		CHAR_setInt( index, CHAR_MAILMODE, CHAR_PETMAIL_IDLE3);
		CHAR_setInt( index, CHAR_PETMAILIDLETIME, NowTime.tv_sec);
		CHAR_setInt( index, CHAR_LOOPINTERVAL, PETMAIL_LOOPINTERVAL1);
	}
}

static void PETMAIL_IdleProc3( int index)
{

	int		tocharaindex;
	int		warp = FALSE;

	tocharaindex = PETMAIL_CheckPlayerExist( index, 1);
	if( tocharaindex == -1 ) {
		warp = TRUE;
	}else {
		if( CHAR_getWorkInt( tocharaindex, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE) {
			int		ret ;
			int distance;
			distance = NPC_Util_CharDistance( tocharaindex, index);
			if( distance > CHAR_DEFAULTSEESIZ /2 ) {
				warp = TRUE;

			}else if( distance > 1 ){
				int dir = NPC_Util_GetDirCharToChar( index, tocharaindex, 0);
				if( dir != -1 ) {
					dir = NPC_Util_SuberiWalk( index, dir);
				}
				if( dir != -1 ) {
					ret = CHAR_walk( index, dir, 0);
					if( ret != CHAR_WALKSUCCESSED) {
						dir = -1;
					}
				}
				if( dir == -1 ) {
					warp = TRUE;
				}
			}else {
				//andy_reEdit 2003/05/30
				warp = TRUE;
/*				CHAR_setInt( index, CHAR_MAILMODE,CHAR_PETMAIL_IDLE5);
				CHAR_setInt( index, CHAR_PETMAILIDLETIME, NowTime.tv_sec);
				CHAR_setInt( index, CHAR_LOOPINTERVAL, PETMAIL_LOOPINTERVAL2);
				PETMAIL_returnMail( index, tocharaindex);
*/
			}
		}
	}
	if( warp) {
			CHAR_warpToSpecificPoint( index, 
								PETMAIL_SPOOLFLOOR, 
								PETMAIL_SPOOLX,PETMAIL_SPOOLX);
			CHAR_setInt( index, CHAR_MAILMODE,CHAR_PETMAIL_IDLE4);
			CHAR_setInt( index, CHAR_PETMAILIDLETIME, NowTime.tv_sec);
			CHAR_setInt( index, CHAR_LOOPINTERVAL, PETMAIL_LOOPINTERVAL2);
	}
}
static void PETMAIL_IdleProc4( int index)
{
  	unsigned int t = CHAR_getInt( index, CHAR_PETMAILIDLETIME);
	{
		int		tocharaindex;
		int		cnt;
		tocharaindex = PETMAIL_CheckPlayerExist( index, 1);//�M��D�H
		if( tocharaindex != -1 ) {
			if( CHAR_getWorkInt( tocharaindex, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE &&
				CHAR_getWorkInt( tocharaindex, CHAR_WORKTRADEMODE) == CHAR_TRADE_FREE ){

				int		fl, x, y, ch_x, ch_y;//�^��D�H����
				
				fl = CHAR_getInt( tocharaindex, CHAR_FLOOR);
				ch_x = CHAR_getInt( tocharaindex, CHAR_X);
				ch_y = CHAR_getInt( tocharaindex, CHAR_Y);
				for( cnt = 0; cnt < 10; cnt ++ ) {
					x = RAND( ch_x -1, ch_x + 1);
					y = RAND( ch_y -1, ch_y + 1);
					if( MAP_walkAble( index, fl,x,y)) {
						break;
					}
				}
				if( cnt == 10 ) {
					x = ch_x;	y = ch_y;
				}
				CHAR_setInt( index, CHAR_MAILMODE,CHAR_PETMAIL_IDLE5);
				CHAR_setInt( index, CHAR_PETMAILIDLETIME, NowTime.tv_sec);
				CHAR_warpToSpecificPoint( index,fl, x,y);
				PETMAIL_returnMail( index, tocharaindex);
			}
		}else {
			if( NowTime.tv_sec > t + PETMAIL_IDLE_RETURNOWNER ) {//�ˬd���ݥD�H�ɶ�
				int itemindex;
				CHAR_warpToSpecificPoint( index, //�^�����D�H�Ҧb�y��
									CHAR_getInt( index, CHAR_PETMAILFROMFLOOR),
									CHAR_getInt( index, CHAR_PETMAILFROMX),
									CHAR_getInt( index, CHAR_PETMAILFROMY));
									
				CHAR_setInt( index, CHAR_MAILMODE, CHAR_PETMAIL_IDLE5);//�����d�l���A
				CHAR_setInt( index,	CHAR_LOOPINTERVAL, PETMAIL_LOOPINTERVAL2);
				CHAR_setInt( index, CHAR_PETMAILIDLETIME, NowTime.tv_sec);
				if( CHAR_getInt( index, CHAR_PETMAILIDLETIME) != -1 ) {
					PETMAIL_deleteOffmsg( CHAR_getInt( index, CHAR_PETMAILBUFINDEX));
				}
				itemindex = CHAR_getItemIndex( index, CHAR_STARTITEMARRAY);//�R�����a�D��
				if( ITEM_CHECKINDEX( itemindex ) ) {
					CHAR_DropItem( index, CHAR_STARTITEMARRAY);

					LogItem(
						CHAR_getChar( index, CHAR_OWNERCHARANAME ),
						CHAR_getChar( index, CHAR_OWNERCDKEY ),
						itemindex,
						"pm_returntimeout(�d�l->�O�ɧR���D��)",
						CHAR_getInt( index,CHAR_FLOOR),
						CHAR_getInt( index,CHAR_X ),
			 			CHAR_getInt( index,CHAR_Y ),
						ITEM_getChar( itemindex, ITEM_UNIQUECODE),
						ITEM_getChar( itemindex, ITEM_NAME),
						ITEM_getInt( itemindex, ITEM_ID)
					);
				}
			}
		}
	}

}

static void PETMAIL_IdleProc5( int index)
{
  	unsigned int t = CHAR_getInt( index, CHAR_PETMAILIDLETIME);
	if( NowTime.tv_sec > t + PETMAIL_IDLEDISCARD) {
		Char	*ch;
		CHAR_setInt( index, CHAR_MAILMODE, CHAR_PETMAIL_NONE);
		CHAR_setInt( index, CHAR_PETMAILIDLETIME, 0);

#ifdef _FIX_PETMAIL         // WON ADD �ץ��d�l
		CHAR_setInt( index, CHAR_PUTPETTIME, NowTime.tv_sec );
#endif
#ifdef _PETMAIL_DEFNUMS
		PETMAIL_delPetMailTotalnums( 1); //M PET �ܦۥ��d
		if( PetMailTotalnums < 0 ) PetMailTotalnums = 0;
#endif
	    ch  = CHAR_getCharPointer( index);
	    if( ch != NULL ) {
			strcpysafe( ch->charfunctable[CHAR_LOOPFUNC].string,
						sizeof( ch->charfunctable[CHAR_LOOPFUNC]), "" );
			CHAR_constructFunctable( index);
		}
#ifdef _PET_LOSTPET
		CHAR_CharSaveLostPet( index, 2);
		LogPet(
			"�t��",
			"Watchfunc",
			CHAR_getChar( index, CHAR_NAME),
			CHAR_getInt( index, CHAR_LV),
			"timeout_lost(�t�Φ��d-�d�l�ۥ��d)",
			CHAR_getInt( index, CHAR_FLOOR),
			CHAR_getInt( index,CHAR_X ),
			CHAR_getInt( index,CHAR_Y ),
			CHAR_getChar( index, CHAR_UNIQUECODE)   // shan 2001/12/14
		);
		print("�t�Φ��d-�d�l�ۥ��d:%s\n", CHAR_getUseName( index));
		CHAR_CharaDelete( index);
#else
		LogPet(
			CHAR_getChar( index, CHAR_OWNERCHARANAME ), 
			CHAR_getChar( index, CHAR_OWNERCDKEY ),
			CHAR_getChar( index, CHAR_NAME),
			CHAR_getInt( index, CHAR_LV),
			"FreePet(�d�l�ܦ^�ۥ��d)",
			CHAR_getInt( index,CHAR_FLOOR),
			CHAR_getInt( index,CHAR_X ),
			CHAR_getInt( index,CHAR_Y ),
			CHAR_getChar( index, CHAR_UNIQUECODE)  
		);
#endif

	}else {
		int tocharaindex = PETMAIL_CheckPlayerExist( index, 1);
		if( tocharaindex != -1 ) {
			if( NPC_Util_CharDistance( tocharaindex, index ) > CHAR_DEFAULTSEESIZ/2) {
				int		fl, x, y, ch_x, ch_y;
				int		cnt;
				fl = CHAR_getInt( tocharaindex, CHAR_FLOOR);
				ch_x = CHAR_getInt( tocharaindex, CHAR_X);
				ch_y = CHAR_getInt( tocharaindex, CHAR_Y);
				for( cnt = 0; cnt < 10; cnt ++ ) {
					x = RAND( ch_x -1, ch_x + 1);
					y = RAND( ch_y -1, ch_y + 1);
					if( MAP_walkAble( index, fl,x,y)) {
						break;
					}
				}
				if( cnt == 10 ) {
					x = ch_x;	y = ch_y;
				}
				CHAR_warpToSpecificPoint( index,fl, x,y);
			}
		}
	}
}

static void PETMAIL_returnMail( int index, int tocharaindex)
{
	char	msgbuf[512];
	int itemindex = CHAR_getItemIndex( index, CHAR_STARTITEMARRAY);
	if( ITEM_CHECKINDEX( itemindex ) ) {
		int ret = CHAR_addItemSpecificItemIndex( tocharaindex, itemindex);
		
		if( ret < 0 || ret >= CHAR_MAXITEMHAVE){
			CHAR_DropItem( index, CHAR_STARTITEMARRAY);

			LogItem(
				CHAR_getChar( tocharaindex, CHAR_NAME ), 
				CHAR_getChar( tocharaindex, CHAR_CDKEY ),
				itemindex,
				"pm_returnputground(�d�l->�D��w���N�D���m�a�W)",
				CHAR_getInt( index,CHAR_FLOOR),
				CHAR_getInt( index,CHAR_X ),
	 			CHAR_getInt( index,CHAR_Y ),
				ITEM_getChar( itemindex, ITEM_UNIQUECODE),
						ITEM_getChar( itemindex, ITEM_NAME),
						ITEM_getInt( itemindex, ITEM_ID)
			);
		}else {
			CHAR_setItemIndex( index, CHAR_STARTITEMARRAY, -1);
			CHAR_sendItemDataOne( tocharaindex, ret);
		}
	}

#ifdef _PETMAIL_DEFNUMS
	{
		int petmailnums = CHAR_getWorkInt( tocharaindex, CHAR_PETMAILNUMS);
		petmailnums--;
		if( petmailnums < 0 ) petmailnums = 0;
		CHAR_setWorkInt( tocharaindex, CHAR_PETMAILNUMS, petmailnums);
	}
#endif
	if( CHAR_getInt( index, CHAR_PETMAILBUFINDEX) != -1 ) {
		snprintf( msgbuf, sizeof( msgbuf), 
					"%s �^�ӤF�I"
					"���G�L�k�H�e�H��C",
					CHAR_getUseName( index));
		CHAR_talkToCli( tocharaindex, -1, msgbuf, CHAR_COLORWHITE);
		PETMAIL_deleteOffmsg( CHAR_getInt( index, CHAR_PETMAILBUFINDEX));
	}else {
		snprintf( msgbuf, sizeof( msgbuf), 
					"%s �^�ӤF�I", CHAR_getUseName( index));
		CHAR_talkToCli( tocharaindex, -1, msgbuf, CHAR_COLORWHITE);		
	}

#ifdef _PETMAIL_DEFNUMS
	CHAR_AutoPickupMailPet( tocharaindex, index);
#endif

}


#ifdef _PETMAIL_DEFNUMS
void CHAR_AutoPickupMailPet( int charaindex, int petindex )
{
	int objindex;
	int havepetindex;
	char category[3];
	Char 	*ch;
	if( !CHAR_CHECKINDEX( charaindex ) ) return;
#ifdef _AVID_TRADETRYBUG //��X�d��
	if( CHAR_getWorkInt( charaindex, CHAR_WORKTRADEMODE) != CHAR_TRADE_FREE ){
		CHAR_talkToCli( charaindex, -1, "������A���L�k�۰ʬB�^�l���d���I", CHAR_COLORYELLOW );
		return;
	}
#endif
	if( !CHAR_CHECKINDEX( petindex ) ) return;
	havepetindex = CHAR_getCharPetElement( charaindex);
	if( havepetindex == -1 ) {

		CHAR_talkToCli(charaindex,-1,"�d����w���I�L�k�۰ʬB�^�l���d���I",CHAR_COLORYELLOW);
		// WON ADD
		LogPet(
			CHAR_getChar( charaindex, CHAR_NAME ), 
			CHAR_getChar( charaindex, CHAR_CDKEY ),
			CHAR_getChar( petindex, CHAR_NAME),
			CHAR_getInt( petindex, CHAR_LV),
			"Pet_Full(�d����w���A�L�k�B�_�d��)",
			CHAR_getInt( charaindex,CHAR_FLOOR),
			CHAR_getInt( charaindex,CHAR_X ),
			CHAR_getInt( charaindex,CHAR_Y ),
			CHAR_getChar( petindex, CHAR_UNIQUECODE)   
	    );		 
		return;
	}
	ch  = CHAR_getCharPointer( petindex);
	if( ch == NULL ) return;

	objindex = CHAR_getWorkInt( petindex, CHAR_WORKOBJINDEX );
	CHAR_ObjectDelete(objindex);
	CHAR_setWorkInt( petindex, CHAR_WORKPLAYERINDEX, charaindex);
	CHAR_setCharPet( charaindex, havepetindex, petindex);

	CHAR_setChar( petindex, CHAR_OWNERCDKEY, CHAR_getChar( charaindex, CHAR_CDKEY));
	CHAR_setChar( petindex, CHAR_OWNERCHARANAME, CHAR_getChar( charaindex, CHAR_NAME));

	snprintf( category,sizeof( category),"K%d",havepetindex);
	CHAR_sendStatusString( charaindex, category);
	snprintf( category,sizeof( category),"W%d",havepetindex);
	CHAR_sendStatusString( charaindex, category);

	CHAR_setInt( petindex, CHAR_PUTPETTIME, 0);
	CHAR_setWorkInt( petindex, CHAR_WORKOBJINDEX, -1);
	CHAR_setInt( petindex, CHAR_MAILMODE, CHAR_PETMAIL_NONE);
	PETMAIL_delPetMailTotalnums( 1); // M PET �۰ʾ߰_

	strcpysafe( ch->charfunctable[CHAR_LOOPFUNC].string,
				sizeof( ch->charfunctable[CHAR_LOOPFUNC]),"");
	CHAR_setInt( petindex, CHAR_LOOPINTERVAL, 0);
	CHAR_constructFunctable( petindex);
	CHAR_complianceParameter( petindex );

	if( PetMailTotalnums < 0 ) PetMailTotalnums = 0;
	{
		char    mesg[256];
		char    *p = NULL;
		p = CHAR_getChar( petindex, CHAR_USERPETNAME);
		if( strlen( p) == 0 ) {
			p = CHAR_getChar( petindex, CHAR_NAME);
		}
		snprintf( mesg,sizeof(mesg), "�B�^�l���d�� %s", p);
		CHAR_talkToCli(charaindex,-1,mesg,CHAR_COLORYELLOW);
	}
	LogPet(
		CHAR_getChar( charaindex, CHAR_NAME ), 
		CHAR_getChar( charaindex, CHAR_CDKEY ),
		CHAR_getChar( petindex, CHAR_NAME),
		CHAR_getInt( petindex, CHAR_LV),
		"PickupMailPet(�۰ʦ��^�l�d)",
		CHAR_getInt( charaindex,CHAR_FLOOR),
		CHAR_getInt( charaindex,CHAR_X ),
		CHAR_getInt( charaindex,CHAR_Y ),
		CHAR_getChar( petindex, CHAR_UNIQUECODE)   
	);
	return;
}
#endif

int PETMAIL_getPetMailTotalnums()
{
#ifdef _PETMAIL_DEFNUMS
	if( PetMailTotalnums < 0 ) PetMailTotalnums = 0;
	if( PetMailTotalnums >= PETMAIL_DEFTOTALNUM ) PetMailTotalnums = PETMAIL_DEFTOTALNUM;
	return PetMailTotalnums;
#else
	return 0;
#endif
}

void PETMAIL_delPetMailTotalnums( int numflg)
{
#ifdef _PETMAIL_DEFNUMS
	PetMailTotalnums = PetMailTotalnums - numflg;
	if( PetMailTotalnums < 0 ) PetMailTotalnums = 0;
#endif
}

void PETMAIL_setPetMailTotalnums( int numflg)
{
	PetMailTotalnums = numflg;
	if( PetMailTotalnums < 0 ) PetMailTotalnums = 0;
	if( PetMailTotalnums > PETMAIL_DEFTOTALNUM+10 ) PetMailTotalnums = PETMAIL_DEFTOTALNUM + 5;
}


int PETMAIL_CheckIsMyOffmsg( int fromindex, char *tocdkey, char *tocharaname)
{
    int i, nums=0;
    for( i = 0 ; i < PETMAIL_offmsg_max; i++ ) {
        if( PETMAIL_offmsgbuf[i].use == 0 ) continue;
		if( !strcmp( tocdkey, PETMAIL_offmsgbuf[i].destcd) &&
			!strcmp( tocharaname, PETMAIL_offmsgbuf[i].destcharname) ){
			nums++;
		}
    }
    return nums;
}



