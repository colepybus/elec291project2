void turn_right_man ()
{
    pwm1 = 100;
    pwm2 = 70;
}

void turn_left_man()
{
    pwm1 = 70;
    pwm2 = 100;
}

void robot_stop()
{
    pwm1 = 0;
    pwm2 = 0;
}
