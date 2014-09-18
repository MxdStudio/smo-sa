#ifndef __LOG_H__
#define __LOG_H__
#include <time.h>
typedef enum
{
    LOG_TALK,
    LOG_PROC,
    LOG_ITEM,
    LOG_STONE,
    LOG_PET,
    LOG_TENSEI,
    LOG_KILL,     //ttom 12/26/2000 kill the pet & items
    LOG_TRADE,	// CoolFish: 2001/4/19
    LOG_HACK,	// Arminius 2001/6/14
    LOG_SPEED,	// Nuke 0626
    LOG_FMPOP,	// CoolFish: 2001/9/12
    LOG_FAMILY, // Robin 10/02
    LOG_GM,     // Shan 
#ifdef _SERVICE    
    LOG_SERVICE,  // Terry 2001/09/28
#endif
#ifdef _GAMBLE_ROULETTE
	LOG_GAMBLE,
#endif
#ifdef _TEST_PETCREATE
	LOG_CREATPET,
	LOG_AVGCREATPET,
#endif

	LOG_LOGIN,
	PETTRANS,
//Syu �W�[����ԳӭtLog
	LOG_FMPKRESULT,

// Syu ADD �s�W�a�ڭӤH�Ȧ�s��Log (���t�a�ڻȦ�)
	LOG_BANKSTONELOG,

	LOG_ACMESS,
	LOG_PKCONTEND,
#ifdef _STREET_VENDOR
	LOG_STREET_VENDOR,
#endif

#ifdef _ANGEL_SUMMON
	LOG_ANGEL,
#endif

#ifdef _LOG_OTHER
	LOG_OTHER,
#endif
#ifdef _NEW_MANOR_LAW
	LOG_FMPK_GETMONEY,
	LOG_FM_FAME_SHOP,
#endif
#ifdef _JZ_VIPLOG
	LOG_VIP_ACTION,
#endif
    LOG_TYPE_NUM,
}LOG_TYPE;

void closeAllLogFile( void );
BOOL initLog( char* filename );
void printl( LOG_TYPE logtype, char* format , ... );


void LogAcMess( int fd, char *type, char *mess );

void LogItem(
	char *CharName, /* ����饯��   */
	char *CharID,
	int ItemNo, 	/* ����  ��  �� */
	char *Key, 		/* ������� */
	int floor,		/* ��   */
	int x,
	int y,
    char *uniquecode, // shan 2001/12/14
	char *itemname, int itemID
);
void LogPkContend( char *teamname1, char *teamname2,
	int floor, int x, int y, int flg );

void LogPetTrans(
	char *cdkey, char *uniwuecde, char *uniwuecde2, char *CharName, int floor, int x, int y,
	int petID1, char *PetName1, int petLV, int petrank, int vital1, int str1, int tgh1, int dex1, int total1,
	int petID2, char *PetName2, int vital2, int str2, int tgh2, int dex2, int total2,
	int work0, int work1, int work2, int work3, int ans, int trans
);

void LogPet(
	char *CharName, /* ����饯��   */
	char *CharID,
	char *PetName,
	int PetLv,
	char *Key, 		/* ������� */
	int floor,		/* ��   */
	int x,
	int y,
	char *uniquecode  // shan 2001/12/14
);

#ifdef _STREET_VENDOR
void LogStreetVendor(
 	char *SellName,
	char *SellID,
	char *BuyName,
	char *BuyID,
	char *ItemPetName,
	int PetLv,	//�Y�O�D�㦹�Ȭ� -1
	int iPrice,
	char *Key,
	int Sfloor,
	int Sx,
	int Sy,
	int Bfloor,
	int Bx,
	int By,
	char *uniquecode
);
#endif

void LogTensei(
	char *CharName, /* ����饯��   */
	char *CharID,
	char *Key, 		/* ������� */
	int level,		//��٥�
	int transNum,	//ž�����
	int quest,		//�������ȿ�
	int home,		//�пȼM
	int item,		//  �å���  ��  �����
	int pet,		//  �åڥå�  �����
	int vital,		//  ����Vital
	int b_vital,	//  ��  vital
	int str,		//  ����str
	int b_str,		//  ��  str
	int tgh,		//  ����      
	int b_tgh,		//  ��        
	int dex,		//  ����      
	int b_dex		//  ��        
);
// Syu ADD �s�W�a�ڭӤH�Ȧ�s��Log (���t�a�ڻȦ�)
void LogFamilyBankStone(
        char *CharName,
        char *CharId,  
        int Gold,      
		int MyGold,
        char *Key,     
		int floor,           
        int x,
        int y,
		int banksum
);

void LogStone(
				int TotalGold,
        char *CharName, /* ����饯��   */
        char *CharId,   /* �桼����ID */
        int Gold,               /* ��� */
		int MyGold,
        char *Key,              /* ������� */
        int floor,              /* ��   */
        int x,
        int y
);

void LogTalk(
	char *CharName, /* ����饯��   */
	char *CharID,
	int floor,		/* ��   */
	int x,
	int y,
	char *message
);

// Terry 2001/09/28
#ifdef _SERVICE
void LogService(
     char *CharName,    //����W��
     char *CharID,      //���aID
     int  itemid,       //���~ID
     char *Key,         //����
     int floor,
     int x,
     int y
);
#endif                                   
//ttom 12/26/2000 kill pet & items
void LogKill(
        char *CharName,
        char *CharId,
        char *CharPet_Item
);
//ttom

// CoolFish: 2001/4/19
void LogTrade(char *message);
// CoolFish: 2001/9/12
void LogFMPOP(char *message);

// Arminius 2001/6/14
enum
{
    HACK_NOTHING,
    HACK_GETFUNCFAIL,
    HACK_NOTDISPATCHED,
    HACK_CHECKSUMERROR, 
    HACK_HP,
#ifdef _JZ_BEST
    HACK_FUNCERR,
    HACK_LSFUNCNULL,
#endif
	HACK_TYPE_NUM,
}HACK_TYPE;
void logHack(int fd, int errcode);
// Nuke 0626
void logSpeed(int fd);

void closeAllLogFile( void );
int openAllLogFile( void );

// Robin 10/02
void LogFamily(
	char *FMName,
	int fmindex,
        char *charName,
        char *charId,
	char *keyWord,
	char *data
);

// Shan 11/02
void LogGM(
        char *CharName,    //����W��
        char *CharID,      //���aID
        char *Message,     //���O���e
        int  floor,
        int  x,
        int  y
);

void LogLogin(
        char *CharID,   //���aID
        char *CharName, //����W��
		int  saveIndex,
		char *ipadress
);

#ifdef _TEST_PETCREATE
void LogCreatPet(
	char *PetName, int petid, int lv,int hp, int char_vital, int char_str, int char_tgh, int char_dex,
	int vital, int str, int tgh, int dex, int fixstr, int fixtgh, int fixdex, 
	int lvup,	int petrank, int flg
	);
#endif

void LogCreatFUPet(
	char *PetName, int petid, int lv, int hp,
	int vital, int str, int tgh, int dex,
	int fixstr, int fixtgh, int fixdex, int trans, int flg);

#ifdef _GAMBLE_ROULETTE

void LogGamble(
        char *CharName, //����W��
        char *CharID,   //���aID
        char *Key,      //����
        int floor,
        int x,
        int y,
		int player_stone,	//�Ҿ֦�����
		int Gamble_stone,	//�U�`����
		int get_stone,		//��o
		int Gamble_num,
		int flg	//flg = 1 ���a 2 ���a
);
#endif

void LogBankStone(
        char *CharName, /* ����饯��   */
        char *CharId, /* �桼����ID */
		int	meindex,
        int Gold,               /* ��� */
        char *Key,              /* ������� */
        int floor,              /* ��   */
        int x,
        int y,
		int my_gold,
		int my_personagold
);

//Syu �W�[����ԳӭtLog
void Logfmpk(
			 char *winner, int winnerindex, int num1,
			 char *loser, int loserindex, int num2, char *date, char *buf1, char *buf2, int flg);

#ifdef _NEW_MANOR_LAW
void LogFMPKGetMomey(
	char *szFMName,
	char *szID,
	char *szCharName,
	int iMomentum,
	int iGetMoney,
	int iDest
);
void LogFMFameShop(
	char *szFMName,
	char *szID,
	char *szCharName,
	int iFame,
	int iCostFame
);
#endif

void backupAllLogFile( struct tm *ptm );


#ifdef _TEST_PETCREATE
void backupTempLogFile( char *buf, char *entryname, int Num);
#endif

void LogPetPointChange( 
	char * CharName, char *CharID, char *PetName, int petindex, int errtype,
	int PetLv, char *Key,int floor, int x, int y);

void LogPetFeed(
	char * CharName, char *CharID, char *PetName, int petindex,
	int PetLv, char *Key,int floor, int x, int y, char *ucode);

#ifdef _ANGEL_SUMMON
void LogAngel( char *msg);
#endif

void warplog_to_file( void );
void warplog_from_file( void );

typedef struct {
	int floor;
	int incount;
	int outcount;
}tagWarplog;
#define MAXMAPNUM 700
extern tagWarplog warplog[MAXMAPNUM];

typedef struct {
	int floor1;
	int floor2;
	int count;
}tagWarpCount;
#define MAXMAPLINK 1000
extern tagWarpCount warpCount[MAXMAPLINK];

#ifdef _LOG_OTHER
void LogOther(
	char *CharName,
	char *CharID,
	char *message
);
#endif

#ifdef _JZ_VIPLOG
void LogVipAction( 
	char *CharName,									//�H���W��
	char *Key,											//�b��
	char *pet,											//�d���W��
	char *logmess,										//�Ƶ�
	int petenemyindex,							//�d�� enemy ID
	int floor, 											//�H���Ҧb�a��
	int x, int y, 									//�H���Ҧb����
	char *ucode											//�d���s�X
	);

void LogVipShop( 
	char *CharName,									//�H���W��
	char *Key,											//�b��
	int memberq1,										//�ʶR�e���|���I
	int memberh1,										//�ʶR�᪺�|���I
	int floor, 											//�H���Ҧb�a��
	int x, int y, 									//�H���Ҧb����
	char *logmess										//�Ƶ�
	);
#endif

#endif
