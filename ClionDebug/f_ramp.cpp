#include "f_elems.h"

////////////////////////////////////////
// judge--------------------------------
////////////////////////////////////////
void ramp_judge()
{
  f2.ramp_top_row = 118;
  for (int i = MIN(f1.leftstartrow, f1.rightstartrow);
       i > f2.toppoint; i--)
  {
    if (f1.leftline[i] - f1.leftline[i + 1] > 2 ||
        f1.rightline[i] - f1.rightline[i + 1] < -2)
    {
      f2.ramp_top_row = i;
    }
  }

  if (
      f2.ramp_top_row < 50 && //90 &&

      is_continuous_line(f2.ramp_top_row + 1,
                         f1.leftstartrow,
                         f1.leftline, 3) &&
      is_continuous_line(f2.ramp_top_row + 1,
                         f1.rightstartrow,
                         f1.rightline, 3) &&

      is_L_R_nonlose(f2.ramp_top_row + 1,
                     110,
                     0, 100) &&
      is_L_R_nonlose(f2.ramp_top_row + 1,
                     110,
                     1, 100) &&

      ((f1.leftline[90] - f1.leftline[110]) -
       (f1.rightline[110] - f1.rightline[90])) < 12 &&

      f1.real_width[85] > 100 &&
      f1.real_width[85] < 120 &&

      f1.real_width[95] > 100 &&
      f1.real_width[105] > 110 &&
      f1.real_width[110] > 120)

  {
    f2.ramp_flag = 1;
    f2.ramp_frames++;
  }
  else
  {
    f2.ramp_flag = 0;
    f2.ramp_frames = 0;
  }
}
