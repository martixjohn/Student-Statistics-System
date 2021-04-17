/*
    THIS PROGRAM FOLLOWS GPL
    All Rights Reserved.
*/
#include "dec.h"
//函数定义 

//FILE
void fgetALine(char *buf,int num,FILE *fp){//不读\n 
	/*使用请把最后一位留给\0*/
	int i=0;
	if(fp==NULL||feof(fp)!=0) return;//直接是eof
	if(fgetc(fp)==EOF) return;//判断即将读取是否为EOF,fp移动了
	else fseek(fp,-1,SEEK_CUR);//
	for(i=0;i<num;i++)
	{
		*buf=(char)fgetc(fp);
		if(*buf=='\r'||*buf=='\n'||feof(fp)!=0) break;
		buf++;
	}
	if(feof(fp)!=0) return;
	*buf = '\0';//\n替换成\0 
}
long fileToNode(file_type filetype){//前提是文件
	FILE* fp=NULL;
	Data *p1=NULL,*p2=NULL;
	long i=0L,num=0L;
	switch(filetype){
		case FILE_DATA:fp=fopen(URL,"r");break;
		case FILE_DATA_BAK:fp=fopen(URL_BAK,"r");break;
	}
	if(fp==NULL){//无文件
		return 0L;
	}
	for(i=0L;feof(fp)==0;i++,p2=p1){
		p1=(Data *)malloc(sizeof(Data));
		if(i==0L) headOfNode=p2=p1;
		fgetALine(p1->num,11,fp);//读到文件尾会自动识别
		fgetALine(p1->name,21,fp);
		fgetALine(p1->score[0],7,fp);
		fgetALine(p1->score[1],7,fp);
		fgetALine(p1->score[2],7,fp);
		fgetALine(p1->score[3],7,fp);
		if(feof(fp)!=0){//读到文件尾
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
		
		//最后一次fp指向\n 
		p2->next=p1;
	}
	if(i==0&&p1==NULL){//第一次就读取到文件尾
		headOfNode = p2 = NULL;//!!
		return 0;
	}
	num=i;
	if(num>1) sortNode(headOfNode);//排序 
	return num;
}
long nodeToFile(file_type filetype){//前提：链表，来源：输入、备份
	FILE* fp=NULL;
	long num=0L;
	Data *Nhead=headOfNode;
	if(Nhead==NULL) return 0L;//没有数据
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
int initStd(){//初始化设置链表并保存文件 
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
	//先保存到文件 
	//再读取到链表 
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
	long flag=0;//文件有效标志,非负有效 
	if(fp_st==NULL){//设置文件不存在 
		if(initStd()==SUCCESS){
			printf("检测到数据文件不存在，已恢复成功!");
			Sleep(1000);
			system("cls");
			return SUCCESS;
		}else{
			printf("出现未知错误，正在停止...");
			exit(0);
		} 
	};
	fseek(fp_st,0,SEEK_SET); 
	
	//判断文件是否含有所有标题名称 
	if(isAllExistedST_T()==b_false) goto INIT_ST_FILE;
	
	//STANDARD
	title_pos = fp_st_ttitleTurnToPos(fp_st,T_STANDARD);
	
	//外for 
	for(j=1;j<=3;j++){//等级 
			switch(j){
				case 1: level='A';break;
				case 2: level='B';break;
				case 3: level='C';break;
			}
			//内for 
			for(i=0;i<=4;i++){//科目 
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
					
					//滑到对应位置 同时判断是否有效 
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
			//内forEND
			if(flag==FAIL) break; 
	}
	//外forEND
	
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
		if(strSame("\0",setting.subject_name[i])==0){//科目名称不存在视为出错 
			flag=FAIL;
			break;
		}
	}
	flag=fp_st_ctitleTurnToPos(fp_st,C_LENOFLIST,title_pos);
	fgetALine(buf,31,fp_st);
	if((setting.list_len=strToInt(buf))==FAIL) flag=FAIL;
	
	if(flag==FAIL){//文件出错 
INIT_ST_FILE:
		system("cls");
		printf("检查到设置文件数据损坏，正在进行初始化...");
		fclose(fp_st);
		
		Sleep(1000);
		if(initStd()==SUCCESS){
			printf("初始化完成！");
			Sleep(1000);
			system("cls"); 
		}
		else{printf("初始化失败！正在退出程序...");exit(0);}
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
			}//内for 
	}//外for 
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
long fp_st_ttitleTurnToPos(FILE *fp,const char* dst){//标题：指针移动到并返回]位置 
	char buf;
	int i;
	int flag=FAIL;
	char titlename[10];
//	long titlestart=0L;//[ 
	long titleend=0L;//]
	//先重置位置
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
		if(flag!=1){//找不到[ 
			fseek(fp,0,SEEK_SET);
			return FAIL;
		}
		//此时指向[,下一次指向titlestart 
		for(i=0;i<10;i++){
			buf=fgetc(fp);
			titlename[i] = buf;
			//如果文件中 [ 后为 ] ,第一次buf=']'
			titleend++;
			if(buf==']'&&i!=0){
				flag=2;
				break;
			}
		}
		if(flag!=2){//找不到] 或 [后接] 
			fseek(fp,0,SEEK_SET);
			return FAIL;	
		}
		
		//flag=2找到 
		//结束后fp指向] 
		//]->\0
		titlename[i]='\0';
		if(strSame(dst,titlename)==0) return titleend;
	}
	
	return FAIL;
}
long fp_st_ctitleTurnToPos(FILE *fp,const char * dst,long posT){//查找内容标题：返回=位置 
	int i=0,j=0;
	char buf_line[100];
	long len_title;long p1,p2;
	int flag=FAIL;
	
	fseek(fp,posT,SEEK_SET);
	len_title = (long)getstrlen(dst);
	
	while(feof(fp)==0){
		p1=ftell(fp)+1;//title起始 
		fgets(buf_line,41,fp);//可能一排文字过多
		//注意,Windows换行占用2字节\r\n,文件指针经过会移动2字节单位,下一次指向后面 
		p2=ftell(fp);//title结束 
		
		for(i=0;buf_line[i]!='\0'&&i<41;i++)
		{
			if(buf_line[i]=='='){//找到等号
				for(j=i+1;j<41;j++)
				{
					if(buf_line[j]=='=') return FAIL;//有等号直接出错
					if(buf_line[j]=='\n'){//后面找到有回车才有效
						flag=1; 
						break;
					}

				}
				break;
			}
		}
		buf_line[i] = '\0';//\n

		if(flag==FAIL) continue;

		//符合条件
		if(strSame(buf_line,dst)==0){
			fseek(fp,p1+i,SEEK_SET); //注意，即将读写的是下一个
			return ftell(fp);
		}
	}
	//文件结束也没找到 
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
		//数字及.外 
		if((*score>'9'||*score<'0')&&*score!='.') return INVALID;
		//小数点位置不能在第一位，最后一位 
		if(count==1&&*score=='.') return INVALID;
		if(*score=='.') dot=count;
		count++;
		score++;
	}

	if(dot!=0){//有小数点 
	//小数点不在4位及之后及最后一位之前3位之前
		if(dot>4||count-1-dot>2) return INVALID;
	//最后一位不为. 
		if(*(--score)=='.') return INVALID; 
	}else{//无小数点 
		if(count-1>4) return INVALID;
	}

	if(count>7) return INVALID;
	return VALID;
}
Data* findStu(char stunum[11],int list_or_no){//返回指针位置 
	Data *phead=headOfNode;
	Data *flag=NULL;
	int pos=1;
	while(phead!=NULL){
		//找学生 
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
			printf("\n数据库中已查询到该学生信息：\n");
			prtInfo(phead);
		break;
		case DONT:
			printf("\n数据库中已查询到该学生信息。\n");
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
	PrtS("*删除学生*\n1.逐一或批量删除\n2.全部删除");
	scanf("%c",&buf);
	fflush(stdin);
	switch(buf){
		case '1':
			printf("输入待删除的学生ID:");
			gets(trgt);
			if(validID(trgt)==INVALID){
				printf("输入有误，请重新输入！");
				Sleep(1000);
				system("cls");
				deleteNode(p);
			}else{//有效输入 
				if((temp=findStu(trgt,LIST_SURE))!=NULL){//查询到学生 
					PrtS("确认删除?(Y/N)");
					scanf("%c",&buf);fflush(stdin);
					if(buf=='Y'||buf=='y'){
						//查询到前一节点 
						last=p;
						while(last->next!=temp){
							last=last->next;
						}
						last->next=temp->next;
						free(temp);temp=NULL;
						printf("删除成功!");return SUCCESS;
					}else{
						printf("未删除");return FAIL;
					}
				}else{//未找到学生
					printf("未查询到学生信息！");return FAIL;
				}
				
			}
			
			break;
		case '2':
			PrtS("**确认完全删除?(Y/N)**");
			scanf("%c",&buf);fflush(stdin);
			if(buf=='Y'||buf=='y'){
				while(temp){
					temp1 = temp->next;
					free(temp);
					temp = temp1;
				}
				printf("已完全删除!");
				headOfNode=NULL;
				return SUCCESS;
			}else return FAIL;
			break;
		default:
			printf("输入有误");
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
//打印对话框，要求每行不超过100字符，以\n分行 最多20行
void PrtS(const char * text){
	
	int textnum[TEXT_LINE]={0};//每行字符个数
	char textcontent[TEXT_LINE][TEXT_LINE_C];
	int textline=0;
	int i=0,j=0,maxlength=0;
	
	for(i=0;i<TEXT_LINE;i++){
		while(*text!='\n'&&*text){
			textcontent[i][textnum[i]]=*text;
			textnum[i]++;//计算每一行字符长度
			text++;//指针偏移 
			
		}
		textcontent[i][textnum[i]]='\0';
		textline++;
		if(*text=='\0') break;
		text++;
	}
	//保存最大字符数 
	maxlength=textnum[0];
	for(i=1;i<TEXT_LINE;i++){
		if(textnum[i]>maxlength) maxlength = textnum[i];
	}
	//打印开始 
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
	
	strCmb(6,text,"ID: ",p->num,"\n姓名: ",p->name,"\n");
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
	strCmb(10,text,"请选择一项进行修改\n1.姓名\n2.",G1N,"\n3.",G2N,"\n4.",G3N,"\n5.",G4N,"\n6.取消修改");
	PrtS(text);
	printf(">>");scanf("%c",&buf);fflush(stdin);
	switch(buf){
		case '1':
			printf("请输入姓名:");
			scanf("%s",input->name);fflush(stdin);
		break;
		case '2':case'3':case'4':case'5':
			switch(buf){
				case '2':printf("请输入%s:",G1N);i=0;break;
				case '3':printf("请输入%s:",G2N);i=1;break;
				case '4':printf("请输入%s:",G3N);i=2;break;
				case '5':printf("请输入%s:",G4N);i=3;break;
			}
				
RE_INPUT1:		
			scanf("%s",buf_i);fflush(stdin);
			if(validSc(buf_i)==VALID) strCpy(input->score[i],buf_i);
			else{
				printf("输入无效数据(请确保整数位不大于3位，小数位不大于2位)！请重新输入:");
				goto RE_INPUT1;
			}
		break;
		default:
			printf("未进行修改！\n");
			Sleep(1000);
			return flag;
	}
	printf("修改成功!可以继续修改\n");
	Sleep(1000);
	flag=SUCCESS;
	editOne(input);
	return SUCCESS;
}
int addOne(Data *input){//input必须是已创建好的空间
	char buf;
	char buf_sc[10];
	int i;
	Data* temp=NULL;
	printf("\n请输入ID(10位):");
	scanf("%s",input->num);fflush(stdin);
	if(validID(input->num)==INVALID){
		printf("输入无效数据！停止录入...");
		return FAIL;
	}
	if((temp=findStu(input->num,LIST_DONT))!=NULL)//找到相同 
	{
		printf("是否继续修改?(Y/N)");
		printf(">>");
		scanf("%c",&buf);fflush(stdin);
		if(buf!='Y'&&buf!='y'){
			printf("未修改任何内容！\n");
		}else{
			editOne(temp);
		}
		return FAIL;
	}
	printf("\n请为ID为%s的学生录入信息\n请输入姓名:",input->num);
	scanf("%s",input->name);fflush(stdin);
	for(i=0;i<4;i++){
		switch(i){
			case 0:printf("请输入%s:",G1N);break;
			case 1:printf("请输入%s:",G2N);break;
			case 2:printf("请输入%s:",G3N);break;
			case 3:printf("请输入%s:",G4N);break;
			
		}
RE_INPUT:
		scanf("%s",buf_sc);fflush(stdin);
		if(validSc(buf_sc)==VALID) strCpy(input->score[i],buf_sc);
		else{
			printf("输入无效数据(请确保整数位不大于3位，小数位不大于2位)！请重新输入:");
			goto RE_INPUT;
		}
	}
	
	printf("添加成功!");
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
    st_fileToNode();//读取设置文件，自动判定错误 
	
	 
    PrtS("学生数据统计程序\n1.录入学生数据\n2.读取学生数据\n3.统计程序\n4关于帮助\n5.备份与恢复\n6.设置\n7.退出程序\n");
    printf(">>");scanf("%d",&console);
    fflush(stdin); 
    Choose(console);
    return SUCCESS;
}
void inputDataUI() {
	int choose=-1;
	system("cls");

	
	//创建备份
	fileToNode(FILE_DATA);
	nodeToFile(FILE_DATA_BAK);;
	
	PrtS("**录入数据**\n1.批量录入数据\n2.单个录入数据\n3.删除数据\n4.修改数据\n5.返回上一级");

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
		printf("输入有误！重新输入");
		
		Sleep(1000);
		inputDataUI();
	}
	printf("任意键继续...");
	if(getch()) inputDataUI();
	
}
void outputDataUI() {
    FILE* fp = NULL;

	int choose=-1;
	system("cls");
    fp = fopen(URL, "r");//用于判断文件是否存在 

    if(!fp){
    	printf("检测到数据文件不存在!正在返回主菜单...");
    	Sleep(1000);
    	Menu(); 
	}else fclose(fp); 
	fp=NULL;
	fileToNode(FILE_DATA);//文件到链表 

//	TST_prtNode(headOfNode);
	PrtS("**读取学生数据**\n1.列出所有数据\n2.筛选数据\n3.返回上一级");
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
		printf("输入有误！重新输入");
		fclose(fp);
		Sleep(1000);
		outputDataUI();
	}

	
    
}
void aboutUI(){
	system("cls");
	PrtS("**帮助**\n程序数据库文件目录:data\\data.dat\n数据库备份文件:data\\data.bak\n设置文件:data\\data.bak\n当对数据库进行写入前会进行备份操作\n\n**关于**\n作者:Martix_CN");
	if(getch()) Menu();
}
void backupUI(){
	FILE * fp = NULL;
	FILE * fp_bak = NULL;
	Data *data=headOfNode;
	int buf;
	system("cls");
	PrtS("**备份与恢复**\n\n1.备份现有数据库\n2.从备份文件恢复数据库\n3.返回上一级");
	iStream(&buf);
	switch(buf){
		case 1://备份 
			//打开数据库文件 
			if((fp = fopen(URL,"r"))==NULL){//数据库文件不存在 
				printf("数据库文件不存在!");
			}
			else{
				//数据库文件存在 
				fileToNode(FILE_DATA_BAK);//读取至链表 
				nodeToFile(FILE_DATA_BAK);//备份 
				fclose(fp);
				
				printf("\n备份成功!\n");
			}
			
			 
		break;
		case 2://恢复 
			//打开备份文件 
			if((fp_bak=fopen(URL_BAK,"r"))!=NULL){//备份文件存在 
				printf("确定从备份文件恢复数据吗？(Y/N)\n");
				printf(">>");scanf("%c",&buf);fflush(stdin);
				if(buf!='Y'&&buf!='y'){
					printf("未造成修改！");
					fclose(fp_bak);
					backupUI();
				}
				
				fileToNode(FILE_DATA_BAK);//备份读取到链表 
				nodeToFile(FILE_DATA);//链表读取到数据库 
				
				printf("\n恢复备份成功!\n");
			}else{//备份不存在 
				printf("\n不存在备份文件！\n");
			}
		break;
		case 3:Menu();break;
		default:
			printf("\n输入有误！\n");		
	}
	Sleep(1000);
	backupUI(); 
}
void statisticsUI(){
	int choose;
	system("cls");
	fileToNode(FILE_DATA);
	saveSta(headOfNode);//进行统计，保存于链表 
	PrtS("**统计程序**\n1.总体情况\n2.具体查询\n3.返回上一级\n");
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
		printf("\n输入有误！\n");Sleep(1000);
	}
	
	statisticsUI();
}
void settingUI(){//待完成
	int choose;

	system("cls");
	PrtS("**设置**\n1.设置或查看分数标准\n2.自定义科目名称\n3.设置每页显示学生个数\n4.返回上一级");
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
			printf("输入有误，请重新输入");
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
        printf("输入有误，请重新输入");
        Sleep(2000);
        Menu();
    }
}

//outputDataUI
int listData(int way){//从链表读取并列出数据，方式自选 
	char buf[11];
	Data *phead=headOfNode;
	Data *p=phead;
	int pagechange;
	int all=0,now=0,rest=0;
	int list_num=setting.list_len;//一页列出的节点数
	int this_page_pos=0;
	system("cls"); 
	
	if(way!=ALL&&way!=SINGLE) return FAIL;
	if(phead==NULL){
		printf("\n**数据库中无信息！**\n");
		Sleep(1000);
		return FAIL;
	}//节点数量一定不少于1
	switch(way)
	{
		case ALL:
			//计算总数 
			while(p){
				all++;
				p->pos=all;
				p=p->next;
			}
			p=phead;//指向第一个 
			now=1;//当前位置 
			while(1){
				
				rest=all-now;//剩余个数
				//记录页面的第几个
				if(now%list_num>0)
					this_page_pos=now%list_num;//当前页面的第几个
				else this_page_pos=list_num;
				//记录结束

				if(all<=list_num){//不需要分页
					prtInfo(p);
					if(now==all){//p=NULL
						printf("\n\n按ESC返回");
						
						while(1){
							if(getch()==0x1b) return SUCCESS;
						}
					}
					p=p->next;
					now++;//记录当前位置
				}else{//需要分页

						if(this_page_pos==1&&now<all){//这页第一且不为最后一个
							system("cls");
							prtInfo(p);
							p=p->next;
							now++;
						}else if(this_page_pos==list_num || now==all){//这页第list_num或节点最后一个
							if(this_page_pos==1){
								system("cls");//若为第一个
							}
							prtInfo(p);
							
							if(now==list_num)        printf("\n\n             (ESC退出)     下一页->\n");//位于第一页
							else{
								if(now<all)
								                     printf("\n\n<-上一页     (ESC退出)     下一页->\n");
								else if(now==all) 
								                     printf("\n\n<-上一页     (ESC退出)\n");
							}
							printf("%17d/%-17d\n",now/all+1,all/list_num+1); 
							
RE_KEYBOARD:
							pagechange=getch();
							if(pagechange==0||pagechange==224) pagechange=getch();
							switch(pagechange){
							case 72:case 75://上一页 
								if(now==list_num) goto RE_KEYBOARD;//位于第一页
								else{
									p=listData_findList(previous_list,now,list_num);
									now-=this_page_pos+list_num-1;
								}
								
								break;
							case 80:case 77://下一页
								if(now!=all){//可以下一页
									p=listData_findList(next_list,now,list_num);
									now+=list_num-this_page_pos+1;
								}
								else goto RE_KEYBOARD;//没有下一页，重新输入
								break;
							case 0x1b: 
								return SUCCESS;
								break;
							default:
								goto RE_KEYBOARD;//按其他键重新录入按键
							}
							/*case 72: printf("The key you Pressed is : ↑ \n");break;  
							case 80: printf("The key you Pressed is : ↓ \n");break; 
	             			case 75: printf("The key you Pressed is : ← \n");break;
	             			case 77: printf("The key you Pressed is : → \n");break;  
							*/
						}
						else{//这页中间
							prtInfo(p);p=p->next;now++;
						}
						
					
				}//分页END
				
				
			}//while END
			
			return SUCCESS;
		break;
		case SINGLE:
			printf("请输入查询的学生ID(10位):\n");
			gets(buf);
			fflush(stdin);
			if(findStu(buf,LIST_SURE)==NULL) printf("\n数据库中未查询到该生信息！\n");
			printf("\n\n任意键继续...");
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
int saveData(int way){//录入信息至链表，方式自选 
	Data *add=NULL;
	Data *previous=NULL;
	Data *temp=headOfNode;
	char buf[12],buf1;
	system("cls");
	
	
	//跳转到最后一个节点
	if(temp!=NULL)
		while(temp->next){ 
			temp=temp->next;
		}
	switch(way){
		case MASS:
			PrtS("批量录入\n输入不合法id即可停止录入");
			while(1){
				
				add=(Data *)malloc(sizeof(Data));
				if(addOne(add)==SUCCESS){
					if(temp==NULL)
						headOfNode=temp=add;
					
					add->next = NULL;
					temp->next = add;//上一个节点temp
					temp = add;//移动节点至新建
					
				}else{
					temp->next = NULL;
					free(add);add=NULL;
					break;
				}
				
			}
			
		break;
		case SINGLE:
			PrtS("单个录入"); 
			add=(Data *)malloc(sizeof(Data));
			if(addOne(add)==SUCCESS){
				if(temp==NULL)
					headOfNode=temp=add;
				temp->next=add;
				add->next=NULL;
				
			}else{free(add);add=NULL;}
			
		break;
		case DELETE_MENU:
			PrtS("--正在进行删除操作，是否继续？(Y/N)--\n");
			scanf("%c",&buf1);
			fflush(stdin);
			if(buf1!='Y'&&buf1!='y') return FAIL;
			else deleteNode(headOfNode);
		break;
		case EDIT_ONE:
			PrtS("修改信息"); 
			printf("请输入要修改的学生ID:");
			gets(buf);fflush(stdin);
			if(validID(buf)==VALID){//输入符合规范 
				if((temp=findStu(buf,LIST_SURE))!=NULL){
					editOne(temp);
					
					return SUCCESS;
				}else{
					printf("未查询到该生信息！");
					return FAIL;
				}
			}
			else{
				printf("输入有误!");
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
	PrtS("总体情况统计信息");
	showSta(S_ALL);
	showSta(S_S1);
	showSta(S_S2);
	showSta(S_S3);
	showSta(S_S4);
	printf("\n1.将详细结果保存在本地\n2.返回上一级\n>>");
	iStream(&choose);
	switch(choose){
		case 1:
			if(staToFile(ALL_SUB)==FAIL) printf("保存失败...");
			else printf("保存成功！结果已保存到本地目录。");
		break;
		case 2:return SUCCESS;break;
		default: statisticsAll();
	}
	printf("\n按任意键继续...");
	if(getch()) return SUCCESS;
	//GUI END
	return SUCCESS;
}
int statisticsSingle(){
	char text[300]={'\0'};
	int choose_sub,choose;
	/*
	各科及成绩排名 
	*/ 
	system("cls");
	strCmb(10,text,"统计详情\n\n1.",G1N,"\n2.",G2N,"\n3.",G3N,"\n4.",G4N,"\n5.总分详情");
	strCmb(2,text,"\n6.返回上一级\n");
	PrtS(text);

	iStream(&choose_sub);
	if(choose_sub!=6){
		system("cls");
		showSta_detail(choose_sub);
		printf("\n1.将详细结果保存在本地\n2.返回上一级\n>>");
		iStream(&choose);
		switch(choose){
		case 1:
			if(staToFile(choose_sub)==FAIL) printf("保存失败...");
			else printf("保存成功！结果已保存到本地目录。");
		break;
		case 2:return SUCCESS;break;
		default: statisticsSingle();
	}
	}
	else{
		return SUCCESS;
	}
	printf("\n按任意键继续...");
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
	
	//各等级人数
	int stu[5][4];
	
	//该科目所有学生分数和
	double score[5]; 
	double m_score[5][2]; 
	//初始化为0 
	for(i=0;i<5;i++)
	{
		for(j=0;j<4;j++){
			stu[i][j]=0;
		}
		score[i]=0;
	}
	while(pD){
		
		for(i=0;i<5;i++){
			//realscore[i],第i科成绩
			//realsocre[4]总分
			if(count==0||pD->realscore[i]>m_score[i][0]) m_score[i][0] = pD->realscore[i];//MAX_SCORE
			if(count==0||pD->realscore[i]<m_score[i][1]) m_score[i][1] = pD->realscore[i];//MIN
			if(pD->realscore[i]>=setting.level_C[i]){
				stu[i][2]++;//及格人数 
				if(pD->realscore[i]>=setting.level_A[i]) stu[i][0]++;
				if(pD->realscore[i]>=setting.level_B[i]) stu[i][1]++;
			}
			else stu[i][3]++;//不及格人数 
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
	char text[500]={'\0'};//临时存放显示文字 
	char num[7];//临时存数据
	int choose=way-1;
	switch(way){
		case 5:
			strCmb(4,text,"学生人数：",intToStr(num,&statis.stu_num),"\n总分情况\n");

			break;
		case 1:
			
			strCmb(4,text,"科目：",G1N,"\n");
			break;
		case 2:
			strCmb(4,text,"科目：",G2N,"\n");
			break;
		case 3:
			strCmb(4,text,"科目：",G3N,"\n");
			break;
		case 4:
			strCmb(4,text,"科目：",G4N,"\n");
			break;
		
	}
	strCmb(3,text,"优秀线上线人数：",intToStr(num,&statis.level_num[choose][0]));
	strCmb(4,text,"     优秀线比例：",doubleToStr(num,&statis.rate[choose][0]),"\n");
	strCmb(3,text,"良好线上线人数：",intToStr(num,&statis.level_num[choose][1]));
	strCmb(4,text,"     良好线比例：",doubleToStr(num,&statis.rate[choose][1]),"\n");
	strCmb(3,text,"及格线上线人数：",intToStr(num,&statis.level_num[choose][2]));
	strCmb(4,text,"     及格比例：",doubleToStr(num,&statis.rate[choose][2]),"\n");
	strCmb(3,text,"未及格人数：",intToStr(num,&statis.level_num[choose][3]));
	strCmb(4,text,"     未及格比例：",doubleToStr(num,&statis.rate[choose][3]),"\n");
	strCmb(4,text,"\n","学生平均成绩：",doubleToStr(num,&statis.aver_sc[choose]));
	PrtS(text);
	return SUCCESS;
}
int showSta_detail(int way){
	char text[600]={'\0'};//临时存放显示文字 
	char num[7];//临时存数据
	int choose=way-1;
	switch(way){
		case S_ALL:
			strCmb(4,text,"学生人数：",intToStr(num,&statis.stu_num),"\n总分情况\n");
			break;
		case S_S1:
			strCmb(4,text,"科目：",G1N,"\n");
			break;
		case 2:
			strCmb(4,text,"科目：",G2N,"\n");
			break;
		case 3:
			strCmb(4,text,"科目：",G3N,"\n");
			break;
		case 4:
			strCmb(4,text,"科目：",G4N,"\n");
			break;
		
	}
	strCmb(3,text,"优秀线上线人数：",intToStr(num,&statis.level_num[choose][0]));
	strCmb(4,text,"     优秀线比例：",doubleToStr(num,&statis.rate[choose][0]),"\n");
	strCmb(3,text,"良好线上线人数：",intToStr(num,&statis.level_num[choose][1]));
	strCmb(4,text,"     良好线比例：",doubleToStr(num,&statis.rate[choose][1]),"\n");
	strCmb(3,text,"及格线上线人数：",intToStr(num,&statis.level_num[choose][2]));
	strCmb(4,text,"     及格比例：",doubleToStr(num,&statis.rate[choose][2]),"\n");
	strCmb(3,text,"未及格人数：",intToStr(num,&statis.level_num[choose][3]));
	strCmb(4,text,"     未及格比例：",doubleToStr(num,&statis.rate[choose][3]),"\n");
	strCmb(4,text,"\n","学生平均成绩：",doubleToStr(num,&statis.aver_sc[choose]));
	strCmb(3,text,"\n最高分：",doubleToStr(num,&statis.m_score[choose][0]));
	strCmb(3,text,"\n最低分：",doubleToStr(num,&statis.m_score[choose][1]));
	if(way==S_ALL)
		strCmb(3,text,"\n总体情况评级：",analyzeGradeOfAllStu(s_all));
	else
		strCmb(3,text,"\n总体情况评级：",analyzeGradeOfAllStu(choose));
	PrtS(text);
	return SUCCESS;
}
int staToFile(int type){//统计数据保存至文件，待完成 

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
	//确定文件名称
	switch(type){
		case ALL_SUB:strCpy(type_str,"总体情况");break;
		case S_TOTAL:strCpy(type_str,"总分详情");break;
		default:strCpy(type_str,setting.subject_name[type-1]);break;
		
	}
	for(i=0;i<5;i++){
		if(i<4) strCmb(3,time_text,intToStr(time_buf[i],tr_time+i),"-");
		else strCmb(2,time_text,intToStr(time_buf[i],tr_time+i));
	}

	strCmb(5,file_name,time_text,"-",type_str,"成绩报告.txt"); 
	
	if((fp=fopen(file_name,"w+"))==NULL) return FAIL;
	strCmb(5,fputs_text,"=====学生统计成绩详情结果报告====","\n报告生成时间：",time_text,"\n\n");
	fputs(fputs_text,fp);
	strarrayclr(fputs_text,500);
	if(type!=ALL_SUB){
		choose = type-1;
		if(type==S_TOTAL) strCmb(4,fputs_text,"\n【",type_str,"】\n");
		else strCmb(4,fputs_text,"\n\n学科:【",type_str,"】\n");
		strCmb(3,fputs_text,"优秀线上线人数：",intToStr(num,&statis.level_num[choose][0]));
		strCmb(3,fputs_text,"\n优秀线比例：",doubleToStr(num,&statis.rate[choose][0]));
		strCmb(3,fputs_text,"\n良好线上线人数：",intToStr(num,&statis.level_num[choose][1]));
		strCmb(3,fputs_text,"\n良好线比例：",doubleToStr(num,&statis.rate[choose][1]));
		strCmb(3,fputs_text,"\n及格线上线人数：",intToStr(num,&statis.level_num[choose][2]));
		strCmb(3,fputs_text,"\n及格比例：",doubleToStr(num,&statis.rate[choose][2]));
		strCmb(3,fputs_text,"\n未及格人数：",intToStr(num,&statis.level_num[choose][3]));
		strCmb(3,fputs_text,"\n未及格比例：",doubleToStr(num,&statis.rate[choose][3]));
		strCmb(3,fputs_text,"\n学生平均成绩：",doubleToStr(num,&statis.aver_sc[choose]));
		strCmb(3,fputs_text,"\n最高分：",doubleToStr(num,&statis.m_score[choose][0]));
		strCmb(3,fputs_text,"\n最低分：",doubleToStr(num,&statis.m_score[choose][1]));
		strCmb(3,fputs_text,"\n总体情况评级：",analyzeGradeOfAllStu(choose));
		fputs(fputs_text,fp);
		strarrayclr(fputs_text,500);

	}else{//ALL_SUB
		strCmb(3,fputs_text,"\n\n【总体情况分析结果】","\n");
		for(i=0;i<5;i++){
			choose = i;
			type = choose+1;
			
			if(type==S_TOTAL) strCmb(2,fputs_text,"\n\n【总分详情】\n");
			else{
				strCpy(type_str,setting.subject_name[type-1]);
				strCmb(4,fputs_text,"\n\n学科:【",type_str,"】\n");		
			}


			strCmb(3,fputs_text,"优秀线上线人数：",intToStr(num,&statis.level_num[choose][0]));
			strCmb(3,fputs_text,"\n优秀线比例：",doubleToStr(num,&statis.rate[choose][0]));
			strCmb(3,fputs_text,"\n良好线上线人数：",intToStr(num,&statis.level_num[choose][1]));
			strCmb(3,fputs_text,"\n良好线比例：",doubleToStr(num,&statis.rate[choose][1]));
			strCmb(3,fputs_text,"\n及格线上线人数：",intToStr(num,&statis.level_num[choose][2]));
			strCmb(3,fputs_text,"\n及格比例：",doubleToStr(num,&statis.rate[choose][2]));
			strCmb(3,fputs_text,"\n未及格人数：",intToStr(num,&statis.level_num[choose][3]));
			strCmb(3,fputs_text,"\n未及格比例：",doubleToStr(num,&statis.rate[choose][3]));
			strCmb(3,fputs_text,"\n学生平均成绩：",doubleToStr(num,&statis.aver_sc[choose]));
			strCmb(3,fputs_text,"\n最高分：",doubleToStr(num,&statis.m_score[choose][0]));
			strCmb(3,fputs_text,"\n最低分：",doubleToStr(num,&statis.m_score[choose][1]));
			strCmb(3,fputs_text,"\n总体情况评级：",analyzeGradeOfAllStu(choose));
			fputs(fputs_text,fp);
			strarrayclr(fputs_text,500);
		}
	}
	
	fclose(fp);
	fp=NULL;
	return SUCCESS;
}
char* analyzeGradeOfAllStu(sub subject){
	//根据aver_sc和level
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
	strCmb(3,text,"**设置或查看分数标准**\n1.",setting.subject_name[0]);
	strCmb(3,text,"\n2.",setting.subject_name[1]);
	strCmb(3,text,"\n3.",setting.subject_name[2]);
	strCmb(4,text,"\n4.",setting.subject_name[3],"\n5.总分\n6.返回上一级");
	PrtS(text);
	iStream(&choose);
	if(choose>6||choose<1){
		printf("输入有误，请重新输入");
		Sleep(1000);
		settingLevel();
	}
	else if(choose==6) settingUI(); 
	else{
		if(setLevel(choose)==SUCCESS){
			st_nodeToFile();
			printf("修改成功。");
			
		}
		else printf("未造成修改。");
		printf("按任意键继续...");
		if(getch()) settingLevel();
	}	
}
void settingName(){
	char text[500]={'\0'};
	int choose;
	int flag=FAIL;
	char name[32]; 
	system("cls");
	strCmb(3,text,"**自定义科目名称**\n科目1.",setting.subject_name[0]);
	strCmb(3,text,"\n科目2.",setting.subject_name[1]);
	strCmb(3,text,"\n科目3.",setting.subject_name[2]);
	strCmb(4,text,"\n科目4.",setting.subject_name[3],"\n5.返回上一级");
	PrtS(text);
	printf("已列出当前名称，请输入需要修改的项目");iStream(&choose);
	if(choose>5||choose<1){
		printf("输入有误，请重新输入");
		Sleep(1000);
		settingName();
	}
	else if(choose==5) settingUI(); 
	else{
RE_NAME:
		printf("请输入修改后的名字：");
		scanf("%s",name);fflush(stdin);
		if(getstrlen(name)>30){//名字不规范 
			printf("名字太长，请重新输入");
			goto RE_NAME;
		}else{
			strCpy(setting.subject_name[choose-1],name);
			flag=SUCCESS;
		}
	}
	if(flag==SUCCESS){
		st_nodeToFile();
		printf("修改成功。");		
	}
	else printf("未造成修改。");
	printf("按任意键继续...");
	if(getch()) settingName();
}
int setLevel(int choose){
	int buf = choose-1;
	char text_level[10]={'\0'};
	int editl;
	char text[400]={'\0'};
	double value;
	system("cls");
	if(choose==S_ALL) strCmb(3,text,"总成绩","\n目前的分数标准为:\n");
	else strCmb(3,text,setting.subject_name[buf],"\n目前的分数标准为:\n");
	strCmb(3,text,"\n1.优秀:",doubleToStr(text_level,&setting.level_A[buf]));
	strCmb(3,text,"\n2.良好:",doubleToStr(text_level,&setting.level_B[buf]));
	strCmb(3,text,"\n3.合格:",doubleToStr(text_level,&setting.level_C[buf]));
	strCmb(2,text,"\n4.返回上一级");
	PrtS(text);printf("\n请输入你要修改的内容:"); 
	iStream(&editl);
	
	switch(editl){
		case 1:case 2:case 3:
			printf("请输入修改后的值(不超过1000)：");
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
	PrtS("**设置每页显示的学生数目**\n");
	printf("\n当前为：%d\n",setting.list_len);
	printf("是否修改?(Y/N)\n>>");
	scanf("%c",&buf);fflush(stdin);
	if(buf=='y'||buf=='Y'){
		printf("请设置新的显示数目：");
		scanf("%d",&len);
		if(len>0&&len<1000){
			setting.list_len=len;
			if(st_nodeToFile()==SUCCESS){
				printf("设置成功！\n");
				printf("\n按任意键返回...");
				if(getch()) Menu();
			}else{
				printf("设置失败。\n");
				printf("\n按任意键返回...");
				if(getch()) Menu();
			}
		}else{
				printf("数值设置不合理，设置失败。\n");
				printf("\n按任意键返回...");
				if(getch()) Menu();
		}
		
	}
	else{
		printf("\n按任意键返回...");
		if(getch()) Menu();
	}
}

/*旧代码，只用文件不借助结构体*/
/*int listData(FILE *fp,int way){
	
	char buf[21];
	int posl=1;//记录目前所在或即将读的行数，默认为1 
	//单行 
	int i = 0;
	system("cls");
	//注意，最开始文件指针指向了第一行，第一次fgets变成第二行起始
	switch(way){
		case ALL:
			while (fgets(buf,21,fp) != NULL) {
				switch(POS(posl)){
					case NO:printf("ID:");break;
					case NAME:printf("NAME:");break;

					case G1:printf("第一科成绩:");break;
					case G2:printf("第二科成绩:");break;
					case G3:printf("第三科成绩:");break;
					case G4:printf("第四科成绩:");break;
					
					default: exit(0);
				}
				printf("%s  ",buf); 
				if(POS(posl)==G4) printf("\n");
				posl++;
			}
		break;
		case SINGLE://查找单个
			printf("输入查询的学号:");
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
			printf("请输入添加的学生ID:");
			scanf("%c",stu.num);fflush(stdin);
			printf("\n");
			if((idpos=findStu(stu.num,SURE))==SUCCESS){
				printf("\n**数据库中已有该学生，该学生详情已列出**\n");
				printf("是否继续对其进行修改？(Y/N)"); 
				scanf("%c",&buf);fflush(stdin);
				switch(buf){
					case'Y':
						PrtS("请输入需要修改的内容项目：\ni.ID\nn.姓名\n1.第一科成绩\n2.第二科成绩\n3.第三科成绩\n4.第4科成绩");
						scanf("%c",buf);fflush(stdin);
						editStu(idpos,buf);
					break;
					default: printf("提示：不对其进行修改...");
				}
			}
		break;
		case ALL:
			
		break;
	}
	printf("\n\n任意键继续...");
	if(getch()) outputData();
	return 0;
}*/
/*int findStu(char stunum[11],int list_or_no){
	FILE * fp = fopen(URL,"r");
	char buf[21];
	int flag=-1;//-1:未找到学号ID，1找到
	Data stu;
	int i=0;
	int idpos=1;
	strCpy(stu.num,stunum); 
	while(fgets(buf,21,fp)!=NULL){//查找相同学号 
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
				
				printf("\n\nID为%s的学生信息\n姓名：%s\n",stu.num,stu.name);
				for(i=0;i<4;i++){
					printf("第");
					switch(i){
						case 0:printf("一");break;
						case 1:printf("二");break;
						case 2:printf("三");break;
						case 3:printf("四");break;
					}
					printf("科成绩为:%s",stu.score[i]);
					
				}
				fclose(fp);
				return idpos;
				
			}else{
				printf("未找到！");
				fclose(fp);
				return NOT_FOUND;
			}	
			
		break;
	}
}*/
