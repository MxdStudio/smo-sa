#ifndef __LINK_H__
#define __LINK_H__

#include "common.h"

/*
 * �ꥹ�ȹ�¤���������  �e
 * ���Υꥹ�Ȥ�val��    Ū�ˤϲ��⤷�ʤ��e�Ĥޤ�ݥ��󥿡��Υ��ԡ��Τ�
 * �Ԥ��e
 */
typedef struct tagNode
{
    struct tagNode* next;           /*���ΥΡ��ɤؤΥݥ��󥿡�*/
    char* val;                      /*��  ����  ��  */
    int size;                       /*val�Υ����N*/
}Node;

BOOL Nodeappendhead( Node** top  , Node* add );
BOOL Nodeappendtail( Node** top  , Node* add );
BOOL Noderemovehead( Node** top , Node* ret);
BOOL Noderemovetail( Node** top , Node* ret);
#endif
