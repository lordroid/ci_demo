#ifndef __DENOISE_API_H
#define __DENOISE_API_H

#ifdef __cplusplus
//extern "C" {
#endif


  
/**
  * @功能:进入降噪的参数调整接口函数
  * @注意:无      
  * @参数:1.api_denoise_threshold：该参数为自适应降噪门限,该值物理意义代表为(1帧噪声功率谱能量值)/10亿，
  *                                主要用于判断是否进行降噪，该值越小，越容易进入降噪。
  *                                该参数建议设置为2至5。默认设定值为3.5。     
  *       2.api_decide_frame_len:  该参数为判断降噪的窗长。该参数建议设置为15至40。默认设定值为20。      
  * @返回值:无
  */
extern int enter_denoise_api(float api_denoise_threshold, float api_decide_frame_len );


/**
  * @功能:降噪算法的参数调节接口函数
  * @注意:无      
  * @参数:1.denoise_alpha_l：参数为一个大于0小于1的系数，主要用来避免噪声估计值过大。
  *                          该值越大，音乐噪声越小，相应的会带来语音失真。该值建议设置为0.5-0.9。
  *                          默认设定值为0.85。
  *       2.denoise_alpha_m：参数为一个大于0小于1的系数，主要用来平滑先验信噪比。
  *                          该值越大，音乐噪声越小，相应的会带来语音失真。该参数建议设置为0.75-0.9。
  *                          默认设定值为0.85。
  *       3.denoise_beta：   参数为一个大于0小于1的系数，主要用于话音存在概率求解过程中的先验信噪比平滑。
  *                          适当减小该值，可以加快平滑速度。该参数建议设置为0.3-0.7。
  *                          默认设定值为0.5。
  *       4.denoise_G_min：  参数为一个大于0小于1的系数，主要用于控制话音残留噪声（即底噪，该参数对音乐噪声以及话音失真影响不大）,
  *                          适当减小该值有利于消除底噪，该参数建议设置为0.03至0.12。
  *                          默认设定值为0.07。
  * @返回值:无
  */

extern int denoise_parameter_api(float denoise_alpha_l,float denoise_alpha_m,float denoise_beta,float denoise_G_min );


/**
  * @功能:降噪算法参数初始化函数
  * @注意:1.denoise_alpha_l       默认设定值为 0.85
  *       2.denoise_alpha_m       默认设定值为 0.85
  *       3.denoise_beta          默认设定值为 0.5
  *       4.denoise_G_min         默认设定值为 0.07
  *       5.api_denoise_threshold 默认设定值为 3.5
  *       6.api_decide_frame_len  默认设定值为 20
  * @参数:无      
  * @返回值:无
  */
extern void denoise_init(void);


/**
  * @功能:执行降噪算法函数
  * @注意:无      
  * @参数:1.pcm_in： 降噪前输入信号     
  *       2.pcm_out：降噪后输出信号	  
  *	      
  * @返回值:无
  */
extern void apply_denoise(short * pcm_in, short * pcm_out);


/**
  * @功能:降噪算法版本信息函数
  * @注意:无      
  * @参数:ver_buf：版本信息写入的字符串的缓冲区 。   
  * 	  
  *	      
  * @返回值:无
  */
extern int denoise_getver(char* ver_buf);


#ifdef __cplusplus
//}
#endif

#endif //__DENOISE_API_H

