#include "version.h"
#include <string.h>
#include "object.h"
#include "char_base.h"
#include "char.h"
#include "util.h"
#include "npcutil.h"
#include "npc_oldman.h"
#include "title.h"
#include "char_data.h"
#include "readmap.h"
#include "lssproto_serv.h"
#include "log.h"

#define RANGE 2


void NPC_SetFlg(int talker,int shiftbit);
BOOL NPC_CheckFlg(int point,int shiftbit);
BOOL NPC_SavePointItemCheck(int meindex,int talker,int itemno,int kosuu);
void NPC_MessageDisp(int meindex,int talker,int MseNo);
BOOL NPC_UsedCheck(int meindex,int talker,int flg);
BOOL NPC_SavePointItemDelete(int meindex,int talker,int itemNo,int kosuu);

extern time_t initTime;

BOOL NPC_SavePointInit( int meindex )
{
    int     oldmanid=0, bornfl=0, bornx=0 ,borny=0;
    char  argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
    char *argstr;
   	char token[32];
	char buf2[512];

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))==NULL){
		print("NPC_Savepoint.c Init: GetArgStrErr");
		return FALSE;
	}

	if(NPC_Util_GetStrFromStrWithDelim(argstr,"ID",buf2,sizeof( buf2) )==NULL){
		print("SavePoint:ID�|���]�w ");
		return FALSE;
	}
		
	oldmanid=atoi(buf2);
	NPC_Util_GetStrFromStrWithDelim(argstr,"Born",buf2,sizeof( buf2) );
	getStringFromIndexWithDelim( buf2,",",1,token,sizeof(token));
	bornfl = atoi( token );
	getStringFromIndexWithDelim( buf2,",",2,token,sizeof(token));
    bornx = atoi( token );
	getStringFromIndexWithDelim( buf2,",",3,token,sizeof(token));
    borny = atoi( token );

    CHAR_setWorkInt( meindex , CHAR_WORKOLDMANID ,oldmanid );
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPESAVEPOINT );
    CHAR_setFlg( meindex , CHAR_ISATTACKED , 1 );

    if( MAP_IsValidCoordinate( bornfl,bornx,borny )== TRUE
    && CHAR_ElderSetPosition( oldmanid ,bornfl,bornx, borny ) == TRUE ){
        return TRUE;
    }else{
		printf( "Invalid elder npcarg=%s\n", argstr );
		return FALSE;
    }
}

void NPC_SavePointTalked( int meindex , int talkerindex , char *msg ,
                     int color )
{
    int	fd;
   	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
   	char *argstr;
    	char token[512];
	char buf2[1024];
	char timeMsg[1024];
	int oldmanid;

	if( ! NPC_Util_charIsInFrontOfChar(talkerindex,meindex,RANGE)
	   || CHAR_getFlg( talkerindex,CHAR_ISDIE )){
		if( (CHAR_getInt(talkerindex,CHAR_X) == CHAR_getInt(meindex,CHAR_X))
		 && (CHAR_getInt(talkerindex,CHAR_Y) == CHAR_getInt(meindex,CHAR_Y))
		 && (CHAR_getInt(talkerindex,CHAR_FLOOR) ==CHAR_getInt(meindex,CHAR_FLOOR))	 ){
	   	}else{
	   		return;
		}
	}

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))==NULL){
		print("NPC_Savepoint.c TypeCheck: GetArgStrErr\n");
		return ;
	}

	if(NPC_Util_GetStrFromStrWithDelim(argstr,"ID",buf2,sizeof( buf2) )==NULL) return ;
	oldmanid=atoi(buf2);

	if(strstr(argstr,"NOITEM")!=NULL) {
		NPC_SetFlg(talkerindex,oldmanid);
	}

	if(NPC_CheckFlg(CHAR_getInt(talkerindex,CHAR_SAVEPOINT),oldmanid)==FALSE){

		if(NPC_UsedCheck(meindex,talkerindex,0)==TRUE){
			NPC_MessageDisp(meindex,talkerindex,2);
		}else{
			NPC_MessageDisp(meindex,talkerindex,0);
		}
	}else{
    	CHAR_setInt( talkerindex, CHAR_LASTTALKELDER ,CHAR_getWorkInt( meindex, CHAR_WORKOLDMANID ) );
		fd = getfdFromCharaIndex( talkerindex);
		if( fd != -1 ) {
#ifndef _NPC_NOSAVEPOINT
			Char	*chwk;
	    	chwk = CHAR_getCharPointer( talkerindex);
			// CoolFish: +1 2001/11/05
			if (!chwk)	return;

			CHAR_charSaveFromConnectAndChar( fd, chwk ,FALSE );
			// Nuke 0907: For acsv test
			{
				CHAR_charSaveFromConnectAndChar( fd, chwk ,FALSE );
			}
#endif			                                                                                                                                                       
		}

		NPC_Util_GetStrFromStrWithDelim(argstr,"NomalMsg",token,sizeof( token) );
	    sprintf(buf2,token,CHAR_getChar(talkerindex,CHAR_NAME));
	    
	    // Robin 0619 display time
	    {
	    	time_t new_t;
	    	int dd,hh,mm,ss;
	    	
	    	time(&new_t);
	    	new_t-=initTime;
		
		dd=(int) new_t / 86400; new_t=new_t % 86400;
    		hh=(int) new_t / 3600;  new_t=new_t % 3600;
	        mm=(int) new_t / 60;    new_t=new_t % 60;
	        ss=(int) new_t;
		
		if (dd>0) {
	        	snprintf( timeMsg, sizeof( timeMsg ) ,
        			  "���P�y�w�g�ҰʤF %d �� %d �p�� %d �� %d ��C",dd,hh,mm,ss);
                } else if (hh>0) {
 	                snprintf( timeMsg, sizeof( timeMsg ) ,
      		                  "���P�y�w�g�ҰʤF %d �p�� %d �� %d ��C",hh,mm,ss);
                } else {
                        snprintf( timeMsg, sizeof( timeMsg ) ,
                                  "���P�y�w�g�ҰʤF %d �� %d ��C",mm,ss);
                }
                strcat(buf2,timeMsg);
	    
	    }
	    {
	        char temp[1024];
	        //char *answer;// CoolFish: Rem 2001/4/18
		extern unsigned int StateTable[];
		// Robin 03/20 �U�઺�ѯ��A�н絹��������O�q!!!!
		#ifdef _JZ_VIPCODE
	        sprintf(temp,"�ثe�u�W�H�Ƭ� %d �H�C",abs((StateTable[LOGIN]) * getSaveNumberMultiple()));
	  #else
	  			sprintf(temp,"�ثe�u�W�H�Ƭ� %d �H�C",abs((StateTable[LOGIN])));
	  #endif
	        strcat(buf2,temp);
	        
	                                                       
	    }
	    CHAR_talkToCli( talkerindex , meindex , buf2 , CHAR_COLORWHITE );

	}



}


/*-----------------------------------------
 * ���饤����Ȥ����֤äƤ������˸ƤӽФ����e
 *
-------------------------------------------*/
void NPC_SavePointWindowTalked( int meindex, int talkerindex, 
								int seqno, int select, char *data)
{

	/*--       �Τޤ��ˤ��ʤ��Ȥ��Ͻ�   --*/
	if( NPC_Util_CharDistance( talkerindex, meindex ) > 2) {
		return;
	}

	 if(seqno==CHAR_WINDOWTYPE_WINDOWSAVEPOINT_START){
		if(select==WINDOW_BUTTONTYPE_YES){
			if(NPC_UsedCheck(meindex,talkerindex,1)==TRUE){
				NPC_MessageDisp(meindex,talkerindex,1);
			}
		}
	}


}


/*
 *--����  ��Υ����å�������Ԥ�
 */
BOOL NPC_AndReduce(int meindex,int talker,char *buf,int flg)
{

	char buf2[512];
	char buf3[256];
	int itemNo=0;
	int kosuu;
	int j=1;	
	
	if(flg==0){
	
		while(getStringFromIndexWithDelim(buf,"&",j,buf2,sizeof(buf2))!=FALSE)
		{
			j++;
			/*--  ��  ������Сb���Υ���  ���  �����ꤷ�Ƥ�ȸ�����  --*/
			if(strstr( buf2, "*") != NULL){
				getStringFromIndexWithDelim(buf2,"*",1,buf3,sizeof(buf3));
				itemNo = atoi(buf3);
				getStringFromIndexWithDelim(buf2,"*",2,buf3,sizeof(buf3));
				kosuu = atoi(buf3);
	
				
				/*--�����å��⡼��--*/
				if(NPC_SavePointItemCheck(meindex,talker,itemNo,kosuu)==FALSE){
					return FALSE;
				}

			}else{
				itemNo = atoi(buf2);

				/*--�����å��⡼��--*/
				if(NPC_SavePointItemCheck(meindex,talker,itemNo,1)==FALSE){
					return FALSE;
				}

			}
		}

	}else{
		/*--  ��  ������Сb���Υ���  ���  �����ꤷ�Ƥ�ȸ�����  --*/
		if(strstr( buf, "*") != NULL){
			getStringFromIndexWithDelim(buf,"*",1,buf3,sizeof(buf3));
			itemNo = atoi(buf3);
			getStringFromIndexWithDelim(buf,"*",2,buf3,sizeof(buf3));
			kosuu = atoi(buf3);

			/*--�����å��⡼��--*/
			if(NPC_SavePointItemCheck(meindex,talker,itemNo,kosuu)==FALSE){
				return FALSE;
			}

		}else{
			itemNo = atoi(buf);
			/*--�����å��⡼��--*/
			if(NPC_SavePointItemCheck(meindex,talker,itemNo,1)==FALSE){
					return FALSE;
			}
		}
	
	}


	return TRUE;
} 


BOOL NPC_AndReduceDelete(int meindex,int talker,char *buf,int flg)
{

	char buf2[512];
	char buf3[256];
	int itemNo=0;
	int kosuu;
	int j=1;	
	
	if(flg==0){
	
		while(getStringFromIndexWithDelim(buf,"&",j,buf2,sizeof(buf2))!=FALSE)
		{
			j++;
			/*--  ��  ������Сb���Υ���  ���  �����ꤷ�Ƥ�ȸ�����  --*/
			if(strstr( buf2, "*") != NULL){
				getStringFromIndexWithDelim(buf2,"*",1,buf3,sizeof(buf3));
				itemNo = atoi(buf3);
				getStringFromIndexWithDelim(buf2,"*",2,buf3,sizeof(buf3));
				kosuu = atoi(buf3);

				/*--����⡼��--*/
				if(NPC_SavePointItemDelete(meindex,talker,itemNo,kosuu)==FALSE){
					return FALSE;
				}

			}else{
				itemNo = atoi(buf2);
				/*--����⡼��--*/
				if(NPC_SavePointItemDelete(meindex,talker,itemNo,1)==FALSE){
					return FALSE;
				}

			}
		}

	}else{
		/*--  ��  ������Сb���Υ���  ���  �����ꤷ�Ƥ�ȸ�����  --*/
		if(strstr( buf, "*") != NULL){
			getStringFromIndexWithDelim(buf,"*",1,buf3,sizeof(buf3));
			itemNo = atoi(buf3);
			getStringFromIndexWithDelim(buf,"*",2,buf3,sizeof(buf3));
			kosuu = atoi(buf3);

			/*--����⡼��--*/
			if(NPC_SavePointItemDelete(meindex,talker,itemNo,kosuu)==FALSE){
				return FALSE;
			}

		}else{
			itemNo = atoi(buf);
			
			/*--����⡼��--*/
			if(NPC_SavePointItemDelete(meindex,talker,itemNo,1)==FALSE){
				return FALSE;
			}
		}
	
	}


	return TRUE;
} 



/*
 * ���򸫤��������Υ����å�
 */
BOOL NPC_UsedCheck(int meindex,int talker,int flg)
{

   	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
   	char *argstr;
 	char buf[128];
	char buf2[512];
	int i=1;
	int checkflg=0;
	
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))==NULL){
		print("NPC_savePoint.c UseCheck: GetArgStrErr");
		return FALSE;
	}

	/*--  �פʥ���  ����äƤ��뤫�Υ����å�-*/
	if(NPC_Util_GetStrFromStrWithDelim(argstr,"GetItem",buf,sizeof( buf) )!=NULL)
	{
		i=1;
		while( getStringFromIndexWithDelim(buf,",",i,buf2,sizeof(buf2)) !=FALSE )
		{
			i++;
			if(strstr( buf2, "&") != NULL){
				
				if(NPC_AndReduce( meindex, talker, buf2,0)==TRUE)
				{
					checkflg=1;
					break;
				}
			}else{
				if(NPC_AndReduce( meindex, talker, buf2,1)==TRUE)
				{
					checkflg=2;
					break;
				}
			}
		}
		if(checkflg==0) return FALSE;
  } else return TRUE;   // Arminius 2.5 �p�G�S���] GetItem �h�@���x�s

	if(flg==1){
		if(checkflg==1){
			/*--�����Ǻ���ν�  --*/
			if(NPC_AndReduceDelete(meindex,talker,buf2,0)==FALSE){
				return FALSE;
			}
		}else{
			/*--�����Ǻ���ν�  --*/
			if(NPC_AndReduceDelete(meindex,talker,buf2,1)==FALSE){
				return FALSE;
			}
		}	
	}
	
	return TRUE;

}



void NPC_MessageDisp(int meindex,int talker,int MesNo)
{

	int fd = getfdFromCharaIndex( talker);
	int buttontype=WINDOW_BUTTONTYPE_OK;
	int windowtype=WINDOW_MESSAGETYPE_MESSAGE;
	int windowno=CHAR_WINDOWTYPE_WINDOWSAVEPOINT_START; 

	char token[1024];
   	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
   	char *argstr;
	char buf[32];

	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1)))==NULL){
		print("NPC_savePoint.c UseCheck: GetArgStrErr");
		return ;
	}

	switch(MesNo){
	  case 0:/*--  �פʥ���  ����äƤ��뤫�Υ����å�-*/
		NPC_Util_GetStrFromStrWithDelim(argstr,"RequestMsg",token,sizeof( token) );

		break;

	  case 1:/*--  �פʥ���  ����äƤ��뤫�Υ����å�-*/
		NPC_Util_GetStrFromStrWithDelim(argstr,"OkMsg",token,sizeof( token) );
		NPC_Util_GetStrFromStrWithDelim(argstr,"ID",buf,sizeof( buf) );
		NPC_SetFlg(talker,atoi(buf));
		/*--�����֤����Ƥ�����--*/
		/* �Ϥʤ��������ۤˡb��ʬ��ID�򵭲������� */
		CHAR_setInt( talker, CHAR_LASTTALKELDER ,CHAR_getWorkInt( meindex, CHAR_WORKOLDMANID ) );
	
		/* �����֤��� */
		/*  �ҥå����Ƥ���Ϥ����ʤ��Τǥ���ҥå����ʤ�    */
		fd = getfdFromCharaIndex( talker);
		if( fd != -1 ) {
#ifndef _NPC_NOSAVEPOINT
			Char	*chwk;
			chwk = CHAR_getCharPointer( talker);
			// CoolFish: +1 2001/11/05
			if (!chwk)	return;
			CHAR_charSaveFromConnectAndChar( fd, chwk ,FALSE );
#endif
		}

		break;
	
	  case 2:/*--  �פʥ���  ����äƤ��뤫�Υ����å�-*/
		NPC_Util_GetStrFromStrWithDelim(argstr,"RealyMsg",token,sizeof( token) );
	  	buttontype=WINDOW_BUTTONTYPE_YESNO;
		break;

	}
		lssproto_WN_send( fd, windowtype, 
						buttontype, 
						windowno,
						CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX),
						token);


}

BOOL NPC_SavePointItemDelete(int meindex,int talker,int itemNo,int kosuu)
{
	int i;
	int itemindex;
	int id;
	int cnt=0;
	
	for( i=CHAR_STARTITEMARRAY;i<CHAR_MAXITEMHAVE;i++ ){

		itemindex=CHAR_getItemIndex( talker , i );
		if( ITEM_CHECKINDEX(itemindex) ){
			id=ITEM_getInt(itemindex ,ITEM_ID );
			if(itemNo==id){
				cnt++;
				{
					LogItem(
					CHAR_getChar( talker, CHAR_NAME ),
					CHAR_getChar( talker, CHAR_CDKEY ),
#ifdef _add_item_log_name  // WON ADD �bitem��log���W�[item�W��
					itemindex,
#else
	       			ITEM_getInt( itemindex, ITEM_ID ),
#endif
					"SP_DelItem(�浹�O���I�O�����D��)",
					CHAR_getInt( talker,CHAR_FLOOR),
					CHAR_getInt( talker,CHAR_X ),
					CHAR_getInt( talker,CHAR_Y ),
					ITEM_getChar( itemindex, ITEM_UNIQUECODE),
					ITEM_getChar( itemindex, ITEM_NAME),
					ITEM_getInt( itemindex, ITEM_ID)
				);
				}

				CHAR_DelItem( talker, i) ;

				if(cnt==kosuu){
					return TRUE;
				}
			}
		}
	}

	return FALSE;


}


/*--����  �������å�--*/
BOOL NPC_SavePointItemCheck(int meindex,int talker,int itemno,int kosuu)
{
	int i;
	int itemindex;
	int id;
	int cnt=0;
	
	for( i=CHAR_STARTITEMARRAY;i<CHAR_MAXITEMHAVE;i++ ){

		itemindex=CHAR_getItemIndex( talker , i );
		if( ITEM_CHECKINDEX(itemindex) ){
			id=ITEM_getInt(itemindex ,ITEM_ID );
			if(itemno==id){
				cnt++;
				if(cnt==kosuu){
					return TRUE;
				}
			}
		}
	}

	return FALSE;

}


/*
 * �ӥåȥե饰��  �Ƥ�����ν�  
 */
void  NPC_SetFlg(int talker,int shiftbit)
{
	int point;

	point = CHAR_getInt(talker,CHAR_SAVEPOINT);

	point = point | (1<<shiftbit);

	CHAR_setInt(talker,CHAR_SAVEPOINT,point);
}

/*
 * �ӥåȥե饰��  �äƤ뤫��Ĵ�٤�
 */
BOOL NPC_CheckFlg(int point,int shiftbit)
{
//	int anser;

//	anser = point & (1<<shiftbit);
	if( (point & (1<<shiftbit))  == (1<<shiftbit)){
		return TRUE;
	}
	return FALSE;

}


