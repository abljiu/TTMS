#include "../View/Schedule_UI.h"
#include <stdio.h>
#include "../Common/List.h"
#include "../Persistence/Schedule_Persist.h"
#include "../Service/Studio.h"
//有一个问题 
static const int SCHEDULE_PAGE_SIZE = 5;

void Schedule_UI_ListAll(void)
{
	char choice;
	char name[31];
	int i, id;
	
	schedule_list_t head;
	schedule_node_t *pos;
	Pagination_t paging;
	
	List_Init(head, schedule_node_t);
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	
	//载入数据
	paging.totalRecords = Schedule_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	//Play_Srv_FetchByID(haed, head->data.play_id);
	//Studio_Srv_FindByID( , head->data.studio_id);
	
	do{
		printf(
				"\n======================================================================================\n");
		printf(
				"*********************************  演出计划表  *****************************************\n");
		printf("%5s  %12s  %12s  %18s  %18s  %13s\n", "ID", "剧目ID", "演出厅ID",
				 "放映日期", "放映时间", "座位数");
		printf(
				"----------------------------------------------------------------------------------------\n");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, schedule_node_t, pos, i){
			printf("%5d  %10d  %10d  %18d.%02d.%02d  %10d:%02d:%02d  %10d\n",pos->data.id, pos->data.play_id,
			  pos->data.studio_id, pos->data.data.year, pos->data.data.month,
			  pos->data.data.day,	pos->data.time.hour, pos->data.time.minute, pos->data.time.second, pos->data.seat_count );
		}
		
		
		printf(
				"------- 演出计划总数:%2d --------------------------------------------------- 页数 %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"[P]上一页 | [N]下一页 | [Q]查询 | [R]返回");
		printf(
				"\n==================================================================\n");
		printf("请选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
		
		switch(choice)
		{
			case 'q':
			case 'Q':
				printf("请输入剧目名称:");
				scanf("%s", name);
				if( Schedule_UI_Query(name)){
					paging.totalRecords = Schedule_Srv_FetchAll(head);
				    List_Paging(head, paging, schedule_node_t);
				}
		
			case 'p':
			case 'P':
				if (!Pageing_IsFirstPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
				}
				break;
			case 'n':
			case 'N':
				if (!Pageing_IsLastPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
				}
				break;
		}
	}while (choice != 'r' && choice != 'R');
	List_Destroy(head, schedule_node_t);
}


int Schedule_UI_Query( char *play_name){
	schedule_list_t list_s;
	play_list_t list_p;
	schedule_node_t  *snode;
	play_node_t *node;
	int count = 0, sum = 0, j;
	
	List_Init(list_p, play_node_t);
	List_Init(list_s, schedule_node_t);
	Play_Srv_FetchByName(list_p, play_name);
	
	node = (play_node_t *)malloc(sizeof(play_node_t));
	node = list_p;	
	j = 0;
	while(node->next != list_p)
	{
		if( j != 0)
			node = (play_node_t *)malloc(sizeof(play_node_t));
		if(count = Schedule_Srv_FetchByPlay(list_s, node->data.id ) )
			sum += count;
		node = node->next;
		j++;
	
	}
	
	int i;
	Pagination_t paging;
	schedule_node_t *pos;
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	paging.totalRecords = Schedule_Srv_FetchAll(list_s);
	Paging_Locate_FirstPage(list_s, paging);
	

	printf(
			"\n======================================================================================\n");
	printf(
			"********************************* 计划   查询 *****************************************\n");
	printf("%5s  %12s  %12s  %18s  %18s  %13s\n", "ID", "剧目ID", "演出厅ID",
			 "放映日期", "放映时间", "座位数");
	printf(
			"----------------------------------------------------------------------------------------\n");
	//显示数据
	Paging_ViewPage_ForEach(list_s, paging, schedule_node_t, pos, i){
		printf("%5d  %10d  %10d  %18d.%02d.%02d  %10d:%02d:%02d  %10d\n",pos->data.id, pos->data.play_id,
		  pos->data.studio_id, pos->data.data.year, pos->data.data.month,
		  pos->data.data.day,	pos->data.time.hour, pos->data.time.minute, pos->data.time.second, pos->data.seat_count );
	}
	
	printf(
		"\n------- 演出计划总数:%2d ----------------------- 页数 %2d/%2d ---------\n",
		paging.totalRecords, Pageing_CurPage(paging),
		Pageing_TotalPages(paging));
	

/*
	j = 0;
	while(snode->next != list_s)//问题，(list_s == snode->next)失效，循环不能终止 
	{
		if(j != 0 )
			snode = (schedule_node_t*)malloc( sizeof(schedule_node_t) );
		printf(
				"\n==================================================================\n");
		printf(
				"******************************Schedule   Query **********************\n");
		printf("%5s  %10s  %10s  %10s\n", "play_id", "studio_id", "data", "time");
		printf(	"---------------------------------------------------------------------\n");
		
		//显示数据
		Paging_ViewPage_ForEach(list_s, paging, schedule_node_t, pos, i) {
		printf("%5d  %10d  %10d.%d.%d  %10d:%02d:%02d\n",pos->data.play_id, pos->data.studio_id,  pos->data.data.year, pos->data.data.month,
			 pos->data.data.day, pos->data.time.hour, pos->data.time.minute, pos->data.time.second );
		}
		
		printf(
			"\n------- Total Records:%2d ----------------------- Page %2d/%2d ---------\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));

		snode = snode->next;
		j++;
		
	}
*/
	return sum;
}
