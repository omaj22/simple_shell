#ifndef _SHELL_H_
#define _SHELL_H_

/*libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

void SignalHandler(int sig);
void printPrompt(void);

char *safeStrncpyConst(char *dest, char *src, int n);
char *safeStrncpyConstconst(char *dest, const char *src, int n);
unsigned int stringLengthConst(const char *str);
unsigned int stringLength(char *str);
int compareStrings(char *s1, char *s2);

unsigned int getCommandLength(char *s);
char **tokenizeStringToArray(char *str);
void executeCommands(char **commands, char *buffer, char **env, char **argv, int count);
unsigned int countDirectories(char *path);
char **storeEnvironmentVariables(char *fir_com, char **environ);
char *getEnvironmentVariable(const char *name, char **environ);
char *copyCommandToBuffer(char *dest, char *src, char *command, int n, int c);
void printEnvironment(char **environ);
void freeAllDoublePointers(char **ptr);
void freeParentCommands(char *buffer, char **commands);
void freeToSend(char *buffer, char **commands);
void buildErrorMessage(char **av, char *fir_com, int count);
int writeToErrorStream(char c);
void _endFile(char *buffer);
void _forkFailure(void);
void handleNullCommand(char *buffer);
void exitShell(char *buffer, char **commands);
void cleanupEnvironment(char *buffer, char **commands, char **env);
void resolveCommandPath(char **commands, char *buffer, char **env, char **argv, int count);

#endif  

