#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>

#define UNUSED(x) do {\
 (void)(x);\
 fprintf(stderr, "WARNING: UNUSED VARIABLE %s ON LINE %d\n", #x, __LINE__);\
 fflush(stderr);\
} while(0)

#define IS_DIR 4
#define IS_HDIR 10
#define IS_FILE 8
#define FILE_RESULT_PATH "/home/darius/foundDuplicates.txt"
#define START_PATH "/home/darius/go"
#define WRITE_TO_FILE false
#define LOG_RESULT true

typedef struct {
	char* name;

	char** files;
	unsigned int filesLength;

	char** dirs;
	unsigned int dirsLength;
	
} Directory;

typedef struct {
	Directory** dirs;
	unsigned int length;
} Directories;

Directory* initDirectory(char* name) {
	Directory *directory = NULL;
	
	directory = malloc(sizeof(Directory));
	unsigned int nameSize = strlen(name) + 1;
	
	directory->name = malloc(nameSize);
	memcpy(directory->name,name,nameSize);

	directory->files = NULL;
	directory->filesLength = 0;
	directory->dirs = NULL;
	directory->dirsLength = 0;

	return directory;
}

Directories* initDirectories() {
	Directories* directories = malloc(sizeof(Directories));
	
	directories->dirs = NULL;
	directories->length = 0;

	return directories;
}

Directory* directoryFrom(char* name) {
	DIR *dir = opendir(name);
	struct dirent *ep;
	
	if(dir == NULL) {
		fprintf(stderr, "Failed to open directory %s\n", name); 
		return NULL;
	}
		
	Directory *directory = initDirectory(name);
	
	while((ep = readdir(dir))) {
		bool itsDir = ep->d_type == IS_DIR;
		bool itsFile = ep->d_type == IS_FILE;
		
		bool itsCurrentDir = strcmp(ep->d_name, ".") == 0;
		bool itsUpperDir = strcmp(ep->d_name, "..") == 0;
		bool itsHidden = ep->d_name[0] == '.' && !(itsCurrentDir || itsUpperDir);
		bool ignorable = itsCurrentDir || itsUpperDir || itsHidden;
		
		bool readDirectory = itsDir && !ignorable;
		bool useFile = itsFile && !ignorable;
		bool usuable = useFile || readDirectory;
		if(!usuable) continue;
		
		char* foundTemplate = "%s/%s";
		unsigned int length = snprintf(NULL,0,foundTemplate, name, ep->d_name);
		unsigned int size = length + 1;
		
		char* path = malloc(size);

		if(path == NULL) {
			free(directory->name);
			free(directory);
			directory = NULL;
			break;
		}
		snprintf(path, size ,foundTemplate, name, ep->d_name);
		path[length] = '\0';

		if(readDirectory) {
			char** temp = realloc(directory->dirs, sizeof(char**) * (directory->dirsLength + 1));
			if(temp == NULL) {
				free(path);
				continue;
			}
			
			directory->dirs = temp;
			directory->dirs[directory->dirsLength] = path;
			directory->dirsLength++;
			
		} 

		if(useFile) {
			char** temp = realloc(directory->files, sizeof(char**) * (directory->filesLength + 1));
			if(temp == NULL) {
				free(path);
				continue;
			}
			directory->files = temp;
			directory->files[directory->filesLength] = path;
			directory->filesLength++; 

		} 

		
	}

	closedir(dir);	
	
	return directory;
}

void printDirectory(Directory *directory) {
	
	if(directory == NULL) {
		printf("Directory \"%s\" IS NULL\n", directory->name);
		return;
	}
	bool hasFiles = directory->filesLength > 0;
	bool hasDirectories = directory->dirsLength > 0;
		
	printf("%s: \n", directory->name);

	
	printf(" files: %s\n", hasFiles ? "" : "no files");
	for(unsigned int i = 0; i < directory->filesLength; i++){
		printf("  %s\n", directory->files[i]);

	}

	printf(" dirs: %s\n", hasDirectories ? "" : "no dirs");
	for(unsigned int i = 0; i < directory->dirsLength; i++){
			printf("  %s\n", directory->dirs[i]);

	}
	 
}

void freeDirectory(Directory *directory) {
	if(directory == NULL) return;
	
	for(unsigned int i = 0; i < directory->filesLength; i++){
		free(directory->files[i]);
	}
	directory->filesLength = 0;
	
	for(unsigned int i = 0; i < directory->dirsLength; i++){
		free(directory->dirs[i]);
	}
	directory->dirsLength = 0;
	
	free(directory->files);
	free(directory->dirs);
	free(directory->name);
	free(directory);
	directory = NULL;
}

void appendDirectory(Directories *directories, Directory *directory) {
	unsigned int size = (directories->length + 1) * sizeof(Directory*);
	
	
	 

	Directory** temp = realloc(directories->dirs, size);
	if(temp == NULL) return;
	
	directories->dirs = temp;
	directories->dirs[directories->length] = directory;
	directories->length++;
}

Directories *gatherDirsContents(char* startDir) {
	Directory *firstDir = directoryFrom(startDir);
	
 	Directories *directories =  initDirectories();

	appendDirectory(directories, firstDir);
		
	for(unsigned int i = 0; i < directories->length; i++){
		for(unsigned int j = 0; j < directories->dirs[i]->dirsLength; j++) {
			Directory* directory = directoryFrom(directories->dirs[i]->dirs[j]);
			if(directory == NULL) continue;
			appendDirectory(directories, directory);
		}
 	} 
 	
	return directories;
}

void freeDirectories(Directories* directories) {
	if(directories == NULL) return;
	
	for(unsigned int i = 0; i < directories->length; i++) {
		freeDirectory(directories->dirs[i]);
	}
	directories->length = 0;

	free(directories->dirs);
	free(directories);
	directories = NULL;
}


char* extractFileName(char* name) {
	if(name == NULL) return name;
	
	char* fileName = name;
	unsigned int length = strlen(name);

	for(unsigned int i = length; i > 0; i--) {
		if(name[i] == '/') {
			fileName = name + i + 1;
			break;
		}
	}

	return fileName;
}

int filePathArrIncludesFileName(char** arr, unsigned int arrLength, char* str) {
	if(arr == NULL) return -1;

	if(str == NULL) return -1;
	if(strlen(str) == 0) return -1;

	for(unsigned int i = 0; i < arrLength; i++) {
		if(strcmp(extractFileName(arr[i]), extractFileName(str)) == 0) return i;
	}

	return -1;
}

void listAllDuplicateFiles(char* dirName) {
	#if WRITE_TO_FILE
		FILE* fileResult = fopen("/home/darius/foundDuplicates.txt", "w+");
	#endif
	char* foundTemplate = "[START] FOUND DUPLICATE FOR: %s\n"
									 "\t%s\n\t%s\n"
									 "[STOP] FOUND DUPLICATE\n";

	char* foundData = NULL;
	
	Directories *directories = gatherDirsContents(dirName);
	unsigned int posibbleUniqueFilesCount = 0;
	
	for(unsigned int i = 0; i < directories->length; i++){ 
		posibbleUniqueFilesCount += directories->dirs[i]->filesLength;
 	}
 	
	char** uniqueFiles = malloc(sizeof(char*) * posibbleUniqueFilesCount);
	
	unsigned int uniqueFilesLength = 0;
	
	for(unsigned int i = 0; i < directories->length; i++){
		for(unsigned int j = 0; j < directories->dirs[i]->filesLength; j++) {
		
			char* filePath = directories->dirs[i]->files[j];
			char* fileName = extractFileName(filePath);

			int index = filePathArrIncludesFileName(uniqueFiles, uniqueFilesLength, filePath);
			
			bool existsAsUniqueFile = index != -1;
			
			if(existsAsUniqueFile) {
				
				unsigned int length = snprintf(NULL, 0, foundTemplate, fileName, uniqueFiles[index], filePath);
				unsigned int size = length + 1;
				foundData = realloc(foundData, size);
				snprintf(foundData, size, foundTemplate, fileName, uniqueFiles[index], filePath);
				foundData[length] = '\0';
				
				if(LOG_RESULT) printf(foundTemplate, fileName, uniqueFiles[index], filePath);
				#if WRITE_TO_FILE
					fwrite(foundData, size,1, fileResult);
				#endif 

				
			} else {
				uniqueFiles[uniqueFilesLength] = filePath;
				uniqueFilesLength++;
			}	
 			
		}
		 
	}
	
	freeDirectories(directories);
	free(uniqueFiles);
	free(foundData);
	
	#if WRITE_TO_FILE
 		fclose(fileResult);
 	#endif
}

void listDirectory(char* dirName) {
	Directory* directory = directoryFrom(dirName);
	printDirectory(directory);
	freeDirectory(directory);
}

int main(void){
	char* dirName = START_PATH;
	listAllDuplicateFiles(dirName);
	
	// listDirectory(dirName);
	
 return 0;
}
