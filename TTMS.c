/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* File name: TTMS.c			  
* Description : Main in C, Ansi-style	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015年4月22日	
*/
#include "./View/Schedule_UI_MgtEntry.h"
#include "./View/Schedule_UI.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <io.h>
#include "./View/Main_Menu.h"

//系统运行入口函数main 
int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	Main_Menu();
	//int play_id  = 5;
//	Schedule_UI_MgtEntry(1);
	//Schedule_UI_ListAll();
	//Schedule_UI_Add( 2);
	return EXIT_SUCCESS;
}





