#ifndef __BATTLE_H__
#define __BATTLE_H__

#define	BATTLE_ENTRY_MAX 10
#define	BATTLE_PLAYER_MAX 5
#define SIDE_OFFSET 	10

#define BATTLE_STRING_MAX	4096
#define BATTLE_TIME_LIMIT	(60*60)


#define DUELPOINT_RATE	(0.1)

enum{
	BATTLE_MODE_NONE = 0,	// �L�԰����A
	BATTLE_MODE_INIT,		// �԰���l��
	BATTLE_MODE_BATTLE,		// �԰���
	BATTLE_MODE_FINISH,		// �԰�����
	BATTLE_MODE_STOP,		// �԰�����(���ϥ�)
	BATTLE_MODE_WATCHBC,	// �[�Ԫ�l��
	BATTLE_MODE_WATCHPRE,	// �[��(�S�@��)
	BATTLE_MODE_WATCHWAIT,	// �[��(�S�@��)
	BATTLE_MODE_WATCHMOVIE, // �[��(�S�@��)
	BATTLE_MODE_WATCHAFTER, // �[��(�S�@��)
	BATTLE_MODE_END
};


enum{
	BATTLE_TYPE_NONE = 0,
	BATTLE_TYPE_P_vs_E = 1,
	BATTLE_TYPE_P_vs_P = 2,
	BATTLE_TYPE_E_vs_E = 3,
	BATTLE_TYPE_WATCH = 4,
	BATTLE_TYPE_DP_BATTLE = 5,
	BATTLE_TYPE_BOSS_BATTLE = 6,
	BATTLE_TYPE_END
};


enum{
	BATTLE_CHARMODE_NONE = 0,		// �L�԰����A
	BATTLE_CHARMODE_INIT,			// �԰���l��
	BATTLE_CHARMODE_C_WAIT,			// ���ݾ԰����O
	BATTLE_CHARMODE_C_OK,			// �w��J�԰����O
	BATTLE_CHARMODE_BATTLE,			// ���ϥ�
	BATTLE_CHARMODE_RESCUE,			// ��help���A�i�J��
	BATTLE_CHARMODE_FINAL,			// �԰�����
	BATTLE_CHARMODE_WATCHINIT,		// �[�Ԫ�l��
	BATTLE_CHARMODE_COMMAND,		// ���ϥ�
	BATTLE_CHARMODE_END
};




typedef enum{
	BATTLE_ERR_NONE = 0,
	BATTLE_ERR_NOTASK,
	BATTLE_ERR_NOUSE,
	BATTLE_ERR_PARAM,
	BATTLE_ERR_ENTRYMAX,
	BATTLE_ERR_TYPE,
	BATTLE_ERR_CHARAINDEX,
	BATTLE_ERR_BATTLEINDEX,
	BATTLE_ERR_NOENEMY,
	BATTLE_ERR_ALREADYBATTLE,
	BATTLE_ERR_SAMEPARTY,
	BATTLE_ERR_END
}BATTLE_ERR;


enum{
	BATTLE_S_TYPE_PLAYER = 0,
	BATTLE_S_TYPE_ENEMY,
	BATTLE_S_TYPE_END
};


typedef enum{
	BATTLE_COM_NONE,
	BATTLE_COM_ATTACK,
	BATTLE_COM_GUARD,
	BATTLE_COM_CAPTURE,
	BATTLE_COM_ESCAPE,
	BATTLE_COM_PETIN,
	BATTLE_COM_PETOUT,
	BATTLE_COM_ITEM,
	BATTLE_COM_BOOMERANG,
	BATTLE_COM_COMBO,
	BATTLE_COM_COMBOEND,
	BATTLE_COM_WAIT,

	BATTLE_COM_SEKIBAN = 1000,
	BATTLE_COM_S_RENZOKU,
	BATTLE_COM_S_GBREAK,
	BATTLE_COM_S_GUARDIAN_ATTACK,
	BATTLE_COM_S_GUARDIAN_GUARD,
	BATTLE_COM_S_CHARGE,
	BATTLE_COM_S_MIGHTY,
	BATTLE_COM_S_POWERBALANCE,
	BATTLE_COM_S_STATUSCHANGE,
	BATTLE_COM_S_EARTHROUND0,
	BATTLE_COM_S_EARTHROUND1,
	BATTLE_COM_S_LOSTESCAPE,
	BATTLE_COM_S_ABDUCT,
	BATTLE_COM_S_STEAL,
	BATTLE_COM_S_NOGUARD,
	BATTLE_COM_S_CHARGE_OK,
	BATTLE_COM_JYUJYUTU = 2000,

	BATTLE_COM_COMPELESCAPE, //�j�����}

#ifdef __ATTACK_MAGIC
  BATTLE_COM_S_ATTACK_MAGIC,		// �d���]�k
#endif

#ifdef _PSKILL_FALLGROUND
	BATTLE_COM_S_FALLRIDE,			//�����N
#endif
#ifdef _PETSKILL_EXPLODE
	BATTLE_COM_S_EXPLODE,
#endif
#ifdef _PETSKILL_TIMID
	BATTLE_COM_S_TIMID,
#endif
#ifdef _PETSKILL_2TIMID
	BATTLE_COM_S_2TIMID,
#endif
#ifdef _PETSKILL_ANTINTER
	BATTLE_COM_S_ANTINTER,
#endif
#ifdef _PETSKILL_PROPERTY
	BATTLE_COM_S_PROPERTYSKILL,
#endif
#ifdef _PETSKILL_TEAR
	BATTLE_COM_S_PETSKILLTEAR,
#endif
#ifdef _BATTLE_LIGHTTAKE
	BATTLE_COM_S_LIGHTTAKE,
#endif
#ifdef _BATTLE_ATTCRAZED		// ANDY �ƨg�ɨ�
	BATTLE_COM_S_ATTCRAZED,
#endif
#ifdef _SHOOTCHESTNUT			// Syu ADD �d�ޡG��ߤl
	BATTLE_COM_S_ATTSHOOT,
#endif
#ifdef _BATTLESTEAL_FIX
	BATTLE_COM_S_STEALMONEY,
#endif
#ifdef _PRO_BATTLEENEMYSKILL
	BATTLE_COM_S_ENEMYRELIFE,		//NPC ENEMY �_���ޯ�
	BATTLE_COM_S_ENEMYREHP,			//NPC ENEMY �ɦ�ޯ�
	BATTLE_COM_S_ENEMYHELP,			//NPC ENEMY �ۤH
#endif
#ifdef _SKILL_DAMAGETOHP
	BATTLE_COM_S_DAMAGETOHP,		//�ݦ��
#endif
#ifdef _Skill_MPDAMAGE
	BATTLE_COM_S_MPDAMAGE,			//MP�ˮ`
#endif
#ifdef _SKILL_WILDVIOLENT_ATT
    BATTLE_COM_S_WILDVIOLENTATTACK, //�g�ɧ���  vincent add 2002/05/16
#endif

#ifdef _SKILL_SPEEDY_ATT
    BATTLE_COM_S_SPEEDYATTACK,		//�e�t����  vincent add 2002/05/20
#endif
#ifdef _SKILL_GUARDBREAK2
    BATTLE_COM_S_GBREAK2,			//�}�����m2 vincent add 2002/05/20
#endif
#ifdef _SKILL_SACRIFICE
    BATTLE_COM_S_SACRIFICE,			//�ϴ�		vincent add 2002/05/30
#endif
#ifdef _SKILL_WEAKEN  
	    BATTLE_COM_S_WEAKEN,		//��z		vincent add 2002/07/11
#endif
#ifdef _SKILL_DEEPPOISON  
	    BATTLE_COM_S_DEEPPOISON,	//�@�r		vincent add 2002/07/16
#endif
#ifdef _SKILL_BARRIER  
	    BATTLE_COM_S_BARRIER,		//�]��		vincent add 2002/07/16
#endif
#ifdef _SKILL_NOCAST 
	    BATTLE_COM_S_NOCAST,		//�I�q		vincent add 2002/07/16
#endif
#ifdef _SKILL_ROAR 
    BATTLE_COM_S_ROAR,			    //�j�q		vincent add 2002/07/11
#endif
#ifdef _BATTLENPC_WARP_PLAYER
	BATTLE_COM_WARP,                // npc warp player
#endif
#ifdef _SKILL_TOOTH
	BATTLE_COM_S_TOOTHCRUSHE,
#endif
#ifdef _PSKILL_MODIFY
	BATTLE_COM_S_MODIFYATT,
#endif
#ifdef _PSKILL_MDFYATTACK
	BATTLE_COM_S_MDFYATTACK,
#endif
#ifdef _MAGIC_SUPERWALL
	BATTLE_COM_S_SUPERWALL,
#endif
#ifdef _SKILL_REFRESH
    BATTLE_COM_S_REFRESH,
#endif
#ifdef _VARY_WOLF
	BATTLE_COM_S_VARY,
#endif
#ifdef _PETSKILL_SETDUCK
	BATTLE_COM_S_SETDUCK,
#endif
#ifdef _MAGICPET_SKILL
	BATTLE_COM_S_SETMAGICPET,
#endif
#ifdef _PROFESSION_SKILL			// WON ADD �H��¾�~�ޯ�

// �Ův
	BATTLE_COM_S_VOLCANO_SPRINGS,	// ���s�u	
	BATTLE_COM_S_FIRE_BALL,			// ���P�y
	BATTLE_COM_S_FIRE_SPEAR,		// ���s�j
	BATTLE_COM_S_SUMMON_THUNDER,	// �l�p�N
	BATTLE_COM_S_CURRENT,			// �q�y�N
	BATTLE_COM_S_STORM,				// �ɭ��B
	BATTLE_COM_S_ICE_ARROW,			// �B�b�N
	BATTLE_COM_S_ICE_CRACK,			// �B�z�N
	BATTLE_COM_S_ICE_MIRROR,		// �B��N
	BATTLE_COM_S_DOOM,				// �@�ɥ���
	BATTLE_COM_S_BLOOD,				// �ݦ妨��
	BATTLE_COM_S_BLOOD_WORMS,		// �ݦ���
	BATTLE_COM_S_SIGN,				// �@�w����
	BATTLE_COM_S_FIRE_ENCLOSE,		// ������	
	BATTLE_COM_S_ICE_ENCLOSE,		// �B����
	BATTLE_COM_S_THUNDER_ENCLOSE,	// �p����
	BATTLE_COM_S_ENCLOSE,			// �����N
	BATTLE_COM_S_TRANSPOSE,			// ���δ���
// �i�h
	BATTLE_COM_S_BRUST,				// �z��			
	BATTLE_COM_S_CHAIN_ATK,			// �s������
	BATTLE_COM_S_AVOID,				// �j��
	BATTLE_COM_S_RECOVERY,			// �ɦ�
	BATTLE_COM_S_WEAPON_FOCUS,		// �Z���M��
	BATTLE_COM_S_REBACK,			// ���A�^�_
	BATTLE_COM_S_CHAIN_ATK_2,		// ��������
	BATTLE_COM_S_SCAPEGOAT,			// �ˤw����
	BATTLE_COM_S_ENRAGE,			// �E�Ƨ���
	BATTLE_COM_S_COLLECT,			// ��q�E��
	BATTLE_COM_S_FOCUS,				// �M�`�԰�
	BATTLE_COM_S_SHIELD_ATTACK,		// ����
	BATTLE_COM_S_DUAL_WEAPON,		// �G�M�y
	BATTLE_COM_S_DEFLECT,			// ����
	BATTLE_COM_S_THROUGH_ATTACK,	// �e�����
	BATTLE_COM_S_CAVALRY,			// �y�M����
	BATTLE_COM_S_DEAD_ATTACK,		// �x������
	BATTLE_COM_S_CONVOLUTE,			// �j�ۧ���
	BATTLE_COM_S_CHAOS,				// �V�ç���
// �y�H
	BATTLE_COM_S_TRAP,				// ����			
	BATTLE_COM_S_TRACK,				// �l�M����
	BATTLE_COM_S_DOCILE,			// �����d��	
	BATTLE_COM_S_ENRAGE_PET,		// �E���d��	
	BATTLE_COM_S_DRAGNET,			// ��ù�a��
	BATTLE_COM_S_ENTWINE,			// �����¶
	BATTLE_COM_S_AUTARKY,			// �۵��ۨ�
	BATTLE_COM_S_PLUNDER,			//   �鱰��
	BATTLE_COM_S_TOXIN_WEAPON,		// �r���Z��
	BATTLE_COM_S_RESIST_FIRE,		// ���ܩʴ���
	BATTLE_COM_S_RESIST_ICE,		// �B�ܩʴ���
	BATTLE_COM_S_RESIST_THUNDER,	// �p�ܩʴ���
	BATTLE_COM_S_G_RESIST_FIRE,		// ������ܩʴ���
	BATTLE_COM_S_G_RESIST_ICE,		// ����B�ܩʴ���
	BATTLE_COM_S_G_RESIST_THUNDER,	// ����p�ܩʴ���
	BATTLE_COM_S_ATTACK_WEAK,		// �z�I����
	BATTLE_COM_S_INSTIGATE,			// �D��
	BATTLE_COM_S_OBLIVION,			// ���
#ifdef _PROFESSION_ADDSKILL
	BATTLE_COM_S_RESIST_F_I_T,      // �۵M�¯�
	BATTLE_COM_S_CALL_NATURE,       // ���l�۵M
    BATTLE_COM_S_BOUNDARY,          // �|�ݩʵ���
#endif
#endif	

#ifdef _PET_SKILL_SARS				// WON ADD �r�ٽ���
	BATTLE_COM_S_SARS,
#endif
#ifdef _SONIC_ATTACK				// WON ADD ���i����
	BATTLE_COM_S_SONIC,
	BATTLE_COM_S_SONIC2,
#endif
#ifdef _PETSKILL_REGRET
	BATTLE_COM_S_REGRET,
	BATTLE_COM_S_REGRET2,
#endif
#ifdef _PETSKILL_GYRATE
    BATTLE_COM_S_GYRATE,
#endif
#ifdef _PETSKILL_ACUPUNCTURE
    BATTLE_COM_S_ACUPUNCTURE,
#endif
#ifdef _PETSKILL_RETRACE
	BATTLE_COM_S_RETRACE,
#endif
#ifdef _PETSKILL_HECTOR
	BATTLE_COM_S_HECTOR,
#endif
#ifdef _PETSKILL_FIREKILL
    BATTLE_COM_S_FIREKILL,
#endif
#ifdef _PETSKILL_DAMAGETOHP
	BATTLE_COM_S_DAMAGETOHP2, //�t��g�T(�ݦ�ު�����) 
#endif
#ifdef _PETSKILL_BECOMEFOX
    BATTLE_COM_S_BECOMEFOX,
#endif
#ifdef _PETSKILL_BECOMEPIG
    BATTLE_COM_S_BECOMEPIG,
#endif
#ifdef _PETSKILL_SHOWMERCY
    BATTLE_COM_S_SHOWMERCY,
#endif
#ifdef _PETSKILL_LER
		BATTLE_COM_S_BAT_FLY,					// �p���� - �s���|«
		BATTLE_COM_S_DIVIDE_ATTACK,		// �p���� - �����a��
#endif
#ifdef _PETSKILL_BATTLE_MODEL
		BATTLE_COM_S_BATTLE_MODEL,		// �d���ޯ�԰��Ҳ�
#endif

	BATTLE_COM_END
}BATTLE_COM;


enum{
	BATTLE_RET_NORMAL,
	BATTLE_RET_CRITICAL,
	BATTLE_RET_MISS,
	BATTLE_RET_DODGE,
	BATTLE_RET_ALLGUARD,
#ifdef _EQUIT_ARRANGE
	BATTLE_RET_ARRANGE,
#endif
	BATTLE_RET_END
}BATTLE_RET;


#define BC_FLG_NEW			(1<<0)
#define BC_FLG_DEAD			(1<<1)
#define BC_FLG_PLAYER		(1<<2)
#define BC_FLG_POISON		(1<<3)
#define BC_FLG_PARALYSIS	(1<<4)
#define BC_FLG_SLEEP		(1<<5)
#define BC_FLG_STONE		(1<<6)
#define BC_FLG_DRUNK		(1<<7)
#define BC_FLG_CONFUSION	(1<<8)
#define BC_FLG_HIDE			(1<<9)
#define BC_FLG_REVERSE		(1<<10)
#ifdef _MAGIC_WEAKEN
#define BC_FLG_WEAKEN		(1<<11) // ��z
#endif
#ifdef _MAGIC_DEEPPOISON
#define BC_FLG_DEEPPOISON	(1<<12) // �@�r
#endif
#ifdef _MAGIC_BARRIER
#define BC_FLG_BARRIER		(1<<13) // �]��
#endif
#ifdef _MAGIC_NOCAST
#define BC_FLG_NOCAST		(1<<14) // �I�q
#endif

#ifdef _PET_SKILL_SARS			// WON ADD �r�ٽ���
#define BC_FLG_SARS			(1<<15) // �r��
#endif

#ifdef _PROFESSION_SKILL			// WON ADD �H��¾�~�ޯ�
#define BC_FLG_DIZZY		(1<<16)	// �w�t	
#define BC_FLG_ENTWINE		(1<<17)	// �����¶
#define BC_FLG_DRAGNET		(1<<18)	// ��ù�a��	
#define BC_FLG_ICECRACK	    (1<<19)	// �B�z�N	
#define BC_FLG_OBLIVION  	(1<<20)	// ���
#define BC_FLG_ICEARROW		(1<<21)	// �B�b
#define BC_FLG_BLOODWORMS 	(1<<22)	// �ݦ���
#define BC_FLG_SIGN 		(1<<23)	// �@�w����
#define BC_FLG_CARY			(1<<24) // �D��
#define BC_FLG_F_ENCLOSE	(1<<25) // ������
#define BC_FLG_I_ENCLOSE	(1<<26) // �B����
#define BC_FLG_T_ENCLOSE	(1<<27) // �p����
#ifdef _PROFESSION_ADDSKILL
#define BC_FLG_WATER	    (1<<28) // ������
#define BC_FLG_FEAR			(1<<29) // ����
#endif
#ifdef _PETSKILL_LER
#define BC_FLG_CHANGE			(1<<30) // �p���ܨ�
#endif
#endif




#define BP_FLG_JOIN					(1<<0)
#define BP_FLG_PLAYER_MENU_OFF		(1<<1)
#define BP_FLG_BOOMERANG			(1<<2)
#define BP_FLG_PET_MENU_OFF			(1<<3)
#define BP_FLG_ENEMY_SURPRISAL		(1<<4)
#define BP_FLG_PLAYER_SURPRISAL		(1<<5)


#define CHAR_BATTLEFLG_ULTIMATE		(1<<0)
#define CHAR_BATTLEFLG_AIBAD		(1<<1)
#define CHAR_BATTLEFLG_REVERSE		(1<<2)
#define CHAR_BATTLEFLG_GUARDIAN		(1<<3)
#define CHAR_BATTLEFLG_NORETURN		(1<<4)
#define CHAR_BATTLEFLG_RECOVERY		(1<<5)
#define CHAR_BATTLEFLG_ABIO			(1<<6)
#define CHAR_BATTLEFLG_NODUCK		(1<<7)

#define GETITEM_MAX	3
typedef struct
{
	int attacker;
}INVADER;

typedef struct
{
	int		charaindex;
	int		bid;
	int		escape;
	int		flg;
	int		guardian;
	int		duelpoint;
	int		getitem[GETITEM_MAX];
}BATTLE_ENTRY;
#define BENT_FLG_ULTIMATE	(1<<0)

typedef struct
{
	int		type;
	int		flg;
	int		common_dp;
	BATTLE_ENTRY	Entry[BATTLE_ENTRY_MAX];
}BATTLE_SIDE;

#define BSIDE_FLG_SURPRISE ( 1 << 0 )
#define BSIDE_FLG_HELP_OK ( 1 << 1 )



typedef struct _Battle
{
	BOOL	use;	/* �ȤäƤ��뤫�ɤ��� */
	int		battleindex; /* �Хȥ�  �� */
	int		mode;	/* ���ߤ���  �⡼�� */
	int		type;	/* ��  ������ (0:�̾�)(1:DUEL)(2:�ܥ���) */
	int		dpbattle;	/* DP�Хȥ뤫�� */
	int		norisk;	/* ���Ǥ�ꥹ����  ���Хȥ뤫�� */
	int		turn;	/* ������� */
	int		timer;	/* �����������˻Ȥ������� */
	int		leaderindex; /* ���󥫥���Ȥ򵯤����������Υ���ǥå��� */
	int		winside; /*   ������������ */
	int		field_att;	/* �ե�����ɤ�°�� */
	int		att_count;	/* �ե�����ɤ�°��  ��  �Υ����� */
	int		att_pow;	/* �ե�����ɤ�°��  ��  �Υѥ */
	int		field_no;
	int		flg;		/* �Ƽ�ե饰 */
	BATTLE_SIDE		Side[2];
#ifdef _BATTLE_TIMESPEED
	unsigned int CreateTime;
	unsigned int EndTime;
	unsigned int PartTime;
	int flgTime;
	
#endif

#ifdef _PROFESSION_ADDSKILL
	//�B�z�N�s��
	int ice_count;//�Ȧs�ƶq
	int ice_bout[20];//�^�X�p��
	BOOL ice_use[20];//�O�_�ϥ�
	int ice_toNo[20];//�ﹳ
	int ice_level[20];//�����O
	int ice_array[20];
	int ice_charaindex[20];
	int ice_attackNo[20];
#endif

#ifdef _DEATH_CONTEND
	int menum;
	int tonum;
#endif

#ifdef _ACTION_BULLSCR
	int enemynum;
#endif
	int		iEntryBack[BATTLE_ENTRY_MAX*2];		// ����������  ����Ƥ������С�
	int		iEntryBack2[BATTLE_ENTRY_MAX*2];		// ����������  ����Ƥ������С�
	int		createindex;	/* ������  ����Ф�������饤��ǥå���(NPC�ʤ�) */
	int    (*WinFunc)( int battleindex, int charaindex );
#ifdef _DEATH_CONTEND//�p��ӱ�func
	int    (*PkFunc)( int menum, int tonum, int winside, int battlemap);
	int battlemap;
#endif
	struct _Battle *pNext;		// ���δ��殺�롼��
	struct _Battle *pBefore;	// ���δ��殺�롼��
}BATTLE;

#define BATTLE_FLG_FREEDP ( 1 << 0 )	//   ���Ƥ�    ����ʤ�
#define BATTLE_FLG_CHARALOST   ( 1 << 1 )	// ����餬������  �����ꤷ�Ƥ���



enum{	// ���field_att ��  �����
	BATTLE_ATTR_NONE = 0,	//   °��
	BATTLE_ATTR_EARTH,		// �M
	BATTLE_ATTR_WATER,		//   
	BATTLE_ATTR_FIRE,		// ��
	BATTLE_ATTR_WIND,		// ��
	BATTLE_ATTR_END
};


//krynn 2001/12/28
enum
{
	BATTLE_SIDE_RIGHT,		// �԰����k��	right side of battle
	BATTLE_SIDE_LEFT,		// �԰�������	left side of battle
	BATTLE_SIDE_WATCH,		// �[��			watch battle player
};
//krynn end

#define TARGET_SIDE_0	20	// ��¦  �����ɣ�  ����
#define TARGET_SIDE_1	21	// ��¦  �����ɣ�  ����
#define TARGET_ALL	22	// ��  ����

#ifdef __ATTACK_MAGIC

#define TARGET_SIDE_0_B_ROW     26      // �k�U��@�C
#define TARGET_SIDE_0_F_ROW     25      // �k�U�e�@�C
#define TARGET_SIDE_1_F_ROW     24      // ���W�e�@�C
#define TARGET_SIDE_1_B_ROW     23      // ���W��@�C

// won add
#define	TARGER_THROUGH			27

#endif

extern int	gItemCrushRate;		//   ��  �ǥե����
extern BATTLE *BattleArray; 	/* ��  ������     */
extern int BATTLE_battlenum;	/*     �ο� */
extern char szAllBattleString[BATTLE_STRING_MAX];	/* ��  �˻Ȥ����ޥ��  ��   */
extern char *pszBattleTop, *pszBattleLast;	/* ��  �˻Ȥ�  ��  �ΰ�   */
extern char szBadStatusString[];	// ��  �������۾���  ��  
extern int gWeponType;	// ���ߤ�  ��μ�  
extern float gDamageDiv;	// ���᡼��ʬ��

#define BATTLE_CHECKINDEX( a ) ( ((a)>=BATTLE_battlenum || (a)<0 )?(FALSE):(TRUE) )
#define BATTLE_CHECKSIDE( a ) ( ((a)>=2 || (a)<0)?(FALSE):( TRUE) )
#define BATTLE_CHECKNO( a ) ( ((a)>=20 || (a)<0 )?(FALSE):(TRUE) )
#define BATTLE_CHECKADDRESS( a )  ((&BattleArray[0])<=(a) && (a)<=(&BattleArray[BATTLE_battlenum-1] )?(TRUE):(FALSE) ) 

#define IsBATTLING( a ) (CHAR_getWorkInt((a),CHAR_WORKBATTLEMODE)?(TRUE):(FALSE))

#define STRCPY_TAIL( _pszTop, _pszLast, _szBuffer)	{ int _len = strlen( _szBuffer ); ( _pszTop + _len < (_pszLast)-1 )?( memcpy( _pszTop, _szBuffer, _len ),	_pszTop += _len, _pszTop[0] = 0):(0); }

#define BATTLESTR_ADD( _szBuffer ){ int _len = strlen( _szBuffer ); ( pszBattleTop + _len < ( pszBattleLast)-1 )?( memcpy( pszBattleTop, _szBuffer, _len ),	pszBattleTop += _len, pszBattleTop[0] = 0):(0); }

#define BATTLE_MAP_MAX 219

#define CH_FIX_PLAYERLEVELUP	(+2)	// �ץ쥤�䡼�Υ�٥뤬���å�
#define CH_FIX_PLAYERDEAD		(-2)	// �ץ쥤�䡼���̾��
#define CH_FIX_PLAYEULTIMATE	(-4)	// �ץ쥤�䡼������  ����åȻ�
#define CH_FIX_PETESCAPE		(-1)	// �ڥåȤ�  ����

#define AI_FIX_PETLEVELUP		(+5*100)	// �ڥåȤ���٥륢�å�
#define AI_FIX_PETWIN			(+1)		// �ڥåȤ�Ũ���ݤ���
#define AI_FIX_PETGOLDWIN		(+2*10)	// �ڥåȤ���٥�ι⤤Ũ���ݤ���
#define AI_FIX_PETRECOVERY		(+10)		// ��    �˲������Ƥ��ä�
#define AI_FIX_PETRESSURECT		(+3*100)	// ��    �����褷�Ƥ��ä�
//#define AI_FIX_PETRECOVERY	(+50)		// ��    �˲������Ƥ��ä�

#define AI_FIX_SEKKAN 			(-2*100)	// ��ʬ�ΥڥåȤ�  ��
#define AI_FIX_PLAYERULTIMATE	(-10*100)	// ��ʬ�μ�ͤ�����  ����åȻ�
#define AI_FIX_PETULTIMATE		(-10*100)	// �ڥåȤ�����  ����åȻ�
#define AI_FIX_PLAYERDEAD		(-1*100)	// ��ʬ�μ�ͤ�����
#define AI_FIX_PETDEAD			(-5*100)	// �ڥåȤ�����


#ifdef _Item_ReLifeAct
int BATTLE_getBattleDieIndex( int battleindex, int bid );
#endif

int BATTLE_No2Index( int battleindex, int No);

int BATTLE_Index2No( int battleindex, int charaindex);

BOOL BATTLE_initBattleArray( int battlenum);

int BATTLE_CreateBattle( void );
int BATTLE_DeleteBattle( int battleindex);

int BATTLE_NewEntry( int charaindex, int battleindex, int side);

#define BATTLE_Exit( charaindex, battleindex) _BATTLE_Exit( __FILE__, __LINE__, charaindex, battleindex)
INLINE int _BATTLE_Exit( char *file, int line, int charaindex ,int battleindex);

#define BATTLE_ExitAll( battleindex) _BATTLE_ExitAll( __FILE__, __LINE__, battleindex)
INLINE void _BATTLE_ExitAll( char *file, int line, int battleindex);

int BATTLE_CreateVsPlayer( int charaindex0,	int charaindex1);

int BATTLE_CreateVsEnemy( int charaindex, int mode,	int	npcindex);

int BATTLE_CountEntry( int battleindex,	int side);

int BATTLE_Loop( void );

int BATTLE_FinishSet( int battleindex );
int BATTLE_StopSet( int battleindex );
int BATTLE_RescueEntry( int charaindex, int toindex);

int BATTLE_PetDefaultExit( int charaindex, int battleindex);

int	BATTLE_PetDefaultEntry(
	int charaindex,	// �ڥåȤ�  �äƤ���ץ쥤�䡼��          
	int battleindex,// �Хȥ륤��ǥå���
	int side
);

BOOL BATTLE_RescueTry( int charaindex);

BOOL BATTLE_RescueParentTry(
		int charaindex,
		int pindex
	);

int BATTLE_DefaultAttacker(	int battleindex, int side);

BOOL BATTLE_IsThrowWepon( int itemindex);

void BATTLE_BadStatusString( int defNo, int status );
int BATTLE_MultiList( int battleindex, int toNo, int ToList[] );
BOOL BATTLE_IsCharge( int com );
BOOL BATTLE_CanMoveCheck( int charaindex );
int BATTLE_TargetCheck( int battleindex, int defNo);
char *BATTLE_CharTitle( int charaindex );
void BATTLE_EscapeDpSend( int battleindex, int charaindex );
int BATTLE_GetDuelPoint( int battleindex, int side, int num);
int BATTLE_TargetCheckDead(	int battleindex, int defNo);

void BATTLE_MultiListDead( int battleindex, int toNo, int ToList[] );
BOOL BATTLE_WatchTry( int charaindex);
int BATTLE_WatchEntry( int charaindex, int toindex);
void BATTLE_WatchStop( int charaindex );
int BATTLE_WatchUnLink( int battleindex );
void BATTLE_BpSendToWatch( BATTLE *pBattle,	char *pszBcString);

int BATTLE_GetWepon( int charaindex );

#ifdef _ITEM_EQUITSPACE
int BATTLE_GetEqShield( int charaindex );
#endif

int BATTLE_GetAttackCount( int charaindex );
int DoujyouRandomWeponSet( int charaindex );
void BATTLE_AttReverse( int charaindex );
void BATTLE_BadStatusAllClr( int charaindex );
#define CHAR_GETWORKINT_HIGH( index, pos )	( CHAR_getWorkInt( (index), (pos) ) >> 16 )
#define CHAR_SETWORKINT_HIGH( index, pos, set )	{ int iTmp = CHAR_getWorkInt( (index), (pos) ) & 0xFFFF, work = (set);	CHAR_setWorkInt( (index), (pos), (work << 16)|iTmp ); }
#define CHAR_GETWORKINT_LOW( index, pos )	( CHAR_getWorkInt( (index), (pos) ) & 0xFFFF )
#define CHAR_SETWORKINT_LOW( index, pos, set )	{ int iTmp = CHAR_getWorkInt( index, pos ) & 0xFFFF0000, work = (set);	CHAR_setWorkInt( (index), (pos), (work & 0x0000FFFF) | iTmp ); }

int Battle_getTotalBattleNum();

#ifdef _TYPE_TOXICATION
void CHAR_ComToxicationHp( int charaindex);
#endif

#ifdef _PROFESSION_SKILL			// WON ADD �H��¾�~�ޯ�
void BATTLE_ProfessionStatus_init( int battleindex, int charaindex );
void BATTLE_ProfessionStatusSeq( int battleindex, int charaindex);
#endif


#endif
