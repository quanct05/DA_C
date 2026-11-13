#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
struct theLoai *danhSachTheLoai[MAX];
int soLuong = 0;
int subChoice;
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
void luuTheLoaiVaoFile()
{
    FILE *f = fopen("theloai.txt", "w");

    if (f == NULL)
    {
        printf("Khong mo duoc file!\n");
        return;
    }
    int i = 0;
    for (i; i < soLuong; i++)
    {
        fprintf(f, "%d|%s\n",
                danhSachTheLoai[i]->maTheLoai,
                danhSachTheLoai[i]->tenTheLoai);
    }

    fclose(f);

    printf("Da luu danh sach vao file!\n");
}
void docTheLoaiTuFile()
{
    FILE *f = fopen("theloai.txt", "r");

    if (f == NULL)
    {
        printf("Khong tim thay file!\n");
        return;
    }

    int ma;
    char ten[50];

    while (soLuong < MAX &&
           fscanf(f, "%d|%49[^\n]", &ma, ten) == 2)
    {
        struct theLoai *theLoaiPhim = malloc(sizeof(struct theLoai));

        if (theLoaiPhim == NULL)
            break;

        theLoaiPhim->maTheLoai = ma;
        strcpy(theLoaiPhim->tenTheLoai, ten);

        danhSachTheLoai[soLuong++] = theLoaiPhim;

        fgetc(f);
    }

    fclose(f);
}
void checkInput()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void xuatTheLoai()
{
    printf("\n=========Danh sach the loai phim=========\n\n");
            int i = 0;
            for (i; i < soLuong; i++)
            {
                printf("Ma the loai: %d, Ten the loai: %s\n",
                       danhSachTheLoai[i]->maTheLoai,
                       danhSachTheLoai[i]->tenTheLoai);
            }
}
void crudLoaiPhim()
{
    do
    {
        printf("\n=========CHU NANG============\n");
        printf("1.	Danh sach the loai phim\n");
        printf("2.	Them the loai phim\n");
        printf("3.	Sua the loai phim\n");
        printf("4.	Xoa the loai phim\n");
        printf("0.	De thoat\n");
        printf("Chon chuc nang: ");

        if (scanf("%d", &subChoice) != 1)
        {
            checkInput();
            continue;
        }

        switch (subChoice)
        {
        case 1:
        {
            xuatTheLoai();
            break;
        }
        case 2:
        {
            if (soLuong >= MAX)
            {
                printf("Danh sach da day!\n");
                break;
            }

            struct theLoai *theLoaiPhim = malloc(sizeof(struct theLoai));

            printf("Nhap ma the loai: ");
            scanf("%d", &theLoaiPhim->maTheLoai);

            int i = 0;
            for (i = 0; i < soLuong; i++)
            {
                if (danhSachTheLoai[i]->maTheLoai == theLoaiPhim->maTheLoai)
                {
                    printf("Ma the loai da ton tai!\n");
                    free(theLoaiPhim);
                    return;
                }
            }

            printf("Nhap ten the loai: ");
            scanf(" %49[^\n]", theLoaiPhim->tenTheLoai);
            danhSachTheLoai[soLuong] = theLoaiPhim;
            soLuong++;
            luuTheLoaiVaoFile();
            xuatTheLoai();

            break;
        }

        case 3:
        {
            int maCanSua;
            printf("Nhap ma the loai can sua: ");
            scanf("%d", &maCanSua);
            int i = 0;
            char tenTheLoaiMoi[50];
            for (i = 0; i < soLuong; i++)
            {
                if (danhSachTheLoai[i]->maTheLoai == maCanSua)
                {
                    printf("Nhap ten the loai moi: ");
                    scanf(" %49[^\n]", tenTheLoaiMoi);
                    char tenCu[50];
                    strcpy(tenCu, danhSachTheLoai[i]->tenTheLoai);
                    strcpy(danhSachTheLoai[i]->tenTheLoai, tenTheLoaiMoi);
                    printf("Da sua %s --> %s \n", tenCu, danhSachTheLoai[i]->tenTheLoai);
                    break;
                }
            }
        }

        case 4:
        {

            printf("Nhap ma the loai can xoa: ");
            int maCanXoa;
            scanf("%d", &maCanXoa);
            int i = 0;
            for (i = 0; i < soLuong; i++)
            {
                if (danhSachTheLoai[i]->maTheLoai == maCanXoa)
                {
                    free(danhSachTheLoai[i]);
                    int j = i;
                    for (j; j < soLuong - 1; j++)
                    {
                        danhSachTheLoai[j] = danhSachTheLoai[i + 1];
                        soLuong--;
                    }
                }
            }
        }
        }

    } while (subChoice != 0);
}

void functionPhim()
{

    do
    {
        printf("\n=========CHU NANG============\n");
        printf("1.	Quan ly the loai phim\n");
        printf("2.	Quan ly phim\n");
        printf("Chon chuc nang: ");
        if (scanf("%d", &subChoice) != 1)
        {
            printf("Vui long nhap so nguyen!\n");
            checkInput();
            continue;
        }
        switch (subChoice)
        {
        case 1:
            // printf("---------------Quan ly the loai phim----------------------\n");

            crudLoaiPhim();

            break;

        case 2:
            printf("---------------Nhap thong tin phim----------------------\n");

            struct typePhim Phim;

            printf("Nhap ma the loai: ");
            scanf("%d", &Phim.loaiPhim);
            printf("Nhap ma phim: ");
            scanf("%d", &Phim.maPhim);
            printf("Nhap ten phim: ");
            scanf(" %[^\n]s", Phim.tenPhim);
            printf("Nhap dao dien: ");
            scanf(" %[^\n]s", Phim.daoDien);
            printf("Nhap nam san xuat: ");
            scanf("%d", &Phim.namSX);

            printf("Nhap thoi luong: ");
            scanf("%f", &Phim.thoiLuong);
            printf("Nhap do tuoi: ");
            scanf("%d", &Phim.doTuoi);
            break;
        }
    } while (subChoice != 1 && subChoice != 2);
}

int main()
{
    int choice;
    docTheLoaiTuFile();
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

            checkInput();

            continue;
        }
        switch (choice)
        {
        case 1:
        {

            functionPhim();
        }

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
            printf("Quản lý vé\n");
            break;
        case 6:
            printf("Thong ke doanh thu\n");
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
