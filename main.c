#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


enum phone_type {
    smart_phone, pushbutton_phone
};

struct Phone {
    enum phone_type type;
    char model[100];
    char company[100];
    union {
        char nano[50];
        int count;
    } sim;
    int f;
    int battery;
    unsigned int storage;
    short weight;
    float diagonal;
    unsigned long price;
};

struct List {
    struct Phone *phone;
    struct List *next_phone;
};

struct List *search_one_phone(struct List *db_start);

void search_phones(struct List *start);

void add_line(struct List **db_start);

void print_info() {
    printf("Порядковый номер                            - 1\n"
           "Компания                                    - 2\n"
           "Название(вместо пробела использовать _)     - 3\n"
           "Емкость аккумулятора                        - 4\n"
           "Вес                                         - 5\n"
           "Емкость хранилища                           - 6\n"
           "Диагональ                                   - 7\n"
           "Стоимость                                   - 8\n");
}

void print_header() {
    for (int i = 0; i < 11 + 130; i++, printf("-"));
    printf("\n");
    printf("|  №  |    Company    |     Type      |          Model          |      SIM      |  Battery |  Storage |  Weight  | Diagonal |     Price     |\n");
    for (int i = 0; i < 11 + 130; i++, printf("-"));
    printf("\n");

}

void print_phone(struct Phone *pPhone, int n);

void print_db(struct List *db_start);

void edit_phone(struct List *pList);

void del_phone(struct List **db_start, struct List *ph_del);

void clear_db(struct List **db_start);

void save_in_file(struct List *db_start);

void read_from_file(struct List **db_start);

void search_phones(struct List *start) {
    struct Phone *phone_search = (struct Phone *) malloc(sizeof(struct Phone));
    struct List *tmp = start;
    print_db(start);
    printf("  Введите параметры для поиска без пробелов:\n");
    print_info();
    short options[9] = {0};
    char ops[10];
    scanf("%s", ops);
    printf("\n");
    for (int i = 0, l = strlen(ops); i < l; i++) {
        switch (ops[i]) {
            case '1': {
                int n;
                printf("Введите номер: ");
                scanf("%d", &n);
                printf("\n");
                for (int j = 1; j < n; j++) tmp = tmp->next_phone;
                if (tmp == NULL) printf("Такого телефона не найдено");
                else {
                    print_header();
                    print_phone(tmp->phone, 1);
                }
                return;
            }
            case '2': {
                options[2]++;
                printf("Введите компанию: ");
                scanf("%s", phone_search->company);
                printf("\n");
                break;
            }
            case '3': {
                printf("Введите название: ");
                scanf("%s", phone_search->model);
                printf("\n");
                options[3]++;
                break;
            }
            case '4': {
                printf("Введите емкость: ");
                scanf("%d", &phone_search->battery);
                printf("\n");
                options[4]++;
                break;
            }
            case '5': {
                printf("Введите вес: ");
                scanf("%hd", &phone_search->weight);
                printf("\n");
                options[5]++;
                break;
            }
            case '6': {
                printf("Введите объем хранилища: ");
                scanf("%d", &phone_search->storage);
                printf("\n");
                options[6]++;
                break;
            }
            case '7': {
                printf("Введите диагональ: ");
                scanf("%f", &phone_search->diagonal);
                printf("\n");
                options[7]++;
                break;
            }
            case '8': {
                printf("Введите стоимость: ");
                scanf("%ld", &phone_search->price);
                printf("\n");
                options[8]++;
                break;
            }
            default:
                printf("Я не знаю параметра с номером %c :(\n", ops[i]);
        }
    }
    int f = 0;
    while (tmp != NULL) {
        if ((!strcmp(phone_search->company, tmp->phone->company) || !options[2]) &&
            (!strcmp(phone_search->model, tmp->phone->model) || !options[3]) &&
            (phone_search->battery == tmp->phone->battery || !options[4]) &&
            (phone_search->weight == tmp->phone->weight || !options[5]) &&
            (phone_search->storage == tmp->phone->storage || !options[6]) &&
            (phone_search->diagonal == tmp->phone->diagonal || !options[7]) &&
            (phone_search->price == tmp->phone->price || !options[8])) {
            if (!f) print_header();
            f++;
            print_phone(tmp->phone, f);
        }
        tmp = tmp->next_phone;
    }
}

int main() {
//    system("chcp 65001");

    struct List *start = NULL;

    setlocale(LC_ALL, "RU");
    char instruction;
    while (1) {
        system("clear");
        printf("Выберите действие с базой данных:\n \
Вывести все записи   - 1;\n \
Редактировать запись - 2;\n \
Добавить запись      - 3;\n \
Поиск записей        - 4;\n \
Удалить одну запись  - 5;\n \
Удалить все записи   - 6;\n \
Экспорт базы данных  - 7;\n \
Импорт базы данных   - 8;\n \
Выход                - 0;\n:");
        scanf("%c", &instruction);
        system("clear");
        switch (instruction) {
            case '1': {
                if (start == NULL) printf("БД пуста\n");
                else print_db(start);
                break;
            }
            case '2': {
                if (start == NULL) printf("Запись пока пуста\n");
                else {
                    struct List *ph_edit = search_one_phone(start);
                    if (ph_edit == NULL) printf("Запись не найдена");
                    else {
                        edit_phone(ph_edit);
                        printf("Запись успешно изменена\n");
                    }
                }
                break;
            }
            case '3': {
                add_line(&start);
                printf("Запись была успешно добавлена\n");
                break;
            }
            case '4': {
                if (start == NULL) printf("БД еще пуста\n");
                else search_phones(start);
                break;
            }
            case '5': {
                if (start == NULL) printf("База данных пуста\n");
                else {
                    struct List* ph_to_del = search_one_phone(start);
                    if (ph_to_del == NULL) printf("Поиск закончился с ошибкой, телефон не найден\n");
                    else del_phone(&start, ph_to_del);
                }
                break;
            }
            case '6': {
                if (start == NULL) printf("Кто-то сделал это раньше (бд и так пуста)\n");
                else {
                    char confirmation[11];
                    printf("Необходимо подтверждение, введите: \"123\"\n");
                    scanf("%10s", confirmation);
                    if (!strcmp(confirmation, "123")) {
                        clear_db(&start);
                        printf("Бд удалена\n");
                    } else printf("Удаление отменено");
                }
                break;
            }
            case '7': {
                save_in_file(start);
                break;
            }
            case '8': {
                read_from_file(&start);
                break;
            }
            case '0':
                exit(0);
            default:
                printf("Неверный ввод\n");
        }
        printf("\nНажмите Enter для продолжения...");
        rewind(stdin);
        getchar();
//        getchar();
    }
    return 0;
}

void read_from_file(struct List **db_start) {
    char path[50] = "../";
    char file[45];
    printf("Введите название файла(по умолчанию db.txt): ");
    scanf("%s", file);
    strcat(path, file);
    printf("path: %s\n", path);
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        printf("Файл не найден :(\n");
        return;
    }
    while (!feof(f)) {
        struct Phone *cur_ph = (struct Phone *) malloc(sizeof(struct Phone));

        fscanf(f, "%s", cur_ph->company);
        fscanf(f, "%d", &cur_ph->type);
        fscanf(f, "%s", cur_ph->model);
        fscanf(f, "%d", &cur_ph->f);
        if (cur_ph->f == 1) {
            fscanf(f, "%d ", &cur_ph->sim.count);
        } else {
            fscanf(f, "%s", cur_ph->sim.nano);
        }
        fscanf(f, "%d", &cur_ph->battery);
        fscanf(f, "%d", &cur_ph->storage);
        fscanf(f, "%hd", &cur_ph->weight);
        fscanf(f, "%f", &cur_ph->diagonal);
        fscanf(f, "%ld", &cur_ph->price);
        fgetc(f);
        struct List *new = (struct List *) malloc(sizeof(struct List));
        new->phone = cur_ph;
        if (*db_start == NULL) {
            new->next_phone = NULL;
        } else {
            new->next_phone = *db_start;
        }
        *db_start = new;
    }
    fclose(f);
    printf("\nБаза данных успешно загружена из файла. ");
}

void save_in_file(struct List *db_start) {
    char path[50] = "../";
    char file[45];
    printf("Введите название файла: ");
    scanf("%s", file);
    strcat(path, file);
    printf("path: %s\n", path);
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        printf("Файл не найден :(\n");
        return;
    }
    struct List *tmp = db_start;
    while (tmp != NULL) {
        fprintf(f, "\n%s %d %s %d ", tmp->phone->company, tmp->phone->type, tmp->phone->model, tmp->phone->f);
        tmp->phone->f == 1 ? fprintf(f, "%d ", tmp->phone->sim.count) : fprintf(f, "%s ", tmp->phone->sim.nano);
        fprintf(f, "%d %d %hd %.1f %ld", tmp->phone->battery, tmp->phone->storage, tmp->phone->weight,
                tmp->phone->diagonal, tmp->phone->price);
        tmp = tmp->next_phone;
    }
    fclose(f);
    printf("Запись в файл завершена\n");
}

void clear_db(struct List **db_start) {
    struct List *next;
    while (*db_start != NULL) {
        next = (*db_start)->next_phone;
        free((*db_start)->phone);
        free(*db_start);
        *db_start = next;
    }
    *db_start = NULL;
}

void del_phone(struct List **db_start, struct List *ph_del) {
    if ((*db_start)->next_phone == NULL) {
        char inp;
        printf("Остался последний телефон, удалить его?\n"
               "0 - Отменить удаление, 1 - Для подтверждения удаления\n");
        scanf("%d", &inp);
        if (inp == 1) {
            free((*db_start)->phone);
            free((*db_start));
            *db_start = NULL;
            return;
        } else {
            printf("Удаление отменено\n");
            return;
        }
    };
    struct List *prev_ph = *db_start, *cur_ph = *db_start;
    while (cur_ph != ph_del) {
        prev_ph = cur_ph;
        cur_ph = cur_ph->next_phone;
    }
    if (prev_ph == cur_ph) { //Значит удаляется первая запись
        *db_start = cur_ph->next_phone;
        free(cur_ph->phone);
        free(cur_ph);
    } else {
        prev_ph->next_phone = cur_ph->next_phone;
        free(cur_ph->phone);
        free(cur_ph);
    }
    printf("Запись успешно удалена!\n");
}

void edit_phone(struct List *pList) {
    printf("Введите что хотите исправить без пробелов\n");
    printf("Компания                                    - 2\n"
           "Название(вместо пробела использовать _)     - 3\n"
           "Тип                                         - 4\n"
           "Емкость аккумулятора                        - 5\n"
           "Вес                                         - 6\n"
           "Емкость хранилища                           - 7\n"
           "Диагональ                                   - 8\n"
           "Стоимость                                   - 9\n");
    char ops[10];
    scanf("%s", ops);
    printf("\n");
    for (int i = 0, l = strlen(ops); i < l; i++) {
        switch (ops[i]) {
            case '2': {
                printf("Введите компанию: ");
                scanf("%s", pList->phone->company);
                printf("\n");
                break;
            }
            case '3': {
                printf("Введите название: ");
                scanf("%s", pList->phone->model);
                printf("\n");
                break;
            }
            case '4': {
                printf("Введите новый тип: ");
                scanf("%d", &pList->phone->type);
                printf("\n");
                break;
            }
            case '5': {
                printf("Введите емкость: ");
                scanf("%d", &pList->phone->battery);
                printf("\n");
                break;
            }
            case '6': {
                printf("Введите вес: ");
                scanf("%hd", &pList->phone->weight);
                printf("\n");
                break;
            }
            case '7': {
                printf("Введите объем хранилища: ");
                scanf("%d", &pList->phone->storage);
                printf("\n");
                break;
            }
            case '8': {
                printf("Введите диагональ: ");
                scanf("%f", &pList->phone->diagonal);
                printf("\n");
                break;
            }
            case '9': {
                printf("Введите стоимость: ");
                scanf("%ld", &pList->phone->price);
                printf("\n");
                break;
            }
            default:
                printf("Я не знаю параметра с номером %c :(\n", ops[i]);
        }
    }
}

void print_db(struct List *db_start) {
    int n = 0;
    print_header();
    struct List *tmp = db_start;
    while (tmp != NULL) {
        print_phone(tmp->phone, ++n);
        tmp = tmp->next_phone;
    }
}

void add_line(struct List **db_start) {
    struct Phone *cur_ph = (struct Phone *) malloc(sizeof(struct Phone));

    printf("Введите компанию: ");
    scanf("%s", cur_ph->company);

    printf("\nВведите тип(0 - смартфон, 1 - кнопочный телефон): ");
    scanf("%d", &cur_ph->type);

    printf("\nВведите модель: ");
    scanf("%s", cur_ph->model);

    printf("\nУкажите тип сим(1 - количество, 0 - указание спецификации текстом): ");
    scanf("%d", &cur_ph->f);
    if (cur_ph->f == 1) {
        printf("\nУкажите количество сим: ");
        scanf("%d ", &cur_ph->sim.count);
    } else {
        printf("\nВведите спецификацию СИМ: ");
        scanf("%s", cur_ph->sim.nano);
    }
    printf("\nУкажите емкость аккумулятора: ");
    scanf("%d", &cur_ph->battery);

    printf("\nВведите емкость хранилища: ");
    scanf("%d", &cur_ph->storage);

    printf("\nВведите вес телефона: ");
    scanf("%hd", &cur_ph->weight);

    printf("\nУкажите диагональ: ");
    scanf("%f", &cur_ph->diagonal);

    printf("\nВведите стоимость телефона: ");
    scanf("%ld", &cur_ph->price);
    struct List *new = (struct List *) malloc(sizeof(struct List));
    new->phone = cur_ph;
    if (*db_start == NULL) {
        new->next_phone = NULL;
    } else {
        new->next_phone = *db_start;
    }
    *db_start = new;
}

void print_phone(struct Phone *pPhone, int n) {
    printf("|%-5d|%-15s|", n, pPhone->company);
    if (pPhone->type == smart_phone) printf("  Smartphone   ");
    else if (pPhone->type == pushbutton_phone) printf("  Pushbutton phone  ");
//    else if (pPhone->type == landline_phone) printf("   Landline phone   ");
    else printf("   undefined   ");

    printf("|");
    int f = 0;
    for (int i = 0; i < 25; i++) {
        if (pPhone->model[i] == '\0') f = 1;
        if (f) printf(" ");
        else pPhone->model[i] == '_' ? printf(" ") : printf("%c", pPhone->model[i]);
    };

    if (pPhone->f == 1) printf("|%15d", pPhone->sim.count);
    else if (pPhone->f == 0) printf("|%15s", pPhone->sim.nano);
    else printf("|      err      ");
    printf("|%6d mAh|%7d Gb|%7d Gm|%7.2f in|%11ld RUB|\n", pPhone->battery, pPhone->storage,
           pPhone->weight, pPhone->diagonal, pPhone->price);
    for (int i = 0; i < 11 + 130; i++, printf("-"));
    printf("\n");
}

struct List *search_one_phone(struct List *db_start) {
    struct Phone *phone_search = (struct Phone *) malloc(sizeof(struct Phone));
    struct List *tmp = db_start;
    print_db(db_start);
    printf("  Введите параметры для поиска без пробелов:\n");
    print_info();
    short options[9] = {0};
    char ops[10];
    scanf("%s", ops);
    printf("\n");
    for (int i = 0, l = strlen(ops); i < l; i++) {
        switch (ops[i]) {
            case '1': {
                int n;
                printf("Введите номер: ");
                scanf("%d", &n);
                printf("\n");
                for (int j = 1; j < n && tmp != NULL; j++) tmp = tmp->next_phone;
                if (tmp != NULL) {
                    print_header();
                    print_phone(tmp->phone, 1);
                }
//                else {
//
//                }
                return tmp;
            }
            case '2': {
                options[2]++;
                printf("Введите компанию: ");
                scanf("%s", phone_search->company);
                printf("\n");
                break;
            }
            case '3': {
                printf("Введите название: ");
                scanf("%s", phone_search->model);
                printf("\n");
                options[3]++;
                break;
            }
            case '4': {
                printf("Введите емкость: ");
                scanf("%d", &phone_search->battery);
                printf("\n");
                options[4]++;
                break;
            }
            case '5': {
                printf("Введите вес: ");
                scanf("%hd", &phone_search->weight);
                printf("\n");
                options[5]++;
                break;
            }
            case '6': {
                printf("Введите объем хранилища: ");
                scanf("%d", &phone_search->storage);
                printf("\n");
                options[6]++;
                break;
            }
            case '7': {
                printf("Введите диагональ: ");
                scanf("%f", &phone_search->diagonal);
                printf("\n");
                options[7]++;
                break;
            }
            case '8': {
                printf("Введите стоимость: ");
                scanf("%ld", &phone_search->price);
                printf("\n");
                options[8]++;
                break;
            }
            default:
                printf("Я не знаю параметра с номером %c :(\n", ops[i]);
        }
    }
    while (tmp != NULL) {
        if ((!strcmp(phone_search->company, tmp->phone->company) || !options[2]) &&
            (!strcmp(phone_search->model, tmp->phone->model) || !options[3]) &&
            (phone_search->battery == tmp->phone->battery || !options[4]) &&
            (phone_search->weight == tmp->phone->weight || !options[5]) &&
            (phone_search->storage == tmp->phone->storage || !options[6]) &&
            (phone_search->diagonal == tmp->phone->diagonal || !options[7]) &&
            (phone_search->price == tmp->phone->price || !options[8])) {
            print_header();
            print_phone(tmp->phone, 1);
            return tmp;
        }

        tmp = tmp->next_phone;
    }
    return NULL;
}
