#include <stdio.h>
#include <windows.h>

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

int main()
{
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

            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;

            continue;
        }
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

                    int c;
                    while ((c = getchar()) != '\n' && c != EOF)
                        ;

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
                    scanf(" %[^\n]s", theLoaiPhim.tenTheLoai);

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
