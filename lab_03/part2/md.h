#ifndef MD_H
#define MD_H

// Спецификатор extern сообщает компилятору, что следующие за ним 
// типы и имена переменных объявляются где-то в другом месте

extern char *md1_data;
extern char *md1_proc(void);


// Не компилируется
// extern char* md1_local(void);
// extern char *md1_noexport(void);


#endif