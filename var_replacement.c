#include "shell.h"

/**
 * _get_current_pid - func that gets the current process ID.
 * Return: no return value.
 */
char *_get_current_pid(void)
{
	size_t x;
	char *buff;
	ssize_t file;

	x = 0;
	file = open("/proc/self/stat", O_RDONLY);
	if (file == -1)
	{
		perror("Cant read file");
		return (NULL);
	}
	buff = malloc(120);
	if (!buff)
	{
		close(file);
		return (NULL);
	}
	read(file, buff, 120);
	while (buff[x] != ' ')
		x++;
	buff[x] = '\0';

	close(file);
	return (buff);
}

/**
 * _get_env_val - func that gets the value corresponding to the env variable.
 * @len: The environment variable to search for.
 * @proximal: pointer to a list.
 * Return: returns NULL on failure.
 **/
char *_get_env_val(char *proximal, int len)
{
	char *sub, *tmp, *var;
	char **var_loc;

	sub = NULL;
	var = malloc(len + 1);
	if (!var)
		return (NULL);
	var[0] = '\0';
	_strncat(var, proximal, len);
	var_loc = _get_env(var);
	free(var);
	if (var_loc)
	{
		tmp = *var_loc;
		while (*tmp != '=')
			tmp++;
		tmp++;
		sub = malloc(_strlen(tmp) + 1);
		if (sub)
			_strcpy(sub, tmp);
	}
	return (sub);
}

/**
 * _replace_var- func replaces $$ with current PID.
 * @line: a pointer.
 * @exe_ex_val: pointer to a list.
 * Return: retunrs NULL on failure.
 **/
void _replace_var(char **line, int *exe_ex_val)
{
	int len, y, z;
	char *sub, *line_old, *line_new;

	y = z = 0;
	sub = line_old = NULL;
	line_old = *line;
	for (; line_old[y]; y++)
	{
		if (line_old[y] == '$' && line_old[y + 1]
				&& line_old[y + 1] != ' ')
		{
			if (line_old[y + 1] == '$')
			{
				sub = _get_current_pid();
				z = y + 2;
			}
			else if (line_old[y + 1] == '?')
			{
				sub = _itoa(*exe_ex_val);
				z = y + 2;
			}
			else if (line_old[y + 1])
			{
				for (z = y + 1; line_old[z] &&
						line_old[z] != '$' &&
						line_old[z] != ' '; z++)
					;
				len = z - (y + 1);
				sub = _get_env_val(&line_old[y + 1], len);
			}
	line_new = malloc(y + _strlen(sub) + _strlen(&line_old[z]) + 1);
			if (!line)
				return;
			line_new[0] = '\0';
			_strncat(line_new, line_old, y);
			if (sub)
			{
				_strcat(line_new, sub);
				free(sub);
				sub = NULL;
			}
			_strcat(line_new, &line_old[z]);
			free(line_old);
			*line = line_new;
			line_old = line_new;
			y = -1;
		}
	}
}

/**
 * free_args - func that frees up memory space.
 * @argv: pointer to a list.
 * @begin: pointer to a list.
 * Return: returns NULL on failure.
 */
void free_args(char **argv, char **begin)
{
	size_t i;

	for (i = 0; argv[i] || argv[i + 1]; i++)
		free(argv[i]);
	free(begin);
}
