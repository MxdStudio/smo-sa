#ifndef __NPC_AUCTIONEER_H__
#define __NPC_AUCTIONEER_H__

#ifdef _AUCTIONEER

// �H�U�o��ӱ`�ƭn�� saac/auction.h �̭����w�q�@�P
#define MAX_AUCTION	100	// �̤j���e�U��
#define MAX_OVERDUE	500	// �L�����e�U��O�d�� (�̤p�]�w=�L���O�d�Ѽ�*�̤j�e�U��)
#define AUC_PET     1
#define AUC_ITEM    2

typedef struct tagAuctionTable {
	int  flag;		      // 0=(null) 1=�ϥΤ� 2=�L��
	char cdkey[32];		  // �e�U�H cdkey
	char customer[32];	// �e�U�H�H���W��
	int itemtype;		    // 1=�d�� 2=�D�� otherwise=error
	char goods[4096];	  // ���~ (�d�� or �D��)
	char description[256];	// �ԭz
  char listdata[512]; // ��ܦb��檺���
	int price;		      // �л�
  int onsaletime;
  int overduetime;
} AuctionTable;

extern AuctionTable onsale[MAX_AUCTION];	// ��椤���D��B�d��
//extern AuctionTable overdue[MAX_OVERDUE];	// �L�����D��B�d��

BOOL NPC_AuctioneerInit(int meindex);
void NPC_AuctioneerTalked(int meindex, int talkerindex, char *msg, int color);
void NPC_AuctioneerWindowTalked(int meindex, int talkerindex, int seqno, int select, char *data);
void NPC_AuctioneerLoop(int meindex);

#endif /* _AUCTIONEER */

#endif 
