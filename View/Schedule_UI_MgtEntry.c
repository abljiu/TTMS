#include "Schedule_UI.h"
#include "../Persistence/Schedule_Persist.h"
#include "../Common/List.h"
#include <stdio.h>

static const int SCHEDULE_PAGE_SIZE = 5;

void Schedule_UI_MgtEntry( int play_id){
	int i, id;
	
	schedule_list_t list;
	play_t buf;
	schedule_node_t *pos;
	Pagination_t paging;
	char choice;
	if( !Play_Srv_FetchByID( play_id, buf ) )
	{
		printf("error!\n");
		return 0;
	}

/*	if( list == NULL)
	{
		printf("error\n");
		return;
	}
*/
	List_Init(list, schedule_node_t);
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	
	//载入数据
//	paging.totalRecords = Schedule_Srv_FetchAll( list );
	paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
	Paging_Locate_FirstPage(list, paging);
	
	
	do {
		printf(
				"\n======================================================================================\n");
		printf(
				"*********************************  演出计划表  *****************************************\n");
		printf("%5s  %12s  %12s  %18s  %18s  %13s\n", "ID", "剧目ID", "演出厅ID",
				 "放映日期", "放映时间", "座位数");
		printf(
				"----------------------------------------------------------------------------------------\n");
		//显示数据
		Paging_ViewPage_ForEach(list, paging, schedule_node_t, pos, i){
			printf("%5d  %10d  %10d  %18d.%02d.%02d  %10d:%02d:%02d  %10d\n",pos->data.id, pos->data.play_id,
			  pos->data.studio_id, pos->data.data.year, pos->data.data.month,
			  pos->data.data.day,	pos->data.time.hour, pos->data.time.minute, pos->data.time.second, pos->data.seat_count );
		}

		
		printf(
				"------- 演出计划总数:%2d ----------------------------------------------- 页数 %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"*****************************************************************************************\n");
		printf(
				"[P]上一页 | [N]下一页 | [A]添加 | [D]删除 | [U]更新 | [T]查询 | [R]返回");
		printf(
				"\n========================================================================================\n");
		printf("请选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
		
		schedule_t rec;
		
		switch (choice) {
		case 'a':
		case 'A':
			if (Schedule_UI_Add(play_id)) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
				Paging_Locate_LastPage(list, paging, schedule_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("请输入ID:");
			scanf("%d", &id);
			if (Schedule_UI_Delete(id)) {	//从新载入数据
				paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
				List_Paging(list, paging, schedule_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("请输入ID:");
			scanf("%d", &id);
			if (Schedule_UI_Modify(id)) {	//从新载入数据
				paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
				List_Paging(list, paging, schedule_node_t);
			}
			break;
/*		case 't':
		case 'T':
			printf("请输入ID:");
			scanf("%d", &id);
			paging.totalRecords = Schedule_Srv_FetchAll(list);
			List_Paging(list, paging, schedule_node_t)
			if (Schedule_Srv_FetchByID(id, &rec)){
				//Ticket_UI_MgtEntry(void)
			}
			else{
				printf("演出计划不存在!\n");
			}
			
			break;
*/
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(list, paging, -1, schedule_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(list, paging, 1, schedule_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(list, schedule_node_t);
}

int Schedule_UI_Add( int play_id){
	schedule_t rec;
	int newRecCount = 0;
	char choice;

	do {
		printf("\n=======================================================\n");
		printf("******************** 添加新的演出计划  ******************\n");
		printf("-------------------------------------------------------\n");
		printf("演出计划ID:");
		fflush(stdin);
		scanf("%d", &(rec.id)); //修改ID无效.  studio_UI的ID无ID修改，只会按序编辑ID 
		printf("剧目ID:");
		scanf("%d", &(rec.play_id)); 
		printf("演出厅ID:");
		scanf("%d", &(rec.studio_id));
		printf("放映日期(如2023.6.6):");
		scanf("%d.%d.%d", &(rec.data.year), &(rec.data.month), &(rec.data.day));
		printf("放映时间(如12:30:00 ):");
		scanf("%d:%d:%d", &(rec.time.hour), &(rec.time.minute), &(rec.time.second));
		rec.seat_count = 0;
		printf("=======================================================\n");

		if (Schedule_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("新计划添加成功了!\n");
		} else
			printf("新计划添加失败!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]添加, [R]返回:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

int Schedule_UI_Modify( int id){
	schedule_t rec;
	int rtn = 0;
	schedule_list_t list;
	
		/*Load record*/
	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("这个计划不存在!\n 请按[Enter]返回!\n");
		getchar();
		return 0;
	}

	printf("\n=======================================================\n");
	printf("********************  更新演出计划  *********************\n");
	printf("-------------------------------------------------------\n");
	printf("演出计划ID:%d\n", rec.id);
	printf("演出厅ID[%d]:", rec.studio_id);
	scanf("%d", &(rec.studio_id));
	printf("剧目ID[%d]:", rec.play_id);
	scanf("%d", &(rec.play_id));
	printf("放映日期[%d.%d.%d]:",rec.data.year, rec.data.month, rec.data.day);
	scanf("%d.%d.%d", &(rec.data.year), &(rec.data.month), &(rec.data.day) );
	printf("放映时间[%d:%d:%d]:", rec.time.hour, rec.time.minute, rec.time.second );
	scanf("%d.%d.%d", &(rec.time.hour), &(rec.time.minute), &(rec.time.second) );
	fflush(stdin);

	printf("-------------------------------------------------------\n");

	if (Schedule_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"计划更新成功了!\n请按 [Enter] 返回!\n");
	} 
	else
		printf("计划更新失败了!\n请按 [Enter] 返回n!\n");

	getchar();
	return rtn;
}

int Schedule_UI_Delete(int id){
	int rtn = 0;

	if (Schedule_Srv_DeleteByID(id)) {
		printf("计划删除成功了!请按 [Enter] 返回!\n\n");
		rtn = 1;
	}
	else {
		printf("这个计划不存在!\n请按 [Enter] 返回!\n");
	}
	getchar();
	return rtn;
}
