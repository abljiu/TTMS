#include "../Service/Schedule.h"





//安排演出 
int Schedule_Perst_SelectByPlay( schedule_list_t list, int play_id);


int Schedule_Perst_Insert( schedule_t *data);


int Schedule_Perst_Updata( const schedule_t *data);


int Schedule_Perst_RemByID(int id);


int Schedule_Perst_SelectByID(int id, schedule_t *buf);


//查询演出 
int Schedule_Perst_SelectAll(schedule_list_t list);


int Play_Perst_SelectByName(play_list_t list, char condt[]);
