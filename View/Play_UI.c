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
		printf("%5s %10s %3s %9s %3s %5s %8s  %8s    %4s\n", "��ĿID", "��Ŀ����", "��Ŀ����",
				"��Ŀ��Ʒ����", "��Ŀ�ȼ�","ʱ����min��","��ӳ����","�µ�����","Ʊ��");
		printf(
				"---------------------------------------------------------------------------------------\n");
		//��ʾ����
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
			if (Play_UI_Add()) //����ӳɹ����������һҳ��ʾ
			{
				paging.totalRecords = Play_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, play_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Play_UI_Delete(id)) {	//������������
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("Input the ID:");
			scanf("%d", &id);
			if (Play_UI_Modify(id)) {	//������������
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
				printf("������.....");
			}
			else printf("��Ŀ�����ڣ�");
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
	//�ͷ�����ռ�
	List_Destroy(head, play_node_t);
}
int Play_UI_Add(void)
{
    play_t rec;
	int newRecCount = 0;
	char choice;

	do {
		printf("\n=======================================================\n");
		printf("****************  ����¾�Ŀ  ****************\n");
		printf("-------------------------------------------------------\n");
		printf("��Ŀ����:");
		fflush(stdin);
		gets(rec.name);
		printf("��Ŀ��Ʒ����:");
		gets(rec.area);
		printf("��Ŀ����:");
		scanf("%d", &(rec.type));
		printf("��Ŀ�ȼ�:");
		scanf("%d", &(rec.rating));
		printf("��Ŀʱ�������ӣ�:");
		scanf("%d", &(rec.duration));
		printf("��ʼ��ӳ����:(year.month.day)");
		scanf("%d.%d.%d", &(rec.start_date.year),&(rec.start_date.month),&(rec.start_date.day));
		printf("������ӳ����:(year.month.day)");
		scanf("%d.%d.%d", &(rec.end_date.year),&(rec.end_date.month),&(rec.end_date.day));
		printf("Ʊ��:");
		scanf("%d", &(rec.price));
		printf("=======================================================\n");

		if (Play_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("��Ŀ��ӳɹ�!\n");
		} else
			printf("��Ŀ���ʧ��!\n");
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
		printf("��ID��Ӧ�ľ�Ŀ�����ڣ�\n");
		return 0;
	}

	printf("\n======================================================\n");
	printf("*********************  �޸ľ�Ŀ��Ϣ *********************\n");
	printf("------------------------------------------------------\n");
	printf("��ĿID:%d\n", rec.id);
	printf("��Ŀ����[%s]:", rec.name);
	fflush(stdin);
	gets(rec.name);
	printf("��Ŀ��Ʒ����:");
	gets(rec.area);
	printf("��Ŀ����:");
	scanf("%d", &(rec.type));
	printf("��Ŀ�ȼ�:");
	scanf("%d", &(rec.rating));
	printf("��Ŀʱ�������ӣ�:");
	scanf("%d", &(rec.duration));
	printf("��ʼ��ӳ����:(year.month.day)");
	scanf("%d.%d.%d", &(rec.start_date.year),&(rec.start_date.month),&(rec.start_date.day));
	printf("������ӳ����:(year.month.day)");
	scanf("%d.%d.%d", &(rec.end_date.year),&(rec.end_date.month),&(rec.end_date.day));
	printf("Ʊ��:");
	scanf("%d", &(rec.price));
	printf("-------------------------------------------------------\n");

	if (Play_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"��Ŀ�޸ĳɹ�!\n");
	} else
		printf("��Ŀ�޸�ʧ��!\n");
	return rtn;
}
int Play_UI_Delete(int id)
{
		int rtn = 0;

	if (Play_Srv_DeleteByID(id)) {
		printf(
				"ɾ���ɹ�!\n");
		rtn = 1;
	} else {
		printf("ɾ��ʧ��!\n");
	}
	return rtn;
}
int Play_UI_Query(int id)
{
	play_t rec;
	int rtn = 0;
	if(!Play_Srv_FetchByID(id,&rec)){
		printf("��ID��Ӧ�ľ�Ŀ�����ڣ�");
	}
	else{
		printf("���ҳɹ�");
		rtn = 1;
//printf("\n======================================================\n");
//printf("**********************  ��Ŀ��Ϣ **********************\n");
//printf("------------------------------------------------------\n");
//printf("��ĿID:%d\n", rec.id);
//printf("��Ŀ����:%s\n:", rec.name);
//printf("��Ŀ��Ʒ����:%s",rec.area);
//printf("��Ŀ����:%d",rec.type);
//printf("��Ŀ�ȼ�:%d",rec.rating);
//printf("��Ŀʱ�������ӣ�%d:",rec.duration);
//printf("��ʼ��ӳ����:%d.%d.%d",rec.start_date.year,rec.start_date.month,rec.start_date.day);
//printf("������ӳ����:%d.%d.%d",rec.end_date.year,rec.end_date.month,rec.end_date.day);
//printf("Ʊ��:%d",rec.price);
//printf("-------------------------------------------------------\n");
	}
}
