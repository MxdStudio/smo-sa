#include "version.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef _REDHAT_V9
#include <errno.h>
#endif

#include "common.h"
#include "util.h"
#include "buf.h"
#include "char_base.h"
#include "char.h"
#include "configfile.h"
#include "encount.h"
#include "enemy.h"

#ifdef _ADD_ENCOUNT           // WON ADD �W�[�ľD�JĲ�o�ץ�
#include "encount.h"
#endif

/* ���󥫥���ȴ�  �Υ����� */

#ifndef _ADD_ENCOUNT           // WON ADD �W�[�ľD�JĲ�o�ץ�
typedef struct tagENCOUNT_Table
{
    int                 index;
    int                 floor;
    int                 encountprob_min;                /* ���󥫥���ȳ�   */
    int                 encountprob_max;                /* ���󥫥���ȳ�   */
    int                 enemymaxnum;        /* �ɤ����Ũ���뤫 */
    int                 zorder;
    int                 groupid[ENCOUNT_GROUPMAXNUM];       /* ���롼��No */
    int                 createprob[ENCOUNT_GROUPMAXNUM];    /* ���Υ��롼�פνи�   */
    RECT                rect;
}ENCOUNT_Table;
ENCOUNT_Table           *ENCOUNT_table;
#endif

int                     ENCOUNT_encountnum;
#define     ENCOUNT_ENEMYMAXCREATENUM   10

static INLINE BOOL ENCOUNT_CHECKENCOUNTTABLEARRAY( int array)
{
    if( array < 0 || array > ENCOUNT_encountnum-1) return FALSE;
    return TRUE;
}

/*------------------------------------------------------------
 * ���󥫥��������ν�����򤹤�e
 * ����
 *  filename        char*       ����ե�����  
 * �֤���
 *  ��      TRUE(1)
 *  ��      FALSE(0)
 *------------------------------------------------------------*/
BOOL ENCOUNT_initEncount( char* filename )
{
    FILE*   f;
    char    line[256];
    int     linenum=0;
    int     encount_readlen=0;

    f = fopen(filename,"r");
    if( f == NULL ){
        errorprint;
        return FALSE;
    }

    ENCOUNT_encountnum=0;

    /*  �ޤ�  ���ʹԤ����Ԥ��뤫�ɤ���Ĵ�٤�    */
    while( fgets( line, sizeof( line ), f ) ){
        linenum ++;
        if( line[0] == '#' )continue;        /* comment */
        if( line[0] == '\n' )continue;       /* none    */
        chomp( line );

        ENCOUNT_encountnum++;
    }

    if( fseek( f, 0, SEEK_SET ) == -1 ){
        fprint( "Seek Error\n" );
        fclose(f);
        return FALSE;
    }

    ENCOUNT_table = allocateMemory( sizeof(struct tagENCOUNT_Table)
                                   * ENCOUNT_encountnum );
    if( ENCOUNT_table == NULL ){
        fprint( "Can't allocate Memory %d\n" ,
                sizeof(ENCOUNT_table)*ENCOUNT_encountnum);
        fclose( f );
        return FALSE;
    }

    /* ����� */
{
    int     i,j;
    for( i = 0; i < ENCOUNT_encountnum; i ++ ) {
        ENCOUNT_table[i].index = -1;
        ENCOUNT_table[i].floor = 0;
        ENCOUNT_table[i].encountprob_min = 1;
        ENCOUNT_table[i].encountprob_min = 50;
        ENCOUNT_table[i].enemymaxnum = 4;
        ENCOUNT_table[i].rect.x = 0;
        ENCOUNT_table[i].rect.y = 0;
        ENCOUNT_table[i].rect.height = 0;
        ENCOUNT_table[i].rect.width = 0;
        ENCOUNT_table[i].zorder = 0;
        for( j = 0; j < ENCOUNT_GROUPMAXNUM; j ++ ) {
            ENCOUNT_table[i].groupid[j] = -1;
            ENCOUNT_table[i].createprob[j] = -1;
        }
#ifdef _ADD_ENCOUNT           // WON ADD �W�[�ľD�JĲ�o�ץ�
		ENCOUNT_table[i].event_now = -1;
		ENCOUNT_table[i].event_end = -1;
		ENCOUNT_table[i].enemy_group = -1;
#endif
    }
}

    /*  �ޤ�  ��  ��    */
    linenum = 0;
    while( fgets( line, sizeof( line ), f ) ){
        linenum ++;
        if( line[0] == '#' )continue;        /* comment */
        if( line[0] == '\n' )continue;       /* none    */
        chomp( line );

        /*  �Ԥ���������    */
        /*  �ޤ� tab �� " " ��  ��������    */
        replaceString( line, '\t' , ' ' );
        /* ��  �Υ��ڡ�������e*/
{
        int     i;
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
        int     x1,x2,y1,y2;
		int		j;
        
        /*   �٤�Υ롼�פ�  �ä����ΰ٤ν���� */
        ENCOUNT_table[encount_readlen].index = -1;
        ENCOUNT_table[encount_readlen].floor = 0;
        ENCOUNT_table[encount_readlen].encountprob_min = 1;
        ENCOUNT_table[encount_readlen].encountprob_min = 50;
        ENCOUNT_table[encount_readlen].enemymaxnum = 4;
        ENCOUNT_table[encount_readlen].rect.x = 0;
        ENCOUNT_table[encount_readlen].rect.y = 0;
        ENCOUNT_table[encount_readlen].rect.height = 0;
        ENCOUNT_table[encount_readlen].rect.width = 0;
        ENCOUNT_table[encount_readlen].zorder = 0;
        for( j = 0; j < ENCOUNT_GROUPMAXNUM; j ++ ) {
            ENCOUNT_table[encount_readlen].groupid[j] = -1;
            ENCOUNT_table[encount_readlen].createprob[j] = -1;
        }
#ifdef _ADD_ENCOUNT           // WON ADD �W�[�ľD�JĲ�o�ץ�
		ENCOUNT_table[encount_readlen].event_now = -1;
		ENCOUNT_table[encount_readlen].event_end = -1;
		ENCOUNT_table[encount_readlen].enemy_group = -1;
#endif


        /*  �ҤȤĤ�Υȡ�����򸫤�    */
        ret = getStringFromIndexWithDelim( line,",",1,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].index = atoi(token);
        
        /*  2�Ĥ�Υȡ�����򸫤�    */
        ret = getStringFromIndexWithDelim( line,",",2,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].floor = atoi(token);

        /*  3�Ĥ�Υȡ�����򸫤�    */
        ret = getStringFromIndexWithDelim( line,",",3,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        x1 = atoi(token);

        /*  4�Ĥ�Υȡ�����򸫤�    */
        ret = getStringFromIndexWithDelim( line,",",4,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        y1= atoi(token);
        
        /*  5�Ĥ�Υȡ�����򸫤�    */
        ret = getStringFromIndexWithDelim( line,",",5,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        
        x2 = atoi(token);
        
        /*  6�Ĥ�Υȡ�����򸫤�    */
        ret = getStringFromIndexWithDelim( line,",",6,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        y2= atoi(token);

        ENCOUNT_table[encount_readlen].rect.x      = min(x1,x2);
        ENCOUNT_table[encount_readlen].rect.width  = max(x1,x2) - min(x1,x2);
        ENCOUNT_table[encount_readlen].rect.y      = min(y1,y2);
        ENCOUNT_table[encount_readlen].rect.height = max(y1,y2) - min(y1,y2);

        /*  7��Υȡ�����򸫤�    */
        ret = getStringFromIndexWithDelim( line,",",7,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].encountprob_min = atoi(token);

        /*  8��Υȡ�����򸫤�    */
        ret = getStringFromIndexWithDelim( line,",",8,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].encountprob_max = atoi(token);

{
		int		a,b;
		a = ENCOUNT_table[encount_readlen].encountprob_min;
		b = ENCOUNT_table[encount_readlen].encountprob_max;
		/*   ����Ĵ�� */
        ENCOUNT_table[encount_readlen].encountprob_min 
        	= min( a,b);
        ENCOUNT_table[encount_readlen].encountprob_max 
        	= max( a,b);
}
        /*  9�Ĥ�Υȡ�����򸫤�    */
        ret = getStringFromIndexWithDelim( line,",",9,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        {
            int maxnum = atoi( token);
            /* �����������Υ����å� */
            if( maxnum < 1 || maxnum > ENCOUNT_ENEMYMAXCREATENUM ) {
                fprint("createnum error file:%s line:%d\n",filename,linenum);
                continue;
            }
            ENCOUNT_table[encount_readlen].enemymaxnum = maxnum;
        }
        /*  10��Υȡ�����򸫤�    */
        ret = getStringFromIndexWithDelim( line,",",10,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].zorder = atoi(token);
        #define		CREATEPROB_TOKEN	11
        
        /*  11  31��Υȡ�����򸫤�    */
        {
            int     i;
            
            for( i = CREATEPROB_TOKEN; i < CREATEPROB_TOKEN +ENCOUNT_GROUPMAXNUM*2; i ++) {
                ret = getStringFromIndexWithDelim( line,",",i,token,
                                                   sizeof(token));
                if( ret==FALSE ){
                    fprint("Syntax Error file:%s line:%d\n",filename,linenum);
                    continue;
                }
                if( strlen( token) != 0 ) {
                    if( i < CREATEPROB_TOKEN + ENCOUNT_GROUPMAXNUM ) {
                        ENCOUNT_table[encount_readlen].groupid[i-CREATEPROB_TOKEN] 
                            = atoi(token);
                    }
                    else {
                        ENCOUNT_table[encount_readlen].createprob[i-(CREATEPROB_TOKEN + ENCOUNT_GROUPMAXNUM)] 
                            = atoi(token);
                    }
                }
            }

            /* ��  �����å� */
            if( checkRedundancy( ENCOUNT_table[encount_readlen].groupid, 
            			arraysizeof( ENCOUNT_table[encount_readlen].groupid)))
            {
            	fprint( "error:�ζ�����file:%s line:%d\n", 
            				filename,linenum);
            	continue;
            }
        }


#ifdef _ADD_ENCOUNT           // WON ADD �W�[�ľD�JĲ�o�ץ�
        ret = getStringFromIndexWithDelim( line,",",31,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].event_now = atoi(token);

        ret = getStringFromIndexWithDelim( line,",",32,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].event_end = atoi(token);

        ret = getStringFromIndexWithDelim( line,",",33,token,
                                           sizeof(token));
        if( ret==FALSE ){
            fprint("Syntax Error file:%s line:%d\n",filename,linenum);
            continue;
        }
        ENCOUNT_table[encount_readlen].enemy_group = atoi(token);
#endif

        encount_readlen ++;
}
    }
    fclose(f);

    ENCOUNT_encountnum = encount_readlen;

    print( "Valid encount field Num is %d..", ENCOUNT_encountnum );

#if 0

    {
        int i;
        for( i=0; i <ENCOUNT_encountnum ; i++ )
            print( "encount idx[%d] fl[%d] prob_min[%d] prob_max[%d] e_max[%d] x[%d] wth[%d] y[%d] hgt[%d] \n",
                   ENCOUNT_table[i].index,
                   ENCOUNT_table[i].floor,
                   ENCOUNT_table[i].encountprob_min,
                   ENCOUNT_table[i].encountprob_max,
                   ENCOUNT_table[i].enemymaxnum,
                   ENCOUNT_table[i].rect.x,
                   ENCOUNT_table[i].rect.width,
                   ENCOUNT_table[i].rect.y,
                   ENCOUNT_table[i].rect.height);
    }
#endif
    return TRUE;
}
/*------------------------------------------------------------------------
 * ���󥫥��������ե�����  ��  ��
 *-----------------------------------------------------------------------*/
BOOL ENCOUNT_reinitEncount( void )
{
	freeMemory( ENCOUNT_table);
	return( ENCOUNT_initEncount( getEncountfile()));
}

/*------------------------------------------------------------
 * ���ꤵ�줿��  ��ENCOUNT_table��ź����Ĵ�٤�e
 * zorder�ο����򸫤�ͥ���̤ι⤤  ���  ����e
 * ����
 *  floor       int     �դҥ�ID
 *  x           int     x��  
 *  y           int     y��  
 * �֤���
 *  ����      ź��
 *  ��  ��    -1
 ------------------------------------------------------------*/
int ENCOUNT_getEncountAreaArray( int floor, int x, int y)
{
    int     i;
    int     index = -1;
    for( i=0 ; i<ENCOUNT_encountnum ; i++ ) {
        if( ENCOUNT_table[i].floor == floor ) {
            if( CoordinateInRect( &ENCOUNT_table[i].rect, x, y) ) {
                int curZorder = ENCOUNT_getZorderFromArray(i);
                if( curZorder >0) {
                    if( index != -1 ) {
                        /* ͥ���̤�Ĵ�٤� */
                        /*   ����  ͥ�� */
                        if(  curZorder > ENCOUNT_getZorderFromArray(index)) {
                            index = i;
                        }
                    }
                    else {
                        index = i;
                    }
                }
            }
        }
    }
    return index;
}

/*------------------------------------------------------------
 * ���ꤵ�줿��  �Υ��󥫥���ȳ�  ��Ĵ�٤�e
 * ����
 *  floor       int     �դҥ�ID
 *  x           int     x��  
 *  y           int     y��  
 * �֤���
 *  ����      ���ʾ�γ�  
 *  ��  ��    -1
 ------------------------------------------------------------*/
int ENCOUNT_getEncountPercentMin( int charaindex, int floor , int x, int y )
{
    int ret;
    
    ret = ENCOUNT_getEncountAreaArray( floor, x, y);
    if( ret != -1 ) {
        ret = ENCOUNT_table[ret].encountprob_min;
		/* �ȥؤҥ����̤�Ĥ��� */
		if( CHAR_getWorkInt( charaindex, CHAR_WORK_TOHELOS_COUNT) > 0 ) {
			ret = ceil( ret * 
				((100 + CHAR_getWorkInt( charaindex, CHAR_WORK_TOHELOS_CUTRATE)) 
					/ 100.0));
		}
		if( ret < 0 ) ret = 0;
		if( ret > 100 ) ret = 100;
    }
    return ret;
}
/*------------------------------------------------------------
 * ���ꤵ�줿��  �Υ��󥫥���ȳ�  ��Ĵ�٤�e
 * ����
 *  floor       int     �դҥ�ID
 *  x           int     x��  
 *  y           int     y��  
 * �֤���
 *  ����      ���ʾ�γ�  
 *  ��  ��    -1
 ------------------------------------------------------------*/
int ENCOUNT_getEncountPercentMax( int charaindex, int floor , int x, int y )
{
    int ret;
    
    ret = ENCOUNT_getEncountAreaArray( floor, x, y);
    if( ret != -1 ) {
        ret = ENCOUNT_table[ret].encountprob_max;
		/* �ȥؤҥ����̤�Ĥ��� */
		if( CHAR_getWorkInt( charaindex, CHAR_WORK_TOHELOS_COUNT) > 0 ) {
			ret = ceil( ret * 
				((100 + CHAR_getWorkInt( charaindex, CHAR_WORK_TOHELOS_CUTRATE)) 
					/ 100.0));
		}
		if( ret < 0 ) ret = 0;
		if( ret > 100 ) ret = 100;
    }
    return ret;
}
/*------------------------------------------------------------
 * ���ꤵ�줿��  ��Ũ����MAX����Ĵ�٤�e
 * ����
 *  floor       int     �դҥ�ID
 *  x           int     x��  
 *  y           int     y��  
 * �֤���
 *  ����      ���ʾ�γ�  
 *  ��  ��    -1
 ------------------------------------------------------------*/
int ENCOUNT_getCreateEnemyMaxNum( int floor , int x, int y )
{
    int ret;
    
    ret = ENCOUNT_getEncountAreaArray( floor, x, y);
    if( ret != -1 ) {
        ret = ENCOUNT_table[ret].enemymaxnum;
    }
    return ret;
}
/*------------------------------------------------------------
 * ���ꤵ�줿��  �Υ��󥫥���ȥե�����ɤ�index��Ĵ�٤�e
 * ����
 *  floor       int     �դҥ�ID
 *  x           int     x��  
 *  y           int     y��  
 * �֤���
 *  ����      ���ʾ�
 *  ��  ��    -1
 ------------------------------------------------------------*/
int ENCOUNT_getEncountIndex( int floor , int x, int y )
{
    int ret;
    
    ret = ENCOUNT_getEncountAreaArray( floor, x, y);
    if( ret != -1 ) {
        ret = ENCOUNT_table[ret].index;
    }
    return ret;
}
/*------------------------------------------------------------
 * ���ꤵ�줿��  �Υ��󥫥���ȥե�����ɤ�index��Ĵ�٤�e
 * ����
 *  array           int     ENCOUNTTABLE��ź��
 * �֤���
 *  ����      ���ʾ�
 *  ��  ��    -1
 ------------------------------------------------------------*/
int ENCOUNT_getEncountIndexFromArray( int array )
{
    if( !ENCOUNT_CHECKENCOUNTTABLEARRAY( array)) return -1;
    return ENCOUNT_table[array].index;
}
/*------------------------------------------------------------
 * ���ꤵ�줿��  �Υ��󥫥���ȳ�  ��Ĵ�٤�e
 * ����
 *  array           int     ENCOUNTTABLE��ź��
 * �֤���
 *  ����      ���ʾ�
 *  ��  ��    -1
 ------------------------------------------------------------*/
int ENCOUNT_getEncountPercentFromArray( int array )
{
    if( !ENCOUNT_CHECKENCOUNTTABLEARRAY( array)) return -1;
    return ENCOUNT_table[array].encountprob_min;
}
/*------------------------------------------------------------
 * ���ꤵ�줿��  ��Ũ����MAX����Ĵ�٤�e
 * ����
 *  array           int     ENCOUNTTABLE��ź��
 * �֤���
 *  ����      ���ʾ�
 *  ��  ��    -1
 ------------------------------------------------------------*/
int ENCOUNT_getCreateEnemyMaxNumFromArray( int array )
{
    if( !ENCOUNT_CHECKENCOUNTTABLEARRAY( array)) return -1;
    return ENCOUNT_table[array].enemymaxnum;
}
/*------------------------------------------------------------
 * ���ꤵ�줿ź���Υ��롼��  ���Ĵ�٤�e
 * ����
 *  array           int     ENCOUNTTABLE��ź��
 * �֤���
 *  ����      ���ʾ�
 *  ��  ��    -1
 ------------------------------------------------------------*/
int ENCOUNT_getGroupIdFromArray( int array, int grouparray )
{
    if( !ENCOUNT_CHECKENCOUNTTABLEARRAY( array)) return -1;
    return ENCOUNT_table[array].groupid[grouparray];
}
/*------------------------------------------------------------
 * ���ꤵ�줿ź���Υ��롼�פνи�  ��Ĵ�٤�e
 * ����
 *  array           int     ENCOUNTTABLE��ź��
 * �֤���
 *  ����      ���ʾ�
 *  ��  ��    -1
 ------------------------------------------------------------*/
int ENCOUNT_getGroupProbFromArray( int array, int grouparray )
{
    if( !ENCOUNT_CHECKENCOUNTTABLEARRAY( array)) return -1;
    return ENCOUNT_table[array].createprob[grouparray];
}
/*------------------------------------------------------------
 * ���ꤵ�줿ź����ͥ���̤�Ĵ�٤�e
 * ����
 *  array           int     ENCOUNTTABLE��ź��
 * �֤���
 *  ����      ���ʾ�
 *  ��  ��    -1
 ------------------------------------------------------------*/
int ENCOUNT_getZorderFromArray( int array )
{
    if( !ENCOUNT_CHECKENCOUNTTABLEARRAY( array)) return -1;
    return ENCOUNT_table[array].zorder;
}
