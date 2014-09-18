#include "version.h"
#include <string.h>
#include "char.h"
#include "object.h"
#include "char_base.h"
#include "npcutil.h"
#include "lssproto_serv.h"


//���ߤλ��ѤǤ���ե饰�ο�
#define MAXEVENTFLG 96

/*
 * ���٥�ȤΥե饰������å�����      
 *
 */
 
static void NPC_CheckMan_selectWindow( int meindex, int toindex, int num);
int NPC_NowFlgCheck(int meindex,int talker,int now[MAXEVENTFLG]);
int NPC_EndFlgCheck(int meindex,int talker ,int nowflg[MAXEVENTFLG]);
BOOL NPC_FlgCheckMain( int meindex,int talker,int nowindex,int now[MAXEVENTFLG],char *work2);

/*********************************
* �����  
*********************************/
BOOL NPC_CheckManInit( int meindex )
{
	/*--�����Υ����פ�����--*/
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPECHECKMAN);

	return TRUE;
}


/*********************************
*   ��������줿���ν�  
*********************************/
void NPC_CheckManTalked( int meindex , int talkerindex , char *szMes ,int color )
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

	//  ��Υ�����ɥ���
	NPC_CheckMan_selectWindow( meindex, talkerindex, 0);

}


/*
 * �ƽ�  ��ʬ����
 */
static void NPC_CheckMan_selectWindow( int meindex, int talker, int num)
{

	char token[1024];
	char work[256];
	char work2[512];

	char escapedname[1024];
	int fd = getfdFromCharaIndex( talker);
	int buttontype = 0;
	int windowtype = 0;
	int windowno = 0;
	int now[MAXEVENTFLG];
	int nowindex;
	int i;	
	int page;
	
	work[0] = 0;
	work2[0] = 0;
	token[0] = 0;

	now[0] =0;
	
	/*--������ɥ������ץ�å��������������Τ��������--*/
  	windowtype = WINDOW_MESSAGETYPE_MESSAGE;

	switch( num) {
	
	  case 0:
		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANT,0);
		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC,0);
		
  		/*--��      --*/
		sprintf(token,"3\n �a�a�a�a�֡֡a�����å��ޥ�a�֡� "
				  "\n�a�a�a���ߤΥե饰�����å��򤷤ޡ���"
				  "\n"
				  "\n�a�a�a�a ��aNOW�ե饰�����å��a�� "
				  "\n�a�a�a�a ��aEND�ե饰�����å��a�� "
				  "\n\n�a�a ��aNOW�ե饰�����å�  �ܺ�  �� "
				  "\n�a�a ��aEND�ե饰�����å�  �ܺ�  �� "
		);

	  	buttontype = WINDOW_BUTTONTYPE_CANCEL;
	  	windowtype = WINDOW_MESSAGETYPE_SELECT;
	  	windowno = CHAR_WINDOWTYPE_CHECKMAN_START; 
	  	break;

	//NOW�ե饰�δ�ñ  ��
	  case 1:
	  	
	  	//NOW�ե饰�Υ����å�
		nowindex = NPC_NowFlgCheck( meindex, talker, now);

		//���ڡ���  ��
		page = CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC) ;
		
		if(page == 0 || page == 1){
			i = 0;
		}else{
			i = 83;
		}
		
		//�ڡ���
		for(; i < nowindex ; i++)
		{
			sprintf(work,"%d,",now[ i]);
			strcat(work2,work);
		}
		
		sprintf(token,"�a�a�a�a�a�֡֡a�����å��ޥ�a�֡� "
					"\n�a�a���ߤοv  ��  �äƤ��� NOW���٥��"
					"\n%s"
			 	,work2);	
	  	
	  	if(page != 2 && nowindex > 83)
	  	{
	  		//�⡼��
	  		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANT,1);
	  		//�ڡ���
	  		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC,2);

			buttontype = WINDOW_BUTTONTYPE_NEXT;
			windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		  	windowno = CHAR_WINDOWTYPE_CHECKMAN_MAIN; 

		}else{
			buttontype = WINDOW_BUTTONTYPE_OK;
			windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		}
		
		break;


	//END�ե饰�δ�ñ  ��
	  case 2:
		//��  �ե饰�Υ����å�
		nowindex = NPC_EndFlgCheck( meindex, talker, now);
		
		//���ڡ���  ��
		page = CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC) ;
		
		if(page == 0 || page == 1){
			i = 0;
		}else{
			i = 83;
		}
		work2[0]=0;
		
		//�ڡ���
		for(; i < nowindex ; i++)
		{
			sprintf(work,"%d,",now[ i]);
			strcat(work2,work);
		}
		sprintf(token,"�a�a�a�a�a�֡֡a�����å��ޥ�a�֡� "
					"\n�a�a���ߤοv  ��  �äƤ��� END���٥��"
					"\n%s"
			 	,work2);	
	  	
	  	if(page != 2 && nowindex > 83)
	  	{
	  		//�⡼��
	  		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANT,2);
	  		//�ڡ���
	  		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC,2);

			buttontype = WINDOW_BUTTONTYPE_NEXT;
			windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		  	windowno = CHAR_WINDOWTYPE_CHECKMAN_MAIN; 

		}else{
			buttontype = WINDOW_BUTTONTYPE_OK;
			windowtype = WINDOW_MESSAGETYPE_MESSAGE;
		}
		
		break;
		
	
	// NOW�ե饰�ξܺ�  ��
	  case 4:
		{
			
			//NOW�ե饰�����å�
			nowindex = NPC_NowFlgCheck( meindex, talker, now);

			if(NPC_FlgCheckMain( meindex, talker, nowindex,now,work2)
			 == FALSE)
			 {
			 	return;
			 }
			

			sprintf(token,"�a�a�a�a�a�֡֡a�����å��ޥ�a�֡� "
						"\n�a�a���ߤοv  ��  �äƤ��� NOW���٥��"
						"\n%s"
				 	,work2);	

			page = CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC) ;
		  	nowindex = (nowindex / ((6*page)+1));

		  	if(page != 16 &&  nowindex != 0)
		  	{
		  		//�⡼��
		  		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANT,4);
		  		//�ڡ���
		  		page = CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC);
		  		page++;
		  		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC,page);
			
				buttontype = WINDOW_BUTTONTYPE_NEXT;
				windowtype = WINDOW_MESSAGETYPE_MESSAGE;
			  	windowno = CHAR_WINDOWTYPE_CHECKMAN_MAIN; 
			}else{
				buttontype = WINDOW_BUTTONTYPE_OK;
				windowtype = WINDOW_MESSAGETYPE_MESSAGE;
			}

		}
	break;

	// END�ե饰�ξܺ�  ��
	  case 5:
		{
			//END�ե饰�Υ����å�
			nowindex = NPC_EndFlgCheck( meindex, talker, now);
			
			
			if(NPC_FlgCheckMain( meindex, talker, nowindex, now, work2)
			 == FALSE)
			 {
			 	return;
			 }

			sprintf(token,"�a�a�a�a�a�֡֡a�����å��ޥ�a�֡� "
						"\n�a�a���ߤοv  ��  �äƤ��� END���٥��"
						"\n%s"
				 	,work2);	

			page = CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC) ;

		  	nowindex = (nowindex / ((6*page)+1));

		  	if(page != 16 &&  nowindex != 0)
		  	{
		  		//�⡼��
		  		CHAR_setWorkInt(talker, CHAR_WORKSHOPRELEVANT, 5);
		  		//�ڡ���
		  		page = CHAR_getWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC);
		  		page++;
		  		CHAR_setWorkInt( talker, CHAR_WORKSHOPRELEVANTSEC, page);
			
				buttontype = WINDOW_BUTTONTYPE_NEXT;
				windowtype = WINDOW_MESSAGETYPE_MESSAGE;
			  	windowno = CHAR_WINDOWTYPE_CHECKMAN_MAIN; 
			}else{
				buttontype = WINDOW_BUTTONTYPE_OK;
				windowtype = WINDOW_MESSAGETYPE_MESSAGE;
			}

		}
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
void NPC_CheckManWindowTalked( int meindex, int talkerindex, 
								int seqno, int select, char *data)
{
	int datano;
	
	if( NPC_Util_CharDistance( talkerindex, meindex ) > 2) return;

	datano = atoi(data);
	
	if(select == WINDOW_BUTTONTYPE_OK) 
	{
		NPC_CheckMan_selectWindow( meindex, talkerindex, 0 );
	}else if(select == WINDOW_BUTTONTYPE_CANCEL) {
		return;
	}



	
	switch(CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANT)){
	  case 1:
	  	if(CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTSEC) == 2){
			NPC_CheckMan_selectWindow( meindex, talkerindex, 1 );
	  	}
	  	break;

	  case 2:
	  	if(CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTSEC) == 2){
			NPC_CheckMan_selectWindow( meindex, talkerindex, 2 );
	  	}
	  	break;

	  case 4:
	  	if(CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTSEC) >= 2){
			NPC_CheckMan_selectWindow( meindex, talkerindex, 4 );
	  	}

	  case 5:
		if(CHAR_getWorkInt(talkerindex,CHAR_WORKSHOPRELEVANTSEC) >= 2){
			NPC_CheckMan_selectWindow( meindex, talkerindex, 5 );
		}

		break;
	}

	switch( datano ){
	  case 1:
			NPC_CheckMan_selectWindow( meindex, talkerindex, 1 );
		break;

	  case 2:
			NPC_CheckMan_selectWindow( meindex, talkerindex, 2 );
		break;

	  case 4:
			NPC_CheckMan_selectWindow( meindex, talkerindex, 4 );
		break;
		
	  case 5:
			NPC_CheckMan_selectWindow( meindex, talkerindex, 5 );
		break;
	}

}

/*
 *�a���٥��  �ե饰������å�����
 */
int NPC_NowFlgCheck(int meindex,int talker ,int nowflg[MAXEVENTFLG])
{
	int i = 0;
	int j = 0;
	
	for(i= 0; i < MAXEVENTFLG ; i++){
		if(NPC_NowEventCheckFlg( talker, i) == TRUE)
		{
			nowflg[j] = i;
			j++;
		}
 	}
 	
	return j;
}

/*
 *�a���٥�Ƚ�  �ե饰������å�����
 */
int NPC_EndFlgCheck(int meindex,int talker ,int nowflg[MAXEVENTFLG])
{
	int i = 0;
	int j = 0;
	
	for(i= 0; i < MAXEVENTFLG ; i++){

		if(NPC_EventCheckFlg( talker, i) == TRUE)
		{
			nowflg[j] = i;
			j++;
		}
 	}
 	
	return j;
}

/*
 * �ܺ�
 *
 */
BOOL NPC_FlgCheckMain( int meindex,int talker,int nowindex,int now[MAXEVENTFLG],char *work2)
{
	int page;
	int max;
	int i;
	int shou;
	int j=1;
	char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
	char *argstr;
	char work[512];
	char buf[40];
	char buf2[42];
	
	//���ڡ���  ��
	page = CHAR_getWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC) ;

	if(page == 0) {
		page =1;
		CHAR_setWorkInt(talker,CHAR_WORKSHOPRELEVANTSEC,1) ;
	}

	if(page == 1){
		i = 0;
		if( nowindex >6 ){
			max =7;
		}else{
			max =nowindex;
		}
	}else{
		max = (page * 6) +1;
		i = max - 7;
		shou = nowindex / max;

		if(shou == 0){
			max = nowindex;
		}else{
			i = max - 7;
		}
	}
			
	//���٥�Ȥξܺ٤��񤫤�Ƥ���ե������  �ߤ���
	if((argstr = NPC_Util_GetArgStr( meindex, argstr1, sizeof(argstr1))) == NULL) {
		print("NPC_CheckMan:GetArgStrErr");
		return FALSE;
	}
			
	work[0] = 0;
	work2[0] = 0;

	//�ڡ���
	for(; i < max ; i++)
	{
		sprintf(work,"#%d:",now[ i]);
		j = 1;
		while( getStringFromIndexWithDelim(argstr, "|", j, buf,sizeof( buf))
		 !=FALSE )
		 {	
			j++;
			if(strstr(buf,work) != NULL){
				sprintf(buf2,"%s\n",buf);
				strcat(work2,buf2);
				break;
			}
		}
	}
	
	return TRUE;
}
