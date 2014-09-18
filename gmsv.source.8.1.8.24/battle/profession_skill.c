#include "version.h"


#ifdef _PROFESSION_SKILL			// WON ADD �H��¾�~�ޯ�
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "buf.h"
#include "profession_skill.h"
#include "char_base.h"
#include "battle.h"
#include "magic_base.h"
#include "configfile.h"
#include "char.h"
#include "lssproto_serv.h"
#include "skill.h"

static Professionskill	*PROFESSION_skill;


typedef struct tagProfessionskill_skillFunctionTable
{
	char			*functionname;
	PROFESSION_SKILL_CALLFUNC	func;
	int				hash;
//	int				status;
}PROFESSION_SKILL_skillFunctionTable;

static PROFESSION_SKILL_skillFunctionTable PROFESSION_SKILL_functbl[] = {

// �Ův�ޯ�
	{ "PROFESSION_VOLCANO_SPRINGS", PROFESSION_volcano_springs, 0 },	// ���s�u
	{ "PROFESSION_FIRE_BALL",		PROFESSION_fire_ball,		0 },	// ���P�y
	{ "PROFESSION_FIRE_SPEAR",		PROFESSION_fire_spear,		0 },	// ���s�j
	{ "PROFESSION_SUMMON_THUNDER",  PROFESSION_summon_thunder,  0 },	// �l�p�N
	{ "PROFESSION_CURRENT",			PROFESSION_current,			0 },	// �q�y�N
	{ "PROFESSION_STORM",			PROFESSION_storm,			0 },	// �ɭ��B
	{ "PROFESSION_ICE_ARROW",		PROFESSION_ice_arrow,		0 },	// �B�b�N
	{ "PROFESSION_ICE_CRACK",		PROFESSION_ice_crack,		0 },	// �B�z�N
	{ "PROFESSION_ICE_MIRROR",		PROFESSION_ice_mirror,		0 },	// �B��N
	{ "PROFESSION_DOOM",			PROFESSION_doom,			0 },	// �@�ɥ���
	{ "PROFESSION_BLOOD",			PROFESSION_blood,			0 },	// �ݦ妨��
	{ "PROFESSION_BLOOD_WORMS",		PROFESSION_blood_worms,		0 },	// �ݦ���
	{ "PROFESSION_SIGN",			PROFESSION_sign,			0 },	// �@�w����
	{ "PROFESSION_FIRE_ENCLOSE",	PROFESSION_fire_enclose,	0 },	// ������
	{ "PROFESSION_ICE_ENCLOSE",		PROFESSION_ice_eclose,		0 },	// �B����
	{ "PROFESSION_THUNDER_ENCLOSE",	PROFESSION_thunder_enclose, 0 },	// �p����
	{ "PROFESSION_FIRE_PRACTICE",	PROFESSION_fire_practice,	0 },	// ���]�k���m��
	{ "PROFESSION_ICE_PRACTICE",	PROFESSION_ice_practice,	0 },	// �B�]�k���m��
	{ "PROFESSION_THUNDER_PRACTICE",PROFESSION_thunder_practice,0 },	// �p�]�k���m��
	{ "PROFESSION_ENCLOSE",			PROFESSION_enclose,			0 },	// �����N
	{ "PROFESSION_TRANSPOSE",		PROFESSION_transpose,		0 },	// ���δ���
//	{ "PROFESSION_ALCHEMY",			PROFESSION_alchemy,			0 },	// �Ҫ��N

// �i�h�ޯ�
	{ "PROFESSION_CHAIN_ATK",		PROFESSION_chain_atk,		0 },	// �s������
	{ "PROFESSION_AVOID",			PROFESSION_avoid,			0 },	// �j��
	{ "PROFESSION_WEAPON_FOCUS",	PROFESSION_weapon_focus,	0 },	// �Z���M��	
	{ "PROFESSION_REBACK",			PROFESSION_reback,			0 },	// ���A�^�_
	{ "PROFESSION_BRUST",			PROFESSION_brust,			0 },	// �z��
	{ "PROFESSION_CHAIN_ATK_2",		PROFESSION_chain_atk_2,		0 },	// ��������
	{ "PROFESSION_SCAPEGOAT",		PROFESSION_scapegoat,		0 },	// �ˤw����
	{ "PROFESSION_ENRAGE",			PROFESSION_enrage,			0 },	// �E�Ƨ���
	{ "PROFESSION_ENERGY_COLLECT",	PROFESSION_energy_collect,	0 },	// ��q�E��
	{ "PROFESSION_FOCUS",			PROFESSION_focus,			0 },	// �M�`�԰�
	{ "PROFESSION_SHIELD_ATTACK",	PROFESSION_shield_attack,	0 },	// ����
	{ "PROFESSION_DUAL_WEAPON",		PROFESSION_dual_weapon,		0 },	// �G�M�y	
	{ "PROFESSION_DEFLECT",			PROFESSION_deflect,			0 },	// ����
	{ "PROFESSION_THROUGH_ATTACK",	PROFESSION_through_attack,	0 },	// �e�����
	{ "PROFESSION_CAVALRY",			PROFESSION_cavalry,			0 },	// �y�M����
	{ "PROFESSION_DEAD_ATTACK",		PROFESSION_dead_attack,		0 },	// �x������
	{ "PROFESSION_CONVOLUTE",		PROFESSION_convolute,		0 },	// �j�ۧ���
	{ "PROFESSION_CHAOS",			PROFESSION_chaos,			0 },	// �V�ç���

// �y�H
	{ "PROFESSION_TRACK",			PROFESSION_track,			0 },	// �l�M����
	{ "PROFESSION_ESCAPE",			PROFESSION_escape,			0 },	// �j�׾԰�
	{ "PROFESSION_DOCILE",			PROFESSION_docile,			0 },	// �����d��
	{ "PROFESSION_TRAP",			PROFESSION_trap,			0 },	// ����
	{ "PROFESSION_ENRAGE_PET",		PROFESSION_enrage_pet,		0 },	// �E���d��
	{ "PROFESSION_DRAGNET",			PROFESSION_dragnet,			0 },	// ��ù�a��
	{ "PROFESSION_ENTWINE",			PROFESSION_entwine,			0 },	// �����¶
	{ "PROFESSION_AUTARKY",			PROFESSION_autarky,			0 },	// �۵��ۨ�
	{ "PROFESSION_PLUNDER",			PROFESSION_plunder,			0 },	//   �鱰��
	{ "PROFESSION_TOXIN_WEAPON",	PROFESSION_toxin_weapon,	0 },	// �r���Z��
	{ "PROFESSION_RESIST_FIRE",		PROFESSION_resist_fire,		0 },	// ���ܩ�
	{ "PROFESSION_RESIST_ICE",		PROFESSION_resist_ice,		0 },	// �B�ܩ�
	{ "PROFESSION_RESIST_THUNDER",	PROFESSION_resist_thunder,	0 },	// �p�ܩ�
#ifdef _PROFESSION_ADDSKILL
    { "PROFESSION_RESIST_F_I_T",    PROFESSION_resist_f_i_t,    0 },    // �۵M�¯�
	{ "PROFESSION_CALL_NATURE",     PROFESSION_call_nature,     0 },    // ���l�۵M
	{ "PROFESSION_BOUNDARY",        PROFESSION_boundary,     0 },       // �|�ݩʵ���
#endif
	{ "PROFESSION_G_RESIST_FIRE",	PROFESSION_g_resist_fire,	0 },	// ������ܩ�
	{ "PROFESSION_G_RESIST_ICE",	PROFESSION_g_resist_ice,	0 },	// ����B�ܩ�
	{ "PROFESSION_G_RESIST_THUNDER",PROFESSION_g_resist_thunder,0 },	// ����p�ܩ�
	{ "PROFESSION_ATTACK_WEAK",		PROFESSION_attack_weak,		0 },	// �z�I����
	{ "PROFESSION_INSTIGATE",		PROFESSION_instigate,		0 },	// �D��
	{ "PROFESSION_OBLIVION",		PROFESSION_oblivion,		0 },	// ���

// �@�P�ޯ�
	{ "PROFESSION_FULL_MP",			PROFESSION_full_mp,			0 },	// ��O�R�K
	{ "PROFESSION_STRONG_BACK",		PROFESSION_strong_back,		0 },	// �t���W�[
	{ "PROFESSION_STRENGTHEN",		PROFESSION_strengthen,		0 },	// �ۧڱj��

};


static int profession_skill_num;				// ¾�~�ޯ��`��

// �N profession.txt ���J
BOOL PROFESSION_initSkill( char *filename)
{

	FILE*   f;
    char    line[256];
    int     linenum=-1;
	int     skill_readlen=0;
	int		i, j;

	int max_skillid = 0;

    f = fopen(filename,"r");
    if( f == NULL ){
        print( "file open error\n");
        return FALSE;
    }

    profession_skill_num=0;

    while( fgets( line, sizeof( line ), f ) ){
		char    token[256];

        linenum ++;
        if( line[0] == '#' )continue;
        if( line[0] == '\n' )continue;
        chomp( line );
        profession_skill_num++;
#ifdef _PROSKILL_OPTIMUM // Robin ���X�̤j¾��ID
		if( getStringFromIndexWithDelim( line, ",", 
				PROFESSION_SKILL_DATACHARNUM+PROFESSION_SKILL_ID+1,
				token, sizeof(token)) == FALSE )
			continue;
		max_skillid = max( atoi( token), max_skillid);
#endif
    }

    if( fseek( f, 0, SEEK_SET ) == -1 ){
        fprint( "Seek Error\n" );
        fclose(f);
        return FALSE;
    }

#ifdef _PROSKILL_OPTIMUM // ¾��Table�Ƭ� �̤j�ޯ�ID +1
	profession_skill_num = max_skillid +1;
	print("\n �̤j¾��ID = %d\n", max_skillid);
#endif

	PROFESSION_skill = allocateMemory( sizeof(struct tagProfessionkill) * profession_skill_num + 1 );


	if( PROFESSION_skill == NULL ){
        fprint( "Can't allocate Memory %d\n", sizeof(struct tagProfessionkill)*profession_skill_num);
        fclose( f );
        return FALSE;
    }

	// init skill
    for( i = 0; i < profession_skill_num; i ++ ) {
    	for( j = 0; j < PROFESSION_SKILL_DATAINTNUM; j ++ )
    		PROFESSION_SKILL_setInt( i,j,-1);
    
    	for( j = 0; j < PROFESSION_SKILL_DATACHARNUM; j ++ ) 
    		PROFESSION_SKILL_setChar( i,j,"");    
    }

    linenum = -1;
    while( fgets( line, sizeof( line ), f ) ){
        linenum ++;
        if( line[0] == '#' )continue;
        if( line[0] == '\n' )continue;             
                
        chomp( line );

        replaceString( line, '\t' , ' ' );

		{
		    char    buf[256];
			for( i = 0; i < strlen( line); i ++) {
				if( line[i] != ' ' ) {
					break;
				}
				strcpy( buf, &line[i]);
			}
			
			if( i != 0 ) {
				strcpy( line, buf);
			}
		}  
		{
			char    token[256];
			int     ret;

#ifdef _PROSKILL_OPTIMUM // Ū������¾�ު�ID, �����H¾��ID��Table index
			ret = getStringFromIndexWithDelim( line, ",", PROFESSION_SKILL_DATACHARNUM+PROFESSION_SKILL_ID+1, token, sizeof(token));
			if( ret==FALSE ){
				fprint("Syntax Error file:%s line:%d\n",filename,linenum);
				break;
			}
			skill_readlen = atoi( token);
#endif
			for( i = 0; i < PROFESSION_SKILL_DATACHARNUM; i ++ ) {

			    ret = getStringFromIndexWithDelim( line,",", i + 1, token,sizeof(token));
				if( ret==FALSE ){
					fprint("Syntax Error file:%s line:%d\n",filename,linenum);
					break;
				} 
				PROFESSION_SKILL_setChar( skill_readlen, i, token);
			} 

			for( i = 0; i < PROFESSION_SKILL_DATAINTNUM; i ++ ) {
				ret = getStringFromIndexWithDelim( line, ",", i+PROFESSION_SKILL_DATACHARNUM+1, token, sizeof(token));
				
				if( ret==FALSE ){
					fprint("Syntax Error file:%s line:%d\n",filename,linenum);
					break;
				} 
				if( strlen( token) != 0 ) {
					PROFESSION_SKILL_setInt( skill_readlen, i , atoi( token ));
				} 
			}         		
			skill_readlen ++;
		} 		
    } 
    fclose(f);

    profession_skill_num = skill_readlen;

	for( i = 0; i < arraysizeof( PROFESSION_SKILL_functbl); i ++ ){
		PROFESSION_SKILL_functbl[i].hash = hashpjw( PROFESSION_SKILL_functbl[i].functionname );
	}

    return TRUE;

}


// ���s���J profession.txt
void rePROFESSION_initSkill()
{
	freeMemory( PROFESSION_skill );
	PROFESSION_initSkill(getProfession());
}

INLINE BOOL PROFESSION_SKILL_CHECKINDEX( int index )
{
    if( profession_skill_num < index || index < 0 )return FALSE;
    return TRUE;
}

static INLINE BOOL PROFESSION_SKILL_CHECKINTDATAINDEX( int index)
{
	if( PROFESSION_SKILL_DATAINTNUM < index || index < 0 ) return FALSE;
	return TRUE;
}

static INLINE BOOL PROFESSION_SKILL_CHECKCHARDATAINDEX( int index)
{
	if( PROFESSION_SKILL_DATACHARNUM < index || index < 0 ) return FALSE;
	return TRUE;
}

INLINE int PROFESSION_SKILL_getInt( int index, PROFESSION_SKILL_DATAINT element)
{
	return PROFESSION_skill[index].data[element];
}

INLINE int PROFESSION_SKILL_setInt( int index, PROFESSION_SKILL_DATAINT element, int data)
{
	int buf;
	buf = PROFESSION_skill[index].data[element];
	PROFESSION_skill[index].data[element]=data;
	return buf;
}

INLINE char* PROFESSION_SKILL_getChar( int index, PROFESSION_SKILL_DATACHAR element)
{
	if( !PROFESSION_SKILL_CHECKINDEX( index)) return NULL;
	if( !PROFESSION_SKILL_CHECKCHARDATAINDEX( element)) return NULL;
	return PROFESSION_skill[index].string[element].string;
}

INLINE BOOL PROFESSION_SKILL_setChar( int index ,PROFESSION_SKILL_DATACHAR element, char* new )
{
    if(!PROFESSION_SKILL_CHECKINDEX(index))return FALSE;
    if(!PROFESSION_SKILL_CHECKCHARDATAINDEX(element))return FALSE;

    strcpysafe( PROFESSION_skill[index].string[element].string,
                sizeof(PROFESSION_skill[index].string[element].string),
                new );
    return TRUE;
}

int PROFESSION_SKILL_getskillNum( void)
{
	return profession_skill_num;
}


/*----------------------------------------------------------------------*/

int PROFESSION_SKILL_getskillArray( int skillid)
{
#ifdef _PROSKILL_OPTIMUM // �d��¾��ID�אּ�}�C��
	if( skillid >= 0 && skillid < profession_skill_num )
		return PROFESSION_skill[skillid].data[PROFESSION_SKILL_ID];
#else
	int		i;
	for( i = 0; i <= profession_skill_num; i ++ ) { // ���u��
		if( PROFESSION_skill[i].data[PROFESSION_SKILL_ID] == skillid ) 
			return i;
	}
#endif
	return -1;
}

PROFESSION_SKILL_CALLFUNC PROFESSION_SKILL_getskillFuncPointer(char* name )
{
    int i;
    int hash = hashpjw( name );
	
	for( i = 0; i< arraysizeof( PROFESSION_SKILL_functbl); i++ ){
		if( PROFESSION_SKILL_functbl[i].hash == hash ){
        	if( strcmp( PROFESSION_SKILL_functbl[i].functionname, name ) == 0 ){
	            return PROFESSION_SKILL_functbl[i].func;
			}
		}
	}

    return NULL;
}

int _PROFESSION_SKILL_GetArray( char *file, int line, int charaindex, int skill )
{
	int		skillid,array;

	skillid = CHAR_getCharSkill( charaindex, skill);

	if( skillid < 0 )	return -1;

	array = PROFESSION_SKILL_getskillArray( skillid );
		
	return array;
}


/*----------------------------------------------------------------------*/

// �ϥ�¾�~�ޯ�
int PROFESSION_SKILL_Use( int charaindex, int skill, int toNo, char *data )
{
	int		Pskillid, skillid;
	int		ret=-1, skill_level;
	PROFESSION_SKILL_CALLFUNC func;
	CHAR_HaveSkill* hskill;
	int char_pskill, profession_skill;

	skillid = CHAR_getCharSkill( charaindex, skill);
	Pskillid = PROFESSION_SKILL_getskillArray( skillid);
	//print("\n skillid:%d Pskillid:%d ", skillid, Pskillid); // Robin log
	if( Pskillid == -1 ) {
		printf("\n ��ʥ]??�䤣���¾��! ");
		return ret;
	}

#ifdef _PROSKILL_OPTIMUM	// Robin fix �ˬd¾�~�O�_�۲�
	char_pskill = CHAR_getInt( charaindex, PROFESSION_CLASS );
	profession_skill = PROFESSION_SKILL_getInt( Pskillid, PROFESSION_SKILL_PROFESSION_CLASS);
	if( (char_pskill <=0) || (char_pskill != profession_skill) ) {
		print("\n ��ʥ]??¾�ު�¾�~�����T!:%d:%d ", char_pskill, profession_skill);
		return ret;
	}
#endif

	// ��M���� function
	func = PROFESSION_SKILL_getskillFuncPointer( PROFESSION_SKILL_getChar( Pskillid, PROFESSION_SKILL_FUNCNAME) );

	// �P�_�O�_���H��
	if( CHAR_getInt( charaindex, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER )	return ret;


	if( func ){
		// �H���ޯ൥��
		hskill = CHAR_getCharHaveSkill( charaindex, skill );
		skill_level = SKILL_getInt( &hskill->skill, SKILL_LEVEL);
	
		// �ӷl MP 
		if( PROFESSION_SKILL_DEC_COST_MP( charaindex, skill, Pskillid, skill_level ) != 1 )	{
			print("\n Error:¾��MP:%d:%d:%d ", skill, Pskillid, skill_level);
			return ret;
		}

		// �ϥ�¾�~�ޯ�
		ret = func( charaindex, toNo, Pskillid, data, skill_level );
		if( ret == -1 ){
			int rands = rand()%10;
			//andy_log
			print("ret = %d, rands:%d \n", ret, rands);
			if( rands > 5 ) return ret;
		}

		if( Pskillid == 57 //�E���d�� ���d���~���g���
			&& CHAR_getInt( BATTLE_No2Index( CHAR_getWorkInt( charaindex, CHAR_WORKBATTLEINDEX ), toNo ), CHAR_WHICHTYPE) != CHAR_TYPEPET ) 
			return ret;

		// �@��ޯ�ɯ�
		PROFESSION_NORMAL_SKILL_LEVLE_UP( &hskill->skill, Pskillid, charaindex );
	}
	else {
		print("\n ���~!�䤣��¾��func ");
	}

	return ret;
}

// �S��ޯ�ɯ�
void PROFESSION_SKILL_LVEVEL_UP( int charaindex, char *name )
{
	int skill=-1;
	int skillid=0, Pskillid=0, i;
	char *skill_name;
	CHAR_HaveSkill* hskill;

	for( i=0; i<CHAR_SKILLMAXHAVE; i++ ){
			
		// �ޯ�ID
		skillid = CHAR_getCharSkill( charaindex, i);
		Pskillid = PROFESSION_SKILL_getskillArray( skillid);		
		if( Pskillid < 0 ) continue;

		// �ޯ�W��
		skill_name = PROFESSION_SKILL_getChar( Pskillid, PROFESSION_SKILL_FUNCNAME);
		if( skill_name == NULL ) continue;
			
		if( (strcmp( skill_name , name )) == 0 ){
			skill = i;		break;
		}
	}	

	if( (skill < 0) || (skill > CHAR_SKILLMAXHAVE) ) return;

	skillid = CHAR_getCharSkill( charaindex, skill);
	Pskillid = PROFESSION_SKILL_getskillArray( skillid);
	if( Pskillid == -1 )	return;

	// �H���ޯ൥��
	hskill = CHAR_getCharHaveSkill( charaindex, skill );
	
	// �@��ޯ�ɯ�
	PROFESSION_NORMAL_SKILL_LEVLE_UP( &hskill->skill, Pskillid, charaindex );

	return;
}

// �G�M�y�ޯ�ɯ�
void PROFESSION_SKILL_DUAL_WEAPON_LVEVEL_UP( int charaindex, char *name )
{
	int skill=-1, arm = 0, shield = 0;
	int skillid=0, Pskillid=0, i;
	char *skill_name;
	CHAR_HaveSkill* hskill;

	// �ˬd���k��˳�
	for( i = 0 ; i < CHAR_EQUIPPLACENUM ; i ++ ){
		int id = CHAR_getItemIndex(charaindex,i);
		if( ITEM_CHECKINDEX(id) ){
			if( i == CHAR_ARM ) arm = 1;
			else if( i == CHAR_EQSHIELD ) shield = 1;			
		}
	}

	if( arm != 1 || shield != 1 )	return;	

	for( i=0; i<CHAR_SKILLMAXHAVE; i++ ){
		
		// �ޯ�ID
		skillid = CHAR_getCharSkill( charaindex, i);
		Pskillid = PROFESSION_SKILL_getskillArray( skillid);		
		if( Pskillid < 0 ) continue;

		// �ޯ�W��
		skill_name = PROFESSION_SKILL_getChar( Pskillid, PROFESSION_SKILL_FUNCNAME);
		if( skill_name == NULL ) continue;
			
		if( (strcmp( skill_name , name )) == 0 ){
			skill = i;		break;
		}
	}

	if( (skill < 0) || (skill > CHAR_SKILLMAXHAVE) ) return;

	skillid = CHAR_getCharSkill( charaindex, skill);
	Pskillid = PROFESSION_SKILL_getskillArray( skillid);
	if( Pskillid == -1 )	return;

	// �H���ޯ൥��
	hskill = CHAR_getCharHaveSkill( charaindex, skill );
	
	// �@��ޯ�ɯ�
	PROFESSION_NORMAL_SKILL_LEVLE_UP( &hskill->skill, Pskillid, charaindex );

	return;
}


// �Z���M��ޯ�ɯ�
void PROFESSION_SKILL_WEAPON_FOCUS_LVEVEL_UP( int charaindex, char *name )
{
	int skill = -1;
	int skillid=0, Pskillid = 0, i;
	char *skill_name, *pszOption, *pszP, diff[20];
	int WeaponType = -1;
	CHAR_HaveSkill* hskill;

	memset( diff, -1, sizeof(diff) );
	// �˳ƪZ��
	WeaponType = BATTLE_GetWepon( charaindex );

	switch( WeaponType ){
		case ITEM_AXE:		
			sprintf( diff, "��" ); break;
		case ITEM_CLUB:		
			sprintf( diff, "��" ); break;
		case ITEM_SPEAR:	
			sprintf( diff, "�j" ); break;
		case ITEM_BOW:		
			sprintf( diff, "�}" ); break;
		case ITEM_BOOMERANG:	// �j�ۼ�	
			sprintf( diff, "��" ); break;
		case ITEM_BOUNDTHROW:	// ���Y���Y
			sprintf( diff, "��" ); break;
		case ITEM_BREAKTHROW:	// ���Y��
			sprintf( diff, "��" ); break;
		default: 
			sprintf( diff, "�L" ); break;
	}

	for( i=0; i<CHAR_SKILLMAXHAVE; i++ ){
		// �ޯ�ID
		skillid = CHAR_getCharSkill( charaindex, i);
		Pskillid = PROFESSION_SKILL_getskillArray( skillid);		
		if( Pskillid < 0 ) continue;

		// �ޯ�W��
		skill_name = PROFESSION_SKILL_getChar( Pskillid, PROFESSION_SKILL_FUNCNAME);
		if( skill_name == NULL ) continue;
	
		// �ޯ�Ѽ�
		pszOption = PROFESSION_SKILL_getChar( Pskillid, PROFESSION_SKILL_OPTION);
		if( pszOption == NULL ) continue;
		
		if( (strcmp( skill_name , name )) == 0 ){
			if( (pszP=strstr( pszOption, diff ))!=NULL ){
				skill = i;		break;
			}
		}
	}	

	if( (skill < 0) || (skill > CHAR_SKILLMAXHAVE) ) return;

	skillid = CHAR_getCharSkill( charaindex, skill);
	Pskillid = PROFESSION_SKILL_getskillArray( skillid);
	if( Pskillid == -1 )	return;

	// �H���ޯ൥��
	hskill = CHAR_getCharHaveSkill( charaindex, skill );
	
	// �@��ޯ�ɯ�
	PROFESSION_NORMAL_SKILL_LEVLE_UP( &hskill->skill, Pskillid, charaindex );

	return;
}

// �ӷl MP 
int PROFESSION_SKILL_DEC_COST_MP( int charaindex, int skill, int Pskillid, int skill_level )
{	
	
	if(skill_level > 0){
		int	dec_mp;
		int old_mp = CHAR_getInt( charaindex, CHAR_MP );
		int mp=0;
			
		// ���X�ӷl MP 
		if( (dec_mp = PROFESSION_MAGIC_COST_MP( charaindex, skill )) == -1 )
			dec_mp = PROFESSION_SKILL_getInt( Pskillid, PROFESSION_SKILL_COST_MP );

		// �ˬd MP�q			
		if( old_mp < dec_mp ) return -1;
		
		// �ݦ妨�� 
		if( Pskillid != 11 
#ifdef _PROFESSION_ADDSKILL
		//	&& Pskillid != 2 //�w�w�۹藍�� MP
#endif
			)
			if( dec_mp <= 0 ) return -1;
		
		// �����H�� MP
		mp = old_mp - dec_mp;
		if( mp < 0 ) mp = 0;
		CHAR_setInt( charaindex , CHAR_MP , mp );
		CHAR_send_P_StatusString( charaindex , CHAR_P_STRING_MP);
	
		return 1;
	}
	
	return -1;
}



// �@��ޯ�ɯ�
void PROFESSION_NORMAL_SKILL_LEVLE_UP(Skill *skill, int Pskillid, int charaindex )
{
	int skill_level = 0, up_fix_value = 0;
	int rand_num = RAND( 0, 10000 ), rand_num2 = 0;
	
	// �ޯ൥�Ũ�F�W�����ɯ�
	skill_level = SKILL_getRealInt( skill, SKILL_LEVEL);
	if( skill_level >= PROFESSION_SKILL_MAX_LEVEL * 100 ) return;

	// �ɯŭץ��ƭ�
	up_fix_value = PROFESSION_SKILL_getInt( Pskillid, PROFESSION_SKILL_FIX_VALUE) * 100;
	rand_num2 = RAND( 0, up_fix_value);

	// �ɯ��˩w
#ifdef _75_TEST
#else
	if( rand_num > skill_level + rand_num2){
#endif	
		// �W�[���m��
		skill_level += PROFESSION_SKILL_ADD_POINT;	
#ifdef _75_TEST
		if( skill_level > 10000 )
			skill_level = 10000;
#endif
		SKILL_setInt( skill, SKILL_LEVEL, skill_level );	

		if( ( skill_level % 100 ) == 0 ){
			char msg[256];
			sprintf( msg, "%s�ޯ���m�פW�ɬ�%d", 	PROFESSION_SKILL_getChar( Pskillid, PROFESSION_SKILL_NAME), skill_level/100 );
			CHAR_talkToCli( charaindex, -1, msg, CHAR_COLORYELLOW);
			CHAR_sendStatusString( charaindex , "S");	
			
		
			// ¾�~�g��ȼW�[		
			PROFESSION_LEVEL_CHECK_UP( charaindex );
		}
#ifdef _75_TEST
#else
	}
#endif
	return;
}


void PROFESSION_LEVEL_CHECK_UP( int charaindex )
{
	int skill_level_add = 0, skill_level_sum = 0, i = 0;
	int old_level = CHAR_getInt( charaindex, PROFESSION_LEVEL );
	int next_level_need_point = 9999999;
	CHAR_HaveSkill* hskill;

	// �U�@��¾�~�ɯ��I��
	next_level_need_point = old_level * 70 * 100;

	for( i=0; i<CHAR_SKILLMAXHAVE; i++ ){
		int skillID = -1;

		// �ޯ൥��
		hskill = CHAR_getCharHaveSkill( charaindex, i );
		skillID = SKILL_getInt( &hskill->skill, SKILL_IDENTITY);
		if( skillID <= 0 ) continue;

		// �@�q�ޯ���m�� 50
		if( (skillID == 63) || (skillID == 64) || (skillID == 65) ){
			skill_level_add = 50*100;	
		}else{
			skill_level_add = SKILL_getRealInt( &hskill->skill, SKILL_LEVEL);
		}
		
		skill_level_sum += skill_level_add;	
	}
	
	//print("\n LEVEL_CHECK_UP ==> skill_level_sum(%d) next_level_need_point(%d)", skill_level_sum, next_level_need_point );

	// �ޯ���m�׹F��U�@��¾�~�ɯ��I�ƮɡA¾�~���żW�[
	if(  skill_level_sum >= next_level_need_point ){
		int old_skill_point = CHAR_getInt( charaindex, PROFESSION_SKILL_POINT );
		int next_skill_point = old_skill_point+1;
		char msg[64];

		//print("\n LEVEL_CHECK_UP ==> old_skill_point(%d) next_skill_point(%d)", old_skill_point, next_skill_point );
		
		CHAR_setInt( charaindex, PROFESSION_LEVEL, old_level + 1 );
		CHAR_setInt( charaindex, PROFESSION_SKILL_POINT, next_skill_point );
		CHAR_sendStatusString( charaindex , "P");
		
		CHAR_sendCToArroundCharacter( CHAR_getWorkInt( charaindex , CHAR_WORKOBJINDEX ));

		sprintf( msg, "¾�~���ŤW�ɬ�%d�šA�ޯ��I�ƤW�ɢ��I", old_level + 1 );
		CHAR_talkToCli( charaindex, -1, msg, CHAR_COLORYELLOW);
	}
}

// �W�[�ޯ�
int PROFESSION_SKILL_ADDSK( int charaindex, int skill, int level )
{
	int i;

	CHAR_HaveSkill *pSkil;

	if( level > 100 )		level = 100;
	else if( level < 1 )	level = 1;

	for( i = 0; i < CHAR_SKILLMAXHAVE; i ++ ){
		int skill_id = -1;

		pSkil = CHAR_getCharHaveSkill( charaindex, i );
		skill_id = SKILL_getInt( &pSkil->skill, SKILL_IDENTITY);
		if( skill == skill_id && skill_id != -1 ){
			CHAR_talkToCli(charaindex,-1, "�A�w�g�ǲ߹L���ޯ�", CHAR_COLORYELLOW);
			return -1;
		}

		if( pSkil == NULL )continue;
		if( pSkil->use != 0 )continue;
		break;
	}
	if( i >= CHAR_SKILLMAXHAVE ){
		CHAR_talkToCli(charaindex,-1, "�ޯ�ƶq�w�F�W��", CHAR_COLORYELLOW);
		return -1;
	}else{
		level *= 100;
		SKILL_makeSkillData( &pSkil->skill, skill, level);
		pSkil->use = TRUE;
	}

	CHAR_sendStatusString( charaindex , "S");
	

	return 1;
}

void profession_common_fun( int charaindex, int toNo, int skill_level, int array, int com1 )
{
	CHAR_setWorkInt( charaindex, CHAR_WORKBATTLECOM1, com1 );
	CHAR_setWorkInt( charaindex, CHAR_WORKBATTLECOM2, toNo );
	CHAR_setWorkInt( charaindex, CHAR_WORKBATTLEMODE, BATTLE_CHARMODE_C_OK );
	CHAR_SETWORKINT_HIGH( charaindex, CHAR_WORKBATTLECOM3, skill_level);
	CHAR_SETWORKINT_LOW( charaindex, CHAR_WORKBATTLECOM3, array);
#ifdef _PROFESSION_ADDSKILL
	if( com1 == BATTLE_COM_S_DOOM 
		/*&& CHAR_getWorkInt( charaindex, CHAR_DOOMTIME) == 0*/ ){
		CHAR_setWorkInt( charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_NONE );
		CHAR_setWorkInt( charaindex, CHAR_WORK_com1, com1 );
		CHAR_setWorkInt( charaindex, CHAR_WORK_toNo, toNo );
		CHAR_setWorkInt( charaindex, CHAR_WORK_mode, BATTLE_CHARMODE_C_OK );
		CHAR_setWorkInt( charaindex, CHAR_WORK_skill_level, skill_level );
		CHAR_setWorkInt( charaindex, CHAR_WORK_array, array );
		CHAR_setWorkInt( charaindex, CHAR_DOOMTIME, 3);
	}
	else if( com1 == BATTLE_COM_S_FIRE_SPEAR 
			/*&& CHAR_getWorkInt( charaindex, CHAR_DOOMTIME) == 0*/ ){
		CHAR_setWorkInt( charaindex, CHAR_WORKBATTLECOM1, BATTLE_COM_NONE );
		CHAR_setWorkInt( charaindex, CHAR_WORK_com1, com1 );
		CHAR_setWorkInt( charaindex, CHAR_WORK_toNo, toNo );
		CHAR_setWorkInt( charaindex, CHAR_WORK_mode, BATTLE_CHARMODE_C_OK );
		CHAR_setWorkInt( charaindex, CHAR_WORK_skill_level, skill_level );
		CHAR_setWorkInt( charaindex, CHAR_WORK_array, array );
		CHAR_setWorkInt( charaindex, CHAR_DOOMTIME, 2);
	}
#endif
}

/*----------------------------------------------------------------------*/
// �H�U��¾�~�ޯ�

// �Ův�ޯ�-���s�u
int PROFESSION_volcano_springs( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_VOLCANO_SPRINGS );		
	return TRUE;
}

// �Ův�ޯ�-���P�y
int PROFESSION_fire_ball( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_FIRE_BALL );		
	return TRUE;
}

// �Ův�ޯ�-���s�j
int PROFESSION_fire_spear( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_FIRE_SPEAR );		
	return TRUE;
}

// �Ův�ޯ�-�l�p�N
int PROFESSION_summon_thunder( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_SUMMON_THUNDER );		
	return TRUE;
}

// �Ův�ޯ�-�q�y�N
int PROFESSION_current( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_CURRENT );		
	return TRUE;
}

// �Ův�ޯ�-�ɭ��B
int PROFESSION_storm( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_STORM );		
	return TRUE;
}

// �Ův�ޯ�-�B�b�N
int PROFESSION_ice_arrow( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_ICE_ARROW );		
	return TRUE;
}

// �Ův�ޯ�-�B�z�N
int PROFESSION_ice_crack( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_ICE_CRACK );		
	return TRUE;
}

// �Ův�ޯ�-�B��N
int PROFESSION_ice_mirror( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_ICE_MIRROR );		
	return TRUE;
}

// �Ův�ޯ�-�@�ɥ���
int PROFESSION_doom( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_DOOM );		
	return TRUE;
}

// �Ův�ޯ�-�ݦ妨��
int PROFESSION_blood( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_BLOOD );		
	return TRUE;
}

// �Ův�ޯ�-�ݦ���
int PROFESSION_blood_worms( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_BLOOD_WORMS );		
	return TRUE;
}

// �Ův�ޯ�-�@�w����
int PROFESSION_sign( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_SIGN );		
	return TRUE;
}

// �Ův�ޯ�-������
int PROFESSION_fire_enclose( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_FIRE_ENCLOSE );		
	return TRUE;
}

// �Ův�ޯ�-�B����
int PROFESSION_ice_eclose( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_ICE_ENCLOSE );		
	return TRUE;
}

// �Ův�ޯ�-�p����
int PROFESSION_thunder_enclose( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_THUNDER_ENCLOSE );		
	return TRUE;
}

// �Ův�ޯ�-���]�k���m��
int PROFESSION_fire_practice( int charaindex, int toNo, int array, char *data, int skill_level )
{
	return TRUE;
}

// �Ův�ޯ�-�B�]�k���m��
int PROFESSION_ice_practice( int charaindex, int toNo, int array, char *data, int skill_level )
{
	return TRUE;
}

// �Ův�ޯ�-�p�]�k���m��
int PROFESSION_thunder_practice( int charaindex, int toNo, int array, char *data, int skill_level )
{
	return TRUE;
}

// �Ův�ޯ�-�����N
int PROFESSION_enclose( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_ENCLOSE );		
	return TRUE;
}

// �Ův�ޯ�-���δ���
int PROFESSION_transpose( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_TRANSPOSE );		
	return TRUE;
}


/*----------------------------------------------------------------------*/

// �i�h�ޯ�-�z��
int PROFESSION_brust( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_BRUST );
	return TRUE;
}

// �i�h�ޯ�-�s������
int PROFESSION_chain_atk( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_CHAIN_ATK );
	return TRUE;
}

// �i�h�ޯ�-�j��
int PROFESSION_avoid( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_AVOID );
	return TRUE;
}

// �i�h�ޯ�-�ɦ�
int PROFESSION_recovery( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_RECOVERY );
	return TRUE;
}

// �i�h�ޯ�-�Z���M��
int PROFESSION_weapon_focus( int charaindex, int toNo, int array, char *data, int skill_level )
{
	return TRUE;
}

// �i�h�ޯ�-���A�^�_
int PROFESSION_reback( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_REBACK );		
	return TRUE;
}

// �i�h�ޯ�-��������
int PROFESSION_chain_atk_2( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_CHAIN_ATK_2 );		
	return TRUE;
}

// �i�h�ޯ�-�ˤw����
int PROFESSION_scapegoat( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_SCAPEGOAT );		
	return TRUE;
}

// �i�h�ޯ�-�E�Ƨ���
int PROFESSION_enrage( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_ENRAGE );		
	return TRUE;
}

// �i�h�ޯ�-��q�E��
int PROFESSION_energy_collect( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_COLLECT );		
	return TRUE;
}

// �i�h�ޯ�-�M�`�԰�
int PROFESSION_focus( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_FOCUS );		
	return TRUE;
}

// �i�h�ޯ�-����
int PROFESSION_shield_attack( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_SHIELD_ATTACK );		
	return TRUE;
}

// �i�h�ޯ�-�G�M�y
int PROFESSION_dual_weapon( int charaindex, int toNo, int array, char *data, int skill_level )
{
	return TRUE;
}

// �i�h�ޯ�-����
int PROFESSION_deflect( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_DEFLECT );		
	return TRUE;
}

// �i�h�ޯ�-�e�����
int PROFESSION_through_attack( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_THROUGH_ATTACK );		
	return TRUE;
}

// �i�h�ޯ�-�y�M����
int PROFESSION_cavalry( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_CAVALRY );		
	return TRUE;
}

// �i�h�ޯ�-�x������
int PROFESSION_dead_attack( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_DEAD_ATTACK );		
	return TRUE;
}

// �i�h�ޯ�-�j�ۧ���
int PROFESSION_convolute( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_CONVOLUTE );		
	return TRUE;
}

// �i�h�ޯ�-�V�ç���
int PROFESSION_chaos( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_CHAOS );		
	return TRUE;
}

/*----------------------------------------------------------------------*/

// �y�H�ޯ�-����
int PROFESSION_trap( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_TRAP );		
	return TRUE;
}

// �y�H�ޯ�-�����d��
int PROFESSION_docile( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_DOCILE );		
	return TRUE;
}

// �y�H�ޯ�-�E���d��
int PROFESSION_enrage_pet( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_ENRAGE_PET );		
	return TRUE;
}

// �y�H�ޯ�-��ù�a��
int PROFESSION_dragnet( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_DRAGNET );		
	return TRUE;
}

// �y�H�ޯ�-�����¶
int PROFESSION_entwine( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_ENTWINE );		
	return TRUE;
}

// �y�H�ޯ�-�۵��ۨ�
int PROFESSION_autarky( int charaindex, int toNo, int array, char *data, int skill_level )
{
	return TRUE;
}

// �y�H�ޯ�-  �鱰��
int PROFESSION_plunder( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_PLUNDER );	
	return TRUE;
}

// �y�H�ޯ�-�r���Z��
int PROFESSION_toxin_weapon( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_TOXIN_WEAPON );	
	return TRUE;
}

// �y�H�ޯ�-���ܩʴ���
int PROFESSION_resist_fire( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_RESIST_FIRE );	
	return TRUE;
}

// �y�H�ޯ�-�B�ܩʴ���
int PROFESSION_resist_ice( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_RESIST_ICE );	
	return TRUE;
}

// �y�H�ޯ�-�p�ܩʴ���
int PROFESSION_resist_thunder( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_RESIST_THUNDER );	
	return TRUE;
}

#ifdef _PROFESSION_ADDSKILL
// �y�H�ޯ�-�۵M�¯�
int PROFESSION_resist_f_i_t( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_RESIST_F_I_T );	
	return TRUE;
}
// �y�H�ޯ�-���l�۵M
int PROFESSION_call_nature( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_CALL_NATURE );	
	return TRUE;
}
// �y�H�ޯ�-�|�ݩʵ���
int PROFESSION_boundary( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_BOUNDARY );	
	return TRUE;
}
#endif

// �y�H�ޯ�-������ܩʴ���
int PROFESSION_g_resist_fire( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_G_RESIST_FIRE );	
	return TRUE;
}

// �y�H�ޯ�-����B�ܩʴ���
int PROFESSION_g_resist_ice( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_G_RESIST_ICE );	
	return TRUE;
}

// �y�H�ޯ�-����p�ܩʴ���
int PROFESSION_g_resist_thunder( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_G_RESIST_THUNDER );	
	return TRUE;
}

// �y�H�ޯ�-�z�I����
int PROFESSION_attack_weak( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_ATTACK_WEAK );	
	return TRUE;
}

// �y�H�ޯ�-�D��
int PROFESSION_instigate( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_INSTIGATE );	
	return TRUE;
}

// �y�H�ޯ�-���
int PROFESSION_oblivion( int charaindex, int toNo, int array, char *data, int skill_level )
{
	profession_common_fun( charaindex, toNo, skill_level, array, BATTLE_COM_S_OBLIVION );	
	return TRUE;
}


/*----------------------------------------------------------------------*/
// �D�԰��ޯ�

// �y�H�ޯ�-�l�M����
int PROFESSION_track( int charaindex, int toNo, int array, char *data, int skill_level )
{
	char *pszP = NULL, *pszOption = NULL, msg[20];
	int per=0, rate=0, img1, ret = 1;
	skill_level /= 10;

	if( CHAR_getWorkInt( charaindex, CHAR_ENCOUNT_NUM) >= (int)time(NULL) ){
		ret = -1;
	}

	// �ޯ઺�Ѽ�
	pszOption = PROFESSION_SKILL_getChar( array, PROFESSION_SKILL_OPTION);

	// �ޯ൥�� rate
	if( (pszP = strstr( pszOption, "��%" ) ) != NULL )	sscanf( pszP+3, "%d", &rate );
	per = skill_level * rate;

	// �ǳƴ����ʵe
	img1 = PROFESSION_SKILL_getInt( array, PROFESSION_SKILL_IMG_1 );

	CHAR_setWorkInt( charaindex, CHAR_ENCOUNT_FIX, per);		
	CHAR_talkToCli( charaindex, -1, "�J�Ĳv�W��",  CHAR_COLORRED);
	CHAR_setWorkInt( charaindex, CHAR_ENCOUNT_NUM, (int)time(NULL) + (60 * 3) );

	sprintf( msg, "%d", img1);
	lssproto_BATTLESKILL_send ( getfdFromCharaIndex(charaindex), msg ) ; 

	return ret;
}

// �y�H�ޯ�-�j�׾԰�
int PROFESSION_escape( int charaindex, int toNo, int array, char *data, int skill_level )
{
	char *pszP = NULL, *pszOption = NULL, msg[20];
	int per=0, rate=0, img1, ret =1;
	skill_level /= 10;

	if( CHAR_getWorkInt( charaindex, CHAR_ENCOUNT_NUM) >= (int)time(NULL) ){
		ret = -1;
	}

	// �ޯ઺�Ѽ�
	pszOption = PROFESSION_SKILL_getChar( array, PROFESSION_SKILL_OPTION);

	// �ޯ൥�� rate
	if( (pszP = strstr( pszOption, "��%" ) ) != NULL )	sscanf( pszP+3, "%d", &rate );
	per = skill_level * rate;

	// �ǳƴ����ʵe
	img1 = PROFESSION_SKILL_getInt( array, PROFESSION_SKILL_IMG_1 );

	CHAR_setWorkInt( charaindex, CHAR_ENCOUNT_FIX, (-1) * per);
	CHAR_talkToCli( charaindex, -1, "�J�Ĳv�U��",  CHAR_COLORGREEN);

	CHAR_setWorkInt( charaindex, CHAR_ENCOUNT_NUM, (int)time(NULL) + (60 * 3) );

	sprintf( msg, "%d", img1);
	lssproto_BATTLESKILL_send ( getfdFromCharaIndex(charaindex), msg ) ; 

	return ret;
}



// �@�P�ޯ�-��O�R�K
int PROFESSION_full_mp( int charaindex, int toNo, int array, char *data, int skill_level )
{
	return TRUE;
}
// �@�P�ޯ�-�t���W�[
int PROFESSION_strong_back( int charaindex, int toNo, int array, char *data, int skill_level )
{
	return TRUE;
}
// �@�P�ޯ�-�ۧڱj��
int PROFESSION_strengthen( int charaindex, int toNo, int array, char *data, int skill_level )
{
	return TRUE;
}

#endif

