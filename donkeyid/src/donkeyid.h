/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: liuzhiming 187231450@qq.com                                                      |
  +----------------------------------------------------------------------+
*/

#ifndef DONKEYID_DONKEYID_H
#define DONKEYID_DONKEYID_H

#include <time.h>
#include <stdint.h>

#define TIMESTAMP_BITS 42   //时间所占bit位数
#define NODE_ID_BITS 12      //节点所占bit位数
#define SEQUENCE_BITS 10     //毫秒内自增

#define TIMESTAMP_MASK (-1LL^(-1LL<<TIMESTAMP_BITS))
#define NODE_ID_MASK (-1^(-1<<NODE_ID_BITS))
#define SEQUENCE_MASK (-1^(-1<<SEQUENCE_BITS))

#define TIMESTAMP_LEFT_SHIFT (NODE_ID_BITS+SEQUENCE_BITS)
#define NODE_ID_LEFT_SHIFT (SEQUENCE_BITS)

#define TYPE_1_TIMESTAMP 100000000
#define TYPE_1_NODE_ID 100000
#define TYPE_1_TIMESTAMP_MASK 0xFFFFFFFF
#define TYPE_1_NODE_ID_MASK 0x3e7
#define TYPE_1_SEQUENCE_MASK 0x270f

#define GET_TIMESTAMP_BY_DONKEY_ID(id,type,epoch) (type==0)?((uint64_t)(id>>TIMESTAMP_LEFT_SHIFT)+(epoch*1000)):(uint64_t)((id+epoch*TYPE_1_TIMESTAMP)/TYPE_1_TIMESTAMP)
#define GET_NODE_ID_BY_DONKEY_ID(id,type)  (type==0)?(int)((id>>NODE_ID_LEFT_SHIFT)&NODE_ID_MASK):(int)((id-((id/TYPE_1_TIMESTAMP)*TYPE_1_TIMESTAMP))/TYPE_1_NODE_ID)
#define GET_SEQUENCE_BY_DONKEY_ID(id,type) (type==0)?(int)(id&SEQUENCE_MASK):(int)((((id-((id/TYPE_1_TIMESTAMP)*TYPE_1_TIMESTAMP))-(GET_NODE_ID_BY_DONKEY_ID(id,type)*TYPE_1_NODE_ID)))/10)


typedef struct {
    uint64_t epoch; //自定义起始时间
    uint64_t last_timestamp; //最后使用毫秒数
    int node_id;                //节点ID
    int sequence;               //单服务器毫秒内的自增值
} donkeyid_context_t;

//批量获取id时最大能够获取的数量
#define MAX_BATCH_ID_LEN ((1<<NODE_ID_LEFT_SHIFT)*1000)


int donkeyid_init(int);

void donkeyid_set_type(int);

void donkeyid_shutdown();


void donkeyid_set_epoch(time_t);
uint64_t get_curr_timestamp();

void donkeyid_set_node_id(int);

uint64_t donkeyid_next_id();

int donkeyid_get_id_by_time(uint64_t  *,time_t,int);//批量获取1秒内的id

#endif //DONKEYID_DONKEYID_H
