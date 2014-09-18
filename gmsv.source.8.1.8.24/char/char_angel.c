#include "version.h"
#ifdef _ANGEL_SUMMON
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>   // shan
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include "common.h"
#include "char_base.h"
#include "char_data.h"
#include "char.h"
#include "lssproto_serv.h"
#include "item.h"
#include "item_event.h"
#include "buf.h"
#include "object.h"
#include "map_deal.h"
#include "saacproto_cli.h"
#include "readmap.h"
#include "handletime.h"
#include "char_event.h"
#include "npccreate.h"
#include "addressbook.h"
#include "item_event.h"
#include "magic_base.h"
#include "magic.h"
#include "chatmagic.h"
#include "configfile.h"
#include "log.h"
#include "anim_tbl.h"
#include "encount.h"
#include "battle.h"
#include "pet_skill.h"
#include "util.h"
#include "enemy.h"
#include "npcutil.h"
#include "pet.h"
#include "family.h"
#include "defend.h"
#include "npcserver.h"


struct MissionInfo missionlist[MAXMISSION];
int mission_num =0;
struct MissionTable missiontable[MAXMISSIONTABLE];

//#define ANGELITEM 2884 //20701	//�Ϫ̪��H�� �D��s��
//#define HEROITEM 2885 //20702	//�i�̪��H�� �D��s��

extern int AngelReady;

char* getMissionNameInfo( int charaindex, char* nameinfo)
{
	sprintf( nameinfo, "%s:%s", CHAR_getChar( charaindex, CHAR_CDKEY), CHAR_getChar( charaindex, CHAR_NAME) );
	return nameinfo;
}

int checkIfAngel( int charaindex)
{
	int i;
	char nameinfo[512];

	//sprintf( nameinfo, "%s:%s", CHAR_getChar( charaindex, CHAR_CDKEY), CHAR_getChar( charaindex, CHAR_NAME) );
	getMissionNameInfo( charaindex, nameinfo);
	for( i =0; i <MAXMISSIONTABLE; i++) {
		if( missiontable[i].angelinfo[0] == NULL)
			continue;
		if( !strcmp( nameinfo, missiontable[i].angelinfo) ) {
			return i;
		}
		else if( !strcmp( nameinfo, missiontable[i].heroinfo) ) {
			return i;
		}
	}
	return -1;
}

int checkIfOnlyAngel( int charaindex)
{
	int i;
	char nameinfo[512];

	//sprintf( nameinfo, "%s:%s", CHAR_getChar( charaindex, CHAR_CDKEY), CHAR_getChar( charaindex, CHAR_NAME) );
	getMissionNameInfo( charaindex, nameinfo);
	for( i =0; i <MAXMISSIONTABLE; i++) {
		if( missiontable[i].angelinfo[0] == NULL)
			continue;
		if( !strcmp( nameinfo, missiontable[i].angelinfo) ) {
			return i;
		}
	}
	return -1;
}

int checkIfOnlyHero( int charaindex)
{
	int i;
	char nameinfo[512];

	//sprintf( nameinfo, "%s:%s", CHAR_getChar( charaindex, CHAR_CDKEY), CHAR_getChar( charaindex, CHAR_NAME) );
	getMissionNameInfo( charaindex, nameinfo);
	for( i =0; i <MAXMISSIONTABLE; i++) {
		if( missiontable[i].angelinfo[0] == NULL)
			continue;
		if( !strcmp( nameinfo, missiontable[i].heroinfo) ) {
			return i;
		}
	}
	return -1;
}

int checkIfAngelByName( char* nameinfo)
{
	int i;

	for( i =0; i <MAXMISSIONTABLE; i++) {
		if( missiontable[i].angelinfo[0] == NULL)
			continue;
		if( !strcmp( nameinfo, missiontable[i].angelinfo) ) {
			return i;
		}
		else if( !strcmp( nameinfo, missiontable[i].heroinfo) ) {
			return i;
		}
	}
	return -1;
}


void addAngelData( int angelindex, int heroindex, int mission, int flag)
{
	int i;
	char buf[1024];
	char angelinfo[512];
	char heroinfo[512];

	//if( checkIfAngel( angelindex) != -1 || checkIfAngel( heroindex) != -1 )
	//	return;
	//sprintf( angelinfo, "%s:%s", CHAR_getChar( angelindex, CHAR_CDKEY), CHAR_getChar( angelindex, CHAR_NAME) );
	getMissionNameInfo( angelindex, angelinfo);
	//sprintf( heroinfo, "%s:%s", CHAR_getChar( heroindex, CHAR_CDKEY), CHAR_getChar( heroindex, CHAR_NAME) );
	getMissionNameInfo( heroindex, heroinfo);
	
	sprintf( buf, "%s|%s|%d|%d", angelinfo, heroinfo, mission, flag );
	//saacproto_ACMissionTable_send( acfd, -1, 2, buf, angelindex);
	saacproto_ACMissionTable_send( acfd, -1, 2, buf, angelinfo);

	return;
}

//void selectAngel( int charaindex)
void selectAngel( int charaindex, int heroindex, int mission, int gm_cmd)
{
	// gm_cmd ��ܬO�_��GM���O����, 

	//int heroindex =-1;
	int findindex, startindex;
	char msg[1024];
	int max_char;
	//int mission;
	
	if( AngelReady <= 0 && gm_cmd == FALSE)
		return;

	sprintf( msg, " �Ϫ̸���ˬd: %s %s ", CHAR_getChar( charaindex, CHAR_CDKEY), CHAR_getChar( charaindex, CHAR_NAME));
	//print( msg);
	LogAngel( msg);

	// �Ѩϱ����ˬd
	if( gm_cmd == FALSE )
	{
		if( checkIfAngel( charaindex) != -1) // �O�_�ѨϩΫi��
		{
			//print(" ANGEL�w�g�O�ѨϩΫi�̤F ");
			return;
		}
		
		if( CHAR_getInt( charaindex, CHAR_LV) < 30 || !NPC_EventCheckFlg( charaindex, 4 ) )
		{
			//print(" ANGEL��椣�� ");
			return;
		}
		
		srand( time(NULL));
		if( rand()%3 == 0 )	// �ѨϪ����v 2/3
		{
			//print(" ANGEL���v�ˬd���q�L ");
			return;
		}
	}
	//print(" ANGEL�M�w ");

	
	// �D��i��
	if( heroindex == -1 )
	{
		//heroindex = -1;
		max_char = CHAR_getPlayerMaxNum();
		startindex = RAND( 0, max_char-1);
		findindex = startindex;
		while(1) {
			if( findindex == startindex-1)	break;
			findindex = (findindex+1) % max_char;
			if( !CHAR_CHECKINDEX( findindex) ) continue;
			if( CHAR_getInt( findindex, CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) continue;
			if( findindex == charaindex ) continue;
			if( checkIfAngel( findindex) != -1)	continue; // �O�_�ѨϩΫi��
			if( CHAR_getInt( findindex, CHAR_LV) < 80 || !NPC_EventCheckFlg( findindex, 4 ) ) continue; // �i�̪�����
			if( rand()%3 == 0 )	continue; // �i�̪����v 2/3
			
			heroindex = findindex;
			
			//print(" ANGEL�i�̨M�w ");
			
			break;
		}
		if( heroindex < 0) {
			//sprintf( msg, " �S�����i�̤H��!! ");
			//print( msg);
			//LogAngel( msg);
			return;
		}
	}

	// �M�w����
	if( mission == -1 )
	{
		int mlist[MAXMISSION]; // �ŦX���󪺥��ȦC��
		char eventlist[1024];
		char buf[64];
		int i, j, mindex =0;
		
		memset( mlist, 0, sizeof(mlist));
		for( i =0; i <MAXMISSION; i++) {
			int checked =TRUE;
			if( missionlist[i].id <= 0) continue;
			if( CHAR_getInt( heroindex, CHAR_LV) < missionlist[i].level )
				continue;
			strcpy( eventlist, missionlist[i].eventflag); // �ˬd���n�X��
			j =0;
			while(1) {
				j++;
				if( getStringFromIndexWithDelim( eventlist, ";", j, buf, sizeof(buf)) == FALSE )
					break;
				if( buf[0] != '!') { // ���n�X�ЩθT��X��
					if( !NPC_EventCheckFlg( heroindex, atoi(buf) )) {
						checked = FALSE;
						break;
					}
				}
				else {
					if( NPC_EventCheckFlg( heroindex, atoi(buf+1) )) {
						checked = FALSE;
						break;
					}
				}
			}
			if( checked ) {
				mlist[mindex] = missionlist[i].id;
				print(" mlist[%d]=%d ", mindex, mlist[mindex]);
				mindex++;
			}
		}
		if( mindex <= 0 ) {
			//sprintf( msg, " �䤣��X�A������ ");
			//print( msg);
			//LogAngel( msg);
			return;
		}
		
		mission = mlist[rand()%mindex];
	}


	addAngelData( charaindex, heroindex, mission, MISSION_WAIT_ANSWER); // �Ǩ�AC

	// �M���X�� event8 224~255 �����F�l��M��
	CHAR_setInt( charaindex, CHAR_NOWEVENT8, 0);
	CHAR_setInt( charaindex, CHAR_ENDEVENT8, 0);
	CHAR_setInt( heroindex, CHAR_NOWEVENT8, 0);
	CHAR_setInt( heroindex, CHAR_ENDEVENT8, 0);

	if( gm_cmd == FALSE )
		AngelReady = 0; // �M�����B
	
	{
		sprintf( msg, " ���� %s �ѨϭԸ�: %s %s Lv:%d �i�̭Ը�: %s %s Lv:%d ����:%d ci=%d hi=%d ",
				gm_cmd ? ("(GM���O)") : (" "),
				CHAR_getChar( charaindex, CHAR_CDKEY), CHAR_getChar( charaindex, CHAR_NAME), CHAR_getInt( charaindex, CHAR_LV),
				CHAR_getChar( heroindex, CHAR_CDKEY), CHAR_getChar( heroindex, CHAR_NAME), CHAR_getInt( heroindex, CHAR_LV),
				mission, charaindex, heroindex);
		print( msg);
		LogAngel( msg);
	}
}

void sendAngelCleanToCli( int fd)
{
	lssproto_WN_send( fd, //getfdFromCharaIndex(charaindex),
			WINDOW_MESSAGETYPE_ANGELMESSAGE, -1,
			CHAR_WINDOWTYPE_ANGEL_CLEAN,
			-1,	"");
}

int AngelCreate( int angelindex)
{
	int emptyitem1, emptyitem2;
	int angeltokenindex, herotokenindex;
	char msgbuf[1024]/*, nameinfo[1024]*/;
	int mindex, mission;
	char heroname[64];

	if( !CHAR_CHECKINDEX( angelindex))	return FALSE;

	print(" �Ѩϵ����F!! ");
	
	mindex = checkIfAngel( angelindex);
	if( mindex == -1 || missiontable[mindex].flag == MISSION_TIMEOVER ) {
		//print("\n ANGEL���~!!Table�O�ɩεL���??:%d ", angelindex );
		CHAR_talkToCli( angelindex, -1, "�ܩ�p�A�A�ӱߦ^���A�w�g�O�ɤF�C", CHAR_COLORYELLOW);
		
		sprintf( msgbuf, " �^���O�ɩεL��� i:%d �Ϫ�:%s %s ", mindex, CHAR_getChar( angelindex, CHAR_CDKEY), CHAR_getChar( angelindex, CHAR_NAME));
		print( msgbuf);
		LogAngel( msgbuf);
		
		return FALSE;
	}

	if( missiontable[mindex].flag != MISSION_WAIT_ANSWER ) {
		print(" �X�Ф���:%d ", missiontable[mindex].flag);
		return FALSE;
	}

	if( CHAR_findEmptyItemBoxNo( angelindex ) < 2 ){
		CHAR_talkToCli( angelindex, -1, "�Ŷ���줣���C�ܤ֭n����ӪŹD�����C", CHAR_COLORYELLOW);
		return TRUE;
	}

	getStringFromIndexWithDelim( missiontable[mindex].heroinfo, ":", 2, heroname, sizeof(heroname));

	angeltokenindex = ITEM_makeItemAndRegist( ANGELITEM );
	if( angeltokenindex != -1 ){
		/*if( ITEM_getInt( angeltokenindex, ITEM_TYPE) != ITEM_ANGELTOKEN) {
			print(" �s�@�Ϫ̫H������ ");
			ITEM_endExistItemsOne( angeltokenindex);
			return;
		}*/

		//sprintf( nameinfo, "%s:%s", CHAR_getChar( angelindex, CHAR_CDKEY), CHAR_getChar( angelindex, CHAR_NAME ) );
		ITEM_setChar( angeltokenindex, ITEM_ANGELINFO, missiontable[mindex].angelinfo);
		ITEM_setChar( angeltokenindex, ITEM_HEROINFO, missiontable[mindex].heroinfo);
		sprintf( msgbuf, "%d", missiontable[mindex].mission);
		ITEM_setChar( angeltokenindex, ITEM_ANGELMISSION, msgbuf);
		//sprintf( msgbuf, "%s(%s)", ITEM_getChar( angeltokenindex, ITEM_NAME), CHAR_getChar( angelindex, CHAR_NAME ) );
		//ITEM_setChar( angeltokenindex, ITEM_NAME, msgbuf);
		//ITEM_setChar( angeltokenindex, ITEM_SECRETNAME, msgbuf);
		//sprintf( msgbuf, "�o�O�Ϫ� %s �P�i�� %s �M�ݪ��H��", CHAR_getChar( angelindex, CHAR_NAME), heroname );
		sprintf( msgbuf, "���F�Ϫ� %s ���H���A�˳ƫᤣ�J�ġC", CHAR_getChar( angelindex, CHAR_NAME) );
		ITEM_setChar( angeltokenindex, ITEM_EFFECTSTRING, msgbuf);

		emptyitem1 = CHAR_addItemSpecificItemIndex( angelindex, angeltokenindex);
		CHAR_sendItemDataOne( angelindex, emptyitem1);
		LogItem(
			CHAR_getChar( angelindex, CHAR_NAME ),
			CHAR_getChar( angelindex, CHAR_CDKEY ),
			angeltokenindex,
			"AddItem(�s�@�D�� �Ϫ̫H��)",
			CHAR_getInt( angelindex,CHAR_FLOOR),
			CHAR_getInt( angelindex,CHAR_X ),
			CHAR_getInt( angelindex,CHAR_Y ),
			ITEM_getChar( angeltokenindex, ITEM_UNIQUECODE),
			ITEM_getChar( angeltokenindex, ITEM_NAME),
			ITEM_getInt( angeltokenindex, ITEM_ID) );
		CHAR_talkToCli( angelindex, -1,"�o��Ϫ̪��H���C", CHAR_COLORYELLOW);
	}else{
		print("�s�@�Ϫ̪��H�����ѡC" );
		return FALSE;
	}

	herotokenindex = ITEM_makeItemAndRegist( HEROITEM );
	if( herotokenindex != -1 ){
		/*if( ITEM_getInt( herotokenindex, ITEM_TYPE) != ITEM_HEROTOKEN) {
			print(" �s�@�i�̫H������ ");
			ITEM_endExistItemsOne( herotokenindex);
			ITEM_endExistItemsOne( angeltokenindex);
			CHAR_sendItemDataOne( angelindex, emptyitem1);
			return;
		}*/
		//sprintf( nameinfo, "%s:%s", CHAR_getChar( angelindex, CHAR_CDKEY), CHAR_getChar( angelindex, CHAR_NAME ) );
		ITEM_setChar( herotokenindex, ITEM_ANGELINFO, missiontable[mindex].angelinfo);
		ITEM_setChar( herotokenindex, ITEM_HEROINFO, missiontable[mindex].heroinfo);
		sprintf( msgbuf, "%d", missiontable[mindex].mission);
		ITEM_setChar( herotokenindex, ITEM_ANGELMISSION, msgbuf);
		//sprintf( msgbuf, "%s(%s)", ITEM_getChar( herotokenindex, ITEM_NAME), heroname );
		//ITEM_setChar( herotokenindex, ITEM_NAME, msgbuf);
		//ITEM_setChar( herotokenindex, ITEM_SECRETNAME, msgbuf);
		//sprintf( msgbuf, "�o�O�i�� %s �P�Ϫ� %s �M�ݪ��H��", heroname, CHAR_getChar( angelindex, CHAR_NAME) );
		sprintf( msgbuf, "�i�� %s ���H���A�Ϫ̨ϥΥi�ǰe�ܫi�̨���C", heroname );
		ITEM_setChar( herotokenindex, ITEM_EFFECTSTRING, msgbuf);

		emptyitem2 = CHAR_addItemSpecificItemIndex( angelindex, herotokenindex);
		CHAR_sendItemDataOne( angelindex, emptyitem2);
		LogItem(
			CHAR_getChar( angelindex, CHAR_NAME ),
			CHAR_getChar( angelindex, CHAR_CDKEY ),
			herotokenindex,
			"AddItem(�s�@�D�� �i�̫H��)",
			CHAR_getInt( angelindex,CHAR_FLOOR),
			CHAR_getInt( angelindex,CHAR_X ),
			CHAR_getInt( angelindex,CHAR_Y ),
			ITEM_getChar( herotokenindex, ITEM_UNIQUECODE),
			ITEM_getChar( herotokenindex, ITEM_NAME),
			ITEM_getInt( herotokenindex, ITEM_ID) );
		CHAR_talkToCli( angelindex, -1,"�o��i�̪��H���C", CHAR_COLORYELLOW);
	}else{
		print("�s�@�i�̪��H�����ѡC" );
		ITEM_endExistItemsOne( angeltokenindex);
		CHAR_sendItemDataOne( angelindex, emptyitem1);
		return FALSE;
	}

	// ��s��AC Server
	//sprintf( nameinfo, "%s:%s", CHAR_getChar( angelindex, CHAR_CDKEY), CHAR_getChar( angelindex, CHAR_NAME ) );
	sprintf( msgbuf, "%s|%d", missiontable[mindex].angelinfo, missionlist[missiontable[mindex].mission].limittime );
	saacproto_ACMissionTable_send( acfd, MISSION_DOING, 4, msgbuf, "");

	//CHAR_talkToCli( angelindex, -1, "�Ѥ��n�G�D�`�P�§A���������A���ڴN�N�H���浹�A�F�A�бN�i�̪��H����浹�i�̡C", CHAR_COLORYELLOW);

	lssproto_WN_send( getfdFromCharaIndex(angelindex), WINDOW_MESSAGETYPE_MESSAGE, 
			WINDOW_BUTTONTYPE_YES, -1, -1,
			"�D�`�P�§A���������A���ڴN�N�H���浹�A�F�A�бN�i�̪��H����浹�i�̡C");

	sprintf( msgbuf, " �Ϫ̵��������F i:%d �Ϫ�:%s �i��:%s ci=%d ", mindex, missiontable[mindex].angelinfo, missiontable[mindex].heroinfo, angelindex);
	print( msgbuf);
	LogAngel( msgbuf);

	return FALSE;
}


// �ϥΨϪ̫H����
void Use_AngelToken( int charaindex, int toindex, int haveitemindex )
{
	int itemindex;
	char nameinfo[1024];
	int mindex;
	char msg[1024];
	char tokenbuf[64];

	print(" �ϥΨϪ̫H�� ");
	
	if( !CHAR_CHECKINDEX( charaindex) )	return;
	itemindex = CHAR_getItemIndex( charaindex, haveitemindex);
	if( !ITEM_CHECKINDEX( itemindex) ) return;

	mindex = checkIfAngel( charaindex);
	//sprintf( nameinfo, "%s:%s", CHAR_getChar(charaindex, CHAR_CDKEY), CHAR_getChar(charaindex, CHAR_NAME));
	getMissionNameInfo( charaindex, nameinfo);

	if( mindex == -1 || 
		( strcmp( nameinfo, ITEM_getChar( itemindex, ITEM_ANGELINFO)) && strcmp( nameinfo, ITEM_getChar( itemindex, ITEM_HEROINFO)) ) ) {
		// ���H�Ҩϥή�
		CHAR_talkToCli( charaindex, -1, "�o�ä��O�ݩ�A���H���A���i�H�K�ϥγ�C", CHAR_COLORRED);
		return;
	}

	if( strcmp( ITEM_getChar( itemindex, ITEM_ANGELINFO), missiontable[mindex].angelinfo)
		|| strcmp( ITEM_getChar( itemindex, ITEM_HEROINFO), missiontable[mindex].heroinfo) ) {

		CHAR_talkToCli( charaindex, -1, "�o�O�L�Ϊ��H���A�٬O�ᱼ�a�C", CHAR_COLORRED);
		return;
	}

	if( !strcmp( nameinfo, missiontable[mindex].angelinfo) ) { // �Ϫ̨ϥή�

		int lefttime, hour, min;
		lefttime = missiontable[mindex].time + missiontable[mindex].limittime*60*60 - (int)time(NULL);
		hour = lefttime / (60*60);
		min = (lefttime % (60*60)) / 60;

		if( missiontable[mindex].flag == MISSION_DOING ) {
			// ��ܥ��ȸ��
			getStringFromIndexWithDelim( missiontable[mindex].heroinfo, ":", 2, tokenbuf, sizeof(tokenbuf));
			sprintf( msg, "�A���ϩR�O�N�i�̪��H���浹 %s �A%s�A�ɶ��ٳѾl%d�p��%d���C",
				tokenbuf, missionlist[missiontable[mindex].mission].detail, hour, min);
			CHAR_talkToCli( charaindex, -1, msg, CHAR_COLORRED);
		}
		else if( missiontable[mindex].flag == MISSION_HERO_COMPLETE ) {
			// �i�H�h����F
			getStringFromIndexWithDelim( missiontable[mindex].heroinfo, ":", 2, tokenbuf, sizeof(tokenbuf));
			sprintf( msg, "�i�� %s ���ϩR�w�g�����F�A�A�i�H�h����F�A�ɶ��ٳѾl%d�p��%d���C",
				tokenbuf, hour, min);
			CHAR_talkToCli( charaindex, -1, msg, CHAR_COLORRED);
		}
		else if( missiontable[mindex].flag == MISSION_TIMEOVER ) {
			// �ɶ��W�L�F
			sprintf( msg, "�ܥi���A�Ϫ̩M�i�̨èS���b�ɭ��������ϩR�A�U���A�[�o�a�C");
			CHAR_talkToCli( charaindex, -1, msg, CHAR_COLORRED);
		}
		return;
	}
	else if( !strcmp( nameinfo, missiontable[mindex].heroinfo) ) { // �i�̨ϥή�
		// �ǰe�ܨϪ̨���
		int maxchar, i;
		char cdkey[64], name[64];
		int fl =0, fx =0, fy =0;

		if( checkUnlawWarpFloor( CHAR_getInt( charaindex, CHAR_FLOOR) ) ) {
				CHAR_talkToCli( charaindex, -1, "�A�Ҧb���a��L�k�ǰe�C", CHAR_COLORYELLOW );
				return;
		}

		getStringFromIndexWithDelim( missiontable[mindex].angelinfo, ":", 1, cdkey, sizeof(cdkey));
		getStringFromIndexWithDelim( missiontable[mindex].angelinfo, ":", 2, name, sizeof(name));
		maxchar = CHAR_getPlayerMaxNum();
		for( i =0; i <maxchar; i++)
		{
			if( !CHAR_CHECKINDEX( i) ) continue;
			if( CHAR_getInt( i, CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) continue;
			//sprintf( tempinfo, "%s:%s", CHAR_getChar( i, CHAR_CDKEY), CHAR_getChar( i, CHAR_NAME));
			if( strcmp( cdkey, CHAR_getChar( i, CHAR_CDKEY)) ) continue;
			if( strcmp( name, CHAR_getChar( i, CHAR_NAME)) ) continue;
			
			fl = CHAR_getInt( i, CHAR_FLOOR);
			fx = CHAR_getInt( i, CHAR_X);
			fy = CHAR_getInt( i, CHAR_Y);

			if( checkUnlawWarpFloor( fl) ) {
				CHAR_talkToCli( charaindex, -1, "�ﹳ�Ҧb���a��L�k�ǰe�C", CHAR_COLORYELLOW );
				return;
			}

			break;
		}
		if( fl <= 0 )
		{
			sprintf( msg, "�Ϫ� %s �ثe���b�u�W�Τ��b�����A���W�C", name);
			CHAR_talkToCli( charaindex, -1, msg, CHAR_COLORRED);
			return;
		}
		
		sprintf( msg, "�ǰe�ܨϪ� %s ����C", name);
		CHAR_talkToCli( charaindex, -1, msg, CHAR_COLORYELLOW);
		CHAR_warpToSpecificPoint( charaindex, fl, fx, fy);
	}

}

// �ϥΫi�̫H����
void Use_HeroToken( int charaindex, int toindex, int haveitemindex )
{
	int itemindex;
	int mindex;
	char nameinfo[64];
	char msg[1024];

	print(" �ϥΫi�̫H�� ");

	if( !CHAR_CHECKINDEX( charaindex) )	return;
	itemindex = CHAR_getItemIndex( charaindex, haveitemindex);
	if( !ITEM_CHECKINDEX( itemindex) ) return;

	mindex = checkIfAngel( charaindex);
	//sprintf( nameinfo, "%s:%s", CHAR_getChar(charaindex, CHAR_CDKEY), CHAR_getChar(charaindex, CHAR_NAME));
	getMissionNameInfo( charaindex, nameinfo);

	if( mindex == -1 ||
		( strcmp( nameinfo, ITEM_getChar( itemindex, ITEM_ANGELINFO)) && strcmp( nameinfo, ITEM_getChar( itemindex, ITEM_HEROINFO)) ) ) {
		// ���H�Ҩϥή�
		CHAR_talkToCli( charaindex, -1, "�o�ä��O�ݩ�A���H���A���i�H�K�ϥγ�C", CHAR_COLORRED);
		return;
	}

	if(  strcmp( ITEM_getChar( itemindex, ITEM_ANGELINFO), missiontable[mindex].angelinfo)
		|| strcmp( ITEM_getChar( itemindex, ITEM_HEROINFO), missiontable[mindex].heroinfo) ){

		CHAR_talkToCli( charaindex, -1, "�o�O�L�Ϊ��H���A�٬O�ᱼ�a�C", CHAR_COLORRED);
		return;
	}


	if( !strcmp( nameinfo, missiontable[mindex].heroinfo) ) { // �i�̨ϥή�

		int lefttime, hour, min;
		lefttime = missiontable[mindex].time + missiontable[mindex].limittime*60*60 - (int)time(NULL);
		hour = lefttime / (60*60);
		min = (lefttime % (60*60)) / 60;

		if( missiontable[mindex].flag == MISSION_DOING ) {
			// ��ܥ��ȸ��
			sprintf( msg, "�A���ϩR�O %s�A�ɶ��ٳѾl%d�p��%d���C",
				missionlist[missiontable[mindex].mission].detail, hour, min);
			CHAR_talkToCli( charaindex, -1, msg, CHAR_COLORRED);
		}
		else if( missiontable[mindex].flag == MISSION_HERO_COMPLETE ) {
			// �i�H�h����F
			sprintf( msg, "�A���ϩR�w�g�����F�A�i�H�h����F�A�ɶ��ٳѾl%d�p��%d���C",
				hour, min);
			CHAR_talkToCli( charaindex, -1, msg, CHAR_COLORRED);
		}
		else if( missiontable[mindex].flag == MISSION_TIMEOVER ) {
			// �ɶ��W�L�F
			sprintf( msg, "�ܥi���A�Ϫ̩M�i�̨èS���b�ɭ��������ϩR�A�U���A�[�o�a�C");
			CHAR_talkToCli( charaindex, -1, msg, CHAR_COLORRED);
		}
		return;

	}
	else if( !strcmp( nameinfo, missiontable[mindex].angelinfo) ) { // �Ϫ̨ϥή�
		// �ǰe�ܫi�̨���
		int maxchar, i;
		char cdkey[64], name[64];
		int fl =0, fx =0, fy =0;

		if( CHAR_getWorkInt( charaindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE ) {
				CHAR_talkToCli( charaindex, -1, "�ն����L�k�ǰe�C", CHAR_COLORYELLOW );
				return;
		}
		
		if( checkUnlawWarpFloor( CHAR_getInt( charaindex, CHAR_FLOOR) ) ) {
				CHAR_talkToCli( charaindex, -1, "�A�Ҧb���a��L�k�ǰe�C", CHAR_COLORYELLOW );
				return;
		}

		//if( CHAR_CheckInItemForWares( charaindex, 0) == FALSE ){
		if( CheckDropatLogout( charaindex) == TRUE ) {
			CHAR_talkToCli( charaindex, -1, "��a�n�X��������D��ɵL�k�ϥΡC", CHAR_COLORYELLOW);
			return;
		}

		getStringFromIndexWithDelim( missiontable[mindex].heroinfo, ":", 1, cdkey, sizeof(cdkey));
		getStringFromIndexWithDelim( missiontable[mindex].heroinfo, ":", 2, name, sizeof(name));
		maxchar = CHAR_getPlayerMaxNum();
		for( i =0; i <maxchar; i++)
		{
			if( !CHAR_CHECKINDEX( i) ) continue;
			if( CHAR_getInt( i, CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) continue;
			//sprintf( tempinfo, "%s:%s", CHAR_getChar( i, CHAR_CDKEY), CHAR_getChar( i, CHAR_NAME));
			if( strcmp( cdkey, CHAR_getChar( i, CHAR_CDKEY)) ) continue;
			if( strcmp( name, CHAR_getChar( i, CHAR_NAME)) ) continue;
			
			fl = CHAR_getInt( i, CHAR_FLOOR);
			fx = CHAR_getInt( i, CHAR_X);
			fy = CHAR_getInt( i, CHAR_Y);

			if( checkUnlawWarpFloor( fl) ) {
				CHAR_talkToCli( charaindex, -1, "�ﹳ�Ҧb���a��L�k�ǰe�L�h�C", CHAR_COLORYELLOW );
				return;
			}

			break;
		}
		if( fl <= 0 )
		{
			sprintf( msg, "�i�� %s �ثe���b�u�W�Τ��b�����A���W�C", name);
			CHAR_talkToCli( charaindex, -1, msg, CHAR_COLORRED);
			return;
		}
		
		sprintf( msg, "�ǰe�ܫi�� %s ����C", name);
		CHAR_talkToCli( charaindex, -1, msg, CHAR_COLORYELLOW);
		CHAR_warpToSpecificPoint( charaindex, fl, fx, fy);

	}
	else { // ���H�Ҩϥή�
		CHAR_talkToCli( charaindex, -1, "�o�ä��O�ݩ�A���H���A���i�H�K�ϥγ�C", CHAR_COLORRED);
		return;
	}


}


void CHAR_sendAngelMark( int objindex, int flag)
{
	int		opt[1];
	opt[0] = flag;
	CHAR_sendWatchEvent( objindex,CHAR_ACTANGEL,opt,1,TRUE);
}


#endif
