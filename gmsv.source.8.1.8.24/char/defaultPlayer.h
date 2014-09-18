#include "version.h"

static Char    player=
{
    FALSE,      /*  use     ������������*/
    {
		0,		/*  CHAR_DATAPLACENUMBER */
        0,      /*  CHAR_BASEIMAGENUMBER */
        0,      /*  CHAR_BASEBASEIMAGENUMBER */
		0,		/*  CHAR_FACEIMAGENUMBER */
        0,      /*  CHAR_FLOOR  */
        0,      /*  CHAR_X */
        0,      /*  CHAR_Y  */
        5,      /*  CHAR_DIR 12�ߡ�0���߯ɥ󡼡� */
        1,      /*  CHAR_LV  */
        0,      /*  CHAR_GOLD    */
        1,      /*  CHAR_HP  */
        0,      /*  CHAR_MP  */

        0,      /*  CHAR_MAXMP   */

    	0,		/*  CHAR_VITAL	*/
        0,      /*  CHAR_STR */
        0,      /*  CHAR_TOUGH */
    	0,		/*  CHAR_DEX	*/

    	0,		/*  CHAR_CHARM	*/
    	0,		/*  CHAR_LUCK	*/

		0,		/* �s�۩� */
		0,		/* ���۩� */
		0,		/* �ݾ۩� */
		0,		/* �V�۩� */

		0,		/* CHAR_SLOT*/
		0,		/* CHAR_CRITIAL */
		0,		/* CHAR_COUNTER */
		0,		/* CHAR_RARE */
        0,      /*  CHAR_RADARSTRLENGTH */
        0,      /*  CHAR_CHATVOLUME */
        MAKE2VALUE(100,20),  /*  CHAR_MERCHANTLEVEL  */
        0,      /*  CHAR_HEALERLEVEL    */
        0,      /*  CHAR_DETERMINEITEM  */

        -1,     /*  CHAR_INDEXOFEQTITLE  */


        0,      /*  CHAR_POISON  */
        0,      /*  CHAR_PARALYSIS  */
        0,      /*  CHAR_SILENCE    */
        0,      /*  CHAR_STONE  */
        0,      /*  CHAR_DARKNESS   */
        0,      /*  CHAR_CONFUSION  */

        0,      /*  CHAR_LOGINCOUNT */
        0,      /*  CHAR_DEADCOUNT */
        0,      /*  CHAR_WALKCOUNT */
        0,      /*  CHAR_TALKCOUNT */

    	0,		/*  CHAR_DAMAGECOUNT */
    	0,		/*  CHAR_GETPETCOUNT */
    	0,		/*  CHAR_KILLPETCOUNT */
    	0,		/*  CHAR_DEADPETCOUNT */
    	0,		/*  CHAR_SENDMAILCOUNT */
		0,		/*  CHAR_MERGEITEMCOUNT */

		0,		/*  CHAR_DUELBATTLECOUNT */
		0,		/*  CHAR_DUELWINCOUNT */
		0,		/*  CHAR_DUELLOSECOUNT */
		0,		/*  CHAR_DUELSTWINCOUNT */
		0,		/*  CHAR_DUELMAXSTWINCOUNT */

        CHAR_TYPEPLAYER,    /*  CHAR_WHICHTYPE  */
        1000,   /*  CHAR_WALKINTERVAL   */
        1000,   /*  CHAR_LOOPINTERVAL   */
#ifdef _NEWOPEN_MAXEXP
		0,	// CHAR_OLDEXP,
#endif
		0,	// CHAR_EXP,
        -1,     /*  CHAR_LASTTALKELDER*/
        0,      /*  CHAR_SKILLUPPOINT    */
    	0,		/*  CHAR_LEVELUPPOINT */

        0,      /*  CHAR_IMAGETYPE    */
		CHAR_COLORWHITE,	/* CHAR_NAMECOLOR */
		CHAR_COLORWHITE,	/* CHAR_POPUPNAMECOLOR */
		0,		/*  CHAR_LASTTIMESETLUCK */
		100,	/*  CHAR_DUELPOINT */
		0,	/*  CHAR_EVENT */
		0,	/*  CHAR_EVENT2 */
		0,	/*  CHAR_EVENT3 */
#ifdef _NEWEVENT
		0,	/*  CHAR_EVENT4 */
		0,	/*  CHAR_EVENT5 */
		0,	/*  CHAR_EVENT6 */
#endif
#ifdef _ADD_NEWEVENT
		0,	/*  CHAR_EVENT7 */
		0,	/*  CHAR_EVENT8 */
#endif

		0,	/*  CHAR_NOWEVENT */
		0,	/*  CHAR_NOWEVENT2 */
		0,	/*  CHAR_NOWEVENT3 */
#ifdef _NEWEVENT
		0,	/*  CHAR_NOWEVENT4 */
		0,	/*  CHAR_NOWEVENT5 */
		0,	/*  CHAR_NOWEVENT6 */
#endif
#ifdef _ADD_NEWEVENT
		0,	/*  CHAR_NOWEVENT7 */
		0,	/*  CHAR_NOWEVENT8 */
#endif

		0,  /* CHAR_TRANSMIGRATION */
		0,  /* CHAR_TRANSEQUATION */

        0,      /*  CHAR_INITDATA    */

//#ifdef _PETSKILL_BECOMEPIG
//        -1,
//#endif

		0,	//CHAR_SILENT,				/* char shutup time */    
		0,	//    CHAR_FMINDEX,				// ���� index
		0,	//    CHAR_FMLEADERFLAG,			
								/* �����K�z�P�N
    							 * FMMEMBER_NONE	:��V���͡��z����
    							 * FMMEMBER_APPLY	:���s���Ͳ�����
    							 * FMMEMBER_LEADER	:����
    							 * FMMEMBER_MEMBER	:�����K�z
    							 * FMMEMBER_ELDER	:���l    
    							 * FMMEMBER_INVITE	:�B��			// �J�ȡ���
    							 * FMMEMBER_BAILEE	:��z��			// �J�ȡ���
    							 * FMMEMBER_VICELEADER  :�þ���		// �J�ȡ���
    							*/
		0,	//    CHAR_FMSPRITE,		// �����\�����A

		0,	//    CHAR_BANKGOLD,
		0,	//    CHAR_RIDEPET,
		0,	//    CHAR_LEARNRIDE,
#ifdef _NEW_RIDEPETS
		0,	//	CHAR_LOWRIDEPETS,
		0,	//	CHAR_LOWRIDEPETS1,
#endif
 		0,	//   CHAR_LIMITLEVEL,
#ifdef _PET_FUSION
		0,	//	CHAR_FUSIONCODE,		//�J�P����
		0,	//	CHAR_FUSIONINDEX,		//�����\�J��
		0,	//	CHAR_FUSIONRAISE,		//Ţ�l�[�p
		0,	//	CHAR_FUSIONBEIT,		//�\�|꾧�
		0,	//	CHAR_FUSIONTIMELIMIT,	//Ţ�l�ȤB
#endif

#ifdef _DEATH_CONTEND
		0,	//	CHAR_PKLISTTEAMNUM,
		0,	//	CHAR_PKLISTLEADER,
#endif

#ifdef _PERSONAL_FAME	// Arminius 8.30: �����R������
		0,	//    CHAR_FAME,
#endif
#ifdef _JZ_NEW_ADDMEMBERPOINT
		0,	//		CHAR_MEMBERORDER
#endif
#ifdef _NEWSAVE
		0,	//    CHAR_SAVEINDEXNUMBER,	/*  SaveFile .0.char or .1.char */
#endif
 
#ifdef __ATTACK_MAGIC
		0,	//		CHAR_EARTH_EXP,						// �P���Y�H�U�u�Žu��
		0,	//		CHAR_WATER_EXP,						// �P���Y�V�U�u�Žu��
		0,	//		CHAR_FIRE_EXP,						// �P���Y�W�U�u�Žu��
		0,	//		CHAR_WIND_EXP,						// �P���Y���U�u�Žu��
		0,	//		CHAR_EARTH_RESIST,					// �P���Y�H�U�u���t
		0,	//		CHAR_WATER_RESIST,					// �P���Y�V�U�u���t
		0,	//		CHAR_FIRE_RESIST,					// �P���Y�W�U�u���t
		0,	//		CHAR_WIND_RESIST,					// �P���Y���U�u���t
		0,	//		CHAR_EARTH_ATTMAGIC_EXP,			// �P���Y�H�U�u�Žu���_�Q�G
		0,	//		CHAR_WATER_ATTMAGIC_EXP,			// �P���Y�V�U�u�Žu���_�Q�G
		0,	//		CHAR_FIRE_ATTMAGIC_EXP,				// �P���Y�W�U�u�Žu���_�Q�G
		0,	//		CHAR_WIND_ATTMAGIC_EXP,				// �P���Y���U�u�Žu���_�Q�G
		0,	//	 	CHAR_EARTH_DEFMAGIC_EXP,			// �P���Y�H�U�u���t�_�Q�G
		0,	//		CHAR_WATER_DEFMAGIC_EXP,			// �P���Y�V�U�u���t�_�Q�G
		0,	//		CHAR_FIRE_DEFMAGIC_EXP,				// �P���Y�W�U�u���t�_�Q�G
		0,	//		CHAR_WIND_DEFMAGIC_EXP,				// �P���Y���U�u���t�_�Q�G
#endif


#ifdef _GAMBLE_BANK
		0,	//		CHAR_PERSONAGOLD,	//�٪��R�˽��o
#endif
#ifdef _DROPSTAKENEW
		0,	//		CHAR_GAMBLENUM,		//�٪��ڡ�
#endif
#ifdef _ADD_ACTION          //npc����
		0,	//		CHAR_ACTIONSTYLE,
#endif
#ifdef _AUCTIONEER
		0,	//		CHAR_AUCGOLD,		// �q��s��
#endif
#ifdef _PET_EVOLUTION
		0,	//		CHAR_EVOLUTIONBASEVTL,
		0,	//		CHAR_EVOLUTIONBASESTR,	
		0,	//		CHAR_EVOLUTIONBASETGH,	
		0,	//		CHAR_EVOLUTIONBASEDEX,	
#endif
#ifdef _ACTION_BULLSCR
		0,	//		CHAR_ABULLSTART,
		0,	//		CHAR_ABULLSCORE,
		0,	//		CHAR_ABULLTIME,
		0,	//		CHAR_ABULLSTARTTIME,
#endif

#ifdef _ACTION_GMQUE
		0,	//		CHAR_GMQUEFLG,
		0,	//		CHAR_GMQUENUMS,
#endif

#ifdef _FAMILYBANKSTONELOG
		0,	//		CHAR_FMBANKGOLD, //�������o�Z�O
#endif

#ifdef _FM_JOINLIMIT
		0,	//		CHAR_FMTIMELIMIT,
#endif

#ifdef _CHAR_PROFESSION			// WON ADD ���J���
		0,	//		PROFESSION_CLASS,			// ��򧔸
		0,	//		PROFESSION_LEVEL,			// �����c
//		0,	//		PROFESSION_EXP,				// ���_�Q�G
		0,	//		PROFESSION_SKILL_POINT,		// ���~��p
		0,	//		ATTACHPILE,					// �������X
		0,	//		PROFESSION_FIRE_P,			// �W�Žu��
		0,	//		PROFESSION_ICE_P,			// ��Žu��		
		0,	//		PROFESSION_THUNDER_P,		// ���Žu��
		0,	//		PROFESSION_FIRE_R,			// �W���t
		0,	//		PROFESSION_ICE_R,			// �衼�t	
		0,	//		PROFESSION_THUNDER_R,		// �����t
#endif
#ifdef _ALLDOMAN // (������) Syu ADD �G�o��NPC
		0,	//		CHAR_HEROFLOOR, 
#endif
#ifdef _PETSKILL_BECOMEPIG
		-1,	//		CHAR_BECOMEPIG,
		100250, //CHAR_BECOMEPIG_BBI
#endif
		0,	//		CHAR_LASTLEAVETIME, // Robin add �{�A�f���ȤB

#ifdef _NEW_MANOR_LAW
		0,	//		CHAR_MOMENTUM,
#endif

#ifdef _ITEM_ADDEXP2
		0,	//		CHAR_ADDEXPPOWER,
		0,	//		CHAR_ADDEXPTIME,
#endif

#ifdef _ANGEL_SUMMON
		0,	//		CHAR_HEROCNT, // ���K�i�k���z�Y�[�p
#endif

#ifdef _RACEMAN
		0,	//		CHAR_CHECKIN,	//�\�J�v�����~
		0,	//		CHAR_CATCHCNT,  //�G�\�[�p
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
#endif

    },

    {
        {""},     /*  CHAR_NAME   */
        {""},     /*  CHAR_OWNTITLE   */
        {""},           /*  CHAR_ARGUMENT */
        {""},       /*  CHAR_OWNERCDKEY   */
        {""},       /*  CHAR_OWNERCHARANAME   */
#if 0
        {""},           /*  CHAR_INITFUNC */
        {"core_PreWalk"},    /*  CHAR_WALKPREFUNC    */
        {"core_PostWalk"},    /*  CHAR_WALKPOSTFUNC   */
        {""},           /*  CHAR_PREOVERFUNC    */
        {""},           /*  CHAR_PREOVERFUNC    */
        {"core_PlayerWatch"},     /*  CHAR_WATCHFUNC  */
        {"core_Loop"},  /*  CHAR_LOOPFUNC */
        {"core_Dying"},  /*  CHAR_DYINGFUNC */
        {"core_PlayerTalked"},  /*  CHAR_TALKEDFUNC */
        {""},           /*  CHAR_PREATTACKEDFUNC    */
        {""},           /*  CHAR_POSTATTACKEDFUNC    */
        {""},           /*  CHAR_OFFFUNC    */
        {""},           /*  CHAR_LOOKEDFUNC */
        {""},           /*  CHAR_ITEMPUTFUNC    */
        {""},           /*  CHAR_SPECIALTALKEDFUNC    */
        {""},           /*  CHAR_WINDOWTALKEDFUNC    */
#endif
    },
    {
        SETFLG(1,1,1,1,1,1,0,0 ),
        SETFLG(0,0,0,0,0,0,0,1 ),
    }
};

LevelUpPattern lvplayer00={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer10={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer20={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer30={ {{100,10},{200,10},{50,8}},9,11,10};

LevelUpPattern lvplayer01={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer11={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer21={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer31={ {{100,10},{200,10},{50,8}},9,11,10};

LevelUpPattern lvplayer02={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer12={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer22={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer32={ {{100,10},{200,10},{50,8}},9,11,10};

LevelUpPattern lvplayer03={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer13={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer23={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer33={ {{100,10},{200,10},{50,8}},9,11,10};

LevelUpPattern lvplayer04={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer14={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer24={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer34={ {{100,10},{200,10},{50,8}},9,11,10};

LevelUpPattern lvplayer05={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer15={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer25={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer35={ {{100,10},{200,10},{50,8}},9,11,10};

LevelUpPattern lvplayer06={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer16={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer26={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer36={ {{100,10},{200,10},{50,8}},9,11,10};

LevelUpPattern lvplayer07={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer17={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer27={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer37={ {{100,10},{200,10},{50,8}},9,11,10};

LevelUpPattern lvplayer08={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer18={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer28={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer38={ {{100,10},{200,10},{50,8}},9,11,10};

LevelUpPattern lvplayer09={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer19={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer29={ {{100,10},{200,10},{50,8}},9,11,10};
LevelUpPattern lvplayer39={ {{100,10},{200,10},{50,8}},9,11,10};


