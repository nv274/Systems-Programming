#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>


long get_file_size(char *filename) {
	struct stat file_status;
	if(stat(filename, &file_status) < 0) {
		return -1;
	}
	return file_status.st_size;
}

//Uses binary search to check for word in compiled dictionary array
int wordlookup(char* dict, long dict_size, char* word) {
	char dict_entry[64];
	char clean_word[64];
	memset(clean_word, '\0', 64);
	int index = 0;
	int start = 0;
	int length = strlen(word);
	int end = 64*(dict_size-1);
	int mid;
	while(!isalpha(word[length-1])) length--;
	while(!isalpha(word[index])) index++;
	memcpy(clean_word, (word+index), length);
	int* hyphens = malloc(sizeof(int)*strlen(clean_word));
	hyphens[0] = 0;
	int c = 1;
	for (int i = 1; i < length; i++) {
		if(!(clean_word[i-1] == '-') && clean_word[i] == '-') {
			clean_word[i] = '\0';
			hyphens[c] = i+1;
			c++;
		}
	}
	hyphens = realloc(hyphens, sizeof(int) * c);
	int i = 0;
	//printf("%d %d %d", strcmp(dict_entry, dict_alt1), strcmp(dict_entry, dict_alt2), strcmp(dict_alt2, dict_alt1));

	while(i < c) {
		mid = (start + ((end-start)/2));
		mid -= (mid%64);
		strcpy(dict_entry, dict+(mid));
		if(strcmp(clean_word+hyphens[i], dict_entry) == 0) {
			i++;
			start = 0;
			end = 64*(dict_size-1);
		}
		else{
			if(strcmp(clean_word+hyphens[i], dict_entry) > 0 ) start = (mid + 64);
			else end = (mid - 64);
		}
		if (start > end){
			return 0;
		}
	}
	return 1;
}

//Uses POSIX functions to traverse filepaths
char* traverse(char* filepath, int* size, int* pos) {
	struct stat stats;
	int pathlen = strlen(filepath+(64*(*pos)));
	DIR *dir;
	struct dirent *entry;
	dir = opendir(filepath+(64*(*pos)));
	while((entry = readdir(dir)) != NULL) {
		if(!(entry->d_name[0] == '.')) {
			strcat(filepath+(64*(*pos)), "/");
			strcat(filepath+(64*(*pos)), entry->d_name);
			stat((filepath+(64*(*pos))), &stats);
			pathlen = strlen((filepath+(64*(*pos))));
			if(S_ISDIR(stats.st_mode)) {
				filepath =(char*)traverse(filepath, size, pos);
			}
			else {
				if(strcmp(entry->d_name + strlen(entry->d_name) - 4, ".txt") == 0) {
					(*pos)++;
					(*size) = ((*pos)+1)*64;
					filepath = (char*)realloc(filepath, *size);
					memcpy(filepath+(64*(*pos)), filepath+(64*(*pos-1)), pathlen - strlen(entry->d_name) - 1);
				}
				else memset(filepath+(64*(*pos))+pathlen, ' ', 64-(strlen(entry->d_name)+1));
			}
		}
	}
	closedir(dir);
	return filepath;
}

int main(int argc, char* argv[]) {
	//compiles dictionary
	long dictionary_size = get_file_size(argv[1]);
	if(dictionary_size == 0) {
		perror("Error: Dictionary is empty.\n");
		exit(EXIT_FAILURE);
	}
	if(argc < 3) {
		perror("ERROR: less than 3 arguments given.\n");
		exit(EXIT_FAILURE);
	}
	char* my_dictionary = malloc(64*dictionary_size);
	char buffer[64];
	int dict_fd = open(argv[1], O_RDONLY);
	int bytesread;
	int dict_pos = 0;
	int word_pos = 0;
	int a, b;
	int *pos = &a;
	int *size = &b;
	int* fds;
	int curr_line;
	int curr_col;
	char* fullpath;
	char* word = malloc(64);
	int match;
	int wordcount = 0;
	int fail = 0;
	struct stat path_stats;
	while ((bytesread = read(dict_fd, buffer, 64)) > 0) {
		for (int i = 0; i < 64; i++) {
			if(buffer[i] == '\n') {
				my_dictionary[dict_pos+word_pos] = '\0';
				word_pos = 0;
				dict_pos+=64;
				wordcount++;
			}
			else {
				my_dictionary[dict_pos+word_pos] = buffer[i];
				word_pos++;
			}
		}
	}
	close(dict_fd);
	memset(word, '\0', 64);
	fullpath = malloc(128);
	fds = malloc(16);
	for (int i = 2; i < argc; i++) {
		stat(argv[i], &path_stats);
		*pos = 0;
		*size = 128;
		fds = (int*)realloc(fds, *size);
		fullpath = (char*)realloc(fullpath, 8*(*size));
		strcpy(fullpath, argv[i]);
		stat(argv[i], &path_stats);
		if(S_ISDIR(path_stats.st_mode)) {
			fullpath =(char*)traverse(fullpath, size, pos);
			fds =(int*)realloc(fds, (*pos));
			for (int i = 0; i < (*pos+1); i++) {
				fds[i] = open((fullpath+64*i), O_RDONLY);
			}
		}
		else fds[0] = open(argv[i], O_RDONLY);
		for (int j = 0; j < ((*size)/64)-1; j++) {
			memset(buffer, '\0', 64);
			*pos = 0;
			curr_line = 1;
			curr_col = 1;
			while ((bytesread = read(fds[j], buffer, 64)) > 0) {
				for (int k = 0; k < 64; k++) {
					if((int)(buffer[k]) < 33) {
						if((*pos) != 0) {
							*(word+*pos) = '\0';
							match = wordlookup(my_dictionary, wordcount, word);
							if(!match) {
								printf("%s (%d,%d): %s\n", (fullpath+(64*j)), curr_line, curr_col-(*pos), word);
								fail = 1;
							}
							(*pos) = 0;
							memset(word, '\0', 64);
						}
						if(buffer[k] == '\n') {
							curr_line++;
							curr_col = 1;
						}
						else curr_col++;
					}
					else {
						*(word+*pos) = buffer[k];
						(*pos)++;
						curr_col++;
					}
				}
				memset(buffer, '\0', 64);
			}
			memset(buffer,'\0',64);
			close(fds[j]);
			curr_line=1;
			curr_col=1;
		}
	}
	free(fds);
	free(fullpath);
	free(word);
	free(my_dictionary);
	if(!fail) exit(EXIT_SUCCESS);
	else exit(EXIT_FAILURE);
}
