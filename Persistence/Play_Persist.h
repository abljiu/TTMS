#include "../Service/Play.h"




//����ȫ����Ŀ
int Play_Perst_SelectAll(play_list_t list);
//�����¾�Ŀ
int Play_Perst_Insert( play_t *data);
//���¾�Ŀ
int Play_Perst_Update(const play_t *data);
//����idȥ����Ŀ
int Play_Perst_RemByID(int id);
//����id�����Ŀ
int Play_Perst_SelectByID(int id, play_t *buf);
