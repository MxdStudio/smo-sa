#ifndef _NPCS_HANDLE_H
#define _NPCS_HANDLE_H
#include "version.h"
#include "util.h"

#ifdef _NPCSERVER_NEW
//����P�_
BOOL NPCS_HandleCheckFreeMess( int npcobjindex, int charaindex, int charobjindex,
				char *CheckfreeMess);

BOOL NPCS_FreePassCheck( int charaindex, char *buf);

//����P�_
BOOL NPCS_FreeBigSmallCheck( int charaindex,char* buf);
//�ѼƧP�_
BOOL NPCS_ArgumentFreeCheck( int charaindex, char* Argument, int amount, int temp, int probjID);
//�ѼƤj�p�P�_
BOOL NPCS_ArgumentBigSmallCheck( int point, int mypoint, int flg);
//���o���wID�d����	*reAmount �Ѿl���
int NPCS_getUserPetAmount( int charaindex, int objID, int flg);
//���o���wID�D���	*reAmount �Ѿl���
int NPCS_getUserItemAmount( int charaindex, int objID, int flg);
//���o�ζ��H��
int NPCS_getUserPartyAmount( int charaindex );
BOOL NPCS_NpcstalkToCli( int charaindex,int npcobjindex, char* message, CHAR_COLOR color );
//�ƥ�B�z
BOOL NPCS_RunDoEventAction( int charaindex, char *buf1);
#endif
#endif
