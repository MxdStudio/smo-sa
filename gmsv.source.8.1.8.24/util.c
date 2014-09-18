#include "version.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "common.h"
#include "util.h"
#include "configfile.h"

#define IS_2BYTEWORD( _a_ ) ( (char)(0x80) <= (_a_) && (_a_) <= (char)(0xFF) )
//��ӼҶ��ݭn���p�J�s�u��
/*-----------------------------------------
  ���Ĥβ��Ԥ򤳤�
  �����u ��Ȥ�  ��  �e  ��  ������뤾

  windows , macintosh , unix�ΤɤιԤ��б��Ǥ���e

  windows : \r\n
  macintosh : \n\r
  unix : \n

  ���ä�����
  macintosh : \r
  �Ǥ��e
  ����Ȥ��Υפҥ������Ȥ��Ĥ�  ������ʤ��Ƥ�
  ���Ծä����ɤ�����Ǥ���
  -----------------------------------------*/
void chompex( char *buf )
{
    while( *buf ){
        if( *buf == '\r' || *buf == '\n' ){
            *buf='\0';
        }
        buf++;
    }
}



/*----------------------------------------
 * ���Ĥ�1  ���������
 * ����
 *  src           ������  ��  
 ----------------------------------------*/
void chop( char* src )
{
    /*
     * ���������    ��  ���������
     */
    int length = strlen( src );
    if( length == 0 ) return;
    src[length-1] = '\0';
}


/*----------------------------------------
 * ���Ĥ�  ������e����  ����  ������Ǥ���e
 * ����
 *  src     ����  ��  
 *  delim   �������  ����    �e    ��NULL  ���ǽ��äƤ����
 ----------------------------------------*/
void dchop( char* src , char* del)
{
    int dellen, srclen;
    int i;
    BOOL delete=FALSE;

    /*
     * src��  ����Ĵ�٤Ƥ����e1  ���ˤĤ� del ���٤Ƥ�롼�פ���
     * ���פ���оä��e�����Ʊ���  ���ˤ����e
     */

    srclen = strlen( src );
    dellen = strlen( del );
    if( srclen == 0 || dellen == 0 ) return;

    for( i = 0 ; i < dellen ; i ++ ){
        if( src[srclen-1] == del[i] ){
            delete = TRUE;
            break;
        }
    }

    /*  �ڤ���    */
    if( delete )src[srclen - 1] = '\0';
}


/*----------------------------------------
 * dchop�εաe  ���  ������e����  ����  ������Ǥ���e
 * ����
 *  src     ����  ��  
 *  delim   �������  ����    �e    ��NULL  ���ǽ��äƤ����
 ----------------------------------------*/
void pohcd( char* src , char* del)
{
    int dellen, srclen;
    int i;
    BOOL delete=FALSE;

    /*
     * src��������Ĵ�٤Ƥ����e1  ���ˤĤ� del ���٤Ƥ�롼�פ���
     * ���פ���оä��e�����Ʊ���  ���ˤ����e
     */

    srclen = strlen( src );
    dellen = strlen( del );
    if( srclen == 0 || dellen == 0 ) return;

    for( i = 0 ; i < dellen ; i ++ ){
        if( src[0] == del[i] ){
            delete = TRUE;
            break;
        }
    }

    /*  ���    */
    if( delete )
        /*  ���Υ��ԡ��� NULL  ���⥳�ԡ������ */
        for( i = 0 ; i < srclen ; i ++ )
            src[i] = src[i+1];
}


/*----------------------------------------
 *   ��  ��  �����ޤޤ�Ƥ��뤫�ɤ���Ĵ�٤�e
 * ����
 *  src     char*   Ĵ�٤�  ��  
 *  include char*   �ޤޤ�Ƥ��뤫�ɤ�����  ��  �e
 * �֤���
 *  ��        ��˴ޤޤ�Ƥ���src�ξ��
 *  ��      -1
 ----------------------------------------*/
int charInclude( char* src, char* include )
{
    int     i,j;
    int     srclen,includelen;

    srclen = strlen( src );
    includelen = strlen( include );

    for( i = 0 ; i < srclen ; i ++ ){
        for( j = 0 ; j < includelen ; j ++ )
            if( src[i]  == include[j] )
                return i;
    }
    return -1;
}


/*------------------------------------------------------------
 * EUC��2�Х���  �����򥳥ԡ����ʤ�strncpy
 * freeBSD �Υ饤�֥�꤫�� strncpy ���äƤ��ơb
 * ������Ȥ˲�¤�e*dest++ �Ȥ��Ȥ��������ɡb������˼��Ȥ�
 * �ʤ��Τǡb�ȤäƤ��ʤ��e������    �����ɡb�ݥ��󥿤�ȤäƤ�
 * ���ʤ�  �ʥפҥ����
 * ����
 *  dest        char*   ���ԡ���
 *  src         char*   ������
 *  n           size_t  Ĺ��
 * �֤���
 *  dest
 ------------------------------------------------------------*/
char* strncpy2( char* dest, const char* src, size_t n )
{
    if( n > 0 ){
        char*   d = dest;
        const char*   s = src;
        int i;
        for( i=0; i<n ; i++ ){
            if( *(s+i) == 0 ){
                /*  ���ԡ�������ä��� NULL   ����  ���   */
                *(d+i) = '\0';
                return dest;
            }
            if( *(s+i) & 0x80 ){
                *(d+i)  = *(s+i);
                i++;
                if( i>=n ){
                    *(d+i-1)='\0';
                    break;
                }
                *(d+i)  = *(s+i);
            }else
                *(d+i) = *(s+i);
        }
    }
    return dest;
}

/*----------------------------------------
 * src ���� dest �˥��ԡ�����e ���ԡ���Υ����N��Ĵ�٤�e
 * ����
 *      dest        ���ԡ���
 *      n           �����N
 *      src         ����  ��  
 ----------------------------------------*/
void strcpysafe( char* dest ,size_t n ,const char* src )
{
    /*
     * src ���� dest �إ��ԡ�����.
     * strcpy, strncpy �Ǥ� dest ��� ���ԡ����묤��
     *   �������˸Ƥ֤�,���곰��������������.
     * �����  ���٤�, strncpy �����뤬 strlen( src ) �� n ���
     *   �������ˤ�, dest ��    �� NULL   ���ȤϤʤ�ʤ�.
     *
     * �������ä� dest ��  ������� src �Τۤ���Ĺ�����ˤ�
     * n-1 �� strncpy �򤹤�. ���������Ϥ��Τޤޥ��ԡ�����
     *
     * n ��  �λ��϶��������ʤ�Τ�  �λ��� ���⤷�ʤ��e
     *
     */
    // Nuke +1 (08/25): Danger if src=0
    if (!src) {
        *dest = '\0';
        return;
    }
    if( n <= 0 )        /* ���⤷�ʤ�   */
        return;

    /*  ���λ����ǡb n >= 1 �ʾ夬����  */
    /*  NULL  �����  ����  �Ӥ���  */
    else if( n < strlen( src ) + 1 ){
        /*
         * �Хåե���­��ʤ��Τ� n - 1(NULL  ��)
         * �� strncpy ��Ƥ�
         */
        strncpy2( dest , src , n-1 );
        dest[n-1]='\0';
    }else
        strcpy( dest , src );

}

/*----------------------------------------
 * src ���� length �Ǥ��e   ���ԡ����  ������Ĵ�٤�e
 * ����
 *          dest        ���ԡ���
 *          n           ���ԡ���Υ����N
 *          src         ���ԡ���
 *          length      ��  �����ԡ����뤫
 ----------------------------------------*/
void strncpysafe( char* dest , const size_t n ,
                  const char* src ,const int length )
{
    /*
     * src ���� dest �� length ���ԡ�����
     * strcpy, strncpy �Ǥ� dest ��� ���ԡ����묤��
     *   �������˸Ƥ֤�,���곰��������������.
     * ���δؿ��Ǥϡbstrlen( src ) �� length �ξ������ۤ�
     * (  �ݤ˥��ԡ����묤) �� dest �Υ����N��  �٤ơb
     * strcpysafe ��  �����򤹤�e
     */

    int Short;
    Short = min( strlen( src ) , length );

    /* NULL  �� ���  ����  �� */
    if( n < Short + 1 ){
        /*
         * �Хåե���­��ʤ��Τ� n - 1(NULL  ��)
         * �� strncpy ��Ƥ�
         */
        strncpy2( dest , src , n-1 );
        dest[n-1]='\0';

    }else if( n <= 0 ){
        return;
    }else{
        /*
         * �Хåե��Ͻ�ʬ�ˤ���Τ� Short ��strncpy��Ƥ�
         * �ʶ� src �ˤ� Short ��Ĺ��  �� NULL ���ʤ��Τǡb
         * dest �ˤ� �ɲä��ƶ����e
         */

        strncpy2( dest , src , Short );
        dest[Short]= '\0';

    }
}

/*------------------------------------------------------------
 * strcat�������  ������e�٤��Τǡb    �ʾ�������ƻȤ�ʤ����e
 * ����
 *  src     char*       ���ˤʤ�  ��  
 *  size    int         src �Υ����N(   ��ˤɤ�����ĤäƤ��뤫�Ǥ�
 *                          �ʤ��ƥХåե���   )
 *  ap      char*       �դ��ä���  
 * �֤���
 *  src     (  �����)
 ------------------------------------------------------------*/
char* strcatsafe( char* src, int size, char* ap )
{
    strcpysafe( src + strlen( src ) , size - strlen(src), ap );
    return src;
}

/*----------------------------------------
 *   ��  ��    ��  �Ӥ���e  �����ɤ�������  �ꤷ�ʤ� by ringo
 * char *s1 , *s2 :   �Ӥ���  ��  
 * �֤���
 * 0 �u ���ʤ�
 * 1 : ���Ȥʤ�
 * �����NPC��  �����Ȳ�  �ˤ䤯���ġe
 ----------------------------------------*/
//���u�ƨ�ƦN�JADD
int strcmptail( char *s1 , char *s2 )
{
    int i;
    int len1 = strlen( s1 );
    int len2 = strlen( s2 );
		int ind1,ind2;
    for(i=0;;i++){
        ind1 = len1 - 1 - i;
        ind2 = len2 - 1 - i;
        if( ind1 < 0 || ind2 < 0 ) return 0;
        if( s1[ind1] != s2[ind2] ) return 1;
    }
}



/*----------------------------------------
 *   ��  ��    �˽񤫤�Ƥ��������򥲥åȤ���e
 * NPC��  �����Ȳ�  �ˤ䤯���ġe
 * ������    �ˤ����  ����������  �뤹��e
 * ������  ���ʤ����르��N���  ���e
 * ����
 *  hoge    char*   ��  ����  ��  
 * �֤���
 *  ��  ���������e
 *
 *   �u
 * "ringo ( �a�P�a�P�P�H ) �G 100"
 *      �ξ���100��������e
 * "ringo ( �a�P�a�P�P�H ) �G �O100"
 *      ��100�ˤʤ�e�ۤ�ޤ�NPC�Ѥʤ�͡e
 ----------------------------------------*/
#define DECIMALCHARNUM 10
#define FIGURECHARNUM 3
#define SPECIALCHARNUM 2
char *UTIL_decimalchar[DECIMALCHARNUM] = {"0","1","2",
                                     "3",  "4","5",
                                     "6","7","8","9"};
char *UTIL_figurechar[FIGURECHARNUM] = {"","",""};
char *UTIL_specialchar[SPECIALCHARNUM] = {"",""};

/*
 *   ��  ����b���ͤ�  ����  ����  ʬ��  ���Ф��e��  �Τۤ�����
 * õ����  ���Ф��eeuc�ѡe
 *
 *    : in "�o���ѤC�U�U300�����ܡH"
 *      out "7�U"
 * ����
 * char *in:      ��  
 * char *out:��    ��  
 * int outsiz:��    ��  �ΥХåե������N
 * �֤���
 * ���ͤ�  ��  ʬ�����ä��餽��  ��  ��Ĺ���b�ʤ��ä��飰
 */
static int findNumberString( char *in, char *out, int outsiz )
{
    int len,i,j,k, findflag, numstrflag;
    char tmp[3];

    len = strlen(in);
    if( len == 0 ) return 0;

    numstrflag=0;
    findflag = 0;
    for( i=0,j=0;in[i]!='\0'&& j<outsiz-2;i++ ){
        findflag = 0;
        if( in[i] & 0x80 ){
            /* EUC�ξ�� */
            tmp[0] = in[i];
            tmp[1] = in[i+1];
            tmp[2] = '\0';
        }else{
            /* ASCII�ξ�� */
            tmp[0] = in[i];
            tmp[1] = '\0';
        }
        for( k=0;k<DECIMALCHARNUM;k++ ){
            if( strstr( UTIL_decimalchar[k], tmp ) ){
                numstrflag = 1;
                findflag = 1;
            }
        }
        for( k=0;k<FIGURECHARNUM;k++ ){
            if( strstr( UTIL_figurechar[k], tmp ) ){
                numstrflag = 1;
                findflag = 1;
            }
        }
        for( k=0;k<SPECIALCHARNUM;k++ ){
            if( strstr( UTIL_specialchar[k], tmp ) ){
                numstrflag = 1;
                findflag = 1;
            }
        }
        if( findflag ){
            if( in[i] & 0x80 ){
                out[j] = tmp[0];
                out[j+1] = tmp[1];
                j+=2;
                i++;
            }else{
                out[j] = tmp[0];
                j+=1;
            }
            continue;
        }

        if( numstrflag ){
            out[j] = '\0';
            return j;
        }

    }

    if( findflag ){
        out[j] = '\0';
        return j;
    }else{
        return 0;
    }

}


/*
 *   ��  ��int��  ���e
 *
 */
int numstrIsKanji( char *str );
int decimalstrToInt( char *str );
int kanjistrToInt( char *str );

int textToInt( char *str )
{
    char numstr[256];
    if( !findNumberString(str,numstr,sizeof(numstr)) ) return 0;

    if( numstrIsKanji(numstr) ) return kanjistrToInt(numstr);
    else return decimalstrToInt(numstr);

}

/*
 * ���ͤ�  ��  ��  ���b    �켰�ʤΤ��������ʤΤ���    
 * ����     ����  ��  �e������  ��  ��  �ʳ��ϴޤ�ǤϤ����ʤ��e
 * �֤���     �켰�ʤ飱�b�������ʤ飰
 *    str ��"�@���|�d�U�C"�ΤȤ��a�֤���1
 *    str ��"123"�ΤȤ��a�֤��ͣ�
 *
 */
int numstrIsKanji( char *str )
{
    int i,j;
    char tmp[3];

    for( i=0;str[i]!='\0';i++ ){
        if( str[i] & 0x80 ){
            tmp[0] = str[i];
            tmp[1] = str[i+1];
            tmp[2] = '\0';
            i++;
        }else{
            tmp[0] = str[i];
            tmp[1] = '\0';
        }
        for( j=0;j<FIGURECHARNUM;j++ ){
            if( strstr(UTIL_figurechar[j],tmp) ){
                return 1;
            }
        }
        for( j=0;j<SPECIALCHARNUM;j++ ){
            if( strstr(UTIL_specialchar[j],tmp) ){
                return 1;
            }
        }
    }
    return 0;

}

/*
 * ����ˡ�ǿ��ͤ�  ����  ��  ��int��  ��
 * char *str   ��  
 * �֤���   ��  ��  ������
 */
int decimalstrToInt( char *str )
{

    double val;
    char tmp[3];
    int i,j;

    val = 0;
    for( i=0;str[i]!='\0';i++ ){
        if( str[i] & 0x80 ){
            tmp[0] = str[i];
            tmp[1] = str[i+1];
            tmp[2] = '\0';
            i++;
        }else{
            tmp[0] = str[i];
            tmp[1] = '\0';
        }
        for( j=0;j<DECIMALCHARNUM;j++ ){
            if( strstr(UTIL_decimalchar[j],tmp) ){
                val = val*10+j;
            }
        }
    }

    if( val > 0x7fffffff ) return 0x7fffffff;
    else     return (int)val;

}


/*
 *     ����  �������ͤ�������  ��
 * char *str ���ͤ�  ��  ��  
 * �֤��� ����
 *   
 * str��"�@��4�d�U�H"�ΤȤ��֤��� 140000000
 */
int kanjistrToInt( char *str )
{
    double val,tmpval;
    char tmp[3];
    int num,i,j,numflag;

    numflag = 0;
    num = 1;
    tmpval = 0;
    val = 0;
    for( i=0;str[i]!='\0';i++ ){
        if( str[i] & 0x80 ){
            tmp[0] = str[i];
            tmp[1] = str[i+1];
            tmp[2] = '\0';
            i++;
        }else{
            tmp[0] = str[i];
            tmp[1] = '\0';
        }
        for( j=0;j<SPECIALCHARNUM;j++ ){
            if( strstr(UTIL_specialchar[j],tmp ) ){
                if( numflag == 1 ) tmpval += num;
                if( j == 0 ) val += tmpval*10000;
                else if( j == 1 ) val += tmpval*100000000;
                num = 1;
                numflag = 0;
                tmpval = 0;
                goto nextchar;
            }
        }
        for( j=0;j<FIGURECHARNUM;j++ ){
            if( strstr(UTIL_figurechar[j],tmp) ){
                if( j == 0 ) tmpval += num*10;
                else if( j == 1 ) tmpval += num*100;
                else if( j == 2 ) tmpval += num*1000;
                num = 1;
                numflag = 0;
                goto nextchar;
            }
        }
        for( j=0;j<DECIMALCHARNUM;j++ ){
            if( strstr(UTIL_decimalchar[j],tmp) ){
                num = j;
                numflag = 1;
                goto nextchar;
            }
        }
nextchar:
		continue;
    }
    if( numflag == 1 ) tmpval += num;
    val += tmpval;

    if( val > 0x7fffffff ) return 0x7fffffff;
    else     return (int)val;

}



/*----------------------------------------
 *   ��  ��    �˽񤫤�Ƥ��������򥲥åȤ���e
 * NPC��  �����Ȳ�  �ˤ䤯���ġe
 * ������    �ˤ����  ����������  �뤹��e
 * ������  ���ʤ����르��N���  ���e
 * ����
 *  hoge    char*   ��  ����  ��  
 * �֤���
 *  ��  ���������e
 *
 *   �u
 * "ringo ( �a�P�a�P�P�H ) �G 100"
 *      �ξ���100��������e
 * "ringo ( �a�P�a�P�P�H ) �G �O100"
 *      ��100�ˤʤ�e�ۤ�ޤ�NPC�Ѥʤ�͡e
 ----------------------------------------*/
int texttoiTail( char *hoge)
{
    return 0; /* �Ф��äƤ��ΤǤ��������e���Τ����ؿ����Ⱦä��e */


#if 0
    int len = strlen(hoge);

    if( len == 0 )return atoi(hoge);
    /*    ����õ����  ��˿����򸫤Ĥ���    */
    while(--len && len >= 0){
        if( isdigit(hoge[len]) )
            break;
    }
    while( --len && len >= 0 ){
        if( !isdigit(hoge[len]) )
            break;
        if( hoge[len]=='+' || hoge[len]=='-')
            break;
    }
    return atoi(hoge+len+1);
#endif
#if 0
    int f = 0;
    for(i=len-1;i>=0;i--){
        if( isdigit( hoge[i] ) ) f = 1;
        if( (f == 1)
            && !( isdigit(hoge[i]) || hoge[i]=='+' || hoge[i]=='-') ){
            return atoi( hoge + i + 1 );
        }
    }
    return atoi(hoge);
#endif
}

/*----------------------------------------
 *   ��  ��������ɤ��������å����ơb�������ä���
 *  ������  ����֤�
 * ����
 *      arg               ����
 *      number            ��  ����  �����
 *      base            ���
 *      type            �����η�( CTYPE �� common.h ���������Ƥ��� )
 * �֤���
 *      TRUE(1)    ��  
 *      FALSE(0)   strtol�������ˤϩ�  ���ʤ��ä�
 *                  ���λ���number��strtol���֤��ͤǤ���
 ----------------------------------------*/
BOOL strtolchecknum( char* arg , void* number,int base ,CTYPE type)
{
    char* buf;
    int     num;

    num = strtol( arg, &buf, base);

    switch( type ){
    case CHAR:
        *(char*)number = (char)num;
        break;
    case SHORT:
        *(short*)number = (short)num;
        break;
    case INT:
        *(int*)number = num;
        break;
    case DOUBLE:
        *(double*)number = (double)num;
        break;
    default:
        break;
    }


    if( strlen( buf ) >= 1 )
        /*
         * �Ĥ�  ��������Ȥ�������  ��������Ƥʤ��Ȥ������ʤΤ��e
         */
        return FALSE;

    return TRUE;

}




/*----------------------------------------
 * src ���� dels �ǻ��ꤷ��  ���������
 * �Хå�����ü���ϥ��������פǤ��������աe
 * �Ĥޤ�Хå�����ü���Ϻ���Ǥ��ʤ��e
 * ����
 *  src      �������  ��  
 *  char*  �������  ��(  ��  �ǻ����)
 * �֤���
 *  �ʤ�
 ----------------------------------------*/
void deleteCharFromString( char* src , char* dels )
{
    int index=0;    /* �Ǥ��������  ��  �Ǥ� index */
    int delength;   /* dels ��Ĺ�������ꤹ��(������®�٥��åפΰ� */
    int i=0,j;/* i �� src ��롼�פ���  �� j �� dels ��롼�פ���  �� */

    delength= strlen( dels );

    while( src[i] != '\0' ){
        if( src[i] == BACKSLASH  )
            /*
             * �Хå�����ü���ʤΤǥ��������פ���
             * �Ĥޤ걭��  ����  ���˰�  ����
             */
            src[index++] = src[++i];         /* substitute next char */

        else{
            for( j = 0 ; j < delength ; j ++ ){
                if( src[i] == dels[j] )
                    /*
                     * �������  �����e��ä� i �� 1 �򤿤�
                     */
                    goto incrementi;
            }
            /*
             * �ä�  �˻��ꤵ��Ƥ��ʤ��Τǥ��ԡ�����e
             */
            src[index++] =  src[i];
        }
    incrementi:
        i++;
    }

    src[index] = '\0';
}



/*----------------------------------------
 * src ���� dels �ǻ��ꤷ��  ���������
 * ���������פϤʤ�
 * ����
 *  src      �������  ��  
 *  char*  �������  ��(  ��  �ǻ����)
 * �֤���
 *  �ʤ�
 ----------------------------------------*/
void deleteCharFromStringNoEscape( char* src , char* dels )
{
    int index=0;    /* �Ǥ��������  ��  �Ǥ� index */
    int delength;   /* dels ��Ĺ�������ꤹ��(������®�٥��åפΰ� */
    int i=0,j;/* i �� src ��롼�פ���  �� j �� dels ��롼�פ���  �� */

    delength= strlen( dels );

    while( src[i] != '\0' ){
        for( j = 0 ; j < delength ; j ++ ){
            if( src[i] == dels[j] )
                /*
                 * �������  �����e��ä� i �� 1 �򤿤�
                 */
                goto incrementi;
        }
        /*
         * �ä�  �˻��ꤵ��Ƥ��ʤ��Τǥ��ԡ�����e
         */
        src[index++] =  src[i];
    incrementi:
        i++;
    }

    src[index] = '\0';
}

/*------------------------------------------------------------
 * ���ꤵ�줿  ������ꤵ�줿  ����  ��������
 * ����
 *  src     char*         �������  ��  
 *  oldc    char          �������  ��
 *  newc    char          ������  ��
 * �֤���
 *  src
 ------------------------------------------------------------*/
char*   replaceString( char* src, char oldc ,char newc )
{
    char*   cp=src;

    do{
        if( *cp == oldc ) *cp=newc;
    }while( *cp++ );
    return src;
}

typedef struct tagEscapeChar
{
    char     escapechar;
    char     escapedchar;
}EscapeChar;
static EscapeChar   escapeChar[]=
{
    { '\n',   'n' },
    { ',',    'c' },
    { '|',    'z' },
    { '\\',   'y' },
};


char makeCharFromEscaped( char c )//add this function,because the second had it
{
        int i;
        
        for( i = 0; i < sizeof( escapeChar )/sizeof( escapeChar[0] ); i++ )
        {
            if( escapeChar[i].escapedchar == c )
            {
               c = escapeChar[i].escapechar;
               break;
            }
        }
        return c;
}
/*----------------------------------------
 * makeEscapeString�Ǻ���줿  ��  �򸵤�  ��
 * ����
 *  src             char*       ���ˤʤ�  ��  �e���줬���ؤ�e
 * �֤���
 *  src    ���֤��e(�ؿ���  �줳�ˤ��䤹���褦��)
 ----------------------------------------*/
char   *makeStringFromEscaped( char* src )
{//ttom this function all change,beside copy from the second

    int     i;
    // CoolFish: Fix bug 2001/10/13
    // int     srclen = strlen( src );
    int	    srclen = 0;
    int     searchindex=0;
    
    // CoolFish: Fix bug 2001/10/13
    if (!src)	return	NULL;
    srclen = strlen(src);
    
    for( i = 0; i < srclen; i ++ )
    {
    // for 2Byte Word
    if( IS_2BYTEWORD( src[i] ) ){
        src[searchindex++] = src[i++];
        src[searchindex++] = src[i];
    }else
            if( src[i] == '\\' )
            {	
                // ����  ���˹Ԥ�
                i++;
                src[searchindex++] = makeCharFromEscaped( src[i] );
            }
            else
            {
                src[searchindex++] = src[i];
            }
    }        
            src[searchindex] = '\0';
        
            return src;
}

char*  makeEscapeString( char* src , char* dest, int sizeofdest)
{ //ttom this function all change, copy from the second
    int i;
    int	    srclen = 0;
    int     destindex=0;
    
    // CoolFish: Fix bug 2001/10/13
    if (!src)	return	NULL;
    srclen = strlen(src);
  	BOOL dirty;
    int  j;
    char escapechar='\0';
    for( i = 0 ; i < srclen ; i ++ ){
    	dirty=FALSE;
		if( destindex + 1 >= sizeofdest )break;
         if( IS_2BYTEWORD( src[i] ) ){
               if( destindex + 2 >= sizeofdest )break;
               
            dest[destindex] = src[i];
            dest[destindex+1] = src[i+1];
                 destindex += 2;
                 i ++;
                 continue;
          }
          for( j = 0; j<sizeof(escapeChar)/sizeof(escapeChar[0]); j++){
               if( src[i] == escapeChar[j].escapechar ){
                   dirty=TRUE;
                   escapechar= escapeChar[j].escapedchar;
                   break;
               }
          }
          if( dirty == TRUE ){
              if( destindex + 2 < sizeofdest ){
                  dest[destindex] = '\\';
                  dest[destindex+1] = escapechar;
                  destindex+=2;
                  dirty=TRUE;
                  continue;
              }else{
                  dest[destindex] = '\0';
                  return dest;
              }
          }else{
                dest[destindex] = src[i];
                destindex++;
          }
     }
     dest[destindex] = '\0';
     return dest;
}

//this function copy all from the second
char * ScanOneByte( char *src, char delim ){
	// Nuke
	if (!src) return NULL;

        //   ��  ���ʤ��ʤ�ޤǸ���
        for( ;src[0] != '\0'; src ++ ){
             // ���ѤΣ��Х���  ���ɤ���������å�
          if( IS_2BYTEWORD( src[0] ) ){
              // ���Ѥ��e���ξ��ϣ��Х���;ʬ�˿ʤޤ���e
              // ���������Х��Ȥ����ʤ����Ϥ������ʤ�
              if( src[1] != 0 ){
                  src ++;
              }
              continue;
          }
          //   �Ѥ��ä��e�������оݤ�  ����  ��
          if( src[0] == delim ){
              return src;
          }
        }
        // �롼��  �����鸫�Ĥ���ʤ��ä��e
        return NULL;
}
                                                                                                                                                                                                                                                                                                                


/*----------------------------------------
 * delim �ǻ��ꤵ�줿  ��  ����ڤ�  ���Ȥ���
 * index     ��  ��  ��eindex��1�ǤϤ��ޤ�e
 *   ��  ��  �e
 * ����
 *  src     ����  ��  
 *  delim   �ǥ�ߥ��Ȥʤ�  ��  �e    ��NULL  ���ǽ��äƤ����
 *  index   ��    ��  ���뤫
 *  buf     ���  ��  ���  ������ؤΥݥ��󥿡�
 *  buflen  ���  ��  ���  ������Υ����N
 * �֤���
 *  ���ä��� TRUE(1);
 *  �ʤ��ä��� FALSE(0);
 *  ex
 *      getStringFromIndexWithDelim( "aho=hoge","=",1,buf,sizeof(buf) );
 *      buf ... aho
 *
 *      getStringFromIndexWithDelim( "aho=hoge","=",2,buf,sizeof(buf) );
 *      buf ... hoge
 *      �����  �����e
 *
 *      getStringFromIndexWithDelim( "aho=hoge","=",3,buf,sizeof(buf) );
 *      �֤��� FALSE
 ----------------------------------------*/
BOOL getStringFromIndexWithDelim_body( char* src ,char* delim ,int index,
                                 char* buf , int buflen ,
                                       char *file, int line )
{//ttom this function all change,copy from the second
    int i;          /* �롼��  �� */
    int length =0;  /* ���Ф���  ��  ��Ĺ�� */
    int addlen=0;   /* ­�����Ĺ�� */
    int oneByteMode = 0; /* ���Х��ȥ⡼�ɤ��� */
		char* last;
    if( strlen( delim ) == 1 ){ // ���������Х��Ȥʤ飱�Х��ȥ⡼�ɤˤ���
        oneByteMode = 1;// ���κݣ��Х���  ���ϥ����å����ʤ�
    }
    for( i =  0 ; i < index ; i ++ ){
    	
         src += addlen;/* �ߤĤ��ä�Ĺ����­�� */
      
         if( oneByteMode ){
             // ���Х��ȥ⡼�ɤ��ä��餳����Ǹ���
             last = ScanOneByte( src, delim[0] );
         }else{
                 last  = strstr( src , delim );  /* ���դ��� */
         }
         if( last == NULL ){
            /*
             * �ߤĤ���ʤ��ä��ΤǤ��٤ƥ��ԡ����� return�e
            */
            strcpysafe( buf , buflen, src );

            if( i == index - 1 )
                /*���礦�ɤߤĤ��ä�*/
                return TRUE;
                                                                                                           
                /*�ߤĤ���ʤ��ä�*/
             return FALSE;
          }
          
          /*
           * �ߤĤ��ä����  ��ΰ�  �κ������
           * �Ĥޤ���ڤ��Ƥ���  ��  ��Ĺ��
          */
          length = last - src;
                                           
          /*
           * ���Υ롼�פΰ٤ˤߤĤ��ä�Ĺ���� delim ��Ĺ����­���ƶ���
          */
          addlen= length + strlen( delim );
       }
       strncpysafe( buf, buflen , src,length );

       return TRUE;
}


/*------------------------------------------------------------
 *   ��   "a,b,c,d" ���������  ����e�ǥե���Ȥ� 0 �ˤ���e
 * ���ʤ��  ��ؿ��e���ߤϤ���ä��٤��e
 * ����
 *  src         char*   ����  ��  
 *  int1        int*    int�Υݥ��󥿡e(a����  ����)
 *  int2        int*    int�Υݥ��󥿡e(b����  ����)
 *  int3        int*    int�Υݥ��󥿡e(c����  ����)
 *  int4        int*    int�Υݥ��󥿡e(d����  ����)
 * �֤���
 *  �ʤ�
 ------------------------------------------------------------*/
void getFourIntsFromString(char* src,int* int1,int* int2,int* int3,
                           int* int4)
{
    int     ret;
    char    string[128];

    ret = getStringFromIndexWithDelim( src,"," ,1,string,sizeof(string));
    if( ret == FALSE )*int1=0;
    else    *int1 = atoi(string);

    ret = getStringFromIndexWithDelim( src, ",",2,string,sizeof(string) );
    if( ret == FALSE )*int2=0;
    else    *int2 = atoi(string);

    ret = getStringFromIndexWithDelim( src, ",",3,string,sizeof(string) );
    if( ret == FALSE )*int3=0;
    else    *int3 = atoi(string);

    ret = getStringFromIndexWithDelim( src, ",",4,string,sizeof(string) );
    if( ret == FALSE )*int4=0;
    else    *int4 = atoi(string);

}


/*----------------------------------------------
 * src ��  ��dels�ǻ��ꤷ��  ����  ³���Ƥ�����
 * 1�ĤˤޤȤ��e
 * �Хå�����ü���ϥ��������׼������󥹤Ȥʤ�e
 * �������ä�  ³�����Хå�����ü����ä����ϤǤ��ʤ��e
 * ����
 *  src      ������  ��  
 *  dels   �ä�  ��(  ��  �ǻ����)
 * �֤���
 *  �ʤ�
 ---------------------------------------------*/
void deleteSequentChar( char* src , char* dels )
{
    int length;         /* src ��Ĺ�� */
    int delength;       /* dels ��Ĺ�� */
    int i,j;            /* �롼��  �� */
    int index=0;        /* ����  ��  �� index */
    char backchar='\0';   /* �������  �� */

    length = strlen( src );
    delength = strlen( dels );

    /*
     * �롼�פ��뤴�Ȥ˥��ԡ�����
     */
    for( i = 0 ; i < length ; i ++ ){
        if( src[i] == BACKSLASH ){
            /*
             * �Хå�����ü�����ä���
             * ����Ū��  ��( BACKSLASH���Ȥȡb����  �� )
             * �ʤ��
             */
            src[index++]=src[i++];
            if( i >= length ){
                print( "*\n");
                break;
            }
            src[index++]=src[i];
            /*   ³���ߤä����ˤ��� */
            backchar = '\0';
            continue;
        }
        if( src[i] == backchar )
            /*
             * �������⤸���ä��鲿�⤷�ʤ��e�Ĥޤ����ˤʤ�e
             */
            continue;

        /*
         *   ³�κ������  �����ʤ��ʤä��Τ� backchar �� NULL   ���ˤ���
         * ���줬���󤸤�
         */
        backchar = '\0';
        for( j = 0 ; j < delength ; j ++ ){
            if( src[i] == dels[j] ){
                /*
                 * �������  ��  ���e ������ continue ���ʤ����ˤ��
                 * ��  ���Ϻ�����ʤ������ݾڤǤ���e
                 */
                backchar=src[i];
                break;
            }
        }
        src[index++]=src[i];
    }
    src[index++]='\0';
}

/*----------------------------------------
 * hash�ͤ��֤��e  ��  �ˤΤ߻Ȥ�����
 * �����  ���ΤޤޤʤΤǥ��르��N��Ϥ褦�狼���
 * �������르��N��餷���e
 * ����
 *  s     ��  
 * �֤���
 *  int �ϥü�����
 *----------------------------------------*/
#define PRIME 211
int hashpjw ( char* s )
{
    char *p;
    unsigned int h= 0 ,g;
    for( p = s ; *p ; p ++ ){
        h = ( h<< 4 ) + (*p);
        if( (g = h & 0xf0000000) != 0){
            h = h ^ (g>>24);
            h = h ^ g;
        }
    }
    return h % PRIME;
}

/*----------------------------------------
 * �ҡ�����˥Х���ɤ��������åȤ��֤�( TCP )
 * ����
 *          port        �ҡ�����ݡ���
 * �֤���
 *          -1      ���顼
 *                      1. socket����  �ॳ����Υ��顼
 *                      2. bind����  �ॳ����Υ��顼
 *                      3. listen����  �ॳ����Υ��顼
 ----------------------------------------*/
int bindlocalhost( int port )
{
    struct sockaddr_in sin;         /*�ҡ����륢�ɥ쥹��*/
    int sfd;                        /*�����åȥǥ�������ץ�*/
    int rc;                         /*�ҡ�����꥿���󥳡���*/

    /*AF_INET�פҥȥ��� �� SOCK_STREAM */
    sfd = socket( AF_INET, SOCK_STREAM, 0 );
    if( sfd == -1 ){
        print( "%s\n" , strerror(errno ) );
        return -1;
    }
  #ifdef _JZ_EPOLL
    if(!setnonblocking(sfd))return -1;			//�N�JADD �D����Ҧ�
  #endif
    if( getReuseaddr() ) {
		int sendbuff;
	    /* �ҡ����륢�ɥ쥹�κ����� */
		setsockopt( sfd, SOL_SOCKET, SO_REUSEADDR, 
						(char *)&sendbuff, sizeof( sendbuff));
	}
	
    /*0���ꥢ�����ͤ���  */
    memset( &sin ,0, sizeof( struct sockaddr_in ) );
    sin.sin_family=AF_INET;
    sin.sin_port = htons( port );
    sin.sin_addr.s_addr = INADDR_ANY;

    rc = bind( sfd , (struct sockaddr*)&sin, sizeof(struct sockaddr_in));
    if( rc == -1 ){
        print( "%s\n" , strerror(errno ) );
        return -1;
    }
	
	
    rc = listen( sfd , 5 );
    if( rc == -1 ){
        print( "%s\n" , strerror(errno ) );
        return -1;
    }
    return sfd;
}



/*----------------------------------------
 * ���ͥ��Ȥ���e( TCP )
 * ����
 *      hostname    ��³����ۥ���  
 *                      (xx.xxx.xxx.xxx�Ǥ⤤����xxx.co.jp�Ȥ��Ǥ⤤��)
 *      port        ��³����ۥ��ȤΥݡ���  
 * �֤���
 *      �ǥ�������ץ�( -1 �λ��ϥ��顼 )
 ----------------------------------------*/
int connectHost( char* hostname , unsigned short port )
{
    struct sockaddr_in sock;        /*connect�λ��˻Ȥ�*/
    struct hostent* hoste;          /*hostname��hostent*/
    int     fd;                     /*���ͥ��Ȥ��������åȥǥ�������ץ�*/
    int     lr;                     /*�ҡ�����꥿���󥳡���*/

    memset( &sock ,  0 , sizeof( struct sockaddr_in ) );
    sock.sin_family = AF_INET;
    sock.sin_port = htons( port );

    /* dot notation ���ɤ�����Ĵ�٤� */
    sock.sin_addr.s_addr = inet_addr( hostname );
    if( sock.sin_addr.s_addr == -1 ){
        /*
         *���顼 dot notation �ǤϤʤ��e�������ä�  �̤�  ����褬  �פ��e
         * dns��Ҥ��ʤ���Фʤ�ʤ����ϤҤ��ˤ����Τ⤳���Ǥ�äƤ����e
         */
        hoste = gethostbyname( hostname );
        if( hoste == NULL ){
            print( "gethostbyname error hostname: %s\n", hostname);
            return -1;
        }

        memcpy((void*)&sock.sin_addr.s_addr ,
               hoste->h_addr , sizeof(struct in_addr) );
    }

    /*�����åȤ���*/
    fd = socket( AF_INET, SOCK_STREAM , 0 );
    if( fd == -1 ){
        print("Cannot Create Socket(%s errno:%d)\n"
              ,strerror( errno ) ,errno );
        return -1;
    }
    /*���ͥ��Ȥ���*/
    lr = connect(fd,(struct sockaddr*)&sock,sizeof(struct sockaddr_in));
    if( lr != 0 ){
        print("Cannot connect. (%s errno:%d)\n"
              ,strerror( errno ) ,errno );
        return -1;
    }

    return fd;
}


/*----------------------------------------
 * �Хåե���  �˲��Ե��椬���뤫�ɤ����e
 * �����ᤤ���르��N���  ���e
 * ����
 *  char*   Ĵ�٤�  ��  
 * �֤���
 *    ��˸��Ĥ������Ե���ΰ�       ���������������ˤ����͡e
 *  �ɤ����������Ȥ����ȡb
 *      existsNewLinwCharacter( "aho\nhoge\n" )
 *  ���ȡb�֤��ͤ� 3 �ˤʤ�e�������դ�����e
 *  -1      �ʤ��ä�
 *
 ----------------------------------------*/
int existsNewLineCharacter( char* line )
{
#if 1
    char*   old = line;
    do{
        if( *line == NEWLINE)return line-old;
    }while( *(line++) );
#else
    int i;                          /*�롼��  ��*/
    int length = strlen( line );    /*Ĵ�٤�  ��  ��Ĺ��*/
    for( i = 0 ; i < length ; i ++ ){
        if( line[i] == NEWLINE )
            return i;
    }
#endif

    return -1;
}



/*----------------------------------------
 *    ��    ��  ����  ������򤫤����e��    ��������e
 *  nindex( string , c , 1 ) �� index( string , c ) �� equivalent �Ǥ���e
 *  number��  �ޤ���0�λ����֤��ͤϡbstring��  ���Ǥ���e
 *
 *  ����
 *      string  char*   Ĵ�٤�  ��  
 *      c       int     Ĵ�٤�  ��
 *      number  int       ���  ��
 *  �֤���
 *      ���פ���  ���Υݥ��󥿡�
 *      NULL    �ߤĤ���ʤ��ä��e
 ----------------------------------------*/
char* nindex( char* string, int c , int number )
{
    int i;                          /*�롼��  ��*/
    int num=0;
    int length = strlen( string );  /*Ĵ�٤�  ��  ��Ĺ��*/
    if( number <= 0 )return string;
    for( i = 0 ; i < length ; i ++ ){
        if( string[i] == c )num++;
        if( number == num ) return &string[i];
    }
    return NULL;
}

BOOL rrd( char* dirname , STRING64* buf ,int bufsize, int* index)
{
    DIR*    d;
    char    dirn[1024];

    d = opendir( dirname );
    if( d == NULL )return FALSE;

    while( 1 ){
        struct dirent* dent;
        struct  stat    st;
        dent = readdir( d );
        if( dent == NULL ){
            if( errno == EBADF ){
                errorprint;
                closedir(d);
                return FALSE;
            }else
                /*  successful */
                break;
        }

        /*  . �ǻϤޤ�ե������  �ޤʤ�    */
        if(  dent->d_name[0] == '.' )continue;

        snprintf(dirn, sizeof(dirn), "%s/%s" , dirname,dent->d_name );
        if( stat( dirn , &st ) == -1 )continue;
        if( S_ISDIR( st.st_mode ) ){
            if( rrd( dirn , buf ,bufsize, index) == FALSE ){
                closedir(d);
                return FALSE;
            }
        }else{
            if( *index >= bufsize )
                break;

            strcpysafe( buf[*index].string , sizeof( buf[*index].string ), dirn );
            (*index)++;
        }
    }
    closedir(d);
    return TRUE;
}

int rgetFileName( char* dirname , STRING64* string, int size)
{
    int     index=0;
    if( rrd( dirname , string ,size ,&index ) == FALSE )return -1;
    else return index;
}

BOOL checkStringsUnique( char** strings, int num ,int verbose)
{
    int     i,j;
    for( i = 0 ; i < num - 1; i ++ ){
        for( j = i+1 ; j < num ; j ++){
            if( !strcmp( strings[i] ,strings[j] )){
                if( verbose )
                    print( "Overlapp string is %s\n", strings[i] );
                return FALSE;
            }
		}
	}
    return TRUE;
}

BOOL PointInRect( RECT* rect, POINT* p )
{
    if( rect->x         <= p->x && p->x <= rect->x + rect->width &&
        rect->y         <= p->y && p->y <= rect->y + rect->height )
        return TRUE;
    return FALSE;
}

BOOL CoordinateInRect( RECT* rect, int x, int y)
{
    POINT   p={x,y};
    return PointInRect(rect,&p);
}

int clipRect( RECT *rect1, RECT *rect2, RECT *ret )
{
    if(   rect1->x > rect2->x + rect2->width -1
       || rect2->x > rect1->x + rect1->width -1
       || rect1->y > rect2->y + rect2->height -1
       || rect2->y > rect1->y + rect1->height -1 )
	   return 0;
    ret->x = max( rect1->x, rect2->x );
    ret->y = max( rect1->y, rect2->y );
    ret->width = min( rect1->x+rect1->width, rect2->x+rect2->width ) - ret->x;
    ret->height = min( rect1->y+rect1->height, rect2->y+rect2->height ) - ret->y;
    return 1;
}

BOOL isstring1or0( char*  string )
{
    if( strcasecmp(string,"TRUE" ) == 0 )return TRUE;
    if( strcasecmp(string,"FALSE") == 0 )return FALSE;
    if( strcasecmp(string,"1"    ) == 0 )return TRUE;
    if( strcasecmp(string,"0"    ) == 0 )return FALSE;
    if( strcasecmp(string,"ON"   ) == 0 )return TRUE;
    if( strcasecmp(string,"OFF"  ) == 0 )return FALSE;
    return FALSE;
}

void easyGetTokenFromString( char *src,int count,char*output,int len )
{
    int i;
    int counter = 0;

    if( len <= 0 )return;


#define ISSPACETAB( c )   ( (c) == ' ' || (c) == '\t' )

    for(i=0;;i++){
        if( src[i]=='\0'){
            output[0] = '\0';
            return;
        }
        if( i > 0 && ! ISSPACETAB( src[i-1] ) &&
            ! ISSPACETAB(  src[i] ) ){
            continue;
        }

        if( ! ISSPACETAB( src[i]) ){
            counter++;
            if( counter == count){
                /* copy it */
                int j;
                for(j=0;j<len-1;j++){
                    if( src[i+j] == '\0' ||
                        ISSPACETAB( src[i+j] ) ){
                        break;
                    }
                    output[j]=src[i+j];
                }
                output[j]='\0';
                return;

            }
        }

    }
}

/*------------------------------------------------------------
 * ������  ʬ�����Ȥ��e by ringo
 * ���ΰ�  ��
 *
 * double val1 , val2 : �����ͤΤ�������Ȥ�
 * double d :   ʬ  
 *
 *
 *  -d<0----- val1 ---0<d<1------- val2 ------d>1----
 *
 *
 ------------------------------------------------------------*/
float  linearDiv( float val1 , float val2 , float d )
{
    return val1 + ( val2 - val1 ) * ( d );
}


/*------------------------------------------------------------
 *   ��  �Τʤ�����  �դ�  ����len  ���Ȥ�����ƥ������
 *   ��  ��Ĥ���eNPC�Υ�����ѥ���������ˤĤ��äƤ�
 *
 * char *cand : �����������֡e��  ��  ���e
 * char *out : ��  �Хåե�
 * int len : ��  ��Ĺ��
 ------------------------------------------------------------*/
void makeRandomString( char *cand , char *out , int len )
{
    int i;
    int l = strlen( cand );

    for(i=0;i<len;i++){
        out[i] = cand[ rand() % l];
    }
    out[i] = '\0';
}


/*------------------------------------------------------------
 * �����˻��ꤵ�줿�ե����뤬�ե����뤫�ɤ���Ĵ�٤�
 * ����
 *  filename        char*        �ե�����  
 * �֤���
 *  �ե�����        TRUE(1)
 *  �ե�����Ǥʤ�  FALSE(0)
 ------------------------------------------------------------*/
BOOL isExistFile( char* filename )
{
    /*  fopen ��Ĵ�٤�  */
    FILE*   fp;
    fp = fopen( filename, "w" );
    if( fp ){
        fclose( fp );
        return TRUE;
    }else
        return FALSE;
}
/*------------------------------------------------------------
 * 10��  62��  ��
 * ����
 * a        int     ���Σ�����
 * out      char *  ��  �Хåե���
 * outlen   int     ��  �Хåե���  ����
 *   ����
 *          ��      out�Υ��ɥ쥹
 *          ��      NULL
 ------------------------------------------------------------*/
char *cnv10to62( int a, char *out, int outlen )
{
#if 1
	int		i, j;
    char    base[] = { "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    int     tmp[64];
    int     src;
    int minus;
	int baselen = sizeof( base)-1;
    if( a < 0 ){
		minus = 1;
		a *= -1;
	} else {
		minus = 0;
	}
    /* special case */
    if( a < baselen) {
		if( minus ){
			*(out) = '-';
			*(out+1) = base[a];
			*(out+2) = '\0';
			return (out);
		} else {
			*out = base[a];
			*(out+1) = '\0';
			return( out);
		}
    }
    src = a;
    for( i = 0; src >= baselen; i ++ ) {
        tmp[i] = src % baselen;
        src /= baselen;
    }
    i--;
    if( minus ){
		*out = '-';
     	*(out+1) = base[src];
		for( j = 2; i >= 0; i --, j ++ ) {
			if( j > outlen - 2 ) return NULL;
			*(out+j) = base[tmp[i]];
		}
	} else {
     	*out = base[src];
		for( j = 1; i >= 0; i --, j ++ ) {
			if( j > outlen - 2 ) return NULL;
			*(out+j) = base[tmp[i]];
		}
	}
	*(out+j) = '\0';
    return( out);
#else
/* �Ť�   */
#define     CNV_NUMBER  62
	int		i, j;
    char    base[] = { "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    int     tmp[64];
    int     src;
    if( a < 0 ) return( NULL);
    if( a < CNV_NUMBER) {
	    *out = base[a];
	    *(out+1) = '\0';
        return( out);
    }
    src = a;
    for( i = 0; src >= CNV_NUMBER; i ++ ) {
        tmp[i] = src % CNV_NUMBER;
        src /= CNV_NUMBER;
    }
    i--;
	*out = base[src];
    for( j = 1; i >= 0; i --, j ++ ) {
		if( j > outlen - 2 ) return NULL;
		*(out+j) = base[tmp[i]];
    }
	*(out+j) = '\0';
    return( out);
#undef      CNV_NUMBER
#endif
}
/* 
 * Ϳ����줿    ��  �ǡf���ͤ���  ���Ƥ��뤫Ĵ�٤�ؿ�
 * -1 ���оݳ��Ȥ���  �ʴؿ�
 *
 * ����
 * src		*int		Ĵ�٤븵��    
 * srclen	int			src ��  ����
 *
 *   ����   ��  ����	TRUE
 *				�ʤ�	FALSE
 */
BOOL checkRedundancy( int *src, int srclen)
{
	int		i,j;
	int		ret = FALSE;
	
	for( i = 0; i < srclen; i ++ ) {
		if( *(src + i) != -1 ) {
			for( j = i+1; j < srclen; j ++ ) {
				if( *(src +i) == *( src+j) ) {
					ret = TRUE;
					break;
				}
			}
		}
	}
	return ret;
}
void
shuffle_ints( int num, int *a, int t )
{
    int i;
    int x ,y ,s;
    for(i=0;i<t; i++ )
    {
    	y = random() % num;
			x = random() % num;
      s = a[x];
      a[x] = a[y];
      a[y] = s;
    }
}


static unsigned char BitTable[] =	/*	�ӥåȤ��¤ӽ��դˤ���  ���֥�	*/
{
		0x00 , 0x80 , 0x40 , 0xC0 , 0x20 , 0xA0 , 0x60 , 0xE0 , 
		0x10 , 0x90 , 0x50 , 0xD0 , 0x30 , 0xB0 , 0x70 , 0xF0 , 
		0x08 , 0x88 , 0x48 , 0xC8 , 0x28 , 0xA8 , 0x68 , 0xE8 , 
		0x18 , 0x98 , 0x58 , 0xD8 , 0x38 , 0xB8 , 0x78 , 0xF8 , 
		0x04 , 0x84 , 0x44 , 0xC4 , 0x24 , 0xA4 , 0x64 , 0xE4 , 
		0x14 , 0x94 , 0x54 , 0xD4 , 0x34 , 0xB4 , 0x74 , 0xF4 , 
		0x0C , 0x8C , 0x4C , 0xCC , 0x2C , 0xAC , 0x6C , 0xEC , 
		0x1C , 0x9C , 0x5C , 0xDC , 0x3C , 0xBC , 0x7C , 0xFC , 
		0x02 , 0x82 , 0x42 , 0xC2 , 0x22 , 0xA2 , 0x62 , 0xE2 , 
		0x12 , 0x92 , 0x52 , 0xD2 , 0x32 , 0xB2 , 0x72 , 0xF2 , 
		0x0A , 0x8A , 0x4A , 0xCA , 0x2A , 0xAA , 0x6A , 0xEA , 
		0x1A , 0x9A , 0x5A , 0xDA , 0x3A , 0xBA , 0x7A , 0xFA , 
		0x06 , 0x86 , 0x46 , 0xC6 , 0x26 , 0xA6 , 0x66 , 0xE6 , 
		0x16 , 0x96 , 0x56 , 0xD6 , 0x36 , 0xB6 , 0x76 , 0xF6 , 
		0x0E , 0x8E , 0x4E , 0xCE , 0x2E , 0xAE , 0x6E , 0xEE , 
		0x1E , 0x9E , 0x5E , 0xDE , 0x3E , 0xBE , 0x7E , 0xFE , 
		0x01 , 0x81 , 0x41 , 0xC1 , 0x21 , 0xA1 , 0x61 , 0xE1 , 
		0x11 , 0x91 , 0x51 , 0xD1 , 0x31 , 0xB1 , 0x71 , 0xF1 , 
		0x09 , 0x89 , 0x49 , 0xC9 , 0x29 , 0xA9 , 0x69 , 0xE9 , 
		0x19 , 0x99 , 0x59 , 0xD9 , 0x39 , 0xB9 , 0x79 , 0xF9 , 
		0x05 , 0x85 , 0x45 , 0xC5 , 0x25 , 0xA5 , 0x65 , 0xE5 , 
		0x15 , 0x95 , 0x55 , 0xD5 , 0x35 , 0xB5 , 0x75 , 0xF5 , 
		0x0D , 0x8D , 0x4D , 0xCD , 0x2D , 0xAD , 0x6D , 0xED , 
		0x1D , 0x9D , 0x5D , 0xDD , 0x3D , 0xBD , 0x7D , 0xFD , 
		0x03 , 0x83 , 0x43 , 0xC3 , 0x23 , 0xA3 , 0x63 , 0xE3 , 
		0x13 , 0x93 , 0x53 , 0xD3 , 0x33 , 0xB3 , 0x73 , 0xF3 , 
		0x0B , 0x8B , 0x4B , 0xCB , 0x2B , 0xAB , 0x6B , 0xEB , 
		0x1B , 0x9B , 0x5B , 0xDB , 0x3B , 0xBB , 0x7B , 0xFB , 
		0x07 , 0x87 , 0x47 , 0xC7 , 0x27 , 0xA7 , 0x67 , 0xE7 , 
		0x17 , 0x97 , 0x57 , 0xD7 , 0x37 , 0xB7 , 0x77 , 0xF7 , 
		0x0F , 0x8F , 0x4F , 0xCF , 0x2F , 0xAF , 0x6F , 0xEF , 
		0x1F , 0x9F , 0x5F , 0xDF , 0x3F , 0xBF , 0x7F , 0xFF
};
static unsigned short crctab16[] =	/*	crc�a�η׻�  ���֥�		*/
{
		0x0000,  0x1021,  0x2042,  0x3063,  0x4084,  0x50a5,  0x60c6,  0x70e7,
		0x8108,  0x9129,  0xa14a,  0xb16b,  0xc18c,  0xd1ad,  0xe1ce,  0xf1ef,
		0x1231,  0x0210,  0x3273,  0x2252,  0x52b5,  0x4294,  0x72f7,  0x62d6,
		0x9339,  0x8318,  0xb37b,  0xa35a,  0xd3bd,  0xc39c,  0xf3ff,  0xe3de,
		0x2462,  0x3443,  0x0420,  0x1401,  0x64e6,  0x74c7,  0x44a4,  0x5485,
		0xa56a,  0xb54b,  0x8528,  0x9509,  0xe5ee,  0xf5cf,  0xc5ac,  0xd58d,
		0x3653,  0x2672,  0x1611,  0x0630,  0x76d7,  0x66f6,  0x5695,  0x46b4,
		0xb75b,  0xa77a,  0x9719,  0x8738,  0xf7df,  0xe7fe,  0xd79d,  0xc7bc,
		0x48c4,  0x58e5,  0x6886,  0x78a7,  0x0840,  0x1861,  0x2802,  0x3823,
		0xc9cc,  0xd9ed,  0xe98e,  0xf9af,  0x8948,  0x9969,  0xa90a,  0xb92b,
		0x5af5,  0x4ad4,  0x7ab7,  0x6a96,  0x1a71,  0x0a50,  0x3a33,  0x2a12,
		0xdbfd,  0xcbdc,  0xfbbf,  0xeb9e,  0x9b79,  0x8b58,  0xbb3b,  0xab1a,
		0x6ca6,  0x7c87,  0x4ce4,  0x5cc5,  0x2c22,  0x3c03,  0x0c60,  0x1c41,
		0xedae,  0xfd8f,  0xcdec,  0xddcd,  0xad2a,  0xbd0b,  0x8d68,  0x9d49,
		0x7e97,  0x6eb6,  0x5ed5,  0x4ef4,  0x3e13,  0x2e32,  0x1e51,  0x0e70,
		0xff9f,  0xefbe,  0xdfdd,  0xcffc,  0xbf1b,  0xaf3a,  0x9f59,  0x8f78,
		0x9188,  0x81a9,  0xb1ca,  0xa1eb,  0xd10c,  0xc12d,  0xf14e,  0xe16f,
		0x1080,  0x00a1,  0x30c2,  0x20e3,  0x5004,  0x4025,  0x7046,  0x6067,
		0x83b9,  0x9398,  0xa3fb,  0xb3da,  0xc33d,  0xd31c,  0xe37f,  0xf35e,
		0x02b1,  0x1290,  0x22f3,  0x32d2,  0x4235,  0x5214,  0x6277,  0x7256,
		0xb5ea,  0xa5cb,  0x95a8,  0x8589,  0xf56e,  0xe54f,  0xd52c,  0xc50d,
		0x34e2,  0x24c3,  0x14a0,  0x0481,  0x7466,  0x6447,  0x5424,  0x4405,
		0xa7db,  0xb7fa,  0x8799,  0x97b8,  0xe75f,  0xf77e,  0xc71d,  0xd73c,
		0x26d3,  0x36f2,  0x0691,  0x16b0,  0x6657,  0x7676,  0x4615,  0x5634,
		0xd94c,  0xc96d,  0xf90e,  0xe92f,  0x99c8,  0x89e9,  0xb98a,  0xa9ab,
		0x5844,  0x4865,  0x7806,  0x6827,  0x18c0,  0x08e1,  0x3882,  0x28a3,
		0xcb7d,  0xdb5c,  0xeb3f,  0xfb1e,  0x8bf9,  0x9bd8,  0xabbb,  0xbb9a,
		0x4a75,  0x5a54,  0x6a37,  0x7a16,  0x0af1,  0x1ad0,  0x2ab3,  0x3a92,
		0xfd2e,  0xed0f,  0xdd6c,  0xcd4d,  0xbdaa,  0xad8b,  0x9de8,  0x8dc9,
		0x7c26,  0x6c07,  0x5c64,  0x4c45,  0x3ca2,  0x2c83,  0x1ce0,  0x0cc1,
		0xef1f,  0xff3e,  0xcf5d,  0xdf7c,  0xaf9b,  0xbfba,  0x8fd9,  0x9ff8,
		0x6e17,  0x7e36,  0x4e55,  0x5e74,  0x2e93,  0x3eb2,  0x0ed1,  0x1ef0,
};
unsigned short CheckCRC( unsigned char *p , int size )
{
	unsigned short	crc = 0;
	int		i;
	
	for( i = 0 ; i < size ; i ++ ){
		crc = ( crctab16[ ( crc >> 8 ) & 0xFF ] 
					^ ( crc << 8 ) ^ BitTable[ p[ i ] ] );
	}
	return crc;
}

// Add Code By Shan  2001.06.16
INLINE double time_diff(struct timeval subtrahend,
                      struct timeval subtractor)
{
    return( (subtrahend.tv_sec - subtractor.tv_sec)
             + (subtrahend.tv_usec
             - subtractor.tv_usec  ) / (double)1E6 );
}
#ifdef _JZ_EPOLL
BOOL setnonblocking(int sock)
{
	int opts;
	opts=fcntl(sock,F_GETFL,0);
	if(opts<0)
	{
		perror("fcntl(sock,GETFL)");
		return FALSE;
	}
	opts = opts|O_NONBLOCK;
	if(fcntl(sock,F_SETFL,opts)<0)
	{
		perror("fcntl(sock,SETFL,opts)");
		return FALSE;
	}
	return TRUE;
}
#endif
