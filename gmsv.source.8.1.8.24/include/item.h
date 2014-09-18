
#ifndef __ITEM_H__
#define __ITEM_H__

#include "char.h"

#define NULLITEM    "0"

typedef enum
{
    ITEM_FIST =0,
    ITEM_AXE,
    ITEM_CLUB,
    ITEM_SPEAR,
    ITEM_BOW,
    ITEM_SHIELD,
    ITEM_HELM,
    ITEM_ARMOUR,

	ITEM_BRACELET =8,
	ITEM_MUSIC,
	ITEM_NECKLACE,
	ITEM_RING,
	ITEM_BELT,
	ITEM_EARRING,
	ITEM_NOSERING,
	ITEM_AMULET,
    /* ****** */
    ITEM_OTHER =16,
    ITEM_BOOMERANG,		// �j�ۼ�
    ITEM_BOUNDTHROW,	// ���Y���Y
    ITEM_BREAKTHROW,	// ���Y��
    ITEM_DISH =20,
#ifdef _ITEM_INSLAY
	ITEM_METAL,
	ITEM_JEWEL,
#endif
#ifdef _ITEM_CHECKWARES
	ITEM_WARES,			//�f��
#endif

#ifdef _ITEM_EQUITSPACE
	ITEM_WBELT,			//�y�a
	ITEM_WSHIELD,		//��
	ITEM_WSHOES,		//�c�l
#endif
#ifdef _EQUIT_NEWGLOVE 
	ITEM_WGLOVE,		//��M
#endif

#ifdef _ALCHEMIST
	ITEM_ALCHEMIST =30,
#endif

#ifdef _ANGEL_SUMMON
	//ITEM_ANGELTOKEN,
	//ITEM_HEROTOKEN,
#endif

    ITEM_CATEGORYNUM,
    
}ITEM_CATEGORY;

typedef enum
{
	ITEM_FIELD_ALL,
	ITEM_FIELD_BATTLE,
	ITEM_FIELD_MAP,
}ITEM_FIELDTYPE;

typedef enum
{
	ITEM_TARGET_MYSELF,
	ITEM_TARGET_OTHER,
	ITEM_TARGET_ALLMYSIDE,
	ITEM_TARGET_ALLOTHERSIDE,
	ITEM_TARGET_ALL,
}ITEM_TARGETTYPE;

typedef enum
{
    ITEM_ID,
    ITEM_BASEIMAGENUMBER,
    ITEM_COST,
    ITEM_TYPE,
	ITEM_ABLEUSEFIELD,
	ITEM_TARGET,
    ITEM_LEVEL,                     /*  LEVEL  */
#ifdef _ITEM_MAXUSERNUM
	ITEM_DAMAGEBREAK,				//���~�ϥΦ���
#endif

#ifdef _ITEMSET4_TXT
	ITEM_USEPILENUMS,				//���~���|����
	ITEM_CANBEPILE,					//�O�_�i���|

	ITEM_NEEDSTR,
	ITEM_NEEDDEX,
	ITEM_NEEDTRANS,
	ITEM_NEEDPROFESSION,
#endif

#ifdef _TAKE_ITEMDAMAGE
	ITEM_DAMAGECRUSHE,
	ITEM_MAXDAMAGECRUSHE,
#endif

#ifdef _ADD_DEAMGEDEFC
	ITEM_OTHERDAMAGE,
	ITEM_OTHERDEFC,
#endif

#ifdef _SUIT_ITEM
	ITEM_SUITCODE,
#endif

    ITEM_ATTACKNUM_MIN,             /*    ��  ����  */
    ITEM_ATTACKNUM_MAX,             /*    ��  ����  */
    ITEM_MODIFYATTACK,              /*    ��    ����  */
    ITEM_MODIFYDEFENCE,             /*    ��    ����  */
    ITEM_MODIFYQUICK,               /*  QUICK  ����  */

    ITEM_MODIFYHP,                  /*  HP  ����    */
    ITEM_MODIFYMP,                  /*  MP  ����    */
    ITEM_MODIFYLUCK,                /*  LUCK  ����    */
    ITEM_MODIFYCHARM,               /*  CHARM  ����    */
    ITEM_MODIFYAVOID,               /*  ��    ����    */
	ITEM_MODIFYATTRIB,				/*  °������ */
	ITEM_MODIFYATTRIBVALUE,			/*  °�������� */
	ITEM_MAGICID,					/*  ����  �� */
	ITEM_MAGICPROB,					/*  ����       */
	ITEM_MAGICUSEMP,				/*  ��  MP */

#ifdef _ITEMSET5_TXT
	ITEM_MODIFYARRANGE,
	ITEM_MODIFYSEQUENCE,

	ITEM_ATTACHPILE,
	ITEM_HITRIGHT,	//�B�~�R��
#endif
#ifdef _ITEMSET6_TXT
	ITEM_NEGLECTGUARD,
//	ITEM_BEMERGE,
#endif
    /*  ��  �����������͡e*/
    ITEM_POISON,					/*   �������Ӥ˥��᡼��          */
    ITEM_PARALYSIS,          		/* ���Ӥ�b1  �ι�  ���Ǥ��ʤ��e */
    ITEM_SLEEP,              		/* ̲��e��  �Ǥ��ʤ�            */
    ITEM_STONE,              		/* �Сe��  �Ǥ��ʤ�              */
    ITEM_DRUNK,              		/* �줦�e      ��������     */
    ITEM_CONFUSION,          		/* ����e  ��    ����     */

	ITEM_CRITICAL,					/* ����  ������  ���� */

	ITEM_USEACTION,					/* �Ȥä����Υ�������� */
    ITEM_DROPATLOGOUT,              /* �ҥ������Ȥ������  �����ɤ���  */
    ITEM_VANISHATDROP,              /*   �������˾ä��뤫�ɤ��� */
    ITEM_ISOVERED,                  /*  ��˾�ä�����뤫�ɤ����e*/
	ITEM_CANPETMAIL,				/* �ڥåȥ᡼�������뤫 */
	ITEM_CANMERGEFROM,				/* �穨���ˤʤ�뤫 */
	ITEM_CANMERGETO,				/* �穨��ˤʤ�뤫 */

    ITEM_INGVALUE0,                 /* ��ʬ(5��ʬ) */
    ITEM_INGVALUE1,
    ITEM_INGVALUE2,
    ITEM_INGVALUE3,
    ITEM_INGVALUE4,
    
	ITEM_PUTTIME,					/*  ����  �ब  ���줿���� */
    ITEM_LEAKLEVEL,                 /*    ̩���ɤ�����Ф줿��  */
	ITEM_MERGEFLG,					/*  �穨���줿����  �फ�ɤ��� */
	ITEM_CRUSHLEVEL,				/*  �����ٹ礤 0  2 ���ϲ���Ƥʤ� 2������ */

    ITEM_VAR1,              	/*    �Ѻ��       */
    ITEM_VAR2,              	/*    �Ѻ��       */
    ITEM_VAR3,              	/*    �Ѻ��       */
    ITEM_VAR4,              	/*    �Ѻ��       */

	ITEM_DATAINTNUM,

}ITEM_DATAINT;

typedef enum
{
    ITEM_NAME,                      /*    ��     ����  ��     */
    ITEM_SECRETNAME,                /*    ��    ��������  ��  ��    */
    ITEM_EFFECTSTRING,              /*  ����  ��    */
    ITEM_ARGUMENT,                  /*  ����  ��ΰ���  */
#ifdef _ITEM_INSLAY
	ITEM_TYPECODE,
	ITEM_INLAYCODE,
#endif
	ITEM_CDKEY,						/*  ����  ���  ����  ���  �������ͤ�           */
#ifdef _ITEM_FORUSERNAMES
	ITEM_FORUSERNAME,
	ITEM_FORUSERCDKEY,
#endif
// CoolFish: 2001/10/11
#ifdef _UNIQUE_P_I
    ITEM_UNIQUECODE,		  /* ���~�s�X */
#endif

    ITEM_INGNAME0,                  /*  ��ʬ��  ��(5��ʬ) */
    ITEM_INGNAME1,
    ITEM_INGNAME2,
    ITEM_INGNAME3,
    ITEM_INGNAME4,


    ITEM_INITFUNC,                  /* ����
                                     * ITEM_Item*
                                     * �֤��� BOOL
                                     * �֤��ͤΰ�  �� CHAR_INITFUNC
                                     * ��  ��  */
    ITEM_FIRSTFUNCTION = ITEM_INITFUNC,
    ITEM_PREOVERFUNC,               /* CHAR_PREOVERFUNC �򻲾� */
    ITEM_POSTOVERFUNC,              /* CHAR_POSTOVERFUNC �򻲾�*/
    ITEM_WATCHFUNC,                 /* CHAR_WATCHFUNC �򻲾� */
    ITEM_USEFUNC,                   /* �����ϡb
                                     * int charaindex ����饤��ǥå���
                                     * int charitemindex ��ʬ��
                                     *              ����  ��    �β�    
                                     *              ��Ȥä���
                                     */
    ITEM_ATTACHFUNC,                /* �����ϡb
                                     * int charaindex ����饤��ǥå���
                                     * int itemindex  ����  �।��ǥå���
                                     *      ����饯����  �äƤ��륢��  ��
                                     *      �Υ���  ��  �ǤΥ���ǥå���
                                     *      �ǤϤʤ�������աe
                                     */
    ITEM_DETACHFUNC,                /* �����ϡb
                                     * int charaindex ����饤��ǥå���
                                     * int itemindex  ����  �।��ǥå���
                                     *      ����饯����  �äƤ��륢��  ��
                                     *      �Υ���  ��  �ǤΥ���ǥå���
                                     *      �ǤϤʤ�������աe
                                     */
    ITEM_DROPFUNC, 		            /*   �Ȥ����Ȥ�
                                     * ������
                                     *  int charaindex   �Ȥ��������
                                     *  int itemindex ����  �।��ǥå���
                                     */
    ITEM_PICKUPFUNC,              /* ����  ��򽦤ä���
                                     * ������
                                     *  int charaindex  ���ä������index
                                     *  int itemindex ����  �।��ǥå���
                                     */
#ifdef _Item_ReLifeAct
	ITEM_DIERELIFEFUNC,					/*ANDY_ADD
										�_���D��	
									 */
#endif

#ifdef _CONTRACT
	ITEM_CONTRACTTIME,
	ITEM_CONTRACTARG,
#endif

    ITEM_LASTFUNCTION,

    ITEM_DATACHARNUM = ITEM_LASTFUNCTION,

#ifdef _ANGEL_SUMMON
	ITEM_ANGELMISSION = ITEM_INGNAME0,
	ITEM_ANGELINFO = ITEM_INGNAME1,
	ITEM_HEROINFO = ITEM_INGNAME2,
#endif

}ITEM_DATACHAR;

typedef enum
{
    ITEM_WORKOBJINDEX,
    ITEM_WORKCHARAINDEX,
#ifdef _MARKET_TRADE
	ITEM_WORKTRADEINDEX,
	ITEM_WORKTRADETYPE,
	ITEM_WORKTRADESELLINDEX,
#endif
#ifdef _ITEM_ORNAMENTS
	ITEM_CANPICKUP,
#endif
#ifdef _ITEM_TIME_LIMIT
	ITEM_WORKTIMELIMIT,
#endif
    ITEM_WORKDATAINTNUM,
}ITEM_WORKDATAINT;



typedef struct tagItem
{
    int         data[ITEM_DATAINTNUM];
    STRING64    string[ITEM_DATACHARNUM];
    int         workint[ITEM_WORKDATAINTNUM];

    void*       functable[ITEM_LASTFUNCTION-ITEM_FIRSTFUNCTION];
}ITEM_Item;

typedef struct tagITEM_table
{
    int         use; 
    ITEM_Item   itm;
    int         randomdata[ITEM_DATAINTNUM];
}ITEM_table;


typedef struct tagITEM_exists
{
    BOOL        use;
    ITEM_Item   itm;
}ITEM_exists;

#ifdef _CONTRACT
#define MAX_CONTRACTTABLE	10
typedef struct tagITEM_contract
{
    int         used;
    char		detail[2048];
    int			argnum;
}ITEM_contractTable;
#endif


#define		ITEM_CHECKINDEX(index)		\
	_ITEM_CHECKINDEX( __FILE__, __LINE__, index)
INLINE BOOL _ITEM_CHECKINDEX( char *file, int line, int index);


BOOL ITEM_initExistItemsArray( int num );
BOOL ITEM_endExistItemsArray( void );
#define		ITEM_initExistItemsOne( itm) \
	_ITEM_initExistItemsOne( __FILE__, __LINE__, itm)
int _ITEM_initExistItemsOne( char *file, int line, ITEM_Item* itm );

#define		ITEM_endExistItemsOne( index ) \
			_ITEM_endExistItemsOne( index, __FILE__, __LINE__)

void _ITEM_endExistItemsOne( int index , char *file, int line);

#define ITEM_getInt( Index, element) _ITEM_getInt( __FILE__, __LINE__, Index, element )
INLINE int _ITEM_getInt( char *file, int line, int index ,ITEM_DATAINT element);


#define ITEM_setInt( Index, element, data) _ITEM_setInt( __FILE__, __LINE__, Index, element, data)
INLINE int _ITEM_setInt( char *file, int line, int index ,ITEM_DATAINT element, int data);


INLINE char* ITEM_getChar( int index ,ITEM_DATACHAR element );
INLINE BOOL ITEM_setChar( int index ,ITEM_DATACHAR element , char* new);

INLINE int ITEM_getWorkInt( int index ,ITEM_WORKDATAINT element);
INLINE int ITEM_setWorkInt( int index ,ITEM_WORKDATAINT element, int data);
INLINE int ITEM_getITEM_itemnum( void );
INLINE int ITEM_getITEM_UseItemnum( void );
INLINE BOOL ITEM_getITEM_use( int index );
void ITEM_constructFunctable( int itemindex );
void* ITEM_getFunctionPointer( int itemindex, int functype );
INLINE ITEM_Item *ITEM_getItemPointer( int index );
int ITEM_getItemMaxIdNum( void);


char* ITEM_makeStringFromItemData( ITEM_Item* one, int mode );
char* ITEM_makeStringFromItemIndex( int index, int mode );

BOOL ITEM_makeExistItemsFromStringToArg( char* src , ITEM_Item* item, int mode );
void ITEM_getDefaultItemSetting( ITEM_Item* itm);


INLINE BOOL ITEM_CHECKITEMTABLE( int number );
BOOL    ITEM_readItemConfFile( char* filename );


CHAR_EquipPlace ITEM_getEquipPlace( int charaindex, int itmid );


char*  ITEM_makeItemStatusString( int haveitemindex, int itemindex );
char*   ITEM_makeItemFalseString( void );
char*   ITEM_makeItemFalseStringWithNum( int haveitemindex );


BOOL ITEM_makeItem( ITEM_Item* itm, int number );
int ITEM_makeItemAndRegist( int number );


void ITEM_equipEffect( int index );

void Other_DefcharWorkInt( int index);

char* ITEM_getAppropriateName(int itemindex);
char* ITEM_getEffectString( int itemindex );


int ITEM_getcostFromITEMtabl( int itemid );

#define ITEM_getNameFromNumber( id) _ITEM_getNameFromNumber( __FILE__, __LINE__, id)
INLINE char* _ITEM_getNameFromNumber( char *file, int line, int itemid );


int ITEM_getlevelFromITEMtabl( int itemid );
int ITEM_getgraNoFromITEMtabl( int itemid );
char *ITEM_getItemInfoFromNumber( int itemid );

int ITEM_getdropatlogoutFromITEMtabl( int itemid );
int ITEM_getvanishatdropFromITEMtabl( int itemid );
int ITEM_getcanpetmailFromITEMtabl( int itemid );
int ITEM_getmergeItemFromFromITEMtabl( int itemid );

#ifdef _ITEM_CHECKWARES
BOOL CHAR_CheckInItemForWares( int charaindex, int flg);
#endif

BOOL ITEM_canuseMagic( int itemindex);
// Nuke +1 08/23 : For checking the validity of item target
int ITEM_isTargetValid( int charaindex, int itemindex, int toindex);


#ifdef _IMPOROVE_ITEMTABLE
BOOL ITEMTBL_CHECKINDEX( int ItemID);
int ITEM_getSIndexFromTransList( int ItemID);
int ITEM_getMaxitemtblsFromTransList( void);
int ITEM_getTotalitemtblsFromTransList( void);
#endif

int ITEMTBL_getInt( int ItemID, ITEM_DATAINT datatype);
char *ITEMTBL_getChar( int ItemID, ITEM_DATACHAR datatype);

int ITEM_getItemDamageCrusheED( int itemindex);
void ITEM_RsetEquit( int charaindex);//�۰ʨ����˳Ʀ�m���~�����~
void ITEM_reChangeItemToPile( int itemindex);
void ITEM_reChangeItemName( int itemindex);


#ifdef _SIMPLIFY_ITEMSTRING
void ITEM_getDefaultItemData( int itemID, ITEM_Item* itm);
#endif

#ifdef _CONTRACT
BOOL ITEM_initContractTable( );
#endif

#endif
