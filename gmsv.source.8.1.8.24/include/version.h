#ifndef __VERSION_H__
#define __VERSION_H__
#include "correct_bug.h"  //BUG FIX
#include "version_pk.h"   //PK SERVER

/*Server ��������*/
#define _NEW_SERVER_
#define _SERVER_NUMS
//#define _LONGZORO_MAKE
#define _JZ_VIPCODE								//�N�J ADD VIP�N�X
//#define _DEBUG_JZ								//�N�J ADD �u�Ω�ո�-��L���p������
#ifdef _DEBUG_JZ
	#define SERVER_VERSION "SMO v8.1.8.24 For Linux Debug Marking By �N�J"
#else
	#ifdef _JZ_VIPCODE
		#ifdef _LONGZORO_MAKE
			#define SERVER_VERSION "SMO v8.1.8.24 For Linux VIP By �N�J"
		#else
			#define SERVER_VERSION "SMO v8.1.8.24 For Linux VIP-����szoro�n���� By �N�J"
		#endif
	#else
		#define SERVER_VERSION "SMO v8.1.8.24 For Linux PUB By �N�J"
	#endif
#endif

#define UNIQUE_KEYCODEITEM	100
#define UNIQUE_KEYCODEPET	"i"

#define _SA_VERSION_70				// (�i�}��) WON ADD �۾��ɥN7.0 ����������
//#define _DEFAULT_PKEY "Wonwonwon"	// (�i�}��)( �۾� 6.0 )
//#define _RUNNING_KEY  "11111111"	// (�i�}��)( �۾� 6.0 )
#ifdef _LONGZORO_MAKE
	#define _DEFAULT_PKEY "www.longzoro.com"
	#define _RUNNING_KEY  "www.longzoro.com"
#else
	#define _DEFAULT_PKEY "smosmosmo"	// (�i�}��)( �۾� 6.0 )
	#define _RUNNING_KEY  "20070328"	// (�i�}��)( �۾� 6.0 )
#endif
#define _VERSION_20010911	        // (�i�}��) 9.11 �s�X�ק� 

// -------------------------------------------------------------------
//�s�\��G���զ��A���M��	�H���G�ۻ�
#define _TEST_PETCREATE             // (���i�}) ANDY ���d��GM���O
//#define _LOGIN_ADDITEM			// (���i�}) ANDY �n�J�[�����~
//#define _NEW_TESTSERVER			// (���i�}) ANDY ���a�зs�H���ɡA��������125���ŤΤT���d��
//#define _TEST_PETFUSIONTIME		// (���i�}) ANDY �����d���ĦX  ���ɶ��[��
/* -------------------------------------------------------------------
 * �M�סG�a�ڲ���ƶq�ק�	�H���G�p��B�۾��B�Ӱ��B�p��
 */

/*
�ק�˶�ƶq�覡�G
Account Server �{��:
	acfamily.h	MAX_FMPOINT�n�ק�
Account Server �C������:
	saac/family/fmpoint/db_fmpoint�ɮפ����s�W����J�f���е��]�w
	����i�J�ϼh|����i�J��y��|����i�J��y��|����ڪ��ϼh|����F�񤧧���|������餧�a�� index|������餧�a�� name|������餧�a�ں�X�n���
	�Ҧp�G
		100|607|563|1041|1|3|shan|3132238
		200|73|588|2031|2|||
Game Server �{���G
	version.h FAMILY_MAXHOME�BMANORNUM�n�ץ�
	family.c	 FM_PKFLOOR	���W�]�԰��ϼh
				 FAMILY_RidePet() �����[�J�ڪ��a�ϼh
				 FAMILY_LeaderFunc() �����[�J�ܽШ�id
	npcutil.c addNpcFamilyTax function�����[�J(����ϼh%100)��������|������
Game Server �C������:
	gmsv_v2/data/npc/family/manorsman.arg* id���ץ�
	gmsv_v2/data/npc/family/scheduleman.arg* id���ץ�
*/

#define _FAMILY_MANORNUM_CHANGE	// CoolFish �Ψӭק�˶�ƶq(4.0)
#ifdef _FAMILY_MANORNUM_CHANGE
#define _FIX_10_FMPOINT			//�]�i�}��^�N�JADD �B��
#ifdef _FIX_10_FMPOINT
#define FAMILY_MAXHOME			10	// (4.0) �a�ھ��I
#define MANORNUM                10	// (4.0) ����ƶq
#define FMPOINTNUM				10   // (4.0) �����I�a�ڪ��̤j�ƶq
#else
#define FAMILY_MAXHOME			9	// (4.0) �a�ھ��I
#define MANORNUM                9	// (4.0) ����ƶq
#define FMPOINTNUM				9   // (4.0) �����I�a�ڪ��̤j�ƶq
#endif
#endif
#define _FMVER21		        // (�i�}��) �a��¾�x�\��  
#define _PERSONAL_FAME	    	// (�i�}��) Arminius 8.30 �a�ڭӤH�n��  
// -------------------------------------------------------------------
//�M�סG�۾����ʦ۰ʤ�		�H���G��¡B�a��
//�N�J�ק� �h���۾��A�ȭ�NPC���\��
//#define _SERVICE     	    	// (�i�}��) Terry 2001/10/03 ���ʥ\��۰ʤ� 

#define _EVERYONE_RIDE	    	// (�i�}��) Robin 0924 �M�d������  
// -------------------------------------------------------------------
//�M�סG�����ʩG�N���s�@	�H���G�w��
// #define __ATTACK_MAGIC_MSG   // (���}��)
#define __ATTACK_MAGIC          // (�i�}��)	���ѾԳ����������ʩG�N
#define _ADD_ACTION             // (�i�}��) �@��NPC�ʧ@���p���]�w

// -------------------------------------------------------------------
//�M�סG�d���B�D��s�X		�H���G�p��
#define _UNIQUE_P_I	        	// (�i�}��) CoolFish 2001/10/11 

// -------------------------------------------------------------------
//�M�סG�s���H���x�spn �� ������^�� 1018	�H���G�۾�
#define _NEWSAVE                // (�i�}��) Robin
#define _STORECHAR              // (�i�}��) Robin
#define _DROPCHECK              // (�i�}��) Robin ����ƥ�D��
#define _DROPCHECK2             // (�i�}��) Robin ����ƥ�D��
// -------------------------------------------------------------------
//�M�� : ���F��	�H�� : �ۻ�
#define _PET_TRANS           	// (�i�}��) �d����ͥ\��
#define _NEW_WARPMAN        	// (�i�}��) NPC�h��WARP�\��
#define _PET_TALK	            // (�i�}��) �d�����
#define _PET_LIMITLEVEL	        // (�i�}��) �S���d�]�p(����)
#define _MAP_NOEXIT	            // (�i�}��) �S��a�Ϥ��o��n&��n�^���w�I
// -------------------------------------------------------------------
// �M�סGGM���O�B�h��Reload		�H���G�p��
#define _GMRELOAD           	// (�i�}��) CoolFish: 2001/11/12 
// -------------------------------------------------------------------
// �M�סG��l�j��	�H���G�ۻ��B�����B�p��
#define _GAMBLE_ROULETTE	    // (�i�}��) �ۻ� ������L	�_�_�p��  
#define _FIX_GAMBLENUM          // (�i�}��) ���L�ץ� : �U�u���n�� 
#define _GAMBLE_BANK	        // (�i�}��) �ۻ�  ����Ȧ�  
#define _DROPSTAKENEW	        // (�i�}��) CoolFish:  ����U�`  
#define _PETRACE	            // (�i�}��) CoolFish: 2001/12/3 �d���v�t��  
#define _NEWEVENT               // (�i�}��) CoolFish: 2001/12/5 �s�����ȺX��  
//#define _BIGSMALL             // (���i�}) Arminius 12.15 ��l��j�p 
//#define _AUCTIONEER           // (���i�}) Arminius 12.20 ��橱  
// -------------------------------------------------------------------
//�M�סG�@���s�@	�H���G���
#define _ITEM_FIRECRACKER       // (�i�}��) �԰����ϥ��@���i�H��~�~�~��
// -------------------------------------------------------------------
//�s�\��G����PK�W�h(�ŭ��w��)		�H���G�Ӱ��B�ө�
#define _MANOR_PKRULE           // (�i�}��)
#define _MANOR_FIXFMPK          // (�i�}��) WON ADD �a�ڬ��Ԫ� GM ���O�A���եΪ�
// -------------------------------------------------------------------
// �s�\��G�s�\��&�d���ޯ�		�H���G�ۻ�
#define _PSKILL_FALLGROUND		// (�i�}��) �d���ޯ�  (�����N)
#define _ENEMY_FALLGROUND		// (�i�}��) NPC ����
#define _BATTLE_NEWPOWER		// (�i�}��)
#define _Item_MoonAct	        // (�i�}��) �믫���u�@
#define _Item_DeathAct          // (�i�}��) �c�]������
#define _Item_PetAct            // (�i�}��) ���~�٫�
#define _Item_ReLifeAct         // (�i�}��) �ݭnitemset1.txt �~��}) ��������
#define _BATTLESTEAL_FIX        // (�i�}��) �ץ�����
#define _CFREE_petskill         // (�i�}��) �ݭnpetskill1.txt �~��}) �s�ǲ��d���ޯ�ޯ�
#define _data_newreadfile       // (�i�}��) �s��server setup.cf �ɮ� �]�t itemset1.txt petskill1.txt ��ӷs��
#define _ITEM_MAXUSERNUM        // (�i�}��) �s itemset1.txt ITEM �ϥΦ���
// -------------------------------------------------------------------
//�s�\��G��l�G��		�H���G�ۻ�
#define _PET_TALKPRO      		// (�i�}��) �d����ܥ[�j	& �� END
#define _PRO_NPCFIX	     		// (�i�}��) NPC�\��[�j
#define _PRO_BATTLEENEMYSKILL	// (�i�}��) �԰��[�j	����NPC��AI�ޯ�
#define _NEW_WARPPOINT		    // (�i�}��) �ǰe�I�h������P�_WARP
#define _FIX_CHECKITEM	      	// (�i�}��) �D�㤣���D�k�ˬd
// -------------------------------------------------------------------
//�s�\��G��l�G	�H���G�p��,WON
#define _ADD_ENCOUNT             // (�i�}��) WON �W�[�ľD�JĲ�o�ץ� 
#define _WON_PET_MAIL_LOG		 // (�i�}��) WON ���a�H�d�l LOG 
// -------------------------------------------------------------------
// �s�\��G��l�T��	4.0	�H���G�ۻ�, �Ӱ� , �S�a , �ө�
//#define _LOSE_FINCH_				// (���i�}) ANDY �����ޯ�
#define _FIX_FAMILY_PK_LIMIT	    // (�i�}��) WON �N�i�D�Բ��骺�ѫe�Q�j�אּ�e�G�Q�j
#define _ADD_POOL_ITEM				// (�i�}��) WON �W�[�i�H�񪺹D��
#define _ITEM_METAMO                // (�i�}��) shan metamo sprite. code:robin->shan
#define _ENEMY_ATTACK_AI            // (�i�}��) shan enemy attack mode. code:shan  
#define _ITEM_TIME_LIMIT            // (�i�}��) shan time limit of item. code:shan
#define _MIND_ICON                  // (�i�}��) shan show icon from char mind. code:shan
#define _BATTLENPC_WARP_PLAYER      // (�i�}��) shan npc warp player when n round. code:shan
#define _ITEM_WARP_FIX_BI           // (�i�}��) shan fix bbi to bi. code:shan
#define _SKILL_DAMAGETOHP			// (�i�}��) ANDY �ݦ�� 
#define _Skill_MPDAMAGE				// (�i�}��) ANDY MP�ˮ`
#define _WAEI_KICK					// (�i�}��) ANDY ��P�y��H
#define _BATTLE_TIMESPEED			// (�i�}��) ANDY �԰��ɶ� ����(�_�ʤ����}�A�ץ[�t�\��)
#define _NEW_RIDEPETS				// (�i�}��) ANDY �s�M�d
#define _FIXBUG_ATTACKBOW			// (�i�}��) ANDY �ץ��M�d�ϥΧ��Y�Z��bug
#define _NPCCHANGE_PLAYERIMG		// (�i�}��) ANDY NPC���ܪ��a�ϸ�, �j���,�{���a�ϸ� (����)
#define _ITEM_ATTSKILLMAGIC			// (�i�}��) ANDY �D��ޯ�  
#define _EQUIT_DEFMAGIC				// (�i�}��) ANDY ���]�˳�
#define _FIX_MAGICDAMAGE			// (�i�}��) ANDY �ץ��]�k���� �T���}��
#define _ITEM_CONSTITUTION			// (�i�}��) ANDY �ץ��ӤH���
#define _MAGIC_REHPAI				// (�i�}��) ANDY �ɦ�AI
#define _TRANSER_MAN				// (�i�}��) ANDY �ǰe�v
#define _TAKE_ITEMDAMAGE			// (�i�}��) ANDY �D��l�a	ITEMSET2
#define _FIX_FIRECRACKER			// (�i�}��) ANDY �ץ� �@��
#define _FIX_ITEMPROB				// (�i�}��) ANDY �ץ� ���_�v
#define _ADD_DEAMGEDEFC				// (�i�}��) ANDY �B�~�ˮ` & ���[����	ITEMSET2
#define _FIX_MAXCHARMP				// (�i�}��) ANDY MP�W��
#define _ITEM_INSLAY				// (�i�}��) ANDY �^�_�� �ʧ@
#define _ITEMSET2_ITEM				// (�i�}��) ANDY ITEMSET2
#define _ITEM_NPCCHANGE				// (�i�}��) ANDY �u�K (����, ��u)
#define _PETSKILL_FIXITEM			// (�i�}��) ANDY �״_ && �P�D��l�a
#define _FIX_SETWORKINT				// (�i�}��) ANDY (FIXWORKINT)
#define _ITEM_ADDEXP				// (�i�}��) vincent  �D��:���w�ɶ����W�[�g��ȤW�ɴT��
#define _ITEM_ADDEXP2				// ���G�ɶ��i�H�֥[ Robin
#define _FIX_METAMORIDE				// (�i�}��) ANDY
//#define _M_SERVER					// (�i�}��) GM SERVER (�_��&���ꤣ���})
//#define _FIX_MSERVER				// (�i�}��) GM SERVER PROC (�_��&���ꤣ���})
#define _CHECK_ITEMDAMAGE			// (�i�}��) �D��l�a�ץ�	7/26
#define _SKILL_WILDVIOLENT_ATT		// (�i�}��) vincent  �d��:�g�ɧ���
#define _MAGIC_WEAKEN				// (�i�}��) vincent  ���F:��z
#define _SKILL_WEAKEN				// (�i�}��) vincent  �d��:��z//�ݭn�}#define _MAGIC_WEAKEN
#define _SKILL_SPEEDY_ATT			// (�i�}��) vincent  �d��:�e�t����
#define _SKILL_GUARDBREAK2			// (�i�}��) vincent  �d��:�}�����m2
#define _SKILL_SACRIFICE			// (�i�}��) vincent  �d��:�ϴ�
#define _PSKILL_MODIFY				// (�i�}��) ANDY �ݩʱj�Ƨ���
#define _PSKILL_MDFYATTACK			// (�i�}��) ANDY ��������ݩ�
#define _MAGIC_DEFMAGICATT			// (�i�}��) ANDY �]�k���m
#define _MAGIC_SUPERWALL			// (�i�}��) ANDY �K�����m
#define _OTHER_MAGICSTAUTS			// (�i�}��) ANDY 
#define _SKILL_TOOTH				// (�i�}��) ANDY   ���N
#define _MAGIC_DEEPPOISON			// (�i�}��) vincent  ���F:�@�r
#define _MAGIC_BARRIER				// (�i�}��) vincent  ���F:�]��
#define _MAGIC_NOCAST				// (�i�}��) vincent  ���F:�I�q
#define _ITEM_CRACKER               // (�i�}��) vincent  �D��:�Ԭ�//�ݭn�}#define _MIND_ICON
#define _SKILL_DEEPPOISON			// (�i�}��) vincent  �d��:�@�r//�ݭn�}#define _MAGIC_DEEPPOISON
#define _SKILL_BARRIER				// (�i�}��) vincent  �d��:�]��//�ݭn�}#define _MAGIC_BARRIER
#define _SKILL_NOCAST				// (�i�}��) vincent  �d��:�I�q//�ݭn�}#define _MAGIC_NOCAST
#define _SKILL_ROAR					// (�i�}��) vincent  �d��:�j�q(�J�~�~)
#define _SKILL_REFRESH			    // (�i�}��) vincent  �d��:�Ѱ����`���A
#define _ITEM_REFRESH               // (�i�}��) vincent  �Ѱ����`���A�D��
#define _MAGIC_TOCALL				// (�i�}��) kjl		 �l��	02/06/20 kjl

// -------------------------------------------------------------------
// �s�\��G�ӦN�d�\��  �H���G�ۻ�
#define _ITEMSET3_ITEM				// (�i�}��) ANDY itemset3.txt
#define _SUIT_ITEM					// (�i�}��) ANDY (�M��) �ӦN�d �ݥ�itemset3.txt
#define _PETSKILL_SETDUCK			// (�i�}��) ANDY �X�_�ޯ�
#define _VARY_WOLF                  // (�i�}��) pet skill : vary wolf. code:shan
// -------------------------------------------------------------------
// �s�\��G�M��5.0		�H���G
//#define _ITEMTBL_STAIC			// (���i�}) ANDY (�R�AITEMTBL)
#define _USEWARP_FORNUM				// (�i�}��) ANDY �O�ЦФ� �ӦN�d
#define _IMPRECATE_ITEM				// (�i�}��) ANDY �S��D�� �ӦN�d
#define _MAGICPET_SKILL				// (�i�}��) ANDY �]�k�d�ޯ�
#define _ITEM_CHECKWARES			// (�i�}��) ANDY ��w�ǰe����
#define _NPC_REPLACEMENT			// (�i�}��) ANDY �m���԰�npc
#define _EMENY_CHANCEMAN			// (�i�}��) ANDY EMENY ���
#define _TYPE_TOXICATION			// (�i�}��) ANDY ���r�L�k�^��
#define _PET_FUSION					// (�i�}��) ANDY �d���ĦX enemybase1.txt
#define _NPC_FUSION					// (�i�}��) ANDY NPC�d���ĦX
#define _ITEM_EDITBASES				// (�i�}��) ANDY ��i��
#define _PET_EVOLUTION				// (�i�}��) ANDY �i�ƪ�

// -------------------------------------------------------------------
// �s�\��G5.0�l�[����		�H���G
#define _AVID_TRADETRYBUG			// (�i�}��) ANDY �����������d/�D�� �H(�d��)�l�� �۹�
#define _ASSESS_SYSEFFICACY			// (�i�}��) ANDY �����t�ήį� 12/04 ��s
#define _ASSESS_SYSEFFICACY_SUB		// (�i�}��) Robin �����t�ήį� �W�[�ˬd���j��
#define _PREVENT_TEAMATTACK			// (�i�}��) ANDY ����P������ 12/04 ��s
#define _ITEM_ORNAMENTS				// (�i�}��) ANDY �˹��D��	12/04 ��s //�X����
#define _CHIKULA_STONE				// (�i�}��) ANDY �_�J�Ԥ��� 12/04 ��s //�ӦN�d �d�]
#define _SEND_EFFECT				// (�i�}��) WON  AC�e�U���B�U�B���S�� 12/04 ��s
#define _PETMAIL_DEFNUMS			// (�i�}��) ANDY �d���l�󭭨�
#define _TEAM_KICKPARTY				// (�i�}��) ANDY ������H(�D�԰����A)
#define _PETSKILL_TIMID				// (�i�}��) ANDY �d��-�ľ�
#define _PETS_SELECTCON				// (�i�}��) ANDY �d�����i�I��X��
#define _CHRISTMAS_REDSOCKS			// (�i�}��) ANDY �t�Ϭ���
#define _FIX_ARRAYBUG				// (�i�}��) ANDY �ץ�array ����
#define _USER_CHARLOOPS				// (�i�}��) ANDY �s�Ϊ���LOOP
#define _BATTLE_PROPERTY			// (�i�}��) ANDY �԰��ݩʯS��
#define _PETSKILL_PROPERTY			// (�i�}��) ANDY �L���]�~�d��
#define _ITEM_FIXALLBASE			// (�i�}��) ANDY �״_����
#define _ITEM_LOVERPARTY			// (�i�}��) ANDY ���H�`�D��
#define _ITEM_FORUSERNAMES			// (�i�}��) ANDY �D��Ҧ��H
#define _BATTLECOMMAND_TIME			// (�i�}��) ANDY ����a�}�w�t(�^�X�԰��ɶ�)
#define _NPCENEMY_ADDPOWER			// (�i�}��) ANDY �ק�Ǫ�����O��
//#define _BAD_PLAYER				// (���i�}) WON  �e�a���a�h��


// -------------------------------------------------------------------
// �s�\��G�M��6.0		�H���G
//
#define _PETSKILL_CANNEDFOOD		// (�i�}��) ANDY �d���ޯ����Y	for 6.0
#define _TEST_DROPITEMS				// (�i�}��) ANDY ���ե��d
#define _MAP_WARPPOINT				// (�i�}��) ANDY Map WarpPoint
//#define _AUTO_ADDADDRESS			// (�i�}��) ANDY �۰ʷs�W�W��
#define _DEFEND_TIMECHECK			// (�i�}��) ANDY �������ծɶ�
#define _TREASURE_BOX				// (�i�}��) ANDY �_�c
#define _BENEMY_EVENTRUN			// (�i�}��) ANDY �԰�npc���D��
#define _BATTLE_ABDUCTII			// (�i�}��) ANDY �ȳ~�٦�2
#define _BATTLE_LIGHTTAKE			// (�i�}��) ANDY �����N
#define _BATTLE_ATTCRAZED			// (�i�}��) ANDY �ƨg�ɨ�
#define _STATUS_WATERWORD			// (�i�}��) ANDY ���@�ɪ��A
#define _ITEM_WATERWORDSTATUS		// (�i�}��) ANDY ���@�ɪ��A�D��
//#define _KILL_12_STOP_GMSV			// (�i�}��) WON  �Usigusr2������GMSV
//#define _DELBORNPLACE				// (�i�}��) Syu  6.0 �Τ@�X�ͩ�s���
#define _REEDIT_PETBBIS				// (�i�}��) ANDY �ץ��d���i��ϧ�
#define _CAPTURE_FREES				// (�i�}��) ANDY �ɧ����
#define _THROWITEM_ITEMS			// (�i�}��) ���Y�ᦳ���~
#define _OBJSEND_C					// (�i�}��) ANDY _OBJSEND_C
#define _ADD_SHOW_ITEMDAMAGE		// (�i�}��) WON  ��ܪ��~�@�[��

//--------------------------------------------------------------------------
//�M�� 7.0	¾�~�t��	�H���G�ۻ� �ө� ���� �_��

#define _CHANGETRADERULE		    // (�i�}��) Syu ADD ����W�h�׭q
#define _TRADE_SHOWHP				// (�i�}��) Syu ADD �����ܦ�q
#define _SHOOTCHESTNUT				// (�i�}��) Syu ADD �d�ޡG��ߤl
#define _SKILLLIMIT					// (�i�}��) Syu ADD ���o�����ڤ譭��
#define _TRADESYSTEM2				// (�i�}��) Syu ADD �s����t��
#define _NPC_NOSAVEPOINT			// (�i�}��) ANDY �����I���s�H�����
#define _ACFMPK_LIST				// (�i�}��) ANDY �a�ڹ�ԦC��ץ�
#define _PETSKILL2_TXT				// (�i�}��) ANDY petskill2.txt
#define _PETSKILL_CHECKTYPE			// (�i�}��) ANDY �d���ޯ�ϥήɾ�
#define _PETSKILL_TEAR				// (�i�}��) ANDY �d�� �����ˤf
#define _NPC_MAKEPAIR				// (�i�}��) ANDY NPC �t��
#define _ITEMSET4_TXT				// (�i�}��) ANDY itemset4.txt
#define _ITEM_PILENUMS				// (�i�}��) ANDY �D��������| �� itemset4
#define _ITEM_PILEFORTRADE			// (�i�}��) ANDY ������|
#define _ITEM_EQUITSPACE			// (�i�}��) ANDY �s�˳����
#define _PET_LOSTPET				// (�i�}��) ANDY ���d���j�M�p���d��
#define _ITEMSET5_TXT				// (�i�}��) ANDY itemset5.txt
#define _ITEMSET6_TXT				// (�i�}��) ANDY itemset6.txt
#define _EQUIT_ARRANGE				// (�i�}��) ANDY �׮� �ݭn itemset5.txt
#define _EQUIT_SEQUENCE				// (�i�}��) ANDY �������� �ݭn itemset5.txt
#define _EQUIT_ADDPILE				// (�i�}��) ANDY �W�[�i���|�� �ݭn itemset5.txt
#define _EQUIT_HITRIGHT				// (�i�}��) ANDY �R�� �ݭn itemset5.txt
#define _EQUIT_NEGLECTGUARD			// (�i�}��) ANDY �����ؼШ��m�O% �ݭn itemset6.txt
#define _EQUIT_NEWGLOVE				// (�i�}��) ANDY ��M���
#define _FM_JOINLIMIT				// (�i�}��) ANDY �[�J�a�ڱ��󭭨�
#define _FIX_MAX_GOLD				// (�i�}��) WON ADD �W�[�H�������W�� 
#define _CK_ONLINE_PLAYER_COUNT		// (�i�}��) WON �p��u�W�H�� (�n�t�Xac�A�_�h�|���})	
#define _PET_SKILL_SARS				// (�i�}��) WON ADD �r�ٽ���
#define _SONIC_ATTACK				// (�i�}��) WON ADD ���i����
#define _NET_REDUCESEND				// (�i�}��) ANDY ���DB��ƶǰe
#define _FEV_ADD_NEW_ITEM			// (�i�}��) Change �W�[�_���u��
#define _ALLDOMAN					// (�i�}��) Syu ADD �Ʀ�]NPC
#define _LOCKHELP_OK				// (�i�}��) Syu ADD ��w���i�[�J�԰�
#define _TELLCHANNEL				// (�i�}��) Syu ADD �K�y�W�D
// �s�\��G¾�~
#define _NEWREQUESTPROTOCOL			// (�i�}��) Syu ADD �s�WProtocol�n�D�Ӷ�
#define _OUTOFBATTLESKILL			// (�i�}��) Syu ADD �D�԰��ɧޯ�Protocol
#define _CHAR_PROFESSION			// (�i�}��) WON ADD �H��¾�~���
#define _PROFESSION_SKILL			// (�i�}��) WON ADD �H��¾�~�ޯ�
#define _NPC_WELFARE				// (�i�}��) WON ADD ¾�~NPC
#define _PROFESSION_FIX_LEVEL		// (�i�}��) WON ADD �ץ�¾�~�g���(�_��&���ꤣ���})
#define _GM_SIGUSR2					// (�i�}��) Change �U�Fsigusr2����� gm_sigusr_command�ɮפ���GM���O,�Y�S�����O�N����GMSV
#define _GM_BROADCAST				// (�i�}��) WON ADD �ȪA���i�t��
#define _NPC_ADDLEVELUP				// (�i�}��) ANDY NPC�W�[���a����
#define _CHAR_FIXDATADEF			// (�i�}��) ANDY �ץ��H����ƪ�l
//�t�Χ�}
#define _SIMPLIFY_ITEMSTRING		// (�i�}��) ANDY ²�ƹD��r��
#define _CHAR_NEWLOGOUT				// (�i�}��) ANDY �n�X�s����
#define _CHATROOMPROTOCOL			// (�i�}��) Syu ADD ��ѫ��W�D 8/27

#define _CHAR_POOLITEM				// (�i�}��) ANDY �H���D��ܮw�@��
#define _NPC_DEPOTITEM				// (�i�}��) ANDY �H���D��ܮw�@��

#define _SIMPLIFY_PETSTRING			// (�i�}��) ANDY ²���d���s�ɦr��
#define _SIMPLIFY_ITEMSTRING2		// (�i�}��) ANDY ²�ƹD��r��2
#define _WOLF_TAKE_AXE				// (�i�}��) WON ADD �����Y�T������
#define _FIX_UNNECESSARY			// (�i�}��) ANDY �ץ������n���P�_
#define _ITEM_MAGICRECOVERY			// (�i�}��) ANDY �s����u
//9/17��s
#define _PETSKILL_GYRATE			// (�i�}��) Change �d��:�j�ۧ���
#define _PETSKILL_ACUPUNCTURE		// (�i�}��) Change �d��:�w��~�� (client�ݪ�_PETSKILL_ACUPUNCTURE�]�n�})
#define _PETSKILL_RETRACE			// (�i�}��) Change �d��:�l�����
#define _PETSKILL_HECTOR			// (�i�}��) Change �d��:���~
//10/13��s
#define _PETSKILL_FIREKILL			// (�i�}��) Change �d��:���u�y��
#define _PETSKILL_DAMAGETOHP		// (�i�}��) Change �d��:�t��g�T(�ݦ�ު�����) 
#define _PETSKILL_BECOMEFOX         // (�i�}��) Change �d��:�A�b�N
//10/22��s
// �s�\��G�ק�WORKER(�s�I�O����)
#define _FIX_MESSAGE				// (�i�}��) WON ADD �ק�WORKER�ʥ]���e (�_��&���ꤣ���})     

//11/12��s
#define _PETSKILL_SHOWMERCY         // (�i�}) Change �d��:��U�d��
#define _NPC_ActionFreeCmp          // (�i�}) Change ��NPC�Ѽƥ[�J�s������覡
//--------------------------------------------------------------------------


//11/26
#define _NPC_ProfessionTrans      // (�i�}) Change ��¾�~NPC�[�J�P�_���
#define _NPC_NewDelPet            // (�i�}) Change ��NPC�[�J�R���d��(�쥻�N���F,�u�O��warpman�ϥήɤ����\,�ҥH�����@�ӳ��@�U)
//#define _MUSEUM // �۾��ժ��]  Robin 2003/11
	#ifdef _MUSEUM
	#define _DELBORNPLACE
	#endif
#define _CHANGEGALAXY	// (�i�}) �P�t�����@Robin 2003/10
#define _ALLDOMAN_DEBUG           // (�i�}) Change �ץ�����server��Ʀ�]����bug
//20031217
#define _CHRISTMAS_REDSOCKS_NEW	  // (�i�}) Change �s�t�Ϭ���
#define _PETSKILL_COMBINED      // (�i�}) Change �d��:���o�k�� [��X�]�k(�̰Ѽƶüƨ��X�]�k)]
//���H�`����
#define _NPC_ADDWARPMAN1			// (�i�}��) Change npcgen_warpman�W�[�\��(�]�w�ն��H�ơB�]�w�ն��k�k�H��)
#define _ITEM_QUITPARTY			// (�i�}��) Change ����Ѵ��ɯS�w�D��Y�|����
#define _ITEM_ADDEQUIPEXP       // (�i�}) Change �˳ƫ�g��ȼW�[
//2004/2/18
#define _ITEM_LVUPUP            // (�i�}) Change ��}�d�����ŭ����(�ȪZ�M��)
//2004/02/25
#define _PETSKILL_BECOMEPIG     // (�i�}) Change �d��:�Q�O��
#define _ITEM_UNBECOMEPIG       // (�i�}) Change �Q�O�ƪ�����
#define _PET_TALKBBI            // (�i�}) Change �W�[�P�_bbi
#define _ITEM_USEMAGIC			// (�i�}) Change �ѹD��ϥ��]�k
//2004/04/07
#define _ITEM_PROPERTY          // (�i�}) Change ���ܥ|�ݩʹD��
#define _SUIT_ADDENDUM          // (�i�}) Change ���M�˼W�[�\�� (_SUIT_ITEM�n�})
//2004/05/05
#define _ITEM_CHECKDROPATLOGOUT   // (�i�}) Change �ˬd�^�����I�ɨ��W���L�n�X������D��,�Y��,���i�^�����I
#define _FM_MODIFY				// �a�ڥ\��ק�(�}��)
#define _CHANNEL_MODIFY		// �W�D�\���X(�}��)
#define _STANDBYPET		// Robin �ݾ��d�ʥ]
#define	_PROSKILL_OPTIMUM	// Robin ¾�~�ޯ�B�z�̨Τ�
#define	_PETSKILL_OPTIMUM	// Robin �d���ޯ�B�z�̨Τ�
#define _MAGIC_OPTIMUM		// Robin Magic table �̨Τ�
#define _AC_PIORITY		// Nuke ����AC���u���v(�i�})
//2004/05/12
#define _CHAR_POOLPET				// (�i�}��) Robin �H���d���ܮw�@��
#define _NPC_DEPOTPET				// (�i�}��) Robin �H���d���ܮw�@��
#define _TIMEOUTKICK	// (�i�}) �ɬq���H�@Robin 2003/10
#define _MAGIC_RESIST_EQUIT			// (�i�}) WON ADD ¾�~�ܩʸ˳�
#define _LOG_OTHER // �����T���M��LOG

//--------------------------------------------------------------------------
//��������դ�


// -------------------------------------------------------------------
//#define _NPC_QUIZSCOPE          // (���i�}) Change ��NPC�W�[�D��  ��Ѽ�
//#define _ITEM_ADDCOMBO          // (���i�}) Change �˳ƫ�X���v�W�[(�Գ��W�n��2�ӤH�H�W�a�~����)������...
//#define _NPC_AIRDELITEM         // (���i�}) Change �f�[����ŮɩҦ������D��(�N�O��������)
//#define _NPC_AIRLEVEL           // (���i�}) Change �f�[����Ū��̤j�̵��ŭ���(���ŹL�������f)

//#define _ANDYLOG_TEST
//#define _NPC_WELFARE_2			// (���i�}) WON ADD ¾�~NPC-2
//#define _UNIVERSE_CHATROOM			// (���i�}) ANDY ���P�t���
//#define _FIX_ALLDOMAN				// (���i�}) ANDY �ץ��^���Գ�
//�s�@��
//#define _SEND_PROF_DATA			// (���i�}) ANDY ADD �ǰe¾�~��s��cli
//#define _AUCPROTOCOL				// (���i�}) Syu ADD ����W�D�}��Protocol
//#define _NPC_SELLSTH				// (���i�}) ANDY ���t��
//#define _NPC_SEPARATEDATA			// (���i�}) ANDY NPC ��Ƥ���

//------------------------------------------------------------------------
// �H�U���ᵲ�\��
//------------------------------------------------------------------------
//#define fix_login_event			// (���i�}) WON �ɦ^��l�@�����n�� (�_�ʤ����})
//#define _FIX_DEL_MAP				// (���i�}) WON ���a��a�ϰe�ʺ� 
//#define _PETFIGHT					// (���i�}) �d���v�޳� 
//#define _EXM_RANDITEM				// (���i�}) NPC (Exchangeman) �üƭn�D���~ (�w����)
//#define add_amulet_map			// (���i�}) WON �W�[�Ӷ���������  ��
//#define _ACFMPK_NOFREE			// (���i�}) ANDY �a�ڵL����
//--------------------------------------------------------------------------
// 6.0�l�[�\�� pile

//#define _IMPOROVE_ITEMTABLE		// (���i�}) ANDY �ﵽITEM TABLE
//#define _NPC_SHOPALTER01			// (���i�}) Change �R��t�έק�(�t�XClient�אּ�ϧΤ����Ұ����ק�)
//#define _GM_IDENTIFY				// (���i�}) Change �O�_�q�XGM�Y���W�ѧO�r�ꪺGM���O
//#define _GSERVER_RUNTIME          // (���i�}) Change �ǰeGSERVER����h�֮ɶ���Mserver (Mserver�n�t�X)
//#define _NPC_VERYWELFARE			// (���i�}) Change �D�`�n�dA��NPC
//--------------------------------------------------------------------------
//#define _ADD_NEWEVENT             // (���i�}) WON �h�W���ȺX��
//#define _BLACK_MARKET				// (���i�}) shan a black market. code:shan 
//#define _DYNAMIC_BUILDER_MAP		// (���i�}) shan (test)dynamic builder on the upper map. code:shan
//#define _TEST_ITEMMEMORY			// (���i�}) ANDY (����ITEMTABL)
//#define _LASTERR_FUNCTION			// (���i�}) ANDY (lastFunction)
//#define _MARKET_TRADE				// (���i�}) ANDY �������
//#define _PAUCTION_MAN				// (���i�}) ANDY 
//#define _SKILL_SELFEXPLODE		// (���i�}) vincent  �d��:���z(�ʹ�)
//#define _ADD_FAMILY_TAX			// (���i�}) WON �W�[����|�� (�n�t�Xac�A�_�h�|���})
//#define _SYUTRY 
/* -------------------------------------------------------------------
 * �s�\��GPK�P�t 1121 �H���G�۾�,�p��
 */
//#define _PKPETSHOP				// (���i�})

// -------------------------------------------------------------------
//�S���ʥ\��
//#define _CHECK_GAMESPEED			// (���i�}) ANDY �[�t�˴�
//#define _NPCSERVER_NEW			// (���i�}) ANDY NPC ���A��
//#define _OBJS_TALKFUN				// (���i�}) ANDY TALK �j�M�k
#define _NEWOPEN_MAXEXP			// (���i�}) ANDY MAX�g���
//#define _MERGE_LOG				// (���i�}) ANDY �X�� LOG
//#define _CHECK_UCODE				// (���i�}) ANDY �ˬd

//#define _DEATH_FAMILYCONTEND		// (���i�}) ANDY �a�ھԳ��W

//#define _NEW_INSERVERPOINT		// (���i�}) ANDY �n�J�y��
//#define _DEATH_CONTENDWATCH		// (���i�}) ANDY �M���[��
//#define _DEATH_CONTENDTEST		// (���i�}) ANDY �����ɵ{�[��

//#define _DEATH_FAMILY_GM_COMMAND	// (�i�}) WON ADD �a�ھ�GM���O
//#define _DEATH_FAMILY_LOGIN_CHECK	// (�i�}) WON ADD �a�ھԵn�J�ˬd
//#define _DEATH_FAMILY_STRUCT		// (�i�}) WON ADD �a�ھԦs��ӭt���
//#define _DEATH_FAMILY_CANT_SCHEDULE // (�i�}) WON ADD �a�ھԪ��a����ۦ����

//#define _ACTION_BULLSCR			// (���i�}) ANDY �Z��&���� ���ʥ\�� (7/30�H��~������)
//#define _BULL_FIXPLAYER			// (���i�}) ANDY �ץ��H���Z�����ʥ\��
//#define _BULL_CUTBULLSCR			// (���i�}) ANDY �פ��Z�����ʿn��
//#define _ACTION_GMQUE				// (���i�}) ANDY GM���ʥ\�� �ëO�d
//#define _PET_FUSIONSHOWTIME		// (���i�}) ANDY �d���J�q�ɶ�

//#define _NEW_PLAYERGOLD			// (�i�}��) ANDY �s�b������
// -------------------------------------------------------------------
//�n�J�ˬd�ʧ@

//#define _FIX_TSKILLCAN			// (�i�}��) ANDY �ץ��d��T�����Y �H�� �M��T�r��(�D����)
//#define _CHANGEGOATMETAMO			// (�i�}��) Syu  �Ϧ~�~�󴫷s��

// -------------------------------------------------------------------
// �s�\��GGM�K�W�\���X�R
//#define _GM_WARP_PLAYER			// (���i�})	WON �ǰe���a (�_��&���ꤣ���})
//#define _RECAL_ASK_PLAYER			// (���i�}) WON �n�D�H����� (�_��&���ꤣ���})
//#define _RECAL_SEND_COUNT			// (���i�}) WON �ǰeGS��T (�_��&���ꤣ���})
//#define _RECAL_SERVER_OFF			// (���i�}) WON MSERVER ���� GS	(�_��&���ꤣ���})


//#define	_ENEMYTEMP_OPTIMUM	// Robin EnemyBase table �̨Τ�
//#define	_ENEMY_OPTIMUM		// Robin Enemy table �̨Τ�
//#define	_CHECK_BATTLETIME	// Robin �����C�@�԰����O�ɶ�(���ե�)
//#define	_CHECK_BATTLE_IO	// Robin �����԰��ʥ]�ǰe(���ե�)


//�_�誩
//#define _BACK_VERSION	//�_�誩
//#define _REDHAT_V9 //(�i�}) Change �bREDHAT9.0�������U�sĶ�ɥ��}
//#define _UN_FMPOINT // (�i�}) Change �����ӽЮa�ھ��I
//#define _UN_FMMEMO //(�i�})Change  �����a�ڤ����d���O
//--------------------------------------------------------------------------
//�M�� 7.5	���F���۳�	�H���G�۾� �_�� �p��
#define _NPC_EXCHANGEMANTRANS   // (�i�}) Change ��exchangeman�[�J�P�_���
#define	_ANGEL_SUMMON	// Robin �Ѩϥl��
	#ifdef _ANGEL_SUMMON
	#define _ADD_NEWEVENT
	#endif
// Terry define start -------------------------------------------------------------------
#define _STREET_VENDOR		// �\�u�\��(�}��)
#define _WATCH_EVENT			// �d�ߥ��ȥ\��(�}��)
#define _NEW_MANOR_LAW		// �s����W�h(�}��)
#define _MAP_TIME					// �S��a��,�|�˼�(�}��)
#define _PETSKILL_LER			// �p���ޯ�(�}��)
#define _TEACHER_SYSTEM		// �ɮv�t��(�}��)
// Terry define end   -------------------------------------------------------------------
#define _FIX_MAGIC_RESIST				// (�i�}) Change ¾�~�]�k�ܩ�
#define _ADD_DUNGEON					// (�i�}) Change �l�[�a�c
#define _PROFESSION_ADDSKILL			// (�i�}) Change �l�[¾�~�ޯ�
#define _ITEM_ADDPETEXP				// (�i�}) Change �W�[�d���g��ȹD��(�i�Y���D���d��,���`���i��o�g���)
#define _TRANS_6						// (�i�}) Change �H��6��
#define _PET_2TRANS					// (�i�}) Change �d��2��
#define _PET_2LIMITLEVEL				// (�i�}) Change ���U�d��2�઺����
#define _MAGICSTAUTS_RESIST			// (�i�}) Change (���q�B)�ܩʺ��F
#define _EQUIT_RESIST					// (�i�}) Change ��@�˳Ƨܩ�(�i�ܳ�@�\�ಧ�`���A)���ܤ� �p �B ��z �]�� �I�q ����
#define _SUIT_TWFWENDUM				// (�i�}) Change �|�ݩʮM�˥\�� (_SUIT_ITEM�n�})
#define _EXCHANGEMAN_REQUEST_DELPET	// (�i�}) Change ��exchangeman��TYPE:REQUEST�W�[�R���d��
//#define _75_TEST						// (���i�}) 7.5����server
//#define _PK_SERVER //�ʸUpk����
//#define _DEATH_CONTEND			// (�i�}) �ʸUpk���� ANDY ����
//#define _DEATH_CONTENDAB			// (�i�}) �ʸUpk���� ANDY ��������
//--------------------------------------------------------------------------

//#define _ESCAPE_RESET			// �c�_�k�]��Ȯɤ��i�ն�  Robin 2004/07/22
#define _DEL_DROP_GOLD			// �R���a�W�L�ɪ��۹�	Robin
#define _ITEM_STONE				// cyg �_�Ǫ����Y
#define _HELP_NEWHAND			// cyg �s�Ш��⵹���s��D��) 07.4.5�N�J�ק� �Ω�CF�]�m�Ыطs�H�Ұe�D��item1-item15
#define _DEF_FMFREETIME			// cyg GM���O �h�X�a�ڮɶ�����]��0
#define _DEF_GETYOU				// cyg GM���O �d�ߦۤv���  �򤺪����a�b��
#define _DEF_NEWSEND			// cyg GM���O �ǰe���a�ɥi�[�y�l�]�i���[
#define _DEF_SUPERSEND			// cyg GM���O �i�ǰe�w�Y�b�������ߪ���3�椺�Ҧ����a
#define _SUIT_ADDPART3			// (�i�}) Change �M�˥\��ĤT�u �j�״���% �I�q�ܩʴ���% �Ӵ���% ������% �𴣰�%
#define _PETSKILL_2TIMID		// (�i�}) Change �ľԲĤG��(�g���q)
#define CAVALRY_DEBUG			// (�Ȯɶ}) Change �y�M����������
#define _SHOW_FUSION			// (�i�}) Change �ĦX�d���bCLIENT�ݥu�����ͼ�,�קאּ�ĦX�d��ܬ��ĦX(client�ݤ]�n�})
#define _CONTRACT				// (�i�}) Robin �����\�� 
#define _FONT_SIZE				// (�i�}) Robin �r���j�p�\��
#define _PROSK99				// (�i�}) Change �ˬd¾�޵��Űe�կT
#define _TIME_TICKET			// Robin ���ɤJ�� �˪L���d��
#define _HALLOWEEN_EFFECT		// �U�t�`�S��
#define _ADD_STATUS_2			// �s�W�ǰe�H�����A�ĤG��
#define _ADD_reITEM				// (�i�}) Change �ˬd�D�����Ů�
#define _ADD_NOITEM_BATTLE		// �s�WNPC�ˬd�H�����W�S�D��|�i�J�԰�
#define _PETSKILL_BATTLE_MODEL	// �d���ޯ�԰��Ҳ�
#define _RACEMAN				// cyg �y�d�j�|�snpc
#define _PETSKILL_ANTINTER		// (�i�}) Change �d��:�Ƥ��� �Ѧ�_PETSKILL_2TIMID
#define _PETSKILL_REGRET		// (�i�}) Change �d��:�ѥҤ@�� �Ѧ�_SONIC_ATTACK
#define _SUIT_ADDPART4			// (�i�}) Change �M�˥\��ĥ|�u ���r����% �[�j�k�v�]�k(���v30%) ��ܪk�v���]�k

#define _STONEAGEA8XXX_S						//�N�J 8.0�}��
#ifdef _STONEAGEA8XXX_S
//#define _CHECK_ENEMY_PET		// (���i�}) Change �ˬd���a�O�_�������i���d��������(enemy.txt��petflg��쬰0�N���i���ӷ��d��)
//#define _ITEM_TYPETABLE		// (���i�}) Change �l�[typetable
//#define _MERGE_NEW_8			// Robin 8.0�� �s�X���W�h
//#define _RIGHTCLICK			// Robin �ƹ��k��\��
#define _JOBDAILY				// cyg ���Ȥ�x�\��
//#define	_ALCHEMIST			// Robin ��  �\��
//#define _CHECK_ITEM_MODIFY	// �ˬd���`�[�j���D��
//#define _PETSKILL_EXPLODE		// (���i�}) Change �d��:�z������
#define _ITEM_S_TEMP						//�N�J 8.0�D��ʥ]����A���\�ର�{�ɥ\��
#endif
#define _JZ_NEW_CONF						//�N�J ADD �sCF�r�q (���]�A�ЫؤH���e�D��r�q)
#define _JZ_NEW_GMCOMMAND				//�N�J ADD �sGM���O
#define _JZ_IMPROVE_GMCOMMAND		//�N�J ADD ��iGM���O
#define _JZ_NEW_ITEM1						//�N�J ADD �s�W�D��
#define _JZ_NEW_ADDMEMBERPOINT	//�N�J ADD �|���I��
#define _JZ_MANOR_FIX_DFW				//�N�J�ק�T�w�ɶ��U�Ԯ�
//#define _JZ_MAXSOCKET						//�N�J�K�[�̤j�����ǿ�Ҧ�(���i�})
#define _JZ_RIDE								//�۩w�q�M�d
#define _JZ_CAPTURE_FREES				//�N�J ADD ���y�z��������
#define _JZ_CF_DELPETITEM				//�N�J ADD CF�w�q�n���ɧR�����w�d���M���~
#ifdef _JZ_VIPCODE
	#define _JZ_SOCKETNUM		200		//SOCKET�_�l�ƭ�
	//�}��H����}6��\�ॲ���}��H��6��
	#define _JZ_TRANSX	7					//(�i�}) �N�J ADD �H����}6��(���i������)
	#define _JZ_BASE_IF						//(�i�}) �N�J ADD VIP�i�qCF�]�m�ӽв��骺�n��
	#define _JZ_VIPLOG						//(�i�}) �N�J �ϥη|���I�D�㪺LOG�η|���ө�LOG�]�]�t�F�X�ӰO��LOG�����d���D�㪺NPC���O�^
	#define _JZ_ANGELOPEN					//(�i�}) �N�J ADD ���F�l��P�_�}��
#else
	#define _JZ_SOCKETNUM		250
#endif
#define _JZ_MAXLEVEL  200				//�N�J ADD �̰����ť]�ACF�]�m�h����~��F��̰�����
#define _JZ_CIRC_AN							//�N�J ADD �`�����i
#define _JZ_LOCKIP	100					//�N�J ADD GM���O�d���aIP�A�ʪ��aIP(�٦�SAAC��s������)
//
#define _JZ_NEWLOCK							//�N�J ADD ��i������B�z�Ҧ�
#define _JZ_NPCBUF							//�N�J ADD NPC�w�s�BNPC�p�ɾ��BNPC����s�ˬd����
#define _JZ_MEMREDUCE						//�N�J ADD ���s�`�ٶ���
#define _JZ_BEST								//�N�J ADD �u�ƫʥ]�B�z-�����޲z
//#define _JZ_EPOLL								//�N�J ADD EPOLL�ҫ�
///���ն���
//#define _JZ_CKTEST 1							//�ոծ����]��1 
//#define _JZ_MANORTES					//�]���}�^�������60��X�ԮѨåh��6�G00-11�G00�U�ѭ���
#endif
