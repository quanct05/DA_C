/*
 * CHUONG TRINH DAT VE XEM PHIM TREN TERMINAL (C)
 * - Muc 1: Quan ly phim (phan cua ban) - giu nguyen, chi them luu vao mang
 * - Muc 5: Quan ly ve, Muc 6: Thong ke doanh thu
 * - Muc 2, 3, 4: chua lam (dang de trong o menu chinh)
 *
 * Bien dich: gcc chuong_trinh_dat_ve.c -o datve
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#endif

/* ===================== HANG SO ===================== */
#define MAX_PHIM      100
#define MAX_THE_LOAI  50
#define MAX_SUAT      200
#define MAX_VE        1000
#define MAX_HANG      10     /* hang ghe A..J  */
#define MAX_COT       10     /* cot ghe 01..10 */
#define MAX_GHE_VE    10     /* so ghe toi da trong 1 ve */

#define VE_HIEU_LUC   1
#define VE_DA_HUY     0

/* ===================== STRUCT ===================== */
typedef struct {
    int ngay, thang, nam;
} Ngay;

typedef struct {
    int gio, phut;
} Gio;

/* ---- Struct cua phan Quan ly phim (giu nguyen cua ban) ---- */
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

typedef struct {
    char maSuat[10];
    int  maPhim;                      /* tham chieu typePhim.maPhim */
    Ngay ngayChieu;
    Gio  gioChieu;
    char phong[10];
    double giaVe;
    int  soHang, soCot;               /* kich thuoc so do ghe cua phong */
    int  ghe[MAX_HANG][MAX_COT];      /* 0 = trong, 1 = da dat */
} SuatChieu;

typedef struct {
    char maVe[12];
    char maSuat[10];
    int  maPhim;
    char dsGhe[MAX_GHE_VE][5];        /* vd: "A01" */
    double giaGhe[MAX_GHE_VE];        /* gia tung ghe TAI THOI DIEM DAT */
    int  soGhe;
    double tongTien;                  /* tong tien TAI THOI DIEM DAT */
    Ngay ngayDat;
    int  trangThai;                   /* VE_HIEU_LUC / VE_DA_HUY */
} Ve;

/* ===================== DU LIEU TOAN CUC ===================== */
struct theLoai  dsTheLoai[MAX_THE_LOAI]; int soTheLoai = 0;
struct typePhim dsPhim[MAX_PHIM];        int soPhim = 0;
SuatChieu dsSuat[MAX_SUAT];   int soSuat = 0;
Ve        dsVe[MAX_VE];       int soVe   = 0;

/* ===================== HAM TIEN ICH NHAP LIEU ===================== */
void xoaXuongDong(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}

void docChuoi(const char *nhacNhap, char *s, int kichThuoc) {
    printf("%s", nhacNhap);
    if (fgets(s, kichThuoc, stdin) == NULL) { s[0] = '\0'; return; }
    /* neu nhap qua dai thi bo phan con thua trong bo dem */
    if (strchr(s, '\n') == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    }
    xoaXuongDong(s);
}

/* Doc so nguyen trong khoang [min, max], nhap sai thi bat nhap lai */
int docSoNguyen(const char *nhacNhap, int min, int max) {
    char buf[64];
    char *end;
    long x;
    while (1) {
        docChuoi(nhacNhap, buf, sizeof(buf));
        if (buf[0] == '\0') { printf("  -> Khong duoc de trong.\n"); continue; }
        x = strtol(buf, &end, 10);
        while (*end == ' ') end++;
        if (*end != '\0') { printf("  -> Vui long nhap so nguyen.\n"); continue; }
        if (x < min || x > max) {
            printf("  -> Gia tri phai tu %d den %d.\n", min, max);
            continue;
        }
        return (int)x;
    }
}

void inHoa(char *s) {
    for (; *s; s++) *s = (char)toupper((unsigned char)*s);
}

/* So sanh chuoi khong phan biet hoa/thuong, tim chuoi con */
int chuaChuoiKhongPhanBiet(const char *chuoi, const char *tuKhoa) {
    char a[200], b[200];
    strncpy(a, chuoi, sizeof(a) - 1);  a[sizeof(a) - 1] = '\0';
    strncpy(b, tuKhoa, sizeof(b) - 1); b[sizeof(b) - 1] = '\0';
    inHoa(a); inHoa(b);
    return strstr(a, b) != NULL;
}

int ngayHopLe(Ngay d) {
    int ngayTrongThang[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (d.nam < 1900 || d.nam > 2100 || d.thang < 1 || d.thang > 12) return 0;
    if ((d.nam % 4 == 0 && d.nam % 100 != 0) || d.nam % 400 == 0)
        ngayTrongThang[1] = 29;
    return d.ngay >= 1 && d.ngay <= ngayTrongThang[d.thang - 1];
}

/* Nhap ngay dang dd/mm/yyyy */
Ngay docNgay(const char *nhacNhap) {
    char buf[32];
    Ngay d;
    while (1) {
        docChuoi(nhacNhap, buf, sizeof(buf));
        if (sscanf(buf, "%d/%d/%d", &d.ngay, &d.thang, &d.nam) == 3 && ngayHopLe(d))
            return d;
        printf("  -> Ngay khong hop le (dinh dang dd/mm/yyyy).\n");
    }
}

int soSanhNgay(Ngay a, Ngay b) {
    if (a.nam != b.nam)     return a.nam - b.nam;
    if (a.thang != b.thang) return a.thang - b.thang;
    return a.ngay - b.ngay;
}

void dungManHinh(void) {
    char tmp[8];
    docChuoi("\nNhan Enter de tiep tuc...", tmp, sizeof(tmp));
}

/* ===================== HAM TRA CUU ===================== */
int timChiSoPhim(int maPhim) {
    for (int i = 0; i < soPhim; i++)
        if (dsPhim[i].maPhim == maPhim) return i;
    return -1;
}

int timChiSoSuat(const char *maSuat) {
    for (int i = 0; i < soSuat; i++)
        if (strcmp(dsSuat[i].maSuat, maSuat) == 0) return i;
    return -1;
}

int timChiSoVe(const char *maVe) {
    for (int i = 0; i < soVe; i++)
        if (strcmp(dsVe[i].maVe, maVe) == 0) return i;
    return -1;
}

const char *layTenPhim(int maPhim) {
    int i = timChiSoPhim(maPhim);
    return i >= 0 ? dsPhim[i].tenPhim : "(phim da bi xoa)";
}

const char *chuoiTrangThai(int tt) {
    return tt == VE_HIEU_LUC ? "Hieu luc" : "Da huy";
}

/* Chuyen ma ghe "A01" -> (hang, cot). Tra ve 1 neu hop le */
int phanTichMaGhe(const char *maGhe, const SuatChieu *sc, int *hang, int *cot) {
    if (strlen(maGhe) < 2) return 0;
    char h = (char)toupper((unsigned char)maGhe[0]);
    int c = atoi(maGhe + 1);
    *hang = h - 'A';
    *cot  = c - 1;
    return *hang >= 0 && *hang < sc->soHang && *cot >= 0 && *cot < sc->soCot;
}

/* ============================================================
 *                    MUC 5: QUAN LY VE
 * ============================================================ */

void inTieuDeBangVe(void) {
    printf("%-10s %-25s %-8s %-11s %-6s %-20s %12s %-9s\n",
           "Ma ve", "Phim", "Ma suat", "Ngay chieu", "Gio", "Ghe",
           "Tong tien", "Trang thai");
    printf("------------------------------------------------------------"
           "------------------------------------------------\n");
}

void inMotDongVe(const Ve *v) {
    char chuoiGhe[64] = "";
    for (int j = 0; j < v->soGhe; j++) {
        if (j > 0) strcat(chuoiGhe, ",");
        if (strlen(chuoiGhe) + strlen(v->dsGhe[j]) + 2 >= sizeof(chuoiGhe)) {
            strcat(chuoiGhe, "..");
            break;
        }
        strcat(chuoiGhe, v->dsGhe[j]);
    }

    int is = timChiSoSuat(v->maSuat);
    char ngayChieu[12] = "--", gio[6] = "--";
    if (is >= 0) {
        sprintf(ngayChieu, "%02d/%02d/%04d", dsSuat[is].ngayChieu.ngay,
                dsSuat[is].ngayChieu.thang, dsSuat[is].ngayChieu.nam);
        sprintf(gio, "%02d:%02d", dsSuat[is].gioChieu.gio, dsSuat[is].gioChieu.phut);
    }

    printf("%-10s %-25.25s %-8s %-11s %-6s %-20.20s %12.0f %-9s\n",
           v->maVe, layTenPhim(v->maPhim), v->maSuat, ngayChieu, gio,
           chuoiGhe, v->tongTien, chuoiTrangThai(v->trangThai));
}

/* 5.1 Hien thi danh sach ve */
void hienThiDanhSachVe(void) {
    printf("\n===== DANH SACH VE =====\n");
    if (soVe == 0) { printf("Chua co ve nao.\n"); return; }

    int loc = docSoNguyen("Loc: 1. Tat ca  2. Chi ve hieu luc  3. Chi ve da huy\nChon: ", 1, 3);
    int dem = 0;
    inTieuDeBangVe();
    for (int i = 0; i < soVe; i++) {
        if (loc == 2 && dsVe[i].trangThai != VE_HIEU_LUC) continue;
        if (loc == 3 && dsVe[i].trangThai != VE_DA_HUY)   continue;
        inMotDongVe(&dsVe[i]);
        dem++;
    }
    printf("=> Tong cong: %d ve.\n", dem);
}

/* 5.3 Xem chi tiet ve */
void inChiTietVe(const Ve *v) {
    int is = timChiSoSuat(v->maSuat);
    printf("\n+------------------- CHI TIET VE -------------------+\n");
    printf("  Ma ve       : %s\n", v->maVe);
    printf("  Phim        : %s (ma %d)\n", layTenPhim(v->maPhim), v->maPhim);
    printf("  Suat chieu  : %s\n", v->maSuat);
    if (is >= 0) {
        const SuatChieu *sc = &dsSuat[is];
        printf("  Ngay chieu  : %02d/%02d/%04d\n",
               sc->ngayChieu.ngay, sc->ngayChieu.thang, sc->ngayChieu.nam);
        printf("  Gio chieu   : %02d:%02d\n", sc->gioChieu.gio, sc->gioChieu.phut);
        printf("  Phong       : %s\n", sc->phong);
    } else {
        printf("  (Suat chieu khong con ton tai)\n");
    }
    printf("  Ngay dat    : %02d/%02d/%04d\n",
           v->ngayDat.ngay, v->ngayDat.thang, v->ngayDat.nam);
    printf("  Ghe va gia (tai thoi diem dat):\n");
    for (int j = 0; j < v->soGhe; j++)
        printf("     - %-4s : %10.0f VND\n", v->dsGhe[j], v->giaGhe[j]);
    printf("  Tong tien   : %.0f VND\n", v->tongTien);
    printf("  Trang thai  : %s\n", chuoiTrangThai(v->trangThai));
    printf("+---------------------------------------------------+\n");
}

void xemChiTietVe(void) {
    char ma[12];
    docChuoi("Nhap ma ve: ", ma, sizeof(ma));
    inHoa(ma);
    int i = timChiSoVe(ma);
    if (i < 0) { printf("Khong tim thay ve co ma %s.\n", ma); return; }
    inChiTietVe(&dsVe[i]);
}

/* 5.2 Tim ve theo ma ve hoac thong tin suat chieu */
void timKiemVe(void) {
    printf("\n===== TIM KIEM VE =====\n");
    printf("1. Theo ma ve\n");
    printf("2. Theo ma suat chieu\n");
    printf("3. Theo ma hoac ten phim\n");
    printf("4. Theo ngay chieu\n");
    printf("5. Theo phong chieu\n");
    printf("0. Quay lai\n");
    int chon = docSoNguyen("Chon: ", 0, 5);
    if (chon == 0) return;

    char tuKhoa[100] = "";
    Ngay ngay = {0, 0, 0};

    if (chon == 1) {
        docChuoi("Nhap ma ve: ", tuKhoa, sizeof(tuKhoa));
        inHoa(tuKhoa);
        int i = timChiSoVe(tuKhoa);
        if (i < 0) printf("Khong tim thay ve co ma %s.\n", tuKhoa);
        else inChiTietVe(&dsVe[i]);
        return;
    }
    if (chon == 2) { docChuoi("Nhap ma suat chieu: ", tuKhoa, sizeof(tuKhoa)); inHoa(tuKhoa); }
    if (chon == 3)   docChuoi("Nhap ma hoac ten phim: ", tuKhoa, sizeof(tuKhoa));
    if (chon == 4)   ngay = docNgay("Nhap ngay chieu (dd/mm/yyyy): ");
    if (chon == 5) { docChuoi("Nhap phong chieu: ", tuKhoa, sizeof(tuKhoa)); inHoa(tuKhoa); }

    int dem = 0;
    for (int i = 0; i < soVe; i++) {
        const Ve *v = &dsVe[i];
        int is = timChiSoSuat(v->maSuat);
        int khop = 0;

        switch (chon) {
        case 2: khop = strcmp(v->maSuat, tuKhoa) == 0; break;
        case 3: khop = atoi(tuKhoa) == v->maPhim ||
                       chuaChuoiKhongPhanBiet(layTenPhim(v->maPhim), tuKhoa); break;
        case 4: khop = is >= 0 && soSanhNgay(dsSuat[is].ngayChieu, ngay) == 0; break;
        case 5: khop = is >= 0 && chuaChuoiKhongPhanBiet(dsSuat[is].phong, tuKhoa); break;
        }
        if (khop) {
            if (dem == 0) inTieuDeBangVe();
            inMotDongVe(v);
            dem++;
        }
    }
    if (dem == 0) printf("Khong tim thay ve phu hop.\n");
    else printf("=> Tim thay %d ve.\n", dem);
}

/* 5.5 Giai phong ghe khi huy ve */
void giaiPhongGheCuaVe(const Ve *v) {
    int is = timChiSoSuat(v->maSuat);
    if (is < 0) return;                       /* suat da bi xoa: khong con ghe de tra */
    SuatChieu *sc = &dsSuat[is];
    for (int j = 0; j < v->soGhe; j++) {
        int h, c;
        if (phanTichMaGhe(v->dsGhe[j], sc, &h, &c))
            sc->ghe[h][c] = 0;
    }
}

/* 5.4 Huy ve theo ma ve (5.5 giai phong ghe, 5.6 khong tinh doanh thu) */
void huyVe(void) {
    char ma[12], xacNhan[8];
    printf("\n===== HUY VE =====\n");
    docChuoi("Nhap ma ve can huy: ", ma, sizeof(ma));
    inHoa(ma);

    int i = timChiSoVe(ma);
    if (i < 0) { printf("Khong tim thay ve co ma %s.\n", ma); return; }

    Ve *v = &dsVe[i];
    if (v->trangThai == VE_DA_HUY) {
        printf("Ve %s da bi huy truoc do.\n", v->maVe);
        return;
    }

    inChiTietVe(v);
    docChuoi("Ban chac chan muon huy ve nay? (Y/N): ", xacNhan, sizeof(xacNhan));
    if (toupper((unsigned char)xacNhan[0]) != 'Y') {
        printf("Da huy thao tac. Ve van con hieu luc.\n");
        return;
    }

    v->trangThai = VE_DA_HUY;     /* ve da huy se bi bo qua khi tinh doanh thu */
    giaiPhongGheCuaVe(v);
    printf("Da huy ve %s. Cac ghe da duoc giai phong.\n", v->maVe);
}

void menuQuanLyVe(void) {
    int chon;
    do {
        printf("\n========== QUAN LY VE ==========\n");
        printf("1. Hien thi danh sach ve\n");
        printf("2. Tim kiem ve\n");
        printf("3. Xem chi tiet ve\n");
        printf("4. Huy ve\n");
        printf("0. Quay lai menu chinh\n");
        chon = docSoNguyen("Chon chuc nang: ", 0, 4);
        switch (chon) {
        case 1: hienThiDanhSachVe(); dungManHinh(); break;
        case 2: timKiemVe();         dungManHinh(); break;
        case 3: xemChiTietVe();      dungManHinh(); break;
        case 4: huyVe();             dungManHinh(); break;
        }
    } while (chon != 0);
}

/* ============================================================
 *                 MUC 6: THONG KE DOANH THU
 *  Moi phep tinh deu dung v->tongTien (gia luu luc dat ve),
 *  KHONG lay dsSuat[].giaVe hien tai de tinh lai (6.6).
 *  Chi cong cac ve co trangThai == VE_HIEU_LUC (6.1).
 * ============================================================ */

/* 6.1 Tong doanh thu */
double tinhTongDoanhThu(void) {
    double tong = 0;
    for (int i = 0; i < soVe; i++)
        if (dsVe[i].trangThai == VE_HIEU_LUC)
            tong += dsVe[i].tongTien;
    return tong;
}

/* 6.2 Dem ve */
void demVe(int *hieuLuc, int *daHuy, int *gheBan) {
    *hieuLuc = *daHuy = *gheBan = 0;
    for (int i = 0; i < soVe; i++) {
        if (dsVe[i].trangThai == VE_HIEU_LUC) {
            (*hieuLuc)++;
            *gheBan += dsVe[i].soGhe;
        } else {
            (*daHuy)++;
        }
    }
}

void thongKeTongQuat(void) {
    int hieuLuc, daHuy, gheBan;
    demVe(&hieuLuc, &daHuy, &gheBan);
    printf("\n===== THONG KE TONG QUAT =====\n");
    printf("Tong so ve da tao       : %d\n", soVe);
    printf("  - Ve hieu luc         : %d (%d ghe)\n", hieuLuc, gheBan);
    printf("  - Ve da huy           : %d\n", daHuy);
    printf("Tong doanh thu          : %.0f VND\n", tinhTongDoanhThu());
}

/* 6.3 Doanh thu theo phim */
void thongKeTheoPhim(void) {
    printf("\n===== DOANH THU THEO PHIM =====\n");
    printf("%-8s %-30s %8s %8s %8s %15s\n",
           "Ma phim", "Ten phim", "Ve HL", "Ghe", "Ve huy", "Doanh thu");
    printf("----------------------------------------------------------------------------------\n");

    /* Gom theo ma phim luu trong ve (van thong ke duoc ca khi phim da bi xoa) */
    int daXet[MAX_VE];
    int soDaXet = 0;
    double tongTatCa = 0;

    for (int i = 0; i < soVe; i++) {
        int trung = 0;
        for (int k = 0; k < soDaXet; k++)
            if (daXet[k] == dsVe[i].maPhim) { trung = 1; break; }
        if (trung) continue;
        daXet[soDaXet++] = dsVe[i].maPhim;

        int veHL = 0, ghe = 0, veHuy = 0;
        double dt = 0;
        for (int j = i; j < soVe; j++) {
            if (dsVe[j].maPhim != dsVe[i].maPhim) continue;
            if (dsVe[j].trangThai == VE_HIEU_LUC) {
                veHL++; ghe += dsVe[j].soGhe; dt += dsVe[j].tongTien;
            } else veHuy++;
        }
        tongTatCa += dt;
        printf("%-8d %-30.30s %8d %8d %8d %15.0f\n",
               dsVe[i].maPhim, layTenPhim(dsVe[i].maPhim), veHL, ghe, veHuy, dt);
    }

    /* Phim co trong danh sach nhung chua ban duoc ve nao */
    for (int p = 0; p < soPhim; p++) {
        int coVe = 0;
        for (int k = 0; k < soDaXet; k++)
            if (daXet[k] == dsPhim[p].maPhim) { coVe = 1; break; }
        if (!coVe)
            printf("%-8d %-30.30s %8d %8d %8d %15.0f\n",
                   dsPhim[p].maPhim, dsPhim[p].tenPhim, 0, 0, 0, 0.0);
    }
    printf("----------------------------------------------------------------------------------\n");
    printf("%-74s %7.0f\n", "TONG", tongTatCa);
}

/* 6.4 Doanh thu theo suat chieu */
void thongKeTheoSuat(void) {
    printf("\n===== DOANH THU THEO SUAT CHIEU =====\n");
    printf("%-8s %-25s %-11s %-6s %-6s %10s %15s\n",
           "Ma suat", "Phim", "Ngay", "Gio", "Phong", "Ghe da ban", "Doanh thu");
    printf("------------------------------------------------------------------------------------\n");

    double tongTatCa = 0;
    for (int s = 0; s < soSuat; s++) {
        const SuatChieu *sc = &dsSuat[s];
        int ghe = 0;
        double dt = 0;
        for (int i = 0; i < soVe; i++) {
            if (strcmp(dsVe[i].maSuat, sc->maSuat) != 0) continue;
            if (dsVe[i].trangThai != VE_HIEU_LUC) continue;
            ghe += dsVe[i].soGhe;
            dt  += dsVe[i].tongTien;
        }
        tongTatCa += dt;
        int sucChua = sc->soHang * sc->soCot;
        printf("%-8s %-25.25s %02d/%02d/%04d %02d:%02d  %-6s %4d/%-5d %15.0f\n",
               sc->maSuat, layTenPhim(sc->maPhim),
               sc->ngayChieu.ngay, sc->ngayChieu.thang, sc->ngayChieu.nam,
               sc->gioChieu.gio, sc->gioChieu.phut, sc->phong,
               ghe, sucChua, dt);
    }

    /* Ve thuoc suat chieu da bi xoa (neu co) */
    double dtMoCoi = 0;
    for (int i = 0; i < soVe; i++)
        if (dsVe[i].trangThai == VE_HIEU_LUC && timChiSoSuat(dsVe[i].maSuat) < 0)
            dtMoCoi += dsVe[i].tongTien;
    if (dtMoCoi > 0) {
        printf("%-71s %12.0f\n", "(Suat chieu da bi xoa)", dtMoCoi);
        tongTatCa += dtMoCoi;
    }
    printf("------------------------------------------------------------------------------------\n");
    printf("%-71s %12.0f\n", "TONG", tongTatCa);
}

/* 6.5 Doanh thu theo ngay dat ve */
void thongKeTheoNgay(void) {
    printf("\n===== DOANH THU THEO NGAY DAT VE =====\n");
    printf("1. Xem tat ca cac ngay\n");
    printf("2. Xem trong khoang ngay\n");
    int chon = docSoNguyen("Chon: ", 1, 2);

    Ngay tu = {1, 1, 1900}, den = {31, 12, 2100};
    if (chon == 2) {
        tu  = docNgay("Tu ngay (dd/mm/yyyy): ");
        den = docNgay("Den ngay (dd/mm/yyyy): ");
        if (soSanhNgay(tu, den) > 0) { Ngay t = tu; tu = den; den = t; }
    }

    /* Lay danh sach ngay khac nhau roi sap xep tang dan */
    Ngay dsNgay[MAX_VE];
    int soNgay = 0;
    for (int i = 0; i < soVe; i++) {
        Ngay d = dsVe[i].ngayDat;
        if (soSanhNgay(d, tu) < 0 || soSanhNgay(d, den) > 0) continue;
        int trung = 0;
        for (int k = 0; k < soNgay; k++)
            if (soSanhNgay(dsNgay[k], d) == 0) { trung = 1; break; }
        if (!trung) dsNgay[soNgay++] = d;
    }
    for (int a = 0; a < soNgay - 1; a++)
        for (int b = a + 1; b < soNgay; b++)
            if (soSanhNgay(dsNgay[a], dsNgay[b]) > 0) {
                Ngay t = dsNgay[a]; dsNgay[a] = dsNgay[b]; dsNgay[b] = t;
            }

    if (soNgay == 0) { printf("Khong co ve nao trong khoang thoi gian nay.\n"); return; }

    printf("%-12s %8s %8s %8s %15s\n", "Ngay dat", "Ve HL", "Ghe", "Ve huy", "Doanh thu");
    printf("-------------------------------------------------------\n");
    double tongTatCa = 0;
    for (int k = 0; k < soNgay; k++) {
        int veHL = 0, ghe = 0, veHuy = 0;
        double dt = 0;
        for (int i = 0; i < soVe; i++) {
            if (soSanhNgay(dsVe[i].ngayDat, dsNgay[k]) != 0) continue;
            if (dsVe[i].trangThai == VE_HIEU_LUC) {
                veHL++; ghe += dsVe[i].soGhe; dt += dsVe[i].tongTien;
            } else veHuy++;
        }
        tongTatCa += dt;
        printf("%02d/%02d/%04d   %8d %8d %8d %15.0f\n",
               dsNgay[k].ngay, dsNgay[k].thang, dsNgay[k].nam, veHL, ghe, veHuy, dt);
    }
    printf("-------------------------------------------------------\n");
    printf("%-39s %15.0f\n", "TONG", tongTatCa);
}

void menuThongKe(void) {
    int chon;
    do {
        printf("\n========== THONG KE DOANH THU ==========\n");
        printf("1. Tong doanh thu va so luong ve\n");
        printf("2. Doanh thu theo phim\n");
        printf("3. Doanh thu theo suat chieu\n");
        printf("4. Doanh thu theo ngay dat ve\n");
        printf("0. Quay lai menu chinh\n");
        chon = docSoNguyen("Chon chuc nang: ", 0, 4);
        switch (chon) {
        case 1: thongKeTongQuat(); dungManHinh(); break;
        case 2: thongKeTheoPhim(); dungManHinh(); break;
        case 3: thongKeTheoSuat(); dungManHinh(); break;
        case 4: thongKeTheoNgay(); dungManHinh(); break;
        }
    } while (chon != 0);
}

/* ============================================================
 *  DU LIEU MAU CHO SUAT CHIEU VA VE
 *  Muc 3 (suat chieu) va muc 4 (dat ve) chua lam nen tam dung du lieu mau
 *  de chay thu muc 5-6. Khi lam xong muc 3-4 thi XOA phan nay va
 *  bo dong khoiTaoDuLieuMau() trong main().
 * ============================================================ */
void themPhimMau(int ma, const char *ten, const char *daoDien, int nam,
                 int loai, float thoiLuong, int doTuoi) {
    struct typePhim *p = &dsPhim[soPhim++];
    p->maPhim = ma; strcpy(p->tenPhim, ten); strcpy(p->daoDien, daoDien);
    p->namSX = nam; p->loaiPhim = loai; p->thoiLuong = thoiLuong; p->doTuoi = doTuoi;
}

void themSuatMau(const char *ma, int maPhim, int d, int m, int y,
                 int h, int ph, const char *phong, double gia) {
    SuatChieu *s = &dsSuat[soSuat++];
    memset(s, 0, sizeof(*s));
    strcpy(s->maSuat, ma); s->maPhim = maPhim; strcpy(s->phong, phong);
    s->ngayChieu = (Ngay){d, m, y};
    s->gioChieu  = (Gio){h, ph};
    s->giaVe = gia; s->soHang = 5; s->soCot = 10;
}

/* Tao ve mau, danh dau ghe da dat; ghe cach nhau boi dau phay */
void themVeMau(const char *maSuat, const char *ghe, int d, int m, int y, int trangThai) {
    int is = timChiSoSuat(maSuat);
    if (is < 0 || soVe >= MAX_VE) return;
    SuatChieu *sc = &dsSuat[is];
    Ve *v = &dsVe[soVe];
    memset(v, 0, sizeof(*v));
    snprintf(v->maVe, sizeof(v->maVe), "VE%04d", (soVe + 1) % 10000);
    strcpy(v->maSuat, maSuat);
    v->maPhim = sc->maPhim;
    v->ngayDat = (Ngay){d, m, y};
    v->trangThai = trangThai;

    char buf[64];
    strcpy(buf, ghe);
    for (char *t = strtok(buf, ","); t && v->soGhe < MAX_GHE_VE; t = strtok(NULL, ",")) {
        int h, c;
        if (!phanTichMaGhe(t, sc, &h, &c)) continue;
        strcpy(v->dsGhe[v->soGhe], t);
        v->giaGhe[v->soGhe] = (h >= 3) ? sc->giaVe + 20000 : sc->giaVe; /* hang D,E = VIP */
        v->tongTien += v->giaGhe[v->soGhe];
        v->soGhe++;
        if (trangThai == VE_HIEU_LUC) sc->ghe[h][c] = 1;
    }
    soVe++;
}

void khoiTaoDuLieuMau(void) {
    themPhimMau(1, "Lat Mat 8", "Ly Hai", 2025, 1, 120, 16);
    themPhimMau(2, "Doraemon Movie", "Imai Kazuaki", 2025, 2, 100, 0);

    themSuatMau("S01", 1, 14, 11, 2026, 18,  0, "R1", 90000);
    themSuatMau("S02", 1, 14, 11, 2026, 21,  0, "R1", 100000);
    themSuatMau("S03", 2, 15, 11, 2026,  9, 30, "R2", 70000);

    themVeMau("S01", "A01,A02",     12, 11, 2026, VE_HIEU_LUC);
    themVeMau("S01", "D05",         12, 11, 2026, VE_HIEU_LUC);
    themVeMau("S02", "B03,B04,B05", 13, 11, 2026, VE_HIEU_LUC);
    themVeMau("S03", "C01",         13, 11, 2026, VE_DA_HUY);
    themVeMau("S03", "E09,E10",     13, 11, 2026, VE_HIEU_LUC);
}

/* Bo phan con lai cua dong sau scanf de khong lam hong lan nhap ke tiep */
void xoaBoDem(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/* ============================================================
 *                         MAIN
 *  Giu nguyen menu va muc 1 cua ban; nhung cho co danh dau [THEM]
 *  la phan bo sung de noi voi muc 5-6.
 * ============================================================ */
int main()
{
    khoiTaoDuLieuMau(); /* [THEM] xoa khi da co muc 3-4 */

    int choice;
    do
    {
        printf("=========MENU============\n");
        printf("1.	Quan ly phim\n");
        printf("2.	Quan ly phong / ghe\n");
        printf("3.	Quan ly xuat chieu\n");
        printf("4.	Dat ve xem phim\n");
        printf("5.	Quan ly ve\n");
        printf("6.	Thong ke doanh thu\n");
        printf("7.	Thoat chuong trinh\n");

        printf("Chon menu: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Vui long nhap so nguyen!\n");
            xoaBoDem();
            choice = 0;
            continue;
        }
        xoaBoDem(); /* [THEM] bo '\n' con lai truoc khi vao muc 5-6 (dung fgets) */

        switch (choice)
        {
        case 1:

            printf("Quan ly phim\n");
            int subChoice;
            do
            {
                printf("=========CHU NANG============\n");
                printf("1.	Quan ly the loai phim\n");
                printf("2.	Quan ly phim\n");
                printf("Chon chuc nang: ");
                if (scanf("%d", &subChoice) != 1)
                {
                    printf("Vui long nhap so nguyen!\n");
                    xoaBoDem();
                    subChoice = 0;
                    continue;
                }
                switch (subChoice)
                {
                case 1:
                    printf("---------------Quan ly the loai phim----------------------\n");

                    struct theLoai theLoaiPhim;
                    printf("Nhap ma the loai: ");
                    scanf("%d", &theLoaiPhim.maTheLoai);
                    printf("Nhap ten the loai: ");
                    scanf(" %[^\n]", theLoaiPhim.tenTheLoai);

                    /* [THEM] luu vao mang */
                    if (soTheLoai < MAX_THE_LOAI)
                    {
                        dsTheLoai[soTheLoai++] = theLoaiPhim;
                        printf("Da them the loai.\n");
                    }
                    break;

                case 2:
                    printf("---------------Nhap thong tin phim----------------------\n");

                    struct typePhim Phim;

                    printf("Nhap ma the loai: ");
                    scanf("%d", &Phim.loaiPhim);
                    printf("Nhap ma phim: ");
                    scanf("%d", &Phim.maPhim);
                    printf("Nhap ten phim: ");
                    scanf(" %[^\n]", Phim.tenPhim);
                    printf("Nhap dao dien: ");
                    scanf(" %[^\n]", Phim.daoDien);
                    printf("Nhap nam san xuat: ");
                    scanf("%d", &Phim.namSX);

                    printf("Nhap thoi luong: ");
                    scanf("%f", &Phim.thoiLuong);
                    printf("Nhap do tuoi: ");
                    scanf("%d", &Phim.doTuoi);

                    /* [THEM] kiem tra trung ma (quy tac III.1) roi luu vao mang */
                    if (timChiSoPhim(Phim.maPhim) >= 0)
                        printf("Ma phim %d da ton tai, khong them.\n", Phim.maPhim);
                    else if (soPhim >= MAX_PHIM)
                        printf("Danh sach phim da day.\n");
                    else
                    {
                        dsPhim[soPhim++] = Phim;
                        printf("Da them phim.\n");
                    }
                    break;
                }
                xoaBoDem(); /* [THEM] */
            } while (subChoice != 1 && subChoice != 2);

            break;
        case 2:
            printf("Quan ly ghe\n");
            break;
        case 3:
            printf("Quan ly xuat chieu\n");
            break;
        case 4:
            printf("Dat ve xem phim\n");
            break;
        case 5:
            menuQuanLyVe();   /* [THEM] */
            break;
        case 6:
            menuThongKe();    /* [THEM] */
            break;
        case 7:
            printf("Thoat chuong trinh\n");
            return 0;
        default:
            printf("Lua chon khong hop le.\n");
        }

    } while (choice != 7);

    return 0;
}
