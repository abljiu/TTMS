

//管理剧目 
typedef enum {
	PLAY_TYPE_FILE=1,
	PLAY_TYPE_OPEAR=2,
	PLAY_TYPE_CONCERT=3
}play_type_t;


typedef enum {
	PLAY_RATE_CHILD = 1,
	PLAY_RATE_TEENAGE = 2,
	PLAY_RATE_ADULT = 3
}play_rating_t;


typedef struct{
	int year;
	int month;
	int day;
}ttms_data_t;

typedef struct{
	int hour;
	int minute;
	int second;
}ttms_time_t;

typedef struct {
	int id;
	char name[31];
	play_type_t type;
	char area[9];
	play_rating_t rating;
	int duration;
	ttms_data_t start_data;
	ttms_data_t end_data;
	int price;
}play_t;


typedef struct play_node{
	play_t data;
	struct play_node *next;
	struct play_node *prev;
}play_node_t, *play_list_t;


//演出计划数据类型定义
typedef struct{
	int id;
	int play_id;
	int studio_id;
	ttms_data_t data;
	ttms_time_t time;
	int seat_count;
}schedule_t;

typedef struct schedule_node{
	schedule_t data;
	struct schedule_node *next;
	struct schedule_node *prev;
}schedule_node_t, *schedule_list_t;


//安排演出 
int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id);


int Schedule_Srv_Add( schedule_t *data);


int Schedule_Srv_Modify( const schedule_t *data);


int Schedule_Srv_DeleteByID( int id );


int Schedule_Srv_FetchByID(int id, schedule_t *buf);

//查询演出 
int Schedule_Srv_FetchAll(schedule_list_t list);


int Play_Srv_FetchByName( play_list_t list, char condt[]);

