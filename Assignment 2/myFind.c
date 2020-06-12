#include <stdio.h>
#include <dirent.h>
#include <string.h>

void visitDir(const char *path, int depth) {
	DIR *directory = opendir(path);
	struct dirent* element = NULL;

	if (!directory) {
	       return;
	}

	while (element = readdir(directory)) {
		if (element->d_type == DT_REG) {
			printf("%s/%s\n", path, element->d_name);
		}
		if (element->d_type == DT_DIR) {
			if (strcmp(element->d_name, ".") && strcmp(element->d_name, "..")) {
				char buffer[1000];
				sprintf(buffer, "%s/%s", path, element->d_name);
				visitDir(buffer, depth+1);
			}
		}
	}
}

int main() {
	visitDir(".", 0);
	return 0;
}
