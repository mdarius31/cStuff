
#ifndef MDB_BUILD_SYSTEM
#define MDB_BUILD_SYSTEM
// in case you hate the standard libraries
#ifndef MDB_BUILD_SYSTEM_IMPLEMENTATION_DONT_USE_STDLIB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

// MDB_BUILD_SYSTEM_IMPLEMENTATION_DONT_USE_STDLIB
#endif

#ifdef __cplusplus
extern "C" {
#endif


typedef char* mdb_Flag;
typedef mdb_Flag* mdb_Flags;

typedef struct mdb_Target mdb_Target;
typedef mdb_Target** mdb_Deps;

struct mdb_Target {
  char *compiler;
  char *namingFlag;
  char *name;
  char *src;
  mdb_Deps deps;
  mdb_Flags flags;
  unsigned int depsAmount;
};

typedef struct mdb_allDeps mdb_allDeps;
struct mdb_allDeps {
  mdb_Deps deps;
  unsigned int amount;
};

char* mdb_stpcpy(char *to, const char* src);
char* mdb_addSpace(char *to);

unsigned int mdb_calcLenOfTargetCommand(mdb_Target *target);
char* mdb_getBuildComdForTarget(mdb_Target *target);

mdb_Target *mdb_newTarget(char* name,char* src, char* compiler, char *namingFlag, mdb_Flags flags);
mdb_Target *mdb_addDep(mdb_Target* final, mdb_Target *dep);

mdb_Target *mdb_addNewDep(mdb_Target* target, char* name, char* src, char* compiler, char *namingFlag, mdb_Flags flags);

int mdb_buildTarget(mdb_Target *t);

void mdb_printTarget(mdb_Target *target);

void mdb_gatherAllDeps(mdb_allDeps *allDeps, mdb_Target *target);


mdb_Target *mdb_addCompiledDep(mdb_Target* target, char* name);
#ifdef __cplusplus
}
#endif

// MDB_BUILD_SYSTEM
#endif

#ifdef MDB_BUILD_SYSTEM_IMPLEMENTATION

// used to ignore the unused parameter warnings
#define UNUSED(x) (void)(x)

char* mdb_stpcpy(char *to, const char* src) {
  const unsigned int len = strlen(src);
  return (char *) memcpy(to, src, len + 1) + len;
}

char* mdb_addSpace(char *to) {
  return mdb_stpcpy(to, " ");
}


mdb_Target *mdb_newTarget(char* name, char* src, char* compiler, char *namingFlag, mdb_Flags flags){
  mdb_Target *target = malloc(sizeof(mdb_Target));

  assert(target != NULL);

  target->name = name;
  target->src = src;

  target->compiler = compiler;
  target->namingFlag = namingFlag;

  target->flags = flags;

  target->deps = NULL;
  target->depsAmount = 0;

  return target;
}

mdb_Target *mdb_addDep(mdb_Target* final, mdb_Target *dep) {
  final->depsAmount += 1;

  final->deps = realloc(final->deps, final->depsAmount * sizeof(mdb_Target *));

  assert(final->deps != NULL);

  final->deps[final->depsAmount - 1] = dep;

  return final;
}

mdb_Target *mdb_addNewDep(mdb_Target* target, char* name, char* src, char* compiler, char *namingFlag, mdb_Flags flags){
  mdb_Target *dep = mdb_newTarget(name, src, compiler, namingFlag, flags);
  mdb_addDep(target, dep);

  return dep;
}

mdb_Target *mdb_addCompiledDep(mdb_Target* target, char* name){
 return mdb_addNewDep(target, name, NULL, NULL, NULL, NULL);
}

unsigned int mdb_calcLenOfTargetCommand(mdb_Target *target) {
  unsigned int result = 0;

  result += strlen(target->compiler);
  result += strlen(target->namingFlag);
  result += strlen(target->name);
  result += strlen(target->src);
  result += 4; // account for space, 1 for each previous property

  for(unsigned int i = 0; i < target->depsAmount; i++) {
    result += strlen(target->deps[i]->name);
  }

  if(target->depsAmount) result += target->depsAmount - 1;// account for space

  if(target->flags != NULL) {
   for(unsigned int i = 0; target->flags[i] != NULL; i++) {
     result += strlen(target->flags[i]);
     result += 1; //account for space
   }
  }



  return result + 1;// account for null termination
}




void mdb_gatherAllDeps(mdb_allDeps *allDeps, mdb_Target *target){
  allDeps->amount += 1;
  allDeps->deps = realloc(allDeps->deps, sizeof(allDeps->deps) * allDeps->amount);

  assert(allDeps->deps != NULL);

  allDeps->deps[allDeps->amount - 1] = target;

  for(unsigned int i = 0; i < target->depsAmount; i++) {
    mdb_gatherAllDeps(allDeps, target->deps[i]);
  }
}


char* mdb_getBuildComdForTarget(mdb_Target *target) {
  unsigned int resultLen = mdb_calcLenOfTargetCommand(target);
  char* result = malloc(resultLen * sizeof(char));

  assert(result != NULL);

  char* to = result;

  to = mdb_stpcpy(result, target->compiler);
  to = mdb_addSpace(to);

  to = mdb_stpcpy(to, target->namingFlag);
  to = mdb_addSpace(to);

  to = mdb_stpcpy(to, target->name);
  to = mdb_addSpace(to);
  to = mdb_stpcpy(to, target->src);

  for(unsigned int i = 0; i < target->depsAmount; i++){
    to = mdb_addSpace(to);
    to = mdb_stpcpy(to, target->deps[i]->name);
  }

  for(unsigned int i = 0; target->flags[i] != NULL; i++) {
    to = mdb_addSpace(to);
    to = mdb_stpcpy(to, target->flags[i]);
  }

  return result;
}



int mdb_buildTarget(mdb_Target *t) {
  mdb_allDeps allDeps = { amount: 0, deps: NULL};
  mdb_gatherAllDeps(&allDeps, t);
  int resultOfComds = 0;

  for(int i = allDeps.amount - 1; i >= 0; i--) {
    // in case its precompiled
    if(allDeps.deps[i]->src == NULL) continue;

    char *comd = mdb_getBuildComdForTarget(allDeps.deps[i]);
    printf("%s\n", comd);
    resultOfComds = system(comd);
    free(comd);

    // a command failed
    if(resultOfComds != 0) break;
  }

  free(allDeps.deps);
  allDeps.deps = NULL;

  for(unsigned int i = 0; i < t->depsAmount; i++) {
    free(t->deps[i]->deps);
    free(t->deps[i]);
    t->deps[i] = NULL;
  }

  free(t->deps);
  t->deps = NULL;

  free(t);
  t = NULL;

  return resultOfComds;
}



// MDB_BUILD_SYSTEM_IMPLEMENTATION
#endif
