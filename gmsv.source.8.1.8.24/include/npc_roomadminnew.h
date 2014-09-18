#ifndef __NPC_ROOMADMINNEW_H__
#define __NPC_ROOMADMINNEW_H__


void NPC_RoomAdminNewTalked( int meindex , int talkerindex , char *msg , int color );
void NPC_RoomAdminNewLoop( int meindex );
BOOL NPC_RoomAdminNewInit( int meindex );

BOOL NPC_RankingInit( int meindex );
void NPC_RankingTalked( int meindex , int talkerindex , char *msg ,int color );

BOOL NPC_PrintpassmanInit( int meindex );
void NPC_PrintpassmanTalked( int meindex , int talkerindex , char *msg ,int color );

#if 0
typedef struct roomadmin_tag
{
    int index;              /*   ����index */
    
    char doorname[256];     /* ����  ���Υɥ���  �� */
    char explanation[256];  /*      */
    char passwd[256];       /* �ѥ���� */
    int expire_time_mod;    /* ���� */
    time_t expire_time;     /* �����ڤ�λ��� */
    int least_cost;         /*   ���� */
    
}NPC_ROOMINFO;
#endif

typedef struct npc_roomadminnew_tag {
	int		expire;
	char	cdkey[CDKEYLEN];
	char	charaname[32];
	char	passwd[9];
}NPC_ROOMINFO;

typedef struct npc_roomadminnew_ranking_tag {
	int		gold;				/*   ����� */
	int		biddate;			/*   ������ */
	char	cdkey[CDKEYLEN];	/*   �Ȥ����ͤ�    ���� */
	char	charaname[32];		/*   �Ȥ����ͤ�  �� */
	char	owntitle[32];		/* �ι�*/
	
}NPC_RANKING_INFO;

#endif /*__NPC_ROOMADMINNEW_H__*/

BOOL NPC_RoomAdminNew_ReadFile( char *roomname, NPC_ROOMINFO *data);
