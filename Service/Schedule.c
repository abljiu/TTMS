#include "../Persistence/Schedule_Persist.h"
#include "../Common/List.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



void Schedule_UI_ListAll(void);


int Schedule_UI_Query(char *play_name);

//安排演出
int Schedule_Srv_FetchByPlay(schedule_list_t list, int play_id){
	return Schedule_Perst_SelectByPlay( list, play_id);
} 

int Schedule_Srv_Add( schedule_t *data){
	return Schedule_Perst_Insert( data );
}

int Schedule_Srv_Modify( const schedule_t *data){
	return Schedule_Perst_Updata( data);
}

int Schedule_Srv_DeleteByID( int id ){
	return Schedule_Perst_RemByID( id);
}

int Schedule_Srv_FetchByID(int id, schedule_t *buf) {
	return Schedule_Perst_SelectByID(id, buf);

}

//查询演出
int Schedule_Srv_FetchAll(schedule_list_t list)
{
	
 	return Schedule_Perst_SelectAll(list); 
}


int Play_Srv_FetchByName( play_list_t list, char condt[])
{
	return Play_Perst_SelectByName(list, condt);
}


