#include "controle.hpp"

#define abs(x) x<0?-x:x

static int joystick_fd = -1;

int opcao = 0;

int open_joystick()
{
	joystick_fd = open(JOYSTICK_DEVNAME, O_RDONLY | O_NONBLOCK);
	if (joystick_fd < 0)
		return joystick_fd;

	return joystick_fd;
}

int read_joystick_event(struct js_event *jse)
{
	int bytes;

	bytes = read(joystick_fd, jse, sizeof(*jse)); 

	if (bytes == -1)
		return 0;

	if (bytes == sizeof(*jse))
		return 1;

	printf("Unexpected bytes from joystick:%d\n", bytes);

	return -1;
}

void close_joystick()
{
	close(joystick_fd);
}

int get_joystick_status(struct wwvi_js_event *wjse)
{
	int rc;
	struct js_event jse;
	if (joystick_fd < 0)
		return -1;

	//memset(wjse, 0, sizeof(*wjse));
	while ((rc = read_joystick_event(&jse) == 1)) {
		jse.type &= ~JS_EVENT_INIT;
		if (jse.type == JS_EVENT_AXIS) {
			switch (jse.number) {
			case 0: wjse->stick_x = jse.value;
				break;
			case 1: wjse->stick_y = jse.value;
				break;
			case 2: wjse->stick_x = jse.value;
				break;
			case 3: wjse->stick_y = jse.value;
				break;
			default:
				break;
			}
		} else if (jse.type == JS_EVENT_BUTTON) {
			if (jse.number < 10) {
				switch (jse.value) {
				case 0:
				case 1: wjse->button[jse.number] = jse.value;
					break;
				default:
					break;
				}
			}
		}
	}
	//printf("%d\n", wjse->stick_y);
	return 0;
}


int main(int argc, char *argv[])
{
	ros::init(argc, argv, "ControleAria");
	ros::NodeHandle nh_;
	ros::Publisher cmd_vel_pub_;
	cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/RosAria/cmd_vel", 1);
	struct js_event jse;
	

	int fd, rc;
	fd = open_joystick();
	if (fd < 0) {
		printf("open failed.\n");
		exit(1);
	}

	geometry_msgs::Twist base_cmd;
	
	int eixoAtual = 0;
	int valx = 0, valy = 0;
	float frente, roda;
	while (ros::ok()) {
		rc = read_joystick_event(&jse);
		if (rc == 1) {
			if(jse.number == 0) {
				valx = jse.value;
			} else if(jse.number == 1) {
				valy = jse.value;
			}
			if(valy > 2000 || valy < -2000){
				frente = -3.0*valy/32000.0;
				roda   = 3.0*valx/32000.0;
			} else if(valx > 2000 || valx < -2000){
				roda   = 3.0*valx/32000.0;
				frente = 0;
			} else {
				frente = 0;
				roda = 0;
			}
			
			base_cmd.linear.x  = frente;
			base_cmd.angular.z = roda;
			cmd_vel_pub_.publish(base_cmd);
		}
		usleep(5000);
	}
	return 0;
}

