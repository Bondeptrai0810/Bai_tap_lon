#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cau truc mon uong
typedef struct {
    int id;
    char ten[50];
    float gia;
} Mon;

// Cau truc don hang 
typedef struct Don {
    Mon mon;
    int sl;
    struct Don* next;
} Don;

// Hàm nhap thuc don
Mon* nhapMenu(int* n) {
    printf("Nhap so mon: ");
    scanf("%d", n);
    getchar(); // xóa \n

    Mon* ds = (Mon*)malloc((*n) * sizeof(Mon));
    for (int i = 0; i < *n; i++) {
        ds[i].id = i + 1;
        printf("Mon %d:\n", i + 1);
        printf("Ten: "); gets(ds[i].ten);
        printf("Gia: "); scanf("%f", &ds[i].gia); getchar();
    }
    return ds;
}

// Ham in thuc don
void xemMenu(Mon* ds, int n) {
    printf("\n--- THUC DON ---\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %s - %.0f VND\n", ds[i].id, ds[i].ten, ds[i].gia);
    }
}

// Tao don hang
Don* taoDon(Mon* ds, int n) {
    Don* head = NULL, *tail = NULL;
    int id, sl;

    while (1) {
        printf("Nhap ID mon (0 de ket thuc): ");
        scanf("%d", &id);
        if (id == 0) break;
        if (id < 1 || id > n) {
            printf("Sai ID!\n");
            continue;
        }

        printf("So luong: ");
        scanf("%d", &sl);

        Don* node = (Don*)malloc(sizeof(Don));
        node->mon = ds[id - 1];
        node->sl = sl;
        node->next = NULL;

        if (head == NULL)
            head = tail = node;
        else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

// In don hang va luu vao file
void xuatDon(Don* head) {
    FILE* f = fopen("donhang.txt", "a");
    float tong = 0;
    printf("\n--- DON HANG ---\n");
    fprintf(f, "=== DON HANG MOI ===\n");

    while (head != NULL) {
        float tien = head->sl * head->mon.gia;
        printf("%s x %d = %.0f\n", head->mon.ten, head->sl, tien);
        fprintf(f, "%s x %d = %.0f\n", head->mon.ten, head->sl, tien);
        tong += tien;
        head = head->next;
    }

    printf("Tong tien: %.0f VND\n", tong);
    fprintf(f, "Tong tien: %.0f\n\n", tong);
    fclose(f);
}

// Tinh doanh thu tu file
void tinhDoanhThu() {
    FILE* f = fopen("donhang.txt", "r");
    if (!f) {
        printf("Chua co file don hang!\n");
        return;
    }

    char line[100];
    float tong = 0;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "Tong tien:", 10) == 0) {
            float tien;
            sscanf(line, "Tong tien: %f", &tien);
            tong += tien;
        }
    }

    fclose(f);
    printf(">> Doanh thu hien tai: %.0f VND\n", tong);
}

// Menu
void menu() {
    printf("\n=== QUAN LY QUAN CAFE ===\n");
    printf("1. Nhap thuc don\n");
    printf("2. Xem thuc don\n");
    printf("3. Tao don hang\n");
    printf("4. Xem don hang hien tai\n");
    printf("5. Tinh tong doanh thu\n");
    printf("0. Thoat\n");
}

int main() {
    Mon* menuCafe = NULL;
    int soMon = 0;
    Don* don = NULL;
    int chon;

    do {
        menu();
        printf("Chon: ");
        scanf("%d", &chon);
        getchar();

        switch (chon) {
            case 1:
                if (menuCafe) free(menuCafe);
                menuCafe = nhapMenu(&soMon);
                break;
            case 2:
                if (menuCafe)
                    xemMenu(menuCafe, soMon);
                else
                    printf("Chua nhap menu!\n");
                break;
            case 3:
                if (!menuCafe) {
                    printf("Nhap menu truoc!\n");
                } else {
                    if (don) {
                        // xóa don cu
                        Don* p = don;
                        while (p) {
                            Don* tmp = p;
                            p = p->next;
                            free(tmp);
                        }
                    }
                    don = taoDon(menuCafe, soMon);
                }
                break;
            case 4:
                if (don)
                    xuatDon(don);
                else
                    printf("Chua co don hang!\n");
                break;
            case 5:
                tinhDoanhThu();
                break;
            case 0:
                printf("Tam biet!\n");
                break;
            default:
                printf("Sai lua chon!\n");
        }
    } while (chon != 0);

    // Giai phong bo nho
    if (menuCafe) free(menuCafe);
    while (don) {
        Don* tmp = don;
        don = don->next;
        free(tmp);
    }

    return 0;
}

