#include "version.h"
#include <time.h>
#include "char.h"
#include "object.h"
#include "lssproto_serv.h"
#include "saacproto_cli.h"
#include "npcutil.h"
#include "handletime.h"
#include "npc_raceman.h"
#include "family.h"
#include "log.h"
#include "enemy.h"

#ifdef _RACEMAN

void Raceman_SelectWindow( int meindex, int talker, int num,int select);
void Raceman_MsgDisp(int meindex,int talker,int num,int select);
BOOL Raceman_EventAddItem( int meindex,int talker );
BOOL Raceman_CheckItem(int meindex,int talker);
BOOL Raceman_ItemFullCheck(int meindex, int talker);
BOOL Raceman_checkpet( int meindex, int talker , int select , char *token );
BOOL Raceman_petcheckin ( int meindex , int talker );	//���w����
BOOL Raceman_checkprize( int meindex , int talker ,char *token);
void Raceman_awardprize( int meindex , int talker );
int  Raceman_CheckGameType(int meindex , int talker );
int  Raceman_readmode( int meindex , int talkerindex );
void Raceman_countrank( int meindex, int talker ); // �ثe�S�Ψ�
void Raceman_renewrank( int meindex , int talker , int select );
int	 Raceman_CheckSelectRank( int meindex , int talkerindex , int datanum);
void Raceman_GetQuestion( int meindex ,int talker , char *token);
BOOL Raceman_CheckAnswer( int meindex , int talker , char *data );



enum {
	CHAR_WORK_PETSELECT	= CHAR_NPCWORKINT1,
	CHAR_WORK_RACEQUIZ	= CHAR_NPCWORKINT2,
};

//int PetSelect;
int Pageindex;
int asktotal;

struct RANKTABLE ranktable1[DEFMAXRANK];
struct RANKTABLE ranktable2[DEFMAXRANK];
struct RANKTABLE ranktable3[DEFMAXRANK];
struct RANKTABLE ranktable4[DEFMAXRANK];
struct RANKTABLE ranktable5[DEFMAXRANK];
struct RANKTABLE history1[DEFMAXRANK];
struct RANKTABLE history2[DEFMAXRANK];
struct RANKTABLE history3[DEFMAXRANK];
struct RANKTABLE history4[DEFMAXRANK];
struct RANKTABLE history5[DEFMAXRANK];

struct ASKTABLE asktable[400];

int petflgtable[640];  //�i���d��table

char  rankname[5][64] = {
	"�y�s����--�嬰�ѤH�Ʀ�]�z",
	"�y������--�嬰�ѤH�Ʀ�]�z",
	"�y�a����--�嬰�ѤH�Ʀ�]�z",
	"�y������--�L�_�����Ʀ�]�z",
	"�y�a����--�L�_�����Ʀ�]�z"
};

char  historyrank[5][64] = {
	"�y�s����v�嬰�ѤH�Ʀ�]�z",
	"�y������v�嬰�ѤH�Ʀ�]�z",
	"�y�a�ھ��v�嬰�ѤH�Ʀ�]�z",
	"�y������v�L�_�����Ʀ�]�z",
	"�y�a�ھ��v�L�_�����Ʀ�]�z"
};


#define  DEF_READ_WAITTIME            (10)
unsigned long RTIME1 = 0,
              RTIME2 = 0,
              RTIME3 = 0,
			  RTIME4 = 0,
              RTIME5 = 0;

BOOL NPC_RacemanInit( int meindex)
{
	char npcarg1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *npcarg;
	char buf1[128];
	int  ranknum,i;
	char file[10][20] = {
		"Mode1",
		"Mode2",
		"Mode3",
		"Mode4",
		"Mode5",
		"History1",
		"History2",
		"History3",
		"History4",
		"History5"
	};
	
	//PetSelect = 0;
	Pageindex = 0;
	
	if((npcarg = NPC_Util_GetArgStr( meindex, npcarg1, sizeof( npcarg1)))== NULL)	return FALSE;

	NPC_Util_GetStrFromStrWithDelim( npcarg , "RankNum", buf1, sizeof( buf1));
	ranknum = atoi(buf1);

	CHAR_setInt( meindex, CHAR_WHICHTYPE, CHAR_RACEMAN);  //�s�@�@��npc

	if( RTIME1 == 0 || RTIME2 == 0 || RTIME3 == 0 || RTIME4 == 0 || RTIME5 == 0){
	    RTIME1 = NowTime.tv_sec+DEF_READ_WAITTIME,
		RTIME2 = NowTime.tv_sec+DEF_READ_WAITTIME,
		RTIME3 = NowTime.tv_sec+DEF_READ_WAITTIME,
		RTIME4 = NowTime.tv_sec+DEF_READ_WAITTIME;
		RTIME5 = NowTime.tv_sec+DEF_READ_WAITTIME;
		
		//��5�ؤ��ɰO��Ū���i��
		for( i = 0 ; i < 10 ; i++){
			NPC_Util_GetStrFromStrWithDelim( npcarg , file[i] , buf1, sizeof( buf1));
			if(strlen(buf1) == 0) continue;
			if(i < 5)
				saacproto_ACRaceRecordandSort_send( acfd,meindex,buf1,"","",i+1,0,ranknum );
			else
				saacproto_ACRaceRecordandSort_send( acfd,meindex,buf1,"","",i+6,0,ranknum );
		}

	}
	return TRUE;

}

void NPC_RacemanTalked( int meindex , int talkerindex , char *szMes ,int color )
{
	
    if( CHAR_getInt( talkerindex , CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER ) {
    	return;
    }

	if(NPC_Util_isFaceToFace(talkerindex,meindex,1 )==FALSE){
		if( NPC_Util_CharDistance( talkerindex, meindex ) > 1) return;
	}

	Raceman_SelectWindow( meindex, talkerindex,0,-1);
}


void Raceman_SelectWindow( int meindex, int talker, int num,int select)
{
	char token[1024];
	int  fd , ret;	

    fd = getfdFromCharaIndex( talker );
    if( fd == -1 )  return;
	
	token[0] = '\0';
		 	
	switch( num) {
		
		  case 0:	//���
		  
		  	Raceman_MsgDisp( meindex, talker, 0 , -1 );
			break;

		  case 1:	//�����D��
		  
	  		Raceman_MsgDisp( meindex, talker, 1 , -1 );
		  	break;

		  case 2:	//���y�d��
			
			ret = Raceman_CheckGameType(meindex,talker);
			if(ret == 0)
				Raceman_MsgDisp( meindex, talker, 2 , -1 );
			else if(ret == 1)
				Raceman_MsgDisp( meindex, talker, 9 , -1 );
			else if(ret == 2)
				Raceman_MsgDisp( meindex, talker, 90 , -1 );
			else if(ret == 3)
				Raceman_MsgDisp( meindex, talker, 91 , -1 );
			else if(ret == 4)
				Raceman_MsgDisp( meindex, talker, 92 , -1 );
			break;

		  case 3:	//�y�d�n�O

			ret = Raceman_CheckGameType(meindex,talker);
			if(ret == 1)	
				Raceman_MsgDisp( meindex, talker, 9 , -1 );	//�ڪ��������X
			else if(ret == 4)
				Raceman_MsgDisp( meindex, talker, 92 , -1 );	//�a�ڨS����������
			else
				Raceman_MsgDisp( meindex, talker, 3 , -1 );
			break;

		  case 4:	//�y�d�Ʀ�]
			
			Raceman_MsgDisp( meindex, talker, 4 , -1 );
			break;

		  case 5:	//�y�d�ԧQ�~

			ret = Raceman_CheckGameType(meindex,talker);
			if(ret == 1)	
				Raceman_MsgDisp( meindex, talker, 9 , -1 );	//�ڪ��������X
			else if(ret == 4)
				Raceman_MsgDisp( meindex, talker, 92 , -1 );	//�a�ڨS����������
			else
				Raceman_MsgDisp( meindex, talker, 5 , -1 );
			break;

		  case 6:   //�q���K�y

			ret = Raceman_CheckGameType(meindex,talker);
			if(ret == 1)	
				Raceman_MsgDisp( meindex, talker, 9 , -1 );	//�ڪ��������X
			else if(ret == 4)
				Raceman_MsgDisp( meindex, talker, 92 , -1 );	//�a�ڨS����������
			else
				Raceman_MsgDisp( meindex, talker, 6 , -1 );
			break;

	}	
}

void NPC_RacemanWindowTalked( int meindex , int talkerindex , int seqno, int select, char *data)
{
	int datanum = -1;
	int buttontype;
	int i,open,ret ;	
	char listbuf[4096],tmp_buffer[4096];

	listbuf[0] = '\0';
	tmp_buffer[0] = '\0';

	if( NPC_Util_CharDistance( talkerindex, meindex ) > 2) {
		return;
	}
	print("\n NPC_RacemanWindowTalked: data:%s seq:%d sel:%d ", data, seqno, select);

	makeStringFromEscaped( data);

	datanum = atoi( data);
	switch( seqno){
	case CHAR_WINDOWTYPE_RACE_START:
		if(datanum == 2){	// �����D��
			Raceman_SelectWindow( meindex, talkerindex,1,-1);
		}else if(datanum == 3){ // ����y�d��
			Raceman_SelectWindow( meindex, talkerindex,2,-1);
		}else if(datanum == 4){ // �y�d�n�O�B
			//Raceman_SelectWindow( meindex, talkerindex,3,-1);�ק�  �[�J�q���K�y�~��n�O
			Raceman_SelectWindow( meindex, talkerindex,6,-1);	
		}else if(datanum == 5){ // �y�d�Ʀ�]
			Raceman_SelectWindow( meindex, talkerindex,4,-1);
		}else if(datanum == 6){	// �y�d�ԧQ�~
			Raceman_SelectWindow( meindex, talkerindex,5,-1);
		}
		break;
	case CHAR_WINDOWTYPE_RACE_SUBJECT:
		if(select == WINDOW_BUTTONTYPE_OK){
			Raceman_SelectWindow( meindex, talkerindex,0,-1);
		}
		break;
	case CHAR_WINDOWTYPE_RACE_TICKET:
		if(select == WINDOW_BUTTONTYPE_OK)
		{
			if(Raceman_CheckItem(meindex,talkerindex)==TRUE){ //TRUE:���ҤF
				Raceman_MsgDisp( meindex, talkerindex,22,-1);
			}else{
				if(Raceman_EventAddItem(meindex, talkerindex)==TRUE){
					Raceman_SelectWindow( meindex, talkerindex,0,-1); 
				}
			}
		}
		break;
	case CHAR_WINDOWTYPE_RACE_PETSELECT:
		if(Raceman_CheckItem(meindex,talkerindex)==FALSE){ //FALSE:�S����
			Raceman_MsgDisp( meindex, talkerindex,32,-1);
		}else{
			if(select == WINDOW_BUTTONTYPE_CANCEL){
				Raceman_SelectWindow( meindex, talkerindex,0,-1);
			}else{
				Raceman_MsgDisp( meindex, talkerindex,31,datanum);
			}
		}
		break;
	case CHAR_WINDOWTYPE_RACE_CHECKIN:
		if(select == WINDOW_BUTTONTYPE_YES){
			ret = Raceman_petcheckin( meindex, talkerindex );   //�n�O
			if(ret == TRUE)
				Raceman_SelectWindow( meindex, talkerindex,0,-1);	
			else
				Raceman_MsgDisp( meindex, talkerindex,93,-1);	
		}else if(select == WINDOW_BUTTONTYPE_NO){
			Raceman_SelectWindow( meindex, talkerindex,0,-1);
		}
		break;
	case CHAR_WINDOWTYPE_RACE_RANK:
		open = Raceman_CheckSelectRank(meindex,talkerindex,datanum);
		switch(open)
		{
		case 1:
			if( NowTime.tv_sec > RTIME1 ){
				Raceman_renewrank( meindex , talkerindex , datanum-1 );  //��s
				RTIME1 = NowTime.tv_sec+DEF_READ_WAITTIME;				
			}
			if(ranktable1[0].rank == 0) Raceman_MsgDisp( meindex, talkerindex,46,-1);
			else Raceman_MsgDisp( meindex, talkerindex,41,-1);
			break;
		case 2:
			if( NowTime.tv_sec > RTIME2 ){
				Raceman_renewrank( meindex , talkerindex , datanum-1 );  //��s
				RTIME2 = NowTime.tv_sec+DEF_READ_WAITTIME;				
			}
			if(ranktable2[0].rank == 0) Raceman_MsgDisp( meindex, talkerindex,46,-1);
			else Raceman_MsgDisp( meindex, talkerindex,42,-1);				  	
			break;
		case 3:
			if( NowTime.tv_sec > RTIME3 ){
				Raceman_renewrank( meindex , talkerindex , datanum-1 );  //��s
				RTIME3 = NowTime.tv_sec+DEF_READ_WAITTIME;				
			}
			if(ranktable3[0].rank == 0) Raceman_MsgDisp( meindex, talkerindex,46,-1);
			else Raceman_MsgDisp( meindex, talkerindex,43,-1);				  	
			break;
		case 4:
			if( NowTime.tv_sec > RTIME4 ){
				Raceman_renewrank( meindex , talkerindex , datanum-1 );  //��s
				RTIME4 = NowTime.tv_sec+DEF_READ_WAITTIME;				
			}
			if(ranktable4[0].rank == 0) Raceman_MsgDisp( meindex, talkerindex,46,-1);
			else Raceman_MsgDisp( meindex, talkerindex,44,-1);				  	
			break;
		case 5:
			if( NowTime.tv_sec > RTIME5 ){
				Raceman_renewrank( meindex , talkerindex , datanum-1 );  //��s
				RTIME5 = NowTime.tv_sec+DEF_READ_WAITTIME;				
			}
			if(ranktable5[0].rank == 0) Raceman_MsgDisp( meindex, talkerindex,46,-1);
			else Raceman_MsgDisp( meindex, talkerindex,45,-1);				  	
			break;
		case 11:
			if(history1[0].rank == 0) Raceman_MsgDisp( meindex, talkerindex,46,-1);
			else Raceman_MsgDisp( meindex, talkerindex,411,-1);
			break;
		case 12:
			if(history2[0].rank == 0) Raceman_MsgDisp( meindex, talkerindex,46,-1);
			else Raceman_MsgDisp( meindex, talkerindex,412,-1);
			break;
		case 13:
			if(history3[0].rank == 0) Raceman_MsgDisp( meindex, talkerindex,46,-1);
			else Raceman_MsgDisp( meindex, talkerindex,413,-1);
			break;
		case 14:
			if(history4[0].rank == 0) Raceman_MsgDisp( meindex, talkerindex,46,-1);
			else Raceman_MsgDisp( meindex, talkerindex,414,-1);
			break;
		case 15:
			if(history5[0].rank == 0) Raceman_MsgDisp( meindex, talkerindex,46,-1);
			else Raceman_MsgDisp( meindex, talkerindex,415,-1);
			break;
		default:
			Raceman_SelectWindow( meindex, talkerindex,0,-1);
			break;
		}
		break;
	case CHAR_WINDOWTYPE_RACE_PRIZE:
		if(select == WINDOW_BUTTONTYPE_OK){
			Raceman_awardprize( meindex, talkerindex );
			Raceman_SelectWindow( meindex, talkerindex,0,-1);  
		}		
		break;
	case CHAR_WINDOWTYPE_RACE_NORMAL:
		if(select == WINDOW_BUTTONTYPE_OK){
			Raceman_SelectWindow( meindex, talkerindex,0,-1);
		}
		break;
	case CHAR_WINDOWTYPE_RACE_SHOWRANK1:
		switch(select){
			case WINDOW_BUTTONTYPE_NEXT:
			case WINDOW_BUTTONTYPE_PREV:
				{
					int fd;
					fd = getfdFromCharaIndex( talkerindex );
					if( fd == -1 )  return;
					
					Pageindex += 10 * (( select == WINDOW_BUTTONTYPE_NEXT) ? 1 : -1);

					if(ranktable1[Pageindex].catchcnt==0)  {
						Raceman_SelectWindow( meindex, talkerindex,0,-1);
						return;
					}
					if( Pageindex < 0 ) return; 

					buttontype = WINDOW_BUTTONTYPE_OK;
					if( ranktable1[Pageindex+10].catchcnt==0)
						buttontype |= WINDOW_BUTTONTYPE_PREV;
					else if( Pageindex == 0)
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					else{
						buttontype |= WINDOW_BUTTONTYPE_PREV;
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					}
					//�������
					strcpy(listbuf,"�y�s���嬰�ѤH�Ʀ�]�z "); 
					for (i=Pageindex;i<Pageindex+10;i++)
					{
						if(ranktable1[i].rank == 0 || strlen(ranktable1[i].name) == 0 || ranktable1[i].catchcnt == 0) break;
						sprintf(tmp_buffer,"%d %s %d ",ranktable1[i].rank,ranktable1[i].name , ranktable1[i].catchcnt);
						strcat(listbuf,tmp_buffer);
					}

					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_RACEMAN_RANK,
						buttontype,
						CHAR_WINDOWTYPE_RACE_SHOWRANK1,
						CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
						listbuf	);
				}
				break;
			case WINDOW_BUTTONTYPE_OK:
				Raceman_SelectWindow( meindex, talkerindex,0,-1);	break;
			default:break;
			}
		break;
	case CHAR_WINDOWTYPE_RACE_SHOWRANK2:
		switch(select){
			case WINDOW_BUTTONTYPE_NEXT:
			case WINDOW_BUTTONTYPE_PREV:
				{
					int fd;
					fd = getfdFromCharaIndex( talkerindex );
					if( fd == -1 )  return;
					
					Pageindex += 10 * (( select == WINDOW_BUTTONTYPE_NEXT) ? 1 : -1);

					if(ranktable2[Pageindex].catchcnt==0)  {
						Raceman_SelectWindow( meindex, talkerindex,0,-1);
						return;
					}
					if( Pageindex < 0 ) return; 

					buttontype = WINDOW_BUTTONTYPE_OK;
					if( ranktable2[Pageindex+10].catchcnt==0)
						buttontype |= WINDOW_BUTTONTYPE_PREV;
					else if( Pageindex == 0)
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					else{
						buttontype |= WINDOW_BUTTONTYPE_PREV;
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					}
					//�������					
					strcpy(listbuf,"�y�����嬰�ѤH�Ʀ�]�z "); 
					for (i=Pageindex;i<Pageindex+10;i++)
					{
						if(ranktable2[i].rank == 0 || strlen(ranktable2[i].name) == 0 || ranktable2[i].catchcnt == 0) break;
						sprintf(tmp_buffer,"%d %s %d ",ranktable2[i].rank,ranktable2[i].name , ranktable2[i].catchcnt);
						strcat(listbuf,tmp_buffer);
					}

					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_RACEMAN_RANK,
						buttontype,
						CHAR_WINDOWTYPE_RACE_SHOWRANK2,
						CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
						listbuf	);
				}
				break;
			case WINDOW_BUTTONTYPE_OK:
				Raceman_SelectWindow( meindex, talkerindex,0,-1);	break;
			default:break;
			}
		break;
	case CHAR_WINDOWTYPE_RACE_SHOWRANK3:
		switch(select){
			case WINDOW_BUTTONTYPE_NEXT:
			case WINDOW_BUTTONTYPE_PREV:
				{
					int fd;
					fd = getfdFromCharaIndex( talkerindex );
					if( fd == -1 )  return;
					
					Pageindex += 10 * (( select == WINDOW_BUTTONTYPE_NEXT) ? 1 : -1);

					if(ranktable3[Pageindex].catchcnt==0)  {
						Raceman_SelectWindow( meindex, talkerindex,0,-1);
						return;
					}
					if( Pageindex < 0 ) return; 

					buttontype = WINDOW_BUTTONTYPE_OK;
					if( ranktable3[Pageindex+10].catchcnt==0)
						buttontype |= WINDOW_BUTTONTYPE_PREV;
					else if( Pageindex == 0)
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					else{
						buttontype |= WINDOW_BUTTONTYPE_PREV;
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					}
					//�������
					strcpy(listbuf,"�y�a���嬰�ѤH�Ʀ�]�z "); 
					for (i=Pageindex;i<Pageindex+10;i++)
					{
						if(ranktable3[i].rank == 0 || strlen(ranktable3[i].name) == 0 || ranktable3[i].catchcnt == 0) break;
						sprintf(tmp_buffer,"%d %s %d ",ranktable3[i].rank,ranktable3[i].name , ranktable3[i].catchcnt);
						strcat(listbuf,tmp_buffer);
					}

					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_RACEMAN_RANK,
						buttontype,
						CHAR_WINDOWTYPE_RACE_SHOWRANK3,
						CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
						listbuf	);
				}
				break;
			case WINDOW_BUTTONTYPE_OK:
				Raceman_SelectWindow( meindex, talkerindex,0,-1);	break;
			default:break;
			}
		break;
	case CHAR_WINDOWTYPE_RACE_SHOWRANK4:
		switch(select){
			case WINDOW_BUTTONTYPE_NEXT:
			case WINDOW_BUTTONTYPE_PREV:
				{
					int fd;
					fd = getfdFromCharaIndex( talkerindex );
					if( fd == -1 )  return;
					
					Pageindex += 10 * (( select == WINDOW_BUTTONTYPE_NEXT) ? 1 : -1);

					if(ranktable4[Pageindex].catchcnt==0)  {
						Raceman_SelectWindow( meindex, talkerindex,0,-1);
						return;
					}
					if( Pageindex < 0 ) return; 

					buttontype = WINDOW_BUTTONTYPE_OK;
					if( ranktable4[Pageindex+10].catchcnt==0)
						buttontype |= WINDOW_BUTTONTYPE_PREV;
					else if( Pageindex == 0)
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					else{
						buttontype |= WINDOW_BUTTONTYPE_PREV;
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					}
					//�������					
					strcpy(listbuf,"�y����L�_�����Ʀ�]�z "); 
					for (i=Pageindex;i<Pageindex+10;i++)
					{
						if(ranktable4[i].rank == 0 || strlen(ranktable4[i].name) == 0 || ranktable4[i].catchcnt == 0) break;
						sprintf(tmp_buffer,"%d %s %d ",ranktable4[i].rank,ranktable4[i].name , ranktable4[i].catchcnt);
						strcat(listbuf,tmp_buffer);
					}

					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_RACEMAN_RANK,
						buttontype,
						CHAR_WINDOWTYPE_RACE_SHOWRANK4,
						CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
						listbuf	);
				}
				break;
			case WINDOW_BUTTONTYPE_OK:
				Raceman_SelectWindow( meindex, talkerindex,0,-1);	break;
			default:break;
			}
		break;
	case CHAR_WINDOWTYPE_RACE_SHOWRANK5:
		//�ثe�q����@��		
		switch(select){
			case WINDOW_BUTTONTYPE_NEXT:
			case WINDOW_BUTTONTYPE_PREV:
				{
					int fd;
					fd = getfdFromCharaIndex( talkerindex );
					if( fd == -1 )  return;
					
					Pageindex += 10 * (( select == WINDOW_BUTTONTYPE_NEXT) ? 1 : -1);

					if(ranktable5[Pageindex].catchcnt==0)  {
						//Pageindex = Pageindex - 10;	�L�U�@���@�v�^����
						Raceman_SelectWindow( meindex, talkerindex,0,-1);
						return;
					}
					if( Pageindex < 0 ) return; //

					buttontype = WINDOW_BUTTONTYPE_OK;
					if( ranktable5[Pageindex+10].catchcnt==0)
						buttontype |= WINDOW_BUTTONTYPE_PREV;
					else if( Pageindex == 0)
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					else{
						buttontype |= WINDOW_BUTTONTYPE_PREV;
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					}
					//�������
					strcpy(listbuf,"�y�a�ڵL�_�����Ʀ�]�z "); 					
					for (i=Pageindex;i<Pageindex+10;i++)
					{						
						if(ranktable5[i].rank == 0 || strlen(ranktable5[i].name) == 0 || ranktable5[i].catchcnt == 0) break;
						sprintf(tmp_buffer,"%d %s %d ",ranktable5[i].rank,ranktable5[i].name , ranktable5[i].catchcnt);
						strcat(listbuf,tmp_buffer);
					}

					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_RACEMAN_RANK,
						buttontype,
						CHAR_WINDOWTYPE_RACE_SHOWRANK5,
						CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
						listbuf	);
				}
				break;
			case WINDOW_BUTTONTYPE_OK:
				Raceman_SelectWindow( meindex, talkerindex,0,-1);	break;
			default:break;
		}
		break;
	case CHAR_WINDOWTYPE_RACE_SHOWRANK11:
		switch(select){
			case WINDOW_BUTTONTYPE_NEXT:
			case WINDOW_BUTTONTYPE_PREV:
				{
					int fd;
					fd = getfdFromCharaIndex( talkerindex );
					if( fd == -1 )  return;
					
					Pageindex += 10 * (( select == WINDOW_BUTTONTYPE_NEXT) ? 1 : -1);

					if(history1[Pageindex].catchcnt==0)  {
						Raceman_SelectWindow( meindex, talkerindex,0,-1);
						return;
					}
					if( Pageindex < 0 ) return; 

					buttontype = WINDOW_BUTTONTYPE_OK;
					if( history1[Pageindex+10].catchcnt==0)
						buttontype |= WINDOW_BUTTONTYPE_PREV;
					else if( Pageindex == 0)
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					else{
						buttontype |= WINDOW_BUTTONTYPE_PREV;
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					}
					//�������
					strcpy(listbuf,"�y�s����v�嬰�ѤH�Ʀ�]�z "); 
					for (i=Pageindex;i<Pageindex+10;i++)
					{
						if(history1[i].rank == 0 || strlen(history1[i].name) == 0 || history1[i].catchcnt == 0) break;
						sprintf(tmp_buffer,"%d %s %d ",history1[i].rank,history1[i].name , history1[i].catchcnt);
						strcat(listbuf,tmp_buffer);
					}

					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_RACEMAN_RANK,
						buttontype,
						CHAR_WINDOWTYPE_RACE_SHOWRANK11,
						CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
						listbuf	);
				}
				break;
			case WINDOW_BUTTONTYPE_OK:
				Raceman_SelectWindow( meindex, talkerindex,0,-1);	break;
			default:break;
			}
		break;
	case CHAR_WINDOWTYPE_RACE_SHOWRANK12:
		switch(select){
			case WINDOW_BUTTONTYPE_NEXT:
			case WINDOW_BUTTONTYPE_PREV:
				{
					int fd;
					fd = getfdFromCharaIndex( talkerindex );
					if( fd == -1 )  return;
					
					Pageindex += 10 * (( select == WINDOW_BUTTONTYPE_NEXT) ? 1 : -1);

					if(history2[Pageindex].catchcnt==0)  {
						Raceman_SelectWindow( meindex, talkerindex,0,-1);
						return;
					}
					if( Pageindex < 0 ) return; 

					buttontype = WINDOW_BUTTONTYPE_OK;
					if( history2[Pageindex+10].catchcnt==0)
						buttontype |= WINDOW_BUTTONTYPE_PREV;
					else if( Pageindex == 0)
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					else{
						buttontype |= WINDOW_BUTTONTYPE_PREV;
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					}
					//�������					
					strcpy(listbuf,"�y������v�嬰�ѤH�Ʀ�]�z "); 
					for (i=Pageindex;i<Pageindex+10;i++)
					{
						if(history2[i].rank == 0 || strlen(history2[i].name) == 0 || history2[i].catchcnt == 0) break;
						sprintf(tmp_buffer,"%d %s %d ",history2[i].rank,history2[i].name , history2[i].catchcnt);
						strcat(listbuf,tmp_buffer);
					}

					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_RACEMAN_RANK,
						buttontype,
						CHAR_WINDOWTYPE_RACE_SHOWRANK12,
						CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
						listbuf	);
				}
				break;
			case WINDOW_BUTTONTYPE_OK:
				Raceman_SelectWindow( meindex, talkerindex,0,-1);	break;
			default:break;
			}
		break;
	case CHAR_WINDOWTYPE_RACE_SHOWRANK13:
		switch(select){
			case WINDOW_BUTTONTYPE_NEXT:
			case WINDOW_BUTTONTYPE_PREV:
				{
					int fd;
					fd = getfdFromCharaIndex( talkerindex );
					if( fd == -1 )  return;
					
					Pageindex += 10 * (( select == WINDOW_BUTTONTYPE_NEXT) ? 1 : -1);

					if(history3[Pageindex].catchcnt==0)  {
						Raceman_SelectWindow( meindex, talkerindex,0,-1);
						return;
					}
					if( Pageindex < 0 ) return; 

					buttontype = WINDOW_BUTTONTYPE_OK;
					if( history3[Pageindex+10].catchcnt==0)
						buttontype |= WINDOW_BUTTONTYPE_PREV;
					else if( Pageindex == 0)
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					else{
						buttontype |= WINDOW_BUTTONTYPE_PREV;
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					}
					//�������
					strcpy(listbuf,"�y�a�ھ��v�嬰�ѤH�Ʀ�]�z "); 
					for (i=Pageindex;i<Pageindex+10;i++)
					{
						if(history3[i].rank == 0 || strlen(history3[i].name) == 0 || history3[i].catchcnt == 0) break;
						sprintf(tmp_buffer,"%d %s %d ",history3[i].rank,history3[i].name , history3[i].catchcnt);
						strcat(listbuf,tmp_buffer);
					}

					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_RACEMAN_RANK,
						buttontype,
						CHAR_WINDOWTYPE_RACE_SHOWRANK13,
						CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
						listbuf	);
				}
				break;
			case WINDOW_BUTTONTYPE_OK:
				Raceman_SelectWindow( meindex, talkerindex,0,-1);	break;
			default:break;
			}
		break;
	case CHAR_WINDOWTYPE_RACE_SHOWRANK14:
		switch(select){
			case WINDOW_BUTTONTYPE_NEXT:
			case WINDOW_BUTTONTYPE_PREV:
				{
					int fd;
					fd = getfdFromCharaIndex( talkerindex );
					if( fd == -1 )  return;
					
					Pageindex += 10 * (( select == WINDOW_BUTTONTYPE_NEXT) ? 1 : -1);

					if(history4[Pageindex].catchcnt==0)  {
						Raceman_SelectWindow( meindex, talkerindex,0,-1);
						return;
					}
					if( Pageindex < 0 ) return; 

					buttontype = WINDOW_BUTTONTYPE_OK;
					if( history4[Pageindex+10].catchcnt==0)
						buttontype |= WINDOW_BUTTONTYPE_PREV;
					else if( Pageindex == 0)
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					else{
						buttontype |= WINDOW_BUTTONTYPE_PREV;
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					}
					//�������					
					strcpy(listbuf,"�y������v�L�_�����Ʀ�]�z "); 
					for (i=Pageindex;i<Pageindex+10;i++)
					{
						if(history4[i].rank == 0 || strlen(history4[i].name) == 0 || history4[i].catchcnt == 0) break;
						sprintf(tmp_buffer,"%d %s %d ",history4[i].rank,history4[i].name , history4[i].catchcnt);
						strcat(listbuf,tmp_buffer);
					}

					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_RACEMAN_RANK,
						buttontype,
						CHAR_WINDOWTYPE_RACE_SHOWRANK14,
						CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
						listbuf	);
				}
				break;
			case WINDOW_BUTTONTYPE_OK:
				Raceman_SelectWindow( meindex, talkerindex,0,-1);	break;
			default:break;
			}
		break;
	case CHAR_WINDOWTYPE_RACE_SHOWRANK15:
		//�ثe�q����@��		
		switch(select){
			case WINDOW_BUTTONTYPE_NEXT:
			case WINDOW_BUTTONTYPE_PREV:
				{
					int fd;
					fd = getfdFromCharaIndex( talkerindex );
					if( fd == -1 )  return;
					
					Pageindex += 10 * (( select == WINDOW_BUTTONTYPE_NEXT) ? 1 : -1);

					if(history5[Pageindex].catchcnt==0)  {
						//Pageindex = Pageindex - 10;	�L�U�@���@�v�^����
						Raceman_SelectWindow( meindex, talkerindex,0,-1);
						return;
					}
					if( Pageindex < 0 ) return; //

					buttontype = WINDOW_BUTTONTYPE_OK;
					if( history5[Pageindex+10].catchcnt==0)
						buttontype |= WINDOW_BUTTONTYPE_PREV;
					else if( Pageindex == 0)
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					else{
						buttontype |= WINDOW_BUTTONTYPE_PREV;
						buttontype |= WINDOW_BUTTONTYPE_NEXT;
					}
					//�������
					strcpy(listbuf,"�y�a�ھ��v�L�_�����Ʀ�]�z "); 					
					for (i=Pageindex;i<Pageindex+10;i++)
					{						
						if(history5[i].rank == 0 || strlen(history5[i].name) == 0 || history5[i].catchcnt == 0) break;
						sprintf(tmp_buffer,"%d %s %d ",history5[i].rank,history5[i].name , history5[i].catchcnt);
						strcat(listbuf,tmp_buffer);
					}

					lssproto_WN_send( fd, WINDOW_MESSAGETYPE_RACEMAN_RANK,
						buttontype,
						CHAR_WINDOWTYPE_RACE_SHOWRANK15,
						CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
						listbuf	);
				}
				break;
			case WINDOW_BUTTONTYPE_OK:
				Raceman_SelectWindow( meindex, talkerindex,0,-1);	break;
			default:break;
		}
		break;
	case CHAR_WINDOWTYPE_RACE_QUIZ:
		ret = Raceman_CheckAnswer( meindex , talkerindex , data );
		if(ret == TRUE)		
			Raceman_SelectWindow( meindex, talkerindex,3,-1); //����F
		else
			Raceman_MsgDisp( meindex, talkerindex,94,-1);//�����F
		break;
	default: break;
		}
}


void Raceman_MsgDisp(int meindex,int talker,int num ,int select)
{
	char token[4096],tmp_buffer[4096];
	char buf[256],buf2[256],buf3[256];
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	int  fd = getfdFromCharaIndex( talker);
	int  buttontype=0,windowtype=0,windowno=0;
	int  i;

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {  //Ū�Ѽ�
		print("MsgDisp:GetArgStrErr");
		print("NPCName=%s\n", CHAR_getChar( meindex, CHAR_NAME));
		return ;
	}
	switch( num) {
		case 0:
			// �ťճB�Фŧ��
			strcpy( token, "                    ���y�d�j�|��\n\n");
				strcat( token, "                 �m �y�d�����D�� �n\n");
				strcat( token, "                  �m ����y�d�� �n\n");
				strcat( token, "                   �m �y�d�n�O �n\n");
				strcat( token, "                  �m �y�d�Ʀ�] �n\n");
				strcat( token, "                  �m �y�d�ԧQ�~ �n\n\n");
				strcat( token, "                     �m ���} �n\n");
			buttontype=WINDOW_BUTTONTYPE_NONE;
			windowtype=WINDOW_MESSAGETYPE_SELECT;
			windowno=CHAR_WINDOWTYPE_RACE_START; 
			break;

		case 1:	//�����D��
			strcpy( token, "                  ���y�d�j�|���i��\n\n");
			if( NPC_Util_GetStrFromStrWithDelim( argstr, "subject_msg",
														buf,sizeof(buf))!= NULL)
			{
				sprintf( buf2 ,"�����D�ءG%s\n\n",buf);
			}
			strcat( token , buf2 );
			if( NPC_Util_GetStrFromStrWithDelim( argstr, "time_msg", 
														buf, sizeof( buf)) != NULL){
				sprintf( buf2 ,"�ɶ�����G%s\n\n",buf);
			}
			strcat( token , buf2 );
			buttontype=WINDOW_BUTTONTYPE_OK;
			windowtype=WINDOW_MESSAGETYPE_MESSAGE;
			windowno=CHAR_WINDOWTYPE_RACE_SUBJECT; 
			break;

		case 2:	//���y�d��
			strcpy( token, "                  ���y�d�Үֵo�B��\n\n");
			NPC_Util_GetStrFromStrWithDelim( argstr, "EndFlag",buf, sizeof( buf) );
			if( strcmp(buf,"on")==0){   
				NPC_Util_GetStrFromStrWithDelim( argstr , "ending_msg", buf, sizeof( buf));
				strcat( token , buf );
				//strcat(token,"\n	���ɤw�g�����A�L�k�A����y�d�ҡI");   	
				windowno=CHAR_WINDOWTYPE_RACE_NORMAL; 	
			}else{
				if( NPC_Util_GetStrFromStrWithDelim( argstr, "card_msg",buf, sizeof( buf) ) != NULL)
				strcat( token, buf );
				windowno=CHAR_WINDOWTYPE_RACE_TICKET; 
			}
			buttontype=WINDOW_BUTTONTYPE_OK;
			windowtype=WINDOW_MESSAGETYPE_MESSAGE;			
			break;
		case 21://�D��w���T��
		  	if( NPC_Util_GetStrFromStrWithDelim( argstr, "itemfull_msg",buf, sizeof( buf) ) != NULL)
				strcpysafe(token, sizeof( buf), buf);
			buttontype=WINDOW_BUTTONTYPE_OK;
			windowtype=WINDOW_MESSAGETYPE_MESSAGE;
			windowno=CHAR_WINDOWTYPE_RACE_NORMAL; 
			break;
		case 22://�w�g���y�d�ҰT��
		  	if( NPC_Util_GetStrFromStrWithDelim( argstr, "haditem_msg",buf, sizeof( buf) ) != NULL)
				strcpysafe(token, sizeof( buf), buf);
			buttontype=WINDOW_BUTTONTYPE_OK;
			windowtype=WINDOW_MESSAGETYPE_MESSAGE;
			windowno=CHAR_WINDOWTYPE_RACE_NORMAL; 
			break;
		case 3:	//�y�d�n�O			
			strcpy( token, "                   ���y�d�n�O�B��\n\n");
			buttontype=WINDOW_BUTTONTYPE_CANCEL;
	  		windowtype=WINDOW_MESSAGETYPE_PETSELECT;
			windowno=CHAR_WINDOWTYPE_RACE_PETSELECT;
			break;
		case 31://�ˬd�d��
			//CHAR_getWorkInt( meindex , CHAR_WORKPETSELECT );
			if(Raceman_checkpet( meindex, talker , select , token ) == FALSE){    // false ��ܤw�n�O
				buttontype=WINDOW_BUTTONTYPE_OK;
		  		windowtype = WINDOW_MESSAGETYPE_MESSAGE;
				windowno=CHAR_WINDOWTYPE_RACE_NORMAL; 
			}else{
				//PetSelect = select-1;
				CHAR_setWorkInt( talker , CHAR_WORK_PETSELECT, select-1 );
				if( NPC_Util_GetStrFromStrWithDelim( argstr, "DelFlag",buf, sizeof( buf) ) != NULL){
					if( strcmp(buf,"on")==0)	strcat(token,"\n\n	�Ъ`�N�A�z������X�d���~��n�O��!");
				}
				buttontype = WINDOW_BUTTONTYPE_YESNO;
				windowtype = WINDOW_MESSAGETYPE_MESSAGE;
				windowno=CHAR_WINDOWTYPE_RACE_CHECKIN; 
			}
			break;
		case 32://�L�y�d�ҰT��
			if( NPC_Util_GetStrFromStrWithDelim( argstr, "nonitem_msg",buf, sizeof( buf) ) != NULL){
				strcpysafe(token, sizeof( buf), buf);
			}
			buttontype=WINDOW_BUTTONTYPE_OK;
			windowtype=WINDOW_MESSAGETYPE_MESSAGE;
			windowno=CHAR_WINDOWTYPE_RACE_NORMAL; 
			break;
		case 4:	//�y�d�Ʀ�]
			strcpy( token, "                  ���y�d�Ʀ�]��\n\n");
				NPC_Util_GetStrFromStrWithDelim( argstr, "EndFlag",buf, sizeof( buf) );
				if(strcmp(buf,"off")==0){
					NPC_Util_GetStrFromStrWithDelim( argstr, "HasGame",buf, sizeof( buf) );
					i = 1;
					while( getStringFromIndexWithDelim( buf , "," , i , buf2 , sizeof(buf2) )){						
						sprintf( buf3 , "             %s\n",rankname[atoi(buf2)-1]);		
						strcat( token , buf3 );
						i++ ;
					}
				}else{

					NPC_Util_GetStrFromStrWithDelim( argstr, "GameMode",buf, sizeof( buf) );
					sprintf( buf3 , "             %s\n",historyrank[atoi(buf)-1]);		
					strcat( token , buf3 );
				}
			buttontype=WINDOW_BUTTONTYPE_NONE;
			windowtype=WINDOW_MESSAGETYPE_SELECT;
			windowno=CHAR_WINDOWTYPE_RACE_RANK; 
			break;
		case 41: //�s�⪺���� ���w������ƶq
			token[0]='\0';				
			strcpy(token,"�y�s���嬰�ѤH�Ʀ�]�z ");
			for (i=0;i<10;i++)
			{
				if(ranktable1[i].rank == 0 || strlen(ranktable1[i].name) == 0 || ranktable1[i].catchcnt == 0) break;
				sprintf(tmp_buffer,"%d %s %d ",ranktable1[i].rank , ranktable1[i].name , ranktable1[i].catchcnt);
				strcat(token,tmp_buffer);
			}
			Pageindex = 0;  //�^��Ĥ@��
			buttontype=WINDOW_BUTTONTYPE_OK|WINDOW_BUTTONTYPE_NEXT;
			windowtype=WINDOW_MESSAGETYPE_RACEMAN_RANK;
			windowno=CHAR_WINDOWTYPE_RACE_SHOWRANK1;
			break;
		case 42://�ӤH�P�����ƦW 
			token[0]='\0';
			strcpy(token,"�y�����嬰�ѤH�Ʀ�]�z ");
			for (i=0;i<10;i++)
			{
				if(ranktable2[i].rank == 0 || strlen(ranktable2[i].name) == 0 || ranktable2[i].catchcnt == 0) break;
				sprintf(tmp_buffer,"%d %s %d ",ranktable2[i].rank , ranktable2[i].name , ranktable2[i].catchcnt);
				strcat(token,tmp_buffer);
			}
			Pageindex = 0;
			buttontype=WINDOW_BUTTONTYPE_OK|WINDOW_BUTTONTYPE_NEXT;
			windowtype=WINDOW_MESSAGETYPE_RACEMAN_RANK;
			windowno=CHAR_WINDOWTYPE_RACE_SHOWRANK2;
			break;
		case 43://�a�ڤ��P�����ƦW 
			token[0]='\0';
			strcpy(token,"�y�a���嬰�ѤH�Ʀ�]�z ");
			for (i=0;i<10;i++)
			{
				if(ranktable3[i].rank == 0 || strlen(ranktable3[i].name) == 0 || ranktable3[i].catchcnt == 0) break;
				sprintf(tmp_buffer,"%d %s %d ",ranktable3[i].rank , ranktable3[i].name , ranktable3[i].catchcnt);
				strcat(token,tmp_buffer);
			}
			Pageindex = 0;
			buttontype=WINDOW_BUTTONTYPE_OK|WINDOW_BUTTONTYPE_NEXT;
			windowtype=WINDOW_MESSAGETYPE_RACEMAN_RANK;
			windowno=CHAR_WINDOWTYPE_RACE_SHOWRANK3;
			break;
		case 44://�ӤH�P�����ƦW 
			token[0]='\0';
			strcpy(token,"�y����L�_�����Ʀ�]�z ");
			for (i=0;i<10;i++)
			{
				if(ranktable4[i].rank == 0 || strlen(ranktable4[i].name) == 0 || ranktable4[i].catchcnt == 0) break;
				sprintf(tmp_buffer,"%d %s %d ",ranktable4[i].rank , ranktable4[i].name , ranktable4[i].catchcnt);
				strcat(token,tmp_buffer);
			}
			Pageindex = 0;
			buttontype=WINDOW_BUTTONTYPE_OK|WINDOW_BUTTONTYPE_NEXT;
			windowtype=WINDOW_MESSAGETYPE_RACEMAN_RANK;
			windowno=CHAR_WINDOWTYPE_RACE_SHOWRANK4;
			break;
		case 45://�a�ڤ��P�����ƦW 
			token[0]='\0';
			strcpy(token,"�y�a�ڵL�_�����Ʀ�]�z ");
			for (i=0;i<10;i++)
			{
				if(ranktable5[i].rank == 0 || strlen(ranktable5[i].name) == 0 || ranktable5[i].catchcnt == 0) break;
				sprintf(tmp_buffer,"%d %s %d ",ranktable5[i].rank , ranktable5[i].name , ranktable5[i].catchcnt);
				strcat(token,tmp_buffer);
			}
			Pageindex = 0;
			buttontype=WINDOW_BUTTONTYPE_OK|WINDOW_BUTTONTYPE_NEXT;
			windowtype=WINDOW_MESSAGETYPE_RACEMAN_RANK;
			windowno=CHAR_WINDOWTYPE_RACE_SHOWRANK5;
			break;
		case 411:
			token[0]='\0';
			strcpy(token,"�y�s����v�嬰�ѤH�Ʀ�]�z ");
			for (i=0;i<10;i++)
			{
				if(history1[i].rank == 0 || strlen(history1[i].name) == 0 || history1[i].catchcnt == 0) break;
				sprintf(tmp_buffer,"%d %s %d ",history1[i].rank , history1[i].name , history1[i].catchcnt);
				strcat(token,tmp_buffer);
			}
			Pageindex = 0;
			buttontype=WINDOW_BUTTONTYPE_OK|WINDOW_BUTTONTYPE_NEXT;
			windowtype=WINDOW_MESSAGETYPE_RACEMAN_RANK;
			windowno=CHAR_WINDOWTYPE_RACE_SHOWRANK11;
			break;
		case 412:
			token[0]='\0';
			strcpy(token,"�y������v�嬰�ѤH�Ʀ�]�z ");
			for (i=0;i<10;i++)
			{
				if(history2[i].rank == 0 || strlen(history2[i].name) == 0 || history2[i].catchcnt == 0) break;
				sprintf(tmp_buffer,"%d %s %d ",history2[i].rank , history2[i].name , history2[i].catchcnt);
				strcat(token,tmp_buffer);
			}
			Pageindex = 0;
			buttontype=WINDOW_BUTTONTYPE_OK|WINDOW_BUTTONTYPE_NEXT;
			windowtype=WINDOW_MESSAGETYPE_RACEMAN_RANK;
			windowno=CHAR_WINDOWTYPE_RACE_SHOWRANK12;
			break;
		case 413:
			token[0]='\0';
			strcpy(token,"�y�a�ھ��v�嬰�ѤH�Ʀ�]�z ");
			for (i=0;i<10;i++)
			{
				if(history3[i].rank == 0 || strlen(history3[i].name) == 0 || history3[i].catchcnt == 0) break;
				sprintf(tmp_buffer,"%d %s %d ",history3[i].rank , history3[i].name , history3[i].catchcnt);
				strcat(token,tmp_buffer);
			}
			Pageindex = 0;
			buttontype=WINDOW_BUTTONTYPE_OK|WINDOW_BUTTONTYPE_NEXT;
			windowtype=WINDOW_MESSAGETYPE_RACEMAN_RANK;
			windowno=CHAR_WINDOWTYPE_RACE_SHOWRANK13;
			break;
		case 414:
			token[0]='\0';
			strcpy(token,"�y������v�L�_�����Ʀ�]�z ");
			for (i=0;i<10;i++)
			{
				if(history4[i].rank == 0 || strlen(history4[i].name) == 0 || history4[i].catchcnt == 0) break;
				sprintf(tmp_buffer,"%d %s %d ",history4[i].rank , history4[i].name , history4[i].catchcnt);
				strcat(token,tmp_buffer);
			}
			Pageindex = 0;
			buttontype=WINDOW_BUTTONTYPE_OK|WINDOW_BUTTONTYPE_NEXT;
			windowtype=WINDOW_MESSAGETYPE_RACEMAN_RANK;
			windowno=CHAR_WINDOWTYPE_RACE_SHOWRANK14;
			break;
		case 415:
			token[0]='\0';
			strcpy(token,"�y�a�ھ��v�L�_�����Ʀ�]�z ");
			for (i=0;i<10;i++)
			{
				if(history5[i].rank == 0 || strlen(history5[i].name) == 0 || history5[i].catchcnt == 0) break;
				sprintf(tmp_buffer,"%d %s %d ",history5[i].rank , history5[i].name , history5[i].catchcnt);
				strcat(token,tmp_buffer);
			}
			Pageindex = 0;
			buttontype=WINDOW_BUTTONTYPE_OK|WINDOW_BUTTONTYPE_NEXT;
			windowtype=WINDOW_MESSAGETYPE_RACEMAN_RANK;
			windowno=CHAR_WINDOWTYPE_RACE_SHOWRANK15;
			break;
		case 46:
			strcpy( token, "                   ���y�d�Ʀ�]��\n\n\n");
				strcat( token, "              �Ʀ�]�ثe�S�����\n");
			buttontype=WINDOW_BUTTONTYPE_OK;
			windowtype=WINDOW_MESSAGETYPE_MESSAGE;
			windowno=CHAR_WINDOWTYPE_RACE_NORMAL; 
			break;
		case 5:	//�y�d�ԧQ�~
			if(Raceman_checkprize( meindex , talker , token )==FALSE){
				windowno=CHAR_WINDOWTYPE_RACE_NORMAL; 
			}else{
				Raceman_renewrank( meindex , talker , Raceman_readmode( meindex , talker ) );   //Ū���ƦW
				windowno=CHAR_WINDOWTYPE_RACE_PRIZE; 
			}
			buttontype=WINDOW_BUTTONTYPE_OK;
			windowtype=WINDOW_MESSAGETYPE_MESSAGE;
			break;
		case 6: //�q���K�y
			Raceman_GetQuestion( meindex , talker , token );   //�n�B�z�H��Ū�X�D�� �M�q�X�D��
			buttontype = WINDOW_BUTTONTYPE_OK;
			windowtype = WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT;
			windowno = CHAR_WINDOWTYPE_RACE_QUIZ;
			break;
		case 9:	// �a���� �D�ڪ��T��
			if( NPC_Util_GetStrFromStrWithDelim( argstr, "fmleader_msg",buf, sizeof( buf) ) != NULL){
				strcpysafe(token, sizeof( buf), buf);
			}
			buttontype=WINDOW_BUTTONTYPE_OK;
			windowtype=WINDOW_MESSAGETYPE_MESSAGE;
			windowno=CHAR_WINDOWTYPE_RACE_NORMAL;
			break;
		case 90:	
			if( NPC_Util_GetStrFromStrWithDelim( argstr, "notnew_msg",buf, sizeof( buf) ) != NULL){
				strcpysafe(token, sizeof( buf), buf);
			}
			buttontype=WINDOW_BUTTONTYPE_OK;
			windowtype=WINDOW_MESSAGETYPE_MESSAGE;
			windowno=CHAR_WINDOWTYPE_RACE_NORMAL;
			break;
		case 91:	
			if( NPC_Util_GetStrFromStrWithDelim( argstr, "notnew_msg",buf, sizeof( buf) ) != NULL){
				strcpysafe(token, sizeof( buf), buf);
			}
			buttontype=WINDOW_BUTTONTYPE_OK;
			windowtype=WINDOW_MESSAGETYPE_MESSAGE;
			windowno=CHAR_WINDOWTYPE_RACE_NORMAL;
			break;
		case 92:
			strcpy(token,"�z���a���٨S����������...");
			buttontype=WINDOW_BUTTONTYPE_OK;
			windowtype=WINDOW_MESSAGETYPE_MESSAGE;
			windowno=CHAR_WINDOWTYPE_RACE_NORMAL;
			break;
		case 93:	
			strcpy(token,"�ثe�n�O�H�ƹL�h�еy��A��...");
			buttontype=WINDOW_BUTTONTYPE_OK;
			windowtype=WINDOW_MESSAGETYPE_MESSAGE;
			windowno=CHAR_WINDOWTYPE_RACE_NORMAL;
			break;
		case 94:				
			if( NPC_Util_GetStrFromStrWithDelim( argstr, "wrongans_msg",buf, sizeof( buf) ) != NULL){
				strcpysafe(token, sizeof( buf), buf);
			}
			buttontype=WINDOW_BUTTONTYPE_OK;
			windowtype=WINDOW_MESSAGETYPE_MESSAGE;
			windowno=CHAR_WINDOWTYPE_RACE_NORMAL;
			break;
	}


	lssproto_WN_send( fd, windowtype, 
				buttontype, 
				windowno,
				CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
				token);

}

//return 0: OK  1:�ڪ��������X 2:�s�ⵥ�Ť��X 3:�s���ਭ���X 4:�D�����a��
int Raceman_CheckGameType(int meindex , int talker )
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buf[64];
	int  gmode;

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {  //Ū�Ѽ�
		print("MsgDisp:GetArgStrErr");
		print("NPCName=%s\n", CHAR_getChar( meindex, CHAR_NAME));
		return FALSE;  
	}

	NPC_Util_GetStrFromStrWithDelim( argstr, "GameMode", buf, sizeof( buf) ); 
	gmode = atoi(buf);
	if( gmode == 3 || gmode == 5){	//�a����
		if (CHAR_getWorkInt(talker, CHAR_WORKFMSETUPFLAG)!=1)	
			return 4;
		if (CHAR_getInt(talker,CHAR_FMLEADERFLAG)!=FMMEMBER_LEADER)   //�O�ڪ���
			return 1;
	}
	if( gmode == 1 ){  //�ˬd�s�H����
		NPC_Util_GetStrFromStrWithDelim( argstr, "FornewLv", buf, sizeof( buf) ); 
		//sprintf(buuf,"fornewlv:%s   talkerLV:%d", buf , CHAR_getInt(  talker , CHAR_LV));
		//CHAR_talkToCli( talker, -1,buuf, CHAR_COLORYELLOW);
		if(CHAR_getInt(  talker , CHAR_LV) > atoi(buf) )
			return 2;

		NPC_Util_GetStrFromStrWithDelim( argstr, "FornewTran", buf, sizeof( buf) );
		//sprintf(buuf,"fornewTran:%s   talkerLV:%d", buf , CHAR_getInt( talker , CHAR_TRANSMIGRATION) );
		//CHAR_talkToCli( talker, -1,buuf, CHAR_COLORYELLOW);
		if(CHAR_getInt( talker , CHAR_TRANSMIGRATION) > atoi(buf) )
			return 3;
	}
	return 0;   
}
//�ˬd���L�y�d��
BOOL Raceman_CheckItem(int meindex,int talker)
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	int item,i,itemindex,id;
	char buf[128];

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {  //Ū�Ѽ�
		print("MsgDisp:GetArgStrErr");
		print("NPCName=%s\n", CHAR_getChar( meindex, CHAR_NAME));
		return FALSE;  
	}

  	if( NPC_Util_GetStrFromStrWithDelim( argstr, "CheckItem",
											 buf, sizeof( buf) ) != NULL)
	{
		item = atoi(buf);
		for( i = 0 ; i < CHAR_MAXITEMHAVE ; i++ ) {

			itemindex=CHAR_getItemIndex( talker , i );
			if( ITEM_CHECKINDEX( itemindex) ) {
				id=ITEM_getInt( itemindex ,ITEM_ID );
				if(id==item) return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL Raceman_ItemFullCheck(int meindex, int talker)
{
	int emptyitemindexinchara;
		
	emptyitemindexinchara = CHAR_findEmptyItemBox( talker );

	if( emptyitemindexinchara < 0 ){
		return FALSE;
	}

	return TRUE;
}

//�o���y�d��
BOOL Raceman_EventAddItem( int meindex,int talker )
{
	int itemindex,ret,i,fd;
	char token[256],buf[128],buf1[256];
	char id[16],name[64];	
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	int  gmode;
	unsigned int askno,asklv,temp=0;

	if(Raceman_ItemFullCheck( meindex, talker) == FALSE){
		Raceman_MsgDisp( meindex, talker, 21,-1);
		return FALSE;
	}

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {  //Ū�Ѽ�
		print("MsgDisp:GetArgStrErr");
		print("NPCName=%s\n", CHAR_getChar( meindex, CHAR_NAME));
		return FALSE;  
	}

	if( NPC_Util_GetStrFromStrWithDelim( argstr , "GetItem", buf, sizeof( buf)) == NULL ){		
		print("raceman: GetItem Err");
		return FALSE;
	}

	itemindex = ITEM_makeItemAndRegist( atoi( buf ));
	if(itemindex == -1){
		print("raceman: itemindex Err");
		return FALSE;
	}

	ret = CHAR_addItemSpecificItemIndex( talker, itemindex);
	if( ret < 0 || ret >= CHAR_MAXITEMHAVE ) {
		print( "npc_raceman.c: ACCEPTadditem error itemindex[%d]\n", itemindex);
		ITEM_endExistItemsOne( itemindex);
		return FALSE;
	}

	NPC_Util_GetStrFromStrWithDelim( argstr , "GameMode", buf1, sizeof( buf1));		
	gmode = atoi(buf1);

	if( CHAR_getInt( talker, CHAR_CATCHCNT1+gmode-1 )!=0){
		
		for(i=0;i<10;i++)	CHAR_setInt( talker, CHAR_KINDCNT1 + i , 0 );	
		CHAR_setInt( talker,CHAR_CATCHCNT1+gmode-1, 0 );  //�M���O��

		if(gmode==3||gmode==5){  //�a����
			strcpy(name,CHAR_getChar(talker,CHAR_FMNAME));			//�a�ڦW��
			sprintf(id,"%d",CHAR_getWorkInt(talker, CHAR_WORKFMINDEXI));	//�a��id
		}else{	//�ӤH��
			fd = getfdFromCharaIndex(talker);
			CONNECT_getCharname( fd, name, sizeof( name ) );  //�H���W��
			CONNECT_getCdkey( fd, id , sizeof( id ) );		  //�b��		
		}

		switch(gmode)
		{
		case 1:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode1", buf1, sizeof( buf1)); break;
		case 2:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode2", buf1, sizeof( buf1)); break;
		case 3:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode3", buf1, sizeof( buf1)); break;
		case 4:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode4", buf1, sizeof( buf1)); break;
		case 5:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode5", buf1, sizeof( buf1)); break;
		}
		saacproto_ACRaceRecordandSort_send( acfd , talker , buf1 , id , name,9,0,0);
	}
	if(gmode < 4 )	//���w����
		sprintf( buf1, "�ثe�y�d�O��%d�u�C", CHAR_getInt( talker,CHAR_CATCHCNT1+gmode-1 ) );
	else{	
		//random�@�U�D�ص��L
		askno = rand() % asktotal; 
		asklv = asktable[askno].lowlv + rand()%((asktable[askno].highlv+1)-asktable[askno].lowlv);
		temp = (askno << 22) & 0x7FC00000;
		temp = temp | ((asklv << 14) & 0x7FFFC000);
		temp = temp | ((asktable[askno].bbi - 100000) & 0x00003FFF);
		CHAR_setInt(  talker , CHAR_CHECKIN , temp );  		

		//sprintf(buf1,"checkin:%d askno:%d askname:%s asklv%d askbbi%d",
		//	CHAR_getInt( talker ,CHAR_CHECKIN),askno,asktable[askno].petname,asklv,asktable[askno].bbi);		
		//CHAR_talkToCli( talker, -1,buf1, CHAR_COLORYELLOW);
		if( asktable[askno].highlv == 0 )
			sprintf( buf1,"�Ч쵹�ڵ���%d�y�H�W�z��%s�n�ܡC�z�ثe�O��%d��", asktable[askno].lowlv , asktable[askno].petname, CHAR_getInt( talker,CHAR_CATCHCNT1+gmode-1 ) );
		else
			sprintf( buf1,"�Ч쵹�ڡy�u��z����%d��%s�n�ܡC�z�ثe�O��%d��", asklv , asktable[askno].petname, CHAR_getInt( talker,CHAR_CATCHCNT1+gmode-1 ) );
	}
	ITEM_setChar( itemindex, ITEM_EFFECTSTRING, buf1);
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

	return TRUE;
}

// �ˬd�O�_�����ɪ��d���M�O�_�n�O�L           
BOOL Raceman_checkpet( int meindex, int talker , int select , char *token )
{
	int	 petindex,checkpetID,petimgnum;
	char *petname;
	char buf[256];
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	int  index,gmode;
	unsigned int  askno,asklv,askbbi;

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {  //Ū�Ѽ�
		print("MsgDisp:GetArgStrErr");
		print("NPCName=%s\n", CHAR_getChar( meindex, CHAR_NAME));
		return FALSE;
	}

	NPC_Util_GetStrFromStrWithDelim( argstr , "EndFlag", buf, sizeof( buf));
	if( strcmp(buf,"on")==0){   
		NPC_Util_GetStrFromStrWithDelim( argstr , "ending_msg", buf, sizeof( buf));
		sprintf( token , "\n	%s",buf);		
		return FALSE;
	}

	if(CHAR_getInt( talker,CHAR_CATCHCNT5 ) >= asktotal ){
		strcpy( token ,"���ߧA�w�g�F���y�d�j�ɲ׷��ؼСA�P�±z���ѻP�ï��z�⤤�j��");
		return FALSE;
	}

	petindex = CHAR_getCharPet( talker, select-1);
	petname = CHAR_getUseName( petindex);
	if( !CHAR_CHECKINDEX(petindex) ) return FALSE;
  
	if (CHAR_getInt(petindex, CHAR_PETFAMILY) == 1){
		strcpy(token,"\n	�a�ڦu�@�~�L�k�n�O�I");   	
    	return	FALSE;
    }

    if (CHAR_getInt( talker , CHAR_RIDEPET) == select-1 ){
		strcpy(token,"\n	�M�������d���L�k�n�O�I");	
    	return	FALSE;
    }    

	NPC_Util_GetStrFromStrWithDelim( argstr , "GameCode", buf, sizeof( buf));
	if( CHAR_getInt(petindex,CHAR_CHECKIN) == atoi(buf) ){   //�o�����ɤw�n�O�L�F
		strcpy(token,"\n	�o�u�d���w�g�b�������ɵn�O�L�F�@!");
		return  FALSE;
	}

	NPC_Util_GetStrFromStrWithDelim( argstr , "GameMode", buf, sizeof( buf));
	gmode = atoi(buf);
	if(gmode==4||gmode==5)  //��������
	{			
		petimgnum = CHAR_getInt( petindex , CHAR_BASEBASEIMAGENUMBER); 
		askbbi = CHAR_getInt( talker ,CHAR_CHECKIN) & 0x00003FFF;
		asklv = ( CHAR_getInt( talker ,CHAR_CHECKIN ) & 0x003FC000 ) >> 14 ;
		askno = ( CHAR_getInt( talker ,CHAR_CHECKIN ) & 0x7FC00000 ) >> 22 ;

		//sprintf(buf,"checkin:%d   petimgnum:%d askbbi:%d  petname:%s askname:%s petlv%d asklv%d",
		//	CHAR_getInt( talker ,CHAR_CHECKIN),petimgnum,askbbi,petname,asktable[askno].petname,
		//	CHAR_getInt(petindex , CHAR_LV),asklv);
		//CHAR_talkToCli( talker, -1,buf, CHAR_COLORYELLOW);

		if( petimgnum != askbbi+100000 ){
			strcpy(token,"\n	�нT�{�d���A�o�u���O�ڭn���@!");
			return FALSE;
		}
		if( strcmp(petname,asktable[askno].petname)!=0) {
			strcpy(token,"\n	�нT�{�d���A�o�u���O�ڭn���@!");
			return FALSE;		
		}
		if( asktable[askno].highlv == 0 ){
			if( CHAR_getInt(petindex , CHAR_LV) < asktable[askno].lowlv ){
				strcpy(token,"\n	�o�u�d�����Ť����@�I");   	
				return  FALSE;
			}
		}else{
			if( CHAR_getInt(petindex , CHAR_LV) != asklv ){
				strcpy(token,"\n	�нT�{�d�����šA���Ť����@!");
				return FALSE;		
			}
		}
		
		sprintf(token,"		�סס�         �d���n�O         �סס�"
						"\n\n\n �z��ܵn�O���d���O    %s Lv:%d"
					,petname,CHAR_getInt( petindex, CHAR_LV));					
		
		/*
		NPC_Util_GetStrFromStrWithDelim( argstr , "PetLevel", buf, sizeof( buf));
		if (CHAR_getInt(petindex , CHAR_LV) < atoi(buf)){
			strcpy(token,"\n	�o�u�d�����Ť����I");   	
			return  FALSE;
		}

		petimgnum = CHAR_getInt( petindex , CHAR_BASEBASEIMAGENUMBER); 
		for(petNo=0;petNo<640;petNo++){
			if(	 petimgnum == petflgtable[petNo] )  // ���ӭn�@�w�btable��
				break;
		}
		if( petNo >= 640 ){
			//sprintf(buuf2,"petimgnum:%d   pftable:%d   petNo:%d", petimgnum , petflgtable[2] , petNo);
			//CHAR_talkToCli( talker, -1,buuf2, CHAR_COLORYELLOW);
			strcpy(token,"\n	�o�u�d���ǩǪ���!");
			//log
			LogPet( CHAR_getChar( talker, CHAR_NAME ),
					CHAR_getChar( talker, CHAR_CDKEY ),
					CHAR_getChar( petindex, CHAR_NAME),
					CHAR_getInt( petindex, CHAR_LV),
					"raceman(�y�d�j�|�D�k�d�O��)",
					CHAR_getInt( talker,CHAR_FLOOR),
					CHAR_getInt( talker,CHAR_X ),
					CHAR_getInt( talker,CHAR_Y ),
					CHAR_getChar( petindex, CHAR_UNIQUECODE)   
					);

			return FALSE;// �X���Dù�A���a������֦����d���ӵn�O 
		}

		//sprintf(buuf2,"petimgnum:%d   pftable:%d   petNo:%d", petimgnum , petflgtable[petNo] , petNo);
		//CHAR_talkToCli( talker, -1,buuf2, CHAR_COLORYELLOW);

		base = 1;
		intNo = petNo / 32 ;	
		shift = petNo % 32 ;
		base  = base << shift;

		//sprintf(buuf2,"intNo:%d   shift:%d   base:%d   data:%d", intNo , shift , base , CHAR_getInt( talker , CHAR_KINDCNT1 + intNo ));
		//CHAR_talkToCli( talker, -1,buuf2, CHAR_COLORYELLOW);

		if( CHAR_getInt( talker , CHAR_KINDCNT1 + intNo ) & base ){  
			NPC_Util_GetStrFromStrWithDelim( argstr , "repeat_msg", buf, sizeof( buf));
			sprintf(token,"\n	 %s",buf);
			return FALSE; 
		}else{
			sprintf(token,"		�סס�         �d���n�O         �סס�"
							"\n\n\n �z��ܵn�O���d���O    %s Lv:%d"
							,petname,CHAR_getInt( petindex, CHAR_LV)
							);		
		}
		*/
	}
	else
	{		
		if(NPC_Util_GetStrFromStrWithDelim( argstr , "CheckPet", buf, sizeof( buf)) != NULL ){
			checkpetID = atoi(buf);
			if( !ENEMY_CHECKINDEX( checkpetID )) return FALSE;

	//		sprintf(buuf2,"petindex:%d   checkpetID:%d  index:%d",petindex,checkpetID,index);
	//		CHAR_talkToCli( talker, -1,buuf2, CHAR_COLORYELLOW);

			/*�O�dgm���Odropmypet���k  ���G�P�U�q
			petarray = ENEMY_getEnemyArrayFromId( checkpetID );
			petindex2 = ENEMY_TEST_createPetIndex( petarray );
			test5  = CHAR_getInt( petindex2 , CHAR_BASEBASEIMAGENUMBER);
			*/
			
			index = ENEMYTEMP_getEnemyTempArray(ENEMY_getEnemyArrayFromId(checkpetID));
			if( !ENEMYTEMP_CHECKINDEX( index )) return FALSE;
			petimgnum  = ENEMYTEMP_getInt(index,E_T_IMGNUMBER); //GET ENEMYTEMP_enemy[index].intdata[E_T_IMGNUMBER];

			//sprintf(buuf,"check:%d   be_check:%d",petimgnum, CHAR_getInt( petindex , CHAR_BASEBASEIMAGENUMBER));           //petarry:%d   petindex2:%d  test5:%d",test1,test2,petarray,petindex2,test5);
			//CHAR_talkToCli( talker, -1,buuf, CHAR_COLORYELLOW);
		
			if(	CHAR_getInt( petindex , CHAR_BASEBASEIMAGENUMBER) == petimgnum ){

				NPC_Util_GetStrFromStrWithDelim( argstr , "PetLevel", buf, sizeof( buf));
				if (CHAR_getInt(petindex , CHAR_LV) < atoi(buf)){
					strcpy(token,"\n	�o�u�d�����Ť����I");   	
					return  FALSE;
				}

				sprintf(token,"		�סס�         �d���n�O         �סס�"
								"\n\n\n �z��ܵn�O���d���O    %s Lv:%d"
								,petname,CHAR_getInt( petindex, CHAR_LV)
								);		
			}else{
				strcpy(token,"\n	���ɪ��D�بS���o�u�d���@!");
				return FALSE;
			}
		}
	}
	return TRUE;	
}

int Raceman_getokask( int talker, int random )
{
	int base,i,shift;

	base = random / 32 ;
	shift = random % 32 ;
	i = 1 << shift ;
	if(	CHAR_getInt( talker , CHAR_KINDCNT1 + base ) & i ){	//���u��ܸѹL�F
		random ++;
		if( random >= asktotal ) random = 0;
		return Raceman_getokask( talker , random );
	}else
		return random;	
}
		
BOOL  Raceman_petcheckin( int meindex , int talker )
{
	int  petindex,fd,item,i,itemindex,itemid,gmode;
	char *petname;
	char id[16],name[64];
	char buf[64],buf1[64]; //buuf[128];
	char category[12];
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	int  base,intNo,shift,petNo,j;	
	unsigned int  askno,asklv,temp;
	
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {  //Ū�Ѽ�
		print("MsgDisp:GetArgStrErr");
		print("NPCName=%s\n", CHAR_getChar( meindex, CHAR_NAME));
		return FALSE;
	}
	
	//�O�I�ˬd
	for( i = 0 ; i < 5 ; i ++ ){
		j = CHAR_getInt( talker,CHAR_CATCHCNT1 + i ) ;
		if( j++ >= 0xFFFFFF ) {
			CHAR_talkToCli( talker, -1, "�ܩ�p�A�z�w�g�F��t�ΤW���A�L�k�A�n�O�F", CHAR_COLORYELLOW);
			return FALSE;
		}
	}
	
	petindex = CHAR_getCharPet( talker, CHAR_getWorkInt( talker , CHAR_WORK_PETSELECT ) );
	if( !CHAR_CHECKINDEX(petindex) ) return FALSE;
	petname = CHAR_getUseName( petindex);

	//�g�J�d�����
	NPC_Util_GetStrFromStrWithDelim( argstr , "GameCode", buf, sizeof( buf));		
	CHAR_setInt( petindex, CHAR_CHECKIN , atoi(buf) );  
	LogPet( CHAR_getChar( talker, CHAR_NAME ),
			CHAR_getChar( talker, CHAR_CDKEY ),
			CHAR_getChar( petindex, CHAR_NAME),
			CHAR_getInt( petindex, CHAR_LV),
			"raceman(�y�d�j�|���d�ɰO��)",
			CHAR_getInt( talker,CHAR_FLOOR),
			CHAR_getInt( talker,CHAR_X ),
			CHAR_getInt( talker,CHAR_Y ),
			CHAR_getChar( petindex, CHAR_UNIQUECODE)   
			);

	//���d
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "DelFlag",buf, sizeof( buf) ) != NULL){
		if( strcmp(buf,"on")==0){
			CHAR_endCharOneArray( petindex );
			CHAR_setCharPet( talker ,  CHAR_getWorkInt( talker , CHAR_WORK_PETSELECT ) , -1);
			snprintf( category, sizeof( category), "K%d", CHAR_getWorkInt( talker , CHAR_WORK_PETSELECT ));
			CHAR_sendStatusString( talker , category );
			snprintf( category, sizeof( category), "W%d",CHAR_getWorkInt( talker , CHAR_WORK_PETSELECT ));
			CHAR_sendStatusString( talker , category );
			//log
			LogPet( CHAR_getChar( talker, CHAR_NAME ),
					CHAR_getChar( talker, CHAR_CDKEY ),
					CHAR_getChar( petindex, CHAR_NAME),
					CHAR_getInt( petindex, CHAR_LV),
					"raceman(�y�d�j�|���d�ɰO��)",
					CHAR_getInt( talker,CHAR_FLOOR),
					CHAR_getInt( talker,CHAR_X ),
					CHAR_getInt( talker,CHAR_Y ),
					CHAR_getChar( petindex, CHAR_UNIQUECODE)   
					);
		}
	}	

	//�����w�����٬O���P����
	NPC_Util_GetStrFromStrWithDelim( argstr , "GameMode", buf, sizeof( buf));		
	gmode = atoi(buf);
	if(gmode==4||gmode==5)  //���P����
	{
		petNo = ( CHAR_getInt( talker ,CHAR_CHECKIN ) & 0x7FC00000 ) >> 22 ;	
		//petNo -= 1; 
		intNo = petNo / 32 ;	
		shift = petNo % 32 ;
		base  = 1 << shift;
		CHAR_setInt( talker , CHAR_KINDCNT1 + intNo , CHAR_getInt( talker , CHAR_KINDCNT1 + intNo ) | base );
		//sprintf(buuf,"chkin data:%d", CHAR_getInt( talker , CHAR_KINDCNT1 + intNo ) );
		//CHAR_talkToCli( talker, -1,buuf, CHAR_COLORYELLOW);

		//�����ӤH�٬O�a����
		if(gmode == 5) //�]�w���a����
		{
			CHAR_setInt( talker,CHAR_CATCHCNT5,CHAR_getInt( talker,CHAR_CATCHCNT5 ) +1 );
			strcpy(name,CHAR_getChar(talker,CHAR_FMNAME));			//�a�ڦW��
			sprintf(id,"%d",CHAR_getWorkInt(talker, CHAR_WORKFMINDEXI));	//�a��id
		}
		else	// 4
		{
			//���a���d���ƥ[1
			fd = getfdFromCharaIndex(talker);
			CHAR_setInt( talker,CHAR_CATCHCNT4,CHAR_getInt( talker,CHAR_CATCHCNT4 ) +1 );
			CONNECT_getCharname( fd, name, sizeof( name ) );  //�H���W��
			CONNECT_getCdkey( fd, id , sizeof( id ) );		  //�b��		
		}
	}
	else	// 1 2 3
	{
		//�����ӤH�٬O�a����
		if(gmode == 3) //�]�w���a����
		{
			//�ڪ����ƥ[1
			CHAR_setInt( talker,CHAR_CATCHCNT3,CHAR_getInt( talker,CHAR_CATCHCNT3 ) +1 );
			strcpy(name,CHAR_getChar(talker,CHAR_FMNAME));			//�a�ڦW��
			sprintf(id,"%d",CHAR_getWorkInt(talker, CHAR_WORKFMINDEXI));	//�a��id
		}
		else
		{
			//���a���d���ƥ[1
			fd = getfdFromCharaIndex(talker);
			CONNECT_getCharname( fd, name, sizeof( name ) );  //�H���W��
			CONNECT_getCdkey( fd, id , sizeof( id ) );		  //�b��		
			if(gmode == 1 )
				CHAR_setInt( talker,CHAR_CATCHCNT1,CHAR_getInt( talker,CHAR_CATCHCNT1 ) +1 );
			else
				CHAR_setInt( talker,CHAR_CATCHCNT2,CHAR_getInt( talker,CHAR_CATCHCNT2 ) +1 );
		}
	}

	//�P�_�{�b�O�񤰻�
	switch(gmode)
	{
	case 1:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode1", buf, sizeof( buf)); break;
	case 2:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode2", buf, sizeof( buf)); break;
	case 3:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode3", buf, sizeof( buf)); break;
	case 4:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode4", buf, sizeof( buf)); break;
	case 5:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode5", buf, sizeof( buf)); break;
	}
	//ac���@���O����ƦW (fd,charaindex,id,name,limittime,racetype,catchcnt,ranknum)
	//sprintf(buuf,"acfd:%d talker:%d mode:%s id:%s name:%s catchcnt:%d",acfd,talker,buf,id,name,CHAR_getInt( talker,CHAR_CATCHCNT ) );
	//CHAR_talkToCli( talker, -1, buuf, CHAR_COLORYELLOW);
	if(gmode == 3 || gmode == 5){
		saacproto_ACRaceRecordfmdo_send( acfd , talker , CHAR_getInt( talker, CHAR_FMINDEX )
			, CHAR_getInt( petindex, CHAR_BASEBASEIMAGENUMBER) , CHAR_getChar( petindex, CHAR_UNIQUECODE ) , CHAR_getChar( petindex, CHAR_NAME) );
		
		//sprintf(buuf,"acfd:%d talker:%d fmid:%d bbi:%d unicode:%s petn:%s",acfd,talker,CHAR_getInt( talker, CHAR_FMINDEX )
		//	,CHAR_getInt( petindex, CHAR_BASEBASEIMAGENUMBER),CHAR_getChar( petindex, CHAR_UNIQUECODE ),CHAR_getChar( petindex, CHAR_NAME));
		//CHAR_talkToCli( talker, -1, buuf, CHAR_COLORYELLOW);
	}
	saacproto_ACRaceRecordandSort_send( acfd , talker , buf , id , name ,
		0 , CHAR_getInt( talker,CHAR_CATCHCNT1+gmode-1 ) , 0 ) ;
	

	//�y�d�Ҧ��ƥ[1 �� �M�w�U�@���D��
  	if( NPC_Util_GetStrFromStrWithDelim( argstr, "CheckItem",
											 buf, sizeof( buf) ) != NULL)
	{
		item = atoi(buf);
		for( i = 0 ; i < CHAR_MAXITEMHAVE ; i++ ) {
			itemindex=CHAR_getItemIndex( talker , i );
			if( ITEM_CHECKINDEX( itemindex) ) {
				itemid=ITEM_getInt( itemindex ,ITEM_ID );
				//print("itemindex:%d  itemid:%d  item:%d\n",itemindex,itemid,item);
				if(itemid==item){	//����y�d��
					if(gmode < 4)
						sprintf( buf1, "�ثe�y�d�O��%d�u�C", CHAR_getInt( talker,CHAR_CATCHCNT1+gmode-1 ) );
					else{
						if(CHAR_getInt( talker,CHAR_CATCHCNT5 ) >= asktotal ){
							sprintf( buf1,"���ߧA�F���y�d�j�ɲ׷��ؼСA���z�⤤�j��");
						}else{
							askno = Raceman_getokask( talker , rand() % asktotal );
							asklv = asktable[askno].lowlv + rand()%((asktable[askno].highlv+1)-asktable[askno].lowlv);
							temp = (askno << 22) & 0x7FC00000;
							temp = temp | ((asklv << 14) & 0x003FC000);
							temp = temp | ((asktable[askno].bbi - 100000) & 0x00003FFF);
							CHAR_setInt(  talker , CHAR_CHECKIN , temp );  		
							if( asktable[askno].highlv == 0 )
								sprintf( buf1,"�Ч쵹�ڵ���%d�y�H�W�z��%s�n�ܡC�z�ثe�O��%d��", asktable[askno].lowlv , asktable[askno].petname, CHAR_getInt( talker,CHAR_CATCHCNT1+gmode-1 ) );
							else
								sprintf( buf1,"�Ч쵹�ڡy�u��z����%d��%s�n�ܡC�z�ثe�O��%d��", asklv , asktable[askno].petname, CHAR_getInt( talker,CHAR_CATCHCNT1+gmode-1 ) );
						}
					}
					ITEM_setChar( itemindex, ITEM_EFFECTSTRING, buf1);
					CHAR_sendItemDataOne( talker , i );
					break;
				}					
			}
		}
	}

	sprintf( buf,"%sLv:%d�n�O�����F�A�[�o�[�o�C",petname,CHAR_getInt( petindex, CHAR_LV ));
	CHAR_talkToCli( talker, -1, buf , CHAR_COLORWHITE);

	return TRUE;
}

//Ū��bsaac���ƦW���,�ӥB�n�O�ثe���ɪ�����
void Raceman_countrank( int meindex, int talker )
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	int  ranknum = 0,gmode;
	char buf[64],buf1[64];

	if((argstr= NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {  //Ū�Ѽ�
		print("MsgDisp:GetArgStrErr");
		print("NPCName=%s\n", CHAR_getChar( meindex, CHAR_NAME));
		return;
	}

	if(NPC_Util_GetStrFromStrWithDelim( argstr , "RankNum", buf1, sizeof( buf1)) != NULL ){	
		ranknum = atoi(buf1);
	}
	
	NPC_Util_GetStrFromStrWithDelim( argstr , "GameMode", buf, sizeof( buf));		
	gmode = atoi(buf);

	switch(gmode)
	{
	case 1:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode1", buf, sizeof( buf)); break;
	case 2:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode2", buf, sizeof( buf)); break;
	case 3:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode3", buf, sizeof( buf)); break;
	case 4:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode4", buf, sizeof( buf)); break;
	case 5:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode5", buf, sizeof( buf)); break;
	}

	saacproto_ACRaceRecordandSort_send( acfd,talker,buf,"","",gmode,0,ranknum );

}

void Raceman_renewrank( int meindex , int talker , int select )
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	int  ranknum = 0;
	char buf[64],buf1[64];

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {  //Ū�Ѽ�
		print("MsgDisp:GetArgStrErr");
		print("NPCName=%s\n", CHAR_getChar( meindex, CHAR_NAME));
		return;
	}

	if(NPC_Util_GetStrFromStrWithDelim( argstr , "RankNum", buf1, sizeof( buf1)) != NULL ){	
		ranknum = atoi(buf1);
	}

	if(NPC_Util_GetStrFromStrWithDelim( argstr , "HasGame", buf, sizeof( buf)) != NULL ){
		getStringFromIndexWithDelim( buf , "," , select , buf1 , sizeof(buf1) );		
		select = atoi(buf1);
		switch(select)
		{
		case 1:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode1", buf, sizeof( buf)); break;
		case 2:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode2", buf, sizeof( buf)); break;
		case 3:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode3", buf, sizeof( buf)); break;
		case 4:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode4", buf, sizeof( buf)); break;
		case 5:NPC_Util_GetStrFromStrWithDelim( argstr , "Mode5", buf, sizeof( buf)); break;
		default: return;
		}
		saacproto_ACRaceRecordandSort_send( acfd,talker,buf,"","",select,0,ranknum );
	}
}

//�ˬd�O�_������� 
BOOL Raceman_checkprize( int meindex , int talker ,char *token)
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buf[256],buf1[64];
	int  lowlevel;//�̧C�з�  �U��	
	int  gmode;

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {  //Ū�Ѽ�
		print("MsgDisp:GetArgStrErr");
		print("NPCName=%s\n", CHAR_getChar( meindex, CHAR_NAME));
		return FALSE;
	}

	NPC_Util_GetStrFromStrWithDelim( argstr , "EndFlag", buf1, sizeof( buf1));
	if( strcmp(buf1,"off")==0){   // ���ɨS���� �ҥH�����
		NPC_Util_GetStrFromStrWithDelim( argstr , "notend_msg", buf, sizeof( buf));
		strcpy( token, "                ���y�d�ԧQ�~�{�o��\n\n");
		strcat( token , buf );
			//strcat( token, "	�����٦b�i�椤��...\n");			
		return FALSE;
	}

	if(Raceman_CheckItem( meindex , talker ) == FALSE){		
		NPC_Util_GetStrFromStrWithDelim( argstr , "nonitem_msg", buf, sizeof( buf));
		strcpy( token, "                ���y�d�ԧQ�~�{�o��\n\n");		
		strcat( token , buf );
			//strcat( token, "	�ܩ�p�A�z�S���y�d�ҵL�k���...\n");			
		return FALSE;
	}

	NPC_Util_GetStrFromStrWithDelim( argstr , "LowLevel", buf1, sizeof( buf1));
	lowlevel = atoi(buf1);
	NPC_Util_GetStrFromStrWithDelim( argstr , "GameMode", buf1, sizeof( buf1));
	gmode = atoi(buf1);

	if( CHAR_getInt( talker,CHAR_CATCHCNT1+gmode-1 ) < lowlevel ){
		NPC_Util_GetStrFromStrWithDelim( argstr , "low_msg", buf, sizeof( buf));
		strcpy( token, "                ���y�d�ԧQ�~�{�o��\n\n");
			//strcat( token, "	�ܩ�p�A�z�S���F��̧C�з�...\n");			
			strcat( token , buf );
		return TRUE;
	}	

	//�ˬd���L���
	if(Raceman_ItemFullCheck( meindex , talker) == FALSE){
		NPC_Util_GetStrFromStrWithDelim( argstr, "itemfull_msg",buf, sizeof( buf) );
		strcpy( token, "                ���y�d�ԧQ�~�{�o��\n\n		");
		strcat( token, buf );			
		return FALSE;
	}

	strcpy( token, "                ���y�d�ԧQ�~�{�o��\n\n");
		strcat( token, "	�y�d�i�h�A�o�O�A���o���ԧQ�~...\n");			
	return TRUE;
}

void Raceman_awardprize( int meindex , int talker )
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	int  fd,itemindex,ret,gmode,rank,lowlevel;
	char id[20],name[64],buf[128],buf2[20],buf3[256]; //buuf[256];
	int  i,item,itemid,random,prize;
	char prizebuf[20][20];

	memset(prizebuf,0,sizeof(prizebuf));

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {  //Ū�Ѽ�
		print("MsgDisp:GetArgStrErr");
		print("NPCName=%s\n", CHAR_getChar( meindex, CHAR_NAME));
		return;
	}

	NPC_Util_GetStrFromStrWithDelim( argstr , "LowLevel", buf2, sizeof( buf2));
	lowlevel = atoi(buf2);

	NPC_Util_GetStrFromStrWithDelim( argstr, "GameMode", buf, sizeof( buf) );
	gmode = atoi( buf );

	if( CHAR_getInt( talker,CHAR_CATCHCNT1+gmode-1 ) < lowlevel ){
		goto DELITEM;
	}

    if(gmode == 3 || gmode == 5) 
	{
		strcpy(name,CHAR_getChar(talker,CHAR_FMNAME));			//�a�ڦW��
		sprintf(id,"%d",CHAR_getWorkInt(talker, CHAR_WORKFMINDEXI));	//�a��id
	}
	else	// 1 2 4 �ӤH
	{
		fd = getfdFromCharaIndex(talker);
		CONNECT_getCharname( fd, name, sizeof( name ) );  
		CONNECT_getCdkey( fd, id , sizeof( id ) );		  //�b��	
	}

	rank = 0; 
	/*	���� �M�w�@�G�T�W
	switch(gmode)
	{
	case 1:
		for(i=0;i<3;i++){
			if(strcmp(history1[i].id,id)==0 && strcmp(history1[i].name,name)==0 
			   && history1[i].catchcnt == CHAR_getInt( talker,CHAR_CATCHCNT1 )){  
				rank = i+1;	
				break;
			}
		}break;
	case 2:
		for(i=0;i<3;i++){
			if(strcmp(history2[i].id,id)==0 && strcmp(history2[i].name,name)==0 
			   && history2[i].catchcnt == CHAR_getInt( talker,CHAR_CATCHCNT2 )){  
				rank = i+1;	
				break;
			}
		}break;
	case 3:
		for(i=0;i<3;i++){
			if(strcmp(history3[i].id,id)==0 && strcmp(history3[i].name,name)==0 
			   && history3[i].catchcnt == CHAR_getInt( talker,CHAR_CATCHCNT3 )){  
				rank = i+1;	
				break;
			}
		}break;
	case 4:
		for(i=0;i<3;i++){
			if(strcmp(history4[i].id,id)==0 && strcmp(history4[i].name,name)==0 
			   && history4[i].catchcnt == CHAR_getInt( talker,CHAR_CATCHCNT4 )){  
				rank = i+1;	
				break;
			}
		}break;
	case 5:
		for(i=0;i<3;i++){
			if(strcmp(history5[i].id,id)==0 && strcmp(history5[i].name,name)==0 
			   && history5[i].catchcnt == CHAR_getInt( talker,CHAR_CATCHCNT5 )){  
				rank = i+1;	
				break;
			}
		}break;
	}
	*/
	buf[0]='\0';
	if(rank == 1){
		NPC_Util_GetStrFromStrWithDelim( argstr , "thanks1_msg", buf, sizeof( buf));
		NPC_Util_GetStrFromStrWithDelim( argstr , "First", buf2, sizeof( buf2));
	}else if(rank == 2){
		NPC_Util_GetStrFromStrWithDelim( argstr , "thanks2_msg", buf, sizeof( buf));
		NPC_Util_GetStrFromStrWithDelim( argstr , "Second", buf2, sizeof( buf2));
	}else if(rank == 3){
		NPC_Util_GetStrFromStrWithDelim( argstr , "thanks3_msg", buf, sizeof( buf));
		NPC_Util_GetStrFromStrWithDelim( argstr , "Third", buf2, sizeof( buf2));
	}else{
		NPC_Util_GetStrFromStrWithDelim( argstr , "thanks_msg", buf, sizeof( buf));
		NPC_Util_GetStrFromStrWithDelim( argstr , "Normal", buf3, sizeof( buf3));
		//�ѥ[���H���M�w
		i = 0;
		while( getStringFromIndexWithDelim( buf3 , "," , i+1 , buf2 , sizeof(buf2) ))
		{
			sprintf( prizebuf[i],"%s",buf2);
			i++ ;
		}
		random = rand() % i;
		sprintf( buf2 , "%s" , prizebuf[random]);
		//sprintf(buuf,"random:%d  item:%s",random,buf2);
		//CHAR_talkToCli( talker , -1, buuf , CHAR_COLORYELLOW);
	}
	CHAR_talkToCli( talker, -1, buf, CHAR_COLORYELLOW);
	prize = atoi(buf2);

	//�����~
	itemindex = ITEM_makeItemAndRegist( prize );
	if(itemindex == -1) return;

	ret = CHAR_addItemSpecificItemIndex( talker, itemindex);
	if( ret < 0 || ret >= CHAR_MAXITEMHAVE ) {
		print( "npc_raceman.c: ACCEPTadditem error itemindex[%d]\n", itemindex);
		ITEM_endExistItemsOne( itemindex);
		return ;
	}
	CHAR_sendItemDataOne( talker, ret);

DELITEM:
	//�R���y�d��
  	if( NPC_Util_GetStrFromStrWithDelim( argstr, "CheckItem", buf , sizeof( buf ) ) != NULL )
	{
		item = atoi(buf);
		for( i = 0 ; i < CHAR_MAXITEMHAVE ; i++ ) {
			itemindex=CHAR_getItemIndex( talker , i );
			if( ITEM_CHECKINDEX( itemindex) ) {
				itemid=ITEM_getInt( itemindex ,ITEM_ID );
				if(itemid==item){	//����y�d��
					LogItem(
						CHAR_getChar( talker, CHAR_NAME ), 
						CHAR_getChar( talker, CHAR_CDKEY ),
			#ifdef _add_item_log_name  // WON ADD �bitem��log���W�[item�W��
						itemindex,
			#else
       					ITEM_getInt( itemindex, ITEM_ID ),  
			#endif
						"�]����Ӯ���",
						CHAR_getInt( talker,CHAR_FLOOR),
						CHAR_getInt( talker,CHAR_X ),
 	      				CHAR_getInt( talker,CHAR_Y ),
						ITEM_getChar( itemindex, ITEM_UNIQUECODE),
						ITEM_getChar( itemindex, ITEM_NAME),
						ITEM_getInt( itemindex, ITEM_ID)
					);

					CHAR_DelItem( talker , i );
					CHAR_talkToCli( talker, -1, "�y�d�Ү����F", CHAR_COLORYELLOW);
					break;
				}					
			}
		}
	}
}

int Raceman_readmode( int meindex , int talkerindex )
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	int  gmode;  // 1:�s�⭭ 2:�ӤH�� 3:���魭 4:�ӤH���� 5:���餣��
	char buf[10];

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {  //Ū�Ѽ�
		print("MsgDisp:GetArgStrErr");
		print("NPCName=%s\n", CHAR_getChar( meindex, CHAR_NAME));
	}

	if(NPC_Util_GetStrFromStrWithDelim( argstr , "GameMode", buf, sizeof( buf)) != NULL ){
		gmode = atoi(buf);	
		return gmode;
	}
	return 0;
}

int Raceman_CheckSelectRank(meindex,talkerindex,datanum)
{
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char buf[64],buf2[64];
	int  select = 0;

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))== NULL) {  //Ū�Ѽ�
		print("MsgDisp:GetArgStrErr");
		print("NPCName=%s\n", CHAR_getChar( meindex, CHAR_NAME));
	}

	NPC_Util_GetStrFromStrWithDelim( argstr, "EndFlag",buf, sizeof( buf) );
	if(strcmp(buf,"off")==0){
		if(NPC_Util_GetStrFromStrWithDelim( argstr , "HasGame", buf, sizeof( buf)) != NULL ){
			getStringFromIndexWithDelim( buf , "," , datanum-1 , buf2 , sizeof(buf2) );
			select = atoi(buf2);
		}
	}else{
		if(NPC_Util_GetStrFromStrWithDelim( argstr , "GameMode", buf, sizeof( buf)) != NULL ){
			select = atoi(buf) + 10 ;
		}

	}
	
	return select;
}

void Raceman_GetQuestion( int meindex ,int talker , char *token)
{
	char line[2000];
	char buf[512];//buuf[128];
	int  i,j ,quiztotal = 0, quiz = 0 ;
	FILE* fp;	

	fp = fopen(".//data//racequiz.txt", "r");
	if (fp == NULL)
	{
		print("racequiz File Open Error\n");
		strcpy( token,"�t�εo�Ϳ��~�I");		
		return ;
	}
	while(1){		
		if (fgets(line, sizeof(line), fp) == NULL)	break;				
		quiztotal++;
	}
	fclose(fp);

	fp = fopen(".//data//racequiz.txt", "r");
	if (fp == NULL)
	{
		print("racequiz File Open Error\n");
		strcpy( token,"�t�εo�Ϳ��~�I");		
		return ;
	}

	quiz = rand() % (quiztotal) ; //�üƨM�w���D

	j = 0 ;
	while(1){		

		line[0]='\0';	
		if (fgets(line, sizeof(line), fp) == NULL){
			break;
		}
		chop(line);

		// #������
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}

		if( j == quiz ){
			getStringFromIndexWithDelim(line, "|", 1, buf, sizeof(buf)); //Ū�X�Y�쪱�a���D��	
			CHAR_setWorkInt( talker , CHAR_WORK_RACEQUIZ , quiz );		 //�O���Y�쪱�a�諸�D��	
			break;
		}
		j++;
	}
	fclose(fp);	
	
	
	if(strlen( buf) < 40) {
		strcat( buf,"\n\n");
	}else if( strlen( buf) < 80) {
		strcat( buf,"\n\n");
	}else{
		strcat( buf,"\n");
	
	}
	sprintf( token,"�����ʤ@�ʡI����F�A�A���d���~��n�O��\n"
			"\n%s "
			"\n���g�b�o��\n"
			"  (�g���Ы��ݢ�)",
			buf);	

	//sprintf(buuf,"j%d quiz:%d qtotal:%d setint:%d",j,quiz,quiztotal,CHAR_getWorkInt( talker , CHAR_WORK_RACEQUIZ ));
	//CHAR_talkToCli( talker , -1, buuf , CHAR_COLORYELLOW);

}

BOOL Raceman_CheckAnswer( int meindex , int talker , char *data )
{
	char line[2000];
	char buf[128];//buuf[128];
	int	 answer,i,j;
	FILE* fp;	

	fp = fopen(".//data//racequiz.txt", "r");
	if (fp == NULL)
	{
		print("racequiz File Open Error\n");
		return FALSE;
	}
	
	answer = CHAR_getWorkInt( talker , CHAR_WORK_RACEQUIZ );
	j=0;
	while(1){

		line[0]='\0';	
		if (fgets(line, sizeof(line), fp) == NULL){
			break;
		}
		chop(line);

		// #������
		if( line[0] == '#' )
			continue;
		for( i=0; i<strlen(line); i++ ){
            if( line[i] == '#' ){
			    line[i] = '\0';
		        break;
			}
		}

		if( j == answer ){
			getStringFromIndexWithDelim(line, "|", 2 , buf, sizeof(buf));				
			break;
		}

		print("\answer: %d %d  answer:%s", 
			j,
			CHAR_getWorkInt( talker , CHAR_WORK_RACEQUIZ),
			buf);
		j++;
	}
	fclose(fp);	

	//sprintf(buuf,"answerNO:%d  j:%d  data:%s answer:%s",answer,j,data,buf);
	//CHAR_talkToCli( talker , -1, buuf , CHAR_COLORYELLOW);

	if( strcmp( data , buf ) == 0 ) 
		return TRUE;
	else 
		return FALSE;
}

#endif



