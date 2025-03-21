#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/types.h>

#define BUFFER  			100
#define MAX_FILE_SIZE 50
#define SUCCESS 			0

void sys_write(int FILENO, char msg[]) {			
				// since FILENO is a predefined int :)
	int msg_length = 0;
	int index = 0;
	while(msg[index] != '\0') {
		msg_length++;
		index++;
	}
	syscall(SYS_write, FILENO, msg, msg_length);
}

ssize_t sys_read(int FILENO, char buffer[], int size) {
				// as convenient of a wrapper as I can do :)
	syscall(SYS_read, FILENO, buffer, size);
}

int str_find(char str_a[], char character) {
	int index = 0;
	while(str_a[index] != '\0') {
		if(str_a[index] == character) {
			return index;
		}
		index++;
	}
	return -1;
}

bool str_contains(char str_a[], char character) {
	int index = 0;
	while(str_a[index] != '\0') {
		if(str_a[index] == character) {
			return true;
		}
		index++;
	}
	return false;
}

bool str_compare(char str_a[], char str_b[]) {
	int index = 0;
	while(str_a[index] != '\0' || str_b[index] != '\0') {
		if(str_a[index] != str_b[index]) {
			return false;	
		}
		index++;
	}
	return true;
}

int main(void) {
	char buffer[BUFFER];
	int bytes_read;
	char prompt[] = "wall_nut> ";
	while(true) {
		start:
		sys_write(STDOUT_FILENO, prompt);
	
		bytes_read = sys_read(STDIN_FILENO, buffer, sizeof(buffer) - 1);

		if(bytes_read > 0 && bytes_read < BUFFER - 1) {
			buffer[bytes_read] = '\0';	
		}

		if(str_compare(buffer, "exit\n")) {
			exit(SUCCESS);	
		}

		if(str_compare(buffer, "clear\n")) {
			sys_write(STDOUT_FILENO, "\033[H\033[J");
			goto start;
		}
		
		if(str_contains(buffer, '>')) {
			int index = 0;
			int char_index = str_find(buffer, '>');
			char txt[char_index + 1];
			while(index < char_index - 1) {
				txt[index] = buffer[index];
				index++;
			}
			txt[char_index - 1] = '\n';
			txt[char_index] = '\0';
			index =  char_index + 2;
			int file_index = 0;
			char file_name[MAX_FILE_SIZE];
			while(buffer[index] != '\0' && buffer[index] != '\n') {
				file_name[file_index] = buffer[index];
				file_index++;
				index++;
			}
			file_name[file_index] = '\0';
			int fd = syscall(SYS_open, file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			sys_write(fd, txt);
			syscall(SYS_close, fd);
		}
		// still under construction :)
		// Finished :) al hamdu lillah.
		// need to implement cat :D


		sys_write(STDOUT_FILENO, buffer);
	}
}
