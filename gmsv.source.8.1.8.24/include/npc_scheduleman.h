#ifndef __NPC_SCHEDULEMAN_H__
#define __NPC_SCHEDULEMAN_H__

#define CHARNAME_MAX	32

// �̤j scheduleman �ƶq
#ifdef _FAMILY_MANORNUM_CHANGE
#define MAX_SCHEDULEMAN	20
#else
#define MAX_SCHEDULEMAN	12
#endif

// �C�@�� scheduleman ���ު� schedule �ƶq
#define MAX_SCHEDULE	24

// schedule �C���@���ү�C�X���̤j�ƶq
#define MAXSCHEDULEINONEWINDOW	4

// family �C���@���ү�C�X���̤j�ƶq
#define MAXFAMILYINONEWINDOW	8

// �C�@�� scheduleman �ҨϥΪ� schedule �x�s��m
// �����b fmpks �� ID*MAX_SCHEDULE �W (ID=0~MAX_SCHEDULEMAN-1)
// ID �O�g�b data/npc �����w�� "id:" ����

// dueltime = (�j��?10000:0) + (�p��*100)

typedef struct tagFamilyPKSchedule {
	int	dueltime;	// ���ɮɨ�
	int	host_index;	// �D���a��
	char host_name[CHARNAME_MAX];	// �D���a�ڦW��
	int	guest_index;	// �ȶ��a��
	char guest_name[CHARNAME_MAX];	// �ȶ��a�ڦW��
	int	prepare_time;	// �ǳƮɶ� (1~40 ����)
	int	max_player;	// �̤j�X���H�� (1~�a�ڤH�ƤW��)
	int	flag;		// ���A
	int	win;		// �ӧQ����]�w
	int	challenge_timeout;	// �D�Ԯɭ�
	int	setting_timeout;	// �]�w���A�� timeout
	char gmsv_name[256];
} FamilyPKSchedule;

// �����b FamilyPKSchedule (fmpks) �� flag ��
#define FMPKS_FLAG_NONE		-1	// �S������Ƶ{
#define FMPKS_FLAG_CHALLENGE	0	// ���ݫȶ��P�N��
#define FMPKS_FLAG_SETTING	1	// �D�����b�]�w�Ƶ{ (�������ܦ� NONE)
#define FMPKS_FLAG_CONFIRMING	2	// �ȶ����b�P�N��
#define FMPKS_FLAG_SCHEDULED	3	// �w�g�Ʀn�Ƶ{�A�|���}��
#define FMPKS_FLAG_DUEL		4	// �}����
#define FMPKS_FLAG_HOSTWIN	5	// �D����
#define FMPKS_FLAG_GUESTWIN	6	// �ȶ���
#define FMPKS_FLAG_MANOR_BATTLEBEGIN	7	// ����D�� �԰���
#define FMPKS_FLAG_MANOR_PREPARE	8	// ����D�� �ǳƤ�
#define FMPKS_FLAG_MANOR_PEACE	9	// ����D�� ��Ԥ�
#define FMPKS_FLAG_MANOR_OTHERPLANET	10	// ����D�� �b�O���P�y�M��
#define FMPKS_FLAG_MANOR_BATTLEEND	11	// ����D�� �԰�����
#define FMPKS_FLAG_MANOR_PEACE_SAVE	12	// �N����D�Ԧs��
#define FMPKS_FLAG_MANOR_READYTOFIGHT	13	// (GM: manorpk) ���P�t�ܦ��i���Ԫ��A
#define FMPKS_FLAG_MANOR_CLEANFLAG	14	// (GM: manorpk) ���P�y�M�����A
#ifdef _NEW_MANOR_LAW
#define FMPKS_FLAG_WAIT		15	// �i�J�D�Դ�,�w�O���a�ڮ��,���ݬD�ԱƵ{
#endif

// �ǵ� client �� flag
#define FLAG_NONE	-1
#define FLAG_MODIFY	0
#define FLAG_ACCEPT	1
#define FLAG_SETTING	2
#define FLAG_CONFIRM	3
#define FLAG_SCHEDULED	4

extern FamilyPKSchedule fmpks[MAX_SCHEDULE*MAX_SCHEDULEMAN];

BOOL NPC_SchedulemanInit(int meindex);
void NPC_SchedulemanTalked(int meindex, int talkerindex, char *msg, int color);
void NPC_SchedulemanWindowTalked(int meindex, int talkerindex,
				 int seqno, int select, char *data);
void NPC_SchedulemanLoop(int meindex);

#endif 

/*__NPC_SCHEDULEMAN_H__*/
