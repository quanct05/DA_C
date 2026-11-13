#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define FILE_THE_LOAI "theloai.txt"
#define FILE_PHIM "phim.txt"

struct theLoai
{
    int maTheLoai;
    char tenTheLoai[50];
};

struct typePhim
{
    int maPhim;
    char tenPhim[50];
    char daoDien[50];
    int namSX;
    int loaiPhim;
    float thoiLuong;
    int doTuoi;
};

struct theLoai *danhSachTheLoai[MAX] = {NULL};
struct typePhim *danhSachPhim[MAX] = {NULL};

int soLuong = 0;
int soLuongPhim = 0;

/* Xoa phan con lai cua dong sau khi scanf */
void checkInput(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        /* Bo qua ky tu thua */
    }
}

/* Doc so nguyen an toan hon scanf truc tiep */
int nhapSoNguyen(const char *thongBao, int *giaTri)
{
    printf("%s", thongBao);

    if (scanf("%d", giaTri) != 1)
    {
        printf("Du lieu khong hop le. Vui long nhap so nguyen!\n");
        checkInput();
        return 0;
    }

    checkInput();
    return 1;
}

/* Doc so thuc */
int nhapSoThuc(const char *thongBao, float *giaTri)
{
    printf("%s", thongBao);

    if (scanf("%f", giaTri) != 1)
    {
        printf("Du lieu khong hop le. Vui long nhap so!\n");
        checkInput();
        return 0;
    }

    checkInput();
    return 1;
}

/* Doc chuoi co khoang trang */
int nhapChuoi(const char *thongBao, char *chuoi, int kichThuoc)
{
    printf("%s", thongBao);

    if (fgets(chuoi, kichThuoc, stdin) == NULL)
    {
        return 0;
    }

    chuoi[strcspn(chuoi, "\n")] = '\0';

    /* Neu nguoi dung nhap dai hon kich thuoc mang, bo phan con lai */
    if (strchr(chuoi, '\0') != NULL)
    {
        int c;
        if (strlen(chuoi) == (size_t)(kichThuoc - 1))
        {
            while ((c = getchar()) != '\n' && c != EOF)
            {
                /* Bo qua */
            }
        }
    }

    if (chuoi[0] == '\0')
    {
        printf("Chuoi khong duoc de trong!\n");
        return 0;
    }

    return 1;
}

int timViTriTheLoai(int maTheLoai)
{
    int i = 0;
    for (i = 0; i < soLuong; i++)
    {
        if (danhSachTheLoai[i] != NULL &&
            danhSachTheLoai[i]->maTheLoai == maTheLoai)
        {
            return i;
        }
    }

    return -1;
}

int timViTriPhim(int maPhim)
{
    int i = 0;
    for (i = 0; i < soLuongPhim; i++)
    {
        if (danhSachPhim[i] != NULL &&
            danhSachPhim[i]->maPhim == maPhim)
        {
            return i;
        }
    }

    return -1;
}

int tonTaiPhimTheoTheLoai(int maTheLoai)
{
    int i = 0;
    for (i = 0; i < soLuongPhim; i++)
    {
        if (danhSachPhim[i] != NULL &&
            danhSachPhim[i]->loaiPhim == maTheLoai)
        {
            return 1;
        }
    }

    return 0;
}

/* ==================== LUU FILE ==================== */

void luuPhimVaoFile(void)
{
    int i = 0;
    FILE *f = fopen(FILE_PHIM, "w");

    if (f == NULL)
    {
        printf("Khong mo duoc file %s!\n", FILE_PHIM);
        return;
    }

    for (i = 0; i < soLuongPhim; i++)
    {
        fprintf(f, "%d|%s|%s|%d|%d|%.3f|%d\n",
                danhSachPhim[i]->maPhim,
                danhSachPhim[i]->tenPhim,
                danhSachPhim[i]->daoDien,
                danhSachPhim[i]->namSX,
                danhSachPhim[i]->loaiPhim,
                danhSachPhim[i]->thoiLuong,
                danhSachPhim[i]->doTuoi);
    }

    fclose(f);
    printf("Da luu danh sach phim vao file!\n");
}

void luuTheLoaiVaoFile(void)
{
    int i = 0;
    FILE *f = fopen(FILE_THE_LOAI, "w");

    if (f == NULL)
    {
        printf("Khong mo duoc file %s!\n", FILE_THE_LOAI);
        return;
    }

    for (i = 0; i < soLuong; i++)
    {
        fprintf(f, "%d|%s\n",
                danhSachTheLoai[i]->maTheLoai,
                danhSachTheLoai[i]->tenTheLoai);
    }

    fclose(f);
    printf("Da luu danh sach the loai vao file!\n");
}

/* ==================== DOC FILE ==================== */

void docTheLoaiTuFile(void)
{
    FILE *f = fopen(FILE_THE_LOAI, "r");

    if (f == NULL)
    {
        return;
    }

    int ma;
    char ten[50];

    while (soLuong < MAX &&
           fscanf(f, "%d|%49[^\n]", &ma, ten) == 2)
    {
        struct theLoai *theLoaiPhim = malloc(sizeof(struct theLoai));

        if (theLoaiPhim == NULL)
        {
            printf("Khong du bo nho khi doc the loai!\n");
            break;
        }

        theLoaiPhim->maTheLoai = ma;
        strcpy(theLoaiPhim->tenTheLoai, ten);

        danhSachTheLoai[soLuong++] = theLoaiPhim;

        /* Bo ky tu xuong dong */
        int c;
        while ((c = fgetc(f)) != '\n' && c != EOF)
        {
            /* Bo qua */
        }
    }

    fclose(f);
}

void docPhimTuFile(void)
{
    FILE *f = fopen(FILE_PHIM, "r");

    if (f == NULL)
    {
        return;
    }

    int maPhim, namSX, loaiPhim, doTuoi;
    char tenPhim[50], daoDien[50];
    float thoiLuong;

    while (soLuongPhim < MAX &&
           fscanf(f, "%d|%49[^|]|%49[^|]|%d|%d|%f|%d",
                  &maPhim,
                  tenPhim,
                  daoDien,
                  &namSX,
                  &loaiPhim,
                  &thoiLuong,
                  &doTuoi) == 7)
    {
        struct typePhim *dataPhim = malloc(sizeof(struct typePhim));

        if (dataPhim == NULL)
        {
            printf("Khong du bo nho khi doc phim!\n");
            break;
        }

        dataPhim->maPhim = maPhim;
        strcpy(dataPhim->tenPhim, tenPhim);
        strcpy(dataPhim->daoDien, daoDien);
        dataPhim->namSX = namSX;
        dataPhim->loaiPhim = loaiPhim;
        dataPhim->thoiLuong = thoiLuong;
        dataPhim->doTuoi = doTuoi;

        danhSachPhim[soLuongPhim++] = dataPhim;

        int c;
        while ((c = fgetc(f)) != '\n' && c != EOF)
        {
            /* Bo qua */
        }
    }

    fclose(f);
}

/* ==================== XUAT DANH SACH ==================== */

void xuatTheLoai(void)
{
    int i = 0;
    printf("\n========== DANH SACH THE LOAI ==========\n");

    if (soLuong == 0)
    {
        printf("Chua co the loai nao!\n");
        return;
    }

    for (i = 0; i < soLuong; i++)
    {
        printf("Ma the loai: %d | Ten the loai: %s\n",
               danhSachTheLoai[i]->maTheLoai,
               danhSachTheLoai[i]->tenTheLoai);
    }
}

void xuatphim(void)
{
    int i = 0;
    printf("\n================ DANH SACH PHIM ================\n");

    if (soLuongPhim == 0)
    {
        printf("Chua co phim nao!\n");
        return;
    }

    for (i = 0; i < soLuongPhim; i++)
    {
        printf("Ma phim: %d | Ten phim: %s | Dao dien: %s | "
               "Nam SX: %d | Ma the loai: %d | "
               "Thoi luong: %.3f | Do tuoi: %d\n",
               danhSachPhim[i]->maPhim,
               danhSachPhim[i]->tenPhim,
               danhSachPhim[i]->daoDien,
               danhSachPhim[i]->namSX,
               danhSachPhim[i]->loaiPhim,
               danhSachPhim[i]->thoiLuong,
               danhSachPhim[i]->doTuoi);
    }
}

/* ==================== CRUD THE LOAI ==================== */

void themTheLoai(void)
{
    if (soLuong >= MAX)
    {
        printf("Danh sach the loai da day!\n");
        return;
    }

    struct theLoai *data = malloc(sizeof(struct theLoai));

    if (data == NULL)
    {
        printf("Khong du bo nho!\n");
        return;
    }

    if (!nhapSoNguyen("Nhap ma the loai: ", &data->maTheLoai))
    {
        free(data);
        return;
    }

    if (timViTriTheLoai(data->maTheLoai) != -1)
    {
        printf("Ma the loai da ton tai!\n");
        free(data);
        return;
    }

    if (!nhapChuoi("Nhap ten the loai: ", data->tenTheLoai, 50))
    {
        free(data);
        return;
    }

    danhSachTheLoai[soLuong++] = data;
    luuTheLoaiVaoFile();
}

void suaTheLoai(void)
{
    int maCanSua;

    if (!nhapSoNguyen("Nhap ma the loai can sua: ", &maCanSua))
    {
        return;
    }

    int viTri = timViTriTheLoai(maCanSua);

    if (viTri == -1)
    {
        printf("Khong tim thay the loai co ma %d!\n", maCanSua);
        return;
    }

    char tenMoi[50];

    if (!nhapChuoi("Nhap ten the loai moi: ", tenMoi, 50))
    {
        return;
    }

    char tenCu[50];
    strcpy(tenCu, danhSachTheLoai[viTri]->tenTheLoai);
    strcpy(danhSachTheLoai[viTri]->tenTheLoai, tenMoi);

    printf("Da sua %s --> %s\n",
           tenCu,
           danhSachTheLoai[viTri]->tenTheLoai);

    luuTheLoaiVaoFile();
}

void xoaTheLoai(void)
{
    int j = 0;
    int maCanXoa;

    if (!nhapSoNguyen("Nhap ma the loai can xoa: ", &maCanXoa))
    {
        return;
    }

    int viTri = timViTriTheLoai(maCanXoa);

    if (viTri == -1)
    {
        printf("Khong tim thay the loai!\n");
        return;
    }

    if (tonTaiPhimTheoTheLoai(maCanXoa))
    {
        printf("Khong the xoa: van con phim thuoc the loai nay!\n");
        return;
    }

    free(danhSachTheLoai[viTri]);

    for (j = viTri; j < soLuong - 1; j++)
    {
        danhSachTheLoai[j] = danhSachTheLoai[j + 1];
    }

    soLuong--;
    danhSachTheLoai[soLuong] = NULL;

    printf("Da xoa the loai thanh cong!\n");
    luuTheLoaiVaoFile();
}

void crudLoaiPhim(void)
{
    int subChoice;

    do
    {
        printf("\n========= QUAN LY THE LOAI =========\n");
        printf("1. Danh sach the loai phim\n");
        printf("2. Them the loai phim\n");
        printf("3. Sua the loai phim\n");
        printf("4. Xoa the loai phim\n");
        printf("0. Quay lai\n");

        if (!nhapSoNguyen("Chon chuc nang: ", &subChoice))
        {
            continue;
        }

        switch (subChoice)
        {
        case 1:
            xuatTheLoai();
            break;
        case 2:
            themTheLoai();
            break;
        case 3:
            suaTheLoai();
            break;
        case 4:
            xoaTheLoai();
            break;
        case 0:
            break;
        default:
            printf("Lua chon khong hop le!\n");
        }
    } while (subChoice != 0);
}

/* ==================== CRUD PHIM ==================== */

void themPhim(void)
{
    if (soLuongPhim >= MAX)
    {
        printf("Danh sach phim da day!\n");
        return;
    }

    if (soLuong == 0)
    {
        printf("Can them the loai truoc khi them phim!\n");
        return;
    }

    struct typePhim *dataPhim = malloc(sizeof(struct typePhim));

    if (dataPhim == NULL)
    {
        printf("Khong du bo nho!\n");
        return;
    }

    if (!nhapSoNguyen("Nhap ID phim: ", &dataPhim->maPhim))
    {
        free(dataPhim);
        return;
    }

    if (timViTriPhim(dataPhim->maPhim) != -1)
    {
        printf("Ma phim da ton tai!\n");
        free(dataPhim);
        return;
    }

    xuatTheLoai();

    if (!nhapSoNguyen("Nhap ID the loai: ", &dataPhim->loaiPhim))
    {
        free(dataPhim);
        return;
    }

    if (timViTriTheLoai(dataPhim->loaiPhim) == -1)
    {
        printf("Ma the loai khong ton tai!\n");
        free(dataPhim);
        return;
    }

    if (!nhapChuoi("Nhap ten phim: ", dataPhim->tenPhim, 50) ||
        !nhapChuoi("Nhap dao dien: ", dataPhim->daoDien, 50) ||
        !nhapSoNguyen("Nhap nam san xuat: ", &dataPhim->namSX) ||
        !nhapSoThuc("Nhap thoi luong: ", &dataPhim->thoiLuong) ||
        !nhapSoNguyen("Nhap do tuoi: ", &dataPhim->doTuoi))
    {
        free(dataPhim);
        return;
    }

    danhSachPhim[soLuongPhim++] = dataPhim;

    printf("Them phim thanh cong!\n");
    luuPhimVaoFile();
}

void suaPhim(void)
{
    int maCanSua;

    if (!nhapSoNguyen("Nhap ma phim can sua: ", &maCanSua))
    {
        return;
    }

    int viTri = timViTriPhim(maCanSua);

    if (viTri == -1)
    {
        printf("Khong tim thay phim co ma %d!\n", maCanSua);
        return;
    }

    int maTheLoaiMoi;

    xuatTheLoai();

    if (!nhapSoNguyen("Nhap ma the loai moi: ", &maTheLoaiMoi))
    {
        return;
    }

    if (timViTriTheLoai(maTheLoaiMoi) == -1)
    {
        printf("Ma the loai khong ton tai! Khong cap nhat phim.\n");
        return;
    }

    char tenMoi[50];
    char daoDienMoi[50];
    int namSXMoi;
    float thoiLuongMoi;
    int doTuoiMoi;

    if (!nhapChuoi("Nhap ten phim moi: ", tenMoi, 50) ||
        !nhapChuoi("Nhap dao dien moi: ", daoDienMoi, 50) ||
        !nhapSoNguyen("Nhap nam san xuat moi: ", &namSXMoi) ||
        !nhapSoThuc("Nhap thoi luong moi: ", &thoiLuongMoi) ||
        !nhapSoNguyen("Nhap do tuoi moi: ", &doTuoiMoi))
    {
        return;
    }

    danhSachPhim[viTri]->loaiPhim = maTheLoaiMoi;
    strcpy(danhSachPhim[viTri]->tenPhim, tenMoi);
    strcpy(danhSachPhim[viTri]->daoDien, daoDienMoi);
    danhSachPhim[viTri]->namSX = namSXMoi;
    danhSachPhim[viTri]->thoiLuong = thoiLuongMoi;
    danhSachPhim[viTri]->doTuoi = doTuoiMoi;

    printf("Da sua thong tin phim thanh cong!\n");
    luuPhimVaoFile();
}

void xoaPhim(void)
{
    int j = 0;
    int maCanXoa;

    if (!nhapSoNguyen("Nhap ma phim can xoa: ", &maCanXoa))
    {
        return;
    }

    int viTri = timViTriPhim(maCanXoa);

    if (viTri == -1)
    {
        printf("Khong tim thay phim!\n");
        return;
    }

    free(danhSachPhim[viTri]);

    for (j = viTri; j < soLuongPhim - 1; j++)
    {
        danhSachPhim[j] = danhSachPhim[j + 1];
    }

    soLuongPhim--;
    danhSachPhim[soLuongPhim] = NULL;

    printf("Da xoa phim thanh cong!\n");
    luuPhimVaoFile();
}

void crudPhim(void)
{
    int subChoice;

    do
    {
        printf("\n============= QUAN LY PHIM =============\n");
        printf("1. Danh sach phim\n");
        printf("2. Them phim\n");
        printf("3. Sua phim\n");
        printf("4. Xoa phim\n");
        printf("0. Quay lai\n");

        if (!nhapSoNguyen("Chon chuc nang: ", &subChoice))
        {
            continue;
        }

        switch (subChoice)
        {
        case 1:
            xuatphim();
            break;
        case 2:
            themPhim();
            break;
        case 3:
            suaPhim();
            break;
        case 4:
            xoaPhim();
            break;
        case 0:
            break;
        default:
            printf("Lua chon khong hop le!\n");
        }
    } while (subChoice != 0);
}

/* ==================== MENU QUAN LY ==================== */

void functionPhim(void)
{
    int choice;

    do
    {
        printf("\n========== QUAN LY PHIM ==========\n");
        printf("1. Quan ly the loai phim\n");
        printf("2. Quan ly phim\n");
        printf("0. Quay lai menu chinh\n");

        if (!nhapSoNguyen("Chon chuc nang: ", &choice))
        {
            continue;
        }

        switch (choice)
        {
        case 1:
            crudLoaiPhim();
            break;
        case 2:
            crudPhim();
            break;
        case 0:
            break;
        default:
            printf("Lua chon khong hop le!\n");
        }
    } while (choice != 0);
}

/* Giai phong bo nho da cap phat */
void giaiPhongBoNho(void)
{
    int i = 0;
    for (i = 0; i < soLuong; i++)
    {
        free(danhSachTheLoai[i]);
        danhSachTheLoai[i] = NULL;
    }

    for (i = 0; i < soLuongPhim; i++)
    {
        free(danhSachPhim[i]);
        danhSachPhim[i] = NULL;
    }

    soLuong = 0;
    soLuongPhim = 0;
}

int main(void)
{
    int choice;

    docTheLoaiTuFile();
    docPhimTuFile();

    do
    {
        printf("\n=============== MENU CHINH ===============\n");
        printf("1. Quan ly phim\n");
        printf("2. Quan ly phong / ghe\n");
        printf("3. Quan ly suat chieu\n");
        printf("4. Dat ve xem phim\n");
        printf("5. Quan ly ve\n");
        printf("6. Thong ke doanh thu\n");
        printf("7. Thoat chuong trinh\n");

        if (!nhapSoNguyen("Chon menu: ", &choice))
        {
            continue;
        }

        switch (choice)
        {
        case 1:
            functionPhim();
            break;
        case 2:
            printf("Quan ly ghe\n");
            break;
        case 3:
            printf("Quan ly suat chieu\n");
            break;
        case 4:
            printf("Dat ve xem phim\n");
            break;
        case 5:
            printf("Quan ly ve\n");
            break;
        case 6:
            printf("Thong ke doanh thu\n");
            break;
        case 7:
            printf("Thoat chuong trinh\n");
            break;
        default:
            printf("Lua chon khong hop le!\n");
        }

    } while (choice != 7);

    giaiPhongBoNho();
    return 0;
}
