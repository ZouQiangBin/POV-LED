/*************************************************************************************************/
/*文件：音乐播放-蜂鸣器H文件			                                                               */
/*记录：By Zouqb@2022.1.1																																			 	 */
/*************************************************************************************************/
#ifndef __MUSIC_H
#define	__MUSIC_H

#include "main.h"
#include "TIM.h"

#define	MAX_SONGS			3			//== 歌曲数量

#define L1	262
#define L11	277
#define L2	294
#define L22	311
#define L3	330
#define L4	349
#define L44	370
#define L5	392
#define L55	415
#define L6	440
#define L66	466
#define L7	494

#define M1	262*2
#define M11	277*2
#define M2	294*2
#define M22	311*2
#define M3	330*2
#define M4	349*2
#define M44	370*2
#define M5	392*2
#define M55	415*2
#define M6	440*2
#define M66	466*2
#define M7	494*2

#define H1	262*4
#define H11	277*4
#define H2	294*4
#define H22	311*4
#define H3	330*4
#define H4	349*4
#define H44	370*4
#define H5	392*4
#define H55	415*4
#define H6	440*4
#define H66	466*4
#define H7	494*4

typedef struct{

		Uint16 Tone:11;	//== 音调
		Uint16 Beat:5;	//== 节拍：以十六分音符为1个单位

}TYPE_WORD_MUSIC;

typedef struct{

		Uint8 Mode:2;
		Uint8 Mode0:2;
		Uint8 SongSel:4;

}MUSIC_BITS;
typedef struct{

 MUSIC_BITS bit;
		Uint8 	Sts;
		Uint8 	SectionMax;
		Uint8 	Section;
		Uint8 	BeatCnt;

}TYPE_MUSIC;

extern TYPE_MUSIC Music;

extern const TYPE_WORD_MUSIC JinYuLiangYuan[];
extern const TYPE_WORD_MUSIC FengZhongYouDuoYuZuoDeYun[];

extern void MusicRun(TYPE_MUSIC *p);

#endif
