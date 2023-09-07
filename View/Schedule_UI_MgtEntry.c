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
	
	//��������
//	paging.totalRecords = Schedule_Srv_FetchAll( list );
	paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
	Paging_Locate_FirstPage(list, paging);
	
	
	do {
		printf(
				"\n======================================================================================\n");
		printf(
				"*********************************  �ݳ��ƻ���  *****************************************\n");
		printf("%5s  %12s  %12s  %18s  %18s  %13s\n", "ID", "��ĿID", "�ݳ���ID",
				 "��ӳ����", "��ӳʱ��", "��λ��");
		printf(
				"----------------------------------------------------------------------------------------\n");
		//��ʾ����
		Paging_ViewPage_ForEach(list, paging, schedule_node_t, pos, i){
			printf("%5d  %10d  %10d  %18d.%02d.%02d  %10d:%02d:%02d  %10d\n",pos->data.id, pos->data.play_id,
			  pos->data.studio_id, pos->data.data.year, pos->data.data.month,
			  pos->data.data.day,	pos->data.time.hour, pos->data.time.minute, pos->data.time.second, pos->data.seat_count );
		}

		
		printf(
				"------- �ݳ��ƻ�����:%2d ----------------------------------------------- ҳ�� %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"*****************************************************************************************\n");
		printf(
				"[P]��һҳ | [N]��һҳ | [A]��� | [D]ɾ�� | [U]���� | [T]��ѯ | [R]����");
		printf(
				"\n========================================================================================\n");
		printf("��ѡ��:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
		
		schedule_t rec;
		
		switch (choice) {
		case 'a':
		case 'A':
			if (Schedule_UI_Add(play_id)) //����ӳɹ����������һҳ��ʾ
			{
				paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
				Paging_Locate_LastPage(list, paging, schedule_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("������ID:");
			scanf("%d", &id);
			if (Schedule_UI_Delete(id)) {	//������������
				paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
				List_Paging(list, paging, schedule_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("������ID:");
			scanf("%d", &id);
			if (Schedule_UI_Modify(id)) {	//������������
				paging.totalRecords = Schedule_Srv_FetchByPlay(list, play_id);
				List_Paging(list, paging, schedule_node_t);
			}
			break;
/*		case 't':
		case 'T':
			printf("������ID:");
			scanf("%d", &id);
			paging.totalRecords = Schedule_Srv_FetchAll(list);
			List_Paging(list, paging, schedule_node_t)
			if (Schedule_Srv_FetchByID(id, &rec)){
				//Ticket_UI_MgtEntry(void)
			}
			else{
				printf("�ݳ��ƻ�������!\n");
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
	//�ͷ�����ռ�
	List_Destroy(list, schedule_node_t);
}

int Schedule_UI_Add( int play_id){
	schedule_t rec;
	int newRecCount = 0;
	char choice;

	do {
		printf("\n=======================================================\n");
		printf("******************** ����µ��ݳ��ƻ�  ******************\n");
		printf("-------------------------------------------------------\n");
		printf("�ݳ��ƻ�ID:");
		fflush(stdin);
		scanf("%d", &(rec.id)); //�޸�ID��Ч.  studio_UI��ID��ID�޸ģ�ֻ�ᰴ��༭ID 
		printf("��ĿID:");
		scanf("%d", &(rec.play_id)); 
		printf("�ݳ���ID:");
		scanf("%d", &(rec.studio_id));
		printf("��ӳ����(��2023.6.6):");
		scanf("%d.%d.%d", &(rec.data.year), &(rec.data.month), &(rec.data.day));
		printf("��ӳʱ��(��12:30:00 ):");
		scanf("%d:%d:%d", &(rec.time.hour), &(rec.time.minute), &(rec.time.second));
		rec.seat_count = 0;
		printf("=======================================================\n");

		if (Schedule_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("�¼ƻ���ӳɹ���!\n");
		} else
			printf("�¼ƻ����ʧ��!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]���, [R]����:");
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
		printf("����ƻ�������!\n �밴[Enter]����!\n");
		getchar();
		return 0;
	}

	printf("\n=======================================================\n");
	printf("********************  �����ݳ��ƻ�  *********************\n");
	printf("-------------------------------------------------------\n");
	printf("�ݳ��ƻ�ID:%d\n", rec.id);
	printf("�ݳ���ID[%d]:", rec.studio_id);
	scanf("%d", &(rec.studio_id));
	printf("��ĿID[%d]:", rec.play_id);
	scanf("%d", &(rec.play_id));
	printf("��ӳ����[%d.%d.%d]:",rec.data.year, rec.data.month, rec.data.day);
	scanf("%d.%d.%d", &(rec.data.year), &(rec.data.month), &(rec.data.day) );
	printf("��ӳʱ��[%d:%d:%d]:", rec.time.hour, rec.time.minute, rec.time.second );
	scanf("%d.%d.%d", &(rec.time.hour), &(rec.time.minute), &(rec.time.second) );
	fflush(stdin);

	printf("-------------------------------------------------------\n");

	if (Schedule_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"�ƻ����³ɹ���!\n�밴 [Enter] ����!\n");
	} 
	else
		printf("�ƻ�����ʧ����!\n�밴 [Enter] ����n!\n");

	getchar();
	return rtn;
}

int Schedule_UI_Delete(int id){
	int rtn = 0;

	if (Schedule_Srv_DeleteByID(id)) {
		printf("�ƻ�ɾ���ɹ���!�밴 [Enter] ����!\n\n");
		rtn = 1;
	}
	else {
		printf("����ƻ�������!\n�밴 [Enter] ����!\n");
	}
	getchar();
	return rtn;
}
