
#ifndef PLAY_H_
#define PLAY_H_


//��Ŀ����
typedef enum{
	PLAY_TYPE_FILE=1,
	PLAY_TYPE_OPEAR=2,
	PLAY_TYPE_CONCERT=3
}play_type_t;
//��Ŀ�ּ�����
typedef enum{
	PLAY_RATE_CHILD=1,
	PLAY_RATE_TENNAGE=2,
	PLAY_RATE_ADULT=3
}play_rating_t;
//��������
typedef struct{
	int year;
	int month;
	int day;
}ttms_date_t;
//���ݽṹ
typedef struct {
	int                id;			   //��ĿID
	char               name[31];	   //��Ŀ����
	play_type_t        type;           //��Ŀ����
	char               area[9];        //��Ŀ��Ʒ����
	play_rating_t      rating;         //��Ŀ�ȼ�
	int                duration;       //ʱ�������ӣ�
	ttms_date_t        start_date;     //��ʼ��ӳ����
	ttms_date_t        end_date;       //��ӳ��������
	int                price;          //Ʊ��
} play_t;
//˫������
typedef struct play_node {
	play_t data;		    //ʵ������
	struct play_node *next;	//����ָ��
	struct play_node *prev;	//ǰ��ָ��
} play_node_t, *play_list_t;
//��ȡȫ����Ŀ
int Play_Srv_FetchAll(play_list_t list);
//����¾�Ŀ
int Play_Srv_Add( play_t *data);
//�޸ľ�Ŀ
int Play_Srv_Modify(const play_t *data);
//����idɾ����Ŀ
int Play_Srv_DeleteByID(int ID);
//����id��ȡ��Ŀ
int Play_Srv_FetchByID(int ID, play_t *buf);





#endif
