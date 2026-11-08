#include <stdio.h>
#include <windows.h>

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
            while ((c = getchar()) != '\n' && c != EOF);

            continue;
        }
        switch (choice)
        {
        case 1:
            printf("Quan ly phim\n");
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
