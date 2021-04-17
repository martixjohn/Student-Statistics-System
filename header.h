/*
    THIS PROGRAM FOLLOWS GPL
    All Rights Reserved.
*/
#include "dec.h"
//�������� 

//FILE
void fgetALine(char *buf,int num,FILE *fp){//����\n 
	/*ʹ��������һλ����\0*/
	int i=0;
	if(fp==NULL||feof(fp)!=0) return;//ֱ����eof
	if(fgetc(fp)==EOF) return;//�жϼ�����ȡ�Ƿ�ΪEOF,fp�ƶ���
	else fseek(fp,-1,SEEK_CUR);//
	for(i=0;i<num;i++)
	{
		*buf=(char)fgetc(fp);
		if(*buf=='\r'||*buf=='\n'||feof(fp)!=0) break;
		buf++;
	}
	if(feof(fp)!=0) return;
	*buf = '\0';//\n�滻��\0 
}
long fileToNode(file_type filetype){//ǰ�����ļ�
	FILE* fp=NULL;
	Data *p1=NULL,*p2=NULL;
	long i=0L,num=0L;
	switch(filetype){
		case FILE_DATA:fp=fopen(URL,"r");break;
		case FILE_DATA_BAK:fp=fopen(URL_BAK,"r");break;
	}
	if(fp==NULL){//���ļ�
		return 0L;
	}
	for(i=0L;feof(fp)==0;i++,p2=p1){
		p1=(Data *)malloc(sizeof(Data));
		if(i==0L) headOfNode=p2=p1;
		fgetALine(p1->num,11,fp);//�����ļ�β���Զ�ʶ��
		fgetALine(p1->name,21,fp);
		fgetALine(p1->score[0],7,fp);
		fgetALine(p1->score[1],7,fp);
		fgetALine(p1->score[2],7,fp);
		fgetALine(p1->score[3],7,fp);
		if(feof(fp)!=0){//�����ļ�β
			free(p1);p1=NULL;//!!!
			if(i>0L) p2->next=NULL;
			break;
		}
		strToDouble(&p1->realscore[0],p1->score[0]);
		strToDouble(&p1->realscore[1],p1->score[1]);
		strToDouble(&p1->realscore[2],p1->score[2]);
		strToDouble(&p1->realscore[3],p1->score[3]);
		p1->realscore[4]=p1->realscore[0]+p1->realscore[1]+p1->realscore[2]+p1->realscore[3];
		p1->aver=p1->realscore[4]/4;
		
		//���һ��fpָ��\n 
		p2->next=p1;
	}
	if(i==0&&p1==NULL){//��һ�ξͶ�ȡ���ļ�β
		headOfNode = p2 = NULL;//!!
		return 0;
	}
	num=i;
	if(num>1) sortNode(headOfNode);//���� 
	return num;
}
long nodeToFile(file_type filetype){//ǰ�᣺������Դ�����롢����
	FILE* fp=NULL;
	long num=0L;
	Data *Nhead=headOfNode;
	if(Nhead==NULL) return 0L;//û������
	switch(filetype){
		case FILE_DATA:fp=fopen(URL,"w+");break;
		case FILE_DATA_BAK:fp=fopen(URL_BAK,"w+");break;
	}
	sortNode(Nhead);
	fseek(fp,0,SEEK_SET);
	while(Nhead){
		fputs(Nhead->num,fp);fputc('\n',fp);
		fputs(Nhead->name,fp);fputc('\n',fp);
		fputs(Nhead->score[0],fp);fputc('\n',fp);
		fputs(Nhead->score[1],fp);fputc('\n',fp);
		fputs(Nhead->score[2],fp);fputc('\n',fp);
		fputs(Nhead->score[3],fp);fputc('\n',fp);
		num++;
		Nhead=Nhead->next;
	}
	fclose(fp);
	return num;
}
int initStd(){//��ʼ���������������ļ� 
	FILE *fp_st=fopen(URL_S,"w+");
	fseek(fp_st,0,SEEK_SET);
	fputs("[STANDARD]\n",fp_st);
	fputs(C_SUBJECT1_A"="S1A"\n"C_SUBJECT2_A"="S2A"\n"C_SUBJECT3_A"="S3A"\n"C_SUBJECT4_A"="S4A"\n"C_ALL_A"="ALLA"\n",fp_st);
	fputs(C_SUBJECT1_B"="S2B"\n"C_SUBJECT2_B"="S3B"\n"C_SUBJECT3_B"="S3B"\n"C_SUBJECT4_B"="S4B"\n"C_ALL_B"="ALLB"\n",fp_st);
	fputs(C_SUBJECT1_C"="S1C"\n"C_SUBJECT2_C"="S3C"\n"C_SUBJECT3_C"="S3C"\n"C_SUBJECT4_C"="S4C"\n"C_ALL_C"="ALLC"\n",fp_st);

	fputs("[SETTING]\n",fp_st);
	fputs(C_SUBJECT1_NAME"="S1N"\n",fp_st);
	fputs(C_SUBJECT2_NAME"="S2N"\n",fp_st);
	fputs(C_SUBJECT3_NAME"="S3N"\n",fp_st);
	fputs(C_SUBJECT4_NAME"="S4N"\n",fp_st);
	fputs(C_LENOFLIST"="LEN_OF_LIST"\n",fp_st);
	fseek(fp_st,0,SEEK_SET);
	fclose(fp_st);
	//�ȱ��浽�ļ� 
	//�ٶ�ȡ������ 
	st_fileToNode();
	return SUCCESS; 
}
int st_fileToNode(){
	FILE *fp_st=fopen(URL_S,"r");
	char buf[20];
	int i=0,j=0;
	char level='\0';
	char * temp=NULL;
	long title_pos=0L;
	long flag=0;//�ļ���Ч��־,�Ǹ���Ч 
	if(fp_st==NULL){//�����ļ������� 
		if(initStd()==SUCCESS){
			printf("��⵽�����ļ������ڣ��ѻָ��ɹ�!");
			Sleep(1000);
			system("cls");
			return SUCCESS;
		}else{
			printf("����δ֪��������ֹͣ...");
			exit(0);
		} 
	};
	fseek(fp_st,0,SEEK_SET); 
	
	//�ж��ļ��Ƿ������б������� 
	if(isAllExistedST_T()==b_false) goto INIT_ST_FILE;
	
	//STANDARD
	title_pos = fp_st_ttitleTurnToPos(fp_st,T_STANDARD);
	
	//��for 
	for(j=1;j<=3;j++){//�ȼ� 
			switch(j){
				case 1: level='A';break;
				case 2: level='B';break;
				case 3: level='C';break;
			}
			//��for 
			for(i=0;i<=4;i++){//��Ŀ 
					switch(level){
						case 'A':
							switch(i){
								case 0: temp=C_SUBJECT1_A; break;
								case 1: temp=C_SUBJECT2_A; break;
								case 2: temp=C_SUBJECT3_A; break;
								case 3: temp=C_SUBJECT4_A; break;
								case 4: temp=C_ALL_A;break;
							}
						break;
						case 'B':
							switch(i){
								case 0: temp=C_SUBJECT1_B; break;
								case 1: temp=C_SUBJECT2_B; break;
								case 2: temp=C_SUBJECT3_B; break;
								case 3: temp=C_SUBJECT4_B; break;
								case 4: temp=C_ALL_B;break;
							}
						break;
						case 'C':
							switch(i){
								case 0: temp=C_SUBJECT1_C; break;
								case 1: temp=C_SUBJECT2_C; break;
								case 2: temp=C_SUBJECT3_C; break;
								case 3: temp=C_SUBJECT4_C; break;
								case 4: temp=C_ALL_C;break;
							}
						break;
					}
					
					//������Ӧλ�� ͬʱ�ж��Ƿ���Ч 
					if(fp_st_ctitleTurnToPos(fp_st,temp,title_pos)==FAIL){
						flag=FAIL;break;
					}
					fgetALine(buf,20,fp_st);
					switch(level){
						case 'A':strToDouble(&setting.level_A[i],buf);break;
						case 'B':strToDouble(&setting.level_B[i],buf);break;
						case 'C':strToDouble(&setting.level_C[i],buf);break;
					}
				
			}
			//��forEND
			if(flag==FAIL) break; 
	}
	//��forEND
	
	if(flag==FAIL) goto INIT_ST_FILE;

	//SETTING
	title_pos = fp_st_ttitleTurnToPos(fp_st,T_SETTING);
	for(i=0;i<=3;i++){
		switch(i){
			case 0:flag=fp_st_ctitleTurnToPos(fp_st,C_SUBJECT1_NAME,title_pos);break;
			case 1:flag=fp_st_ctitleTurnToPos(fp_st,C_SUBJECT2_NAME,title_pos);break;
			case 2:flag=fp_st_ctitleTurnToPos(fp_st,C_SUBJECT3_NAME,title_pos);break;
			case 3:flag=fp_st_ctitleTurnToPos(fp_st,C_SUBJECT4_NAME,title_pos);break;
		}
		fgetALine(buf,31,fp_st);
		strCpy(setting.subject_name[i],buf);
	}
	for(i=0;i<4;i++){
		if(strSame("\0",setting.subject_name[i])==0){//��Ŀ���Ʋ�������Ϊ���� 
			flag=FAIL;
			break;
		}
	}
	flag=fp_st_ctitleTurnToPos(fp_st,C_LENOFLIST,title_pos);
	fgetALine(buf,31,fp_st);
	if((setting.list_len=strToInt(buf))==FAIL) flag=FAIL;
	
	if(flag==FAIL){//�ļ����� 
INIT_ST_FILE:
		system("cls");
		printf("��鵽�����ļ������𻵣����ڽ��г�ʼ��...");
		fclose(fp_st);
		
		Sleep(1000);
		if(initStd()==SUCCESS){
			printf("��ʼ����ɣ�");
			Sleep(1000);
			system("cls"); 
		}
		else{printf("��ʼ��ʧ�ܣ������˳�����...");exit(0);}
	}
	fclose(fp_st);
	return SUCCESS;
}
int st_nodeToFile(){
	FILE *fp_st=fopen(URL_S,"w+");
	char temp[31]={'\0'};
	int i=0,j=0;
	char level='\0';
	fseek(fp_st,0,SEEK_SET);
	fputs("[STANDARD]\n",fp_st);
	//STANDARD
	for(j=1;j<=3;j++){
			switch(j){
					case 1: level='A';break;
					case 2: level='B';break;
					case 3: level='C';break;
			}
		
			for(i=0;i<5;i++){
				switch(level){
					case 'A':
						switch(i){
							case 0:fputs(C_SUBJECT1_A"=",fp_st);break;
							case 1:fputs(C_SUBJECT2_A"=",fp_st);break;
							case 2:fputs(C_SUBJECT3_A"=",fp_st);break;
							case 3:fputs(C_SUBJECT4_A"=",fp_st);break;
							case 4:fputs(C_ALL_A"=",fp_st);break;
						}	
						fputs(doubleToStr(temp,&setting.level_A[i]),fp_st);
					break;
					case 'B':
						switch(i){
							case 0:fputs(C_SUBJECT1_B"=",fp_st);break;
							case 1:fputs(C_SUBJECT2_B"=",fp_st);break;
							case 2:fputs(C_SUBJECT3_B"=",fp_st);break;
							case 3:fputs(C_SUBJECT4_B"=",fp_st);break;
							case 4:fputs(C_ALL_B"=",fp_st);break;
						}
						fputs(doubleToStr(temp,&setting.level_B[i]),fp_st);
					break;
					case 'C':
						switch(i){
							case 0:fputs(C_SUBJECT1_C"=",fp_st);break;
							case 1:fputs(C_SUBJECT2_C"=",fp_st);break;
							case 2:fputs(C_SUBJECT3_C"=",fp_st);break;
							case 3:fputs(C_SUBJECT4_C"=",fp_st);break;
							case 4:fputs(C_ALL_C"=",fp_st);break;
						}
						fputs(doubleToStr(temp,&setting.level_C[i]),fp_st);
					break;
					
				}
				
				
				fputc('\n',fp_st);
			}//��for 
	}//��for 
	fputs("[SETTING]\n",fp_st);
	for(i=0;i<4;i++){
		switch(i){
			case 0:strCpy(temp,C_SUBJECT1_NAME"=");break;
			case 1:strCpy(temp,C_SUBJECT2_NAME"=");break;
			case 2:strCpy(temp,C_SUBJECT3_NAME"=");break;
			case 3:strCpy(temp,C_SUBJECT4_NAME"=");break;
		}
		fputs(temp,fp_st);
		fputs(setting.subject_name[i],fp_st);
		fputc('\n',fp_st);
	}

	fputs(C_LENOFLIST"=",fp_st);
	fputs(intToStr(temp,&setting.list_len),fp_st);
	fputc('\n',fp_st);
	fclose(fp_st);
	return SUCCESS;
}
long fp_st_ttitleTurnToPos(FILE *fp,const char* dst){//���⣺ָ���ƶ���������]λ�� 
	char buf;
	int i;
	int flag=FAIL;
	char titlename[10];
//	long titlestart=0L;//[ 
	long titleend=0L;//]
	//������λ��
	fseek(fp,0,SEEK_SET); 
	while(feof(fp)==0){
		while(feof(fp)==0){
			buf=fgetc(fp);
			titleend++;
			if(buf=='['){
				flag=1;
				break;
			}
		}
		if(flag!=1){//�Ҳ���[ 
			fseek(fp,0,SEEK_SET);
			return FAIL;
		}
		//��ʱָ��[,��һ��ָ��titlestart 
		for(i=0;i<10;i++){
			buf=fgetc(fp);
			titlename[i] = buf;
			//����ļ��� [ ��Ϊ ] ,��һ��buf=']'
			titleend++;
			if(buf==']'&&i!=0){
				flag=2;
				break;
			}
		}
		if(flag!=2){//�Ҳ���] �� [���] 
			fseek(fp,0,SEEK_SET);
			return FAIL;	
		}
		
		//flag=2�ҵ� 
		//������fpָ��] 
		//]->\0
		titlename[i]='\0';
		if(strSame(dst,titlename)==0) return titleend;
	}
	
	return FAIL;
}
long fp_st_ctitleTurnToPos(FILE *fp,const char * dst,long posT){//�������ݱ��⣺����=λ�� 
	int i=0,j=0;
	char buf_line[100];
	long len_title;long p1,p2;
	int flag=FAIL;
	
	fseek(fp,posT,SEEK_SET);
	len_title = (long)getstrlen(dst);
	
	while(feof(fp)==0){
		p1=ftell(fp)+1;//title��ʼ 
		fgets(buf_line,41,fp);//����һ�����ֹ���
		//ע��,Windows����ռ��2�ֽ�\r\n,�ļ�ָ�뾭�����ƶ�2�ֽڵ�λ,��һ��ָ����� 
		p2=ftell(fp);//title���� 
		
		for(i=0;buf_line[i]!='\0'&&i<41;i++)
		{
			if(buf_line[i]=='='){//�ҵ��Ⱥ�
				for(j=i+1;j<41;j++)
				{
					if(buf_line[j]=='=') return FAIL;//�еȺ�ֱ�ӳ���
					if(buf_line[j]=='\n'){//�����ҵ��лس�����Ч
						flag=1; 
						break;
					}

				}
				break;
			}
		}
		buf_line[i] = '\0';//\n

		if(flag==FAIL) continue;

		//��������
		if(strSame(buf_line,dst)==0){
			fseek(fp,p1+i,SEEK_SET); //ע�⣬������д������һ��
			return ftell(fp);
		}
	}
	//�ļ�����Ҳû�ҵ� 
	return FAIL;
}
int findStrFromSTT(const char* seek_for){
//C_SUBJECT_LEVEL:
			if(strSame(seek_for,C_SUBJECT1_A)==0) return FOUND;
			else if(strSame(seek_for,C_SUBJECT1_B)==0) return FOUND;
			else if(strSame(seek_for,C_SUBJECT1_C)==0) return FOUND;
			else if(strSame(seek_for,C_SUBJECT2_A)==0) return FOUND;
			else if(strSame(seek_for,C_SUBJECT2_B)==0) return FOUND;
			else if(strSame(seek_for,C_SUBJECT2_C)==0) return FOUND;
			else if(strSame(seek_for,C_SUBJECT3_A)==0) return FOUND;
			else if(strSame(seek_for,C_SUBJECT3_B)==0) return FOUND;
			else if(strSame(seek_for,C_SUBJECT3_C)==0) return FOUND;
			else if(strSame(seek_for,C_SUBJECT4_A)==0) return FOUND;
			else if(strSame(seek_for,C_SUBJECT4_B)==0) return FOUND;
			else if(strSame(seek_for,C_SUBJECT4_C)==0) return FOUND;
			else if(strSame(seek_for,C_ALL_A)==0) return FOUND;
			else if(strSame(seek_for,C_ALL_B)==0) return FOUND;
			else if(strSame(seek_for,C_ALL_C)==0) return FOUND;
			
//C_SUBJECT_NAME:
			else if(strSame(seek_for,C_SUBJECT1_NAME)==0) return FOUND;
			else if(strSame(seek_for,C_SUBJECT2_NAME)==0) return FOUND;
			else if(strSame(seek_for,C_SUBJECT3_NAME)==0) return FOUND;
			else if(strSame(seek_for,C_SUBJECT4_NAME)==0) return FOUND;
			else return NOT_FOUND;
}
bool_type isAllExistedST_T(){
	FILE *fp_st=fopen(URL_S,"r");
	int tcount=0;
	long pos = 0L;
	bool_type ret=b_false;
	if(fp_st==NULL) return b_false;
	
	if((pos=fp_st_ttitleTurnToPos(fp_st,T_STANDARD))==FAIL) goto isExistedST_T_END;
	else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT1_A,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT2_A,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT3_A,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT4_A,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_ALL_A,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT1_B,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT2_B,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT3_B,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT4_B,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_ALL_B,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT1_C,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT2_C,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT3_C,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT4_C,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_ALL_C,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		
	if((pos=fp_st_ttitleTurnToPos(fp_st,T_SETTING))==FAIL) goto isExistedST_T_END;
	else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT1_NAME,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT2_NAME,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT3_NAME,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_SUBJECT4_NAME,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

		if(fp_st_ctitleTurnToPos(fp_st,C_LENOFLIST,pos)==FAIL) goto isExistedST_T_END;
		else tcount++;

isExistedST_T_END:
	
	fclose(fp_st);
	
	if(tcount==ST_TT_NUM+ST_CT_NUM) ret=b_true;
	return ret;
}
void cpyNodeExpNext(Data *dst,const Data *from){
	int i,j;
	for(i=0;i<11;i++)
		dst->num[i]=from->num[i];
	for(i=0;i<21;i++)
		dst->name[i]=from->name[i];
	
	for(i=0;i<4;i++){
		for(j=0;j<7;j++){
			dst->score[i][j]=from->score[i][j];
		}
	}
	
}
void swapNode(Data *p1,Data *p2){
	Data temp;
	cpyNodeExpNext(&temp,p1);
	cpyNodeExpNext(p1,p2);
	cpyNodeExpNext(p2,&temp);
}
void sortNode(Data *Nhead){
	Data *p=Nhead;
	int i,j,count=0;
	while(p){
		count++;
		p=p->next;
	}
	p=Nhead;
	if(p==NULL||p->next==NULL) return;
	for(i=0;i<count-1;i++){//count>=2
		for(p=Nhead,j=0;j<count-i-1;j++,p=p->next){
			if(strSame(p->num,p->next->num)>0){
				swapNode(p,p->next);
			}
		}
	}
}
int validID(const char *id){
	int limit_len=10;
	if(getstrlen(id)==limit_len){
		while(*id<='9'&&*id>='0'){
			id++;
		}
		if(*id=='\0') return VALID;
	}
	return INVALID;
} 
int validSc(const char *score){
	int count=1;
	int dot=0;
	while(*score){
		//���ּ�.�� 
		if((*score>'9'||*score<'0')&&*score!='.') return INVALID;
		//С����λ�ò����ڵ�һλ�����һλ 
		if(count==1&&*score=='.') return INVALID;
		if(*score=='.') dot=count;
		count++;
		score++;
	}

	if(dot!=0){//��С���� 
	//С���㲻��4λ��֮�����һλ֮ǰ3λ֮ǰ
		if(dot>4||count-1-dot>2) return INVALID;
	//���һλ��Ϊ. 
		if(*(--score)=='.') return INVALID; 
	}else{//��С���� 
		if(count-1>4) return INVALID;
	}

	if(count>7) return INVALID;
	return VALID;
}
Data* findStu(char stunum[11],int list_or_no){//����ָ��λ�� 
	Data *phead=headOfNode;
	Data *flag=NULL;
	int pos=1;
	while(phead!=NULL){
		//��ѧ�� 
		if(strSame(stunum,phead->num)==0){
			flag = phead;
			break;	
		}
		pos++;
		phead=phead->next;
	}
	if(flag==NULL){
		return NULL;
	}
	switch(list_or_no){
		case SURE:
			printf("\n���ݿ����Ѳ�ѯ����ѧ����Ϣ��\n");
			prtInfo(phead);
		break;
		case DONT:
			printf("\n���ݿ����Ѳ�ѯ����ѧ����Ϣ��\n");
		break;
	}
	return flag;
	
}



int deleteNode(){
	char buf;
	char trgt[12];
	Data * p=headOfNode;
	Data * temp=NULL,*last=NULL,*temp1=NULL;
	system("cls");
	PrtS("*ɾ��ѧ��*\n1.��һ������ɾ��\n2.ȫ��ɾ��");
	scanf("%c",&buf);
	fflush(stdin);
	switch(buf){
		case '1':
			printf("�����ɾ����ѧ��ID:");
			gets(trgt);
			if(validID(trgt)==INVALID){
				printf("�����������������룡");
				Sleep(1000);
				system("cls");
				deleteNode(p);
			}else{//��Ч���� 
				if((temp=findStu(trgt,LIST_SURE))!=NULL){//��ѯ��ѧ�� 
					PrtS("ȷ��ɾ��?(Y/N)");
					scanf("%c",&buf);fflush(stdin);
					if(buf=='Y'||buf=='y'){
						//��ѯ��ǰһ�ڵ� 
						last=p;
						while(last->next!=temp){
							last=last->next;
						}
						last->next=temp->next;
						free(temp);temp=NULL;
						printf("ɾ���ɹ�!");return SUCCESS;
					}else{
						printf("δɾ��");return FAIL;
					}
				}else{//δ�ҵ�ѧ��
					printf("δ��ѯ��ѧ����Ϣ��");return FAIL;
				}
				
			}
			
			break;
		case '2':
			PrtS("**ȷ����ȫɾ��?(Y/N)**");
			scanf("%c",&buf);fflush(stdin);
			if(buf=='Y'||buf=='y'){
				while(temp){
					temp1 = temp->next;
					free(temp);
					temp = temp1;
				}
				printf("����ȫɾ��!");
				headOfNode=NULL;
				return SUCCESS;
			}else return FAIL;
			break;
		default:
			printf("��������");
			Sleep(1000);
			return FAIL;
					
	}
	return SUCCESS;
}

void TST_prtNode(Data *phead){
	while(phead!=NULL){
		printf("%s",phead->num);
		phead=phead->next;
	}
}
//��ӡ�Ի���Ҫ��ÿ�в�����100�ַ�����\n���� ���20��
void PrtS(const char * text){
	
	int textnum[TEXT_LINE]={0};//ÿ���ַ�����
	char textcontent[TEXT_LINE][TEXT_LINE_C];
	int textline=0;
	int i=0,j=0,maxlength=0;
	
	for(i=0;i<TEXT_LINE;i++){
		while(*text!='\n'&&*text){
			textcontent[i][textnum[i]]=*text;
			textnum[i]++;//����ÿһ���ַ�����
			text++;//ָ��ƫ�� 
			
		}
		textcontent[i][textnum[i]]='\0';
		textline++;
		if(*text=='\0') break;
		text++;
	}
	//��������ַ��� 
	maxlength=textnum[0];
	for(i=1;i<TEXT_LINE;i++){
		if(textnum[i]>maxlength) maxlength = textnum[i];
	}
	//��ӡ��ʼ 
	printf("\n");
	for(i=0;i<maxlength*2;i++){
		printf("*");
	} 
	printf("\n");
	for(i=0;i<textline;i++){
		printf("*");
		for(j=0;j<(maxlength*2-textnum[i]-2)/2;j++){
			printf("-");
		}
		printf("%s",textcontent[i]);
		for(j=0;j<(maxlength*2-textnum[i]-2)/2;j++){
			printf("-");
		}
		printf("*\n");
		
	}
	for(i=0;i<maxlength*2;i++){
		printf("*");
	}
	printf("\n"); 
	
}
void prtInfo(const Data *p){
	char text[400]={'\0'};
	
	strCmb(6,text,"ID: ",p->num,"\n����: ",p->name,"\n");
	strCmb(16,text,G1N,":",p->score[0],"\n",G2N,":",p->score[1],"\n",G3N,":",p->score[2],"\n",G4N,":",p->score[3]);
	PrtS(text);
}
int editOne(Data *input){
	char buf;
	char text[400]={'\0'};
	char buf_i[11];
	int i;
	int flag=FAIL; 
	system("cls");
	prtInfo(input);
	strCmb(10,text,"��ѡ��һ������޸�\n1.����\n2.",G1N,"\n3.",G2N,"\n4.",G3N,"\n5.",G4N,"\n6.ȡ���޸�");
	PrtS(text);
	printf(">>");scanf("%c",&buf);fflush(stdin);
	switch(buf){
		case '1':
			printf("����������:");
			scanf("%s",input->name);fflush(stdin);
		break;
		case '2':case'3':case'4':case'5':
			switch(buf){
				case '2':printf("������%s:",G1N);i=0;break;
				case '3':printf("������%s:",G2N);i=1;break;
				case '4':printf("������%s:",G3N);i=2;break;
				case '5':printf("������%s:",G4N);i=3;break;
			}
				
RE_INPUT1:		
			scanf("%s",buf_i);fflush(stdin);
			if(validSc(buf_i)==VALID) strCpy(input->score[i],buf_i);
			else{
				printf("������Ч����(��ȷ������λ������3λ��С��λ������2λ)������������:");
				goto RE_INPUT1;
			}
		break;
		default:
			printf("δ�����޸ģ�\n");
			Sleep(1000);
			return flag;
	}
	printf("�޸ĳɹ�!���Լ����޸�\n");
	Sleep(1000);
	flag=SUCCESS;
	editOne(input);
	return SUCCESS;
}
int addOne(Data *input){//input�������Ѵ����õĿռ�
	char buf;
	char buf_sc[10];
	int i;
	Data* temp=NULL;
	printf("\n������ID(10λ):");
	scanf("%s",input->num);fflush(stdin);
	if(validID(input->num)==INVALID){
		printf("������Ч���ݣ�ֹͣ¼��...");
		return FAIL;
	}
	if((temp=findStu(input->num,LIST_DONT))!=NULL)//�ҵ���ͬ 
	{
		printf("�Ƿ�����޸�?(Y/N)");
		printf(">>");
		scanf("%c",&buf);fflush(stdin);
		if(buf!='Y'&&buf!='y'){
			printf("δ�޸��κ����ݣ�\n");
		}else{
			editOne(temp);
		}
		return FAIL;
	}
	printf("\n��ΪIDΪ%s��ѧ��¼����Ϣ\n����������:",input->num);
	scanf("%s",input->name);fflush(stdin);
	for(i=0;i<4;i++){
		switch(i){
			case 0:printf("������%s:",G1N);break;
			case 1:printf("������%s:",G2N);break;
			case 2:printf("������%s:",G3N);break;
			case 3:printf("������%s:",G4N);break;
			
		}
RE_INPUT:
		scanf("%s",buf_sc);fflush(stdin);
		if(validSc(buf_sc)==VALID) strCpy(input->score[i],buf_sc);
		else{
			printf("������Ч����(��ȷ������λ������3λ��С��λ������2λ)������������:");
			goto RE_INPUT;
		}
	}
	
	printf("��ӳɹ�!");
	return SUCCESS;
} 


//GUI
int iStream(int* choose){
	printf(">>");scanf("%d",choose);fflush(stdin);
	return *choose;
}
int Menu() {
    int console = 0;
	 
    system("cls");
    CreateDirectory("data",NULL);
    st_fileToNode();//��ȡ�����ļ����Զ��ж����� 
	
	 
    PrtS("ѧ������ͳ�Ƴ���\n1.¼��ѧ������\n2.��ȡѧ������\n3.ͳ�Ƴ���\n4���ڰ���\n5.������ָ�\n6.����\n7.�˳�����\n");
    printf(">>");scanf("%d",&console);
    fflush(stdin); 
    Choose(console);
    return SUCCESS;
}
void inputDataUI() {
	int choose=-1;
	system("cls");

	
	//��������
	fileToNode(FILE_DATA);
	nodeToFile(FILE_DATA_BAK);;
	
	PrtS("**¼������**\n1.����¼������\n2.����¼������\n3.ɾ������\n4.�޸�����\n5.������һ��");

	iStream(&choose);
	fflush(stdin);
	if(choose == 1 || choose == 2 || choose == 3 || choose == 4)
	{
		saveData(choose);
		
		nodeToFile(FILE_DATA);
		
	}
	else if(choose == 5)
	{
		
		Menu();

	}
	else{
		printf("����������������");
		
		Sleep(1000);
		inputDataUI();
	}
	printf("���������...");
	if(getch()) inputDataUI();
	
}
void outputDataUI() {
    FILE* fp = NULL;

	int choose=-1;
	system("cls");
    fp = fopen(URL, "r");//�����ж��ļ��Ƿ���� 

    if(!fp){
    	printf("��⵽�����ļ�������!���ڷ������˵�...");
    	Sleep(1000);
    	Menu(); 
	}else fclose(fp); 
	fp=NULL;
	fileToNode(FILE_DATA);//�ļ������� 

//	TST_prtNode(headOfNode);
	PrtS("**��ȡѧ������**\n1.�г���������\n2.ɸѡ����\n3.������һ��");
	iStream(&choose);
	fflush(stdin);
	if(choose == 1 || choose == 2){
		listData(choose);
		outputDataUI();
	}
		
	else if(choose == 3){
		Menu();
	}
		
	else{
		printf("����������������");
		fclose(fp);
		Sleep(1000);
		outputDataUI();
	}

	
    
}
void aboutUI(){
	system("cls");
	PrtS("**����**\n�������ݿ��ļ�Ŀ¼:data\\data.dat\n���ݿⱸ���ļ�:data\\data.bak\n�����ļ�:data\\data.bak\n�������ݿ����д��ǰ����б��ݲ���\n\n**����**\n����:Martix_CN");
	if(getch()) Menu();
}
void backupUI(){
	FILE * fp = NULL;
	FILE * fp_bak = NULL;
	Data *data=headOfNode;
	int buf;
	system("cls");
	PrtS("**������ָ�**\n\n1.�����������ݿ�\n2.�ӱ����ļ��ָ����ݿ�\n3.������һ��");
	iStream(&buf);
	switch(buf){
		case 1://���� 
			//�����ݿ��ļ� 
			if((fp = fopen(URL,"r"))==NULL){//���ݿ��ļ������� 
				printf("���ݿ��ļ�������!");
			}
			else{
				//���ݿ��ļ����� 
				fileToNode(FILE_DATA_BAK);//��ȡ������ 
				nodeToFile(FILE_DATA_BAK);//���� 
				fclose(fp);
				
				printf("\n���ݳɹ�!\n");
			}
			
			 
		break;
		case 2://�ָ� 
			//�򿪱����ļ� 
			if((fp_bak=fopen(URL_BAK,"r"))!=NULL){//�����ļ����� 
				printf("ȷ���ӱ����ļ��ָ�������(Y/N)\n");
				printf(">>");scanf("%c",&buf);fflush(stdin);
				if(buf!='Y'&&buf!='y'){
					printf("δ����޸ģ�");
					fclose(fp_bak);
					backupUI();
				}
				
				fileToNode(FILE_DATA_BAK);//���ݶ�ȡ������ 
				nodeToFile(FILE_DATA);//�����ȡ�����ݿ� 
				
				printf("\n�ָ����ݳɹ�!\n");
			}else{//���ݲ����� 
				printf("\n�����ڱ����ļ���\n");
			}
		break;
		case 3:Menu();break;
		default:
			printf("\n��������\n");		
	}
	Sleep(1000);
	backupUI(); 
}
void statisticsUI(){
	int choose;
	system("cls");
	fileToNode(FILE_DATA);
	saveSta(headOfNode);//����ͳ�ƣ����������� 
	PrtS("**ͳ�Ƴ���**\n1.�������\n2.�����ѯ\n3.������һ��\n");
	iStream(&choose);
	switch(choose){
		case 1:
			statisticsAll();
		break;
		case 2:
			statisticsSingle();
		break;
		case 3:
			Menu();
		break;
		default:
		printf("\n��������\n");Sleep(1000);
	}
	
	statisticsUI();
}
void settingUI(){//�����
	int choose;

	system("cls");
	PrtS("**����**\n1.���û�鿴������׼\n2.�Զ����Ŀ����\n3.����ÿҳ��ʾѧ������\n4.������һ��");
	iStream(&choose);
	
	switch(choose){
		case 1:
			settingLevel();
			break;
		case 2:
			settingName();
			break; 
		case 3:
			settingListLen();
			break;
		case 4:
			Menu();
			break;
		default:
			printf("������������������");
			Sleep(1000);
			settingUI();
	}
	
}


void Choose(int console) {
    switch (console) {
    case 1:
    	inputDataUI();
        break;
    case 2:
    	outputDataUI();
        break;
	case 3:
		statisticsUI(); 
		break;
	case 4:
		aboutUI();
		break;
	case 5:
		backupUI();
		break;
	case 6:
		settingUI();
		break; 
	case 7:
		exit(0);
		break;
    default:
        printf("������������������");
        Sleep(2000);
        Menu();
    }
}

//outputDataUI
int listData(int way){//�������ȡ���г����ݣ���ʽ��ѡ 
	char buf[11];
	Data *phead=headOfNode;
	Data *p=phead;
	int pagechange;
	int all=0,now=0,rest=0;
	int list_num=setting.list_len;//һҳ�г��Ľڵ���
	int this_page_pos=0;
	system("cls"); 
	
	if(way!=ALL&&way!=SINGLE) return FAIL;
	if(phead==NULL){
		printf("\n**���ݿ�������Ϣ��**\n");
		Sleep(1000);
		return FAIL;
	}//�ڵ�����һ��������1
	switch(way)
	{
		case ALL:
			//�������� 
			while(p){
				all++;
				p->pos=all;
				p=p->next;
			}
			p=phead;//ָ���һ�� 
			now=1;//��ǰλ�� 
			while(1){
				
				rest=all-now;//ʣ�����
				//��¼ҳ��ĵڼ���
				if(now%list_num>0)
					this_page_pos=now%list_num;//��ǰҳ��ĵڼ���
				else this_page_pos=list_num;
				//��¼����

				if(all<=list_num){//����Ҫ��ҳ
					prtInfo(p);
					if(now==all){//p=NULL
						printf("\n\n��ESC����");
						
						while(1){
							if(getch()==0x1b) return SUCCESS;
						}
					}
					p=p->next;
					now++;//��¼��ǰλ��
				}else{//��Ҫ��ҳ

						if(this_page_pos==1&&now<all){//��ҳ��һ�Ҳ�Ϊ���һ��
							system("cls");
							prtInfo(p);
							p=p->next;
							now++;
						}else if(this_page_pos==list_num || now==all){//��ҳ��list_num��ڵ����һ��
							if(this_page_pos==1){
								system("cls");//��Ϊ��һ��
							}
							prtInfo(p);
							
							if(now==list_num)        printf("\n\n             (ESC�˳�)     ��һҳ->\n");//λ�ڵ�һҳ
							else{
								if(now<all)
								                     printf("\n\n<-��һҳ     (ESC�˳�)     ��һҳ->\n");
								else if(now==all) 
								                     printf("\n\n<-��һҳ     (ESC�˳�)\n");
							}
							printf("%17d/%-17d\n",now/all+1,all/list_num+1); 
							
RE_KEYBOARD:
							pagechange=getch();
							if(pagechange==0||pagechange==224) pagechange=getch();
							switch(pagechange){
							case 72:case 75://��һҳ 
								if(now==list_num) goto RE_KEYBOARD;//λ�ڵ�һҳ
								else{
									p=listData_findList(previous_list,now,list_num);
									now-=this_page_pos+list_num-1;
								}
								
								break;
							case 80:case 77://��һҳ
								if(now!=all){//������һҳ
									p=listData_findList(next_list,now,list_num);
									now+=list_num-this_page_pos+1;
								}
								else goto RE_KEYBOARD;//û����һҳ����������
								break;
							case 0x1b: 
								return SUCCESS;
								break;
							default:
								goto RE_KEYBOARD;//������������¼�밴��
							}
							/*case 72: printf("The key you Pressed is : �� \n");break;  
							case 80: printf("The key you Pressed is : �� \n");break; 
	             			case 75: printf("The key you Pressed is : �� \n");break;
	             			case 77: printf("The key you Pressed is : �� \n");break;  
							*/
						}
						else{//��ҳ�м�
							prtInfo(p);p=p->next;now++;
						}
						
					
				}//��ҳEND
				
				
			}//while END
			
			return SUCCESS;
		break;
		case SINGLE:
			printf("�������ѯ��ѧ��ID(10λ):\n");
			gets(buf);
			fflush(stdin);
			if(findStu(buf,LIST_SURE)==NULL) printf("\n���ݿ���δ��ѯ��������Ϣ��\n");
			printf("\n\n���������...");
			if(getch()) return SUCCESS;
		
		break;
	}

	return SUCCESS;
}
Data* listData_findList(list_pos next_or_previous,int this_pos,int list_num){
	Data *p=headOfNode;
	Data *dst=NULL;
	int thispage_pos;
	int dstpos;
	
	if(this_pos%list_num==0) thispage_pos = list_num;
	else thispage_pos=this_pos%list_num;
	
	switch(next_or_previous){
		case previous_list:
			dstpos=this_pos-list_num-thispage_pos+1;
			break;
		case next_list:
			dstpos=this_pos+list_num-thispage_pos+1;
			break;
	}
	//find
	while(p){
		if(p->pos==dstpos){
			dst=p;
			break;
		}
		p=p->next;
	}
	return dst;
}
//inputDataUI
int saveData(int way){//¼����Ϣ��������ʽ��ѡ 
	Data *add=NULL;
	Data *previous=NULL;
	Data *temp=headOfNode;
	char buf[12],buf1;
	system("cls");
	
	
	//��ת�����һ���ڵ�
	if(temp!=NULL)
		while(temp->next){ 
			temp=temp->next;
		}
	switch(way){
		case MASS:
			PrtS("����¼��\n���벻�Ϸ�id����ֹͣ¼��");
			while(1){
				
				add=(Data *)malloc(sizeof(Data));
				if(addOne(add)==SUCCESS){
					if(temp==NULL)
						headOfNode=temp=add;
					
					add->next = NULL;
					temp->next = add;//��һ���ڵ�temp
					temp = add;//�ƶ��ڵ����½�
					
				}else{
					temp->next = NULL;
					free(add);add=NULL;
					break;
				}
				
			}
			
		break;
		case SINGLE:
			PrtS("����¼��"); 
			add=(Data *)malloc(sizeof(Data));
			if(addOne(add)==SUCCESS){
				if(temp==NULL)
					headOfNode=temp=add;
				temp->next=add;
				add->next=NULL;
				
			}else{free(add);add=NULL;}
			
		break;
		case DELETE_MENU:
			PrtS("--���ڽ���ɾ���������Ƿ������(Y/N)--\n");
			scanf("%c",&buf1);
			fflush(stdin);
			if(buf1!='Y'&&buf1!='y') return FAIL;
			else deleteNode(headOfNode);
		break;
		case EDIT_ONE:
			PrtS("�޸���Ϣ"); 
			printf("������Ҫ�޸ĵ�ѧ��ID:");
			gets(buf);fflush(stdin);
			if(validID(buf)==VALID){//������Ϲ淶 
				if((temp=findStu(buf,LIST_SURE))!=NULL){
					editOne(temp);
					
					return SUCCESS;
				}else{
					printf("δ��ѯ��������Ϣ��");
					return FAIL;
				}
			}
			else{
				printf("��������!");
				return FAIL;
			}	
			
		break;
	}
	 
	return SUCCESS;
}
//statisticsUI
int statisticsAll(){
	int choose;
	//GUI
	system("cls");
	PrtS("�������ͳ����Ϣ");
	showSta(S_ALL);
	showSta(S_S1);
	showSta(S_S2);
	showSta(S_S3);
	showSta(S_S4);
	printf("\n1.����ϸ��������ڱ���\n2.������һ��\n>>");
	iStream(&choose);
	switch(choose){
		case 1:
			if(staToFile(ALL_SUB)==FAIL) printf("����ʧ��...");
			else printf("����ɹ�������ѱ��浽����Ŀ¼��");
		break;
		case 2:return SUCCESS;break;
		default: statisticsAll();
	}
	printf("\n�����������...");
	if(getch()) return SUCCESS;
	//GUI END
	return SUCCESS;
}
int statisticsSingle(){
	char text[300]={'\0'};
	int choose_sub,choose;
	/*
	���Ƽ��ɼ����� 
	*/ 
	system("cls");
	strCmb(10,text,"ͳ������\n\n1.",G1N,"\n2.",G2N,"\n3.",G3N,"\n4.",G4N,"\n5.�ܷ�����");
	strCmb(2,text,"\n6.������һ��\n");
	PrtS(text);

	iStream(&choose_sub);
	if(choose_sub!=6){
		system("cls");
		showSta_detail(choose_sub);
		printf("\n1.����ϸ��������ڱ���\n2.������һ��\n>>");
		iStream(&choose);
		switch(choose){
		case 1:
			if(staToFile(choose_sub)==FAIL) printf("����ʧ��...");
			else printf("����ɹ�������ѱ��浽����Ŀ¼��");
		break;
		case 2:return SUCCESS;break;
		default: statisticsSingle();
	}
	}
	else{
		return SUCCESS;
	}
	printf("\n�����������...");
	if(getch()) statisticsSingle();
	return SUCCESS;
}

int saveSta(const Data *pD){
	int count=0,i=0,j=0;
	/*
	int s1_A=0,s1_B=0,s1_C=0;D 
	int s2_A=0,s2_B=0,s2_C=0;D
	int s3_A=0,s3_B=0,s3_C=0;D
	int s4_A=0,s4_B=0,s4_C=0;D
	int all_A...
	*/
	
	//���ȼ�����
	int stu[5][4];
	
	//�ÿ�Ŀ����ѧ��������
	double score[5]; 
	double m_score[5][2]; 
	//��ʼ��Ϊ0 
	for(i=0;i<5;i++)
	{
		for(j=0;j<4;j++){
			stu[i][j]=0;
		}
		score[i]=0;
	}
	while(pD){
		
		for(i=0;i<5;i++){
			//realscore[i],��i�Ƴɼ�
			//realsocre[4]�ܷ�
			if(count==0||pD->realscore[i]>m_score[i][0]) m_score[i][0] = pD->realscore[i];//MAX_SCORE
			if(count==0||pD->realscore[i]<m_score[i][1]) m_score[i][1] = pD->realscore[i];//MIN
			if(pD->realscore[i]>=setting.level_C[i]){
				stu[i][2]++;//�������� 
				if(pD->realscore[i]>=setting.level_A[i]) stu[i][0]++;
				if(pD->realscore[i]>=setting.level_B[i]) stu[i][1]++;
			}
			else stu[i][3]++;//���������� 
			score[i]+=pD->realscore[i];
			
		}
		
		
		pD=pD->next;
		count++;
	}
	statis.stu_num=count;
	for(i=0;i<5;i++){
		statis.m_score[i][0]=m_score[i][0];
		statis.m_score[i][1]=m_score[i][1];
		for(j=0;j<4;j++){
			statis.level_num[i][j] = stu[i][j];
		}
	}
	for(i=0;i<5;i++){//SUBJECT
		statis.aver_sc[i]=score[i]/statis.stu_num;
		for(j=0;j<4;j++)
			statis.rate[i][j]=(double)stu[i][j]/statis.stu_num;
	}
	
		
		
	return SUCCESS;
}
int showSta(int way){
	char text[500]={'\0'};//��ʱ�����ʾ���� 
	char num[7];//��ʱ������
	int choose=way-1;
	switch(way){
		case 5:
			strCmb(4,text,"ѧ��������",intToStr(num,&statis.stu_num),"\n�ܷ����\n");

			break;
		case 1:
			
			strCmb(4,text,"��Ŀ��",G1N,"\n");
			break;
		case 2:
			strCmb(4,text,"��Ŀ��",G2N,"\n");
			break;
		case 3:
			strCmb(4,text,"��Ŀ��",G3N,"\n");
			break;
		case 4:
			strCmb(4,text,"��Ŀ��",G4N,"\n");
			break;
		
	}
	strCmb(3,text,"����������������",intToStr(num,&statis.level_num[choose][0]));
	strCmb(4,text,"     �����߱�����",doubleToStr(num,&statis.rate[choose][0]),"\n");
	strCmb(3,text,"����������������",intToStr(num,&statis.level_num[choose][1]));
	strCmb(4,text,"     �����߱�����",doubleToStr(num,&statis.rate[choose][1]),"\n");
	strCmb(3,text,"����������������",intToStr(num,&statis.level_num[choose][2]));
	strCmb(4,text,"     ���������",doubleToStr(num,&statis.rate[choose][2]),"\n");
	strCmb(3,text,"δ����������",intToStr(num,&statis.level_num[choose][3]));
	strCmb(4,text,"     δ���������",doubleToStr(num,&statis.rate[choose][3]),"\n");
	strCmb(4,text,"\n","ѧ��ƽ���ɼ���",doubleToStr(num,&statis.aver_sc[choose]));
	PrtS(text);
	return SUCCESS;
}
int showSta_detail(int way){
	char text[600]={'\0'};//��ʱ�����ʾ���� 
	char num[7];//��ʱ������
	int choose=way-1;
	switch(way){
		case S_ALL:
			strCmb(4,text,"ѧ��������",intToStr(num,&statis.stu_num),"\n�ܷ����\n");
			break;
		case S_S1:
			strCmb(4,text,"��Ŀ��",G1N,"\n");
			break;
		case 2:
			strCmb(4,text,"��Ŀ��",G2N,"\n");
			break;
		case 3:
			strCmb(4,text,"��Ŀ��",G3N,"\n");
			break;
		case 4:
			strCmb(4,text,"��Ŀ��",G4N,"\n");
			break;
		
	}
	strCmb(3,text,"����������������",intToStr(num,&statis.level_num[choose][0]));
	strCmb(4,text,"     �����߱�����",doubleToStr(num,&statis.rate[choose][0]),"\n");
	strCmb(3,text,"����������������",intToStr(num,&statis.level_num[choose][1]));
	strCmb(4,text,"     �����߱�����",doubleToStr(num,&statis.rate[choose][1]),"\n");
	strCmb(3,text,"����������������",intToStr(num,&statis.level_num[choose][2]));
	strCmb(4,text,"     ���������",doubleToStr(num,&statis.rate[choose][2]),"\n");
	strCmb(3,text,"δ����������",intToStr(num,&statis.level_num[choose][3]));
	strCmb(4,text,"     δ���������",doubleToStr(num,&statis.rate[choose][3]),"\n");
	strCmb(4,text,"\n","ѧ��ƽ���ɼ���",doubleToStr(num,&statis.aver_sc[choose]));
	strCmb(3,text,"\n��߷֣�",doubleToStr(num,&statis.m_score[choose][0]));
	strCmb(3,text,"\n��ͷ֣�",doubleToStr(num,&statis.m_score[choose][1]));
	if(way==S_ALL)
		strCmb(3,text,"\n�������������",analyzeGradeOfAllStu(s_all));
	else
		strCmb(3,text,"\n�������������",analyzeGradeOfAllStu(choose));
	PrtS(text);
	return SUCCESS;
}
int staToFile(int type){//ͳ�����ݱ������ļ�������� 

	FILE *fp=NULL;
	time_t t=time(NULL);
	struct tm *local=localtime(&t);
	char time_buf[5][5]={'\0'};
	char type_str[31]={'\0'};
	int i,choose;
	int tr_time[5]={
		local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min
	};
	char fputs_text[500]={'\0'};
	char time_text[50]={'\0'};
	char file_name[50]={'\0'};
	char num[7];
	//ȷ���ļ�����
	switch(type){
		case ALL_SUB:strCpy(type_str,"�������");break;
		case S_TOTAL:strCpy(type_str,"�ܷ�����");break;
		default:strCpy(type_str,setting.subject_name[type-1]);break;
		
	}
	for(i=0;i<5;i++){
		if(i<4) strCmb(3,time_text,intToStr(time_buf[i],tr_time+i),"-");
		else strCmb(2,time_text,intToStr(time_buf[i],tr_time+i));
	}

	strCmb(5,file_name,time_text,"-",type_str,"�ɼ�����.txt"); 
	
	if((fp=fopen(file_name,"w+"))==NULL) return FAIL;
	strCmb(5,fputs_text,"=====ѧ��ͳ�Ƴɼ�����������====","\n��������ʱ�䣺",time_text,"\n\n");
	fputs(fputs_text,fp);
	strarrayclr(fputs_text,500);
	if(type!=ALL_SUB){
		choose = type-1;
		if(type==S_TOTAL) strCmb(4,fputs_text,"\n��",type_str,"��\n");
		else strCmb(4,fputs_text,"\n\nѧ��:��",type_str,"��\n");
		strCmb(3,fputs_text,"����������������",intToStr(num,&statis.level_num[choose][0]));
		strCmb(3,fputs_text,"\n�����߱�����",doubleToStr(num,&statis.rate[choose][0]));
		strCmb(3,fputs_text,"\n����������������",intToStr(num,&statis.level_num[choose][1]));
		strCmb(3,fputs_text,"\n�����߱�����",doubleToStr(num,&statis.rate[choose][1]));
		strCmb(3,fputs_text,"\n����������������",intToStr(num,&statis.level_num[choose][2]));
		strCmb(3,fputs_text,"\n���������",doubleToStr(num,&statis.rate[choose][2]));
		strCmb(3,fputs_text,"\nδ����������",intToStr(num,&statis.level_num[choose][3]));
		strCmb(3,fputs_text,"\nδ���������",doubleToStr(num,&statis.rate[choose][3]));
		strCmb(3,fputs_text,"\nѧ��ƽ���ɼ���",doubleToStr(num,&statis.aver_sc[choose]));
		strCmb(3,fputs_text,"\n��߷֣�",doubleToStr(num,&statis.m_score[choose][0]));
		strCmb(3,fputs_text,"\n��ͷ֣�",doubleToStr(num,&statis.m_score[choose][1]));
		strCmb(3,fputs_text,"\n�������������",analyzeGradeOfAllStu(choose));
		fputs(fputs_text,fp);
		strarrayclr(fputs_text,500);

	}else{//ALL_SUB
		strCmb(3,fputs_text,"\n\n������������������","\n");
		for(i=0;i<5;i++){
			choose = i;
			type = choose+1;
			
			if(type==S_TOTAL) strCmb(2,fputs_text,"\n\n���ܷ����顿\n");
			else{
				strCpy(type_str,setting.subject_name[type-1]);
				strCmb(4,fputs_text,"\n\nѧ��:��",type_str,"��\n");		
			}


			strCmb(3,fputs_text,"����������������",intToStr(num,&statis.level_num[choose][0]));
			strCmb(3,fputs_text,"\n�����߱�����",doubleToStr(num,&statis.rate[choose][0]));
			strCmb(3,fputs_text,"\n����������������",intToStr(num,&statis.level_num[choose][1]));
			strCmb(3,fputs_text,"\n�����߱�����",doubleToStr(num,&statis.rate[choose][1]));
			strCmb(3,fputs_text,"\n����������������",intToStr(num,&statis.level_num[choose][2]));
			strCmb(3,fputs_text,"\n���������",doubleToStr(num,&statis.rate[choose][2]));
			strCmb(3,fputs_text,"\nδ����������",intToStr(num,&statis.level_num[choose][3]));
			strCmb(3,fputs_text,"\nδ���������",doubleToStr(num,&statis.rate[choose][3]));
			strCmb(3,fputs_text,"\nѧ��ƽ���ɼ���",doubleToStr(num,&statis.aver_sc[choose]));
			strCmb(3,fputs_text,"\n��߷֣�",doubleToStr(num,&statis.m_score[choose][0]));
			strCmb(3,fputs_text,"\n��ͷ֣�",doubleToStr(num,&statis.m_score[choose][1]));
			strCmb(3,fputs_text,"\n�������������",analyzeGradeOfAllStu(choose));
			fputs(fputs_text,fp);
			strarrayclr(fputs_text,500);
		}
	}
	
	fclose(fp);
	fp=NULL;
	return SUCCESS;
}
char* analyzeGradeOfAllStu(sub subject){
	//����aver_sc��level
	if(statis.aver_sc[subject] <= setting.level_C[subject]) return sub_sc_level[lv_d];
	else if(statis.aver_sc[subject] < setting.level_B[subject]) return sub_sc_level[lv_c];
	else if(statis.aver_sc[subject] < setting.level_A[subject]) return sub_sc_level[lv_b];
	else return sub_sc_level[lv_a];
	
}
//settingUI
void settingLevel(){
	char text[500]={'\0'};
	int choose;
	FILE *fp_st=NULL; 
	system("cls");
	strCmb(3,text,"**���û�鿴������׼**\n1.",setting.subject_name[0]);
	strCmb(3,text,"\n2.",setting.subject_name[1]);
	strCmb(3,text,"\n3.",setting.subject_name[2]);
	strCmb(4,text,"\n4.",setting.subject_name[3],"\n5.�ܷ�\n6.������һ��");
	PrtS(text);
	iStream(&choose);
	if(choose>6||choose<1){
		printf("������������������");
		Sleep(1000);
		settingLevel();
	}
	else if(choose==6) settingUI(); 
	else{
		if(setLevel(choose)==SUCCESS){
			st_nodeToFile();
			printf("�޸ĳɹ���");
			
		}
		else printf("δ����޸ġ�");
		printf("�����������...");
		if(getch()) settingLevel();
	}	
}
void settingName(){
	char text[500]={'\0'};
	int choose;
	int flag=FAIL;
	char name[32]; 
	system("cls");
	strCmb(3,text,"**�Զ����Ŀ����**\n��Ŀ1.",setting.subject_name[0]);
	strCmb(3,text,"\n��Ŀ2.",setting.subject_name[1]);
	strCmb(3,text,"\n��Ŀ3.",setting.subject_name[2]);
	strCmb(4,text,"\n��Ŀ4.",setting.subject_name[3],"\n5.������һ��");
	PrtS(text);
	printf("���г���ǰ���ƣ���������Ҫ�޸ĵ���Ŀ");iStream(&choose);
	if(choose>5||choose<1){
		printf("������������������");
		Sleep(1000);
		settingName();
	}
	else if(choose==5) settingUI(); 
	else{
RE_NAME:
		printf("�������޸ĺ�����֣�");
		scanf("%s",name);fflush(stdin);
		if(getstrlen(name)>30){//���ֲ��淶 
			printf("����̫��������������");
			goto RE_NAME;
		}else{
			strCpy(setting.subject_name[choose-1],name);
			flag=SUCCESS;
		}
	}
	if(flag==SUCCESS){
		st_nodeToFile();
		printf("�޸ĳɹ���");		
	}
	else printf("δ����޸ġ�");
	printf("�����������...");
	if(getch()) settingName();
}
int setLevel(int choose){
	int buf = choose-1;
	char text_level[10]={'\0'};
	int editl;
	char text[400]={'\0'};
	double value;
	system("cls");
	if(choose==S_ALL) strCmb(3,text,"�ܳɼ�","\nĿǰ�ķ�����׼Ϊ:\n");
	else strCmb(3,text,setting.subject_name[buf],"\nĿǰ�ķ�����׼Ϊ:\n");
	strCmb(3,text,"\n1.����:",doubleToStr(text_level,&setting.level_A[buf]));
	strCmb(3,text,"\n2.����:",doubleToStr(text_level,&setting.level_B[buf]));
	strCmb(3,text,"\n3.�ϸ�:",doubleToStr(text_level,&setting.level_C[buf]));
	strCmb(2,text,"\n4.������һ��");
	PrtS(text);printf("\n��������Ҫ�޸ĵ�����:"); 
	iStream(&editl);
	
	switch(editl){
		case 1:case 2:case 3:
			printf("�������޸ĺ��ֵ(������1000)��");
			printf(">>");scanf("%lf",&value);fflush(stdin);
			if(value>0&&value<1000){
				switch(editl){
					case 1: setting.level_A[buf]=value;break;
					case 2: setting.level_B[buf]=value;break;
					case 3: setting.level_C[buf]=value;break;
				}
				return SUCCESS;
			}
			else{
				return FAIL; 
			}
		break;

		default:
			return FAIL;
	}
	
}
void settingListLen(){
	char buf;int len;
	system("cls");
	PrtS("**����ÿҳ��ʾ��ѧ����Ŀ**\n");
	printf("\n��ǰΪ��%d\n",setting.list_len);
	printf("�Ƿ��޸�?(Y/N)\n>>");
	scanf("%c",&buf);fflush(stdin);
	if(buf=='y'||buf=='Y'){
		printf("�������µ���ʾ��Ŀ��");
		scanf("%d",&len);
		if(len>0&&len<1000){
			setting.list_len=len;
			if(st_nodeToFile()==SUCCESS){
				printf("���óɹ���\n");
				printf("\n�����������...");
				if(getch()) Menu();
			}else{
				printf("����ʧ�ܡ�\n");
				printf("\n�����������...");
				if(getch()) Menu();
			}
		}else{
				printf("��ֵ���ò���������ʧ�ܡ�\n");
				printf("\n�����������...");
				if(getch()) Menu();
		}
		
	}
	else{
		printf("\n�����������...");
		if(getch()) Menu();
	}
}

/*�ɴ��룬ֻ���ļ��������ṹ��*/
/*int listData(FILE *fp,int way){
	
	char buf[21];
	int posl=1;//��¼Ŀǰ���ڻ򼴽�����������Ĭ��Ϊ1 
	//���� 
	int i = 0;
	system("cls");
	//ע�⣬�ʼ�ļ�ָ��ָ���˵�һ�У���һ��fgets��ɵڶ�����ʼ
	switch(way){
		case ALL:
			while (fgets(buf,21,fp) != NULL) {
				switch(POS(posl)){
					case NO:printf("ID:");break;
					case NAME:printf("NAME:");break;

					case G1:printf("��һ�Ƴɼ�:");break;
					case G2:printf("�ڶ��Ƴɼ�:");break;
					case G3:printf("�����Ƴɼ�:");break;
					case G4:printf("���ĿƳɼ�:");break;
					
					default: exit(0);
				}
				printf("%s  ",buf); 
				if(POS(posl)==G4) printf("\n");
				posl++;
			}
		break;
		case SINGLE://���ҵ���
			printf("�����ѯ��ѧ��:");
			gets(buf);
			findStu(buf,LIST_SURE);
		break;
	}
	
	
	fclose(fp);

	return 0;
}
*//*int saveData(FILE *fp,int way){
	Data stu;
	int i=0,idpos=0;
	char buf;
	system("cls");
	switch(way){
		case SINGLE:
			printf("��������ӵ�ѧ��ID:");
			scanf("%c",stu.num);fflush(stdin);
			printf("\n");
			if((idpos=findStu(stu.num,SURE))==SUCCESS){
				printf("\n**���ݿ������и�ѧ������ѧ���������г�**\n");
				printf("�Ƿ������������޸ģ�(Y/N)"); 
				scanf("%c",&buf);fflush(stdin);
				switch(buf){
					case'Y':
						PrtS("��������Ҫ�޸ĵ�������Ŀ��\ni.ID\nn.����\n1.��һ�Ƴɼ�\n2.�ڶ��Ƴɼ�\n3.�����Ƴɼ�\n4.��4�Ƴɼ�");
						scanf("%c",buf);fflush(stdin);
						editStu(idpos,buf);
					break;
					default: printf("��ʾ������������޸�...");
				}
			}
		break;
		case ALL:
			
		break;
	}
	printf("\n\n���������...");
	if(getch()) outputData();
	return 0;
}*/
/*int findStu(char stunum[11],int list_or_no){
	FILE * fp = fopen(URL,"r");
	char buf[21];
	int flag=-1;//-1:δ�ҵ�ѧ��ID��1�ҵ�
	Data stu;
	int i=0;
	int idpos=1;
	strCpy(stu.num,stunum); 
	while(fgets(buf,21,fp)!=NULL){//������ͬѧ�� 
		idpos++;
		if(strSame(buf,stunum)==0){
			flag=1;
			break;
		}
		
	}
	switch(list_or_no){
		case DONT:
			fclose(fp);
			if(flag==1) return idpos;
			else return NOT_FOUND;
		break;
		case SURE:
			if(flag==1){
				fgets(buf,21,fp);
				strCpy(stu.name,buf);
				for(i=0;i<4;i++){
					fgets(buf,21,fp);
					strCpy(stu.score[i],buf);
				}
				
				printf("\n\nIDΪ%s��ѧ����Ϣ\n������%s\n",stu.num,stu.name);
				for(i=0;i<4;i++){
					printf("��");
					switch(i){
						case 0:printf("һ");break;
						case 1:printf("��");break;
						case 2:printf("��");break;
						case 3:printf("��");break;
					}
					printf("�Ƴɼ�Ϊ:%s",stu.score[i]);
					
				}
				fclose(fp);
				return idpos;
				
			}else{
				printf("δ�ҵ���");
				fclose(fp);
				return NOT_FOUND;
			}	
			
		break;
	}
}*/
