#include "version.h"
#include "char.h"

/*


  ������١e

  ��꡼��  �Ǥ�2��  �Τ��Ȥ��ޡe

 NPCARGUMENT:  0���ä��餳�Ȥ���0�b1���ä��餳�Ȥ���1�θ��  �e  

 ���֤�3�Ĥ���b��˿ʹԤ���e

 ����0�u �����ɥ�󥰡e���ξ��֤�talk�����ȡb
 �֤褯�����b�۹��۹���e�����ˤ�b�路����  �Τ�����٤���e�פ�
 ����1�˰ܹԡe
 ����1�utalk���줿��b
 �֤��ʤ����Τ���ʤ�Сb���ȤҤ������ˡb��  �ʸ��ʤ�ҤȤġb �������褦�e
        ���θ��ʤ�Τ��फ? yes/no��yes�Ǿ���2�ءeno��
        �֤Ǥϡb����Ф���e�פǾ���0��
        
 ����2�u talk���줿��b
 �֤��ʤ�����  �θ��ʡ֤۹��۹��פ����������e  �ΤĤ���  �ڤʤ�ΤȤ���
        ����  �ڤˤ���٤��e����Ф���e��
        �Ȥ��äƺ��򸺤餷�b���ʥե饰���ɲä���e
        �Ǿ���0�ˤ�ɤ�e



 
 Talked:

 if(     �ˤϤʤ��Ƥ���1ʬ���äƤ��뤫��){
   ����0�ˤ�ɤ�e
 }
   
 switch(����){
 case 0:  ��  �e����1�ءebreak;
 case 1:  ��  �e����2�ءebreak;
 case 2:  ��  �e����0�ءebreak;
 } 

 }

 
 
 

 



 */
BOOL NPC_StoryTellerInit( int meindex )
{
    CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPESTORYTELLER );
    CHAR_setFlg( meindex , CHAR_ISATTACKED , 0 );
    CHAR_setFlg( meindex , CHAR_ISOVERED , 0 );

    return TRUE;
}


void NPC_StoryTellerTalked( int meindex , int talker , char *msg , int col )
{
    
}
