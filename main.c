#include <dirent.h> 
#include <stdio.h> 
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

void printFilePermissions(struct stat stats);
void printFileSize(struct stat stats);
void printFileModTime(struct stat stats);
void printFileCreationDate(struct stat stats);
int is_regular_file(struct stat stats);
int contain(char* c, char** argv,int argc);
int Info(char* path,char* argv[],int argc);
char *pathcat(const char *str1, char *str2);
int ouvrirDir(char* path,struct dirent *dp,char* *argv,int argc );
void levelSearch(char* *argv,int argc);
void listdir(const char *name, int indent,char** argv, int argc,int *found);


int main(int argc, char *argv[])
{
    levelSearch(argv,argc);
}


    void levelSearch(char* *argv,int argc)
     {
   
    char* path;
    char* filename;
    filename = pathcat("",argv[argc-1]);
    struct dirent *dp;
    char* firstArg = pathcat("",argv[1]);
    int found = 0;
   
    // if first option is a path then look in that path else look in current directory
     
    if (!((strcmp("-s",firstArg)) && (strcmp("-t",firstArg)) && (strcmp("-p",firstArg)) 
     && (strcmp("-m",firstArg)) && (strcmp("-a",firstArg)) ) ){
      
      path = "./";
      
    }
    else if( !(strchr(firstArg,'/') != NULL) )
    {
                
                path = "./";
               // printf("path is :   %s \n", path);
        

    }
    else {
      
      //path = pathcat("",argv[1]);
      
      path=pathcat(path,"/");
      //path = pathcat(path,filename);
          }
    
     // printf("before ouvrirDir %s\n",path);
     //printf("path :   %s \n", path);
     listdir(path,0,argv,argc,&found);
    
     sleep(1);
     return;
	 // ouvrirDir(firstArg,dp,argv,argc);
	  
      //Info(path,argv,argc);
      if(!found){
          printf("fichier non trouve\n");
      }
    }





    int Info(char* path,char* argv[],int argc)
    { 
    struct stat stats;
    printf("\nTouve dans l'emplacement suivant: %s \n",path);
    //choosing path
    
    // stat() returns 0 on successful operation,
    // otherwise returns -1 if unable to get file properties.
    DIR* opendir (const char* path);
    //printf("stat value  : %d",stat(path, &stats));
    if (stat(path, &stats) == 0) 
    {
    //permissions
    
    if(contain("-p",argv,argc) || contain("-a",argv,argc) ) {
        printFilePermissions(stats);
}
      
      
       
      
      
      //Taille 
      if( contain("-s",argv,argc) || contain("-a",argv,argc) ) {
        printFileSize(stats);
}
          
       //date modification ....
    if( contain("-m", argv,argc) || contain("-a",argv,argc) ) {
        printFileModTime(stats);
}


        //type  fichier
        char* type;
        
        if (contain("-t",argv,argc) || contain("-a",argv,argc))
         {
        if(is_regular_file(stats)==1) { 
          type = "fichier";

        }
        else if (is_regular_file(stats) == 0 ) {
          type = "repertoire";
        }
    printf("\ntype du fichier : %s\n", type );


    }
    }
    else
    {
        printf("Unable to get file properties.\n");
        printf("Please check whether '%s' file exists.\n", path);
    }

    return 0;
}



/**
 * Function to print file properties.
 */
void printFilePermissions(struct stat stats)
{
    struct tm dt;

    // File permissions
    printf("\nProtection: ");
    if (stats.st_mode & R_OK)
        printf("Lecture ");
    if (stats.st_mode & W_OK)
        printf("Ecriture ");
    if (stats.st_mode & X_OK)
        printf("Execution");
}
void printFileSize(struct stat stats)
{
    struct tm dt;
    // File size
    printf("\nTaille fichier: %ld", stats.st_size);
}
void printFileCreationDate(struct stat stats)
{
    struct tm dt;
    // Get file creation time in seconds and 
    // convert seconds to date and time format
    dt = *(gmtime(&stats.st_ctime));
    printf("\nCree: %d-%d-%d %d:%d:%d", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900, 
                                              dt.tm_hour, dt.tm_min, dt.tm_sec);
}
void printFileModTime(struct stat stats)
{
    struct tm dt;
    // File modification time
    dt = *(gmtime(&stats.st_mtime));
    printf("\nModifie: %d-%d-%d %d:%d:%d\n", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900, 
                                              dt.tm_hour, dt.tm_min, dt.tm_sec);
}
                                          
   // File ttype
  int is_regular_file(struct stat stats)
{
    
    return S_ISREG(stats.st_mode);
}


int contain(char* c, char** argv,int argc)
{
int len = argc;
int i=0;

while(argv[i]) {
        if(strcmp(argv[i], c) == 0) {
            return 1;
            break;
        }
        i++;
    }
    return 0;
}
char *pathcat(const char *str1, char *str2)
{
    char *res;
    size_t strlen1 = strlen(str1);
    size_t strlen2 = strlen(str2);
    int i, j;
    res = malloc((strlen1+strlen2+1)*sizeof *res);
    strcpy(res, str1);
    for (i=strlen1, j=0; ((i<(strlen1+strlen2)) && (j<strlen2)); i++, j++)
        res[i] = str2[j];
    res[strlen1+strlen2] = '\0';
    return res;
}

void listdir(const char *name, int indent,char** argv, int argc,int *found)
{
    DIR *dir;
    struct dirent *dp;
    
    char* fullpath;
    //printf("name inside the listdir : %s",name);
    if (!(dir = opendir(name)))
        return;

    while ((dp = readdir(dir)) != NULL) {
        fullpath = pathcat(name, dp->d_name);
        if (! strcmp(dp->d_name,argv[argc-1]))
        {
            *found = 1;
            //printf("fichier courant:%s /fichier cherche %s\n",dp->d_name,argv[argc-1]);
            Info(fullpath,argv,argc);
            break;
            return;
        }
        if (dp->d_type == DT_DIR) {
            char path[1024];
        
            if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s/", name, dp->d_name);
            //printf("%*s[%s]\n", indent, "", dp->d_name);
            listdir(path, indent + 2,argv,argc,found);
        } else {
            //printf("%*s- %s\n", indent, "", dp->d_name);
            
        }
    
    }
        
    closedir(dir);
    return;
}





// int ouvrirDir(char* path,struct dirent *dp,char* *argv,int argc ){
    
//     char *fullpath;
//      // Directory target
//     int found = 0;
//     DIR *dir = opendir(path); // Open the directory - dir contains a pointer to manage the dir
    
//     while (dp=readdir(dir)) // if dp is null, there's no more content to read
//     {

//         fullpath = pathcat(path, dp->d_name);
//         //printf("%s\n", fullpath);
//         if (! strcmp(dp->d_name,argv[argc-1]))
//         {
//             found = 1;
//             //printf("fichier courant:%s /fichier cherche %s\n",dp->d_name,argv[argc-1]);
//             Info(fullpath,argv,argc);
//         }

        
//     }

//     if(!found) {
//         printf("fichier non trouve\n");
//     }
//      // close the handle (pointer)
//     return 0;
//   }