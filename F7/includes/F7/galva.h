#ifndef GALVA_H
#define GALVA_H

#define AXE_X     0
#define AXE_Y     1

void lsm_galva_init(void);
void lsm_ctrl_galva(int axe, uint16_t value );
void lsm_ctrl_galvaXY(int16_t x_value, uint16_t y_value);
void lsm_sin_ctrl_galva(void);
void lsm_print_line(int16_t x_value);
void lsm_galva_test(void);

#endif
