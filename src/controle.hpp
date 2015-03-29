#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <sstream>
#include <stdio.h>
#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#define JOYSTICK_DEVNAME "/dev/input/js1"

#define JS_EVENT_BUTTON         0x01    
#define JS_EVENT_AXIS           0x02    
#define JS_EVENT_INIT           0x80    


struct js_event {
	unsigned int time;	
	short value;   
	unsigned char type;     
	unsigned char number;  
};

struct wwvi_js_event {
	int button[11];
	int stick_x;
	int stick_y;
};

extern int open_joystick(char *joystick_device);
extern int read_joystick_event(struct js_event *jse);
extern void set_joystick_y_axis(int axis);
extern void set_joystick_x_axis(int axis);
extern void close_joystick();
extern int get_joystick_status(struct wwvi_js_event *wjse);

#endif
