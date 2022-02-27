#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "apue.h"

#include <sys/time.h>
#include <sys/resource.h>

#define LOCKFILE "/var/run/daemon.pid"
// пользователь -- чтение, запись; группа -- чтение; остальные -- чтение
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

sigset_t mask;


int lockfile(int fd)
{
	struct flock fl;

	fl.l_type = F_WRLCK; // режим разделения записи блокируется
	fl.l_start = 0; // относительное смещение в байтах (зависит от l_whence) -- начало файла
	fl.l_whence = SEEK_SET; // с начала файла
	fl.l_len = 0; // 0 - разделение до конца файла

    // файл fd будет заблокирован на запись
	return (fcntl(fd, F_SETLK, &fl));
}


// Обеспечивает запуск только одного демона
int already_running(void)
{
	int fd;
	char buf[16];

    // открыть или создать для чтения и записи
	fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);

	if (fd < 0)
	{
		syslog(LOG_ERR, "невозможно открыть %s: %s", LOCKFILE, strerror(errno));
		exit(1);
	}

    // проверка, заблокирован ли файл
	if (lockfile(fd) < 0)
	{
		if (errno == EACCES || errno == EAGAIN)
		{
			close(fd);
			return (1);
		}
		syslog(LOG_ERR, "невозможно установить блокировку на %s: %s", LOCKFILE, strerror(errno));
		exit(1);
	}
    
	ftruncate(fd, 0); // усекает размер файла до нуля
	sprintf(buf, "%ld", (long)getpid());
	write(fd, buf, strlen(buf) + 1); // записать в файл идентификатор процесса
	return (0);
}


// Для приложения, которое "желает" стать демоном
void daemonize(const char *cmd)
{
	int i, fd0, fd1, fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;

    // 1. Сброс маски режима создания файла
    // Чтобы демон мог создавать любые файлы с любыми правами (fork() унаследует чистую маску)
	umask(0); // user mask

    // Получить максимально возможный номер дескриптора файла
	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
		err_quit("%s: невозможно получить максимальный номер дескриптора", cmd);

    // 2. Вызов fork() для setsid() -- процесс не должен быть лидером группы (родительский процесс при этом завершается)
	if ((pid = fork()) < 0)
    {
		err_quit("%s: ошибка вызова функции fork", cmd);
    }
	else if (pid != 0)
    {
		exit(0);
    }

    // 3. Вызов setsid() (создание новой сессии)
    // Послен вызова процесс -- лидер новой сессии, лидер новой группы процессов и лишается управляющего терминала
	if (setsid() < 0)
	{
		err_quit("%s: ошибка setsid", cmd);
	}

    // Запретить приобретение управляющего терминала

    // Игнорирование сигнала
	sa.sa_handler = SIG_IGN;

    // sigemptyset() инициализирует набор сигналов (который передается), и очищает его от всех сигналов
    // sa_mask -- маска сигналов, которые должны блокироваться при обработке сигнала
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

    // SIGHUP -- сигнал, посылаемый процессу для сообщения, что потеряно соединение с управляющим терминалом
    // sigaction() -- изменяет поведение процесса при получении соответсвующего сигнала
    // Игнорирование сигнала SIGHUP
	if (sigaction(SIGHUP, &sa, NULL) < 0)
    {
		err_quit("%s: невозможно игнорировать сигнал SIGHUP", cmd);
    }

    // 3.1. Вызов fork() еще раз, чтобы гарантировать, что демон не является лидером сеанса

    // 4. Корневой каталог назначается текущим рабочим каталогом (для возможности отмонтировать файловую систему)
	if (chdir("/") < 0)
    {
		err_quit("%s: невозможно сделать текущим рабочим каталогом /", cmd);
    }

    // 5. Закрыть все ненужные файловые дескрипторы
    // RLIM_INFINITY -- определяет отсутствие ограничений для ресурса
	if (rl.rlim_max == RLIM_INFINITY)
    {
		rl.rlim_max = 1024;
    }

	for (int i = 0; i < rl.rlim_max; i++)
    {
		close(i);
    }

    // 6. Присоединить файловые дескрипторы 0 (ввод), 1 (вывод), 2 (для ошибок) к /dev/null
	fd0 = open("/dev/null", O_RDWR);

	fd1 = dup(0);
	fd2 = dup(0);

    // Инициализировать файл журнала (нужно, так как любые библиотечные функции, которые пытаются читать
    // со стандартного устройства ввода или писать в стандартное устройство вывода (или сообщения об ошибках)
    // не будут оказывать никакого влияния)
	openlog(cmd, LOG_CONS, LOG_DAEMON);

	if (fd0 != 0 || fd1 != 1 || fd2 != 2)
	{
		syslog(LOG_ERR, "ошибочные файловые дескрипторы %d %d %d", fd0, fd1, fd2);
		exit(1);
	}
}


void *thr_fn(void *arg)
{
	int err, signo;

	for (;;)
	{   
        // Ожидает сигналы, указанные в mask
		err = sigwait(&mask, &signo);

		if (err != 0)
		{
			syslog(LOG_ERR, "ошиб­ка вы­зо­ва функ­ции sigwait");
			exit(1);
		}

		switch (signo)
		{
		case SIGHUP:
			syslog(LOG_INFO, "Получен сигнал SIGHUP getlogin = %s", getlogin());
			break;
		case SIGTERM:
			// SIGTERM -- сигнал, применяемый в POSIX-системах для запроса завершения процесса
			syslog(LOG_INFO, "Получен сигнал SIGTERM; вы­ход");

            // Удалить файл при удалении демона
            remove(LOCKFILE);
			exit(0);
		default:
			syslog(LOG_INFO, "по­лу­чен не­пред­ви­ден­ный сиг­нал %d\n", signo);
		}
	}
	return (0);
}


int main(int argc, char *argv[])
{
	int err;
	pthread_t tid;
	char *cmd;

	struct sigaction sa;

	daemonize("my_daemon");
	
    // Блокировка файла для всего одного демона
	if (already_running() != 0)
	{
		// syslog - от­прав­ля­ет со­об­ще­ния че­рез со­кет до­ме­на UNIX – /dev/log.
		syslog(LOG_ERR, "Демон уже запущен!\n");
		exit(1);
	}

    // Восстановить сигнал SIGHUP и заблокировать все сигналы
    // Восстановление из-за игнорирования в функции daemonize()

    // SIG_DFL -- выполнение дкйствий по умолчанию
    sa.sa_handler = SIG_DFL;

    // sa_mask -- маска сигналов, которые должны блокироваться при обработке сигнала
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
    
    // Изменяет поведение процесса при соответствующем сигнале
	if (sigaction(SIGHUP, &sa, NULL) < 0)
		err_quit("%s: не­воз­мож­но вос­ста­но­вить дей­ст­вие SIG_DFL для SIGHUP");
	
    // Инициализирует набор set, в котором содержатся все маски
	sigfillset(&mask);
	
	if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0)
		err_exit(err, "ошиб­ка вы­пол­не­ния опе­ра­ции SIG_BLOCK");

	// Создать поток для обработки SUGHUP и SIGTERM
	err = pthread_create(&tid, NULL, thr_fn, 0);
	if (err != 0)
		err_exit(err, "не­воз­мож­но соз­дать по­ток");

	syslog(LOG_WARNING, "Проверка пройдена!");

	// Переменная для сохранения текущего времени
	long int cur_time;

	while (1)
	{   
        // Считываем текущее время
	    cur_time = time(NULL);

		syslog(LOG_INFO, "Демон! Время: %s", ctime(&cur_time));
		sleep(3);
	}
}

