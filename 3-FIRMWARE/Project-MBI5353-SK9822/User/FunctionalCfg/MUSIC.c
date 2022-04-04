/*************************************************************************************************/
/*文件：音乐播放-蜂鸣器C文件			                                                               */
/*记录：By Zouqb@2022.1.1																																			 	 */
/*************************************************************************************************/
#include "MUSIC.h"

#define TIME_BASE				 			2			//== 节拍基准
#define JYLY_SECTION_MAX 			168		//== 金玉良缘音符数量
#define FZYDYZDY_SECTION_MAX 	128		//== 风中有朵雨做的云音符数量
#define SRKL_SECTION_MAX 			98		//== 生日快乐音符数量

TYPE_MUSIC Music = {0};

//== 1、金玉良缘简谱 A4/4 每小节4拍 4分音符为1拍 以16分音符为1个单位
const TYPE_WORD_MUSIC JinYuLiangYuan[JYLY_SECTION_MAX] = 
{
		{M3,2}, {M2,1}, {M3,7}, {M2,1}, {M3,2}, {M5,3}, 
		{M5,2}, {M2,1}, {M3,2}, {M6,3}, {M3,4}, {M2,2}, {M2,1}, {M3,1},
		{M5,2}, {M2,1}, {M3,2}, {M6,1}, {M3,4}, {M2,1}, {M3,1}, {M2,1}, {M5,1}, {M3,2},
		{M5,3}, {M6,9}, {L3,2}, {L5,2},
		{L6,2}, {L5,2}, {L6,2}, {M1,2}, {L7,2}, {L6,2}, {L5,4}, 
		{L2,2}, {L3,12},{L5,2}, 
		{L6,2}, {L5,2}, {L6,2}, {M1,2}, {L7,2}, {L6,6}, 
		{L5,2}, {M3,6}, { 0,4}, {L6,2}, {M3,2},
		{M2,6}, {M1,2}, {L7,3}, {L6,3}, {L5,2}, 
		{L2,4}, {L5,2}, {L3,6}, {L6,2}, {M3,2},
		{M2,6}, {M1,2}, {L7,3}, {L6,3}, {L5,2},
		{L6,12},{L3,2}, {L5,2},
		
		{L6,2}, {L5,2}, {L6,2}, {M1,2}, {L7,2}, {L6,2}, {L5,4}, 
		{L2,2}, {L3,12},{L5,2}, 
		{L6,2}, {L5,2}, {L6,2}, {M1,2}, {L7,2}, {L6,6}, 
		{L5,2}, {M3,10},{M3,2}, {M5,2},
		{M2,6}, {M1,2}, {L7,3}, {L6,3}, {L5,2}, 
		{L2,4}, {L5,2}, {L3,6}, {L6,2}, {M3,2},
		{M2,6}, {M1,2}, {L7,3}, {L6,3}, {L5,2},
		{L6,12},{ 0,4}, 
		
		{ 0,4}, { 0,4}, { 0,2}, {M3,2}, {M3,2}, {M5,2},
		{M6,2}, {M5,2}, {M3,1}, {M2,1}, {M2,2}, {M3,4}, {M5,2}, {M6,1}, {M3,1}, 
		{M3,4}, {M2,2}, {M3,1}, {L6,1}, {L6,4}, {L6,2}, {M3,2}, 
		{M2,6}, {M1,2}, {M2,2}, {M3,2}, {M5,4}, 
		{M6,2}, {M3,2}, {M3,8}, { 0,2}, {M3,2}, 
		{M6,6}, {M3,1}, {M2,1}, {M2,4}, { 0,2}, {M1,2}, 
		{M2,4}, {M5,2}, {M3,2}, {M3,4}, {M2,2}, {M3,2}, 
		{M2,2}, {M1,1}, {L6,1}, { 0,2}, {M3,2}, {M2,2}, {M1,2}, {L7,2}, {L5,2}, 
		{L5,2}, {L6,2}, {L6,4}, {L6,4}, {L3,2}, {L5,2},
		{L5,2}, {L6,2}, {L6,4}, {L6,4}, {L6,4}, 
};

//== 2、风中有朵雨做的云简谱 F4/4 每小节4拍 4分音符为1拍 以16分音符为1个单位
const TYPE_WORD_MUSIC FengZhongYouDuoYuZuoDeYun[FZYDYZDY_SECTION_MAX] = 
{
		{M3,4}, {M6,8}, {M3,4}, 
		{M5,4}, {M6,2}, {M5,2}, {M6,8}, 
		{M6,4}, {M3,4}, {M5,4}, {M6,2}, {M5,2},
		{M3,8}, { 0,8}, 
		{L6,4}, {M2,8}, {L6,4}, 
		{M1,4}, {M2,2}, {M1,2}, {M2,8}, 
		{M2,4}, {L6,4}, {M1,4}, {M2,2}, {M1,2},
		{L6,8}, { 0,8},
		{M3,4}, {M6,8}, {M3,4}, 
		{M5,4}, {M6,2}, {M5,2}, {M6,8}, 
		{M6,4}, {M3,4}, {M5,4}, {M6,2}, {M5,2},
		{M3,8}, { 0,8}, 
		{L6,4}, {M1,2}, {M2,6}, {L6,4}, 
		{M1,4}, {M2,2}, {M1,2}, {M2,4}, {M1,2}, {M2,2}, 
		{M3,4}, {M6,4}, {M7,2}, {M6,2}, {M5,4},
		{M6,12},{M6,2}, {M7,2}, 
		{H1,4}, {M7,2}, {M6,2}, {M5,2}, {M3,6}, 
		{M6,4}, {H1,4}, {M7,2}, {M6,2}, {M5,4},
		{M3,8}, { 0,4}, {M6,2}, {M7,2},
		{H1,4}, {M7,2}, {M6,2}, {M5,2}, {M3,6},
		{M6,12},{M3,2}, {M5,2}, 
		{M6,4}, {H1,4}, {H2,2}, {H1,2}, {M7,2}, {M6,2},
		{M7,16},
		{ 0,12},{M3,4},
		{H1,4}, {M7,2}, {M6,2}, {M5,3}, {M3,5},
		{M6,4}, {M5,2}, {M4,2}, {M3,4}, {M1,2}, {M2,2},
		{M3,4}, {M6,4}, {M5,4}, {M6,2}, {M5,2},
		{M6,8}, { 0,4}, {M3,4},
		{H1,4}, {M7,2}, {M6,2}, {M5,4}, {M3,4},
		{M6,4}, {M5,2}, {M4,2}, {M3,4}, {M1,2}, {M2,2},
		{M3,4}, {M6,4}, {M5,4}, {M6,2}, {M5,2},
		{M6,12},{ 0,4},
};

//== 3、生日快乐简谱 F3/4 4/4 每小节3/4拍 4分音符为1拍 以16分音符为1个单位
const TYPE_WORD_MUSIC ShengRiKuaiLe[SRKL_SECTION_MAX] = 
{
		{0 ,8}, {M4,4}, 
		{M3,4}, {M1,4}, {M2,4},
		{M1,8}, {L5,4}, 
		{L6,4}, {L5,4}, {M1,4}, 
		{L7,8}, {L5,4},
		{L6,4}, {L5,4}, {M2,4}, 
		{M1,8}, {L5,4},
		{M5,4}, {M3,4}, {M1,4}, 
		{L7,4}, {L6,4}, {M4,4},
		{M3,4}, {M1,4}, {M3,4},
		{M2,8}, {L5,4}, 
		{L6,4}, {L5,4}, {M1,4}, 
		{L7,8}, {L5,4},
		{L6,4}, {L5,4}, {M2,4}, 
		{M1,8}, {L5,4},
		{M5,4}, {M3,4}, {M1,4}, 
		{L7,4}, {L6,20}, 
		
		{M4,8}, {M3,8},
		{M1,8}, {M2,8},
		{M1,24},
		{ 0,4}, {L5,4},
		
		{L6,6}, {L5,6}, {M1,4}, 
		{L7,8}, { 0,4}, {L5,4},
		{L6,6}, {L5,6}, {M2,4}, 
		{M1,8},	{ 0,4}, {L5,4},
		{M5,6}, {M3,6}, {M1,4}, 
		{L7,6}, {L6,6}, {M4,4},
		{M3,6}, {M1,6}, {M3,4},
		{M2,8}, { 0,4}, {L5,4}, 
		
		{L6,6}, {L5,6}, {M1,4},
		{L7,8}, { 0,4}, {L5,4},
		{L6,6}, {L5,6}, {M2,4}, 
		{M1,8},	{ 0,4}, {L5,4},
		{M5,6}, {M3,6}, {M1,4}, 
		{L7,6}, {L6,18},
		{ 0,4}, {M4,4},
		
		{M3,8}, {M1,8}, {M1,4},
		{M2,16}, 
		{M1,31},
};

/*************************************************************************************************/
/* 功能：定时器播放音乐								    																						 */
/* 输入：*RunFlag---启动标志 Sel-歌曲选择																			 						 			 */
/* 输出：无			                                                                                 */
/* 描述：0-停止 1-单曲播放启动 2-循环播放启动 3-正在播放																																			 */
/*			 调度周期 64ms*/
/*************************************************************************************************/
void MusicRun(TYPE_MUSIC *p)
{
		static const TYPE_WORD_MUSIC *pSong = JinYuLiangYuan;
		
		//== 0、播放启动判断，触发后使能输出通道
		if(p->bit.Mode == 0) 
		{
				p->bit.Mode0 = p->bit.Mode;
				p->Section = 0;
				p->BeatCnt = 0;
				TIM_Cmd(TIM3, DISABLE);
				return;
		}
		else if((p->bit.Mode != p->bit.Mode0) && (p->bit.Mode == 1))	//== 单曲
		{
				//== 选择歌曲
				switch(p->bit.SongSel)
				{
						case 0:	p->SectionMax = JYLY_SECTION_MAX;
										pSong = (TYPE_WORD_MUSIC *)JinYuLiangYuan;
										break;
						case 1:	p->SectionMax = FZYDYZDY_SECTION_MAX;
										pSong = (TYPE_WORD_MUSIC *)FengZhongYouDuoYuZuoDeYun;
										break;
						case 2:	p->SectionMax = SRKL_SECTION_MAX;
										pSong = (TYPE_WORD_MUSIC *)ShengRiKuaiLe;
										break;	
						default:p->SectionMax = JYLY_SECTION_MAX;
										pSong = (TYPE_WORD_MUSIC *)JinYuLiangYuan;
										break;	
				}
				
				p->Section = 0;
				p->BeatCnt = 0;
				p->bit.Mode0 = p->bit.Mode;
				TIM_Cmd(TIM3,ENABLE);
		}
		else if((p->bit.Mode != p->bit.Mode0) && (p->bit.Mode == 2))	//== 列表循环
		{
				//== 选择歌曲
				switch(p->bit.SongSel)
				{
						case 0:	p->SectionMax = JYLY_SECTION_MAX;
										pSong = (TYPE_WORD_MUSIC *)JinYuLiangYuan;
										break;
						case 1:	p->SectionMax = FZYDYZDY_SECTION_MAX;
										pSong = (TYPE_WORD_MUSIC *)FengZhongYouDuoYuZuoDeYun;
										break;
						case 2:	
										break;	
						default:p->SectionMax = JYLY_SECTION_MAX;
										pSong = (TYPE_WORD_MUSIC *)JinYuLiangYuan;
										break;	
				}
				
				p->Section = 0;
				p->BeatCnt = 0;
				p->bit.Mode0 = p->bit.Mode;
				TIM_Cmd(TIM3,ENABLE);
		}
		else
		{
//				p->bit.Mode = 3;
		}
		
		//== 1、每拍计数
		p->BeatCnt++;
		if(p->BeatCnt > (pSong+p->Section)->Beat*TIME_BASE)
		{
				p->BeatCnt = 0;
				
				//== 播放完毕，关闭输出(增加切换循环播放功能)
				if(++p->Section >= p->SectionMax)
				{
						p->Section = 0;
						
						if(p->bit.Mode == 2)	
						{
								p->bit.Mode0 = 0;	//== 重新触发一次
								if(++p->bit.SongSel >= MAX_SONGS)				
								{
										p->bit.SongSel = 0;
								}
						}
						else
						{
								p->bit.Mode = 0;
						}
						TIM_Cmd(TIM3, DISABLE);
						
						return;
				}
				
				//== 设置下一音节输出
				if((pSong+p->Section)->Tone)
				{
						TIM3_Init((pSong+p->Section)->Tone);	//== 偷下懒 嘿嘿
				}
				else
				{
						TIM3->CCR2 = 0;//TIM_Cmd(TIM3,DISABLE);
				}
		}
		
		//== 模拟收尾，音量逐渐下降
		//TIM3->CCR2 = (TIM3->ARR >> 1) * ((p+Section)->Beat - BeatCnt + 1) / ((p+Section)->Beat);
}
