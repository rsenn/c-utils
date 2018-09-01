#if defined(_WIN32) || defined(_WIN64)

char*
getenv(const char* variable) {
  char env[MAX_PATH]; /* MAX_PATH is from windef.h */
  char* temp = getenv(variable);
  env[0] = '\0';
  if(temp != NULL) ExpandEnvironmentStringsA(temp, env, sizeof(env));
  return (env[0] != '\0') ? strdup(env) : NULL;

  char* env = getenv(variable);
  return (env && env[0]) ? strdup(env) : NULL;
}
#endif
