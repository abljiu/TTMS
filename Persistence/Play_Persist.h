#include "../Service/Play.h"




//载入全部剧目
int Play_Perst_SelectAll(play_list_t list);
//储存新剧目
int Play_Perst_Insert( play_t *data);
//更新剧目
int Play_Perst_Update(const play_t *data);
//根据id去除剧目
int Play_Perst_RemByID(int id);
//根据id载入剧目
int Play_Perst_SelectByID(int id, play_t *buf);
