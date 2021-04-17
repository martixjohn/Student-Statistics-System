#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#ifdef VS
#define scanf scanf_s
#define printf printf_s
#define fopen fopen_s
#endif

/*URL*/
#define URL "data\\save.dat"
#define URL_BAK "data\\save.bak"
#define URL_S "data\\settings.ini"

/*ERROR CODE*/
#define SUCCESS 0
#define FAIL -1
#define FOUND SUCCESS
#define NOT_FOUND -404
#define SURE 1
#define DONT 2
#define LIST_SURE SURE
#define LIST_DONT DONT
#define VALID SUCCESS
#define INVALID FAIL

#define WAY_OF_LIST_
#define ALL 1
#define MASS 1
#define SINGLE 2
#define DELETE_MENU 3
#define DELETE_STEP 1
#define DELETE_ALL 2
#define EDIT_ONE 4

#define WAY_OF_LIST_STATISTICS_
#define S_ALL 5//总分
#define S_TOTAL S_ALL
#define S_S1 1
#define S_S2 2
#define S_S3 3
#define S_S4 4
#define ALL_SUB 6//全部学科
//真实数据存储在i-1下标 
#define NAME_OF_SCORE
#define G1N setting.subject_name[0]
#define G2N setting.subject_name[1]
#define G3N setting.subject_name[2]
#define G4N setting.subject_name[3]


#define ST_TITLE
typedef enum{
	T_tt=1,T_ct=2
}st_t;

typedef char* st_tt;
#define ST_TTITLE_NAME st_tt
#define T_STANDARD "STANDARD"
#define T_SETTING "SETTING"
#define ST_TT_NUM 2

typedef char* st_ct;
#define ST_CTITLE_NAME st_ct
#define C_SUBJECT1_A "SUBJECT1_A"
#define C_SUBJECT1_B "SUBJECT1_B"
#define C_SUBJECT1_C "SUBJECT1_C"
#define C_SUBJECT2_A "SUBJECT2_A"
#define C_SUBJECT2_B "SUBJECT2_B"
#define C_SUBJECT2_C "SUBJECT2_C"
#define C_SUBJECT3_A "SUBJECT3_A"
#define C_SUBJECT3_B "SUBJECT3_B"
#define C_SUBJECT3_C "SUBJECT3_C"
#define C_SUBJECT4_A "SUBJECT4_A"
#define C_SUBJECT4_B "SUBJECT4_B"
#define C_SUBJECT4_C "SUBJECT4_C"
#define C_ALL_A "ALL_A"
#define C_ALL_B "ALL_B"
#define C_ALL_C "ALL_C"
#define C_SUBJECT1_NAME "SUBJECT1"
#define C_SUBJECT2_NAME "SUBJECT2"
#define C_SUBJECT3_NAME "SUBJECT3"
#define C_SUBJECT4_NAME "SUBJECT4"
#define C_LENOFLIST "LISTLEN"
#define ST_CT_NUM 20

typedef enum{
	C_SUBJECT_LEVEL=10,C_SUBJECT_NAME=20
}st_ct_type;
#define ST_CONTENT_TITLE_TYPE st_ct_type

typedef enum{
	FILE_DATA=100,FILE_DATA_BAK=200,FILE_SET=300,FILE_STA=400
}file_type;
#define FILE_TYPE_ file_type

typedef enum{
	b_true=1,b_false=0
}bool_type;

typedef enum{
	NO=1,NAME,G1,G2,G3,G4=0
}e_lst;
#define END G4
#define POS(X) (X%6)

#define GRADE_ANALYSE

#define L_A "优秀"
#define L_B "较好"
#define L_C "一般"
#define L_D "较差"
char * sub_sc_level[]={L_A,L_B,L_C,L_D};
typedef enum{
	lv_a=0,lv_b,lv_c,lv_d
}level_ana;
typedef struct DataList{
	int pos;
	char num[11];
	char name[21];
	char score[4][7];
	double realscore[5];//1,2,3,4,ALL
	double aver;
	struct DataList *next;
}Data;
Data * headOfNode=NULL;

//SETTING
typedef struct{
	//[STANDARD]
	double level_A[5];//优秀4科成绩+总分A级 
	double level_B[5];//良好 
	double level_C[5];//及格
	//总分等级ABC 
	//OTHERSETTING[SETTING]
	char subject_name[4][31];
	int list_len;
}Set;
Set setting;

//STATISTICS
typedef struct{
	//DATA
	int stu_num;//总人数 
	//ABC等级人数 
	int level_num[5][4];
	//S1: A B C D(不及格)
	//S2: A B C
	//...ALL: A B C
	double aver_sc[5];//s1~ALL
	double rate[5][4];
	/*
	S1 A B C D
	*/
	double m_score[5][2];//科目MAX,MIN分数 
}Sta;
Sta statis;

//INIT STANDARD SETTING
#define S1A "120"
#define S2A "130"
#define S3A "130"
#define S4A "110"
#define ALLA "480"
#define S1B "100"
#define S2B "110"
#define S3B "90"
#define S4B "95"
#define ALLB "380"
#define S1C "60"
#define S2C "70"
#define S3C "60"
#define S4C "75"
#define ALLC "280"
#define S1N "第一科成绩"
#define S2N "第二科成绩"
#define S3N "第三科成绩"
#define S4N "第四科成绩"
#define LEN_OF_LIST "5"
typedef enum{
	s_1=0,s_2,s_3,s_4,s_all
}sub;

//函数声明 
/*GUI*/
int iStream(int* choose);
int Menu();

void Choose(int console);

void outputDataUI();
int listData(int way);
typedef enum{
	previous_list,next_list
}list_pos;
Data* listData_findList(list_pos next_or_previous,int this_pos,int list_num);//查找每列第一个位置

void inputDataUI();
int saveData(int way);

void aboutUI();

void backupUI(); 

void statisticsUI();
int statisticsAll();
int statisticsSingle();

void settingUI();
void settingLevel();
int setLevel(int choose);
void settingName();
void settingListLen();
//int listData(FILE *fp,int way);
//int saveData(FILE *fp,int way);



void prtInfo(const Data *p);//打印 
Data* findStu(char stunum[11],int list_or_no);
int editStu(int idpos,char opt);

//文件与链表转换，都涉及排序 
//DATALIST 动态链表 
long fileToNode(file_type filetype);
long nodeToFile(file_type filetype);//fp,fp_bak
//SETTING 静态单结构体 
int st_fileToNode();
int st_nodeToFile();
//链表 
int addOne(Data *input);
int inputOne(Data *input);
int initStd();
int saveSta(const Data *pD);//FILE TO STATISTICS LINK
int staToFile(int subject_type);
int showSta(int way);
int showSta_detail(int way);

//基本算法 
void sortNode(Data *Nhead);
void swapNode(Data *p1,Data *p2);
void cpyNodeExpNext(Data *dst,const Data *from);

int outOfRange(const char *text,int limit);
int getstrlen(const char *text);
int validID(const char *id);
int validSc(const char *score);
void fgetALine(char *buf,int num,FILE *fp);//fgets除去\n 
long fp_st_ttitleTurnToPos(FILE *fp,const char * dst);//从头开始，每次使用复位指针位置 
long fp_st_ctitleTurnToPos(FILE *fp,const char * dst,long posT);//每次从posT位置开始， 
void PrtS(const char * text);
#define TEXT_LINE 20
#define TEXT_LINE_C 100

int charToInt(char c);
char intToChar(int i);
double strToDouble(double * dst,const char from[7]);
char* doubleToStr(char *to,const double* from);
char* intToStr(char to[6],const int* from_5_bits_limited);
int strToInt(const char *from_5_bits_limited);
char* strCmb(int num,char *p1,...); 
void strclr(char *p);
void strarrayclr(char arr[],int num);
int strCpy(char * dest,const char *from);

int strSame(const char* t1,const char* t2);

int findStrFromSTT(const char* seek_for);
bool_type isAllExistedST_T();
char* analyzeGradeOfAllStu(sub subject);

//基本算法 
int strSame(const char* t1,const char* t2){
	char gap=*t1-*t2;
	while(!gap&&*t1&&*t2){
		t1++;
		t2++;
		gap=*t1-*t2;
	}
	return gap;
}
int strCpy(char * dest,const char *from){
	while((*dest++=*from++)!='\0') ;
	
	return SUCCESS;
}
char* strCmb(int count,char *p1,...){
	char* init=p1;
	int i=0;
	va_list ap;
	char *new_p=NULL;
	if(count<2||p1==NULL) return NULL;
	while(*init) init++;//移动到\0 
	va_start(ap,p1);
	for(i=1;i<count;i++){
		new_p=va_arg(ap,char *);	
		while(*new_p!='\0'){
			*init=*new_p;
			init++;
			new_p++;
		}

	}
	*init='\0';
	va_end(ap);
	
	return p1;
}
int charToInt(char c){
	if(c=='0') return 0;
	else if(c=='1') return 1;
	else if(c=='2') return 2;
	else if(c=='3') return 3;
	else if(c=='4') return 4;
	else if(c=='5') return 5;
	else if(c=='6') return 6;
	else if(c=='7') return 7;
	else if(c=='8') return 8;
	else if(c=='9') return 9;
	else return FAIL;
}
char intToChar(int i){
	if(i==0) return '0';
	else if(i==1) return '1';
	else if(i==2) return '2';
	else if(i==3) return '3';
	else if(i==4) return '4';
	else if(i==5) return '5';
	else if(i==6) return '6';
	else if(i==7) return '7';
	else if(i==8) return '8';
	else if(i==9) return '9';
	else return '\0';
}
double strToDouble(double * dst,const char from[7]){
	int dotpos=0,i=0;
	int len=0;//字符串实际长度不含\0
	int left[3],right[2];//最先最靠近小数点
	for(i=0;i<3;i++)
		left[i]=0;
	for(i=0;i<2;i++)
		right[i]=0; 
	for(i=0;i<=5&&from[i]!='\0';i++){
		if(from[i]=='.') dotpos=i;
		len++;//len-1为下标 
	}
	if(dotpos==0) dotpos=len;//3
	for(i=dotpos-1;i>=0;i--){
		left[dotpos-1-i] = charToInt(from[i]);
	}
	if(dotpos==len){//没有小数点
		right[0]=right[1]=0;
	}else if(len-1-dotpos==1) right[0]=charToInt(from[dotpos+1]);
	else{
		right[0]=charToInt(from[dotpos+1]);
		right[1]=charToInt(from[dotpos+2]);
	}
	*dst = left[0]+left[1]*10+left[2]*10*10+right[0]*0.1+right[1]*0.01;
	return *dst;
}
char* doubleToStr(char* to,const double* from){
	int left[4];
	int i;
	int right[2];
	//left[2][1][0] right[0][1]
	//     0  1  2 3      4  5  
	//double:
	//     0
	for(i=0;i<8;i++){
		to[i]='\0';
	}
	left[0]=((int)(*from))%10;
	left[1]=((int)(*from))%100/10;
	left[2]=((int)(*from))%1000/100;
	left[3]=((int)(*from))%10000/1000;
	right[0]=((int)((*from)*10))%10;
	right[1]=((int)((*from)*100))%10;
	if(left[3]!=0){
		to[0] = intToChar(left[3]);
		to[1] = intToChar(left[2]);
		to[2] = intToChar(left[1]);
		to[3] = intToChar(left[0]);
		to[4] = '.';
		to[5] = intToChar(right[0]);
		to[6] = intToChar(right[1]);
		to[7] ='\0';
		
	}
	else if(left[3]==0&&left[2]!=0){
		to[0] = intToChar(left[2]);
		to[1] = intToChar(left[1]);
		to[2] = intToChar(left[0]);
		to[3] = '.';
		to[4] = intToChar(right[0]);
		to[5] = intToChar(right[1]);
		to[6] ='\0';
	}else if(left[2]==0&&left[1]!=0){
		to[0] = intToChar(left[1]);
		to[1] = intToChar(left[0]);
		to[2] = '.';
		to[3] = intToChar(right[0]);
		to[4] = intToChar(right[1]);
		to[5] = '\0';
	}else{
		to[0] = intToChar(left[0]);
		to[1] = '.';
		to[2] = intToChar(right[0]);
		to[3] = intToChar(right[1]);
		to[4] = '\0';
	}
	return to;
}
char* intToStr(char to[6],const int* from){
	int b[5];
	int i=0,j=0,init=0;
	char temp[6];
	for(i=0;i<7;i++){
		to[i]='\0';
	}
	if(*from==0){
		to[0]='0';
		to[1]='\0';
		return to; 
	}
	b[4] = *from%10;
	b[3] = *from%100/10;
	b[2] = *from%1000/100;
	b[1] = *from%10000/1000;
	b[0] = *from%100000/10000;
	for(i=0;i<5;i++)
		to[i] = intToChar(b[i]);
	to[i]='\0';
	for(i=0;i<5;i++){
		if(to[i]!='0'){
			init = i;
			break;
		}
	}
	for(j=0,i=init;to[i]!='\0';i++,j++){
		temp[j] = to[i];
	}
	temp[j]='\0';
	for(i=0;i<6;i++)
		to[i] = temp[i];
	return to;
}
int strToInt(const char *from){//正int 
	int dst[5]={0},i=0,j=0;
	int to=0;
	int bit=0;
	const char *p=from;
	int ten[5];//ten[i]表示10的i次方 
	//str 123
	//
	if(from==NULL||*from=='\0') return FAIL;
	while(*p){
		if(*p<'0'||*p>'9') return FAIL;
		p++;
	}
	
	while(*from&&i<5){
		dst[i]=charToInt(*from);
		from++;i++;bit++;
	}
	ten[0]=1;
	for(i=1;i<5;i++){
		for(j=1,ten[i]=10;j<i;j++){
			ten[i]*=10;
		}
	}
	switch(bit){
		case 1:to=dst[0];break;
		case 2:to=dst[0]*ten[1]+dst[1];break;
		case 3:to=dst[0]*ten[2]+dst[1]*ten[1]+dst[2];break;
		case 4:to=dst[0]*ten[3]+dst[1]*ten[2]+dst[2]*ten[1]+dst[3];break;
		case 5:to=dst[0]*ten[4]+dst[1]*ten[3]+dst[2]*ten[2]+dst[3]*ten[1]+dst[4];break;
	}
	return to;
}
void strclr(char *p){
	while(*p){
		*p='\0';
		p++;
	}
	
}
void strarrayclr(char arr[],int num){
	int i;
	for(i=0;i<num;i++){
		arr[i]='\0';
	}
}

int outOfRange(const char *text,int limit){//包含\0 
	int flag=SURE;//超出
	int i=0;
	//limit <--> text[limit-1]及之前出现\0 
	while(*text){
		i++;//该处为\0跳出 
		text++;
	}
	if(i<limit) flag = DONT;
	return flag; 
}
int getstrlen(const char *text){
	int len=0;
	while(*text){
		text++;
		len++;
	}
	return len;
}
