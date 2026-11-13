#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define FILE_THE_LOAI "theloai.txt"
#define FILE_PHIM "phim.txt"
#define FILE_SUAT_CHIEU "suatchieu.txt"


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
    float thoiLuong; /* Tinh bang phut */
    int doTuoi;
};

struct ngayThang
{
    int ngay;
    int thang;
    int nam;
};

struct gioPhut
{
    int gio;
    int phut;
};

struct suatChieu
{
    int maSuat;
    int maPhim;
    struct ngayThang ngayChieu;
    struct gioPhut gioChieu;
    int phongChieu;
    float giaVe;
    int soVeDaDat;
};


struct theLoai *danhSachTheLoai[MAX] = {NULL};
struct typePhim *danhSachPhim[MAX] = {NULL};
struct suatChieu *danhSachSuatChieu[MAX] = {NULL};

int soLuong = 0;
int soLuongPhim = 0;
int soLuongSuatChieu = 0;


void checkInput(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

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

int nhapChuoi(const char *thongBao, char *chuoi, int kichThuoc)
{
    int c;
    printf("%s", thongBao);
    if (fgets(chuoi, kichThuoc, stdin) == NULL)
        return 0;

    chuoi[strcspn(chuoi, "\n")] = '\0';
    if (strchr(chuoi, '\0') != NULL)
    {
        if (strlen(chuoi) == (size_t)(kichThuoc - 1))
        {
            while ((c = getchar()) != '\n' && c != EOF)
            {
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
    int i;
    for (i = 0; i < soLuong; i++)
    {
        if (danhSachTheLoai[i] != NULL && danhSachTheLoai[i]->maTheLoai == maTheLoai)
            return i;
    }
    return -1;
}

int timViTriPhim(int maPhim)
{
    int i;
    for (i = 0; i < soLuongPhim; i++)
    {
        if (danhSachPhim[i] != NULL && danhSachPhim[i]->maPhim == maPhim)
            return i;
    }
    return -1;
}

int tonTaiPhimTheoTheLoai(int maTheLoai)
{
    int i;
    for (i = 0; i < soLuongPhim; i++)
    {
        if (danhSachPhim[i] != NULL && danhSachPhim[i]->loaiPhim == maTheLoai)
            return 1;
    }
    return 0;
}

int kiemTraNgayHopLe(int d, int m, int y)
{
    int nhuan;
    if (y < 2023 || m < 1 || m > 12 || d < 1 || d > 31)
        return 0;
    if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30)
        return 0;
    if (m == 2)
    {
        nhuan = ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0));
        if (d > 28 + nhuan)
            return 0;
    }
    return 1;
}

int kiemTraTrungLich(int maSuatDangXet, int phong, struct ngayThang nc, struct gioPhut gc, float thoiLuong)
{
    int startDangXet = gc.gio * 60 + gc.phut;
    int endDangXet = startDangXet + (int)thoiLuong;
    int i, vtPhim, startDaCo, endDaCo;
    float tlDaCo;

    for (i = 0; i < soLuongSuatChieu; i++)
    {
        if (danhSachSuatChieu[i] == NULL)
            continue;
        if (maSuatDangXet != -1 && danhSachSuatChieu[i]->maSuat == maSuatDangXet)
            continue;

        if (danhSachSuatChieu[i]->phongChieu == phong &&
            danhSachSuatChieu[i]->ngayChieu.ngay == nc.ngay &&
            danhSachSuatChieu[i]->ngayChieu.thang == nc.thang &&
            danhSachSuatChieu[i]->ngayChieu.nam == nc.nam)
        {

            vtPhim = timViTriPhim(danhSachSuatChieu[i]->maPhim);
            tlDaCo = (vtPhim != -1) ? danhSachPhim[vtPhim]->thoiLuong : 120.0;

            startDaCo = danhSachSuatChieu[i]->gioChieu.gio * 60 + danhSachSuatChieu[i]->gioChieu.phut;
            endDaCo = startDaCo + (int)tlDaCo;

            if (startDangXet < (endDaCo + 15) && (endDangXet + 15) > startDaCo)
            {
                return 1;
            }
        }
    }
    return 0;
}


void luuTheLoaiVaoFile(void)
{
    int i;
    FILE *f = fopen(FILE_THE_LOAI, "w");
    if (f == NULL)
        return;
    for (i = 0; i < soLuong; i++)
    {
        fprintf(f, "%d|%s\n", danhSachTheLoai[i]->maTheLoai, danhSachTheLoai[i]->tenTheLoai);
    }
    fclose(f);
}

void luuPhimVaoFile(void)
{
    int i;
    FILE *f = fopen(FILE_PHIM, "w");
    if (f == NULL)
        return;
    for (i = 0; i < soLuongPhim; i++)
    {
        fprintf(f, "%d|%s|%s|%d|%d|%.3f|%d\n",
                danhSachPhim[i]->maPhim, danhSachPhim[i]->tenPhim, danhSachPhim[i]->daoDien,
                danhSachPhim[i]->namSX, danhSachPhim[i]->loaiPhim, danhSachPhim[i]->thoiLuong, danhSachPhim[i]->doTuoi);
    }
    fclose(f);
}

void luuSuatChieuVaoFile(void)
{
    int i;
    FILE *f = fopen(FILE_SUAT_CHIEU, "w");
    if (f == NULL)
        return;
    for (i = 0; i < soLuongSuatChieu; i++)
    {
        fprintf(f, "%d|%d|%d|%d|%d|%d|%d|%d|%.0f|%d\n",
                danhSachSuatChieu[i]->maSuat, danhSachSuatChieu[i]->maPhim,
                danhSachSuatChieu[i]->ngayChieu.ngay, danhSachSuatChieu[i]->ngayChieu.thang, danhSachSuatChieu[i]->ngayChieu.nam,
                danhSachSuatChieu[i]->gioChieu.gio, danhSachSuatChieu[i]->gioChieu.phut,
                danhSachSuatChieu[i]->phongChieu, danhSachSuatChieu[i]->giaVe, danhSachSuatChieu[i]->soVeDaDat);
    }
    fclose(f);
}

void docTheLoaiTuFile(void)
{
    FILE *f;
    int ma, c;
    char ten[50];
    struct theLoai *data;

    f = fopen(FILE_THE_LOAI, "r");
    if (f == NULL)
        return;

    while (soLuong < MAX && fscanf(f, "%d|%49[^\n]", &ma, ten) == 2)
    {
        data = malloc(sizeof(struct theLoai));
        if (data == NULL)
            break;
        data->maTheLoai = ma;
        strcpy(data->tenTheLoai, ten);
        danhSachTheLoai[soLuong++] = data;
        while ((c = fgetc(f)) != '\n' && c != EOF)
        {
        }
    }
    fclose(f);
}

void docPhimTuFile(void)
{
    FILE *f;
    struct typePhim temp;
    struct typePhim *data;
    int c;

    f = fopen(FILE_PHIM, "r");
    if (f == NULL)
        return;

    while (soLuongPhim < MAX && fscanf(f, "%d|%49[^|]|%49[^|]|%d|%d|%f|%d",
                                       &temp.maPhim, temp.tenPhim, temp.daoDien, &temp.namSX,
                                       &temp.loaiPhim, &temp.thoiLuong, &temp.doTuoi) == 7)
    {
        data = malloc(sizeof(struct typePhim));
        if (data == NULL)
            break;
        *data = temp;
        danhSachPhim[soLuongPhim++] = data;
        while ((c = fgetc(f)) != '\n' && c != EOF)
        {
        }
    }
    fclose(f);
}

void docSuatChieuTuFile(void)
{
    FILE *f;
    struct suatChieu temp;
    struct suatChieu *data;

    f = fopen(FILE_SUAT_CHIEU, "r");
    if (f == NULL)
        return;

    while (soLuongSuatChieu < MAX && fscanf(f, "%d|%d|%d|%d|%d|%d|%d|%d|%f|%d",
                                            &temp.maSuat, &temp.maPhim, &temp.ngayChieu.ngay, &temp.ngayChieu.thang, &temp.ngayChieu.nam,
                                            &temp.gioChieu.gio, &temp.gioChieu.phut, &temp.phongChieu, &temp.giaVe, &temp.soVeDaDat) == 10)
    {
        data = malloc(sizeof(struct suatChieu));
        if (data == NULL)
            break;
        *data = temp;
        danhSachSuatChieu[soLuongSuatChieu++] = data;
    }
    fclose(f);
}


void xuatTheLoai(void)
{
    int i;
    printf("\n========== DANH SACH THE LOAI ==========\n");
    if (soLuong == 0)
    {
        printf("Chua co the loai nao!\n");
        return;
    }
    for (i = 0; i < soLuong; i++)
    {
        printf("Ma: %d | Ten: %s\n", danhSachTheLoai[i]->maTheLoai, danhSachTheLoai[i]->tenTheLoai);
    }
}

void xuatphim(void)
{
    int i;
    printf("\n================ DANH SACH PHIM ================\n");
    if (soLuongPhim == 0)
    {
        printf("Chua co phim nao!\n");
        return;
    }
    for (i = 0; i < soLuongPhim; i++)
    {
        printf("Ma: %d | Phim: %s | DDien: %s | Nam: %d | TL: %d | Dai: %.0f phut | Tuoi: %d\n",
               danhSachPhim[i]->maPhim, danhSachPhim[i]->tenPhim, danhSachPhim[i]->daoDien,
               danhSachPhim[i]->namSX, danhSachPhim[i]->loaiPhim, danhSachPhim[i]->thoiLuong, danhSachPhim[i]->doTuoi);
    }
}


void themTheLoai(void)
{
    struct theLoai *data;
    if (soLuong >= MAX)
        return;

    data = malloc(sizeof(struct theLoai));
    if (!nhapSoNguyen("Nhap ma the loai: ", &data->maTheLoai))
    {
        free(data);
        return;
    }
    if (timViTriTheLoai(data->maTheLoai) != -1)
    {
        printf("Da ton tai!\n");
        free(data);
        return;
    }
    if (!nhapChuoi("Nhap ten: ", data->tenTheLoai, 50))
    {
        free(data);
        return;
    }

    danhSachTheLoai[soLuong++] = data;
    luuTheLoaiVaoFile();
    printf("Thanh cong!\n");
}

void suaTheLoai(void)
{
    int ma, vt;
    if (!nhapSoNguyen("Nhap ma the loai can sua: ", &ma))
        return;

    vt = timViTriTheLoai(ma);
    if (vt == -1)
    {
        printf("Khong tim thay!\n");
        return;
    }
    if (!nhapChuoi("Nhap ten moi: ", danhSachTheLoai[vt]->tenTheLoai, 50))
        return;

    luuTheLoaiVaoFile();
    printf("Thanh cong!\n");
}

void xoaTheLoai(void)
{
    int ma, vt, j;
    if (!nhapSoNguyen("Nhap ma the loai can xoa: ", &ma))
        return;

    vt = timViTriTheLoai(ma);
    if (vt == -1)
    {
        printf("Khong tim thay!\n");
        return;
    }
    if (tonTaiPhimTheoTheLoai(ma))
    {
        printf("Khong the xoa, con phim lien quan!\n");
        return;
    }

    free(danhSachTheLoai[vt]);
    for (j = vt; j < soLuong - 1; j++)
        danhSachTheLoai[j] = danhSachTheLoai[j + 1];
    danhSachTheLoai[--soLuong] = NULL;

    luuTheLoaiVaoFile();
    printf("Thanh cong!\n");
}

void crudLoaiPhim(void)
{
    int choice;
    do
    {
        printf("\n--- QUAN LY THE LOAI ---\n1. Xem\n2. Them\n3. Sua\n4. Xoa\n0. Back\n");
        if (!nhapSoNguyen("Chon: ", &choice))
            continue;
        switch (choice)
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
        }
    } while (choice != 0);
}


void themPhim(void)
{
    struct typePhim *data;
    if (soLuongPhim >= MAX || soLuong == 0)
    {
        printf("Day hoac chua co The loai!\n");
        return;
    }

    data = malloc(sizeof(struct typePhim));
    if (!nhapSoNguyen("Nhap ID phim: ", &data->maPhim))
    {
        free(data);
        return;
    }
    if (timViTriPhim(data->maPhim) != -1)
    {
        printf("Trung ID!\n");
        free(data);
        return;
    }

    xuatTheLoai();
    if (!nhapSoNguyen("Nhap ID the loai: ", &data->loaiPhim) || timViTriTheLoai(data->loaiPhim) == -1)
    {
        free(data);
        return;
    }
    if (!nhapChuoi("Ten phim: ", data->tenPhim, 50) || !nhapChuoi("Dao dien: ", data->daoDien, 50) ||
        !nhapSoNguyen("Nam SX: ", &data->namSX) || !nhapSoThuc("Thoi luong (phut): ", &data->thoiLuong) ||
        !nhapSoNguyen("Do tuoi: ", &data->doTuoi))
    {
        free(data);
        return;
    }

    danhSachPhim[soLuongPhim++] = data;
    luuPhimVaoFile();
    printf("Thanh cong!\n");
}

void suaPhim(void)
{
    int ma, vt;
    if (!nhapSoNguyen("Nhap ma phim can sua: ", &ma))
        return;

    vt = timViTriPhim(ma);
    if (vt == -1)
    {
        printf("Khong thay!\n");
        return;
    }

    xuatTheLoai();
    if (!nhapSoNguyen("Nhap ma TL moi: ", &danhSachPhim[vt]->loaiPhim) || timViTriTheLoai(danhSachPhim[vt]->loaiPhim) == -1)
        return;
    if (!nhapChuoi("Ten: ", danhSachPhim[vt]->tenPhim, 50) || !nhapChuoi("DD: ", danhSachPhim[vt]->daoDien, 50) ||
        !nhapSoNguyen("Nam: ", &danhSachPhim[vt]->namSX) || !nhapSoThuc("Thoi luong: ", &danhSachPhim[vt]->thoiLuong) ||
        !nhapSoNguyen("Tuoi: ", &danhSachPhim[vt]->doTuoi))
        return;

    luuPhimVaoFile();
    printf("Thanh cong!\n");
}

void xoaPhim(void)
{
    int ma, vt, j;
    if (!nhapSoNguyen("Nhap ma can xoa: ", &ma))
        return;

    vt = timViTriPhim(ma);
    if (vt == -1)
    {
        printf("Khong tim thay!\n");
        return;
    }

    free(danhSachPhim[vt]);
    for (j = vt; j < soLuongPhim - 1; j++)
        danhSachPhim[j] = danhSachPhim[j + 1];
    danhSachPhim[--soLuongPhim] = NULL;

    luuPhimVaoFile();
    printf("Thanh cong!\n");
}

void crudPhim(void)
{
    int choice;
    do
    {
        printf("\n--- QUAN LY PHIM ---\n1. Xem\n2. Them\n3. Sua\n4. Xoa\n0. Back\n");
        if (!nhapSoNguyen("Chon: ", &choice))
            continue;
        switch (choice)
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
        }
    } while (choice != 0);
}

void functionPhim(void)
{
    int choice;
    do
    {
        printf("\n========== MENU PHIM ==========\n1. Quan ly The Loai\n2. Quan ly Phim\n0. Quay lai\n");
        if (!nhapSoNguyen("Chon: ", &choice))
            continue;
        if (choice == 1)
            crudLoaiPhim();
        else if (choice == 2)
            crudPhim();
    } while (choice != 0);
}


void hienThiSuatChieu(void)
{
    int i, vt;
    printf("\n================ DANH SACH SUAT CHIEU ================\n");
    if (soLuongSuatChieu == 0)
    {
        printf("Chua co suat chieu nao!\n");
        return;
    }
    for (i = 0; i < soLuongSuatChieu; i++)
    {
        vt = timViTriPhim(danhSachSuatChieu[i]->maPhim);
        printf("ID: %d | Phim: %s | Ngay: %02d/%02d/%d | Gio: %02d:%02d | Phong: %d | Gia: %.0f | Da dat: %d\n",
               danhSachSuatChieu[i]->maSuat, (vt != -1 ? danhSachPhim[vt]->tenPhim : "N/A"),
               danhSachSuatChieu[i]->ngayChieu.ngay, danhSachSuatChieu[i]->ngayChieu.thang, danhSachSuatChieu[i]->ngayChieu.nam,
               danhSachSuatChieu[i]->gioChieu.gio, danhSachSuatChieu[i]->gioChieu.phut,
               danhSachSuatChieu[i]->phongChieu, danhSachSuatChieu[i]->giaVe, danhSachSuatChieu[i]->soVeDaDat);
    }
}

void themSuatChieu(void)
{
    struct suatChieu *data;
    int vtPhim, i;

    if (soLuongSuatChieu >= MAX)
        return;
    if (soLuongPhim == 0)
    {
        printf("Vui long them phim truoc!\n");
        return;
    }

    data = malloc(sizeof(struct suatChieu));
    if (!nhapSoNguyen("Nhap ma suat: ", &data->maSuat))
    {
        free(data);
        return;
    }

    for (i = 0; i < soLuongSuatChieu; i++)
    {
        if (danhSachSuatChieu[i]->maSuat == data->maSuat)
        {
            printf("Ma suat da ton tai!\n");
            free(data);
            return;
        }
    }

    xuatphim();
    if (!nhapSoNguyen("Nhap ma phim: ", &data->maPhim))
    {
        free(data);
        return;
    }
    vtPhim = timViTriPhim(data->maPhim);
    if (vtPhim == -1)
    {
        printf("Loi: Phim khong ton tai!\n");
        free(data);
        return;
    }

    printf("Ngay chieu (DD MM YYYY): ");
    if (scanf("%d %d %d", &data->ngayChieu.ngay, &data->ngayChieu.thang, &data->ngayChieu.nam) != 3 ||
        !kiemTraNgayHopLe(data->ngayChieu.ngay, data->ngayChieu.thang, data->ngayChieu.nam))
    {
        printf("Ngay khong hop le!\n");
        checkInput();
        free(data);
        return;
    }
    checkInput();

    printf("Gio chieu (HH MM): ");
    if (scanf("%d %d", &data->gioChieu.gio, &data->gioChieu.phut) != 2 ||
        data->gioChieu.gio < 0 || data->gioChieu.gio > 23 || data->gioChieu.phut < 0 || data->gioChieu.phut > 59)
    {
        printf("Gio khong hop le!\n");
        checkInput();
        free(data);
        return;
    }
    checkInput();

    if (!nhapSoNguyen("Phong chieu (doi merge UC 3): ", &data->phongChieu) || data->phongChieu < 1 || data->phongChieu > 10)
    {
        printf("Phong khong hop le!\n");
        free(data);
        return;
    }

    if (kiemTraTrungLich(-1, data->phongChieu, data->ngayChieu, data->gioChieu, danhSachPhim[vtPhim]->thoiLuong))
    {
        printf("Loi: Trung lich voi suat khac cung phong!\n");
        free(data);
        return;
    }

    if (!nhapSoThuc("Gia ve: ", &data->giaVe) || data->giaVe <= 0)
    {
        printf("Gia ve khong hop le!\n");
        free(data);
        return;
    }

    data->soVeDaDat = 0;
    danhSachSuatChieu[soLuongSuatChieu++] = data;
    luuSuatChieuVaoFile();
    printf("Them suat chieu thanh cong!\n");
}

void xoaSuatChieu(void)
{
    int ma, vt = -1, i;
    if (!nhapSoNguyen("Nhap ma suat can xoa: ", &ma))
        return;

    for (i = 0; i < soLuongSuatChieu; i++)
    {
        if (danhSachSuatChieu[i]->maSuat == ma)
        {
            vt = i;
            break;
        }
    }
    if (vt == -1)
    {
        printf("Khong tim thay!\n");
        return;
    }
    if (danhSachSuatChieu[vt]->soVeDaDat > 0)
    {
        printf("Khong the xoa, da co khach dat ve!\n");
        return;
    }

    free(danhSachSuatChieu[vt]);
    for (i = vt; i < soLuongSuatChieu - 1; i++)
        danhSachSuatChieu[i] = danhSachSuatChieu[i + 1];
    danhSachSuatChieu[--soLuongSuatChieu] = NULL;

    luuSuatChieuVaoFile();
    printf("Xoa thanh cong!\n");
}

void capNhatSuatChieu(void)
{
    int ma, vt = -1, i, thoiLuongPhim, vtPhim;
    struct suatChieu temp;

    if (!nhapSoNguyen("Nhap ma suat can sua: ", &ma))
        return;

    for (i = 0; i < soLuongSuatChieu; i++)
    {
        if (danhSachSuatChieu[i]->maSuat == ma)
        {
            vt = i;
            break;
        }
    }
    if (vt == -1)
    {
        printf("Khong tim thay suat!\n");
        return;
    }
    if (danhSachSuatChieu[vt]->soVeDaDat > 0)
    {
        printf("Suat chieu da co khach dat, viec sua doi co the anh huong den ve cua khach!\n");
    }

    temp = *danhSachSuatChieu[vt];
    thoiLuongPhim = 120;
    vtPhim = timViTriPhim(temp.maPhim);
    if (vtPhim != -1)
        thoiLuongPhim = danhSachPhim[vtPhim]->thoiLuong;

    printf("Ngay chieu moi (DD MM YYYY): ");
    if (scanf("%d %d %d", &temp.ngayChieu.ngay, &temp.ngayChieu.thang, &temp.ngayChieu.nam) == 3 &&
        kiemTraNgayHopLe(temp.ngayChieu.ngay, temp.ngayChieu.thang, temp.ngayChieu.nam))
    {
        checkInput();
    }
    else
    {
        printf("Nhap sai! Giu nguyen gia tri cu.\n");
        checkInput();
    }

    printf("Gio chieu moi (HH MM): ");
    if (scanf("%d %d", &temp.gioChieu.gio, &temp.gioChieu.phut) == 2 &&
        temp.gioChieu.gio >= 0 && temp.gioChieu.gio <= 23 && temp.gioChieu.phut >= 0 && temp.gioChieu.phut <= 59)
    {
        checkInput();
    }
    else
    {
        printf("Nhap sai! Giu nguyen gia tri cu.\n");
        checkInput();
    }

    if (kiemTraTrungLich(ma, temp.phongChieu, temp.ngayChieu, temp.gioChieu, thoiLuongPhim))
    {
        printf("Thoi gian moi bi trung lich! Tu choi cap nhat.\n");
        return;
    }

    if (nhapSoThuc("Gia ve moi: ", &temp.giaVe) && temp.giaVe > 0)
    {
        *danhSachSuatChieu[vt] = temp;
        luuSuatChieuVaoFile();
        printf("Cap nhat thanh cong!\n");
    }
    else
    {
        printf("Loi nhap gia ve!\n");
    }
}

void timKiemSuatChieu(void)
{
    int ma, found = 0, i, vt;
    if (!nhapSoNguyen("Nhap ma suat hoac ma phim can tim: ", &ma))
        return;

    for (i = 0; i < soLuongSuatChieu; i++)
    {
        if (danhSachSuatChieu[i]->maSuat == ma || danhSachSuatChieu[i]->maPhim == ma)
        {
            vt = timViTriPhim(danhSachSuatChieu[i]->maPhim);
            printf("Thay suat: ID %d | Phim %s | Ngay %02d/%02d/%d | Gio %02d:%02d | Phong %d\n",
                   danhSachSuatChieu[i]->maSuat, (vt != -1 ? danhSachPhim[vt]->tenPhim : "N/A"),
                   danhSachSuatChieu[i]->ngayChieu.ngay, danhSachSuatChieu[i]->ngayChieu.thang, danhSachSuatChieu[i]->ngayChieu.nam,
                   danhSachSuatChieu[i]->gioChieu.gio, danhSachSuatChieu[i]->gioChieu.phut, danhSachSuatChieu[i]->phongChieu);
            found = 1;
        }
    }
    if (!found)
        printf("Khong tim thay ket qua!\n");
}

void functionSuatChieu(void)
{
    int choice;
    do
    {
        printf("\n========= QUAN LY SUAT CHIEU =========\n");
        printf("1. Xem tat ca suat chieu\n2. Tim kiem suat chieu\n3. Them suat chieu\n");
        printf("4. Cap nhat suat chieu\n5. Xoa suat chieu\n0. Quay lai\n");
        if (!nhapSoNguyen("Chon: ", &choice))
            continue;

        switch (choice)
        {
        case 1:
            hienThiSuatChieu();
            break;
        case 2:
            timKiemSuatChieu();
            break;
        case 3:
            themSuatChieu();
            break;
        case 4:
            capNhatSuatChieu();
            break;
        case 5:
            xoaSuatChieu();
            break;
        }
    } while (choice != 0);
}


void giaiPhongBoNho(void)
{
    int i;
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
    for (i = 0; i < soLuongSuatChieu; i++)
    {
        free(danhSachSuatChieu[i]);
        danhSachSuatChieu[i] = NULL;
    }
}

int main(void)
{
    int choice;

    docTheLoaiTuFile();
    docPhimTuFile();
    docSuatChieuTuFile();

    do
    {
        printf("\n=============== MENU CHINH ===============\n");
        printf("1. Quan ly Phim / The loai\n");
        printf("2. Quan ly Suat chieu\n");
        printf("3. Quan ly Phong / Ghe (Tam thoi chua co)\n");
        printf("4. Dat ve xem phim (Sap toi)\n");
        printf("5. Quan ly ve (Sap toi)\n");
        printf("6. Thong ke doanh thu\n");
        printf("7. Thoat\n");

        if (!nhapSoNguyen("Chon menu: ", &choice))
            continue;

        switch (choice)
        {
        case 1:
            functionPhim();
            break;
        case 2:
            functionSuatChieu();
            break;
        case 3:
            printf("Dang phat trien...\n");
            break;
        case 4:
            printf("Dang phat trien...\n");
            break;
        case 5:
            printf("Dang phat trien...\n");
            break;
        case 6:
            printf("Dang phat trien...\n");
            break;
        case 7:
            printf("Hen gap lai!\n");
            break;
        default:
            printf("Lua chon khong hop le!\n");
        }
    } while (choice != 7);

    giaiPhongBoNho();
    return 0;
}