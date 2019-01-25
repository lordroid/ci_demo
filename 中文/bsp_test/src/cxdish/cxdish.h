#ifndef __CXDISH__H__
#define __CXDISH__H__

#define OUTPUT_PROCESSED	0
#define OUTPUT_MIC_LEFT		1
#define OUTPUT_REFLEFT		2
#define OUTPUT_EQLEFT		3
#define OUTPUT_MIC_RIGHT	4
#define OUTPUT_REFRIGHT		5
#define OUTPUT_EQRIGHT		6

typedef struct {
	int type;
	int freq;
	int gain;
	int q_factor;
}TYPE_EQ;

typedef struct {
	int high_input_threshold;
	int high_output_threshold;
	int low_input_threshold;
	int low_output_threshold;
	int hign_region_compression;
	int middle_region_compression;
	int low_region_expansion;
	int attacktime;
	int release_threshold;
	int releas_time;
	int release_delay;
	int fast_release_time;
	int fast_release_delay;
	int pre_drc_volume;
}TYPE_DRC;

int set_output_type(int left_type, int right_type);
int get_output_type(int *left_type, int *right_type);
int set_mic_boost(int index, int value);
void cxdish_init(void);
int get_mic_boost(int index);
int set_mic_space(int space);
int upgrade_firmware(int *ver, char* path, int needcheck);
int get_direction(int *dir, int *prob);
int get_foucus_angle(void);
int set_foucus_angle(int angle);
int get_foucus_half_width(void);
int set_foucus_half_width(int width);
int set_enable_ssp(int enable);
int get_enable_ssp(void);
int get_nr_aggresiveness(void);
int set_nr_aggresiveness(int value);
int get_soft_noise_gating(void);
int set_soft_noise_gating(int value);
int get_eq_parm(int index, TYPE_EQ *eq);
int set_eq_parm(int index, TYPE_EQ *eq);
int get_drc_parm(TYPE_DRC *drc);
int set_drc_parm(TYPE_DRC *drc);
int cx2092x_upgrade(void);
int set_mode(char *argv);
int getfwver(int *ver);
int get_mode();
#endif
