#include "../View/Play_UI.h"
#include "../Common/List.h"
#include "../Service/Play.h"

#include<stdio.h>

static const int PLAY_PAGE_SIZE = 5;

	void Play_UI_MgtEntry(void)
{
	int i, id;
	char choice;

	play_t buf;
	play_list_t head;
	play_node_t *pos;
	Pagination_t paging;

	List_Init(head, play_node_t);
	paging.offset = 0;
	paging.pageSize = PLAY_PAGE_SIZE;
	
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	do {
		printf(
				"\n=======================================================================================\n");
		printf(
				"************************ Projection Room List *****************************************\n");
		printf("%5s %10s %3s %9s %3s %5s %8s  %8s    %4s\n", "剧目ID", "剧目名称", "剧目类型",
				"剧目出品地区", "剧目等级","时长（min）","上映日期","下档日期","票价");
		printf(
				"---------------------------------------------------------------------------------------\n");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, play_node_t, pos, i)
		{
			printf("%-8d %-8s %-8d %-12s %-8d %-11d %-4d.%-2d.%-2d %-4d.%-2d.%-2d %-4d\n", pos->data.id,pos->data.name,
			pos->data.type, pos->data.area,pos->data.rating,pos->data.duration,pos->data.start_date.year,
					pos->data.start_date.month,pos->data.start_date.day,
					pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day,pos->data.price);
		}
		printf(
				"------------- Total Records:%2d ------------------------------------ Page %2d/%2d -----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"***************************************************************************************\n");
		printf(
				"[P]revPage     |[N]extPage |  [A]dd  [S]earch  |[D]elete   |[U]pdate     || [R]eturn");
		printf(
				"\n=======================================================================================\n");
		printf("Your Choice:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Play_UI_Add()) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Play_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, play_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Play_UI_Delete(id)) {	//从新载入数据
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Play_UI_Modify(id)) {	//从新载入数据
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Play_Srv_FetchByID(id,&buf))
			{
				Schedule_UI_MgtEntry(id);
				printf("待完善.....");
			}
			else printf("剧目不存在！");
			break;
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, play_node_t);
}
int Play_UI_Add(void)
{
    play_t rec;
	int newRecCount = 0;
	char choice;

	do {
		printf("\n=======================================================\n");
		printf("****************  添加新剧目  ****************\n");
		printf("-------------------------------------------------------\n");
		printf("剧目名称:");
		fflush(stdin);
		gets(rec.name);
		printf("剧目出品地区:");
		gets(rec.area);
		printf("剧目类型:");
		scanf("%d", &(rec.type));
		printf("剧目等级:");
		scanf("%d", &(rec.rating));
		printf("剧目时长（分钟）:");
		scanf("%d", &(rec.duration));
		printf("开始放映日期:(year.month.day)");
		scanf("%d.%d.%d", &(rec.start_date.year),&(rec.start_date.month),&(rec.start_date.day));
		printf("结束放映日期:(year.month.day)");
		scanf("%d.%d.%d", &(rec.end_date.year),&(rec.end_date.month),&(rec.end_date.day));
		printf("票价:");
		scanf("%d", &(rec.price));
		printf("=======================================================\n");

		if (Play_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("剧目添加成功!\n");
		} else
			printf("剧目添加失败!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]dd more, [R]eturn:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}
int Play_UI_Modify(int id)
{
    play_t rec;
	int rtn = 0;
	/*Load record*/
	if (!Play_Srv_FetchByID(id, &rec)) {
		printf("该ID对应的剧目不存在！\n");
		return 0;
	}

	printf("\n======================================================\n");
	printf("*********************  修改剧目信息 *********************\n");
	printf("------------------------------------------------------\n");
	printf("剧目ID:%d\n", rec.id);
	printf("剧目名称[%s]:", rec.name);
	fflush(stdin);
	gets(rec.name);
	printf("剧目出品地区:");
	gets(rec.area);
	printf("剧目类型:");
	scanf("%d", &(rec.type));
	printf("剧目等级:");
	scanf("%d", &(rec.rating));
	printf("剧目时长（分钟）:");
	scanf("%d", &(rec.duration));
	printf("开始放映日期:(year.month.day)");
	scanf("%d.%d.%d", &(rec.start_date.year),&(rec.start_date.month),&(rec.start_date.day));
	printf("结束放映日期:(year.month.day)");
	scanf("%d.%d.%d", &(rec.end_date.year),&(rec.end_date.month),&(rec.end_date.day));
	printf("票价:");
	scanf("%d", &(rec.price));
	printf("-------------------------------------------------------\n");

	if (Play_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"剧目修改成功!\n");
	} else
		printf("剧目修改失败!\n");
	return rtn;
}
int Play_UI_Delete(int id)
{
		int rtn = 0;

	if (Play_Srv_DeleteByID(id)) {
		printf(
				"删除成功!\n");
		rtn = 1;
	} else {
		printf("删除失败!\n");
	}
	return rtn;
}
int Play_UI_Query(int id)
{
	play_t rec;
	int rtn = 0;
	if(!Play_Srv_FetchByID(id,&rec)){
		printf("该ID对应的剧目不存在！");
	}
	else{
		printf("查找成功");
		rtn = 1;
//printf("\n======================================================\n");
//printf("**********************  剧目信息 **********************\n");
//printf("------------------------------------------------------\n");
//printf("剧目ID:%d\n", rec.id);
//printf("剧目名称:%s\n:", rec.name);
//printf("剧目出品地区:%s",rec.area);
//printf("剧目类型:%d",rec.type);
//printf("剧目等级:%d",rec.rating);
//printf("剧目时长（分钟）%d:",rec.duration);
//printf("开始放映日期:%d.%d.%d",rec.start_date.year,rec.start_date.month,rec.start_date.day);
//printf("结束放映日期:%d.%d.%d",rec.end_date.year,rec.end_date.month,rec.end_date.day);
//printf("票价:%d",rec.price);
//printf("-------------------------------------------------------\n");
	}
}
