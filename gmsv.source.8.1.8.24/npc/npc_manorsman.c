#include "version.h"
#include <time.h>
#include "char.h"
#include "object.h"
#include "lssproto_serv.h"
#include "npcutil.h"
#include "handletime.h"
#include "npc_scheduleman.h"
#include "family.h"
#include "npc_manorsman.h"
#include "saacproto_cli.h"
#include "net.h"
#include "configfile.h"
#include "log.h"

/*
 * ���� PK �n�O��
 *
 */
#define MAX_MANORSMAN 22
static int FMPK_ManorsmanList[MAX_MANORSMAN]={
-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,
-1,-1 };

enum {
	NPC_WORK_ID = CHAR_NPCWORKINT1,		// �n�O�� ID, �q 0 �}�l
	NPC_WORK_MANORID = CHAR_NPCWORKINT2,	// ����s��
	NPC_WORK_CHALLENGEWAIT = CHAR_NPCWORKINT3,	// �D�Ե��ݮɶ�
	NPC_WORK_PEACEWAIT = CHAR_NPCWORKINT4,	// ��Ԯɶ�
	NPC_WORK_PREVLOOPTIME = CHAR_NPCWORKINT5,	// �e�@���B�z Loop ���ɶ�
#ifdef _NEW_MANOR_LAW
	NPC_WORK_BETTLETIME = CHAR_NPCWORKINT6	// �O���}���ɪ����
#endif
};

#define SCHEDULEFILEDIR		"./Schedule/"

#ifdef _FIX_FAMILY_PK_LIMIT	   // WON ADD �N�i�D�Բ��骺�ѫe�Q�j�אּ�e�G�Q�j
#define PK_LIMIT	20	       
#endif


extern struct FM_POINTLIST  fmpointlist;
extern void NPC_talkToFloor(int floor, int index1, int index2, char *data);
#ifdef _MANOR_PKRULE
extern struct  FMS_DPTOP fmdptop;
#endif

void NPC_ManorLoadPKSchedule(int meindex);	// Load schedule from disk
#ifdef _NEW_MANOR_LAW
void SortManorSchedule();
int SortManorScheduleMomentum(const void *indexa, const void *indexb);
extern int familyMemberIndex[FAMILY_MAXNUM][FAMILY_MAXMEMBER];
int g_iSortManor;
#endif

int NPC_getManorsmanListIndex( int ID)
{

	if( ID < 0 || ID >= MAX_MANORSMAN ){
		return	FMPK_ManorsmanList[ ID];
	}
	return -1;
}
BOOL NPC_ManorSmanInit( int meindex )
{
  char argstr1[NPC_UTIL_GETARGSTR_BUFSIZE];
  char *argstr;
  int meid;
  int a;

  CHAR_setInt( meindex, CHAR_WHICHTYPE, CHAR_TYPEMANORSCHEDULEMAN );

  // �Ѽ�
  argstr = NPC_Util_GetArgStr(meindex, argstr1, sizeof(argstr1));
  meid = NPC_Util_GetNumFromStrWithDelim(argstr, "id" );
  if ((meid<0) || (meid>=MAX_SCHEDULEMAN)) {
    print("MANORSMAN init error: invalid ID(%d)\n",meid);
    meid=0;
  }
  CHAR_setWorkInt(meindex, NPC_WORK_ID, meid);

  a = NPC_Util_GetNumFromStrWithDelim(argstr, "loop" );
  if ((a<100) || (a>10000)) a=1000;
  CHAR_setInt(meindex, CHAR_LOOPINTERVAL, a);

  a = NPC_Util_GetNumFromStrWithDelim(argstr, "manorid" );
  if ((a<1) || (a>/*4*/MANORNUM)) {// CoolFish 2002/2/25
    print("MANORSMAN init error: invalid manor id(%d).", a);
    a=1;
  }
  CHAR_setWorkInt(meindex, NPC_WORK_MANORID, a);

  a = NPC_Util_GetNumFromStrWithDelim(argstr, "challengewait");
  if ((a<1) || (a>259200)) {
    print("MANORSMAN init error: invalid challengewait(%d).",a);
    a=259200;
  }
  CHAR_setWorkInt(meindex, NPC_WORK_CHALLENGEWAIT, a);

  a = NPC_Util_GetNumFromStrWithDelim(argstr, "peacewait");
  if ((a<1) || (a>432000)) {
    print("MANORSMAN init error: invalid peacewait(%d).",a);
    a=604800;
  }
  CHAR_setWorkInt(meindex, NPC_WORK_PEACEWAIT, a);

  NPC_ManorLoadPKSchedule(meindex);

  CHAR_setWorkInt(meindex, NPC_WORK_PREVLOOPTIME, NowTime.tv_sec);
#ifdef _NEW_MANOR_LAW
	CHAR_setWorkInt(meindex,NPC_WORK_BETTLETIME,0);
#endif

  if( CHAR_getWorkInt( meindex, NPC_WORK_ID) >= 0 &&
	  CHAR_getWorkInt( meindex, NPC_WORK_ID) <MAX_MANORSMAN ){
		FMPK_ManorsmanList[ CHAR_getWorkInt( meindex, NPC_WORK_ID)] = meindex;
  }

  return TRUE;
}

#ifndef _NEW_MANOR_LAW
void NPC_ManorSmanTalked(int meindex, int talkerindex, char *msg, int color)
{
  char buf[4096];
  int fd;
  int fmpks_pos = CHAR_getWorkInt(meindex, NPC_WORK_ID)*MAX_SCHEDULE;

  if (NPC_Util_CharDistance(meindex, talkerindex) > 2) return;
  if (!NPC_Util_isFaceToFace(meindex, talkerindex, 2)) return;
  fd = getfdFromCharaIndex(talkerindex);
  if (fd == -1) return;

  switch (fmpks[fmpks_pos+1].flag) {
  case FMPKS_FLAG_NONE:
    // �S�����w�԰��A���ɥi�H�U�Ԯ�
    saacproto_ACFMPointList_send(acfd);
#ifdef _MANOR_PKRULE
	sprintf(buf, "����Ҧ��v���ܾԪ��D�Ը��\n\n"
                 "�@�B�S���֦����骺�a��\n"
	#ifdef _FIX_FAMILY_PK_LIMIT	   // WON ADD �N�i�D�Բ��骺�ѫe�Q�j�אּ�e�G�Q�j
				 "�G�B�a�ڱƦ楲�ݬ��e�G�Q�j�a��\n"
	#else
                 "�G�B�a�ڱƦ楲�ݬ��e�Q�j�a��\n"
	#endif
				 "�T�B���]�ɶ��G�U�Ȥ��I�ܭ��T�I\n\n"
                 "�еy�ݡA�ڱN�T�{�A�����C");
#else
    sprintf(buf, "����Ҧ��v���ܾԪ��D�Ը��\n\n"
                 "�@�B�S���֦����骺�a��\n"
                 "�G�B�a�ڪ��n�氪�����a�ڪ��n��\n\n"
                 "�еy�ݡA�ڱN�T�{�A�����C");
#endif
    lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
	    		WINDOW_BUTTONTYPE_YESNO,
    			CHAR_WINDOWTYPE_CHECKMAN_START,
    			CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
    			buf);
    break;
  case FMPKS_FLAG_MANOR_PREPARE:
    // �ثe�w�g�w�n�԰��A�ǳƤ�
    {
      int timeleft=fmpks[fmpks_pos+1].dueltime-NowTime.tv_sec;
      int dd,hh,mm;
      char buf2[4096];
      dd = /*fmpks[fmpks_pos+1].dueltime*/ timeleft / 86400;
      hh = /*fmpks[fmpks_pos+1].dueltime*/ timeleft / 3600 - dd*24;
      mm = /*fmpks[fmpks_pos+1].dueltime*/ timeleft / 60 - dd*24*60 - hh*60;
      memset(buf2,0,sizeof(buf2));
      if (dd>0) sprintf(buf, " %d ��", dd); else strcpy(buf, "");
      strcat(buf2, buf);
      if (hh>0) sprintf(buf, " %d �p��", hh); else strcpy(buf, "");
      strcat(buf2, buf);
      if (mm>0) sprintf(buf, " %d ����", mm); else strcpy(buf, "");
      strcat(buf2, buf);
      if (strlen(buf2)==0)
        sprintf(buf, "����Ҧ��v���ܾ�\n�y%s ��� %s�z\n\n"
                    "�Y�N�}�l�A�зǳƤJ���C",
    		fmpks[fmpks_pos+1].guest_name,
    		fmpks[fmpks_pos+1].host_name);
      else
        sprintf(buf, "����Ҧ��v���ܾ�\n�y%s ��� %s�z\n\n"
                    "�w�w�N�b%s��}�l�C",
    		fmpks[fmpks_pos+1].guest_name,
    		fmpks[fmpks_pos+1].host_name,
    		buf2);
      lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
              	    	WINDOW_BUTTONTYPE_OK,
    			CHAR_WINDOWTYPE_CHECKMAN_END,
    			CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
    			buf);
    }
    break;
  case FMPKS_FLAG_MANOR_PEACE:
    // �԰��w�g�������M���ɴ�
    {
      int timeleft=fmpks[fmpks_pos+1].dueltime-NowTime.tv_sec;
      int dd,hh,mm;
      char buf2[4096];
      dd = /*fmpks[fmpks_pos+1].dueltime*/ timeleft / 86400;
      hh = /*fmpks[fmpks_pos+1].dueltime*/ timeleft / 3600 - dd*24;
      mm = /*fmpks[fmpks_pos+1].dueltime*/ timeleft / 60 - dd*24*60 - hh*60;
      strcpy(buf2,"");
      if (dd>0) sprintf(buf, " %d ��", dd); else strcpy(buf, "");
      strcat(buf2, buf);
      if (hh>0) sprintf(buf, " %d �p��", hh); else strcpy(buf, "");
      strcat(buf2, buf);
      if (mm>0) sprintf(buf, " %d ����", mm); else strcpy(buf, "");
      strcat(buf2, buf);

      if (strlen(buf2)==0)
        strcpy(buf, "�еy���A���ڷǳƤ@�U�ӽн��]�����C");
      else
        sprintf(buf, "�{�b�O��Ԯɴ��A�n���]����\n��%s��A�ӥӽСC",buf2);
      lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
              	    	WINDOW_BUTTONTYPE_OK,
    			CHAR_WINDOWTYPE_CHECKMAN_END,
    			CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX), buf);
    }
    break;
  case FMPKS_FLAG_MANOR_BATTLEBEGIN:
    // �ثe���b�i����]
    sprintf(buf, "����Ҧ��v���ܾ�\n�y%s ��� %s�z\n\n"
                 "�}�㥴��ù��\n�٨S�i�����H���ֶi���a�C",
    		fmpks[fmpks_pos+1].guest_name,
    		fmpks[fmpks_pos+1].host_name);
    lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
              	    	WINDOW_BUTTONTYPE_OK,
    			CHAR_WINDOWTYPE_CHECKMAN_END,
    			CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
    			buf);
    break;
  case FMPKS_FLAG_MANOR_OTHERPLANET:
    // �b�O���P�y�i��԰�
    sprintf(buf, "����Ҧ��v���ܾ�\n�y%s ��� %s�z\n\n"
    		 "�M���a�I�b %s �C",
              fmpks[fmpks_pos+1].guest_name,
              fmpks[fmpks_pos+1].host_name,
              fmpks[fmpks_pos+2].host_name);
    lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
              	    	WINDOW_BUTTONTYPE_OK,
    			CHAR_WINDOWTYPE_CHECKMAN_END,
    			CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
    			buf);
    break;
  }
}
#else
void NPC_ManorSmanTalked(int meindex, int talkerindex, char *msg, int color)
{
  char buf[4096];
  int fd;
  int fmpks_pos = CHAR_getWorkInt(meindex,NPC_WORK_ID)*MAX_SCHEDULE;
	int manorid = CHAR_getWorkInt(meindex,NPC_WORK_MANORID);

	if(CHAR_CHECKINDEX(meindex) == FALSE){
		printf("\nNPC_ManorSmanTalked error!(meindex:%d)",meindex);
		return;
	}
  if(NPC_Util_CharDistance(meindex,talkerindex) > 2) return;
  if(!NPC_Util_isFaceToFace(meindex,talkerindex,2)) return;
  fd = getfdFromCharaIndex(talkerindex);
  if(fd == -1) return;

	if(fmpks[fmpks_pos+1].flag == FMPKS_FLAG_CHALLENGE) fmpks[fmpks_pos+1].flag = FMPKS_FLAG_NONE;

  switch(fmpks[fmpks_pos+1].flag){
	// �D�Դ� 1800~2200
  case FMPKS_FLAG_NONE:
	case FMPKS_FLAG_WAIT:
    saacproto_ACFMPointList_send(acfd);
		sprintf(buf,"����Ҧ��v���ܾԪ��D�Ը��\n\n"
								"�@�B�S�����骺�a��\n"
								"�G�B�a�ڪ���հ������a�ڪ����\n"
								"�T�B���Ԯɶ��פ�ɡA��ձƦW�Ĥ@����o�D�Ը��\n"
								"�|�B��լۦP�̥H�̥����W����o���\n"
								"�� OK �ڱN�T�{�A�����A�� NO �[�ݬD�ԱƦW");
    lssproto_WN_send(fd,WINDOW_MESSAGETYPE_MESSAGE,WINDOW_BUTTONTYPE_YESNO,CHAR_WINDOWTYPE_CHECKMAN_START,
    								 CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),buf);
		if(fmpointlist.fm_momentum[manorid-1] <= -1){
			int hadfmindex,index;
			char token[256];

			getStringFromIndexWithDelim(fmpointlist.pointlistarray[manorid-1],"|",5,token,sizeof(token));
			hadfmindex = atoi(token);
			if(hadfmindex != -1){
				for(index=0;index<FAMILY_MAXNUM;index++){
					// ��X�u���a�ڪ�����
					if(fmdptop.fmtopid[index] == (hadfmindex - 1)) break;
				}
				if(index >= FAMILY_MAXNUM){
					getStringFromIndexWithDelim(fmpointlist.pointlistarray[manorid-1],"|",6,token,sizeof(token));
					printf("\nNPC_ManorSmanTalked():save fm_momentum error(%d:%s)",hadfmindex,token);
					break;
				}
				// �O���o�Ӯɨ�u���a�ڪ����
				else fmpointlist.fm_momentum[manorid-1] = fmdptop.fmMomentum[index];
			}
		}
		SortManorSchedule();
    break;
	// �D�Էǳƴ� 2200~���o�D�Ը�檺�a�ڷ�ɬD�Ԫ��ɶ�(�j�Ѫ� 1800~2200 ����)
  case FMPKS_FLAG_MANOR_PREPARE:
    // �ثe�w�g�w�n�԰��A�ǳƤ�
    {
      int timeleft = fmpks[fmpks_pos+1].dueltime - NowTime.tv_sec;
      int mm,hh,dd;
      char buf2[4096];

			dd = timeleft / 86400;
			hh = timeleft / 3600 - dd * 24;
      mm = timeleft / 60 - dd * 24 * 60 - hh * 60;
      memset(buf2,0,sizeof(buf2));
			if(dd > 0) sprintf(buf," %d ��",dd); else strcpy(buf,"");
      strcat(buf2,buf);
			if(hh > 0) sprintf(buf," %d �p��",hh); else strcpy(buf,"");
      strcat(buf2,buf);
      if(mm > 0) sprintf(buf," %d ����",mm); else strcpy(buf,"");
      strcat(buf2,buf);
      if(strlen(buf2) == 0)
        sprintf(buf,"����Ҧ��v���ܾ�\n�y%s ��� %s�z\n\n�Y�N�}�l�A�зǳƤJ���C",
    						fmpks[fmpks_pos+1].guest_name,fmpks[fmpks_pos+1].host_name);
      else
				sprintf(buf,"����Ҧ��v���ܾ�\n�y%s ��� %s�z\n\n�w�w�N�b%s��}�l�C",
    						fmpks[fmpks_pos+1].guest_name,fmpks[fmpks_pos+1].host_name,buf2);
      lssproto_WN_send(fd,WINDOW_MESSAGETYPE_MESSAGE,WINDOW_BUTTONTYPE_OK,CHAR_WINDOWTYPE_CHECKMAN_END,
    									 CHAR_getWorkInt(meindex,CHAR_WORKOBJINDEX),buf);
    }
    break;
	// ��Դ��ηǳƴ�,����
  case FMPKS_FLAG_MANOR_PEACE:
	case FMPKS_FLAG_MANOR_PEACE_SAVE:
    {
      int timeleft = fmpks[fmpks_pos+1].dueltime-NowTime.tv_sec;
      int dd,hh,mm;
      char buf2[4096];
      dd = timeleft / 86400;
      hh = timeleft / 3600 - dd*24;
      mm = timeleft / 60 - dd*24*60 - hh*60;
      memset(buf2,0,sizeof(buf2));
      if(dd > 0) sprintf(buf," %d ��",dd); else strcpy(buf,"");
      strcat(buf2,buf);
      if(hh > 0) sprintf(buf," %d �p��",hh); else strcpy(buf,"");
      strcat(buf2,buf);
      if(mm > 0) sprintf(buf," %d ����",mm); else strcpy(buf,"");
      strcat(buf2,buf);

      if(strlen(buf2) == 0) strcpy(buf, "�еy���A���ڷǳƤ@�U�ӽн��]�����C");
      else sprintf(buf, "�{�b�D�D�Ԯɴ��A�n�D�Ԫ���\n��%s��A�ӥӽСC",buf2);
      lssproto_WN_send(fd,WINDOW_MESSAGETYPE_MESSAGE,WINDOW_BUTTONTYPE_OK,CHAR_WINDOWTYPE_CHECKMAN_END,
											 CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),buf);
    }
    break;
	// �D�Ԥ�
  case FMPKS_FLAG_MANOR_BATTLEBEGIN:
    sprintf(buf,"����Ҧ��v���ܾ�\n�y%s ��� %s�z\n\n�}�㥴��ù��\n�٨S�i�����H���ֶi���a�C",
    				fmpks[fmpks_pos+1].guest_name,fmpks[fmpks_pos+1].host_name);
    lssproto_WN_send(fd,WINDOW_MESSAGETYPE_MESSAGE,WINDOW_BUTTONTYPE_OK,CHAR_WINDOWTYPE_CHECKMAN_END,
    								 CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),buf);
    break;
  case FMPKS_FLAG_MANOR_OTHERPLANET:
    // �b�O���P�y�i��԰�
    sprintf(buf,"����Ҧ��v���ܾ�\n�y%s ��� %s�z\n\n�M���a�I�b %s �C",
            fmpks[fmpks_pos+1].guest_name,fmpks[fmpks_pos+1].host_name,fmpks[fmpks_pos+2].host_name);
    lssproto_WN_send(fd,WINDOW_MESSAGETYPE_MESSAGE,WINDOW_BUTTONTYPE_OK,CHAR_WINDOWTYPE_CHECKMAN_END,
    								 CHAR_getWorkInt(meindex,CHAR_WORKOBJINDEX),buf);
    break;
  }
}
#endif

void NPC_ManorSmanWindowTalked(int meindex, int talkerindex,
				int seqno, int select, char *data)
{
  int fd, fmpks_pos, manorid, tkfmindex, tkfmdp;
  char buf[4096],token[256];
  int hadfmindex;
#ifndef _ACFMPK_LIST
  char hadfmname[256];
#endif
  int hadfmpopular;
  
  if (NPC_Util_CharDistance(meindex,talkerindex) > 2) return;
  if (!NPC_Util_isFaceToFace(meindex, talkerindex, 2)) return;
  fd = getfdFromCharaIndex(talkerindex);
  if (fd == -1) return;
  fmpks_pos = CHAR_getWorkInt(meindex, NPC_WORK_ID)*MAX_SCHEDULE;
  manorid = CHAR_getWorkInt(meindex, NPC_WORK_MANORID);
  tkfmindex = CHAR_getWorkInt(talkerindex, CHAR_WORKFMINDEXI);
  tkfmdp = CHAR_getWorkInt(talkerindex, CHAR_WORKFMDP);
  getStringFromIndexWithDelim(fmpointlist.pointlistarray[manorid-1], "|", 5, token, sizeof(token));
  hadfmindex = atoi(token);
  getStringFromIndexWithDelim(fmpointlist.pointlistarray[manorid-1], "|", 7, token, sizeof(token));
  hadfmpopular = atoi(token);
  
  switch (seqno){
  case CHAR_WINDOWTYPE_CHECKMAN_START:
    if (select==WINDOW_BUTTONTYPE_YES){
#ifdef _MANOR_PKRULE
			int StartTime = getHouFightWarTime();			//�}�l�U�Ѫ��ɶ�
			int EndTime = StartTime+getHouConWarTime();
			if(EndTime>24)EndTime = EndTime-24;
		  time_t timep;
		  struct tm *p;
		  p = localtime( (time_t *)&NowTime.tv_sec );
		 	sprintf(buf, "\n�{�b�O%d:%d:%d�C\n\n�Щ�%d:00��%d:00�A�Ӭ��ԧa�I",p->tm_hour,p->tm_min,p->tm_sec,StartTime,EndTime);
		  if(p->tm_hour<StartTime || p->tm_hour>EndTime){
			  lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
              	    	WINDOW_BUTTONTYPE_OK,
    			        CHAR_WINDOWTYPE_CHECKMAN_END,
    			        CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
    			        buf);
			  return;
		  }
#endif
#ifdef _ACFMPK_NOFREE
		sprintf(buf, "����D�Է|���@�Ѫ��ǳƴ�\n"
				"�A�i�H�H�ɸ߰ݧڥH�o���ѤU���ɶ�\n"
				"�D�Ե����ᤣ�׳ӭt�N�|���G�Ѫ���Դ�\n"
				"��Դ�������D��\n\n"
				"�A�T�w�n�D�Գo�Ӳ���ܡH");
		lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
				WINDOW_BUTTONTYPE_YESNO,
				CHAR_WINDOWTYPE_CHECKMAN_MAIN,
    			CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX), buf);
				return;
#endif

#ifdef _FMVER21      
		  if ((CHAR_getInt(talkerindex,CHAR_FMLEADERFLAG)==FMMEMBER_LEADER) &&
#else
      if ((CHAR_getInt(talkerindex,CHAR_FMLEADERFLAG)==1) &&
#endif      
				(CHAR_getWorkInt(talkerindex, CHAR_WORKFMSETUPFLAG)==1)){
        if (hadfmindex != -1) {
          if (hadfmindex-1 != tkfmindex){
            int check=0,i;
            // Arminius 2.25 fix: fmpks ���� 1~"MANORNUM" �դ@�w�n�� manorsman
            for (i=0; i<=/*3*/MANORNUM-1; i++) {	// 9�Ӳ���
              getStringFromIndexWithDelim(fmpointlist.pointlistarray[i], "|", 5, token, sizeof(token));
              if (tkfmindex==atoi(token)-1) check=1;
            }
            for (i=1; i<=/*4*/MANORNUM; i++) {	// �O�_�w�g�D�Ԩ�L����
              if ((fmpks[i*MAX_SCHEDULE+1].guest_index==tkfmindex) &&
								(strcmp(fmpks[i*MAX_SCHEDULE+1].guest_name,
								CHAR_getChar(talkerindex, CHAR_FMNAME))==0)
								) {
                check=2;
              }
            }
            if (check==0){
							int won1;
#ifdef _MANOR_PKRULE		    // WON ADD 2002/01/22
	#ifdef _NEW_MANOR_LAW
		//-----------------------------------------
							int index;
							won1 = 1;

							for(index=0;index<FAMILY_MAXNUM;index++){
								// ��X�D�Ԯa�ڪ�����
								if(fmdptop.fmtopid[index] == tkfmindex) break;
							}
							if(index >= FAMILY_MAXNUM){
								printf("\nNPC_ManorSmanWindowTalked():find tkfmIndex error (%d)",tkfmindex);
							}
							else
							// �p�G�D�Ԯa�ڮ�դ����u���a�ڮ�ժ��E��,����D��
							if(fmdptop.fmMomentum[index] < fmpointlist.fm_momentum[manorid-1]*0.9) won1 = 0;
		//-----------------------------------------
	#else
		//-----------------------------------------
							int i;
							char won2[256];
							won1 = 0;
		#ifdef _FIX_FAMILY_PK_LIMIT	   // WON ADD �N�i�D�Բ��骺�ѫe�Q�j�אּ�e�G�Q�j		
							for(i=0; i< PK_LIMIT; i++){
		#else
							for(i=0; i<10; i++){
		#endif
								if( getStringFromIndexWithDelim( fmdptop.topmemo[i], "|", 3, won2, sizeof(won2)) == FALSE )	{
									print("err Get fmdptop.topmemo[%d] if FALSE !!\n", i);
									break;
								}
								if( strcmp(CHAR_getChar(talkerindex, CHAR_FMNAME) ,won2) == 0 ){
									won1= 1;
									break;
								}
		//-----------------------------------------
							}
	#endif

							if(won1 == 1){		
			// WON END
	#ifdef _NEW_MANOR_LAW
							sprintf(buf,"�T�w����D�Ը���|���@�Ѫ��ǳƮɶ�\n"
													"�A�i�H�H�ɸ߰ݧڥH�o���ѤU���ɶ�\n"
													"�D�Ե�����ܤU���D�Դ��N�|���@�Ѫ���Դ�\n"
													"�M�|�Ѫ��ǳƴ��A�b�o���Ѵ���������D��\n\n"
													"�A�T�w�n�D�Գo�Ӳ���ܡH");
	#else
							sprintf(buf,"����D�Է|���@�Ѫ��ǳƴ�\n"
													"�A�i�H�H�ɸ߰ݧڥH�o���ѤU���ɶ�\n"
													"�D�Ե����ᤣ�׳ӭt�N�|���G�Ѫ���Դ�\n"
													"��Դ�������D��\n\n"
													"�A�T�w�n�D�Գo�Ӳ���ܡH");
	#endif
#else
              if(tkfmdp >= hadfmpopular){
								sprintf(buf,"����D�Է|���@�Ѫ��ǳƴ�\n"
                            "�A�i�H�H�ɸ߰ݧڥH�o���ѤU���ɶ�\n"
												    "�D�Ե����ᤣ�׳ӭt�N�|�����Ѫ���Դ�\n"
														"��Դ�������D��\n\n"
														"�A�T�w�n�D�Գo�Ӳ���ܡH");
#endif
                lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
              	    							WINDOW_BUTTONTYPE_YESNO,
    															CHAR_WINDOWTYPE_CHECKMAN_MAIN,
    															CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
    															buf);
    					}
							else{
#ifdef _NEW_MANOR_LAW
								sprintf(buf, "�A���a�ڮ�թ|����F����a�ڮ�ժ��E���A�ЦA�[�o�C");
#else
	#ifdef _MANOR_PKRULE
		#ifdef _FIX_FAMILY_PK_LIMIT	   // WON ADD �N�i�D�Բ��骺�ѫe�Q�j�אּ�e�G�Q�j
                sprintf(buf, "�A���a���n��|���i�J�e�G�Q�j�a�ڡA�ЦA�[�o�C");
		#else
                sprintf(buf, "�A���a���n��|���i�J�e�Q�j�a�ڡA�ЦA�[�o�C");
		#endif
	#else
                sprintf(buf, "�A���a���n��|�����H�D��\n"
                             "�֦��o�Ӳ��骺�a�ڡA�ЦA�[�o�C");
	#endif
#endif
                lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
						              	    	WINDOW_BUTTONTYPE_OK,
													   			CHAR_WINDOWTYPE_CHECKMAN_END,
												    			CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
													   			buf);
		   	      }
            }
						else if(check == 2){
              sprintf(buf, "�A���a�ڥ��b�D�Ԩ�L����A\n"
                           "�Ч���|�d����L�a�ڧa�I");
              lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
													    	WINDOW_BUTTONTYPE_OK,
											    			CHAR_WINDOWTYPE_CHECKMAN_END,
											    			CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
											    			buf);
            }
						else{
              sprintf(buf, "�@�Ӯa�ڥu��֦��@�Ӳ���A\n"
                           "�p�G����a�ڤ����ݭn���R\n"
                           "�Ш�a�ڢޢٳ��C");
              lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
              	    						WINDOW_BUTTONTYPE_OK,
											    			CHAR_WINDOWTYPE_CHECKMAN_END,
											    			CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
										    				buf);
            }
          }
					else{
            sprintf(buf, "�o�Ӳ���w�g�O�A���a�ڪ���C");
            lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
              						  	WINDOW_BUTTONTYPE_OK,
									    				CHAR_WINDOWTYPE_CHECKMAN_END,
											   			CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
										    			buf);
          }
        }
				else{
          sprintf(buf, "�{�b�èS������a�ڡA\n"
                       "�����h�ӽоE�J�N�i�H�F��C");
          lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
			              	    	WINDOW_BUTTONTYPE_OK,
    												CHAR_WINDOWTYPE_CHECKMAN_END,
									    			CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
									    			buf);
        }
      }
			else{
        sprintf(buf, "�u���ڪ��i�H�U�Ԯѳ�C");
        lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
              	    			WINDOW_BUTTONTYPE_OK,
									   			CHAR_WINDOWTYPE_CHECKMAN_END,
								    			CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
								    			buf);
      }
    }
#ifdef _NEW_MANOR_LAW
		else if(select == WINDOW_BUTTONTYPE_NO){
			int i,index,manorindex;
			char szMsg[3072];

			sprintf(buf,"              ����D�ԱƦW��\n�W�� �a�ڦW��         �ɶ�          ���\n");
			manorindex = CHAR_getWorkInt(meindex,NPC_WORK_MANORID) - 1;
			// �e�X�D�ԱƦW
			for(i=0;i<10;i++){
				index = ManorSchedule[manorindex].iSort[i];
				if(ManorSchedule[manorindex].iFmIndex[index] != -1){
					sprintf(szMsg,"%2d   %s\n",i+1,ManorSchedule[manorindex].szMemo[index]);
					strcat(buf,szMsg);
				}
			}
			lssproto_WN_send(fd,WINDOW_FMMESSAGETYPE_MANOR_SCHEDULE,WINDOW_BUTTONTYPE_OK,CHAR_WINDOWTYPE_CHECKMAN_END,
								    	 CHAR_getWorkInt(meindex,CHAR_WORKOBJINDEX),buf);
		}
#endif
    break;
  case CHAR_WINDOWTYPE_CHECKMAN_MAIN:
    if (select==WINDOW_BUTTONTYPE_YES) {
#ifdef _ACFMPK_NOFREE
	#ifndef _NEW_MANOR_LAW
		NPC_ManorSavePKSchedule(meindex, talkerindex, 0);
		sprintf( buf, "����D�Եn���T�{���A�еy��C");
		CHAR_talkToCli( talkerindex, meindex, buf, CHAR_COLORYELLOW);
	#else
		NPC_ManorAddToSchedule(meindex,talkerindex);
	#endif
		return;
#endif

#ifdef _FMVER21    
      if ((CHAR_getInt(talkerindex,CHAR_FMLEADERFLAG)==FMMEMBER_LEADER) && 
#else
      if ((CHAR_getInt(talkerindex,CHAR_FMLEADERFLAG)==1) &&     
#endif      
          (CHAR_getWorkInt(talkerindex, CHAR_WORKFMSETUPFLAG)==1) &&   
          (hadfmindex != -1) &&
		  (hadfmindex - 1 != tkfmindex)    
#ifndef _MANOR_PKRULE
          && (tkfmdp >= hadfmpopular)
#endif
		  ){
#ifndef _ACFMPK_LIST
		  struct tm tm1; 
#endif

#ifndef _NEW_MANOR_LAW
	#ifdef _MANOR_PKRULE
		  time_t timep;
		  struct tm *p;		  
		  time(&timep);
		  p = gmtime(&timep);			 
		 	sprintf(buf, "�{�b�O%2d�G%2d�G%2d�C�Щ󢰢��G�����ܢ����G�����A�Ӭ��ԧa�I",p->tm_hour,p->tm_min,p->tm_sec,);
		  if(p->tm_hour<18 || p->tm_hour>23){
			  //sprintf(buf, "�{�b�O%2d�G%2d�G%2d�C�Щ󢰢��G�����ܢ����G�����A�Ӭ��ԧa�I",p->tm_hour,p->tm_min,p->tm_sec,);
			  lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
              	    	WINDOW_BUTTONTYPE_OK,
    			        CHAR_WINDOWTYPE_CHECKMAN_END,
    			        CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
    			        buf);
			  break;
		  }
	#endif
			// WON ADD �ץ��a��pk�������԰��D
			if( fmpks[fmpks_pos+1].flag != FMPKS_FLAG_NONE ){
			     sprintf(buf, "�o�Ӳ���w�g���H���ԤF��C");
					 lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
						 WINDOW_BUTTONTYPE_OK,
						 CHAR_WINDOWTYPE_CHECKMAN_END,
						 CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
						 buf);
					 return;
			}
#endif
#ifdef _ACFMPK_LIST
	#ifndef _NEW_MANOR_LAW
		NPC_ManorSavePKSchedule(meindex, talkerindex, 0);
		sprintf( buf, "����D�Եn���T�{���A�еy��C");
		CHAR_talkToCli( talkerindex, meindex, buf, CHAR_COLORYELLOW);
	#else
		NPC_ManorAddToSchedule(meindex,talkerindex);
	#endif
#else
	    memcpy( &tm1, localtime( (time_t *)&NowTime.tv_sec), sizeof( tm1));
        getStringFromIndexWithDelim(fmpointlist.pointlistarray[manorid-1], "|", 6, token, sizeof(token));
        strcpy(hadfmname, token);

        fmpks[fmpks_pos+1].dueltime = CHAR_getWorkInt(meindex, NPC_WORK_CHALLENGEWAIT) + NowTime.tv_sec
                                      - tm1.tm_min*60 + 1800; // Arminius 11.1 �令�@�� xx:30 �}��
        fmpks[fmpks_pos+1].host_index = hadfmindex-1;
        strcpy(fmpks[fmpks_pos+1].host_name, hadfmname);
        fmpks[fmpks_pos+1].guest_index = tkfmindex;
        strcpy(fmpks[fmpks_pos+1].guest_name, CHAR_getChar(talkerindex, CHAR_FMNAME));        
        fmpks[fmpks_pos+1].prepare_time = 15;
        fmpks[fmpks_pos+1].max_player = 50;
        fmpks[fmpks_pos+1].win = -1;
        fmpks[fmpks_pos+1].flag = FMPKS_FLAG_MANOR_OTHERPLANET;
        strcpy(fmpks[fmpks_pos+2].host_name, getGameserverID());

	#ifndef _NEW_MANOR_LAW
		NPC_ManorSavePKSchedule(meindex, talkerindex, 0);
		sprintf(buf, "����D�Ԥw�g�]�w�����A�Цn�n�ǳơC");
        lssproto_WN_send( fd, WINDOW_MESSAGETYPE_MESSAGE,
              	    	WINDOW_BUTTONTYPE_OK,
    			CHAR_WINDOWTYPE_CHECKMAN_END,
    			CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),
    			buf);
	#else
		NPC_ManorAddToSchedule(meindex,talkerindex);
	#endif
    fmpks[fmpks_pos+1].flag = FMPKS_FLAG_MANOR_PREPARE;
#endif
        
      }
    }
    break;
  }
}

void NPC_CleanPkList( int ti)
{
// Terry fix �n�M��-1����M��0
/*	fmpks[ ti+1 ].host_index = 0;
	fmpks[ ti+1].guest_index=0;
	fmpks[ ti].host_index=0;
	fmpks[ ti].guest_index=0;*/
	fmpks[ ti+1 ].host_index = -1;
	fmpks[ ti+1].guest_index=-1;
	fmpks[ ti].host_index=-1;
	fmpks[ ti].guest_index=-1;

	strcpy(fmpks[ ti+1].host_name,"");
	strcpy(fmpks[ ti+1].guest_name,"");
	strcpy(fmpks[ ti].host_name,"");
	strcpy(fmpks[ ti].guest_name,"");
	strcpy(fmpks[ ti+2].host_name,"");
}

void NPC_ManorSmanLoop(int meindex)
{
	struct tm tm1;
	int fmpks_pos;
#ifdef _NEW_MANOR_LAW
	int iOffsetTime;
#endif

	if(CHAR_CHECKINDEX(meindex) == FALSE){
		printf("\nNPC_ManorSmanLoop error!(meindex:%d)",meindex);
		return;
	}

	fmpks_pos = CHAR_getWorkInt(meindex, NPC_WORK_ID)*MAX_SCHEDULE;
  memcpy( &tm1, localtime( (time_t *)&NowTime.tv_sec), sizeof( tm1));

  switch (fmpks[fmpks_pos+1].flag) {
  case FMPKS_FLAG_NONE:
#ifdef _NEW_MANOR_LAW
		{
			int hadfmindex,index,manorid;
			char token[256];
			//���ɥi�H�}�l�D��
			manorid = CHAR_getWorkInt(meindex,NPC_WORK_MANORID);
			getStringFromIndexWithDelim(fmpointlist.pointlistarray[manorid-1],"|",5,token,sizeof(token));
			hadfmindex = atoi(token);
			if(hadfmindex != -1){
				for(index=0;index<FAMILY_MAXNUM;index++){
					// ��X�u���a�ڪ�����
					if(fmdptop.fmtopid[index] == (hadfmindex - 1)) break;
				}
				if(index >= FAMILY_MAXNUM){
					getStringFromIndexWithDelim(fmpointlist.pointlistarray[manorid-1],"|",6,token,sizeof(token));
					printf("\nNPC_ManorSmanLoop():save fm_momentum error(%d:%s)",hadfmindex,token);
					break;
				}
				// �O���o�Ӯɨ�u���a�ڪ����
				else fmpointlist.fm_momentum[manorid-1] = fmdptop.fmMomentum[index];
				// �i�J�U�@�Ӫ��A
				fmpks[fmpks_pos+1].flag = FMPKS_FLAG_WAIT;
				// �D�Դ��@4�p��(1800~2200)
				#ifdef _JZ_MANORTES
				print("����D�Դ����A\n");
				fmpks[fmpks_pos+1].dueltime = NowTime.tv_sec + 60;
				#else
				fmpks[fmpks_pos+1].dueltime = NowTime.tv_sec + (3600 * getHouConWarTime());
				#endif
				NPC_ManorSavePKSchedule(meindex, -1, FMPKS_FLAG_WAIT,fmpks[fmpks_pos+1].dueltime,tm1);
			}
			SortManorSchedule();
		}
#endif    
    break;
#ifdef _NEW_MANOR_LAW
	// �i�J�D�Դ�,�w�O���a�ڮ��,���ݬD�ԱƵ{
	case FMPKS_FLAG_WAIT:
		{
			int manorid,i,iPlayerNum = CHAR_getPlayerMaxNum();
			char szMsg[256];

			manorid = CHAR_getWorkInt(meindex,NPC_WORK_MANORID);
			// �٨S�}��,��է��ܭn�h�VAC�n�Y�ɸ��
			fmpointlist.fm_inwar[manorid-1] = TRUE;
			if(fmpks[fmpks_pos+1].dueltime <= NowTime.tv_sec){
				// �D�Ԯa�ڤ���ճ̰�����o�D���v
				int manorindex = CHAR_getWorkInt(meindex,NPC_WORK_MANORID) - 1;
				int iNo1FmIndexSort = ManorSchedule[manorindex].iSort[0];
				int index = ManorSchedule[manorindex].iFmIndex[iNo1FmIndexSort],iFmIndex1,iCharindex;
				char token[256],fmname[256];
				// �S�H�D��
				if(index < 0){
					// �����i�J��D�Ե���
					fmpks[fmpks_pos+1].flag = FMPKS_FLAG_MANOR_BATTLEEND;
					CHAR_setWorkInt(meindex,NPC_WORK_BETTLETIME,tm1.tm_mday);
					// �q���Ҧ��H
					for(i=0;i<iPlayerNum;i++){
						if(CHAR_getCharUse(i) != FALSE){
							CHAR_talkToCli(i,-1,"�ѩ�L�a�ڬD�Բ���A����i�J��Ԯɴ�",CHAR_COLORBLUE2);
						}
					}
					getStringFromIndexWithDelim(fmpointlist.pointlistarray[manorid-1],"|",5,token,sizeof(token));
					iFmIndex1 = atoi(token) - 1;
					if(iFmIndex1 != -1){
						getStringFromIndexWithDelim(fmpointlist.pointlistarray[manorid-1],"|",6,fmname,sizeof(token));
						saacproto_ACFixFMPoint_send(acfd,fmname,iFmIndex1 + 1,iFmIndex1,
							fmname,iFmIndex1 + 1,iFmIndex1,CHAR_getWorkInt(meindex,NPC_WORK_ID));
						// �W�[����ԳӭtLog
						sprintf(token," (%d:%d) %d/%d/%d",tm1.tm_hour,tm1.tm_min,tm1.tm_year+1900,tm1.tm_mon+1,tm1.tm_mday);
						Logfmpk(fmname,iFmIndex1,-1,"�L�H�D��",-1,-1,token,"","",2);
						// ��a�ڦu��F����,�a�ڦ����i�o��۹�
						for(i=0;i<FAMILY_MAXMEMBER;i++){
							iCharindex = familyMemberIndex[iFmIndex1][i];
							// �Y�b�u�W�~���� 
							if(iCharindex >= 0 && CHAR_getCharUse(iCharindex)){
								// ��o���� = �ӤH��� * 5000
								int iAddGold = ((float)CHAR_getInt(iCharindex,CHAR_MOMENTUM)/100.0f) * 5000.0f;
								int iGold = CHAR_getInt(iCharindex,CHAR_BANKGOLD),iMaxGold;
								// ����J�ӤH�Ȧ�
								if(iGold + iAddGold > CHAR_MAXBANKGOLDHAVE){
									CHAR_setInt(iCharindex,CHAR_BANKGOLD,CHAR_MAXBANKGOLDHAVE);
									// �ӤH�Ȧ�񤣤U�F,���ӤH���W
									iAddGold = iGold + iAddGold - CHAR_MAXBANKGOLDHAVE;
									iGold = CHAR_getInt(iCharindex,CHAR_GOLD);
									iMaxGold = CHAR_getMaxHaveGold(iCharindex);
									if(iGold + iAddGold > iMaxGold) CHAR_setInt(iCharindex,CHAR_GOLD,iMaxGold);
									else CHAR_setInt(iCharindex,CHAR_GOLD,iGold + iAddGold);
									LogFMPKGetMomey(CHAR_getChar(iCharindex,CHAR_FMNAME),
																	CHAR_getChar(iCharindex,CHAR_CDKEY),
																	CHAR_getChar(iCharindex,CHAR_NAME),
																	CHAR_getInt(iCharindex,CHAR_MOMENTUM),iAddGold,0);
								}
								else{
									CHAR_setInt(iCharindex,CHAR_BANKGOLD,iGold + iAddGold);
									LogFMPKGetMomey(CHAR_getChar(iCharindex,CHAR_FMNAME),
																	CHAR_getChar(iCharindex,CHAR_CDKEY),
																	CHAR_getChar(iCharindex,CHAR_NAME),
																	CHAR_getInt(iCharindex,CHAR_MOMENTUM),iAddGold,1);
								}
								CHAR_talkToCli(iCharindex,-1,"���W�F!�u�@����骺�����w�פJ�A���ӤH�Ȧ�",CHAR_COLORRED);
								// �a�ڪ��ӤH�ήa�ڮ�ճ��n�k�s
								CHAR_setInt(iCharindex,CHAR_MOMENTUM,0);
								CHAR_talkToCli(iCharindex,-1,"����ԫ�ӤH�ήa�ڮ���k�s",CHAR_COLORRED);
							}
						}
					}
				}
				else{
					// �D�Ԯɶ��w�L,��X���D�Ԯa�کM�u���a�ڦ����@�Ѫ��ɶ��i�H�ǳ�
					NPC_ManorSavePKSchedule(meindex,index,0,-1,ManorSchedule[manorindex].tm1[iNo1FmIndexSort]);
					// �}���F,��է��ܤ]���Υh�VAC�n�Y�ɸ�ƤF
					fmpointlist.fm_inwar[manorid-1] = FALSE;
					// �q���Ҧ��H
					sprintf(szMsg,"%s �a����o�D�Բ�����",ManorSchedule[manorindex].szFmName[iNo1FmIndexSort]);
					
					for(i=0;i<iPlayerNum;i++){
						if(CHAR_getCharUse(i) != FALSE){
							CHAR_talkToCli(i,-1,szMsg,CHAR_COLORBLUE2);
						}
					}
				}
			}
		}
		break;
#endif
  case FMPKS_FLAG_MANOR_PREPARE:
    if(fmpks[fmpks_pos+1].dueltime <= NowTime.tv_sec){
      memcpy(&fmpks[fmpks_pos], &fmpks[fmpks_pos+1], sizeof(FamilyPKSchedule));
      fmpks[fmpks_pos].dueltime = tm1.tm_hour * 100 + tm1.tm_min;
      fmpks[fmpks_pos].flag = FMPKS_FLAG_SCHEDULED;
      fmpks[fmpks_pos+1].dueltime = 0;
      fmpks[fmpks_pos+1].flag = FMPKS_FLAG_MANOR_BATTLEBEGIN;
      NPC_talkToFloor(CHAR_getInt(meindex, CHAR_FLOOR) , fmpks[fmpks_pos].host_index,
         	fmpks[fmpks_pos].guest_index, "���骧�ܾԤw�g�}�l�A�кɧ֤J���I");
#ifdef _NEW_MANOR_LAW
			CHAR_setWorkInt(meindex,NPC_WORK_BETTLETIME,tm1.tm_mday);
#endif
    }
    break;
  case FMPKS_FLAG_MANOR_PEACE:
    if (fmpks[fmpks_pos+1].dueltime <= NowTime.tv_sec) {
      fmpks[fmpks_pos+1].dueltime = 0;
      fmpks[fmpks_pos+1].flag = FMPKS_FLAG_NONE;
#ifndef _NEW_MANOR_LAW
      NPC_ManorSavePKSchedule(meindex, -1, FMPKS_FLAG_NONE);
#else
			NPC_ManorSavePKSchedule(meindex, -1, FMPKS_FLAG_NONE,fmpks[fmpks_pos+1].dueltime,tm1);
#endif
    }
    break;
  case FMPKS_FLAG_MANOR_BATTLEBEGIN:
    break;
  case FMPKS_FLAG_MANOR_BATTLEEND:
		{
#ifdef _NEW_MANOR_LAW
			int manorindex = CHAR_getWorkInt(meindex,NPC_WORK_MANORID) - 1;
#endif

			fmpks[fmpks_pos].flag = FMPKS_FLAG_NONE;
#ifndef _NEW_MANOR_LAW
			fmpks[fmpks_pos+1].dueltime = CHAR_getWorkInt(meindex, NPC_WORK_PEACEWAIT) + NowTime.tv_sec;
#else
			// ������n�A�L���Ѥ~��A�D��
			// �Y�}��������M�����԰���������@�˪�ܥ��W�L�F�@�ѥH�W,�H�@�ѭp
			if(CHAR_getWorkInt(meindex,NPC_WORK_BETTLETIME) != tm1.tm_mday){
				iOffsetTime = tm1.tm_hour * 3600 * -1;	// ���^00:00
			}
			else{	// �԰������ɥ��W�L�@��
				iOffsetTime = (24 - tm1.tm_hour) * 3600;	// �ɦ�00:00
			}
			//#ifdef _75_TEST
			fmpks[fmpks_pos+1].dueltime = NowTime.tv_sec - tm1.tm_min*60 + iOffsetTime + 172800 + (18 * 3600);	// �G��
			//#else
			//	fmpks[fmpks_pos+1].dueltime = NowTime.tv_sec - tm1.tm_min*60 + iOffsetTime + 432000 + (18 * 3600);
			//														|-----------�����I------------| |-�ɦ�00:00-| |-����-|  |18:00�}�l���W|
			//#endif
#endif
			fmpks[fmpks_pos+1].flag = FMPKS_FLAG_MANOR_PEACE_SAVE;
			NPC_CleanPkList( fmpks_pos);
#ifndef _NEW_MANOR_LAW
			NPC_ManorSavePKSchedule(meindex, -1, FMPKS_FLAG_MANOR_PEACE_SAVE);
#else
			NPC_ManorSavePKSchedule(meindex, -1, FMPKS_FLAG_MANOR_PEACE_SAVE,fmpks[fmpks_pos+1].dueltime,tm1);
			// �M�űƵ{
			memset(&ManorSchedule[manorindex],0,sizeof(ManorSchedule[manorindex]));
			{
				int i;
				for(i=0;i<10;i++) ManorSchedule[manorindex].iSort[i] = ManorSchedule[manorindex].iFmIndex[i] = -1;
				for(i=0;i<FAMILY_MAXHOME;i++) fmpointlist.fm_momentum[i] = -1;
			}
		}
#endif
    break;
  case FMPKS_FLAG_MANOR_PEACE_SAVE:
    fmpks[fmpks_pos+1].flag = FMPKS_FLAG_MANOR_PEACE;
    break;
  case FMPKS_FLAG_MANOR_OTHERPLANET:
    break;
  case FMPKS_FLAG_MANOR_READYTOFIGHT:
  case FMPKS_FLAG_MANOR_CLEANFLAG:
    fmpks[fmpks_pos].flag = FMPKS_FLAG_NONE;
    fmpks[fmpks_pos+1].dueltime = CHAR_getWorkInt(meindex, NPC_WORK_PEACEWAIT) + NowTime.tv_sec;
    fmpks[fmpks_pos+1].flag = FMPKS_FLAG_NONE;

		NPC_CleanPkList( fmpks_pos);
#ifndef _NEW_MANOR_LAW
    NPC_ManorSavePKSchedule(meindex, -1, FMPKS_FLAG_NONE);
#else
		NPC_ManorSavePKSchedule(meindex, -1, FMPKS_FLAG_NONE,fmpks[fmpks_pos+1].dueltime,tm1);
#endif
    break;
  }
  CHAR_setWorkInt(meindex, NPC_WORK_PREVLOOPTIME, NowTime.tv_sec);
}

void NPC_ManorLoadPKSchedule(int meindex)
{
#ifdef _ACFMPK_LIST
	int fmpks_pos = CHAR_getWorkInt(meindex, NPC_WORK_ID);
	saacproto_ACLoadFmPk_send(acfd, fmpks_pos);
#else
	char filename[256], tmp[4096], token[256];
	FILE *f;
	int fmpks_pos = CHAR_getWorkInt(meindex, NPC_WORK_ID)*MAX_SCHEDULE;

	snprintf(filename,sizeof(filename), "%s%d_%d_%d",
		       SCHEDULEFILEDIR, CHAR_getInt(meindex, CHAR_FLOOR),
			   CHAR_getInt(meindex, CHAR_X),
			CHAR_getInt(meindex, CHAR_Y) );

	if( ! (f=fopen( filename, "r" )) ){	// create new schedule file
		f = fopen( filename, "w" );
		if( !f ){
			print( "ERROR:Can't create Schedule file %s!\n",filename );
			return;
		}
		strcpy(tmp,"0|-1|-1|-1|-1|-1|-1|-1|-1\n");
		fwrite(tmp, strlen(tmp), 1, f);
	}

	fseek(f, 0, SEEK_SET);	// �ɮ׶}�Y
	fgets(tmp, sizeof(tmp), f);
	fmpks[fmpks_pos].flag=-1;

	// �ɶ�
	if (getStringFromIndexWithDelim(tmp,"|",1,token,sizeof(token))) {
		fmpks[fmpks_pos+1].dueltime=atoi(token);
	}
	// �D�� familyindex
	if (getStringFromIndexWithDelim(tmp,"|",2,token,sizeof(token))) {
	    fmpks[fmpks_pos+1].host_index=atoi(token);
	}
	// �D�� �a�ڦW
	if (getStringFromIndexWithDelim(tmp,"|",3,token,sizeof(token))) {
		strcpy(fmpks[fmpks_pos+1].host_name,makeStringFromEscaped(token));
	}
	// �ȶ� familyindex
	if (getStringFromIndexWithDelim(tmp,"|",4,token,sizeof(token))) {
	    fmpks[fmpks_pos+1].guest_index=atoi(token);
	}
	// �ȶ� �a�ڦW
	if (getStringFromIndexWithDelim(tmp,"|",5,token,sizeof(token))) {
	    strcpy(fmpks[fmpks_pos+1].guest_name,makeStringFromEscaped(token));
	}
	// �ǳƮɶ�
	if (getStringFromIndexWithDelim(tmp,"|",6,token,sizeof(token))) {
		fmpks[fmpks_pos+1].prepare_time=atoi(token);
	}
	// �̤j�H��
	if (getStringFromIndexWithDelim(tmp,"|",7,token,sizeof(token))) {
		fmpks[fmpks_pos+1].max_player=atoi(token);
	}
	// �X��
	if (getStringFromIndexWithDelim(tmp,"|",8,token,sizeof(token))) {
		fmpks[fmpks_pos+1].flag=atoi(token);
	}
	// ��ԬP�y
	if (getStringFromIndexWithDelim(tmp,"|",9,token,sizeof(token))) {
		strcpy(fmpks[fmpks_pos+2].host_name,makeStringFromEscaped(token));
	}
  
	// no schedule
	fmpks[fmpks_pos].flag=-1;
	// prepare time expired?
	if ((fmpks[fmpks_pos+1].flag==FMPKS_FLAG_MANOR_PREPARE) &&
		  (fmpks[fmpks_pos+1].dueltime<NowTime.tv_sec)) {
		fmpks[fmpks_pos].flag=-1;
// Terry fix �n�M��-1����M��0
//		fmpks[fmpks_pos].host_index=0;
		fmpks[fmpks_pos].host_index=-1;
		strcpy(fmpks[fmpks_pos].host_name,"");
// Terry fix �n�M��-1����M��0
//		fmpks[fmpks_pos].guest_index=0;
		fmpks[fmpks_pos].guest_index=-1;
		strcpy(fmpks[fmpks_pos].guest_name,"");
		fmpks[fmpks_pos+1].flag=-1;
		fmpks[fmpks_pos+1].dueltime=0;
	//    fmpks[fmpks_pos+1].dueltime=NowTime.tv_sec+30*60;	// add more 30 minutes
	}
	fclose(f);
#endif // end AC_SEND_FM_PK

}

#ifdef _ACFMPK_LIST
//AC�����Ԫ��A��s
#ifndef _NEW_MANOR_LAW
void NPC_ManorSavePKSchedule(int meindex, int toindex, int flg)
#else
void NPC_ManorSavePKSchedule(int meindex, int toindex, int flg,int setTime,struct tm tm2)
#endif
{
	int fmpks_pos, manorid, dueltime;
	char msg[1024], n1[256], n2[256], n3[256];
	char token[256], hadfmname[256];
	int hadfmindex=0, hadfmpopular=0, tkfmindex=0;
	int PkFlg=0;

	if( toindex == -1 ){
#ifndef _NEW_MANOR_LAW
	#ifndef _JZ_MANOR_FIX_DFW
			int iOffsetTime;
			// ������n�A�L���Ѥ~��A�D��
			// �Y�}��������M�����԰���������@�˪�ܥ��W�L�F�@�ѥH�W,�H�@�ѭp
			if(CHAR_getWorkInt(meindex,NPC_WORK_BETTLETIME) != tm1.tm_mday){
				iOffsetTime = tm1.tm_hour * 3600 * -1;	// ���^00:00
			}
			else{	// �԰������ɥ��W�L�@��
				iOffsetTime = (24 - tm1.tm_hour) * 3600;	// �ɦ�00:00
			}
			//#ifdef _75_TEST
			dueltime = NowTime.tv_sec - tm1.tm_min*60 + iOffsetTime + CHAR_getWorkInt(meindex, NPC_WORK_PEACEWAIT) + (getHouFightWarTime() * 3600);	// �G��
			//#else
			//	fmpks[fmpks_pos+1].dueltime = NowTime.tv_sec - tm1.tm_min*60 + iOffsetTime + 432000 + (18 * 3600);
			//														|-----------�����I------------| |-�ɦ�00:00-| |-����-|  |18:00�}�l���W|
			//#endif
	#else
		dueltime = CHAR_getWorkInt(meindex, NPC_WORK_PEACEWAIT) + NowTime.tv_sec;
	#endif
		sprintf(msg, "%d|%d|%s|%d|%s|%d|%d|%d|%s",
			dueltime, 0, "", 0, "", 0, 0, flg, "");
#else
		sprintf(msg, "%d|%d|%s|%d|%s|%d|%d|%d|%s",
			setTime, 0, "", 0, "", 0, 0, flg, "");
#endif
		PkFlg = 0;
	}else{
		struct tm tm1;
		int index;
#ifndef _NEW_MANOR_LAW
		tkfmindex = CHAR_getWorkInt(toindex, CHAR_WORKFMINDEXI);
#else
		// toindex �ܦ� �a�ڪ�index
		tkfmindex = toindex;
#endif
		fmpks_pos = CHAR_getWorkInt(meindex, NPC_WORK_ID) * MAX_SCHEDULE;
		manorid = CHAR_getWorkInt(meindex, NPC_WORK_MANORID);
		memcpy( &tm1, localtime( (time_t *)&NowTime.tv_sec), sizeof( tm1));
#ifndef _NEW_MANOR_LAW
		dueltime = CHAR_getWorkInt(meindex, NPC_WORK_CHALLENGEWAIT) + NowTime.tv_sec - tm1.tm_min*60 + 1800;
#else
		// �H�D�Ԯɪ��ɶ����}�����ɶ�,�����Ҹg�L���ɶ����ǳƮɶ�
		dueltime = NowTime.tv_sec - tm1.tm_min * 60 + ((24 - tm1.tm_hour) * 3600) + 3600 * tm2.tm_hour;
		//				|-------------�����I-------------|  |--------�[��0000��-------|  |�D�Ԯɪ��ɶ�(����I)| 
#endif
		getStringFromIndexWithDelim( fmpointlist.pointlistarray[manorid-1], "|", 5, token, sizeof(token));
		hadfmindex = atoi( token);
		getStringFromIndexWithDelim( fmpointlist.pointlistarray[manorid-1], "|", 7, token, sizeof(token));
		hadfmpopular = atoi( token);
		getStringFromIndexWithDelim( fmpointlist.pointlistarray[manorid-1], "|", 6, token, sizeof(token));
		strcpy( hadfmname, token);
		makeEscapeString( hadfmname, n1, sizeof(n1));
#ifndef _NEW_MANOR_LAW
		makeEscapeString( CHAR_getChar(toindex, CHAR_FMNAME), n2, sizeof(n2));		
#else
		// ��X�D�Ԯa�ڤ���ճ̰��a�گ���
		index = ManorSchedule[manorid-1].iSort[0];
		makeEscapeString(ManorSchedule[manorid-1].szFmName[index],n2,sizeof(n2));
#endif
		makeEscapeString( getGameserverID(), n3, sizeof(n3));

		sprintf(msg, "%d|%d|%s|%d|%s|%d|%d|%d|%s",
			dueltime, hadfmindex-1, n1, tkfmindex, n2, 15, 50, FMPKS_FLAG_MANOR_OTHERPLANET, n3);
		PkFlg = 1;
		{
			char buf1[256];
			sprintf( buf1,"%d", CHAR_getWorkInt( meindex, NPC_WORK_ID) );
			Logfmpk(
				 n1, hadfmindex-1, 0,
				 n2, tkfmindex, 0,
				 "", buf1, n3, 1);
		}
	}
#ifndef _NEW_MANOR_LAW
	saacproto_ACSendFmPk_send( acfd, toindex, PkFlg, CHAR_getWorkInt( meindex, NPC_WORK_ID), msg);
#else
	// toindex �Τ���]�� -1
	saacproto_ACSendFmPk_send( acfd, -1, PkFlg, CHAR_getWorkInt( meindex, NPC_WORK_ID), msg);
#endif
}

#endif

#ifdef _NEW_MANOR_LAW
// �[�J�D�ԱƵ{
void NPC_ManorAddToSchedule(int meindex,int charaindex)
{
	int i,j,iEmpty = -1,iFmIndex,manorindex = CHAR_getWorkInt(meindex,NPC_WORK_MANORID) - 1;
	struct tm tm1;

	iFmIndex = CHAR_getWorkInt(charaindex,CHAR_WORKFMINDEXI);
	// �ˬd�O�_�w�ƤJ�Ƶ{,�ˬd�Ҧ����������
	for(j=0;j<MANORNUM;j++){
		for(i=0;i<10;i++){
			if(ManorSchedule[j].iFmIndex[i] != -1){
				if(ManorSchedule[j].iFmIndex[i] == iFmIndex){
					lssproto_WN_send(getfdFromCharaIndex(charaindex),WINDOW_MESSAGETYPE_MESSAGE,WINDOW_BUTTONTYPE_OK,CHAR_WINDOWTYPE_CHECKMAN_END,
						CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),"�A�w�g���W�L�D�Բ���F���");
					return;
				}
			}
			else{
				if(j == manorindex) iEmpty = i;	// �O���Ū���m
			}
		}
	}
	// �p�G�{�������o��,��ܨS���[�J�Ƶ{,�ˬd���S���Ū���m
	if(iEmpty != -1){ // ���Ŧ�m,����W�̥[�J
		// �O���a�گ���
		ManorSchedule[manorindex].iFmIndex[iEmpty] = iFmIndex;
		// �O���a�ڦW��
		sprintf(ManorSchedule[manorindex].szFmName[iEmpty],"%s",CHAR_getChar(charaindex,CHAR_FMNAME));
		// �O���D�Ԯɶ�
		memcpy(&tm1,localtime((time_t *)&NowTime.tv_sec),sizeof(tm1));
		memset(ManorSchedule[manorindex].szMemo[iEmpty],0,sizeof(ManorSchedule[manorindex].szMemo[iEmpty]));
		memcpy(&ManorSchedule[manorindex].tm1[iEmpty],&tm1,sizeof(tm1));
		lssproto_WN_send(getfdFromCharaIndex(charaindex),WINDOW_MESSAGETYPE_MESSAGE,WINDOW_BUTTONTYPE_OK,CHAR_WINDOWTYPE_CHECKMAN_END,
				CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),"�[�J�D�ԱƵ{");
	}
	// ���F,�M�D�ԱƵ{���̫�@�W�@���,�p�G��Ӫ���դ�̫�@�W��,�������̫�@�W,�_�h�q���ثe��յL�k���i�Ƶ{
	else{
		// ���o�̫�@�W�bManorSchedule����m
		int index = ManorSchedule[manorindex].iSort[9];

		for(i=0;i<FAMILY_MAXNUM;i++){
			// ��X�D�Ԯa�ڪ�����
			if(fmdptop.fmtopid[i] == iFmIndex) break;
		}
		// �䤣�즹�a��
		if(i >= FAMILY_MAXNUM){
			lssproto_WN_send(getfdFromCharaIndex(charaindex),WINDOW_MESSAGETYPE_MESSAGE,WINDOW_BUTTONTYPE_OK,CHAR_WINDOWTYPE_CHECKMAN_END,
				CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),"��p�A�䤣��A���a�ڮ�ո��");
			return;
		}
		// ������
		if(ManorSchedule[manorindex].iFmMomentum[index] < fmdptop.fmMomentum[i]){
			int fmpks_pos,iPlayerNum = CHAR_getPlayerMaxNum();
			char szMsg[256];

			// �q���Ҧ��H
			fmpks_pos = CHAR_getWorkInt(meindex,NPC_WORK_ID) * MAX_SCHEDULE;
			sprintf(szMsg,"%s �a�ڳQ %s �a�����U�D�ԱƵ{",ManorSchedule[manorindex].szFmName[index],CHAR_getChar(charaindex,CHAR_FMNAME));
			for(i=0;i<iPlayerNum;i++){
				if(CHAR_getCharUse(i) != FALSE){
					CHAR_talkToCli(i,-1,szMsg,CHAR_COLORBLUE2);
				}
			}
			// �̫�@�W�Q����
			ManorSchedule[manorindex].iFmIndex[index] = iFmIndex;
			// �O���a�ڦW��
			sprintf(ManorSchedule[manorindex].szFmName[index],"%s",CHAR_getChar(charaindex,CHAR_FMNAME));
			// �O���D�Ԯɶ�
			memcpy(&tm1,localtime((time_t *)&NowTime.tv_sec),sizeof(tm1));
			memcpy(&ManorSchedule[manorindex].tm1[index],&tm1,sizeof(tm1));
			lssproto_WN_send(getfdFromCharaIndex(charaindex),WINDOW_MESSAGETYPE_MESSAGE,WINDOW_BUTTONTYPE_OK,CHAR_WINDOWTYPE_CHECKMAN_END,
				CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),"�[�J�D�ԱƵ{");
		}
		else{
			lssproto_WN_send(getfdFromCharaIndex(charaindex),WINDOW_MESSAGETYPE_MESSAGE,WINDOW_BUTTONTYPE_OK,CHAR_WINDOWTYPE_CHECKMAN_END,
				CHAR_getWorkInt(meindex, CHAR_WORKOBJINDEX),"��p�A�A���a�ڮ�դ����H�ƶi�D�ԱƵ{");
			return;
		}
	}
	// �Ƨ�
	SortManorSchedule();
}

// �ƧǬD�ԱƵ{
void SortManorSchedule()
{
	int i,j,k;

	// �����o�̷s�a�ڮ��
	for(k=0;k<MANORNUM;k++){
		for(i=0;i<10;i++){
			ManorSchedule[k].iSort[i] = i;
			if(ManorSchedule[k].iFmIndex[i] != -1){
				for(j=0;j<FAMILY_MAXNUM;j++){
					// ��X�D�Ԯa�ڪ�����
					if(fmdptop.fmtopid[j] == ManorSchedule[k].iFmIndex[i]){
						// ���o��խ�
						ManorSchedule[k].iFmMomentum[i] = fmdptop.fmMomentum[j];
						// �@��memo
						sprintf(ManorSchedule[k].szMemo[i],"%-16s %2d:%2d %12d",
							ManorSchedule[k].szFmName[i],ManorSchedule[k].tm1[i].tm_hour,ManorSchedule[k].tm1[i].tm_min,
							ManorSchedule[k].iFmMomentum[i]/100);
						break;
					}
				}
			}
		}
	}
	// �Ƨ�
	for(i=0;i<MANORNUM;i++){
		g_iSortManor = i;
		qsort(&ManorSchedule[g_iSortManor].iSort,10,sizeof(int),&SortManorScheduleMomentum);
	}
}

int SortManorScheduleMomentum(const void *indexa, const void *indexb)
{
  int momentumA,momentumB;
  
  momentumA = ManorSchedule[g_iSortManor].iFmMomentum[*(int*)indexa];
  momentumB = ManorSchedule[g_iSortManor].iFmMomentum[*(int*)indexb];
  return (momentumA > momentumB) ? -1:1;
}
#endif
