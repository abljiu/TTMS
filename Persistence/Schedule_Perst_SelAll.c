//#include "../Service/Schedule.h"
#include "Schedule_Persist.h"
#include "../Common/list.h"
#include "EntityKey_Persist.h"
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <assert.h>

static const char SCHEDULE_DATA_FILE[] = "Schedule.dat"; //�ݳ��ļ������� 
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat";
static const char SCHEDULE_KEY_NAME[] = "Schedule"; //�ݳ������� 
static const char SCHEDULE_SELECT_NAME[] = "Play.dat"; 
//�����ݳ� 

int Schedule_Perst_Insert(schedule_t *data) {	 
	assert(NULL!=data);

	////����������Ʒ���������ӵĴ���
	////����������Ʒ���������ӵĴ���
	long key = EntKey_Perst_GetNewKeys(SCHEDULE_KEY_NAME, 1); //Ϊ���ݳ������ȡ
	if(key<=0)			//��������ʧ�ܣ�ֱ�ӷ���
		return 0;
	data->id = key;		//�����¶�����ص�UI��
	////����������Ʒ���������ӵĴ���
	////����������Ʒ���������ӵĴ���



	FILE *fp = fopen(SCHEDULE_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(schedule_t), 1, fp);

	fclose(fp);
	return rtn;
}
int Schedule_Perst_SelectByPlay( schedule_list_t list, int play_id){
	int reCount = 0;
	schedule_t data;
	schedule_node_t *newnode;
	
	List_Free(list, schedule_node_t);//�����ʼ���������ݳ��ƻ����ܳ��ֳ��� 
	
	FILE *fp = fopen("Schedule.dat", "rb");
	if ( fp == NULL)
	{
		printf("Failed in it\n");
		return reCount;
	}
	while(!feof(fp))
	{
		if (fread(&data, sizeof(schedule_t), 1, fp))
		{
			if (data.play_id == play_id)
			{
				newnode = (schedule_node_t*)malloc( sizeof(schedule_node_t) );
				if( !newnode )
				{
					printf("�����ڴ�ʧ��!");
					break;
				}
				newnode->data = data;
				List_AddTail(list, newnode);
				reCount++;
			}
		}
	} 
	fclose(fp);
	return reCount;
	
}
/*
int Schedule_Perst_Insert( schedule_t *data){
	int rtn = 0;
	
	FILE *fp = fopen("Schedule.dat", "ab");
	if (fp == NULL)
	{
		printf("Cannot open file %s!\n", "Schedule.dat");
		return 0;
	}
	rtn = fwrite(&data, sizeof(schedule_t), 1, fp);
	fclose(fp);
	
	return rtn;
}
*/
int Schedule_Perst_Updata( const schedule_t *data){
	int found = 0;
	schedule_t buf;
	FILE *fp = fopen("Schedule.dat", "rb+");
	if (fp == NULL)
	{
		printf("Cannot open file %s!\n", "Schedule.dat");
		return found;
	}
	
	while( !feof(fp))
	{
		if (fread( &buf, sizeof(schedule_t), 1, fp) )
		{
			if (buf.id == data->id)
			{
				fseek(fp, -((int)sizeof(schedule_t)), SEEK_CUR);
				fwrite(data, sizeof(schedule_t), 1, fp);
				found = 1;
				break;
			}
		}
	}
	fclose(fp);
	
	return found;
}

int Schedule_Perst_RemByID(int id)//ȥ�� 
{
	int found = 0;
	if(rename("Schedule.dat", "ScheduleTmp.dat")<0){
		printf("Cannot open file %s!\n", "Schedule.dat");
		return 0;
	}
	
	FILE *fpsour, *fptmp;
	fpsour = fopen("ScheduleTmp.dat", "rb");
	if( NULL == fpsour)
	{
		printf("Cannot open file %s!\n", "Schedule.dat");
		return 0;
	}
	
	fptmp = fopen("Schedule.dat", "wb");
	if( NULL == fptmp)
	{
		printf("Cannot open file %s!\n", "ScheduleTmp.dat");
		return 0;
	}
	
	schedule_t buf;
	while( !feof(fpsour))
	{
		if( fread(&buf, sizeof(schedule_t), 1, fpsour) );
			if( id == buf.id ){
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(schedule_t), 1, fptmp);
	}
	
	fclose(fpsour);
	fclose(fptmp);
	
	remove("ScheduleTmp.dat");
	return found;
}

int Schedule_Perst_SelectByID(int id, schedule_t *buf) {
	assert(NULL!=buf);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	schedule_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
			if (id == data.id) {
				*buf = data;
				found = 1;
				break;
			};

		}
	}
	fclose(fp);

	return found;
}

//��ѯ�ݳ� 
int Schedule_Perst_SelectAll(schedule_list_t list){
	schedule_node_t *newnode;
	schedule_t data;
	
	int recCount = 0;

	assert(NULL!=list);
	if( access("schedule.dat", 0) != 0)
		return 0;

	List_Free(list, schedule_node_t);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) { //�ļ�������
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
			newnode = (schedule_node_t*) malloc(sizeof(schedule_node_t));
			if (!newnode) {
				printf(	"Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
				break;
			}
			newnode->data = data;
			List_AddTail(list, newnode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
	
	
}


int Play_Perst_SelectByName(play_list_t list, char condt[])
{
	play_node_t *newnode;
	play_t data;
	int recCount = 0;
	if( access("Play.dat", 0) != 0)
		return 0;
		
	List_Free(list, play_node_t);
	FILE *fp = fopen(SCHEDULE_SELECT_NAME, "rb");
	if (NULL == fp)
		return 0;
	while(!feof(fp))
	{
		if( fread(&data, sizeof(play_t), 1, fp)){
			newnode = (play_node_t*)malloc( sizeof(play_node_t));
			if( !newnode )
			{
				printf(	"Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
				break;
			}
			if (strstr(data.name, condt))
				{
					newnode->data = data;
					List_AddTail(list, newnode);
					recCount++;
				}
		}
	}
	fclose(fp);
	return recCount;
	
	
}


