#ifndef __MAGIC_BASE_H__
#define __MAGIC_BASE_H__

#include "util.h"

typedef enum
{
	MAGIC_FIELD_ALL,			/* ���٤Ƥξ��ǻȤ��� */
	MAGIC_FIELD_BATTLE,				/* ��    �Τ� */
	MAGIC_FIELD_MAP,				/* �̾�ޥå׾�Τ� */

}MAGIC_FIELDTYPE;

typedef enum
{
	MAGIC_TARGET_MYSELF,		/* ��ʬ�Τ� */
	MAGIC_TARGET_OTHER,			/* ¾�ο�  ��ʬ�ޤ�) */
	MAGIC_TARGET_ALLMYSIDE,		/*     ��   */
	MAGIC_TARGET_ALLOTHERSIDE,	/* ���¦��   */
	MAGIC_TARGET_ALL,			/* ���� */
	MAGIC_TARGET_NONE,			/* ï����  ��  �ʤ��e  ��䤿��λ� */
	MAGIC_TARGET_OTHERWITHOUTMYSELF,/* ¾�ο�  ��ʬ�ޤޤʤ�) */
	MAGIC_TARGET_WITHOUTMYSELFANDPET,  /* ��ʬ�ȥڥåȰʳ� */
	MAGIC_TARGET_WHOLEOTHERSIDE,/*     �Υ�������   */

#ifdef __ATTACK_MAGIC

        MAGIC_TARGET_SINGLE,            // �w��Ĥ誺�Y�@�H
        MAGIC_TARGET_ONE_ROW,           // �w��Ĥ誺�Y�@�C
        MAGIC_TARGET_ALL_ROWS,          // �w��Ĥ誺�Ҧ��H

#endif
}MAGIC_TARGETTYPE;

typedef enum
{
	MAGIC_ID,					/* ��    �� */
	MAGIC_FIELD,				/* �Ȥ����� */
	MAGIC_TARGET,				/* �о� */
	MAGIC_TARGET_DEADFLG,		/* �����Ԥ��оݤ˴ޤ�뤫 */
#ifdef __ATTACK_MAGIC
  MAGIC_IDX ,
#endif
	MAGIC_DATAINTNUM,
}MAGIC_DATAINT;

typedef enum
{
	MAGIC_NAME,					/* ����   */
	MAGIC_COMMENT,				/* ������*/
	MAGIC_FUNCNAME,				/* �ؿ�   */
	MAGIC_OPTION,				/* ���׼���� */
	MAGIC_DATACHARNUM,
}MAGIC_DATACHAR;

typedef struct tagMagic
{
	int			data[MAGIC_DATAINTNUM];
	STRING64	string[MAGIC_DATACHARNUM];

}Magic;

#ifdef __ATTACK_MAGIC

typedef struct tagAttMagic
{
  unsigned int  uiSpriteNum;// ���G�N�bSpr_x.bin���s��
  unsigned int  uiAttackType;// �������覡�G��H�A���( ���y ) ,  ���( ���y ) , ���( �P�� ) , ����( ���y ) , ����( �P�� )
  unsigned int  uiSliceTime;// ���y�����ɪ��ɶ��t
  unsigned int  uiShowType;             // ��ܪ���m�覡�G�����B���w
  int           siSx;                   // ��ܪ���m - X�b
  int           siSy;                   // ��ܪ���m - Y�b
  unsigned int  uiShowBehindChar;       // ��ܦb�H�����e��ΤU��
  unsigned int  uiShakeScreen;          // �O�_�_�ʵe��
  unsigned int  uiShakeFrom;            // �_�ʵe�����_�l�ɶ�( �@�� )
  unsigned int  uiShakeTo;              // �_�ʵe���������ɶ�( �@�� _
  unsigned int  uiPrevMagicNum;         // �e�m�G�N�����޸�( 0XFFFFFFFFFF ��ܵL�e�m�G�N )
  int           siPrevMagicSx;          // �e�m�G�N����ܦ�m - X�b
  int           siPrevMagicSy;          // �e�m�G�N����ܦ�m - Y�b
  int           siPrevMagicOnChar;      // �e�m�G�N��ܦb�H�����e��ΤU��
  unsigned int  uiPostMagicNum;         // ��m�G�N�����޸�( 0XFFFFFFFF ��ܵL��m�G�N )
  int           siPostMagicSx;          // ��m�G�N����ܦ�m - X�b
  int           siPostMagicSy;          // ��m�G�N����ܦ�m - Y�b
  int           siPostMagicOnChar;      // ��m�G�N��ܦb�H�����e��ΤU��
  int           siField[3][5];          // ��������
}AttMagic;

#endif

#ifdef _MAGIC_TOCALL

typedef struct tagToCallMagic
{
  unsigned int  uiSpriteNum;// ���G�N�bSpr_x.bin���s��
  unsigned int  uiAttackType;// �������覡�G��H�A���( ���y ) ,  ���( ���y ) , ���( �P�� ) , ����( ���y ) , ����( �P�� )
  unsigned int  uiSliceTime;// ���y�����ɪ��ɶ��t
  unsigned int  uiShowType;             // ��ܪ���m�覡�G�����B���w
  int           siSx;                   // ��ܪ���m - X�b
  int           siSy;                   // ��ܪ���m - Y�b
  unsigned int  uiShowBehindChar;       // ��ܦb�H�����e��ΤU��
  unsigned int  uiShakeScreen;          // �O�_�_�ʵe��
  unsigned int  uiShakeFrom;            // �_�ʵe�����_�l�ɶ�( �@�� )
  unsigned int  uiShakeTo;              // �_�ʵe���������ɶ�( �@�� _
  unsigned int  uiPrevMagicNum;         // �e�m�G�N�����޸�( 0XFFFFFFFFFF ��ܵL�e�m�G�N )
  int           siPrevMagicSx;          // �e�m�G�N����ܦ�m - X�b
  int           siPrevMagicSy;          // �e�m�G�N����ܦ�m - Y�b
  int           siPrevMagicOnChar;      // �e�m�G�N��ܦb�H�����e��ΤU��
  unsigned int  uiPostMagicNum;         // ��m�G�N�����޸�( 0XFFFFFFFF ��ܵL��m�G�N )
  int           siPostMagicSx;          // ��m�G�N����ܦ�m - X�b
  int           siPostMagicSy;          // ��m�G�N����ܦ�m - Y�b
  int           siPostMagicOnChar;      // ��m�G�N��ܦb�H�����e��ΤU��
  int			isPostDisappear;		// �G�N�@��������ɬO�_���W����
  int			ToCallMagicNo;			// �l��N���s��
}ToCallMagic;

#endif

typedef int (*MAGIC_CALLFUNC)( int, int, int, int );

INLINE BOOL MAGIC_CHECKINDEX( int index );
INLINE int MAGIC_getInt( int index, MAGIC_DATAINT element);
INLINE int MAGIC_setInt( int index, MAGIC_DATAINT element, int data);
INLINE char* MAGIC_getChar( int index, MAGIC_DATACHAR element);
INLINE BOOL MAGIC_setChar( int index ,MAGIC_DATACHAR element, char* new );
int MAGIC_getMagicNum( void);
BOOL MAGIC_initMagic( char *filename);
BOOL MAGIC_reinitMagic( void );

#ifdef __ATTACK_MAGIC

BOOL ATTMAGIC_initMagic( char *filename );
BOOL ATTMAGIC_reinitMagic( void );

#endif

int MAGIC_getMagicArray( int magicid);
MAGIC_CALLFUNC MAGIC_getMagicFuncPointer(char* name);
// Nuke +1 08/23 : For checking the validity of magic target
int MAGIC_isTargetValid( int magicid, int toindex);

#endif

