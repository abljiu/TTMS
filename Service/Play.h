
#ifndef PLAY_H_
#define PLAY_H_


//剧目类型
typedef enum{
	PLAY_TYPE_FILE=1,
	PLAY_TYPE_OPEAR=2,
	PLAY_TYPE_CONCERT=3
}play_type_t;
//剧目分级类型
typedef enum{
	PLAY_RATE_CHILD=1,
	PLAY_RATE_TENNAGE=2,
	PLAY_RATE_ADULT=3
}play_rating_t;
//日期类型
typedef struct{
	int year;
	int month;
	int day;
}ttms_date_t;
//数据结构
typedef struct {
	int                id;			   //剧目ID
	char               name[31];	   //剧目名称
	play_type_t        type;           //剧目类型
	char               area[9];        //剧目出品地区
	play_rating_t      rating;         //剧目等级
	int                duration;       //时长（分钟）
	ttms_date_t        start_date;     //开始放映日期
	ttms_date_t        end_date;       //放映结束日期
	int                price;          //票价
} play_t;
//双向链表
typedef struct play_node {
	play_t data;		    //实体数据
	struct play_node *next;	//后向指针
	struct play_node *prev;	//前向指针
} play_node_t, *play_list_t;
//获取全部剧目
int Play_Srv_FetchAll(play_list_t list);
//添加新剧目
int Play_Srv_Add( play_t *data);
//修改剧目
int Play_Srv_Modify(const play_t *data);
//根据id删除剧目
int Play_Srv_DeleteByID(int ID);
//根据id获取剧目
int Play_Srv_FetchByID(int ID, play_t *buf);





#endif
