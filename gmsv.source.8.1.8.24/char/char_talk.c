#include "version.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "readmap.h"
#include "object.h"
#include "char.h"
#include "char_base.h"
#include "chatmagic.h"
#include "battle.h"
#include "log.h"
#include "configfile.h"
#include "lssproto_serv.h"
#include "saacproto_cli.h"
#include "family.h"
#ifdef _CHAR_PROFESSION
#include "profession_skill.h"
#endif
#ifdef _M_SERVER
#include "mclient.h"
#endif

#ifdef _NPCSERVER_NEW
#include "npcserver.h"
#endif

#include "net.h"

extern int channelMember[FAMILY_MAXNUM][FAMILY_MAXCHANNEL][FAMILY_MAXMEMBER];

#ifdef _CHANNEL_MODIFY
int *piOccChannelMember = NULL;
#endif
/*------------------------------------------------------------
 * 民乓永玄楮  及末□旦
 ------------------------------------------------------------*/
#define DEBUGCDKEYNUM 100
struct tagDebugCDKey {
	int use;
	char cdkey[9];
};
static struct tagDebugCDKey DebugCDKey[DEBUGCDKEYNUM];

/*====================民乓永玄  芊====================*/
typedef void (*CHATMAGICFUNC)(int,char*);
typedef struct tagCHAR_ChatMagicTable
{
	char*			magicname;
	CHATMAGICFUNC	func;
	BOOL            isdebug;
	int             hash;
	int				level;
	char*			usestring;
}CHAR_ChatMagicTable;

static CHAR_ChatMagicTable CHAR_cmtbl[]={
	//工程師專用
	{ "programming_engineer",	CHAR_CHAT_DEBUG_engineer,	TRUE,	0,	3, ""},
	{ "petlevelup",	CHAR_CHAT_DEBUG_petlevelup,	TRUE,	0,	2, ""},
	{ "help",			CHAR_CHAT_DEBUG_help,				TRUE,	0,	1, "指令/all"},
	{ "showtemp",		CHAR_CHAT_DEBUG_showtemp,			TRUE,	0,	3, "waei"},
#ifdef _EQUIT_NEGLECTGUARD
	{ "setneguard",		CHAR_CHAT_DEBUG_setneguard,			TRUE,	0,	3, "waei"},
#endif
	{ "reload_pkteamlist",	CHAR_CHAT_DEBUG_reloadpkteamlist,		TRUE,	0,	2, ""},
	{ "set_battle",		CHAR_CHAT_DEBUG_setBattle,			TRUE,	0,	2, ""},

#ifdef _DEATH_CONTEND
	{ "updatepklist",	CHAR_CHAT_DEBUG_updatepklist,		TRUE,	0,	3, "無"},
#endif
	//人物屬性
	{ "info",			CHAR_CHAT_DEBUG_info,			TRUE,	0,	1, "數字"},
#ifdef _JZ_RIDE
	{ "reride",			CHAR_CHAT_DEBUG_reride,			TRUE,	0,	3, "無"},
#endif
#ifdef _JZ_CAPTURE_FREES
	{ "recapfree",			CHAR_CHAT_DEBUG_recapfree,			TRUE,	0,	3, "無"},
#endif
#ifdef _JZ_NEW_GMCOMMAND
	{ "維護",		CHAR_CHAT_DEBUG_Support,		TRUE,	0,	3, "時間(分鐘)"},
	{ "settrans",		CHAR_CHAT_DEBUG_setTrans,		TRUE,	0,	2, "數字 帳號 紅利(0為不計紅利,1為計算紅利,此參數可以忽略。默認為1)"},
	{ "BMM",		CHAR_CHAT_DEBUG_BestMM,		TRUE,	0,	3, "1/2 （1=1代MM 2=2代MM）"},
	{ "轉生",		CHAR_CHAT_DEBUG_setNormalTrans,		TRUE,	0,	2, "數字 帳號"},
#else
	{ "settrans",		CHAR_CHAT_DEBUG_setTrans,		TRUE,	0,	2, "數字"},
#endif
	{ "聲望",		CHAR_CHAT_DEBUG_charfame,		TRUE,	0,	1, "帳號 聲望值"},
#ifdef _JZ_IMPROVE_GMCOMMAND
	{ "level",			CHAR_CHAT_DEBUG_level,			TRUE,	0,	1, "數字 帳號"},
	{ "exp",			CHAR_CHAT_DEBUG_exp,			TRUE,	0,	2, "數字 帳號"},
	{ "hp",				CHAR_CHAT_DEBUG_hp,				TRUE,	0,	2, "數字 帳號"},
	{ "mp",				CHAR_CHAT_DEBUG_mp,				TRUE,	0,	2, "數字 帳號"},
	{ "setmp",			CHAR_CHAT_DEBUG_setmp,			TRUE,	0,	2, "數字 帳號"},
	{ "str",			CHAR_CHAT_DEBUG_str,			TRUE,	0,	2, "數字*100 帳號"},
	{ "dex",			CHAR_CHAT_DEBUG_dex,			TRUE,	0,	2, "數字*100 帳號"},
	{ "tgh",			CHAR_CHAT_DEBUG_tgh,			TRUE,	0,	2, "數字*100 帳號"},
	{ "vital",			CHAR_CHAT_DEBUG_vital,			TRUE,	0,	2, "數字*100 帳號"},
	{ "dp",				CHAR_CHAT_DEBUG_dp,				TRUE,	0,	2, "數字"},
#else
	{ "level",			CHAR_CHAT_DEBUG_level,			TRUE,	0,	1, "數字"},
	{ "exp",			CHAR_CHAT_DEBUG_exp,			TRUE,	0,	2, "數字"},
	{ "hp",				CHAR_CHAT_DEBUG_hp,				TRUE,	0,	2, "數字"},
	{ "mp",				CHAR_CHAT_DEBUG_mp,				TRUE,	0,	2, "數字"},
	{ "setmp",			CHAR_CHAT_DEBUG_setmp,			TRUE,	0,	2, "數字"},
	{ "str",			CHAR_CHAT_DEBUG_str,			TRUE,	0,	2, "數字*100"},
	{ "dex",			CHAR_CHAT_DEBUG_dex,			TRUE,	0,	2, "數字*100"},
	{ "tgh",			CHAR_CHAT_DEBUG_tgh,			TRUE,	0,	2, "數字*100"},
	{ "vital",			CHAR_CHAT_DEBUG_vital,			TRUE,	0,	2, "數字*100"},
	{ "dp",				CHAR_CHAT_DEBUG_dp,				TRUE,	0,	2, "數字"},
#endif
	{ "luck",			CHAR_CHAT_DEBUG_luck,			TRUE,	0,	2, "數字"},
	{ "superman",		CHAR_CHAT_DEBUG_superman,		TRUE,	0,	2, "無"},
#ifdef _EQUIT_ARRANGE
	{ "arrange",		CHAR_CHAT_DEBUG_arrange,		TRUE,	0,	3, "數字"},
#endif

#ifdef _EQUIT_SEQUENCE
	{ "sequence",		CHAR_CHAT_DEBUG_sequence,		TRUE,	0,	3, "數字"},
#endif

	//系統
#ifdef _JZ_CIRC_AN
	 { "reloopan",		CHAR_CHAT_DEBUG_reloopan,		TRUE,	0,	1, "無"},
#endif
#ifdef _JZ_LOCKIP
	 { "showip",		CHAR_CHAT_DEBUG_showip,		TRUE,	0,	1, "帳號"},
	 { "lockip",		CHAR_CHAT_DEBUG_lockip,		TRUE,	0,	1, "ip地址"},
	 { "unlockip",		CHAR_CHAT_DEBUG_unlockip,		TRUE,	0,	1, "ip地址"},
	 { "showlockip",		CHAR_CHAT_DEBUG_showlockip,		TRUE,	0,	1, "無"},
#endif
#ifdef _JZ_IMPROVE_GMCOMMAND
	{ "公告",		CHAR_CHAT_DEBUG_announce,		TRUE,	0,	1, "字串"},
#else
	{ "announce",		CHAR_CHAT_DEBUG_announce,		TRUE,	0,	1, "字串"},
#endif
	{ "loginannounce",	CHAR_CHAT_DEBUG_loginannounce,	TRUE,	0,	1, "字串"},
	{ "sysinfo",		CHAR_CHAT_DEBUG_sysinfo,		TRUE,	0,	1, "無"},
	{ "effect",			CHAR_CHAT_DEBUG_effect,			TRUE,	0,	1, "alloff/地圖號 特效"},
	{ "remserver",		CHAR_CHAT_DEBUG_remserver,		TRUE,	0,	2, "on/off"},
	{ "reset",			CHAR_CHAT_DEBUG_reset,			TRUE,	0,	2, "enemy/encount/magic/warppoint/petskill/pettalk/all"},
	{ "clean_floor",	CHAR_CHAT_DEBUG_cleanfloor,		TRUE,	0,	3, "地圖號"},
#ifdef _M_SERVER
	{ "reloadmsip",		CHAR_CHAT_DEBUG_reloadmsip,		TRUE,	0,	3, "address port"},
#endif
	{ "enemyrestart",	CHAR_CHAT_DEBUG_enemyrestart,	TRUE,	0,	3, "無"},
	{ "cleanfreepet",	CHAR_CHAT_DEBUG_cleanfreepet,	TRUE,	0,	3, "無"},
	
#ifdef _GMRELOAD
	{ "gmreload",		CHAR_CHAT_DEBUG_gmreload,		TRUE,	0,	3, "all/cdkey level"},
#endif

	{ "waeikickall",	CHAR_CHAT_DEBUG_waeikickall,	TRUE,	0,	3, "無"},
	{ "checktrade",		CHAR_CHAT_DEBUG_checktrade,		TRUE,	0,	3, "waei"},
	//工具	初級GM
	{ "debug",			CHAR_CHAT_DEBUG_debug,			TRUE,	0,	1, "密碼 debug on/off"},
	{ "metamo",			CHAR_CHAT_DEBUG_metamo,			TRUE,	0,	1, "變身圖號"},
	{ "checklock",		CHAR_CHAT_DEBUG_checklock,		TRUE,	0,	1, "帳號"},
	{ "shutup",			CHAR_CHAT_DEBUG_shutup,			TRUE,	0,	1, "帳號 ON/OFF"},
#ifdef _WAEI_KICK
	{ "gmkick",			CHAR_CHAT_DEBUG_gmkick,			TRUE,	0,	1, "帳號 LSLOCK/KICK/DEUNLOCK/UNLOCKALL/LOCK/TYPE/UNLOCK"},
#endif
    { "battlein",		CHAR_CHAT_DEBUG_battlein,		TRUE,	0,	1, "無"},
    { "battleout",		CHAR_CHAT_DEBUG_battleout,		TRUE,	0,	1, "無"},
	{ "battlewatch",	CHAR_CHAT_DEBUG_battlewatch,	TRUE,	0,	1, "無"},
	{ "getuser",		CHAR_CHAT_DEBUG_getuser,		TRUE,	0,	1, "人物名 地圖號 (npc)"},
	{ "warp",			CHAR_CHAT_DEBUG_warp,			TRUE,	0,	1, "floor x y"},
	{ "waeikick",		CHAR_CHAT_DEBUG_waeikick,		TRUE,	0,	1, "帳號"},
    { "jail",			CHAR_CHAT_DEBUG_jail,			TRUE,	0,	1, "帳號"},
	{ "send",			CHAR_CHAT_DEBUG_send,			TRUE,	0,	1, "floor x y 帳號"},
	{ "sendto",			CHAR_CHAT_DEBUG_sendto,			TRUE,	0,	1, "帳號"},
	{ "noenemy",		CHAR_CHAT_DEBUG_noenemy,		TRUE,	0,	1, "on/off"},
	{ "watchevent",		CHAR_CHAT_DEBUG_watchevent,		TRUE,	0,	1, "無"},
#ifdef _SEND_EFFECT	   	        // WON ADD AC送下雪、下雨等特效
	{ "sendeffect",		CHAR_CHAT_DEBUG_sendeffect,		TRUE,	0,	1, ""},
#endif
	{ "silent",			CHAR_CHAT_DEBUG_silent,			TRUE,	0,	2, "帳號 分鐘"},//禁言
	{ "unlock",			CHAR_CHAT_DEBUG_unlock,			TRUE,	0,	2, "帳號"},
	{ "eventclean",		CHAR_CHAT_DEBUG_eventclean,		TRUE,	0,	2, "all/旗標 帳號 人物名"},
	{ "eventsetend",	CHAR_CHAT_DEBUG_eventsetend,	TRUE,	0,	2, "all/旗標 帳號 人物名"},
	{ "eventsetnow",	CHAR_CHAT_DEBUG_eventsetnow,	TRUE,	0,	2, "all/旗標 帳號 人物名"},

	{ "playerspread",	CHAR_CHAT_DEBUG_playerspread,	TRUE,	0,	3, "waei"},
	{ "shutupall",		CHAR_CHAT_DEBUG_shutupall,		TRUE,	0,	3, "無"},
	{ "unlockserver",	CHAR_CHAT_DEBUG_unlockserver,	TRUE,	0,	3, "無"},
    { "shutdown",		CHAR_CHAT_DEBUG_shutdown,		TRUE,	0,	3, ""},

	//製成
	{ "delitem",		CHAR_CHAT_DEBUG_delitem,		TRUE,	0,	1, "all/位置"},
	{ "delpet",			CHAR_CHAT_DEBUG_deletepet,		TRUE,	0,	1, "all/位置"},
#ifdef _JZ_IMPROVE_GMCOMMAND
	{ "additem",		CHAR_CHAT_DEBUG_additem,		TRUE,	0,	2, "(數量) 道具ID 帳號"},
	{ "petmake",		CHAR_CHAT_DEBUG_petmake,		TRUE,	0,	2, "寵物ID 等級 帳號"},
	{ "gold",			CHAR_CHAT_DEBUG_gold,			TRUE,	0,	2, "數量 帳號"},
#else
	{ "additem",		CHAR_CHAT_DEBUG_additem,		TRUE,	0,	2, "(數量) 道具ID"},
	{ "petmake",		CHAR_CHAT_DEBUG_petmake,		TRUE,	0,	2, "寵物ID"},
	{ "gold",			CHAR_CHAT_DEBUG_gold,			TRUE,	0,	2, "數量"},
#endif
	//家族工具
	{ "manorpk",		CHAR_CHAT_DEBUG_manorpk,		TRUE,	0,	2, "allpeace/peace 莊園編號"},
	{ "fixfmleader",	CHAR_CHAT_DEBUG_fixfmleader,	TRUE,	0,	2, "帳號 1"},
	{ "fixfmpk",		CHAR_CHAT_DEBUG_fixfmpk,		TRUE,	0,	3, ""},
	{ "fixfmdata",		CHAR_CHAT_DEBUG_fixfmdata,		TRUE,	0,	2, ""},
  
	//測試用


#ifdef _CHAR_POOLITEM
	{ "saveditem",		CHAR_CHAT_DEBUG_saveditem,		TRUE,	0,	3, ""},
	{ "insertditem",	CHAR_CHAT_DEBUG_insertditem,	TRUE,	0,	3, ""},
	{ "showdepot",		CHAR_CHAT_DEBUG_ShowMyDepotItems,	TRUE,	0,	3, ""},
	{ "insidedepot",	CHAR_CHAT_DEBUG_InSideMyDepotItems,	TRUE,	0,	3, ""},
#endif

#ifdef _CHAR_POOLPET
	{ "savedpet",		CHAR_CHAT_DEBUG_savedpet,		TRUE,	0,	3, ""},
	{ "insertdpet",		CHAR_CHAT_DEBUG_insertdpet,		TRUE,	0,	3, ""},
	{ "showdepotpet",	CHAR_CHAT_DEBUG_ShowMyDepotPets,	TRUE,	0,	3, ""},
	{ "insidedepotpet",	CHAR_CHAT_DEBUG_InSideMyDepotPets,	TRUE,	0,	3, ""},
#endif


#ifdef _TEST_DROPITEMS
	{ "dropmypet",		CHAR_CHAT_DEBUG_dropmypet,		TRUE,	0,	3, "寵物編號"},
	{ "dropmyitem",		CHAR_CHAT_DEBUG_dropmyitem,		TRUE,	0,	2, "道具編號/(0/1)"},
#endif
#ifdef _CHAR_PROFESSION			// WON ADD 人物職業
	{ "addsk",			CHAR_CHAT_DEBUG_addsk,			TRUE,	0,	2, ""},
	{ "delsk",			CHAR_CHAT_DEBUG_delsk,			TRUE,	0,	2, ""},
#endif
#ifdef _TEST_PETCREATE
	{ "create_pet",		CHAR_CHAT_DEBUG_createpet,		TRUE,	0,	3, ""},
#endif
	{ "checktime",		CHAR_CHAT_DEBUG_checktime,		TRUE,	0,	3, ""},

#ifdef _GAMBLE_BANK
	{ "setgamblenum",	CHAR_CHAT_DEBUG_setgamblenum,	TRUE,	0,	3, ""},
#endif
    // WON ADD 當機指令
	{ "crash",			CHAR_CHAT_DEBUG_crash,			TRUE,	0,	3, ""},
#ifdef _PETSKILL_SETDUCK
	{ "setDuck",		CHAR_CHAT_DEBUG_SetDuck,		TRUE,	0,	3, ""},
#endif

#ifdef _TYPE_TOXICATION
	{ "setTox",			CHAR_CHAT_DEBUG_Toxication,		TRUE,	0,	3, ""},
#endif
#ifdef _ACTION_BULLSCR
	{ "setascore",		CHAR_CHAT_DEBUG_setascore,		TRUE,	0,	3, ""},
	{ "getascore",		CHAR_CHAT_DEBUG_getascore,		TRUE,	0,	2, "all/start/score/btime/stime"},
#endif
	{ "showMem",		CHAR_CHAT_DEBUG_showMem,		TRUE,	0,	2, "無"},
#ifdef _NEW_PLAYERGOLD
	{ "acnewplayer",	CHAR_CHAT_DEBUG_acnewplayer,	TRUE,	0,	3, ""},
	{ "loadnewplayer",	CHAR_CHAT_DEBUG_loadnewplayer,	TRUE,	0,	3, ""},
#endif
#ifdef _ACTION_GMQUE			// ANDY GM活動功能 永保留
	{ "cleanqute",		CHAR_CHAT_DEBUG_cleanqute,		TRUE,	0,	3, ""},
#endif
#ifdef _CHAR_PROFESSION			// WON ADD 人物職業
	{ "profession",		CHAR_CHAT_DEBUG_show_profession,TRUE,	0,	2, ""},
	{ "set_regist",		CHAR_CHAT_DEBUG_set_regist,		TRUE,	0,  1, ""},
#endif
	{ "samecode",		CHAR_CHAT_DEBUG_samecode,		TRUE,	0,	3, ""},
#ifdef _GM_IDENTIFY
	{ "gmidentify",     CHAR_CHAR_DEBUG_gmidentify,     TRUE,   0,  3, ""},
#endif

#ifdef _DEATH_FAMILY_GM_COMMAND	// WON ADD 家族戰GM指令
	{ "reloadfm",		CHAR_CHAR_DEBUG_reloadfm,		TRUE,   0,  1, ""},
	{ "fmpk",			CHAR_CHAR_DEBUG_fmpk,			TRUE,   0,  1, ""},
	{ "fmpk_clean",		CHAR_CHAR_DEBUG_fmpk_clean,		TRUE,   0,  1, ""},
#endif
#ifdef _NEW_MANOR_LAW
	// 設定個人氣勢
	{ "set_momentum",CHAR_CHAT_DEBUG_set_momentum,TRUE,0,2,"[set_momentum <name> <nuber>]"},
	// 設定家族擁有莊園
	{ "set_manor_owner",CHAR_CHAT_DEBUG_set_manor_owner,TRUE,0,2,"[set_manor_owner <manor id> <fmname or fmid>]"},
	// 設定莊園挑戰時間,設定的時間為目前每個挑戰時期的經過時間(單位:分),ex:現在是休戰期,共需要經過五天才會進
	// 入挑戰期,則下指令設定為 5 ,則會由五天的等待期變成了只等五分鐘
	{ "set_schedule_time",CHAR_CHAT_DEBUG_set_schedule_time,TRUE,0,2,"[set_schedule_time <id> <mm>]"},
#endif

#ifdef _ANGEL_SUMMON
	{ "angelinfo",		CHAR_CHAT_DEBUG_angelinfo,		TRUE,   0,  2, ""},
	{ "angelclean",		CHAR_CHAT_DEBUG_angelclean,		TRUE,   0,  2, ""},
	{ "angelcreate",	CHAR_CHAT_DEBUG_angelcreate,	TRUE,   0,  2, ""},
	{ "missionreload",	CHAR_CHAT_DEBUG_missionreload,	TRUE,   0,  2, ""},
#endif

	{ "itemreload",	CHAR_CHAT_DEBUG_itemreload,	TRUE,   0,  2, ""},

	{ "skywalker",	CHAR_CHAT_DEBUG_skywalker,	TRUE,   0,  1, ""},
#ifdef _ITEM_ADDEXP
	{ "itemaddexp",	CHAR_CHAT_DEBUG_itemaddexp,	TRUE,   0,  1, ""},
#endif

#ifdef _DEF_FMFREETIME 
	{ "fmfreetime",		CHAR_CHAT_DEBUG_fmfreetime,		TRUE,   0,  1, "帳號 人物名稱"},
#endif
#ifdef _DEF_GETYOU	  
	{ "getyou",		    CHAR_CHAT_DEBUG_getyou,			TRUE,   0,  1, "格數 1~3"},
#endif 
#ifdef _DEF_NEWSEND
	{ "newsend",        CHAR_CHAT_DEBUG_newsend,        TRUE,   0,  1, "floor x y 帳號 無/要說的話"},   
#endif
#ifdef _DEF_SUPERSEND
	{ "supersend",      CHAR_CHAT_DEBUG_supersend,      TRUE,   0,  3, "floor x y 格數 無/要說的話"},   
#endif
#ifdef _FONT_SIZE
	{ "fsize",		CHAR_CHAT_DEBUG_fsize,	TRUE,   0,  1, ""},
#endif
#ifdef _JOBDAILY
	{ "rejobdaily",		CHAR_CHAT_DEBUG_rejobdaily,     TRUE,   0,  3, "無"},   
#endif
#ifdef _RACEMAN
	{ "changeask",		CHAR_CHAT_DEBUG_changeask,      TRUE,   0,  2, "帳號 寵證id 0/1"},
#endif
};

void CHAR_initDebugChatCdkey( void)
{
	int i;
	for( i = 0; i < DEBUGCDKEYNUM; i ++ ) {
		DebugCDKey[i].use = FALSE;
		DebugCDKey[i].cdkey[0] = '\0';
	}
}
/*------------------------------------------------------------
 * 民乓永玄  芊及甩永撲亙襖毛綜月〔
 * 婁醒｝忒曰襖
 *  卅仄
 ------------------------------------------------------------*/
void CHAR_initChatMagic(void)
{
	int i;
	for( i=0 ; i<arraysizeof(CHAR_cmtbl) ; i++ )
		CHAR_cmtbl[i].hash = hashpjw( CHAR_cmtbl[i].magicname);
	for( i = 0; i < DEBUGCDKEYNUM; i ++ ) {
		DebugCDKey[i].use = FALSE;
		DebugCDKey[i].cdkey[0] = '\0';
	}
}

int CHAR_setChatMagicCDKey( int mode, char *cdkey)
{
	int i;
	BOOL found = FALSE;
	if( strlen( cdkey) > 8 ) {
		return -1;
	}
	for( i = 0; i < DEBUGCDKEYNUM; i ++ ) {
		if( DebugCDKey[i].use == FALSE) {
			if( mode == 0 ) {
				DebugCDKey[i].use = TRUE;
				strcpysafe( DebugCDKey[i].cdkey, sizeof( DebugCDKey[i].cdkey), cdkey);
				found = TRUE;
				break;
			}
		}
		else {
			if( mode == 1 ) {
				if( strcmp( DebugCDKey[i].cdkey, cdkey) == 0 ) {
					DebugCDKey[i].use = FALSE;
					DebugCDKey[i].cdkey[0] = '\0';
					found = TRUE;
				}
			}
		}
				
	}
	if( !found ) return -1;
	return i;
}

static CHATMAGICFUNC CHAR_getChatMagicFuncPointer(char* name, BOOL isDebug)
{
	int i;
	int hash = hashpjw( name );
	for( i=0 ; i<arraysizeof(CHAR_cmtbl) ; i++ )
		if( CHAR_cmtbl[i].hash == hash
			&& CHAR_cmtbl[i].isdebug == isDebug
			&& strcmp( CHAR_cmtbl[i].magicname, name ) == 0 )
			return CHAR_cmtbl[i].func;
	return NULL;
}

#ifdef _GM_SIGUSR2
CHATMAGICFUNC gm_CHAR_getChatMagicFuncPointer(char* name,BOOL isDebug)
{
    return CHAR_getChatMagicFuncPointer(name,isDebug);
}
#endif

int CHAR_getChatMagicFuncLevel(char* name,BOOL isDebug)
{
	int i;
	int hash = hashpjw( name );
	for( i=0 ; i<arraysizeof(CHAR_cmtbl) ; i++ )
		if( CHAR_cmtbl[i].hash == hash
			&& CHAR_cmtbl[i].isdebug == isDebug
			&& strcmp( CHAR_cmtbl[i].magicname, name ) == 0 )
			return CHAR_cmtbl[i].level;
	return -1;
}

int CHAR_getChatMagicFuncNameAndString( int ti, char* name, char *usestring, int level, BOOL isDebug)
{
	if( name == NULL || usestring == NULL ) return -1;
	if( ti < 0 || ti >= arraysizeof( CHAR_cmtbl) ) return -1;
	if( CHAR_cmtbl[ ti].isdebug == isDebug &&
		CHAR_cmtbl[ ti].level <= level ){
		sprintf( name,"%s", CHAR_cmtbl[ ti].magicname );
		sprintf( usestring, "%s", CHAR_cmtbl[ ti].usestring );
		return 1;
	}
	return 0;
}

int CHAR_getChatMagicFuncMaxNum( void)
{
	return arraysizeof( CHAR_cmtbl);
}

/*------------------------------------------------------------
 * 民乓永玄  芊
 * 婁醒
 *  charaindex      int     平乓仿奶件犯永弁旦
 *  message         char*   丟永本□斥
 *  isDebug         BOOL    犯田永弘民乓永玄  芊井升丹井
 * 忒曰襖
 *  卅仄
 ------------------------------------------------------------*/
static BOOL CHAR_useChatMagic( int charaindex, char* data, BOOL isDebug)
{
	char    magicname[256];
	int     ret;
	int	i;
	int	gmLevel = 0, magicLevel;
	
	CHATMAGICFUNC   func;

#ifdef _GMRELOAD
	extern struct GMINFO gminfo[GMMAXNUM];
#else
#endif
	char *p = CHAR_getChar( charaindex, CHAR_CDKEY);
	if( !p ) {
		fprint( "err nothing cdkey\n");
		return FALSE;
	}

	if( getChatMagicCDKeyCheck() == 1 ){ //第一次確認GM帳號
		if( CHAR_getWorkInt( charaindex, CHAR_WORKFLG) & WORKFLG_DEBUGMODE ) {
			gmLevel = CHAR_getWorkInt( charaindex, CHAR_WORKGMLEVEL);
		}else{

		
#ifdef _GMRELOAD
			for (i = 0; i < GMMAXNUM; i++){
				if (strcmp( p, gminfo[i].cdkey) == 0){
					gmLevel = gminfo[i].level;
					CHAR_setWorkInt( charaindex, CHAR_WORKGMLEVEL, gmLevel);
					break;
				}
			}
#else		
#endif
			if( i >= GMMAXNUM ){
				for( i = 0; i < DEBUGCDKEYNUM; i ++ ) {
					if( DebugCDKey[i].use && strcmp( p, DebugCDKey[i].cdkey) == 0 ) {
						break;
					}
				}
				if( i >= DEBUGCDKEYNUM ) return FALSE;
			}
		}

	}else {
		gmLevel = 3;
		CHAR_setWorkInt( charaindex, CHAR_WORKGMLEVEL, gmLevel);
	}

	ret = getStringFromIndexWithDelim( data, " ", 1, magicname,  sizeof( magicname));
	if( ret == FALSE)return FALSE;

	// Robin 0618  chaeck GM Level
	magicLevel = CHAR_getChatMagicFuncLevel( magicname, isDebug);
	if( gmLevel < magicLevel )
		return FALSE;

	func = CHAR_getChatMagicFuncPointer(magicname,isDebug);
	if( func ){
		LogGM( CHAR_getUseName( charaindex), CHAR_getChar( charaindex, CHAR_CDKEY), data, 
			CHAR_getInt( charaindex, CHAR_FLOOR), CHAR_getInt( charaindex, CHAR_X),
			CHAR_getInt( charaindex, CHAR_Y) );
		func( charaindex, data + strlen( magicname)+1);
		return TRUE;
	}else{
		return FALSE;
	}
}

/*------------------------------------------------------------
 * 祥洶井日汔毛菲戶月
 * 婁醒
 *  volume  int     祥洶
 * 忒曰襖
 *  穴永皿匹及穴旦
 ------------------------------------------------------------*/
static int CHAR_getRangeFromVolume( int volume )
{
	static int chatvol[]={
		3,5,7,9,11,13,15,17,19,21,23,25,27,29,31
	};

	if( volume < 0 )return 0;
	else if( arraysizeof( chatvol ) <= volume ) {
		return chatvol[arraysizeof(chatvol)-1];
	}
	return chatvol[volume];
}
/*------------------------------------------------------------
 * 擘及示伉亙□丞毛繭月〔條卅中支曰井凶〔
 * 婁醒
 *  mesg        char*       仄扎屯月蛻邯
 * 忒曰襖
 *  -1 及午五反仇公仇公
 *  0  及午五反孔勾丹
 *  1 方曰云云五中午｝仃勾卞勾中化中月 ! 及醒
 ------------------------------------------------------------*/
static int CHAR_getVolume( char* mesg )
{
	int stringlen = strlen( mesg );
	if( stringlen == 0 )
		return 0;
	else if( stringlen == 1 ){
		if( mesg[0] == '!')return 1;
		else               return 0;
	}else if( stringlen == 2 ){
		if( mesg[1] == '!'){
			if( mesg[0] == '!' )return 2;
			else                return 1;
		}else
			return 0;
	}else{
		/*  3動曉   */
		if( mesg[stringlen-1] == '.' ){
			/*  仇公仇公及第  嶺丐曰    */
			if( mesg[stringlen-2] == '.' && mesg[stringlen-3] == '.' ){
				/*  仇公仇公    */
				return -1;
			}
			return 0;
		}else{
			/*  仇仇井日 ! 毛醒尹月*/
			int exnum=0;
			int i;
			for( i=stringlen-1; i>=0 ; i-- ){
				if( mesg[i] != '!' )
					break;
				else
					exnum++;
			}
			return exnum;
		}
	}
}

/*------------------------------------------------------------
 * 丟永本□斥及    井日丟永本□斥及      坌毛潸曰請允
 * 婁醒
 *  message     char*           丟永本□斥    
 *  kind        char*           p or s or i卅及丟永本□斥
 *  kindlen     int             kind 及贏今
 *  body        char**          丟永本□斥    及禾奶件正□及傘  樺赭
 * 忒曰襖
 *  卅仄
 ------------------------------------------------------------*/
void CHAR_getMessageBody(char* message,char* kind,int kindlen,char** body)
{
    int firstchar;

	/* 1  儂  毛切之勻仁〔1  儂匹丐月仇午反lssproto.html互忡據 */
	// Nuke +1: For invalid message attack
	*body = 0;
    firstchar = message[0];
    if( firstchar == 'P' ||
        firstchar == 'S' ||
        firstchar == 'D'

        // CoolFish: Trade 2001/4/18
	|| firstchar == 'C'
        || firstchar == 'T'
        || firstchar == 'W'
                                
        // CoolFish: Family 2001/5/28
	|| firstchar == 'A'
        || firstchar == 'J'
        || firstchar == 'E'
        || firstchar == 'M'
        
        || firstchar == 'B'
        || firstchar == 'X'
        || firstchar == 'R'
        || firstchar == 'L'

         ){
        if( kindlen >= 2 ){
            kind[0] = firstchar;
            kind[1] = '\0';
        } else {
            return;
        }
    } else {
        return;
    }

	*body = message + 2;
}


static BOOL CHAR_Talk_check( int talkerindex, int talkedindex, int micflg )
{
#if 1
	if( !CHAR_getFlg( talkerindex, CHAR_ISPARTYCHAT )){
		int talker_b_mode = CHAR_getWorkInt( talkerindex, CHAR_WORKBATTLEMODE);
		int talked_b_mode = CHAR_getWorkInt( talkedindex, CHAR_WORKBATTLEMODE);
		if( talker_b_mode != BATTLE_CHARMODE_NONE
			&& CHAR_getInt( talkedindex, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER){
			return FALSE;
		}
		if( micflg != 0 )return TRUE;
		if( talker_b_mode != BATTLE_CHARMODE_NONE && talked_b_mode != BATTLE_CHARMODE_NONE){
			if( CHAR_getWorkInt( talkerindex, CHAR_WORKBATTLEINDEX)
				!= CHAR_getWorkInt( talkedindex, CHAR_WORKBATTLEINDEX)){
				return FALSE;
			}
		}else if( talker_b_mode != BATTLE_CHARMODE_NONE || talked_b_mode != BATTLE_CHARMODE_NONE){
		 	return FALSE;
		}
		return TRUE;
	}
	if( CHAR_getInt( talkerindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER) {
		int talker_b_mode = CHAR_getWorkInt( talkerindex, CHAR_WORKBATTLEMODE);
		int talked_b_mode = CHAR_getWorkInt( talkedindex, CHAR_WORKBATTLEMODE);

		if( talker_b_mode != BATTLE_CHARMODE_NONE && CHAR_getInt( talkedindex, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER){
			return FALSE;
		}
		if( micflg != 0 )return TRUE;
		if( CHAR_getWorkInt( talkerindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE ){
			if( CHAR_getWorkInt( talkedindex, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE){
				if( CHAR_getWorkInt( talkerindex, CHAR_WORKPARTYINDEX1)
					== CHAR_getWorkInt( talkedindex, CHAR_WORKPARTYINDEX1) ){
					return TRUE;
				}
			}
		}else {
			if( talker_b_mode != BATTLE_CHARMODE_NONE && talked_b_mode != BATTLE_CHARMODE_NONE){
				if( CHAR_getWorkInt( talkerindex, CHAR_WORKBATTLEINDEX)
					!= CHAR_getWorkInt( talkedindex, CHAR_WORKBATTLEINDEX)){
					return FALSE;
				}
			}else if( talker_b_mode != BATTLE_CHARMODE_NONE ||
					talked_b_mode != BATTLE_CHARMODE_NONE){
			 	return FALSE;
			}
			return TRUE;
		}
	}
	return FALSE;

#else
	int		MyBattleMode;
	int		ToBattleMode;

	MyBattleMode = CHAR_getWorkInt( talkerindex, CHAR_WORKBATTLEMODE);
	ToBattleMode = CHAR_getWorkInt( talkedindex, CHAR_WORKBATTLEMODE);

	/*   爵    及凜 */
	if( MyBattleMode == BATTLE_CHARMODE_NONE ) {
		if( ToBattleMode == BATTLE_CHARMODE_NONE ) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	/* 爵    及凜 */
	else {
		/* 爵  仄化中卅中諦卞反霜日卅中〔 */
		if( ToBattleMode == BATTLE_CHARMODE_NONE) {
			return FALSE;
		}
		/*   元爵  匹  元扔奶玉及諦卞仄井霜耨請  卅中 */
		if( CHAR_getWorkInt( talkerindex, CHAR_WORKBATTLEINDEX)
			== CHAR_getWorkInt( talkedindex, CHAR_WORKBATTLEINDEX) &&
			CHAR_getWorkInt( talkerindex, CHAR_WORKBATTLESIDE)
			== CHAR_getWorkInt( talkedindex, CHAR_WORKBATTLESIDE ) )
		{
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	return FALSE;
#endif
}
#ifdef _TELLCHANNEL				// (不可開) Syu ADD 密語頻道
static int TalkCount = 0 ; 
void OneByOneTkChannel ( int fd , char *tmp1 , char *tmp2 , int color )
{
	int i ; 
	int myindex ; 
    int playernum = CHAR_getPlayerMaxNum();
	char buf[512];
	int IndexList[10] = { 0 } ; 
	int IndexCount = 0 ; 
	char *addr;
	int target;
	myindex = CONNECT_getCharaindex(fd);

DebugPoint=300;
	if( strlen( tmp2) > (sizeof(buf)-1) ) return;
DebugPoint=301;
	memset ( IndexList , -1 , sizeof( IndexList ) ); 
DebugPoint=302;

	for( i = 0 ; i < playernum ; i++) {
		if( CHAR_getCharUse(i) != FALSE ) {
			if (!CHAR_CHECKINDEX(i))       
				continue;
			if ( strcmp ( tmp1 , CHAR_getChar( i , CHAR_NAME ) ) == 0 ) {
				IndexList[ IndexCount ] = i ; 
				IndexCount ++ ; 
				if ( IndexCount >= 10 ) break;
			}
		}
	}
DebugPoint=303;
	if ( IndexCount == 1 ) {
DebugPoint=304;
		if ( myindex == IndexList[0] )
			return;
#ifdef _CHANNEL_MODIFY
		if(CHAR_getFlg(IndexList[0],CHAR_ISTELL) == FALSE){
			snprintf(buf,sizeof(buf) - 1,"%s 關閉了此頻道" ,tmp1);
			CHAR_talkToCli(myindex,-1,buf,color);
			return;
		}
#endif
#ifndef _CHANNEL_MODIFY
		snprintf( buf, sizeof( buf)-1, "你告訴%s：%s" , tmp1 , tmp2 ) ; 
		CHAR_talkToCli( myindex, -1, buf , color);
		snprintf( buf , sizeof( buf)-1,"%s告訴你：%s" , CHAR_getChar ( myindex , CHAR_NAME ) , tmp2 ) ; 
		CHAR_talkToCli( IndexList[0] , -1, buf , color);
#else
		snprintf(buf,sizeof(buf) - 1,"P|M|你告訴%s：%s",tmp1,tmp2); 
		lssproto_TK_send(fd,CHAR_getWorkInt(myindex,CHAR_WORKOBJINDEX),buf,color);
		snprintf(buf,sizeof(buf) - 1,"P|M|%s告訴你：%s",CHAR_getChar(myindex,CHAR_NAME),tmp2);
		lssproto_TK_send(getfdFromCharaIndex(IndexList[0]),CHAR_getWorkInt(IndexList[0],CHAR_WORKOBJINDEX),buf,color);
#endif
		TalkCount ++ ; 
	}else if ( IndexCount > 1 && IndexCount < 10 ) {
DebugPoint=305;
		print( "\nSyu log 進入同名Func" );
		if ( ( addr = strstr ( tmp2 , "/T" ) ) != NULL ) {
			addr = addr + 2 ; 
			target = atoi ( addr ) ; 
			print ( "\nSyu log addr => %s , target => %d " , addr , target ) ; 
			addr = strtok ( tmp2 , "/T" ) ; 
			if (!CHAR_CHECKINDEX(IndexList[target]))       
				return;
			if ( strcmp ( tmp1 , CHAR_getChar ( IndexList[ target ] , CHAR_NAME ) ) == 0 &&
				IndexList[ target ] != -1 ) {
				if ( myindex == IndexList[ target ] ) 
					return ; 
#ifdef _CHANNEL_MODIFY
				if(CHAR_getFlg(IndexList[target],CHAR_ISTELL) == FALSE){
					snprintf(buf,sizeof(buf) - 1,"%s 關閉了此頻道" ,tmp1);
					CHAR_talkToCli(myindex,-1,buf,color);
					return;
				}
#endif
#ifndef _CHANNEL_MODIFY
				snprintf( buf , sizeof( buf)-1, "你告訴%s：%s" , tmp1 , addr ) ; 
				CHAR_talkToCli( myindex, -1, buf , color);
				snprintf( buf , sizeof( buf)-1, "%s告訴你：%s" , CHAR_getChar ( myindex , CHAR_NAME ) , addr ) ; 
				CHAR_talkToCli( IndexList[ target ] , -1, buf , color);
#else
				snprintf(buf,sizeof(buf) - 1,"P|M|你告訴%s：%s",tmp1,addr); 
				lssproto_TK_send(fd,CHAR_getWorkInt(myindex,CHAR_WORKOBJINDEX),buf,color);
				snprintf(buf,sizeof(buf) - 1,"P|M|%s告訴你：%s",CHAR_getChar(myindex,CHAR_NAME),addr);
				lssproto_TK_send(getfdFromCharaIndex(IndexList[target]),CHAR_getWorkInt(IndexList[target],CHAR_WORKOBJINDEX),buf,color);
#endif
				TalkCount ++ ;
			}
		}else {
DebugPoint=306;
			for ( i = 0 ; i < 10 ; i ++ ) {
				if ( IndexList[ i ] == -1 ) break;
				if (!CHAR_CHECKINDEX(IndexList[i])) break;
#ifndef _CHANNEL_MODIFY
				sprintf( buf , "TK|%d|%d|%d|%s|%s" , 
					i , 
					CHAR_getInt ( IndexList[ i ] , CHAR_TRANSMIGRATION ) , 
					CHAR_getInt ( IndexList[ i ] , CHAR_LV ) , 
					CHAR_getChar ( IndexList[ i ] , CHAR_NAME ) , 
					CHAR_getChar ( IndexList[ i ] , CHAR_OWNTITLE )  
					 ) ; 
				CHAR_talkToCli( myindex , -1, buf , color);
#else
				sprintf( buf , "P|TK|%d|%d|%d|%s|%s" , 
					i , 
					CHAR_getInt ( IndexList[ i ] , CHAR_TRANSMIGRATION ) , 
					CHAR_getInt ( IndexList[ i ] , CHAR_LV ) , 
					CHAR_getChar ( IndexList[ i ] , CHAR_NAME ) , 
					CHAR_getChar ( IndexList[ i ] , CHAR_OWNTITLE )  
					 ) ; 
				lssproto_TK_send(fd,CHAR_getWorkInt(myindex,CHAR_WORKOBJINDEX),buf,color);
#endif
			}
#ifndef _CHANNEL_MODIFY
			snprintf( buf , sizeof( buf)-1, "TE|%s" , tmp2 ) ; 
			CHAR_talkToCli ( myindex , -1 , buf , color ) ; 
#else
			snprintf( buf , sizeof( buf)-1, "P|TE|%s" , tmp2 ) ; 
			lssproto_TK_send(fd,CHAR_getWorkInt(myindex,CHAR_WORKOBJINDEX),buf,color);
#endif
		}
	}else if ( IndexCount == 0 ) {
DebugPoint=307;
		sprintf( buf , "沒有這個人或不位於這個星球" ) ; 
		CHAR_talkToCli( myindex, -1, buf , color);
	}
DebugPoint=308;
//	print ( "\nSyu log TkChannel use ratio : %d " , TalkCount ) ; 
}
#endif


void CHAR_Talk( int fd, int index,char* message,int color, int area )
{
	char    firstToken[64];
	char    messageeraseescape[512];
	char*   messagebody;
	int     mesgvolume=0;
	int     lastvolume=0;
	int     fl,x,y;
	int     stringlen;
	int		micflg = 0;
#ifdef _TELLCHANNEL				// (不可開) Syu ADD 密語頻道
	char    tmp1[128] ; 
	char	*tmp2;
#endif	
	int	fmindexi = CHAR_getWorkInt( index, CHAR_WORKFMINDEXI );
	int	channel = CHAR_getWorkInt( index, CHAR_WORKFMCHANNEL );
	int	quickchannel = CHAR_getWorkInt( index, CHAR_WORKFMCHANNELQUICK );
	{
		char   *cdkey = CHAR_getChar( index, CHAR_CDKEY);
		char *charname = CHAR_getChar( index, CHAR_NAME);
		if( strlen(message) > 3 ){
			if( CHAR_getWorkInt( index, CHAR_WORKFLG) & WORKFLG_DEBUGMODE )	{
				// shan begin
				if(strstr( message, "[")&&strstr( message, "]")){
				}else{
					// original
					LogTalk( charname, cdkey, CHAR_getInt( index, CHAR_FLOOR),
							 CHAR_getInt( index, CHAR_X), CHAR_getInt( index, CHAR_Y),
							 message );
				}
				// end
			}
		}
	}
	//bg|0|r0|fc|d4B8|p0|bn|10|bt|10| 
	// Nuke +1 08/27: For invalid message attack
	if (*message==0) return;
	CHAR_getMessageBody(message,firstToken,sizeof(firstToken),
						&messagebody);
        // Nuke +1: For invalid message attack
        if (!messagebody) return;
                						
	strcpysafe( messageeraseescape,sizeof(messageeraseescape),
				messagebody );
	makeStringFromEscaped( messageeraseescape );
	stringlen = strlen(  messageeraseescape  );
	if( stringlen <= 0 ) return;
#ifdef _TELLCHANNEL				// (不可開) Syu ADD 密語頻道
	if (messageeraseescape[0]== '/' && messageeraseescape[1]== 't' &&
		messageeraseescape[2]== 'e' && messageeraseescape[3]== 'l' && 
		messageeraseescape[4]== 'l'){
DebugPoint=310;
		getStringFromIndexWithDelim( message , " ", 2, tmp1, sizeof(tmp1));
		if ( (tmp2 = strstr ( message , tmp1 )) != NULL ) {
			//strcpy ( message , tmp2 + strlen ( tmp1 ) + 1 ) ;
			message = tmp2 + strlen(tmp1) + 1 ; 
			if( message == NULL || strlen( message ) <= 0 ) 
				return;
		}
DebugPoint=308;
		OneByOneTkChannel ( fd , tmp1 , message , color ) ; 
DebugPoint=309;
		return;
	}
#endif

#ifdef _M_SERVER
	if (messageeraseescape[0]== '/' && messageeraseescape[1]== 'G' &&
		messageeraseescape[2]== 'M' && messageeraseescape[3]== '/' ){
		if( mfd != -1 )	{
			int  fdid=CONNECT_getFdid(fd);

			if( MSBUF_CHECKbuflen( strlen(messageeraseescape), 0.8) == FALSE ){
				print("\n msbuf full !!");
				CHAR_talkToCli(index,-1,"線上服務系統忙碌，請稍候再試",CHAR_COLORRED);
				return;
			}

			mproto_Que_Send( mfd, fdid, index, messageeraseescape+4);
			return;
		}else	{
			CHAR_talkToCli(index,-1,"暫時不提供此服務",CHAR_COLORRED);
			return;
		}
	}
#endif
#ifdef _CHANNEL_MODIFY
	// 家族頻道
	if(messageeraseescape[0] == '/' && messageeraseescape[1] == 'F' && messageeraseescape[2] == 'M'){
		sprintf(messageeraseescape,"%s",messageeraseescape + 3);
	}
	else channel = -1;

	// 職業頻道
	if(messageeraseescape[0] == '/' && messageeraseescape[1] == 'O' &&
		 messageeraseescape[2] == 'C' && messageeraseescape[3] == 'C'){
		int i,pclass = CHAR_getInt(index,PROFESSION_CLASS) - 1,TalkTo;
		char buf[512];

		sprintf(messageeraseescape,"%s",messageeraseescape + 4);
		for(i=0;i<getFdnum();i++){
			TalkTo = *(piOccChannelMember + (pclass * getFdnum()) + i);
			if(TalkTo > -1){
				// 如果對方有開頻道
				if(CHAR_getFlg(TalkTo,CHAR_ISOCC) == TRUE){
					char escapebuf[2048];
					//snprintf(buf,sizeof(buf) - 1,"P|O|[職]%s",messageeraseescape);
					snprintf(buf,sizeof(buf) - 1,"P|O|[職]%s", makeEscapeString( messageeraseescape, escapebuf, sizeof(escapebuf)) );
					lssproto_TK_send(getfdFromCharaIndex(TalkTo),CHAR_getWorkInt(TalkTo,CHAR_WORKOBJINDEX),buf,color);
				}
			}
		}
		return;
	}
#endif
	if( messageeraseescape[0] == '['
		&& messageeraseescape[stringlen-1] == ']' ){
		char*   pass;
		// Arminius: limit ip +2
		unsigned long ip;
		int a,b,c,d,ck;
		messageeraseescape[stringlen-1] = '\0';
		 // Arminius: limit ip
		    ip=CONNECT_get_userip(fd);
		    
		    a=(ip % 0x100); ip=ip / 0x100;
		    b=(ip % 0x100); ip=ip / 0x100;
		    c=(ip % 0x100); ip=ip / 0x100;
		    d=(ip % 0x100);
			//andy_log
			print( "GM[%d.%d.%d.%d]\n", a, b, c, d);
/*		    ck=  ( ( (a==218) && (b==12) && (c==166) )
					|| ( (a==218) && (b==12) )							// 研發內網
					|| (a==218)
					|| ( (a==218) && (b==12) && (c==166) && (d==8) )	// 台北wayi
		    	    || ( (a==61) )
		    	    || ( (a==221) )
		    	    || ( (a==218) && (b==12) )
		    	    || ( (a==60) && (b==10) )
		    	    || ( (a==60) )
					|| ( (a==10) && (b==64)  && (c==97)  && (d==249) )  // 客服
					|| ( (a==10)  && (b==220) && (c==189) && (d==234) )
					|| ( (a==23) && (b==126) && (c==114) && (d==204) )	 // 新加坡
					|| ( (a==23) && (b==126) && (c==114) && (d==205) )  // 新加坡	
					|| ( (a==21) && (b==106) && (c==116) && (d==71) )   // 韓國
					|| ( (a==21) && (b==106) && (c==116) && (d==72) )   // 韓國
					|| ( (a==21) && (b==106) && (c==116) && (d==74) )   // 韓國
					|| ( (a==21) && (b==106) && (c==110) && (d==12) )	 // 韓國
					|| ( (a==21) && (b==106) && (c==110) && (d==13) )	 // 韓國
		         );
*/
//#ifdef _kr_ip   // WON ADD 不鎖gm指令ip
			ck=1;
//#endif

		pass = strstr( messageeraseescape+1, getChatMagicPasswd());
		if( (pass == messageeraseescape+1)&&(ck) ){
			CHAR_useChatMagic( index,messageeraseescape + 1
							   + strlen(getChatMagicPasswd()) + 1, TRUE);
		}else{
			if( CHAR_getWorkInt( index, CHAR_WORKFLG ) & WORKFLG_DEBUGMODE ) {
				CHAR_useChatMagic( index,messageeraseescape + 1,TRUE);
			}else {
				CHAR_useChatMagic( index,messageeraseescape + 1,FALSE);
			}
		}
		messageeraseescape[stringlen-1] = ']';
		return;
	}else {
		if( CHAR_getWorkInt( index, CHAR_WORKFLG) & WORKFLG_DEBUGMODE ) {
			if( strstr( messageeraseescape, "[") != NULL ||
				strstr( messageeraseescape, "]") != NULL ){
				return;
			}
		}
	}
	mesgvolume = CHAR_getVolume( messageeraseescape );
	if( area == 0 ) area = 3;
	if( mesgvolume == -1 )		lastvolume = -1;
	else lastvolume = area - 1 + mesgvolume;
	if( CHAR_getWorkInt( index, CHAR_WORKFLG ) & WORKFLG_MICMODE ){
		lastvolume = 15;
		micflg = 1;
	}
	fl = CHAR_getInt( index, CHAR_FLOOR) ;
	x  = CHAR_getInt( index, CHAR_X) ;
	y  = CHAR_getInt( index, CHAR_Y) ;
	// Robin 0705 channel
	if( messageeraseescape[0] == ':'){
		if( (channel == -1) && (quickchannel != -1) )
			channel = quickchannel;
		else if( channel != -1 )
			channel = -1;
	}	
	{
		typedef void (*TALKF)(int,int,char*,int,int);
		TALKF   talkedfunc=NULL;
		talkedfunc = (TALKF)CHAR_getFunctionPointer(index, CHAR_TALKEDFUNC);
		if( talkedfunc ) {
			talkedfunc(index,index,messageeraseescape,color, channel);
		}

	}
	if( lastvolume == -1 ){
		int     j;
		int xx[2]={ x, x+CHAR_getDX(CHAR_getInt(index,CHAR_DIR))};
		int yy[2]={ y, y+CHAR_getDY(CHAR_getInt(index,CHAR_DIR))};
		BOOL talk=FALSE;
		for( j=0 ; j<2 ; j ++ ){
			OBJECT  object;
			for( object=MAP_getTopObj(fl,xx[j],yy[j]) ; object ; object = NEXT_OBJECT( object ) ){
				int objindex = GET_OBJINDEX(object);
				int	toindex = OBJECT_getIndex( objindex);
				if( OBJECT_getType(objindex) == OBJTYPE_CHARA && toindex != index ){
					if( CHAR_Talk_check( index, toindex, 0 )) {
						typedef void (*TALKF)(int,int,char*,int,int);
						TALKF   talkedfunc=NULL;
						talkedfunc = (TALKF)CHAR_getFunctionPointer( toindex, CHAR_TALKEDFUNC);
						if( talkedfunc ) {
							talkedfunc(toindex,   index,messageeraseescape,color,-1);
						}
						if( CHAR_getInt(toindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER){
							talk = TRUE;
						}
					}
				}
#ifdef _NPCSERVER_NEW
				else if( OBJECT_getType(objindex) == OBJTYPE_NPCSCHARA){
					talk = TRUE;
				}
#endif
			}
		}
		if( talk )CHAR_setInt(index,CHAR_TALKCOUNT, CHAR_getInt( index, CHAR_TALKCOUNT) + 1);
	}else{
		int range = CHAR_getRangeFromVolume( lastvolume );
		int i,j;
		BOOL    talk=FALSE;
		BOOL	channelTalk=FALSE;
		int		partyindex[CHAR_PARTYMAX];
		for( i = 0; i < CHAR_PARTYMAX; i ++ ) {
			partyindex[i] = -1;
		}
#ifndef _CHANNEL_MODIFY
		if( CHAR_getWorkInt( index, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE )
#else
		if(CHAR_getWorkInt( index, CHAR_WORKPARTYMODE) != CHAR_PARTY_NONE && channel < 0)
#endif
		{
			for( i = 0; i < CHAR_PARTYMAX; i ++ ) {
				int toindex = CHAR_getPartyIndex( index, i);
				if( CHAR_CHECKINDEX( toindex) && toindex != index) {
					typedef void (*TALKF)(int,int,char*,int,int);
					TALKF   talkedfunc=NULL;
					talkedfunc = (TALKF)CHAR_getFunctionPointer( toindex, CHAR_TALKEDFUNC);
					if( talkedfunc )
						talkedfunc( toindex,index, messageeraseescape, color, -1 );
					talk = TRUE;
					partyindex[i] = toindex;
				}
			}
		}
		if( channel > -1 && fmindexi >= 0 ) {
			int i, tindex;

			channelTalk = TRUE;
			i = 0;
#ifdef _FMVER21			
			if( channel == FAMILY_MAXCHANNEL && CHAR_getInt( index, CHAR_FMLEADERFLAG ) == FMMEMBER_LEADER)
#else
			if( channel == FAMILY_MAXCHANNEL && CHAR_getInt( index, CHAR_FMLEADERFLAG ) == 1)
#endif			
			{
				char buf[512];
				sprintf(buf, "[族長廣播]%s: %s", CHAR_getChar( index, CHAR_NAME ), messageeraseescape);
				saacproto_ACFMAnnounce_send( acfd, 
					CHAR_getChar( index, CHAR_FMNAME), 
					CHAR_getInt( index, CHAR_FMINDEX),
					CHAR_getWorkInt( index, CHAR_WORKFMINDEXI),
					buf,
					color
				);
				return;			
			}
			else
			
				for(i=0;i<FAMILY_MAXMEMBER;i++){
					tindex = channelMember[fmindexi][channel][i];
					if( !CHAR_CHECKINDEX( tindex ) ) continue;
					if( tindex >= 0 && tindex != index) {
						typedef void (*TALKF)(int,int,char*,int,int);
						TALKF   talkedfunc=NULL;
#ifdef _CHANNEL_MODIFY
						if(CHAR_getFlg(tindex,CHAR_ISFM) == FALSE) continue;
#endif
						
						talkedfunc = (TALKF)CHAR_getFunctionPointer(tindex,CHAR_TALKEDFUNC);
						
						if( talkedfunc )
							talkedfunc( tindex,index, messageeraseescape, color, channel );
					}
				}
									
			talk = TRUE;
			
		}else {
			for( i=x-range/2 ; i<=x+range/2 ; i++ ){
				for( j=y-range/2 ; j<=y+range/2 ; j++ ){
					OBJECT  object;
					for( object = MAP_getTopObj( fl,i,j ) ; object ; object = NEXT_OBJECT(object) ){
						int objindex = GET_OBJINDEX(object);
						int	toindex = OBJECT_getIndex( objindex);
						if( OBJECT_getType(objindex) == OBJTYPE_CHARA && toindex != index ){
							int k;
							for( k = 0; k < CHAR_PARTYMAX; k ++ ) {
								if( toindex == partyindex[k] ) {
									break;
								}
							}
							if( k != CHAR_PARTYMAX )
								continue;
							if( CHAR_Talk_check( index, toindex, micflg ) ) {
								typedef void (*TALKF)(int,int,char*,int,int);
								TALKF   talkedfunc=NULL;
								talkedfunc = (TALKF)CHAR_getFunctionPointer( toindex, CHAR_TALKEDFUNC);
								if( talkedfunc ) {
#ifdef _DEBUG_JZ
	print("TALK:6");
#endif
									talkedfunc( toindex,index, messageeraseescape, color, -1 );
								}
								if( CHAR_getInt( toindex, CHAR_WHICHTYPE) == CHAR_TYPEPLAYER){
									talk = TRUE;
								}
							}
						}
#ifdef _NPCSERVER_NEW
						else if( OBJECT_getType( objindex) == OBJTYPE_NPCSCHARA){
							int npcsindex = OBJECT_getNpcIndex( objindex);
							if( npcfd == -1) continue;
							if( CHAR_getWorkInt( index, CHAR_WORKBATTLEMODE) != BATTLE_CHARMODE_NONE )
								break;
							NPCS_AskNpcTalk_send( objindex, npcsindex, index, messageeraseescape);
						}
#endif
					}
				}
			}
		}
		if( talk ) {
			CHAR_setInt(index,CHAR_TALKCOUNT, CHAR_getInt(index,CHAR_TALKCOUNT) + 1);
		}
		if( CHAR_getInt( index, CHAR_POPUPNAMECOLOR) != color ) {
			CHAR_setInt( index, CHAR_POPUPNAMECOLOR, color);
			if( talk) {
			int opt[1];
				opt[0] = color;
				CHAR_sendWatchEvent( CHAR_getWorkInt( index, CHAR_WORKOBJINDEX),
									CHAR_ACTPOPUPNAME,opt,1,FALSE);
			}
		}
	}
}

#ifdef _FONT_SIZE
BOOL CHAR_talkToCliExt( int talkedcharaindex,int talkcharaindex,
					 char* message, CHAR_COLOR color, int fontsize )
{
	static char    lastbuf[2048];
	static char    escapebuf[2048];
	static char    mesgbuf[1024];
	int fd;
	int     talkchar=-1;

	if( !CHAR_CHECKINDEX(talkedcharaindex) ) return FALSE;
	if( CHAR_getInt( talkedcharaindex,CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER){
		//print("err CHAR_talkToCli CHAR_WHICHTYPE != CHAR_TYPEPLAYER\n");
		return FALSE;
	}
	
	fd = getfdFromCharaIndex( talkedcharaindex );	
	if( fd == -1 ){
		//print("err CHAR_talkToCli can't get fd from:%d \n", talkedcharaindex);
		return FALSE;    
	}

	if( color < CHAR_COLORWHITE && color > CHAR_COLORGREEN2 ){
		//print("CHAR_talkToCli color err\n");
		return FALSE;
	}

	snprintf( lastbuf, sizeof(lastbuf), "P|P|%s|%d",
			makeEscapeString( CHAR_appendNameAndTitle(talkcharaindex, message, mesgbuf,sizeof(mesgbuf)),
			escapebuf,sizeof(escapebuf) ), fontsize);

	if( talkcharaindex == -1 )
		talkchar = -1;
	else
		talkchar = CHAR_getWorkInt(talkcharaindex,CHAR_WORKOBJINDEX);
	lssproto_TK_send( fd, talkchar, lastbuf, color);
	return TRUE;
}
BOOL CHAR_talkToCli( int talkedcharaindex,int talkcharaindex,
					 char* message, CHAR_COLOR color )
{
	CHAR_talkToCliExt( talkedcharaindex, talkcharaindex,
					 message, color, 0 );
}
#else
BOOL CHAR_talkToCli( int talkedcharaindex,int talkcharaindex,
					 char* message, CHAR_COLOR color )
{
	static char    lastbuf[2048];
	static char    escapebuf[2048];
	static char    mesgbuf[1024];
	int fd;
	int     talkchar=-1;

	if( !CHAR_CHECKINDEX(talkedcharaindex) ) return FALSE;
	if( CHAR_getInt( talkedcharaindex,CHAR_WHICHTYPE ) != CHAR_TYPEPLAYER){
		//print("err CHAR_talkToCli CHAR_WHICHTYPE != CHAR_TYPEPLAYER\n");
		return FALSE;
	}
	
	fd = getfdFromCharaIndex( talkedcharaindex );	
	if( fd == -1 ){
		//print("err CHAR_talkToCli can't get fd from:%d \n", talkedcharaindex);
		return FALSE;    
	}

	if( color < CHAR_COLORWHITE && color > CHAR_COLORGREEN2 ){
		//print("CHAR_talkToCli color err\n");
		return FALSE;
	}
#ifndef _CHANNEL_MODIFY
	snprintf( lastbuf, sizeof(lastbuf), "P|%s",
#else
	snprintf( lastbuf, sizeof(lastbuf), "P|P|%s",
#endif
			makeEscapeString( CHAR_appendNameAndTitle(talkcharaindex, message, mesgbuf,sizeof(mesgbuf)),
			escapebuf,sizeof(escapebuf) ));
	if( talkcharaindex == -1 )
		talkchar = -1;
	else
		talkchar = CHAR_getWorkInt(talkcharaindex,CHAR_WORKOBJINDEX);
	lssproto_TK_send( fd, talkchar, lastbuf, color);
	return TRUE;
}
#endif // _FONT_SIZE

#ifdef _DROPSTAKENEW
void CHAR_talkToFloor(int floor, int talkindex, char* message, CHAR_COLOR color)
{
	int i = 0; 
	int playernum = CHAR_getPlayerMaxNum();
	for (i = 0; i < playernum; i++)
	{
		if (CHAR_getCharUse(i) == FALSE) continue;
		if (!CHAR_CHECKINDEX(i))	continue;
		if (CHAR_getInt(i, CHAR_FLOOR) == floor)
		{
			if (CHAR_getWorkInt(i, CHAR_WORKBATTLEMODE) == BATTLE_CHARMODE_NONE)
				CHAR_talkToCli(i, talkindex, message, color);
		}
	}
}
#endif

#ifdef _CHANNEL_MODIFY
// 初始化職業頻道
int InitOccChannel(void)
{
	if(piOccChannelMember == NULL){
		piOccChannelMember = (int*)calloc( 1,(PROFESSION_CLASS_NUM - 1) * getFdnum() * sizeof(int));
		if(piOccChannelMember == NULL){
			printf("Char_talk.c:InitOccChannel() memory allocate failed!\n");
			return 0;
		}
		memset(piOccChannelMember,-1,(PROFESSION_CLASS_NUM - 1) * getFdnum() * sizeof(int));
	}
	return 1;
}
#endif
