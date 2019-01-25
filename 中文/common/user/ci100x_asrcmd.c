/**
  ******************************************************************************
  * @文件    ci100x_asrcmd.c
  * @作者    chipintelli软件团队
  * @版本    V1.0.0
  * @日期    2016-4-9
  * @概要    这个文件是chipintelli公司的CI100X芯片程序的命令词处理文件.
  ******************************************************************************
  * @注意
  *
  * 版权归chipintelli公司所有，未经允许不得使用或修改
  *
  ******************************************************************************
  */ 

#include "ci100x_asrcmd.h"
  
#ifdef __cplusplus
extern "C" {
#endif

/**
 *@brief Struct of asr cmd. Include cmd id and cmd and threshold score.
 */
AsrCmd_TypeDef AsrCmd_Struct[]=
{
{0XFFFE,"<Reserved>",},//我先离开稍后回来
{1,"小达小达",SCORE_THRESHOLD},
{2,"一号",SCORE_THRESHOLD},
{3,"二号",SCORE_THRESHOLD},
{4,"三号",SCORE_THRESHOLD},
{5,"四号",SCORE_THRESHOLD},
{6,"五号",SCORE_THRESHOLD},
{7,"六号",SCORE_THRESHOLD},
{8,"七号",SCORE_THRESHOLD},
{9,"八号",SCORE_THRESHOLD},
{10,"九号",SCORE_THRESHOLD},
{11,"十号",SCORE_THRESHOLD},
{12,"十一号",SCORE_THRESHOLD},
{13,"十二号",SCORE_THRESHOLD},
{14,"十三号",SCORE_THRESHOLD},
{15,"十四号",SCORE_THRESHOLD},
{16,"十五号",SCORE_THRESHOLD},
{17,"十六号",SCORE_THRESHOLD},
{18,"十七号",SCORE_THRESHOLD},
{19,"十八号",SCORE_THRESHOLD},
{20,"十九号",SCORE_THRESHOLD},
{21,"二十号",SCORE_THRESHOLD},
{22,"二十一号",SCORE_THRESHOLD},
{23,"二十二号",SCORE_THRESHOLD},
{24,"二十三号",SCORE_THRESHOLD},
{25,"二十四号",SCORE_THRESHOLD},
{26,"二十五号",SCORE_THRESHOLD},
{27,"二十六号",SCORE_THRESHOLD},
{28,"二十七号",SCORE_THRESHOLD},
{29,"二十八号",SCORE_THRESHOLD},
{30,"二十九号",SCORE_THRESHOLD},
{31,"一百零一号",SCORE_THRESHOLD},
{32,"一百零二号",SCORE_THRESHOLD},
{33,"一百零三号",SCORE_THRESHOLD},
{34,"一百零四号",SCORE_THRESHOLD},
{35,"一百零五号",SCORE_THRESHOLD},
{36,"一百零六号",SCORE_THRESHOLD},
{37,"一百零七号",SCORE_THRESHOLD},
{38,"一百零八号",SCORE_THRESHOLD},
{39,"一百零九号",SCORE_THRESHOLD},
{40,"一百一十号",SCORE_THRESHOLD},
{41,"幺三零号",SCORE_THRESHOLD},
{42,"幺三幺号",SCORE_THRESHOLD},
{43,"幺三二号",SCORE_THRESHOLD},
{44,"幺三三号",SCORE_THRESHOLD},
{45,"幺三四号",SCORE_THRESHOLD},
{46,"幺三五号",SCORE_THRESHOLD},
{47,"幺三六号",SCORE_THRESHOLD},
{48,"幺三七号",SCORE_THRESHOLD},
{49,"幺三八号",SCORE_THRESHOLD},
{50,"幺三九号",SCORE_THRESHOLD},
{200,"welcom"},//该词条为开机提示音对应的词条
{-1,}/*EOF 结束标识*/
};

const AsrSpecial_TypeDef specialwords_lst[] = 
{
  {20,"一号"},
  {20,"二号"},
  {20,"三号"},
  {20,"四号"},
  {30,"十号"},
  {20,"五号"},
  {20,"六号"},
  {20,"七号"},
  {20,"八号"},
  {20,"九号"},
  {20,"二十号"},
  {20,"二十二号"},
  {-1,}
};


/**
 *@brief Wakeup words list. Be Used when USE_SEPARATE_WAKEUP is setted to 1.
 */
const char * wakewords_lst[] =
{
    "小达小达",
    "END"
};

/**
 *@brief Combo cmd first word list. Be used when USE_COMBO_CMD is setted to 1.
 */
#if USE_COMBO_CMD
const char * combo_cmd_first_word_list[] =
{
    "END"
};
#endif

#ifdef __cplusplus
}
#endif
/***************** (C) COPYRIGHT Chipintelli Technology Co., Ltd. *****END OF FILE****/
