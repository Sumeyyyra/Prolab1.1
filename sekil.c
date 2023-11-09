#include "sekil.h"
#include "display.h"
#include <stdio.h>
#include <stdbool.h>
#include "control.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b)) // a b yi kıyasla eğer a küçükse a değeri , değilse b değeri sonuç olur.
#define MAX(a, b) (((a) > (b)) ? (a) : (b)) // a b yi kıyasla eğer a byğkse a değeri , b büyükse b değeri sonuç olur.

FILE *fphedef;
FILE *fp;
FILE *fpkaynak;
vec2_t vertices[100];

void draw_gray(vec2_t sekil[], int sekilIndex, uint32_t color, int window_number)
{
    int maxX = sekil[0].x;
    int minX = sekil[0].x;
    int maxY = sekil[0].y;
    int minY = sekil[0].y;

    for (int h = 0; h < sekilIndex; h++)
    {
        if (sekil[h].x > maxX)
        {
            maxX = sekil[h].x;
        }
        if (sekil[h].x < minX)
        {
            minX = sekil[h].x;
        }
        if (sekil[h].y > maxY)
        {
            maxY = sekil[h].y;
        }
        if (sekil[h].y < minY)
        {
            minY = sekil[h].y;
        }
    }

    for (int y = minY; y <= maxY; y++)
    {
        for (int x = minX; x <= maxX; x++)
        {

            draw_pixel(x, y, 0xFFD3D3D3, window_number);
        }
    }
}

void separate_coordinates(const char *hedefDosyaYolu)
{

    int koordinatSayisi = 0;
    int okumaModu;
    char karakter;
    int x = 0;
    int y = 0;

    fphedef = fopen(hedefDosyaYolu, "r");

    // Dosyanin bulunamamasi dahilinde hata mesaji yazdiriyor.
    if (fphedef == NULL)
    {
        printf("Dosya açma hatasi");
        return;
    }
    // Dosyadan karakter okuma bos olmadigi sürece döngüye girer
    while ((karakter = fgetc(fphedef)) != EOF)
    {
        if (karakter == '(')
        {
            okumaModu = 1; // X değeri okunmaya baslandi
            x = 0;
        }
        else if (karakter == ',')
        {
            okumaModu = 2; // Y değeri okunmaya baslandi
            y = 0;
        }
        else if (karakter == ')')
        {
            // Koordinat tamamlandı, diziye ekle
            vertices[koordinatSayisi].x = x;
            vertices[koordinatSayisi].y = y;
            koordinatSayisi++;
            okumaModu = 0; // Bir sonraki koordinatin  baslangicini bekleyin
        }
        else if (okumaModu == 1)
        {
            // Karakter karakter okuma yaptığımız için burda hem kendi sayımızı tam sayı olarak kaydediyoruz hem de bir basamaklı olmayan sayıların değerlerini ASCII değerine dönüştürüyoruz.
            x = x * 10 + (karakter - '0');
        }
        else if (okumaModu == 2)
        {
            y = y * 10 + (karakter - '0');
        }
    }

    // İstediğiniz koordinatların doğru şekilde alınıp alınmadığını görmek için bu fonksiyonu kullanın.
    /*for (int i = 0; i < koordinatSayisi; i++) {
    printf("Koordinat %d: (%d, %d)\n", i+1, vertices[i].x, vertices[i].y);
    }*/

    fclose(fphedef);
}

void find_shape(vec2_t vertices[], int n, vec2_t sekil1[], int *sekil1Index, vec2_t sekil2[], int *sekil2Index, vec2_t sekil3[], int *sekil3Index)
{
    int i = 0;

    // İlk şekil için
    int basx = vertices[0].x;
    int basy = vertices[0].y;
    int a = 0;
    sekil1[a++] = vertices[i++];
    while (i < n && (basx != vertices[i].x || basy != vertices[i].y))
    {
        sekil1[a++] = vertices[i++];
    }
    if (i < n && basx == vertices[i].x && basy == vertices[i].y)
    {
        i++;
    }
    *sekil1Index = a;

    if (i >= n)
    {
        *sekil2Index = 0;
        *sekil3Index = 0;
        return;
    }

    // İkinci şekil için
    basx = vertices[i].x;
    basy = vertices[i].y;
    int b = 0;
    sekil2[b++] = vertices[i++];
    while (i < n && (basx != vertices[i].x || basy != vertices[i].y))
    {
        sekil2[b++] = vertices[i++];
    }
    if (i < n && basx == vertices[i].x && basy == vertices[i].y)
    {
        i++;
    }

    if (b < 2)
    {
        *sekil2Index = 0;
    }
    else
    {
        *sekil2Index = b;
    }

    if (i >= n)
    {
        *sekil3Index = 0;
        return;
    }

    // Üçüncü şekil için
    basx = vertices[i].x;
    basy = vertices[i].y;
    int c = 0;
    sekil3[c++] = vertices[i++];
    while (i < n && (basx != vertices[i].x || basy != vertices[i].y))
    {
        sekil3[c++] = vertices[i++];
    }
    // Şekil boş olunca 1 c yi 1 veriyordu bu hatayı bu şekilde fixed ettim.
    if (*sekil3Index < 2)
    {
        *sekil3Index = 0;
    }
    else
    {
        *sekil3Index = c;
    }
}

bool is_point_inside_polygon(vec2_t *polygon, int n, vec2_t point)
{
    int intersections = 0;
    for (int i = 0; i < n; i++)
    {
        vec2_t p1 = polygon[i];
        vec2_t p2 = polygon[(i + 1) % n]; // Son noktadan sonra başa dön

        // Yatay kenar kontrolü
        if (p1.y == p2.y && p1.y == point.y &&
            ((p1.x <= point.x && point.x <= p2.x) || (p2.x <= point.x && point.x <= p1.x)))
        {
            return true;
        }

        // Yatay ışının sadece yukarıdaki kenarlarla kesişmesini kontrol et
        if (p1.y > point.y != p2.y > point.y)
        {
            float intersectionX = p1.x + (point.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
            if (intersectionX > point.x)
            {
                intersections++;
            }
        }
    }
    return intersections % 2 == 1;
}

int fill_polygon(vec2_t *polygon, int n, uint32_t color, int window_number)
{
    int x_min = polygon[0].x;
    int x_max = polygon[0].x;
    int y_min = polygon[0].y;
    int y_max = polygon[0].y;

    for (int i = 1; i < n; i++)
    {
        x_min = MIN(x_min, polygon[i].x);
        x_max = MAX(x_max, polygon[i].x);
        y_min = MIN(y_min, polygon[i].y);
        y_max = MAX(y_max, polygon[i].y);
    }

    int draw_count = 0;
    for (int y = y_min; y <= y_max; y++)
    {
        for (int x = x_min; x < x_max; x++)
        {
            vec2_t p = {x, y};
            if (is_point_inside_polygon(polygon, n, p))
            {

                draw_pixel(x, y, color, window_number);
                draw_count++;
            }
        }
    }
    return draw_count;
}

size_t yazdir(void *ptr, size_t boyut, size_t nmemb, void *stream)
{
    return fwrite(ptr, boyut, nmemb, fp);
}

void target_row(const char *kaynakDosyaYolu, const char *hedefDosyaYolu, int istenilen_satir)
{
    // Acilan dosyalar bos ise hata mesajlarını yazıyor.
    fpkaynak = fopen(kaynakDosyaYolu, "r");
    if (fpkaynak == NULL)
    {
        printf("Kaynak dosya açma hatasi");

        return;
    }

    fphedef = fopen(hedefDosyaYolu, "w");
    if (fphedef == NULL)
    {
        printf("Hedef dosya açma hatasi");

        return;
    }

    char satir[1024]; // Satırın maksimum uzunluğu 1024 olarak kabul ettik.
    int aktifSatirSayisi = 0;

    while (fgets(satir, sizeof(satir), fpkaynak) != NULL)
    {
        aktifSatirSayisi++;

        if (aktifSatirSayisi == istenilen_satir)
        {
            fputs(satir, fphedef);
            break; // Belirtilen satırı bulduktan sonra döngüyü sonlandır.
        }
    }

    fclose(fpkaynak);
    fclose(fphedef);
}

bool coordinate_check(vec2_t k1, vec2_t k2)
{
    return k1.x == k2.x && k1.y == k2.y;
}

int curl_main(void)
{
    CURL *curl; // curl fonksiyonunu başlatmak için kullanılır.
    CURLcode sonuc;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        fp = fopen("urldenOkunan.txt", "wb");
        if (!fp)
        {
            printf("Dosya acilamadi");
            return 1;
        }

        curl_easy_setopt(curl, CURLOPT_URL, "https://fastqr.co/prolab.txt");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, yazdir);

        sonuc = curl_easy_perform(curl);

        if (sonuc != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() başarisiz oldu: %s\n", curl_easy_strerror(sonuc));
        }

        fclose(fp);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    const char *kaynakDosyaYolu = "urldenOkunan.txt";
    const char *hedefDosyaYolu = "kullanicinin_istedigi.txt";
    int islem_yapilacak_satir; // Kopyalanacak satirin numarasi

    printf("Lutfen islem yapmak istediginiz satiri girin ...\n");
    scanf("%d", &islem_yapilacak_satir);

    target_row(kaynakDosyaYolu, hedefDosyaYolu, islem_yapilacak_satir);

    separate_coordinates(hedefDosyaYolu);

    return 0;
}
