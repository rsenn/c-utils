#ifndef NEMO_ENV_H
#define NEMO_ENV_H

#ifdef __cplusplus
extern "C" {
#endif

extern char **environ;

const char	*env_get(const char *s);
char		*env_pick(void);

unsigned int	env_init(void);
unsigned int	env_put(const char *s);
unsigned int	env_put2(const char *s, const char *t);
unsigned int	env_putb(const char *s, const char *t, unsigned int n);
unsigned int	env_unset(const char *s);
void		env_clear(void);

#ifdef __cplusplus
}
#endif

#endif
