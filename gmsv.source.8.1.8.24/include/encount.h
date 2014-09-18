#ifndef __ENCOUNT_H__
#define __ENCOUNT_H__

#define		ENCOUNT_GROUPMAXNUM		10

BOOL ENCOUNT_initEncount( char* filename );
BOOL ENCOUNT_reinitEncount( void );
int ENCOUNT_getEncountAreaArray( int floor, int x, int y);
int ENCOUNT_getEncountPercentMin( int charaindex, int floor , int x, int y );
int ENCOUNT_getEncountPercentMax( int charaindex, int floor , int x, int y );
int ENCOUNT_getCreateEnemyMaxNum( int floor , int x, int y );
int ENCOUNT_getEncountIndex( int floor , int x, int y );
int ENCOUNT_getEncountIndexFromArray( int array );
int ENCOUNT_getEncountPercentFromArray( int array );
int ENCOUNT_getCreateEnemyMaxNumFromArray( int array );
int ENCOUNT_getGroupIdFromArray( int array, int grouparray );
int ENCOUNT_getGroupProbFromArray( int array, int grouparray );
int ENCOUNT_getZorderFromArray( int array );

#ifdef _ADD_ENCOUNT           // WON ADD �W�[�ľD�JĲ�o�ץ�
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
	int					event_now;			
	int					event_end;
	int					enemy_group;		// �Ǫ���group �s��
    RECT                rect;
}ENCOUNT_Table;

ENCOUNT_Table           *ENCOUNT_table;
#endif

#endif
